#ifndef __TEST_DEFS
#define __TEST_DEFS
//LIBS -----------
//USR ---
#include <ca_defs.h>
//INC ---
#include <munit.h>
//SYS ---
//END ------------


#define fail() goto error

#define test_resolve()\
    return MUNIT_OK;\
goto fail; \
goto error; \
fail:\
    return MUNIT_FAIL; \
error: \
    return MUNIT_ERROR;
#endif

