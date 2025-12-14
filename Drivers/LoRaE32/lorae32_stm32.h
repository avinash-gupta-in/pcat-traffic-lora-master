/*
 * lorae32_stm32.h
 *
 *  Created on: Nov 8, 2025
 *      Author: gmail
 */

#ifndef LORAE32_LORAE32_STM32_H_
#define LORAE32_LORAE32_STM32_H_

#include <stdbool.h>

// todo remove this
extern uint8_t lora_response[64];

typedef enum LoRaE32Mode
{
	LORAE32_MODE_GENERAL,
	LORAE32_MODE_WAKEUP,
	LORAE32_MODE_PWRSAVE,
	LORAE32_MODE_SLEEP

}LoRaE32Mode_t;

void LoRaE32Init();
bool LoRaE32Configure();
void LoRaE32PushByte(uint8_t byte);
void LoRaE32Send(const uint8_t *data, uint16_t len);
void LoRaE32GetResponse(uint8_t *buff, uint16_t len);

#endif /* LORAE32_LORAE32_STM32_H_ */
