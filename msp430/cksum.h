#ifndef _CKSUM_H_
#define _CKSUM_H_
#include <stdint.h>
#include <stddef.h>

uint32_t fletcher32(uint16_t *pdata, size_t nwords);
uint32_t alder32(uint16_t *pdate, size_t nwords);

#endif
