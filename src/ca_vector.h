#ifndef __CA_VECTOR_
#define __CA_VECTOR_
//LIBS -----------
//USR ---
#include <ca_defs.h>
//INC ---
//SYS ---
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
//END ------------

typedef struct {
    void *buf;
    struct {
       size_t cap; //avaliable writing space
       size_t mem; //Total writing space
       size_t use; //Used writing space
    }len;
} ca_vector;

#define ca_vector_full(VEC)\
    ((VEC)->len.cap == 0)

#define ca_vector_cap(VEC,LEN)\
    (((VEC)->len.mem >= (LEN)) \
     && ((VEC)->len.cap >= (LEN)))

#define ca_vector_ok_start(VEC,START)\
    ((VEC)->len.mem < (START))

#define ca_vector_mem_guard(VEC, LEN)\
    ((LEN) > ((VEC)->len.mem * (LEN)))

ca_vector *
ca_vector_new(_OPTIONAL const size_t mlen);


/*
 * Resized the vector pointed at
 * IF the vector is NULL one will be created
 */
uint8_t
ca_vector_resize(ca_vector **const restrict vec,
                 _OPTIONAL const size_t mlen);

uint8_t
ca_vector_del(ca_vector **const vec),
ca_vector_clear(ca_vector *const vec);

// if from start data will be overwritten from ->buf + 0
// else will write to ->buf + ->len.mlen
// realloc if len.mlen + mlen > ->len.cap
uint8_t
ca_vector_write(ca_vector *restrict vec,
                const uint8_t *const restrict data,
                const size_t mlen,
                _OPTIONAL const size_t start);

// If length is 0 it will read vector->len.mem
uint8_t
ca_vector_copy(ca_vector *const restrict vec,
               void *const restrict to,
               _OPTIONAL const size_t start,
               _OPTIONAL const size_t mlen);

#endif

