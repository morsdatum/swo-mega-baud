#include <stdint.h>
#include <libopencm3/cm3/itm.h>
#include <libopencm3/stm32/dbgmcu.h>
#include "swo.h"

static const uint8_t tab_hexdigit[]="0123456789ABCDEF";

static const uint16_t tab_div16[]={
		10000,
		1000,
		100,
		10,
		1
};

static const uint32_t tab_div32[]={
	1000000000,
	100000000,
	10000000,
	1000000,
	100000,
	10000,
	1000,
	100,
	10,
	1
};

/*inline void swo_init(void){
	DBGMCU_CR = DBGMCU_CR_TRACE_IOEN;
}*/

void swo_xmit_strz(const char *strz){
	do{
		itm_xmit_char((uint8_t)*strz);
		if(*strz=='\n')
			itm_xmit_char((uint8_t)'\r');
	}while(*strz++);
}

void swo_xmit_hex2(uint16_t val){
	itm_xmit_char((uint8_t)tab_hexdigit[(val >> 12) & 0xf]);
	itm_xmit_char((uint8_t)tab_hexdigit[(val >> 8) & 0xf]);
	itm_xmit_char((uint8_t)tab_hexdigit[(val >> 4) & 0xf]);
	itm_xmit_char((uint8_t)tab_hexdigit[val & 0xf]);
}

void swo_xmit_hex4(uint32_t val){
	swo_xmit_hex2((uint16_t)((val >> 16) & 0xffff));
	swo_xmit_hex2((uint16_t)(val & 0xffff));
}

void swo_xmit_dec2(uint16_t val){
	uint16_t d;
	  uint8_t ch;
	  const uint16_t *dp = tab_div16;
	  if(val==0)
	    itm_xmit_char('0');
	  else{
	    while(val < *dp)
	      ++dp;
	    do{
	      d = *dp++;
	      ch = '0';
	      while( val >= d){
		ch++;
		val -= d;
	      }
	      itm_xmit_char(ch);
	    }while(!(d & 1));
	  }
}

void swo_xmit_dec4(uint32_t val){
	  uint32_t d;
	  uint8_t ch;
	  const uint32_t *dp=tab_div32;
	  if(val==0)
	    itm_xmit_char('0');
	  else{
	    while(val < *dp)
	      ++dp;
	    do{
	      d = *dp++;
	      ch = '0';
	      while(val >= d){
		ch++;
		val -= d;
	      }
	      itm_xmit_char(ch);
	    }while(!(d & 1));
	  }

}

void itm_xmit_char(uint8_t ch) {
	if(((ITM_TCR) & ITM_TCR_ITMENA) &&   (ITM_TER[0] & (1UL << 0))){
		while(ITM_STIM32(0) == 0)
			;
		ITM_STIM8(0) = ch;
	}
}
