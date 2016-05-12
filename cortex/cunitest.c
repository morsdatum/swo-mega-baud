
#include "trace_printf.h"
#include "cunitest.h"

void test_runner(tests_fcnptr_t const *suite){
  unsigned int fail = 0, pass = 0;

  while(*suite != 0){
    if((**suite)() == 0)
      pass++;
    else
      fail++;
    *suite++;
  }

  trace_printf("Test passed: %d",pass);
  trace_printf("Test failed: %d",fail);
}
		 
