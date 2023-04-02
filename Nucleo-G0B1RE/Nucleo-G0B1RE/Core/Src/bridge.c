/**
******************************************************************************
* @file    bridge.c 
* @author  SRA
* @version v1.0.0
* @date    30-05-2021
* @brief   Bridge for GUI connectivity.
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
#include "bridge.h"
#include "main.h"
#include <stdbool.h>
#include "stm32g0xx_hal.h"
#include "stm32g0xx.h"

/* Private Variable   --------------------------------------------------------*/
uint8_t bridge_IN_buffer[BUFFER_SIZE];
uint8_t bridge_OUT_buffer[BUFFER_SIZE];
uint8_t RAM_buffer[RAM_BUFFER_SIZE];
InterfaceModeTypedef interface_mode;
volatile uint8_t tim2_timeout=false;
volatile uint8_t LED1202_IRQ_triggered=false;
volatile uint8_t PushButton_IRQ_triggered=false;
volatile uint8_t command_triggered=false;
generic_request *request=(generic_request*)&bridge_OUT_buffer[0];
generic_response *response = (generic_response*)&bridge_IN_buffer[0];
id_response *bridge_id_response = (id_response*)&bridge_IN_buffer[0];
process_item *i2c_request_item;
process_item *i2c_response_item;
int32_t Tstatus;

/* Exported Variable   -------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

/* Exported  Constants   -----------------------------------------------------*/
#define SCRIPTS_AREA_SIZE 24

/* Private Functions     -----------------------------------------------------*/

/**
 * @brief  Clear the buffers
 * @param  None
 * @retval None
 */
void ClearBuffers(void)
{
  uint8_t count;
  for(count=0;count<BUFFER_SIZE;count++)
  {
    bridge_IN_buffer[count]=0x0;
    bridge_OUT_buffer[count]=0x0;
  }
}


/**
 * @brief  Wait for any command to be received
 * @param  None
 * @retval None
 */
