/*
 * ui.h
 *
 *  Created on: Dec 14, 2025
 *      Author: gmail
 */

#ifndef SRC_TASKS_UI_H_
#define SRC_TASKS_UI_H_

extern volatile uint16_t speed[16];

typedef enum ScreenType
{
	SCREEN_CONSOLE,
	SCREEN_MAIN
} ScreenType_t;

void InitMainScreenData();
void DrawMainScreen(void);



#endif /* SRC_TASKS_UI_H_ */
