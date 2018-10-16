#ifndef BSP_H
#define BSP_H

#define HSE_VALUE	8000000

#include "stm32f4xx.h"
#include "printf.h"
#include "stm32f4x7_eth_bsp.h"


void initBoard(void);
void LED_Configuration(void);
void USART_Configuration(void);


#endif
