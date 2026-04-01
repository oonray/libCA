#ifndef __CA_VIEW_
#define __CA_VIEW_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//OPTINALS
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

#ifndef CA_NO_DATA
#define CA_NO_DATA 0
#endif

#ifndef CA_DEF_LEN
#define CA_DEF_LEN (64 * sizeof(uint8_t))
#endif

#ifndef ca_optional
#define ca_optional(VAR, OPTION)\
    ((VAR) == CA_NO_DATA) ? \
              CA_NO_DATA  : (OPTION);
#endif

#ifndef ca_optional_len
#define ca_optional_len(VAR)\
    ((VAR) == CA_NO_DATA) ? \
              CA_DEF_LEN : (VAR);
#endif
//OPTINALS END

//DEFS
typedef uint8_t ca_byte;
typedef size_t ca_mlen;
typedef size_t ca_len;
//DEFS END

#endif

typedef struct {
    void *start;
    void *end;
    ca_mlen span;
}ca_view;

ca_view *
ca_view_new(void *const buffer,
           _OPTIONAL const ca_mlen start,
           _REQUIRED const ca_mlen end);

int
ca_view_init(ca_view *const view,
             void    *const buffer,
            _OPTIONAL const ca_mlen start,
            _REQUIRED const ca_mlen end);

#if defined(CA_VIEW_IMPL) | defined(DEBUG) | defined(TEST)

#include <stdlib.h>
#include <errno.h>

ca_view *
ca_view_new(void *const buffer,
            const ca_mlen start, const ca_mlen end)
{
    if(buffer==NULL) goto inval;
    if(end==0) goto inval;

    ca_view *out = calloc(1, sizeof(ca_view));
    if(out==NULL) goto error;

    out->span  = end;
    out->start = ca_optional(start, buffer + start);

    out->end = out->start + end;
    return out;
inval:
    errno=EINVAL;
error:
    return NULL;
}

int
ca_view_init(ca_view *const view, void *const buffer,
             const ca_mlen start, const ca_mlen end)
{
    if(view==NULL
       || buffer==NULL) goto inval;
    if(end==0) goto inval;

    view->span  = end;
    view->start = ca_optional(start, buffer + start);

    view->end = view->start + end;

    return 1;
inval:
    errno=EINVAL;
    return 0;
}
#endif
