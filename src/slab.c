#include "slab.h"
#include "memory.h"
#include "buddy_alloc.h"
#include "memory_mapping.h"
#include "output.h"

#define SLUB_BLOCK_MAGIC 0xF6ACC566

struct slub_block {
	unsigned long long magic;
	struct slub_block* next;
};
typedef struct slub_block slub_block_t;

slub_block_t* get_nth(void* from, unsigned long long number, unsigned int block_size) {
	return (slub_block_t*)(((char*)  from) + number * block_size);
}

slub_block_t* create_blocks_list(void* first_page, unsigned long long page_number, unsigned int block_size) {
	int block_counter = (page_number * PAGE_SIZE) / block_size;
	if (block_counter == 0) return 0;
	slub_block_t* blocks = (slub_block_t*) first_page;
	for (int i = 0; i < block_counter; i++) {
		get_nth(first_page, i, block_size)->magic = SLUB_BLOCK_MAGIC;
		get_nth(first_page, i, block_size)->next = get_nth(first_page, i + 1, block_size);
	}
	get_nth(first_page, block_counter - 1, block_size)->next = 0;
	return blocks;
}

int init_slab(slub_allocator_t* ptr, unsigned int block_size, unsigned int expansion_rate) {
	ptr->block_size = block_size;
	ptr->expansion_rate = expansion_rate;
	unsigned long long phys_addr = ggmpom(expansion_rate);
	if (phys_addr == 0) return 0;
	void* space_ptr = to_logic_address(phys_addr);
	ptr->list = create_blocks_list(space_ptr, expansion_rate, block_size);
	return 1;
}

void* slab_alloc(slub_allocator_t* ptr) {
	if (ptr->list == 0) {
		unsigned long long phys_addr = ggmpom(ptr->expansion_rate);
		if (phys_addr == 0) return 0;
		void* page_ptr = to_logic_address(phys_addr);
		ptr->list = create_blocks_list(page_ptr, ptr->expansion_rate, ptr->block_size);
	}	
	slub_block_t* res = (slub_block_t*) ptr->list;
	if (res->magic != SLUB_BLOCK_MAGIC) {
		printf("Error: invalid allocator!\n");
		return 0;
	}
	ptr->list = res->next;
	return res;
}

void slab_free(slub_allocator_t* ptr, void* space_ptr) {
	if (space_ptr == 0) return;
	slub_block_t* block = (slub_block_t*) space_ptr;
	block->magic = SLUB_BLOCK_MAGIC;
	block->next = (slub_block_t*) ptr->list;
	ptr->list = block;
}

unsigned long long blocks_available(slub_allocator_t* ptr) {
	slub_block_t* current = (slub_block_t*) ptr->list;
	unsigned long long res = 0;
	while (current != 0) {
		res++;
		if (current->magic != SLUB_BLOCK_MAGIC) {
			printf("Error: invalid allocator!\n");
			return 0;
		}
		current = current->next;
	}
	return res;
}

//----------------------slub_allocators_creating------------------------------

static int init_flag = 0;
slub_allocator_t main_allocator;

void init_slab_allocation() {
	if (init_flag) return;
	init_flag = 1;
	init_slab(&main_allocator, sizeof(slub_allocator_t), 1);
}	

slub_allocator_t* create_slab(unsigned int block_size) {
	return create_slab_2(block_size, 1);
	
}	

slub_allocator_t* create_slab_2(unsigned int block_size, unsigned int expansion_rate) {
	init_slab_allocation();
	if (expansion_rate != (expansion_rate & (~(expansion_rate - 1)))) {
		printf("Warning: it's better to use power of 2 as expansion_rate");
	}
	slub_allocator_t* allocator = (slub_allocator_t*) slab_alloc(&main_allocator);
	if (allocator == 0) return 0;
	return init_slab(allocator, block_size, expansion_rate)? allocator : 0;
	
}







