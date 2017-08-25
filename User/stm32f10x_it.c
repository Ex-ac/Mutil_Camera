/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
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
#include "stm32f10x_it.h"
#include "globalvariable.h"
#include "AbstractSignal.h"
#include "SpiMaster.h"
#include "sdio_sd.h"
#include <stdio.h>
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
	printf("Hard fault error");
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#ifndef RTE_CMSIS_RTOS_RTX
void SVC_Handler(void)
{
}
#endif

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#ifndef RTE_CMSIS_RTOS_RTX
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#ifndef RTE_CMSIS_RTOS_RTX
void SysTick_Handler(void)
{
}
#endif
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
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
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler()
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
//		sysTickDelayUs(100);
//		printf("exti in\n");
//		printf("exti in 5\n");
		if (!pixelBase1.getHasAnswer())
		{
			SpiSignal *p_SpiSignal = new SpiSignal(&pixelBase1, true);
			signalQueue.push(p_SpiSignal);
			
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	else if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
//		printf("exti in 6\n");
		if (!pixelBase2.getHasAnswer())
		{
			SpiSignal *p_SpiSignal = new SpiSignal(&pixelBase2, true);
			signalQueue.push(p_SpiSignal);
			
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if (EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
//		printf("exti in 7\n");
		if (!pixelBase3.getHasAnswer())
		{
			SpiSignal *p_SpiSignal = new SpiSignal(&pixelBase3, true);
			signalQueue.push(p_SpiSignal);
			
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	else if (EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	

}

void DMA1_Channel4_IRQHandler()
{
	if (DMA_GetITStatus(DMA1_IT_TC4) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		DMA_Cmd(DMA1_Channel4, DISABLE);
		DMA_Cmd(DMA1_Channel5, DISABLE);
		dealDMAInterrupt(spiMaster2);
	}
}


void DMA1_Channel5_IRQHandler()
{
	if (DMA_GetITStatus(DMA1_IT_TC5) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		DMA_Cmd(DMA1_Channel5, DISABLE);
	}
}



void DMA1_Channel2_IRQHandler()
{
	if (DMA_GetITStatus(DMA1_IT_TC2) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		DMA_Cmd(DMA1_Channel2, DISABLE);
		DMA_Cmd(DMA1_Channel3, DISABLE);
		dealDMAInterrupt(spiMaster1);
	}
}


void DMA1_Channel3_IRQHandler()
{
	if (DMA_GetITStatus(DMA1_IT_TC3) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		DMA_Cmd(DMA1_Channel5, DISABLE);
		
	}
}

void DMA2_Channel1_IRQHandler()
{
	if (DMA_GetITStatus(DMA2_IT_TC1) == SET)
	{
		DMA_ClearITPendingBit(DMA2_IT_TC1);
		DMA_Cmd(DMA2_Channel1, DISABLE);
		DMA_Cmd(DMA2_Channel2, DISABLE);
//		dealDMAInterrupt();
	}
}


void DMA2_Channel2_IRQHandler()
{
	if (DMA_GetITStatus(DMA2_IT_TC2) == SET)
	{
		DMA_ClearITPendingBit(DMA2_IT_TC2);
		DMA_Cmd(DMA2_Channel2, DISABLE);
		
	}
}

void SDIO_IRQHandler()
{ 
	/* 处理所有的SDIO中断源 */
	SD_ProcessIRQSrc();  
}


void dealDMAInterrupt(SpiMaster &r_spiMaster)
{
	PixelBase *p_PixelBase = dynamic_cast<PixelBase *>(r_spiMaster.getRuningSliver());
		
		if (p_PixelBase != nullptr)
		{
			p_PixelBase->getLast2ByteCommand();
			PackBuff *p_PackBuff = p_PixelBase->p_PackBuff;
			if (MasterProtocols::packCheck(p_PixelBase))
			{
				if (p_PackBuff != nullptr)
				{
					p_PixelBase->sendToPCLong();
					p_PixelBase->_packBuffQueue.push(p_PackBuff);
					// 存储请求
					SDSignal *sdSignal = new SDSignal(p_PixelBase);
					inseratIntoSignalQueue(sdSignal);
					//判断是否结束
					u16 sizeOfPack = p_PackBuff->getNumberOfPack();
					if ( p_PackBuff->getNumberOfPack() < 
						p_PixelBase->_picturePackInfo.sizeOfPack)
					{
						SpiSignal *p_SignalTemp = new SpiSignal(p_PixelBase, false);
						u8 data[RequestCommandBuffSize] = {0x00};
						MasterProtocols::getPicturePackRequest(p_PixelBase, &data, 
							sizeOfPack + 1, &(p_PixelBase->_picturePackInfo));
						p_SignalTemp->setData(data);
						
						inseratIntoSignalQueue(p_SignalTemp);
					}
					else
					{
//						printf("Finished\n");
					}
				}
			}
			else
			{	
//				printf("Data Pack Check Error\n");
				
				delete p_PackBuff;
				SpiSignal *p_SignalTemp = new SpiSignal(p_PixelBase, false);
				p_SignalTemp->setData((p_PixelBase->sendDataBuff));
				inseratIntoSignalQueue(p_SignalTemp);
			}
			p_PixelBase->unselectThisSliver();
		}
		
}


void USART1_IRQHandler(void)
{

	static bool isStartFlag = false;
	static u8 i;
	u8 temp;
	USART_ClearFlag(USART1,USART_FLAG_TC);
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)//检查指定的USART中断发生与否
	{
		temp = USART_ReceiveData(USART1);
		if (!isStartFlag)
		{
			if (temp == 'c' || temp == 's')
			{
				i = 0;
				uartBuff[0] = temp; 
				isStartFlag = true;
				
			}
		}
		else
		{
			i ++;
			uartBuff[i] = temp;
			if (i == uartBuff[1] + 2)
			{
				if (uartBuff[0] == uartBuff[i])
				{
					hasPCRequest = true;
				}
				isStartFlag = false;
			}
		}
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);	
	}
	
	
}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
