/*
 * st75256.h
 *
 *  Created on: Aug 2, 2023
 *      Author: gmail
 */

#ifndef SRC_ST75256_ST75256_H_
#define SRC_ST75256_ST75256_H_

#define ST72256_DISP_WIDTH	256
#define ST72256_DISP_HEIGHT	128

#define ST75256_CMD_DISP_OFF 	0xAE
#define ST75256_CMD_DISP_ON 	0xAF
#define ST75256_CMD_DISP_CTRL	0xCA
#define ST75256_CMD_OSC_OFF 	0xD2
#define ST75256_CMD_OSC_ON 		0xD1
#define ST75256_CMD_SLEEP 		0x95
#define ST75256_CMD_WAKE 		0x94

void ST75256Init();
void ST75256PutPixel(uint16_t x, uint16_t y, uint16_t colour);
void ST75256Update();

#endif /* SRC_ST75256_ST75256_H_ */
