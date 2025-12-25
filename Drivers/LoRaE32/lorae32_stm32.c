/*
 * lorae32_stm32.c
 *
 *  Created on: Nov 8, 2025
 *      Author: gmail
 */
#include <stm32g0xx_hal.h>
#include <main.h>
#include <lorae32_stm32.h>
#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <stdbool.h>

extern UART_HandleTypeDef huart3;
                           //{HEAD, ADDH, ADDL,SPEED, CHNL, OPTN 0xC7=MIN PWR, 0xC4=MAX PWR
uint8_t lora_config_packet[]={0xC0, 0x00, 0x01, 0x1A, 0x06, 0xC4};
QueueHandle_t lora_uart_queue;
uint8_t lora_response[64];

bool LoRaE32WaitforResponse(uint16_t response_length, uint16_t timeout);
bool LoRaWaitforAUX();
void LoRaFlushUARTBuff();

void LoRaE32Init()
{
	lora_uart_queue=xQueueCreate(128,sizeof(uint8_t));
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);//Enable data reception interrupt
}

/*
 * This function must be called from USART ISR
 * to push data to the queue
 */
void LoRaE32PushByte(uint8_t byte)
{
	xQueueSendFromISR(lora_uart_queue, &byte, NULL);
}

static void LoRaE32SetMode(LoRaE32Mode_t mode)
{
	switch(mode)
	{
		case LORAE32_MODE_GENERAL:
			HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, GPIO_PIN_RESET);
			break;

		case LORAE32_MODE_WAKEUP:
			HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, GPIO_PIN_RESET);
			break;

		case LORAE32_MODE_PWRSAVE:
			HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, GPIO_PIN_SET);
			break;

		case LORAE32_MODE_SLEEP:
			HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, GPIO_PIN_SET);
			break;
	}

}

bool LoRaE32Configure()
{
	bool result;

	LoRaE32SetMode(LORAE32_MODE_SLEEP);
	HAL_Delay(10);

	HAL_UART_Transmit(&huart3, lora_config_packet, 6, 1000);
	HAL_Delay(10);

	result=LoRaE32WaitforResponse(6, 1000);

	LoRaE32SetMode(LORAE32_MODE_GENERAL);
	HAL_Delay(10);

	if(!result)
	{
		return false;
	}
	else
	{

		for(int i=0;i<6;i++)
		{
			if(lora_response[i]!=lora_config_packet[i])
			{
				return false;
			}
		}
		return true;
	}

	return result;

}

bool LoRaE32WaitforResponse(uint16_t response_length, uint16_t timeout)
{
	BaseType_t result;

	uint8_t *target_buff=&lora_response[0];

	do
	{
		result=xQueueReceive(lora_uart_queue, target_buff, timeout);

		if(result!=pdPASS)
		{
			return false;
		}

		response_length--;
		target_buff++;

	}while(response_length>0);

	return true;
}

bool LoRaWaitforAUX()
{
	uint16_t count=200;
	while (HAL_GPIO_ReadPin(LORA_AUX_GPIO_Port, LORA_AUX_Pin)==GPIO_PIN_RESET)
	{
		count--;

		if(count==0)
		{
			return false;
		}
		osDelay(100);
	}

	return true;
}

void LoRaFlushUARTBuff()
{
	uint8_t data;
	BaseType_t result;

	do
	{
		result=xQueueReceive(lora_uart_queue, &data, 0);
	}while(result==pdPASS);
}

void LoRaE32Send(const uint8_t *data, uint16_t len)
{
	taskENTER_CRITICAL();
	HAL_UART_Transmit(&huart3, data, len, 1000);
	taskEXIT_CRITICAL();
}

void LoRaE32GetResponse(uint8_t *buff, uint16_t len)
{
	for(int i=0;i<len;i++)
	{
		buff[i]=lora_response[i];
	}
}

