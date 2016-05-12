#include <msp430.h>
#include <stdint.h>
#include "uart0.h"
#include "lsfr.h"
#include "cunitest.h"

#include "test_lsfr16.inc"
#include "test_lsfr32.inc"
#include "test_cksum.inc"

#define P1_BTN BIT3
#define P1_LED1 BIT0
#define P1_LED2 BIT6

#define SYSCLK 8000000
//#define BAUDRATE 9600
#define BAUDRATE 1000000
#define BAUDRATE_0 ((SYSCLK/BAUDRATE) & 0xFF) 
#define BAUDRATE_1 ((SYSCLK/BAUDRATE) >> 8)

#define TMRA_VAL ((32768/64)-1)

static uint16_t tx_cnt = 0;
static uint8_t tx_char = 0;

volatile uint8_t btn_cnt = 0;

tests_fcnptr_t runner[]={
#if(TESTCASE==1) || defined(TESTALL)
  UNITEST_FCT(lsfr16_init,1),
#endif
  
#if(TESTCASE==3) || defined(TESTALL)
  UNITEST_FCT(fibonacci_lsfr16,3),
#endif
  
#if(TESTCASE==4) || defined(TESTALL)
  UNITEST_FCT(galois_lsfr16,4),
#endif

#if(TESTCASE==10) || defined(TESTALL)
  UNITEST_FCT(lsfr32_init,10),
#endif

#if(TESTCASE==20) || defined(TESTALL)
  UNITEST_FCT(fletcher32,20),
#endif

#if(TESTCASE==21) || defined(TESTALL)
  UNITEST_FCT(fletcher32,21),
#endif

#if(TESTCASE==22) || defined(TESTALL)
  UNITEST_FCT(alder32,22),
#endif

  (tests_fcnptr_t)(0)};
  

inline void setup_watchdog(void){
  WDTCTL = WDTPW | WDTHOLD;
}

inline void setup_clk_src(void){
  DCOCTL = 0;
#if (SYSCLK == 8000000)
  BCSCTL1 = CALBC1_8MHZ | DIVA_0; // ACLK -> 32768
#elif (SYSCLK == 1000000)
  BCSCTL1 = CALBC1_1MHZ | DIVA_0;
#elif (SYSCLK == 16000000)
  BCSCTL1 = CALBC1_16MHZ | DIVA_0;
#endif
  BCSCTL2 = DIVS_0 ; // -> SMCLK -> 8M
  BCSCTL3 = LFXT1S_0 | XCAP_3;
#if (SYSCLK == 8000000)
  DCOCTL = CALDCO_8MHZ;
#elif (SYSCLK == 1000000)
  DCOCTL = CALDCO_1MHZ;
#elif (SYSCLK == 16000000)
  DCOCTL = CALDCO_16MHZ;
#endif
}
 
inline void setup_port(void){
  UCA0CTL1 = UCSWRST; // stop UCA0 1st

  P1DIR = 0xFF & ~P1_BTN; // all is output except P2.6 for XTAL32
  P2DIR = 0xFF & ~BIT6;

  P1SEL |= (P1_RXD | P1_TXD);
  P1SEL &= ~P1_BTN;
  P1SEL2 |= (P1_RXD | P1_TXD);
  P1SEL2 &= ~P1_BTN;

  P1OUT |= P1_LED1 | P1_LED2;
  P1REN |= P1_BTN; // enable pull-up on btn pin

  P2SEL = BIT6 | BIT7;
  P2SEL2 &= ~(BIT6 | BIT7);
}

inline void setup_uart(void){
  UCA0CTL1 |= UCSSEL_2;
  UCA0CTL0 = UCMODE_0;
  UCA0BR0 = BAUDRATE_0;
  UCA0BR1 = BAUDRATE_1;
  UCA0MCTL = UCBRS_0;
  UCA0CTL1 &= ~UCSWRST;
}

inline void setup_timera(void){
  TA1CTL = TACLR;
  TA1CCR0 = TMRA_VAL;
  TA1CTL = TASSEL_1;
  // TA1CTL |= (MC_1 | TAIE); // to start timer irq
}

__attribute__((interrupt(PORT1_VECTOR)))
void port1_isr(void){  
  P1OUT ^= P1_LED1;
#if !defined(TEXTCASE) && !defined(TESTALL)
  if((P1IFG & P1_BTN) == P1_BTN){
    P1IFG &= ~P1_BTN; // clear interrupt flag
   #if 0
   tx_cnt = fibonacci_lsfr16() & 0xFF;
    if(tx_cnt > 26)
      tx_cnt = 26;

    tx_char = (galois_lsfr16() >> 8 ) & 0x7F;
    if(tx_char > 93)
      tx_char = 92;
      tx_char += 0x21;
    xmit_char_poll('x');
    IE2 |= UCA0TXIE;
    #endif
    P1IE &= ~P1_BTN;
    btn_cnt = 1;
    TA1CTL &= ~TAIFG;
    TA1CTL |= (MC_1 | TAIE);
  }
#endif
}

__attribute__((interrupt(USCIAB0TX_VECTOR)))
void uart_tx_isr(void){
   P1OUT ^= P1_LED2;
#if !defined(TESTCASE) && !defined(TESTALL)
   #if 0
   if(tx_cnt-- == 0){
     xmit_char_poll('\n');
     IE2 &= ~UCA0TXIE;
   }
   else{
     xmit_char_poll(tx_char++);
     IE2 |= UCA0TXIE;
     }
   #endif
#endif   
}

__attribute__((interrupt(TIMER1_A1_VECTOR)))
void timer1_isr(void){
  TA1CTL &= ~TAIFG;  // clear timer a interrupt flag
  if(btn_cnt > 6){
    xmit_char_poll('x');
    TA1CTL &= ~(MC0 | MC1 | TAIE); // stop timer a
    TA1R = 0;
    
    P1IE |= P1_BTN; // enable failing edge detect
    P1IES |= P1_BTN;
  }else if((P1IN & P1_BTN) == 0)
    btn_cnt++;
  else {
    btn_cnt = 1;
    P1IE |= P1_BTN;
    P1IES |= P1_BTN;
  }
}

int main(void){

  __disable_interrupt();
  __no_operation();

  setup_watchdog();
  setup_clk_src();
  setup_port();
  setup_uart();
  setup_timera();

  xmit_char_poll('S');

#if defined(TESTCASE) || defined(TESTALL)
  lsfr16_init(0);
  test_runner(&runner[0]);
#endif  

  P1IE |= P1_BTN;
  P1IES |= P1_BTN;
  while(1){

    __enable_interrupt();
    __no_operation();
    __low_power_mode_0();
    __no_operation();
  }
  
  return 0;
}
