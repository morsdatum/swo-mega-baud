#include <stdint.h>
#include "lsfr.h"

uint32_t shift_reg32;
static const uint32_t tap32_itu[]={1, 1<<1,1<<2,1<<4,1<<5,1<<7,1<<8,
			    (uint32_t)1<<10,(uint32_t)1<<11,
			    (uint32_t)1<<12,(uint32_t)1<<16,
			    (uint32_t)1<<21,(uint32_t)1<<22,
			    (uint32_t)1<<26,(uint32_t)1<<31};

#define TAP32_SIZE 15

uint32_t fibonacci_lsfr32(void){
  uint8_t cnt ;
  uint32_t bit =0;
  for(cnt = 0; cnt < TAP32_SIZE;cnt++){
    if((tap32_itu[cnt] & shift_reg32) != 0)
      bit ^=0x80000000u ;
  }
  
  shift_reg32 = (shift_reg32 >> 1) | bit;

  return(shift_reg32);
}

uint32_t galois_lsfr32(void){
  uint8_t cnt;

  if((shift_reg32 & 1) == 0)
    shift_reg32 = (shift_reg32 >> 1) & 0x7FFFFFFFu;
  else{
    for(cnt=0; cnt< TAP32_SIZE; cnt++){
      shift_reg32 ^= tap32_itu[cnt];
    }
    shift_reg32 = (shift_reg32 >> 1) | 0x80000000u;
  }
  
  return shift_reg32;
}

void lsfr32_init(uint32_t init_val){
  if(init_val == 0u || init_val == UINT32_MAX)
    shift_reg32 = 0x1EDC6F41u;
  else
    shift_reg32 = init_val;
}
