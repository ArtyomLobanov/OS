#include "memory_map.h"
#include "memory.h" //import PAGE_SIZE
#include "output.h"

#define MAX_LEVEL 16
#define MAX_NUMBER_OF_PAGES 50000

#define LEVEL_MASK ((1 << 7) - 1)
#define get_level(x) (x.level & LEVEL_MASK)
#define is_used(x) ((x.level & (1 << 7)) != 0)

struct buddy_descriptor {
	// links in list on it's current level
	int next_ptr;
	int prev_ptr;
	unsigned char level; // is last bit is set, buddy is reserved
}__attribute__((packed));
typedef struct buddy_descriptor buddy_descriptor_t;

struct simple_buddy_alloc {
	unsigned long long shift;
	unsigned long long length;
	int lists[MAX_LEVEL + 1];
	buddy_descriptor_t buffer[MAX_NUMBER_OF_PAGES];
};
typedef struct simple_buddy_alloc simple_buddy_alloc_t;

void remove(simple_buddy_alloc_t* ptr, int number) {
	buddy_descriptor_t d = ptr->buffer[number];
	if (d.prev_ptr == -1) {
		ptr->lists[get_level(d)] = d.next_ptr;
	} else {
		ptr->buffer[d.prev_ptr].next_ptr = d.next_ptr;
	}
	if (d.next_ptr != -1) {
		ptr->buffer[d.next_ptr].prev_ptr = d.prev_ptr;
	}
}

void add(simple_buddy_alloc_t* ptr, int number) {
	buddy_descriptor_t d = ptr->buffer[number];
	d.prev_ptr = -1;
	d.next_ptr = ptr->lists[d.level];
	if (d.next_ptr != -1) 
		ptr->buffer[d.next_ptr].prev_ptr = number;
	ptr->lists[d.level] = number;
	ptr->buffer[number] = d;
}

int min(int a, int b);

void free_block(simple_buddy_alloc_t* ptr, int number) {
	buddy_descriptor_t d = ptr->buffer[number];
	if (!is_used(d)) return;
	int level = get_level(d);
	while (level < MAX_LEVEL) {
		int buddy_number = number ^ (1 << level);
		if (buddy_number >= MAX_NUMBER_OF_PAGES) break;
		if (is_used(ptr->buffer[buddy_number])) break;
		if (get_level(ptr->buffer[buddy_number]) != level) break;
		remove(ptr, buddy_number);
		ptr->buffer[buddy_number].level ^= 1 << 7;
		level++;
		number = min(number, buddy_number);
	}
	ptr->buffer[number].level = level; // no 7nth bit (free)
	add(ptr, number);	
}

int alloc_block(simple_buddy_alloc_t* ptr, int level) {
	int not_empty_level = level;
	while (ptr->lists[not_empty_level] == -1 && not_empty_level <= MAX_LEVEL) {
		not_empty_level++;
	} 
	if (not_empty_level > MAX_LEVEL) return -1;
	int number = ptr->lists[not_empty_level];
	remove(ptr, number);
	ptr->buffer[number].level ^= 1 << 7;
	while (not_empty_level > level) {
		not_empty_level--;
		int buddy_number = number ^ (1 << not_empty_level);
		ptr->buffer[buddy_number].level ^= 1 << 7; // so it's free now
		add(ptr, buddy_number);
	}
	ptr->buffer[number].level = not_empty_level + (1 << 7);
	return number;	
}

