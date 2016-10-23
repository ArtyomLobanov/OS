#include <serial.h>
#include <stdarg.h>
#include <stddef.h>
#include "output.h"

#define ORDINARY_SYMBOL 0
#define STRING 1
#define CHAR 2
#define SIGNED_CHAR 3
#define UNSIGNED_CHAR 4
#define SHORT 5
#define UNSIGNED_SHORT 6
#define LONG 7
#define UNSIGNED_LONG 8
#define LONG_LONG 9
#define UNSIGNED_LONG_LONG 10

#define COMMENT -1
#define END -2

struct Stream {
	char* buffer;
	int virtual_written;
	int limit;
	int real_written;
};

int begins_with(const char* s, const char* prefix) {
	while (*prefix != 0) {
		if (*prefix != *s)
			return 0;
		prefix++;
		s++;
	}
	return 1;
}

int min(int a, int b) {
	return a < b? a : b;
}

void write_char(const char c, struct Stream* stream) {
	if (stream->buffer == 0) {
		printChar(c);
	} else {
		if (stream->limit != 0) {
			stream->limit--;
			*(stream->buffer++) = c;
			stream->real_written++;
		}
	}
	stream->virtual_written++;
}

void write(const char* string, struct Stream* stream) {
	for (; *string != 0; string++)
		write_char(*string, stream);
}

void write_uint(unsigned long long value, int radix, struct Stream* stream) {
	static char* digits = "0123456789ABCDEF";
	if (radix == 16) {
		write("0x", stream);
	}
	if (radix == 2) {
		write("0b", stream);
	}
	if (value == 0) {
		write_char('0', stream);
		return;
	}
	char res[65];
	res[64] = 0;
	int len = 0;
	while (value > 0) {
		res[63 - len] = digits[value % radix];
		value /= radix;
		len++;
	}
	write(&res[64 - len], stream);
}

unsigned long long abs(long long value) {
	return value > 0? value : -value;
}

void write_int(long long value, int radix, struct Stream* stream) {
	if (value < 0)
		write_char('-', stream);
	write_uint(abs(value), radix, stream);
}

int ptr_shift(int t) {
	if (t == UNSIGNED_CHAR || t == SIGNED_CHAR) return 1;
	if (t == SHORT || t == UNSIGNED_SHORT) return 2;
	if (t == LONG || t == UNSIGNED_LONG) return 4;
	if (t == UNSIGNED_LONG_LONG || t == LONG_LONG || t == STRING) return 8;
	return 0;
}


int get_type(const char* field) {
	if (begins_with(field, _D_UCHAR)) return UNSIGNED_CHAR;
	if (begins_with(field, _D_CHAR)) return SIGNED_CHAR;
	if (begins_with(field, _D_SHORT)) return SHORT;
	if (begins_with(field, _D_USHORT)) return UNSIGNED_SHORT;
	if (begins_with(field, _D_INT)) return LONG;
	if (begins_with(field, _D_UINT)) return UNSIGNED_LONG;
	if (begins_with(field, _D_LONG)) return LONG_LONG;
	if (begins_with(field, _D_ULONG)) return UNSIGNED_LONG_LONG;
	if (begins_with(field, _D_STRING)) return STRING;
	if (begins_with(field, _D_COMMENT)) return COMMENT;
	return END;
}

const char* get_name(const char* field) {
	if (begins_with(field, _D_END)) return 0;
	if (begins_with(field, _D_STRING)) return field+2;
	if (begins_with(field, _D_COMMENT)) return field+2;
	return field + 3;
}

char* write_field(char* internal_ptr, int template, const char* field, const char* prefix, struct Stream* stream) {
	int type = get_type(field);
	if (prefix == 0) {
		return internal_ptr + ptr_shift(type);
	}
	write(prefix, stream);
	write(get_name(field), stream);
	if (type == COMMENT) {
		if (template == CLASS_STYLE) write_char('\n', stream);
		return internal_ptr;
	}
	write(template == CLASS_STYLE? " = " : "=", stream);
	if (type == SIGNED_CHAR) {
		write_int(*((signed char*) internal_ptr), 16, stream);
	} else if (type == UNSIGNED_CHAR) {
		write_uint(*((unsigned char*) internal_ptr), 16, stream);
	} else if (type == SHORT) {
		write_int(*((short*) internal_ptr), 16, stream);
	} else if (type == UNSIGNED_SHORT) {
		write_uint(*((unsigned short*) internal_ptr), 16, stream);
	} else if (type == LONG) {
		write_int(*((int*) internal_ptr), 16, stream);
	} else if (type == UNSIGNED_LONG) {
		write_uint(*((unsigned int*) internal_ptr), 16, stream);
	} else if (type == LONG_LONG) {
		write_int(*((long long*) internal_ptr), 16, stream);
	} else if (type == UNSIGNED_LONG_LONG) {
		write_uint(*((unsigned long long*) internal_ptr), 16, stream);
	} else if (type == STRING) {
		write_char('\"', stream);
		unsigned long long addr = *((unsigned long long*) internal_ptr);
		write((const char*) addr, stream);
		write_char('\"', stream);
	}
	write(template == CLASS_STYLE? ";\n" : ",", stream);
	return internal_ptr + ptr_shift(type);
}

