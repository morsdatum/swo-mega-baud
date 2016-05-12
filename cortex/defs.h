#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>


#define UARTRX	USART1
#define UARTXD USART3 // should not be same as UARTRX

#define LEDPORT GPIOD
#define LEDPINS1 (GPIO12 | GPIO13)
#define LEDPINS2 (GPIO14 | GPIO15)

#define USRBTNPORT GPIOA
#define USRBTNPIN  GPIO0

void EXTI0_IRQHandler(void);

#if (UARTXD == USART2)
void USART2_IRQHandler(void);

#define TXPORT GPIOA
#define TXPIN GPIO2
#define TXF GPIO_AF7
#define TXIRQ_NUM NVIC_USART2_IRQ
#define TXD_RCC RCC_USART2

#elif (UARTXD == USART3)
void USART3_IRQHandler(void);

#define TXPORT GPIOD
#define TXPIN GPIO8
#define TXAF GPIO_AF7
#define TX_IRQ_NUM NVIC_USART3_IRQ
#define TXD_RCC RCC_USART3

#elif (UARTXD == UART4)
void UART4_IRQHandler(void);

#define TXPORT GPIOA
#define TXPIN GPIO0
#define TXAF GPIO_AF8
#define TX_IRQ_NUM NVIC_URAT4_IRQ
#define TXD_RCC RCC_UART4

#elif (UARTXD == UART6)
void UART6_IRQHandler(void);

#define TXPORT GPIOC
#define TXPIN GPIO6
#define TXAF GOIO_AF8
#define TX_IRQ_NUM NVIC_UART6_IRQ
#define TXD_RCC RCC_UART6

#endif // UARTXD

#if(UARTRX == USART1)
void USART1_IRQHandler(void);

#define RXPORT GPIOB
#define RXPIN GPIO7
#define RXAF GPIO_AF7
#define RX_IRQ_NUM NVIC_USART1_IRQ
#define RXD_RCC RCC_USART1

#elif(UARTRX == USART2)
void USART2_IRQHandler(void);
  
#define RXPORT GPIOA
#define RXPIN GPIO3
#define RXAF GPIO_AF7
#define RX_IRQ_NUM NVIC_USART2_IRQ
#define RXD_RCC RCC_USART2

#elif(UARTX ==  USART3)
void USART3_IRQHandler(void);

#define RXPORT GPIOB
#define RXPIN GPIO11
#define RXAF GPPIO_AF7
#define RX_IRQ_NUM NVIC_USART3_IRQ
#define RXD_RCC RCC_USART3

#elif(UARTX == UART4)
void UART4_IRQHandler(void);

#define RXPORT GPIOA
#define RXPIN GPIO1
#define RXAF GPIO_AF8
#define RX_IRQ_NUM NVIC_UART4_IRQ  
#define RXD_RCC RCC_UART4

#elif(UARTX == UART5)
void UART5_IRQHandler(void);

#define RXPORT GPIOD
#define RXPIN GPIO2
#define RXAF GPIO_AF8
#define RX_IRQ_NUM NVIC_UART5_IRQ  
#define RXD_RCC RCC_UART5


#endif // UARTRX

#define SYSCLK 168000000
#define APB1CLK (SYSCLK/4)
#define BASETIM_HERTZ 18
#define BASETIM_DEBOUNCE 12
#define BASETIM_PRESCALR ((APB1CLK*2)/(84*25))
#define BASETIM_CNT (BASETIM_PRESCALR/BASETIM_HERTZ)

#if ( BASETIME_CNT > 65535 )
void TIM2_IRQHandler(void);

#define BASETIMX TIM2
#define BASETIMX_RCC RCC_TIM2
#define BASETIMX_IRQ_NUM NVIC_TIM2_IRQ
#else
void TIM3_IRQHandler(void);

#define BASETIMX TIM3
#define BASETIMX_RCC RCC_TIM3
#define BASETIMX_IRQ_NUM NVIC_TIM3_IRQ
#endif

void hard_fault_handler(void);
void mem_manage_handler(void);
void bus_fault_handler(void);
void usage_fault_handler(void);

#endif // _DEFINES_H_