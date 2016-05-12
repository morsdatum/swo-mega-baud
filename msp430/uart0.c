#include "uart0.h"

const char table_hexdigit[16]="0123456789ABCDEF";


const uint32_t table_div32[]={
  1000000000,
  100000000,
  10000000,
  1000000,
  100000,
  10000,
  1000,
  100,
  10,
  1};


const uint16_t table_div16[]={
  10000,
  1000,
  100,
  10,
  1};
  


inline uint8_t recv_char_poll(void){
  //while(!(IFG2 & UCA0RXIFG));
  return(UCA0RXBUF);
}


void xmit_char_poll(char ch){
  //while((UCA0STAT & (~UCBUSY)) == UCBUSY) ;
  while(!(IFG2 & UCA0TXIFG)) ;
  UCA0TXBUF = ch;
}

void xmit_strz_poll(char const *strz){
  while(*strz!='\0')
    xmit_char_poll(*strz++);
}


void xmit_hex_poll(uint8_t val){
  xmit_char_poll(table_hexdigit[(val >> 4) & 0xF]);
  xmit_char_poll(table_hexdigit[val & 0xF]);
}

void xmit_hex_poll2(uint16_t val){
  xmit_char_poll(table_hexdigit[(val >> 12) & 0xF]);
  xmit_char_poll(table_hexdigit[(val >> 8) & 0xF]);
  xmit_char_poll(table_hexdigit[(val >> 4) & 0xF]);
  xmit_char_poll(table_hexdigit[val & 0xF]);
}

void xmit_hex_poll4(uint32_t val){
  xmit_hex_poll2((uint16_t)((val >> 16) & 0xFFFF));
  xmit_hex_poll2((uint16_t)(val & 0xFFFF));
}

void xmit_hex_poll8(uint64_t val){
  xmit_hex_poll2((uint16_t)((val >> 48) & 0xFFFF));
  xmit_hex_poll2((uint16_t)((val >> 32) & 0xFFFF));
  xmit_hex_poll2((uint16_t)((val >> 16) & 0xFFFF));
  xmit_hex_poll2((uint16_t)(val & 0xFFFF));
}


void xmit_dec_poll2(uint16_t val){
  uint16_t d;
  uint8_t ch;
  const uint16_t *dp = table_div16;
  if(val==0)
    xmit_char_poll('0');
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

      xmit_char_poll(ch);
    }while(!(d & 1));
  }
}


void xmit_dec_poll4(uint32_t val){
  uint32_t d;
  uint8_t ch;
  const uint32_t *dp=table_div32;
  if(val==0)
    xmit_char_poll('0');
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
      
      xmit_char_poll(ch);      
    }while(!(d & 1));
  }
}
