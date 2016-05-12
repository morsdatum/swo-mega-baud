#include "lsfr.h"
#include "cunitest.h"

extern uint16_t shift_reg16;

#if (TESTCASE==1) || defined(TESTALL)
UNITEST_DECLARE(lsfr16_init,1)

UNITEST_START(lsfr16_init,1)

lsfr16_init(0x0);
ASSERT_FALSE(shift_reg16==0xACE5u,"lsfr16 zero init failed");
lsfr16_init(0xffff);
ASSERT_FALSE(shift_reg16==0xACE5u,"lsfr16 max init failed");
lsfr16_init(1);
ASSERT_FALSE(shift_reg16==1,"lsfr16 value 1 init failed");
lsfr16_init(1000);
ASSERT_FALSE(shift_reg16==1000,"lsfr16 value 1000 init failed");
lsfr16_init(65530);
ASSERT_FALSE(shift_reg16==65530,"lsfr16 value 65530 init failed");

UNITEST_END
#endif


#if (TESTCASE==3) || defined(TESTALL)
UNITEST_DECLARE(fibonacci_lsfr16,3)

UNITEST_START(fibonacci_lsfr16,3)

uint16_t tmp;
lsfr16_init(65021);
tmp = fibonacci_lsfr16();
ASSERT_TRUE(fibonacci_lsfr16() == tmp,"two call fibancci_lsfr16 values fail");
fibonacci_lsfr16();
ASSERT_TRUE(fibonacci_lsfr16() == tmp,"two distance call fibancci_lsfr16 values fail");

UNITEST_END
#endif

#if (TESTCASE==4) || defined(TESTALL)
UNITEST_DECLARE(galois_lsfr16,4)

UNITEST_START(galois_lsfr16,4)

uint16_t tmp;
lsfr16_init(22111);
tmp = galois_lsfr16();
ASSERT_TRUE(galois_lsfr16() == tmp,"two call galois_lsfr16 values fail");
galois_lsfr16();
galois_lsfr16();
ASSERT_TRUE(galois_lsfr16() == tmp,"two distance call galois_lsfr16 values fail");

UNITEST_END
#endif