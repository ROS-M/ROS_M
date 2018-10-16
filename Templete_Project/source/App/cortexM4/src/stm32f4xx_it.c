/**
  ******************************************************************************
  * @file    IO_Toggle/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lwip/sys.h"
#include "stm32f4x7_eth.h"
#include "printf.h"

extern xSemaphoreHandle s_xSemaphore;
void ETH_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if ( ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET)
	{
		xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );
	}

	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
	
	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken);
	}

}



// X4
// Handle PE0 interrupt for ENCODER_L - A 
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
} 

// Handle PE1 interrupt for ENCODER_L - B 
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

// Handle PE3 interrupt for ENCODER_R - A 
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

// Handle PE4 interrupt for ENCODER_R - B 
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{


		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}


/* X2
// Handle PE0 interrupt for ENCODER_L - A 
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		update_encoder_L_value_A(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1));

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
} 

// Handle PE1 interrupt for ENCODER_L - B 
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		update_encoder_L_value_B(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0));

		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

// Handle PE3 interrupt for ENCODER_R - A 
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		update_encoder_R_value_A(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4));

		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

// Handle PE4 interrupt for ENCODER_R - B 
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		update_encoder_R_value_B(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3));

		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
*/

/*
//extern xSemaphoreHandle external_interrupt_sem;
void EXTI3_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

		if (toggle == 1) {
			GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
			toggle = 0;
		}
		else {
			GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);
			toggle = 1;
		}

	// Make sure that interrupt flag is set 
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		xSemaphoreGiveFromISR(external_interrupt_sem, &xHigherPriorityTaskWoken);
	}

	// Clear interrupt flag 
	EXTI_ClearITPendingBit(EXTI_Line3);

	if(xHigherPriorityTaskWoken != pdFALSE)
	{
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}
*/

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetITStatus(EXTI_Line7) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line8) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line11) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13) == SET)
	{

		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetITStatus(EXTI_Line15) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
