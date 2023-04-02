/**
******************************************************************************
* @file    callbacks.c
* @author  SRA
* @date    30-05-2021
* @brief   Callback Functions
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under SLA0094 license,
* the "License"; You may not use this file except in compliance with the
* License. You may obtain a copy of the License at:
*                        https://www.st.com/SLA0094
*
******************************************************************************
*/

/* Includes   ----------------------------------------------------------------*/
#include <callbacks.h>
#include <stdbool.h>
#include "bridge.h"
#include "main.h"

/* Exported Variables  -------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;

/* Exported Functions  -------------------------------------------------------*/

/* Global Variables  -------------------------------------------------------*/
int32_t TestStatus;
/**
* @brief  UART RX Complete callback
* @param  huart pointer to the obj
* @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2)
  {
    interface_mode = UART_MODE;
    command_triggered=true;
    
    /*Stop the UART timeout */
    TestStatus = (int32_t)HAL_TIM_Base_Stop_IT(&htim3);
  }
}


/**
* @brief  GPIO EXTI callback
* @param  GPIO_Pin
* @retval None
*/


/**
* @brief  Timer callback
* @param  htim Poinyer to Timer obj
* @retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  uint32_t timer;
  timer = (uint32_t)(htim->Instance);
  
  switch(timer)
  {
  case (uint32_t)TIM2:
    /* timeout used for LED1202 IRQ timeout */
    tim2_timeout=true;
    break;
    
  case (uint32_t)TIM3:
    command_triggered=false;
    bridge_OUT_buffer[0]=NO_CMD;
    /* timeout occurred, RX buffer is reset */
    TestStatus = (int32_t)HAL_TIM_Base_Stop_IT(&htim3);
    TestStatus = (int32_t)HAL_UART_AbortReceive(&huart2);
    TestStatus = (int32_t)HAL_UART_Receive_IT(&huart2,&bridge_OUT_buffer[0],64);
    break;
    
  default:
    break;
    
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
