#ifndef _LSFR_H_
#define _LSFR_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


uint16_t fibonacci_lsfr16(void);
uint16_t galois_lsfr16(void);
void lsfr16_init(uint16_t init_val);
  
uint32_t fibonacci_lsfr32(void);
uint32_t galois_lsfr32(void);
void lsfr32_init(uint32_t init_val);

#ifdef __cplusplus
}
#endif

#endif
