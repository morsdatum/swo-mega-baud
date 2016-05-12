
#include "uart0.h"
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

  
  xmit_strz_poll("\n\rTest passed: ");
  xmit_dec_poll4(pass);
  xmit_strz_poll("\n\rTest failed: ");
  xmit_dec_poll4(fail);
}
		 
