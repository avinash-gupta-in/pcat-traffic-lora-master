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

void StartGLCDTask(void *argument)
{
	for(;;)
	{
		DrawConsole();
		osDelay(250);
	}
}


