#include <stdint.h>
#include <stddef.h>

uint32_t fletcher32(uint16_t *pdata, size_t nwords){
 uint32_t sum0 = UINT16_MAX, sum1 = UINT16_MAX;
 size_t tlen;
  
 while(nwords != 0){
   tlen = nwords > 359? 359 : nwords;
   nwords -= tlen;
   do{
     sum0 += *pdata++;
     sum1 += sum0;
   }while( --tlen != 0);
 }

 sum0 = ( sum0 & UINT16_MAX ) + (sum0 >> 16);
 sum1 = ( sum1 & UINT16_MAX ) + (sum1 >> 16);

 return (sum1 << 16 | sum0);
}

uint32_t alder32(uint16_t *pdata, size_t nwords){
  #define ALDER32_MOD 65521
  
  uint32_t suma = 1L ,sumb = 0L;

  while(nwords-- != 0){
    suma += *pdata++;
    if(suma >= ALDER32_MOD)
      suma -= ALDER32_MOD;

    sumb += suma;
    if(sumb >= ALDER32_MOD)
      sumb -= ALDER32_MOD;

  }

  return ((sumb << 16 ) | suma);
}
