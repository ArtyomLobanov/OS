#include "slab.h"
#include "memory_mapping.h"
#include "buddy_alloc.h"
#include "output.h"
#include "memory.h"

#define MEMORY_SPACE_HEADER_MAGIC 0xA6CAD239
#define MAX_CACHE_LEVEL 11
#define MAX_CACHE_SPACE (1 << MAX_CACHE_LEVEL)

struct memory_space_header {
	unsigned long long space_length;
	unsigned long long magic;
	unsigned long long check_sum;
}__attribute__((packed));
typedef struct memory_space_header memory_space_header_t;

slub_allocator_t* allocators[MAX_CACHE_LEVEL + 1];
static int init_flag = 0;
void init_general_allocator() {
	if (init_flag) return;
	init_flag = 1;
	for (int i = 0; i <= MAX_CACHE_LEVEL; i++)
		allocators[i] = 0;
}

int check_init(unsigned int level) {
	if (allocators[level] != 0) return 1;
	unsigned int expansion_rate = ((1 << level) - 1) / PAGE_SIZE + 1;
	unsigned int best_exp_rate = 1;
	while (best_exp_rate < expansion_rate) best_exp_rate *= 2;
	allocators[level] = create_slab_2(1 << level, best_exp_rate);
	return allocators[level] != 0;
}

void* alloc(unsigned long long size) {
	init_general_allocator();
	if (size == 0) return 0;
	size += sizeof(memory_space_header_t);
	void* res = 0;
	if (size > MAX_CACHE_SPACE) {
		unsigned long long address = ggmpom((size - 1) / PAGE_SIZE + 1);
		if (address != 0)
			res = to_logic_address(address);
	} else {
		int level = 0;
		while (size > (1u << level)) level++;
		while (level <= MAX_CACHE_LEVEL) {
			size = (1 << level);
			if (check_init(level)) {
				res = slab_alloc(allocators[level]);
				if (res != 0) break;
			}
		}
	}
	if (res == 0) return 0;
	memory_space_header_t* space = (memory_space_header_t*) res;
	space->space_length = size;
	space->magic = MEMORY_SPACE_HEADER_MAGIC;
	space->check_sum = -(size + MEMORY_SPACE_HEADER_MAGIC);
	return &space[1];
}

void free(void* ptr) {
	if (ptr == 0) return;
	init_general_allocator();
	memory_space_header_t* space = ((memory_space_header_t*) ptr) - 1;
	if (space->magic != MEMORY_SPACE_HEADER_MAGIC || space->magic + space->space_length + space->check_sum != 0) {
		printf("Error: invalid address or bitten header!\n");
		return;
	} 
	if (space->space_length > MAX_CACHE_SPACE) {
		gttpom(to_phys_address(space));
		return;
	}
	int level = 0; 
	while (space->space_length > (1u << level)) level++;
	if ((1u << level) != space->space_length || allocators[level] == 0) {
		printf("Error: unexpected length!\n");
		return;
	}
	slab_free(allocators[level], space);
}

void general_allocator_status() {
	init_general_allocator();
	printf("---------------------general-allocator-status-----------------------\n");
	for (int i = 0; i <= MAX_CACHE_LEVEL; i++) {
		printf("  slab_allocator for size %u, status: ", (1u << i));
		if (allocators[i] == 0) {
			printf("uninitialized\n");
		} else {
			printf("available %u blocks\n", blocks_available(allocators[i]));
		}
	}
	printf("buddy_allocator has %llu free pages\n\n", free_pages_number());
}

