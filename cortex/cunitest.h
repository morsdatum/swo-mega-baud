#ifndef _C_UNIT_TEST_H_
#define _C_UNIT_TEST_H_

#include "trace_printf.h"

#ifndef __cplusplus

// put prefix #if before macro UNITEST_START
#define UNITEST_DECLARE(funcname, testnum) int test_##funcname##_##testnum(void);

#define UNITEST_START(funcname, testnum) int test_##funcname##_##testnum(void){      

#define ASSERT_TRUE(expr,msg) do{ if((expr)){ \
      trace_printf("%s: %s(), #%d, %s",__FILE__,__FUNCTION__,__LINE__,msg); \
      return -1;} }while(0)

#define ASSERT_FALSE(expr,msg) do{ if(!(expr)){ \
      trace_printf("%s: %s(), #%d, %s",__FILE__,__FUNCTION__,__LINE__,msg); \
      return -1;} }while(0)

#define UNITEST_END return 0; } // put posfix #endif after UNITEST_END

typedef int (*tests_fcnptr_t)(void);

#define UNITEST_FCT(funcname, testnum) (tests_fcnptr_t)(test_##funcname##_##testnum)

void test_runner(tests_fcnptr_t const *suite); // call after clock setup
						

#endif // not cplusplus

#endif // _C_UNIT_TEST_H_

