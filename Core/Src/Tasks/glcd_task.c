/*
 * glcd_task.c
 *
 *  Created on: Dec 10, 2025
 *      Author: gmail
 */
#include <stm32g0xx_hal.h>
#include <cmsis_os2.h>

#include "gfx.h"
#include "font.h"
#include "../myconsole_glcd.h"

#include "ui.h"

ScreenType_t g_screen=SCREEN_CONSOLE;

void StartGLCDTask(void *argument)
{
	InitMainScreenData();
	for(;;)
	{
		switch(g_screen)
		{
			case SCREEN_CONSOLE:
				DrawConsole();
				break;

			case SCREEN_MAIN:
				DrawMainScreen();
				break;
		}


		osDelay(250);
	}
}

void SetCurrentScreen(ScreenType_t screen)
{
	g_screen = screen;
}

