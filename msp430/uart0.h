#ifndef __UART0_H_
#define __UART0_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>


#define P1_RXD  BIT1
#define P1_TXD  BIT2


// trace message output to uart for debugging

uint8_t recv_char_poll(void);

void xmit_char_poll(char ch);
void xmit_strz_poll(char const *strz);

void xmit_hex_poll(uint8_t val);
void xmit_hex_poll2(uint16_t val);
void xmit_hex_poll4(uint32_t val);
void xmit_hex_poll8(uint64_t val);

void xmit_dec_poll2(uint16_t val);
void xmit_dec_poll4(uint32_t val);

#endif
