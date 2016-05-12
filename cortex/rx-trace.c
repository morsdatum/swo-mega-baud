#include <libopencmsis/core_cm3.h>
#include <libopencm3/stm32/memorymap.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/cortex.h>
#include "trace_printf.h"
#include "cunitest.h"
#include "swo.h"
#include "defs.h"

#include "test_crc32.inc"
static const tests_fcnptr_t runner[]={
#if (TESTCASE==1) || defined(TESTALL)
  UNITEST_FCT(Crc32_init,1),
#endif
  
#if (TESTCASE==2) || defined(TESTALL)
  UNITEST_FCT(Update_crc32_u8,2),
#endif

#if (TESTCASE==3) || defined(TESTALL)
  UNITEST_FCT(Update_crc32_u8ptr,3),
#endif
  
  (tests_fcnptr_t)(0)
};

static volatile uint16_t debounce_cnt;

/*
 * interrupt routines must stay with main function
 */ 

#if (UARTXD == USART2)
void USART2_IRQHandler(void)

#elif (UARTXD == USART3)
void USART3_IRQHandler(void)

#elif (UARTXD == UART4)
void UART4_IRQHandler(void)

#elif (UARTXD == UART6)
void UART6_IRQHandler(void)
#endif
{
  static char xmit_data = '0';

  if(usart_get_flag(UARTXD, USART_SR_TXE)==true){
    //trace_printf("\n\rIn fct %s()",__FUNCTION__);

    if(xmit_data < '9')
     usart_send(UARTXD, (uint16_t)(xmit_data++)); // send ten numbers
    else{
     xmit_data = '0';
     usart_disable_tx_interrupt(UARTXD);
     usart_disable(UARTXD);
     nvic_disable_irq(TX_IRQ_NUM);
    }
  }
  nvic_clear_pending_irq(TX_IRQ_NUM);
}



#if(UARTRX == USART1)

void USART1_IRQHandler(void)
#elif(UARTRX == USART2)
    
void USART2_IRQHandler(void)
#elif(UARTX ==  USART3)

void USART3_IRQHandler(void)
#elif(UARTX == UART4)

void UART4_IRQHandler(void)
#elif(UARTX == UART5)

void UART5_IRQHandler(void)
#endif
{
  static char recv_data;

  if(usart_get_flag(UARTRX, USART_SR_RXNE) == true){
    //trace_printf("\n\rIn fct %s() ",__FUNCTION__);

    recv_data = (char)usart_recv(UARTRX);
    trace_putchar(recv_data); // relay uart received data to swo
  }

  nvic_clear_pending_irq(RX_IRQ_NUM);
}


void EXTI0_IRQHandler(void){

 // exti0 using as trigger for debouncing

  if(exti_get_flag_status(EXTI0) == EXTI0){
    //trace_printf("\n\rIn fct %s()",__FUNCTION__);

    exti_reset_request(EXTI0); // call to cancel served irq 

    debounce_cnt = 0; // start debouncing

    nvic_set_priority(BASETIMX_IRQ_NUM, 1);    
    nvic_enable_irq(BASETIMX_IRQ_NUM);

    timer_set_period(BASETIMX, BASETIM_CNT);
    timer_enable_counter(BASETIMX);
    timer_enable_irq(BASETIMX, TIM_DIER_UIE);

    /*usart_enable_tx_interrupt(UARTXD);
    usart_enable(UARTXD);
    nvic_enable_irq(TX_IRQ_NUM);
    usart_send(UARTXD, '\n');*/
  }

  nvic_clear_pending_irq(NVIC_EXTI0_IRQ);
}

#if(BASETIMX == TIM2)
void TIM2_IRQHandler(void)
#elif(BASETIMX == TIM3)
void TIM3_IRQHandler(void)
#endif
{  

  if(gpio_get(USRBTNPORT, USRBTNPIN) == USRBTNPIN){
    if( ++debounce_cnt >  BASETIM_DEBOUNCE){
      //trace_printf("\n\rIn fct %s()",__FUNCTION__); 

      timer_disable_irq(BASETIMX, TIM_DIER_UIE);
      timer_disable_counter(BASETIMX); // already debounced

      usart_enable_tx_interrupt(UARTXD); // execute USART xmit
      usart_enable(UARTXD);
      nvic_enable_irq(TX_IRQ_NUM);
      usart_send(UARTXD, '\n');

    }
  }else
     debounce_cnt = 0; // restart at low level during debouncing period
  
  nvic_clear_pending_irq(BASETIMX_IRQ_NUM);
}


void hard_fault_handler(void){

  trace_printf("In handler %s()",__FUNCTION__);
  while(1);

}

