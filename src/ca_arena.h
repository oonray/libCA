#ifndef __arena_h
#define __arena_h

#include <stdint.h>
#include <stddef.h>

#define KB 1000 * sizeof(uint8_t)
#define MB 1000 * KB

#define CA_ARENA_DEF_SIZE 1 * KB

#define CA_SIZE_T_LEN (sizeof(size_t)/sizeof(uint8_t))
#define CA_ABLK_HEAD_LEN 1 + CA_SIZE_T_LEN

#define ca_arena_len(SIZE) (SIZE) + (3 * CA_SIZE_T_LEN);

struct ca_arena_block {
    size_t len;
    struct {
      uint8_t free:4;
      uint8_t lock:4;
    };
    uint8_t data[];
};

struct ca_arena {
    size_t len;
    size_t start;
    size_t end;
    struct ca_arena_block block[];
};

int ca_arena_free(struct ca_arena *arena);

int ca_free(struct ca_arena *const arena, void *ptr);
void *ca_alloc(struct ca_arena *const arena, size_t size);
void *ca_realloc(void *ptr, size_t size);

#endif
