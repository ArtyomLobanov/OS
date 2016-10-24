#ifndef __SLAB__
#define __SLAB__

struct slub_allocator {
	void* list;
	unsigned int block_size;
	unsigned int expansion_rate;
};
typedef struct slub_allocator slub_allocator_t;

slub_allocator_t* create_slab(unsigned int block_size);

slub_allocator_t* create_slab_2(unsigned int block_size, unsigned int expansion_rate);

void* slab_alloc(slub_allocator_t* ptr);

void slab_free(slub_allocator_t* ptr, void* space_ptr);

unsigned long long blocks_available(slub_allocator_t* ptr);

#endif
