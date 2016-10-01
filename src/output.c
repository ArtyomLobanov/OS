#include <serial.h>
#include <stdarg.h>
#include <stddef.h>

#define unused(arg) if (&arg == &arg + 1){}

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

