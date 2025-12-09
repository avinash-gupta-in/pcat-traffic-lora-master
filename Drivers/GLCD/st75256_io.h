/*
 * st75256_io.h
 *
 *  Created on: Aug 2, 2023
 *      Author: gmail
 */

#ifndef SRC_ST75256_ST75256_IO_H_
#define SRC_ST75256_ST75256_IO_H_

#include "stm32g0xx_hal.h"

void ST75256IOInit();
void __ST75256SPIWriteByte(uint8_t data);
void __ST75256SelectChip();
void __ST75256DeSelectChip();
void __ST75256Reset();
void __ST75256NoReset();
void __ST75256CmdMode();
void __ST75256DataMode();
void ST75256BacklightOn();
void ST75256BacklightOff();

#endif /* SRC_ST75256_ST75256_IO_H_ */