void init_simple_buddy_alloc(simple_buddy_alloc_t* ptr, unsigned long long begin, unsigned long long length) {
	if (begin % PAGE_SIZE != 0) {
		length -= PAGE_SIZE - (begin % PAGE_SIZE);
		begin += PAGE_SIZE - (begin % PAGE_SIZE);
	}
	if (begin == 0) {
		begin += PAGE_SIZE;
		length -= PAGE_SIZE;
	}
	if (length / PAGE_SIZE <= 0) {
		printf("Warning at simple_buddy_alloc: too smal range of memory - buddy_alloc is empty");
		length = 0;
	}
	if (length / PAGE_SIZE > MAX_NUMBER_OF_PAGES) {
		printf("Warning at simple_buddy_alloc: max number of pages is %u, but real is %u. ", MAX_NUMBER_OF_PAGES, length / PAGE_SIZE);
		length = MAX_NUMBER_OF_PAGES * PAGE_SIZE;
	}
	ptr->shift = begin;
	ptr->length = length;
	for (int i = 0; i <= MAX_LEVEL; i++) {
		ptr->lists[i] = -1;
	}
	for (int i = 0; i < MAX_NUMBER_OF_PAGES; i++) {
		ptr->buffer[i].prev_ptr = i - 1;
		ptr->buffer[i].next_ptr = i + 1;
		ptr->buffer[i].level = (1 << 7);
	}
	int page_number = length / PAGE_SIZE;
	for (int i = 0; i < page_number; i++) {
		free_block(ptr, i);
	}
}

unsigned long long get_free_pages_number(simple_buddy_alloc_t* ptr) {
	unsigned long long ans = 0;
	for (int level = 0; level <= MAX_LEVEL; level++) {
		int cnt = 0;
		int current = ptr->lists[level];
		while (current != -1) {
			cnt++;
			current = ptr->buffer[current].next_ptr;
		}
		ans += cnt * (1LL << level);

	}
	return ans;
}


//-------------------------multy_buddy_alloc----------------------------

#define MAX_SIMPLE_BUDDY_ALLOC_NUMBER 5

simple_buddy_alloc_t allocs[MAX_SIMPLE_BUDDY_ALLOC_NUMBER];
int actual_size = 0;
int init_flag = 0;

void init_multy_buddy_alloc() {
	if (init_flag) return;
	init_flag = 1;
	init(); // memory map init
	actual_size = 0;
	for (int i = 0; i < memory_map_size; i++) {
		if (memory_map[i].type != 1) continue;
		if (memory_map[i].len / PAGE_SIZE < 2) continue;
		if (actual_size == MAX_SIMPLE_BUDDY_ALLOC_NUMBER) {
			printf("WARNING: need bigger simple_buddy_alloc table");
			break;
		}
		init_simple_buddy_alloc(&allocs[actual_size++], memory_map[i].addr, memory_map[i].len);
	}
	printf("Multy buddy allocator init finished\n");
}

unsigned long long ggmpom(unsigned long long page_count) { //god_give_me_piece_of_memory
	init_multy_buddy_alloc();
	int level = 0;
	while ((unsigned long long)(1LL << level) < page_count) level++;
	for (int i = 0; i < actual_size; i++) {
		int ptr = alloc_block(&allocs[i], level);
		if (ptr != -1) {
			return allocs[i].shift + ptr * (unsigned long long) PAGE_SIZE;;	
		}
	}
	return 0;
}

void gttpom(unsigned long long phys_address) { //god_take_this_piece_of_memory
	init_multy_buddy_alloc();
	int alloc_number = -1;
	for (int i = 0; i < actual_size; i++) {
		if (allocs[i].shift <= phys_address && phys_address - allocs[i].shift < allocs[i].length) {
			alloc_number = i;
			break;
		}
	}
	if (alloc_number == -1 || phys_address % PAGE_SIZE != 0) {
		printf("Error: address not supported\n");
		return;
	}
	int block_number = (phys_address - allocs[alloc_number].shift) / PAGE_SIZE;
	free_block(&allocs[alloc_number], block_number);
}

unsigned long long free_pages_number() {
	init_multy_buddy_alloc();
	unsigned long long ans = 0;
	for (int i = 0; i < actual_size; i++) {
		ans += get_free_pages_number(&allocs[i]);
	}
	return ans;
}

void status() {
	init_multy_buddy_alloc();
	printf("Multy Buddy allocator status:\n");
	printf("  Number of allocators is %u:\n", actual_size);
	for (int i = 0; i < actual_size; i++) {
		printf("    range[%u, %u) free page number = %u \n", allocs[i].shift, allocs[i].shift + allocs[i].length, 
			get_free_pages_number(&allocs[i]));
	}
	printf("\n");
}










