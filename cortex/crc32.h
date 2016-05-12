#ifndef _CRC_32_H_
#define _CRC_32_H_

#include <stdint.h>
#include <stddef.h>

extern uint32_t _crc32_cksum;

inline void Crc32_init(void){
   _crc32_cksum = UINT32_MAX;
}

void Update_crc32_u8(uint8_t data);
void Update_crc32_u8ptr(uint8_t const *pdata, size_t nbytes);

inline uint32_t Read_crc32(void){
   return (_crc32_cksum);
}

#endif
