/**
 ******************************************************************************
 * @file    bridge.c
 * @author  SRA 
 * @date    30-05-2021
 * @brief   Header file for bridge.c
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

#ifndef __BRIDGE__
#define __BRIDGE__

/* Includes   ----------------------------------------------------------------*/
#include "stm32g0xx.h"
#include <stdbool.h>

/* Exported Constants  -------------------------------------------------------*/
#ifdef __CC_ARM
#pragma anon_unions
#endif

#define BUFFER_SIZE ((uint8_t)64)
#define RAM_BUFFER_SIZE 1024
#define TREG_400KHZ 0x00300F38
#define TREG_100KHZ 0x003038FF
#define TREG_200KHZ 0x00300F88
#define TREG_300KHZ 0x00300F52

#define NO_CMD (uint8_t) 0x0
#define PROCESS_I2C (uint8_t) 'P'
#define RESET_I2C (uint8_t) 'R'
#define SET_I2C_FREQUENCY (uint8_t) 'F'
#define GET_MODEL (uint8_t) 'M'
#define GET_BRIDGE_STATUS (uint8_t) 'S'
#define WRITE_RAM (uint8_t) 'W'
#define EXECUTE_SCRIPT (uint8_t) 'X'
#define DOWNLOAD_SCRIPTS (uint8_t) 'D'
#define UPLOAD_SCRIPTS (uint8_t) 'U'
#define EXECUTE_SEQUENCE (uint8_t) 'Q'
#define GYRO_ENABLE (uint8_t) 'Y'
#define GYRO_DISABLE (uint8_t) 'y'
#define GYRO_GET_DPS (uint8_t) 'T'
#define GYRO_GET_TEMP (uint8_t) 'E'

#define ACK_SUCCESS (uint8_t) 0x0
#define ACK_BUSY (uint8_t) 0x1
#define ACK_ERROR (uint8_t) 0x2
#define ACK_IDLE (uint8_t) 0x3

#define MODEL "LED1202"
#define MAJOR_FW_VERSION (uint16_t)1
#define MINOR_FW_VERSION (uint16_t)4

/*I2c defines */
#define I2C_GetAddress(value) (value & 0xfe)
#define I2C_ClearErrors(i2c_periph) (i2c_periph->SR1 &= 0x00ff)
#define I2C_IsBusy(i2c_periph) (i2c_periph->SR2 & 0x0002)
#define I2C_10BITMASK 0xf8

/*I2C acknowledges */
#define I2C_OK 0x0
#define I2C_BUSY 0x1
#define I2C_ARBITRATION_LOST 0x2
#define I2C_BUS_ERROR 0x4
#define I2C_TIMEOUT 0x10
#define I2C_PEC_ERROR 0x20
#define I2C_ACK_FAILURE 0x40
#define I2C_ADDR_ERROR 0x80

#define RESTART_MODE 0x1
#define IRQ_TRIGGER 0x2

/* Exported Types   ----------------------------------------------------------*/
typedef enum { UART_MODE, VIRTUAL_COM_MODE } InterfaceModeTypedef;
typedef enum { BRIDGE_OK, BRIDGE_ERROR, BRIDGE_BUSY } BridgeStateTypedef;

typedef struct {
	uint8_t s_addr;
	union {
		uint8_t mode;
		uint8_t i2c_status;
	};
	uint8_t size;
	uint8_t buffer[1];	/* This struct has to be used by pointer only */
} process_item;

typedef struct {
	uint8_t command;
	uint8_t ack;
	char model[16];
	uint16_t fw_rev;
	uint8_t scripts_area_size_x1k;
} id_response;

typedef struct {
	uint32_t flash_addr;
	uint8_t buffer[32];
} flash_prog_request;

typedef struct {
	uint8_t command;
	uint8_t ack;
	union {
		uint32_t frequency;
		uint8_t buffer[62];
	} __attribute__ ((packed));
} generic_response;

typedef struct {
	uint8_t command;
	union {
		uint32_t frequency;
		struct {
			uint16_t chunk;
			uint8_t word[32];
			}__attribute__ ((packed));
		struct {
			uint16_t ram_pos;
			uint8_t ram_size;
			uint8_t ram_data[1]; /* This struct has to be used by pointer only */
			}__attribute__ ((packed));
		struct {
			uint8_t packets;
			uint8_t starting_point;
			}__attribute__ ((packed));
		} __attribute__ ((packed));
} generic_request;

/* Exported Variables   ------------------------------------------------------*/
extern InterfaceModeTypedef interface_mode;
extern volatile uint8_t LED1202_IRQ_triggered;
extern volatile uint8_t PushButton_IRQ_triggered;
extern uint8_t bridge_IN_buffer[BUFFER_SIZE];
extern uint8_t bridge_OUT_buffer[BUFFER_SIZE];
extern uint8_t RAM_buffer[RAM_BUFFER_SIZE];
extern volatile uint8_t tim2_timeout;
extern volatile uint8_t command_triggered;
extern generic_request *request;
extern process_item *i2c_request_item;
extern process_item *i2c_response_item;
extern int32_t Tstatus;
/* Exported Functions   ------------------------------------------------------*/
void WaitForCommand(void);
void ClearBuffers(void);

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
