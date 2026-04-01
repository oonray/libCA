#ifndef __CA_DEFS_
#define __CA_DEFS_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define CA_NO_DATA 0

/*
 CAN be set to CA_NO_DATA (0)
 indicating that there is no value
 will use defuault values
 */
#define _OPTIONAL

/*
 CAN not be set to CA_NO_DATA (0)
 requires a value
 function will fail with EINVAL
 */
#define _REQUIRED

#define ca_optional(VAR, OPTION)\
    ((VAR) == CA_NO_DATA) ? \
              CA_NO_DATA  : (OPTION);

#define ca_optional_len(VAR)\
    ((VAR) == CA_NO_DATA) ? \
              CA_DEF_LEN : (VAR) < CA_DEF_LEN? \
              CA_DEF_LEN : (VAR);

#define CA_DEF_LEN 64

#define ca_pp(P) (*(P))

#define ca_free(BUF)\
    if((BUF)!=NULL){\
        free((BUF));\
        (BUF)=NULL;}

#define ca_zero(VEC,LEN)\
    if((VEC)!=NULL && (LEN) > 0)\
        for(int i = LEN;i<LEN;i++)\
            ((uint8_t *)(VEC))[i] = 0

#define ca_go_inval goto inval
#define ca_go_err goto error
#define ca_go_ok goto ok

#endif
