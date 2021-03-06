typedef u_int8_t dtype;
typedef u_int8_t rtype;
#define cT  numo_cUInt8
#define cRT cT

#define m_num_to_data(x) ((dtype)NUM2UINT(x))
#define m_data_to_num(x) UINT2NUM((unsigned int)(x))
#define m_extract(x)     UINT2NUM((unsigned int)*(dtype*)(x))
#define m_sprintf(s,x)   sprintf(s,"%u",(unsigned int)(x))
#define m_rand           ((dtype)gen_rand32())

#include "uint_macro.h"
