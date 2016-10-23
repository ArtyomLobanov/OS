#include "output.h"
#include "memory_map.h"

extern multiboot_header_t multiboot_header;
extern unsigned int multiboot_info_address;

multiboot_header_t* multiboot_header_ptr = (multiboot_header_t*) &multiboot_header;
multiboot_info_t* multiboot_info_ptr = 0;

void override_memory_map();

void init() {
	if (!multiboot_info_ptr) {
		multiboot_info_ptr = (multiboot_info_t*) ((unsigned long long)multiboot_info_address);
		override_memory_map();
	}
}

int is_valid_multiboot_header(multiboot_header_t *ptr) {
	if (ptr->magic != MULTIBOOT_HEADER_MAGIC)
		return 0;
	if (ptr->magic + ptr->flags + ptr->checksum != 0)
		return 0;
	return 1;
}
//-----------------override_memory_map------------------------------

memory_map_entry_t memory_map[MAX_MEMORY_MAP_CAPACITY];
int memory_map_size;
int overflow_flag = 0;

void add_entry(memory_map_entry_t entry) {
	if (memory_map_size == MAX_MEMORY_MAP_CAPACITY) {
		printf("Error: memory_map overflow");
		overflow_flag = 1;
		return;
	}
	memory_map[memory_map_size++] = entry;
}

unsigned long long end(memory_map_entry_t* p) {
	return p->addr + p->len;
}

void override_memory_map() {
	unsigned long long code_section_begin = multiboot_header_ptr->load_addr;
	unsigned long long code_section_end = multiboot_header_ptr->bss_end_addr;
	unsigned long long old_memory_map_address = multiboot_info_ptr->mmap_addr;
	unsigned long long old_memory_map_length = multiboot_info_ptr->mmap_length;
	
	memory_map_entry_t code_section = {20, code_section_begin, code_section_end - code_section_begin, MULTIBOOT_MEMORY_RESERVED};
	add_entry(code_section);
	
	memory_map_entry_t entry;
	
	for (unsigned long long entry_addr = old_memory_map_address; !overflow_flag &&
			entry_addr < old_memory_map_address + old_memory_map_length; entry_addr += entry.size + 4) {
		entry = *((memory_map_entry_t*) entry_addr);
		// [{}]
		if (code_section_begin <= entry.addr && end(&entry) <= code_section_end) {
			continue;
		}
		// {} [] or [] {}
		if (code_section_end <= entry.addr || end(&entry) <= code_section_begin) {
			add_entry(entry);
			continue;
		}
		// {[}]
		if (entry.addr <= code_section_begin && end(&entry) <= code_section_end) {
			entry.len = code_section_begin - entry.addr; // > 0
			add_entry(entry);
			continue;
		}
		// [{]}
		if (code_section_begin <= entry.addr && code_section_end <= end(&entry)) {
			entry.addr = code_section_end; // len > 0
			add_entry(entry);
			continue;
		}
		// {[]}
		if (entry.addr <= code_section_begin && code_section_end <= end(&entry)) {
			unsigned long long old_end = end(&entry);
			entry.len = code_section_begin - entry.addr; // len > 0
			add_entry(entry);
			entry.addr = code_section_end;
			entry.len = old_end - code_section_end;
			add_entry(entry);
			continue;
		}
		printf("Something went wrong\n");
		
	}
}

//-----------------printing_information------------------------------

void print_multiboot_header() {
	init();
	if (!is_valid_multiboot_header(multiboot_header_ptr)) {
		printf("Error: multiboot_header at %llx isn't valid!\n");
		return;
	}
	struct_description_t description = header_descriptor(multiboot_header_ptr);
	printf("%mc", &description);
}	

void print_info_header() {
	init();
	struct_description_t description = info_descriptor(multiboot_info_ptr);
	printf("%mc", &description);
}	

void print_memory_map(unsigned long long mmap_addr, unsigned long long mmap_length) {
	memory_map_entry_t* entry;
	printf("-----Memory map (at %llx)-----\n", mmap_addr);
	for (unsigned long long entry_addr = mmap_addr; entry_addr < mmap_addr + mmap_length; entry_addr += entry->size + 4) {
		entry = (memory_map_entry_t*) entry_addr;
		struct_description_t description = memory_map_entry_descriptor(entry);
		printf("%msso range is [%llu, %llu)\n\n", &description, entry->addr, entry->addr + entry->len);
	}
	printf("-----End of memory map-----\n");
}

void print_default_memory_map() {
	init();
	printf("NB: It's defualt memory map. It can be overriden.\n");
	print_memory_map(multiboot_info_ptr->mmap_addr, multiboot_info_ptr->mmap_length);
}

void print_actual_memory_map() {
	init();
	print_memory_map((unsigned long long) &memory_map, memory_map_size * 24);
}