void write_struct(struct_description_t *ptr, int template, struct Stream* stream) {
	char* internal_ptr = (char*) ptr-> ptr;
	write("Struct ", stream);
	write(ptr->name, stream);
	write(" (at ", stream);
	write_uint((unsigned long long) ptr->ptr, 16, stream);
	write(") {", stream);
	if (template == CLASS_STYLE) write_char('\n', stream);
	for (int i = 0;; i++) {
		if (get_type(ptr->fields[i]) == END) break;
		internal_ptr = write_field(internal_ptr, template, ptr->fields[i],
				ptr->prefix(ptr->ptr, i, template), stream);	
	}
	write("}\n", stream);
}

void writef(const char* s, va_list args, struct Stream* stream) {
	while (*s != 0) {
		if (*s != '%') {
			write_char(*s, stream);
			s++;
			continue;
		}
		s++;
		if (begins_with(s, "s")) {
			write(va_arg(args, const char*), stream);
			s += 1;
		} else if (begins_with(s, "c")) {
			write_char((char) va_arg(args, int), stream);
			s += 1;	
		} else if (begins_with(s, "i") || begins_with(s, "d")) { // int
			write_int(va_arg(args, int), 10, stream);
			s += 1;
		} else if (begins_with(s, "u")) {
			write_uint(va_arg(args, unsigned int), 10, stream);
			s += 1;
		} else if (begins_with(s, "o")) {
			write_uint(va_arg(args, unsigned int), 8, stream);	
			s += 1;
		} else if (begins_with(s, "x")) {
			write_uint(va_arg(args, unsigned int), 16, stream);	
			s += 1;
		} else if (begins_with(s, "hhi") || begins_with(s, "hhd")) { // char
			write_int((signed char) va_arg(args, int), 10, stream);
			s += 3;
		} else if (begins_with(s, "hhu")) {
			write_uint((unsigned char) va_arg(args, int), 10, stream);
			s += 3;
		} else if (begins_with(s, "hho")) {
			write_uint((unsigned char) va_arg(args, int), 8, stream);	
			s += 3;
		} else if (begins_with(s, "hhx")) {
			write_uint((unsigned char) va_arg(args, int), 16, stream);	
			s += 3;
		} else if (begins_with(s, "hi") || begins_with(s, "hd")) { // short
			write_int((short) va_arg(args, int), 10, stream);
			s += 2;
		} else if (begins_with(s, "hu")) {
			write_uint((unsigned short) va_arg(args, int), 10, stream);
			s += 2;
		} else if (begins_with(s, "ho")) {
			write_uint((unsigned short) va_arg(args, int), 8, stream);
			s += 2;	
		} else if (begins_with(s, "hx")) {
			write_uint((unsigned short) va_arg(args, int), 16, stream);	
			s += 2;
		} else if (begins_with(s, "li") || begins_with(s, "ld")) { // long
			write_int(va_arg(args, long), 10, stream);
			s += 2;
		} else if (begins_with(s, "lu")) {
			write_uint(va_arg(args, unsigned long), 10, stream);
			s += 2;
		} else if (begins_with(s, "lo")) {
			write_uint(va_arg(args, unsigned long), 8, stream);	
			s += 2;
		} else if (begins_with(s, "lx")) {
			write_uint(va_arg(args, unsigned long), 16, stream);	
			s += 2;
		} else if (begins_with(s, "lli") || begins_with(s, "lld")) { // long long
			write_int(va_arg(args, long long), 10, stream);
			s += 3;
		} else if (begins_with(s, "llu")) {
			write_uint(va_arg(args, unsigned long long), 10, stream);
			s += 3;
		} else if (begins_with(s, "llo")) {
			write_uint(va_arg(args, unsigned long long), 8, stream);	
			s += 3;
		} else if (begins_with(s, "llx")) {
			write_uint(va_arg(args, unsigned long long), 16, stream);	
			s += 3;
		} else if (begins_with(s, "ms")) {
			write_struct(va_arg(args, struct_description_t*), STRUCT_STYLE, stream);	
			s += 2;
		} else if (begins_with(s, "mc")) {
			write_struct(va_arg(args, struct_description_t*), CLASS_STYLE, stream);	
			s += 2;
		}

	}
}

int vprintf(const char* fmt, va_list args) {
	struct Stream stream = {0, 0, 0, 0};
	writef(fmt, args, &stream);
	return stream.virtual_written;
}

int vsnprintf(char* s, size_t n, const char* fmt, va_list args) {
	struct Stream stream = {s, 0, n - 1, 0};
	writef(fmt, args, &stream);
	s[stream.real_written] = 0;
	return stream.virtual_written;
}

int snprintf(char* s, size_t n, const char* fmt, ...) {
	va_list args;
	
	va_start(args, fmt);
	int res = vsnprintf(s, n, fmt, args);
	va_end(args);
	
	return res;
}

int printf(const char* fmt, ...) {
	va_list args;
	
	va_start(args, fmt);
	int res = vprintf(fmt, args);
	va_end(args);
	
	return res;
}


