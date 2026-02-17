#ifndef __CA_DATA_
#define __CA_DATA_

#include <stddef.h>
#include <string.h>

typedef unsigned char ca_byte;
typedef unsigned int ca_num;

typedef enum {
    CA_DATA_DEF_MIN = 0,
    CA_DATA_DEF_DATA = 64,
    CA_DATA_DEF_MTYPE = sizeof(ca_byte),
    CA_DATA_DEF_MLEN = CA_DATA_DEF_DATA * CA_DATA_DEF_MTYPE,
    CA_DATA_DEF_MAX = CA_DATA_DEF_MLEN,
}ca_data_def_f;

typedef enum {
    CA_VECTOR_MIN = 0,
    CA_VECTOR_NO_DATA = 0,
    CA_VECTOR_NO_ITEMS = 0,
    CA_VECTOR_NO_START = 0,
    CA_VECTOR_NO_OVERWRITE = 0,
    CA_VECTOR_OVERWRITE = 1,
    CA_VECTOR_MAX = 0,
}ca_vector_f;

typedef struct {
    void *buf;
    struct {
       ca_num cap;
       size_t mem;
    }len;
} ca_vector;

typedef struct {
    void *start;
    void *end;
    size_t span;
}ca_data_view;

ca_vector *ca_vector_new(void *init_data, size_t mlen);

int ca_vector_del(ca_vector *vec);
int ca_vector_resize(ca_vector *vec, size_t mlen);

int ca_vector_write(ca_vector *vec, void *data, size_t mlen, int overwrite);
int ca_vector_read(ca_vector *vec, void *out, size_t mlen);


#endif

#if defined(CA_DATA_IMPL) | defined(DEBUG) | defined(TEST)
#include <stdlib.h>
#include <errno.h>

ca_vector *ca_vector_new(void *init, size_t mlen){
    if(mlen < CA_VECTOR_NO_DATA
       || (mlen <= CA_VECTOR_NO_DATA && init != NULL)
      )goto inval;

    ca_vector *out = calloc(1, sizeof(ca_vector));
    if(out==NULL) goto error;

    out->len.mem = CA_DATA_DEF_MLEN;
    if(mlen > CA_VECTOR_NO_DATA)
        if(mlen > CA_DATA_DEF_MLEN)
            out->len.mem = mlen;

    out->len.cap = out->len.mem;
    out->buf = calloc(1, out->len.mem);

    if(mlen > CA_VECTOR_NO_DATA)
        if(init!=NULL)
            for(int i = 0;i < mlen;i++)
                ((ca_byte *)out->buf)[i] = ((ca_byte *)init)[i];

    return out;
inval:
    errno=EINVAL;
error:
    return NULL;
}

int ca_vector_del(ca_vector *vec){
    if(vec==NULL) goto inval;
    if(vec->buf!=NULL) free(vec->buf);
    vec->buf=NULL;
    vec->len.cap=0;
    vec->len.mem=0;
    return 1;
inval:
    errno=EINVAL;
    return 0;
}

int ca_vector_resize(ca_vector *vec, size_t mlen){
    if(mlen < CA_VECTOR_NO_DATA) goto inval;
    if(vec == NULL) vec = ca_vector_new(NULL, CA_VECTOR_NO_DATA);

    if(mlen == CA_VECTOR_NO_DATA) mlen = CA_DATA_DEF_MLEN;
    vec = realloc(vec, mlen);
    vec->len.cap += mlen;
    return 1;
inval:
    errno=EINVAL;
    return 0;
}

int ca_vector_write(ca_vector *vec, void *data, size_t mlen, int overwrite){
  if(vec==NULL
    || data==NULL
    || mlen <= CA_VECTOR_NO_DATA
    || overwrite < CA_VECTOR_NO_OVERWRITE
    || overwrite > CA_VECTOR_OVERWRITE) goto inval;

    if(overwrite) vec->len.cap = vec->len.mem;

    if(mlen > vec->len.mem
       || mlen > vec->len.mem - vec->len.cap)
            ca_vector_resize(vec, CA_VECTOR_NO_DATA);

    ca_byte *src = (ca_byte *)vec->buf + (vec->len.mem - vec->len.cap);
    ca_byte *dst = (ca_byte *)data;
    for(int i=0;i < mlen;i++){
        src[i] = dst[i];
        vec->len.cap--;
    }

    return mlen;
inval:
  errno=EINVAL;
  return 0;
}

int ca_vector_read(ca_vector *vec, void *out, size_t mlen){
    if(vec==NULL
      || out==NULL
      || mlen <= CA_VECTOR_NO_DATA) goto inval;

    size_t rlen = vec->len.mem;
    if(mlen<vec->len.mem) rlen = mlen;

    ca_byte *src = (ca_byte *)vec->buf;
    ca_byte *dst = (ca_byte *)out;
    for(int i=0;i < rlen;i++)
        dst[i] = src[i];

    return mlen;
inval:
  errno=EINVAL;
  return 0;

}
#endif
