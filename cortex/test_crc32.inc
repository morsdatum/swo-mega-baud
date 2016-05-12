#include "crc32.h"

#if (TESTCASE==1) || defined(TESTALL)
UNITEST_DECLARE(Crc32_init,1)

UNITEST_START(Crc32_init,1)

Crc32_init();
ASSERT_FALSE(_crc32_cksum == 0xffffffff,"Crc32 accumlator initail failed");

UNITEST_END
#endif

#if (TESTCASE==2) || defined(TESTALL)
UNITEST_DECLARE(Update_crc32_u8,2)

UNITEST_START(Update_crc32_u8,2)

Crc32_init();  
Update_crc32_u8(1);
ASSERT_TRUE(_crc32_cksum == 0xffffffff,"Crc32 update failed");

UNITEST_END
#endif

#if (TESTCASE==3) || defined(TESTALL)
UNITEST_DECLARE(Update_crc32_u8ptr,3)

UNITEST_START(Update_crc32_u8ptr,3)

const unsigned char data[]={1,2,3,4,5,6};
Crc32_init();
Update_crc32_u8ptr(&data[0],5);
ASSERT_FALSE(_crc32_cksum!=0xffffffff,"Crc32 update array failed");

UNITEST_END
#endif