/*
 * ui.h
 *
 *  Created on: Dec 14, 2025
 *      Author: gmail
 */

#ifndef SRC_TASKS_UI_H_
#define SRC_TASKS_UI_H_

//Light states
#define GO 		1
#define STOP 	0

extern volatile uint16_t speed[16];
extern volatile int16_t vehicle_count;
extern volatile uint16_t light_state;

typedef enum ScreenType
{
	SCREEN_CONSOLE,
	SCREEN_MAIN
} ScreenType_t;

void InitMainScreenData();
void DrawMainScreen(void);



#endif /* SRC_TASKS_UI_H_ */
