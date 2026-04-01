#ifndef __CA_DATA_
#define __CA_DATA_

/*
 CAN be set to CA_NO_DATA (0)
 indicating that there is no value
 will use defuault values
 */
#ifndef _OPTIONAL
#define _OPTIONAL __CA_OPTIONAL
#endif
#ifndef __CA_OPTIONAL
#define __CA_OPTIONAL
#endif

/*
 CAN not be set to CA_NO_DATA (0)
 requires a value
 */
#ifndef _REQUIRED
#define _REQUIRED __CA_REQUIRED
#endif
#ifndef __CA_REQUIRED
#define __CA_REQUIRED
#endif

typedef struct {
    void *buf;
    struct {
       ca_mlen cap;
       ca_mlen mem;
    }len;
} ca_vector;

ca_vector *ca_vector_new(
       __CA_OPTIONAL ca_mlen mlen);


int ca_vector_read(ca_vector *vec,
                   void *to,
         _OPTIONAL ca_mlen mlen);

int ca_vector_resize(
         _OPTIONAL ca_vector *_REQUIRED vec,
         _OPTIONAL ca_mlen mlen);


int ca_vector_del(ca_vector *vec);
int ca_vector_write(ca_vector *vec,
                    void *data, size_t mlen,
                    bool from_start);

#endif

#if defined(CA_DATA_IMPL) | defined(DEBUG) | defined(TEST)

#include <stdlib.h>
#include <errno.h>

//VECTOR START
ca_vector *ca_vector_new(ca_mlen mlen){
    ca_vector *out = calloc(1, sizeof(ca_vector));
    if(out==NULL) goto error;

    out->len.mem = mlen > CA_VECTOR_NO_DATA ?
                   mlen : CA_DATA_DEF_MLEN <= mlen ?
                   mlen : CA_DATA_DEF_MLEN;

    out->buf = calloc(1,out->len.mem);

    out->len.cap = out->len.mem;
    out->buf = calloc(1, out->len.mem);
    if(out==NULL) goto free;


    return out;
free:
    ca_vector_del(out);
    return NULL;
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
//END
}

int ca_vector_resize(ca_vector *vec, size_t mlen){
    if(mlen < CA_NO_DATA) goto inval;
    if(vec == NULL) vec = ca_vector_new(CA_NO_DATA);

    vec->len.mem =  ca_optional_len(mlen);
    vec = realloc(vec, mlen);
    vec->len.cap += mlen;
    return 1;
inval:
    errno=EINVAL;
    return 0;
//END
}

int ca_vector_write(ca_vector *vec, void *data, size_t mlen, bool overwrite){
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
//END
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
//END
}
//VECTOR END
#endif
