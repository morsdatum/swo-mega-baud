#include "lsfr.h"
#include "cunitest.h"

extern uint32_t shift_reg32;

#if (TESTCASE==10) || defined(TESTALL)
UNITEST_DECLARE(lsfr32_init,10)

UNITEST_START(lsfr32_init,10)

lsfr32_init(0);
ASSERT_FALSE(shift_reg32==0x1EDC6F41u,"lsfr32 zero init fail");
lsfr32_init(0xffffffffu);
ASSERT_FALSE(shift_reg32==0x1EDC6F41u,"lsfr32 max value init fail");
lsfr32_init(0x12345113u);
ASSERT_TRUE(shift_reg32==0x1EDC6F41u,"lsfr32 customed value init fail");

UNITEST_END
#endif

