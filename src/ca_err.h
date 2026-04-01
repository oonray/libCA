#ifndef __CA_ERR
#define __CA_ERR

#include <errno.h>

#define ca_ret(RET)\
    return (RET)

#define ca_ret_ok(RET)\
ok: ca_ret(RET)

#define ca_err_rst() errno=0

#define ca_err_inval(RET)\
inval: errno=EINVAL; ca_ret(RET)

#define ca_err_error(RET)\
error: ca_ret(RET)

#define ca_err_error_f(OBJ,RET)\
error:  if(ca_is_nul(OBJ)) free((OBJ)); ca_ret(RET)

#define ca_is_nul(T)\
    (T)==NULL

#define ca_no_data(T)\
    (T)<=CA_NO_DATA

#define ca_is_def_len(T)\
    (T)>CA_DEF_LEN

#define ca_chk_nul(T)\
    if(ca_is_nul(T))

#define ca_chk_nul_e(T)\
    ca_chk_nul(T) ca_go_err

#define ca_chk_nul_i(T)\
    ca_chk_nul(T) ca_go_inval

#endif
