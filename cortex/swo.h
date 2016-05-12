#ifndef _SWO_H_
#define _SWO_H_
#include <stdint.h>
#include <libopencm3/stm32/dbgmcu.h>

inline void swo_init(void){
  DBGMCU_CR = DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_SLEEP | DBGMCU_CR_STANDBY;
}

void swo_xmit_strz(const char *strz);
void swo_xmit_hex2(uint16_t val);
void swo_xmit_hex4(uint32_t val);
void swo_xmit_dec2(uint16_t val);
void swo_xmit_dec4(uint32_t val);

void itm_xmit_char(uint8_t ch);

#endif
