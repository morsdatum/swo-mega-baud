#include <stdint.h>
#include "lsfr.h"

uint16_t shift_reg16;
static const uint16_t tap16_ccitt[]={1, 1<< 8,1<< 9,1<<15};

#define TAP16_SIZE 4

uint16_t fibonacci_lsfr16(void){
  uint16_t bit =0;
  for(uint16_t cnt = 0; cnt < TAP16_SIZE;cnt++){
    if((tap16_ccitt[cnt] & shift_reg16) != 0)
      bit ^=0x8000u ;
  }
  
  shift_reg16 = (shift_reg16 >> 1) | bit;

  return(shift_reg16);
}

uint16_t galois_lsfr16(void){


  if((shift_reg16 & 1) == 0)
    shift_reg16 = (shift_reg16 >> 1) & 0x7FFFu;
  else{
    for(uint16_t cnt = 0; cnt < TAP16_SIZE; cnt++){
      shift_reg16 ^= tap16_ccitt[cnt];
    }
    shift_reg16 = (shift_reg16 >> 1) | 0x8000u;
  }
  
  return shift_reg16;
}

void lsfr16_init(uint16_t init_val){
  if(init_val != 0u && init_val != UINT16_MAX)
    shift_reg16 = init_val;
  else
    shift_reg16 = 0xACE5u;
}