void WaitForCommand(void)
{
  uint32_t count;
  uint8_t process_count;
  volatile static bool comm_inhibit = true;
  
  do
  {}
  while(((bool)command_triggered)==false);
  comm_inhibit=false;
  switch(request->command)
  {
    
  case PROCESS_I2C:
    request->command=NO_CMD;	/*clear the request */
    command_triggered=false;
    response->command=PROCESS_I2C;
    process_count = (uint8_t)request->packets;
    response->ack = (uint8_t)HAL_OK;
    i2c_request_item=(process_item*)&(request->starting_point);
    i2c_response_item=(process_item*)&(response->buffer[0]);
    while(process_count>((uint8_t)0x00))
    {
      i2c_response_item->s_addr=i2c_request_item->s_addr;
      if((i2c_request_item->s_addr)&((uint8_t) 0x1))		/*Read process*/
      {
        if((i2c_request_item->mode) &((uint8_t) IRQ_TRIGGER))
        {
          LED1202_IRQ_triggered=false;
          tim2_timeout=false;
          __HAL_TIM_SET_COUNTER(&htim2,0);
          Tstatus =(int32_t) HAL_TIM_Base_Start_IT(&htim2);
          while((bool)LED1202_IRQ_triggered==false)
          {
            if((bool)tim2_timeout==true)
            {
              break;
            }
            else
            {
              /**Do nothing**/
            }
          }
        }
        i2c_response_item->size=i2c_request_item->size;
        response->ack |= (uint8_t)HAL_I2C_Master_Transmit(&hi2c1,i2c_request_item->s_addr, &(i2c_request_item->buffer[0]),1,100);
        response->ack |= (uint8_t)HAL_I2C_Master_Receive(&hi2c1,i2c_request_item->s_addr, &(i2c_response_item->buffer[0]), i2c_request_item->size,100);
        i2c_response_item->i2c_status =(uint8_t) HAL_I2C_GetError(&hi2c1);							
      }
      else		/*Write process*/
      {
        if(i2c_request_item->mode & ((uint8_t)IRQ_TRIGGER))
        {
          LED1202_IRQ_triggered=false;
          tim2_timeout=false;
          __HAL_TIM_SET_COUNTER(&htim2,0);
          Tstatus =(int32_t) HAL_TIM_Base_Start_IT(&htim2);
          while((bool)LED1202_IRQ_triggered==false)
          {
            if((bool)tim2_timeout==true)
            {
              break;
            }
          }
        }
        i2c_response_item->size=0;
        response->ack |= (uint8_t)HAL_I2C_Master_Transmit(&hi2c1,i2c_request_item->s_addr, &(i2c_request_item->buffer[0]),i2c_request_item->size,100);
        i2c_response_item->i2c_status = (uint8_t)HAL_I2C_GetError(&hi2c1);
      }
      process_count--;
      uint8_t *pointer = ((uint8_t*)&(i2c_request_item->buffer[0])+i2c_request_item->size);
      i2c_request_item = (process_item*)(pointer);//(process_item*)((uint8_t*)&(i2c_request_item->buffer[0])+i2c_request_item->size);
      i2c_response_item=(process_item*)((uint8_t*)&(i2c_request_item->buffer[0])+i2c_response_item->size);
    }
    break;
  case RESET_I2C:
    request->command=NO_CMD;	/*clear the request*/
    command_triggered=false;
    response->command=RESET_I2C;
    response->ack=(uint8_t)HAL_OK;
    hi2c1.Instance->CR1 &=~I2C_CR1_PE;
    hi2c1.Instance->CR1 |=I2C_CR1_PE;
    break;
  case SET_I2C_FREQUENCY:
    request->command=NO_CMD;	/*clear the request*/
    command_triggered=false;
    response->command=SET_I2C_FREQUENCY;
    if(request->frequency!=((uint8_t)0x0))
    {
      hi2c1.Instance->CR1 &=~I2C_CR1_PE;
      hi2c1.Instance->CR1 |=I2C_CR1_PE;
    }

    response->ack=(uint8_t)HAL_OK;
    break;
  case GET_MODEL:
    request->command=NO_CMD;	/*clear the request*/
    command_triggered=false;
    bridge_id_response->command=GET_MODEL;
    for(count=0;count<sizeof(MODEL);count++)
    {
      bridge_id_response->model[count]=(char)(MODEL[count]);
    }
    bridge_id_response->fw_rev=MAJOR_FW_VERSION;
    bridge_id_response->fw_rev= (bridge_id_response->fw_rev << 8) | MINOR_FW_VERSION;
    bridge_id_response->scripts_area_size_x1k=SCRIPTS_AREA_SIZE;
    bridge_id_response->ack=(uint8_t)HAL_OK;
    break;
  case GET_BRIDGE_STATUS:
    request->command=NO_CMD;	/*clear the request*/
    command_triggered=(uint8_t)false;
    response->command=GET_BRIDGE_STATUS;
    response->buffer[0]=(uint8_t)(LED1202_IRQ_triggered);
    response->buffer[1] = (uint8_t) HAL_I2C_GetError(&hi2c1);
    response->ack=(uint8_t)HAL_OK;
    break;
  case WRITE_RAM:
    request->command=NO_CMD;	/*clear the request*/
    command_triggered=false;
    if((request->ram_pos + request->ram_size) >((uint16_t)1024))
    {
      response->ack = (uint8_t)HAL_ERROR;
      break;
    }
    for(count=0;count<request->ram_size;count++)
    {
      RAM_buffer[request->ram_pos+count]=request->ram_data[count];
    }
    response->ack=(uint8_t)HAL_OK;
    break;
  default:
    request->command=NO_CMD;	/*clear the request*/
    comm_inhibit=false;
    command_triggered=false;
    response->command=request->command;
    response->ack=(uint8_t)HAL_ERROR;
    break;
  }
  request=(generic_request*)(&bridge_OUT_buffer[0]);	/*force bridge to point to buffer_out, necessary when exiting from ram execution*/
  request->command=NO_CMD;	/*clear the request*/
  command_triggered=false;
  if(interface_mode == UART_MODE)
  {
    /*prepare UART IT transfer*/
    if(comm_inhibit==false)
    {
    	Tstatus = (int32_t) HAL_UART_Transmit_IT(&huart2,&bridge_IN_buffer[0],64);
    }
    comm_inhibit=false;
    /*Reload Rx Interrupt transfer */
    Tstatus =  (int32_t) HAL_UART_Receive_IT(&huart2,&bridge_OUT_buffer[0],64);
  }
  else
  {
    comm_inhibit=false;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
