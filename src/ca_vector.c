
//LIBS -----------
//USR ---
#include <ca_err.h>
#include <ca_defs.h>
#include <ca_vector.h>
//INC ---
//SYS ---
#include <stdlib.h>
#include <stdio.h>
//END ------------

//VECTOR START
ca_vector *
ca_vector_new(_OPTIONAL const size_t mlen)
{
    ca_vector *out = calloc(1, sizeof(ca_vector));
    ca_chk_nul_e(out);

    out->len.mem =  ca_optional_len(mlen);

    out->buf = calloc(1,out->len.mem);
    ca_chk_nul_e(out->buf);

    out->len.cap = out->len.mem;

    return out;
    ca_err_error_f(out, 0);
}

uint8_t
ca_vector_clear(ca_vector *const vec)
{
    ca_err_rst();
    ca_chk_nul_i(vec);
    ca_chk_nul_i(vec->buf);

    ca_zero(vec->buf,vec->len.mem);

    vec->len.mem=0;
    vec->len.cap=0;

    return 1;
    ca_err_inval(0);
}

uint8_t
ca_vector_del(ca_vector **vec)
{
    ca_err_rst();
    ca_chk_nul_i(vec);
    ca_chk_nul_i(*vec);

    if(ca_pp(vec)->buf!=NULL){
        ca_vector_clear(*vec);
        ca_free(ca_pp(vec)->buf);
    }

    ca_pp(vec)->len.cap = 0;
    ca_pp(vec)->len.mem = 0;

    ca_free(*vec);

    return 1;
    ca_err_inval(0);
//END
}

uint8_t
ca_vector_resize(
         ca_vector **const vec,
         _OPTIONAL const size_t mlen)
{
    ca_chk_nul_i(vec);

    if(ca_is_nul(vec)){
        *vec = ca_vector_new(CA_NO_DATA);
        ca_chk_nul_e(vec);
        ca_go_ok;
    }

    size_t ln = ca_optional_len(mlen);

    ca_pp(vec)->buf = realloc(ca_pp(vec)->buf,
                              ca_pp(vec)->len.mem + ln);
    ca_chk_nul_e(vec);
    ca_pp(vec)->len.cap += mlen;

    ca_ret_ok(1);
    ca_err_inval(0);
    ca_err_error(0);
}

uint8_t
ca_vector_write(ca_vector *vec,
                    const uint8_t *const data,
                    const size_t mlen,
                    _OPTIONAL const size_t start)
{
    if(ca_is_nul(vec)
       || ca_is_nul(data)
       || ca_no_data(mlen)) ca_go_inval;

    if(ca_vector_mem_guard(vec, mlen)) ca_go_inval;

    if(!ca_vector_full(vec)
       || ca_vector_cap(vec, mlen)
       || ca_vector_cap(vec, start + mlen)
       || ca_vector_ok_start(vec, start))
          ca_vector_resize(&vec, mlen);

    size_t st = ca_optional_len(start);

    unsigned char *src =
        (unsigned char *)vec->buf + st
            + (vec->len.mem - vec->len.cap);

    unsigned char *dst = (unsigned char *)data;

    int i;
    for(i=0;i < mlen;i++){
        src[i] = dst[i];
        vec->len.cap--;
        vec->len.use++;
    }

    return i;
    ca_err_inval(0);
}

uint8_t
ca_vector_copy(ca_vector *const vec,
               void *const to,
               _OPTIONAL const size_t start,
               _OPTIONAL const size_t mlen)
{
    if(vec==NULL
      || to==NULL
      || mlen <= CA_NO_DATA) goto inval;

    size_t rlen = vec->len.mem;
    if(mlen<vec->len.mem) rlen = mlen;

    unsigned char *src = (unsigned char *)vec->buf;
    unsigned char *dst = (unsigned char *)to;
    for(int i=0;i < rlen;i++)
        dst[i] = src[i];

    return mlen;
    ca_err_inval(0);
}
