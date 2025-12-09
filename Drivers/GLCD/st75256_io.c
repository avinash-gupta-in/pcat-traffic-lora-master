/*
 * st75256_io.c
 *
 *  Created on: Aug 2, 2023
 *      Author: gmail
 */
#include "st75256_io.h"
#include "stm32g0xx_hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

void ST75256IOInit()
{
	/*
	 * Init GPIO for CS,A0,RESET
	 */

	__ST75256DeSelectChip();


	ST75256BacklightOn();

}

void __ST75256SelectChip()
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

void __ST75256DeSelectChip()
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void __ST75256Reset()
{
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
}

void __ST75256NoReset()
{
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
}

void __ST75256CmdMode()
{
	HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET);
}

void __ST75256DataMode()
{
	HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET);
}

void ST75256BacklightOn()
{
	HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

void ST75256BacklightOff()
{
	HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin,  GPIO_PIN_RESET);
}

void __ST75256SPIWriteByte(uint8_t data)
{
	uint8_t d[2];

	d[0]=data;

	HAL_SPI_Transmit(&hspi1, d, 1, 1);
}

