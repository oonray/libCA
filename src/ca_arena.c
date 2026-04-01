#include <ca_arena.h>

#include <stdlib.h>

struct ca_arena *ca_arena_new(size_t size){
    size_t len = size == 0 ? CA_ARENA_DEF_SIZE : size;
    size_t alen = ca_arena_len(len);

    struct ca_arena *arena = calloc(alen, sizeof(uint8_t));

    if(arena==NULL) return NULL;

    arena->len   = len;
    arena->end   = 1;
    arena->start = 1;

    arena->block[0].len = len;
    arena->block[0].free = 1;
    *(arena->block[0].data + len - CA_SIZE_T_LEN) = len;

    return arena;
};

int ca_arena_free(struct ca_arena *arena){
  if(arena==NULL) return 0;
  free(arena);
  return 1;
}

void *ca_alloc(struct ca_arena *const arena, size_t size){
    if(size==0) return NULL;

    //TODO: check capacity
    //TODO: look for avaliable fragment

    struct ca_arena_block *block = arena->block + arena->start;
    block->len = size + sizeof(size_t);
    arena->start += block->len;

    block->free = 1;
    *(block->data+size) = size;

    return (void *)block->data;
}

static inline struct ca_arena_block *
get_block(
    const uint8_t *const restrict data_ptr)
{
    if(data_ptr==NULL) return NULL;
    return (struct ca_arena_block *)(
            (uint8_t *)(data_ptr
              - CA_ABLK_HEAD_LEN));
}

static inline struct ca_arena_block *
get_prev_block(
    const struct ca_arena_block *const restrict blk_ptr)
{
    if(blk_ptr==NULL) return NULL;
    size_t prev_len = *(size_t *)blk_ptr - 1;
    return (struct ca_arena_block *)(
        ((uint8_t *)blk_ptr
            - prev_len
            - CA_ABLK_HEAD_LEN));
}

int ca_free(struct ca_arena *const arena, void *ptr)
{
    if(ptr==NULL) return 0;

    struct ca_arena_block *blk = get_block(ptr);

    for(int i=0; i<blk->len;i++)
        blk->data[i] = 0;

    blk->free = 1;

    struct ca_arena_block *prev_blk = get_prev_block(blk);

    if(prev_blk->free)
        prev_blk->len+=blk->len;

    return 1;
}