void mem_manage_handler(void){
  trace_printf("In handler %s()",__FUNCTION__);
  while(1);
}

void bus_fault_handler(void){

 trace_printf("In handler %s()",__FUNCTION__);
 while(1);

}

void usage_fault_handler(void){

  trace_printf("In handler %s()",__FUNCTION__);
  while(1);

}

inline void clk_setup(void){
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOD);
  rcc_periph_clock_enable(RXD_RCC);
  rcc_periph_clock_enable(TXD_RCC);
  rcc_periph_clock_enable(RCC_SYSCFG); // for exti
  rcc_periph_clock_enable(BASETIMX_RCC); // for timer 2/3

  trace_init();
  //trace_puts("Clk config'ed");
}

inline void port_setup(void){

  gpio_mode_setup(LEDPORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LEDPINS1 | LEDPINS2);
  gpio_clear(LEDPORT, LEDPINS1 | LEDPINS2);

  gpio_mode_setup(USRBTNPORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, USRBTNPIN);

  gpio_mode_setup(RXPORT, GPIO_MODE_AF, GPIO_PUPD_NONE, RXPIN);
  gpio_set_af(RXPORT, RXAF, RXPIN);

  gpio_mode_setup(TXPORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TXPIN);
  gpio_set_af(TXPORT, TXAF, TXPIN);


  //trace_puts("Ports config'ed");  
}

inline void uart_setup(void){
  usart_disable(UARTRX);

  usart_set_baudrate(UARTRX, 1000000);
  usart_set_databits(UARTRX, 8);
  usart_set_stopbits(UARTRX, USART_STOPBITS_1);
  usart_set_mode(UARTRX, USART_MODE_RX);
  usart_set_parity(UARTRX, USART_PARITY_NONE);
  usart_set_flow_control(UARTRX, USART_FLOWCONTROL_NONE);

  usart_disable(UARTXD);
  usart_set_baudrate(UARTXD, 1000000);
  usart_set_databits(UARTXD, 8);
  usart_set_stopbits(UARTXD, USART_STOPBITS_1);
  usart_set_mode(UARTXD, USART_MODE_TX);
  usart_set_parity(UARTXD, USART_PARITY_NONE);
  usart_set_flow_control(UARTXD, USART_FLOWCONTROL_NONE);

  //trace_puts("Both uart config'ed");  
}

inline void exti_setup(void){
  exti_select_source(EXTI0, GPIOA);
  exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
  exti_enable_request(EXTI0);

  //trace_puts("Ext. intr. config'ed");
}

inline void timer_setup(void){  

  timer_reset(BASETIMX);

  timer_set_mode(BASETIMX, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_DOWN);
  timer_set_period(BASETIMX, BASETIM_CNT);
  timer_continuous_mode(BASETIMX);
  
  timer_disable_oc_output(BASETIMX, TIM_OC1);
  timer_disable_oc_output(BASETIMX, TIM_OC2);
  timer_disable_oc_output(BASETIMX, TIM_OC3);
  timer_disable_oc_output(BASETIMX, TIM_OC4);

  timer_enable_preload(BASETIMX); 

  //timer_enable_counter(BASETIMX);
  //timer_enable_irq(BASETIMX, TIM_DIER_UIE);

}

inline void nvic_setup(void){
  nvic_set_priority(RX_IRQ_NUM, 0);
  nvic_enable_irq(RX_IRQ_NUM);
  //nvic_enable_irq(TX_IRQ_NUM);

  usart_enable_rx_interrupt(UARTRX);
  usart_enable(UARTRX);

  //usart_enable_tx_interrupt(UARTXD);
  //usart_enable(UARTXD);

  nvic_clear_pending_irq(NVIC_EXTI0_IRQ);
  nvic_set_priority(NVIC_EXTI0_IRQ,0);
  nvic_enable_irq(NVIC_EXTI0_IRQ);

  //nvic_set_priority(BASETIMX_IRQ_NUM, 0);
  //nvic_clear_pending_irq(BASETIMX_IRQ_NUM);
  //nvic_enable_irq(BASETIMX_IRQ_NUM);

  //trace_puts("nvic config'ed");
}

int main(void){


  clk_setup();
 

  port_setup();
  uart_setup();
  exti_setup();
  timer_setup();
  nvic_setup();


#if defined(TESTCASE) || defined(TESTALL)
  trace_init();
  test_runner(&runner[0]);
#endif
  DBGMCU_CR |= (DBGMCU_CR_SLEEP | DBGMCU_CR_STANDBY); // enable debug for sleep mode etc.
  cm_enable_interrupts();

  for(;;){

    __WFI();
  }

  return 0;
}
