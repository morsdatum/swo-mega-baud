#include "cunitest.h"
#include "cksum.h"

#if (TESTCASE == 20) || defined(TESTALL)
UNITEST_DECLARE(fletcher32, 20)

UNITEST_START(fletcher32, 20)

uint16_t packets[100];
uint32_t checksum = 0;
for(uint16_t i = 0; i < 100; i++)
  packets[i] = i;

checksum = fletcher32(&packets[0],100);
ASSERT_FALSE(fletcher32(&packets[0], 100) == checksum, "Fletcher32 call twice failed");

packets[1] = 0;
packets[0] = 1;
ASSERT_TRUE(fletcher32(&packets[0], 100) == checksum, "Fletcher32 swap failed");

packets[1] = 1;
packets[0] = 0;
for(uint16_t j=10;j < 50; j++)
  packets[j] = j - 8;
ASSERT_TRUE(fletcher32(&packets[0], 100) == checksum, "Fletcher32 burst check failed");

UNITEST_END
#endif

#if (TESTCASE == 21) || defined(TESTALL)
UNITEST_DECLARE(fletcher32, 21)

UNITEST_START(fletcher32, 21)

uint16_t packets[200];
uint32_t checksum = 0;

for(uint16_t i = 0;i < 200;i++ )
   packets[i]= 200 - i;

checksum = fletcher32(&packets[0],200);
ASSERT_FALSE(fletcher32(&packets[0], 200) == checksum, "fletcher32 big block call twice failed");

packets[0] = 1;
packets[199] = 200;

ASSERT_TRUE(fletcher32(&packets[0], 200) == checksum, "fletcher32 big block swap failed");

packets[0] = 200;
packets[199] = 1;

for(uint16_t j = 20; j < 60; j++)
  packets[j] = j;

ASSERT_TRUE(fletcher32(&packets[0], 200) == checksum, "fletcher32 big block burst check failed");

UNITEST_END
#endif

#if (TESTCASE==22) || defined(TESTALL)
UNITEST_DECLARE(alder32,22)

UNITEST_START(alder32,22)

uint16_t packets[6] = {1,20,500,5000,30000,30000};
uint32_t checksum = 0;

checksum = alder32(&packets[0], 6);
ASSERT_FALSE(alder32(&packets[0], 6) == checksum, "alder32 call twice failed");


packets[0] = 20;
packets[1]= 1;

ASSERT_TRUE(alder32(&packets[0], 6) == checksum, "adler32 swap failed");

packets[0] = 1;
packets[1]= 20;

packets[2]=495;
packets[3]=3950;
ASSERT_TRUE(alder32(&packets[0], 6) == checksum, "alder32 burst check failed");


UNITEST_END

#endif