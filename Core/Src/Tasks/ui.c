/*
 * ui.c
 *
 *  Created on: Dec 14, 2025
 *      Author: gmail
 */
#include <stdio.h>
#include <stdbool.h>
#include <master_config.h>
#include "gfx.h"
#include "font.h"
#include "5x7_single.h"
#include "fnt_arial12.h"
#include "5x7_single.h"
#include "Arial34B.h"
#include "Arial48B.h"
#include "ui.h"

#define WIDGET_WIDTH 60
#define WIDGET_HEIGHT 43
#define TIMEOUT_PERIOD 160 //in unit of 250ms

volatile uint16_t speed[16];
volatile uint8_t dir[8];
volatile uint32_t time_stamp[8];
volatile bool slave_conn_status[SLAVE_COUNT];
//disabled slaves are not polled to save time
extern bool slave_enabled[SLAVE_COUNT];

volatile int16_t vehicle_count=0;
volatile uint16_t light_state=GO;

int32_t timeout_counter;

static void DrawCount(uint16_t count);
static void DrawRadarDataWidget(uint16_t x, uint16_t y, uint16_t sensor_id, uint16_t speed1, uint16_t speed2, uint8_t dir, bool online);

void InitMainScreenData()
{
	for(uint16_t i=0;i<SLAVE_COUNT;i++)
	{
		slave_conn_status[i]=false;//disconnected
	}
}

void UISetRadarState(uint16_t id, bool connected)
{
	slave_conn_status[id]=connected;
}

void DrawMainScreen(void)
{
	GFXClear();
	//Title Bar
	GFXFillRect(0, 0, GFX_SCREEN_WIDTH-1, 14, GFX_COLOR_BLACK);

	//Main Window Border
	GFXRect(0, 0, GFX_SCREEN_WIDTH-1, GFX_SCREEN_HEIGHT-1, GFX_COLOR_BLACK);
	GFXSetFont(Arial12);

	//ProcessInputs();
	GFXHCenterText(3, "Vehicle Counting (Radar System)", GFX_COLOR_WHITE);

	//Sensor 1-4
	DrawRadarDataWidget(3,16,1,speed[0],speed[1],dir[0],slave_conn_status[0]);
	DrawRadarDataWidget(3,16+WIDGET_HEIGHT+2,2,speed[2],speed[3], dir[1],slave_conn_status[1]);

	DrawRadarDataWidget(3+WIDGET_WIDTH+2,16,3,speed[4], speed[5], dir[2],slave_conn_status[2]);
	DrawRadarDataWidget(3+WIDGET_WIDTH+2,16+WIDGET_HEIGHT+2,4,speed[6],speed[7],dir[3],slave_conn_status[3]);

	//Sensor 5-8
	DrawRadarDataWidget(3+WIDGET_WIDTH*2+4,16,5,speed[8],speed[9], dir[4],slave_conn_status[4]);
	DrawRadarDataWidget(3+WIDGET_WIDTH*2+4,16+WIDGET_HEIGHT+2,6,speed[10], speed[11], dir[5],slave_conn_status[5]);

	DrawRadarDataWidget(3+WIDGET_WIDTH*3+6,16,7,speed[12], speed[13], dir[6],slave_conn_status[6]);
	DrawRadarDataWidget(3+WIDGET_WIDTH*3+6,16+WIDGET_HEIGHT+2,8,speed[14],speed[15], dir[7],slave_conn_status[7]);



	DrawCount(vehicle_count);

	GFXSetFont(Arial12);

	//GFXWriteIntXY(2, 2, fps, 3, GFX_COLOR_WHITE);
	GFXWriteIntXY(2, 2, timeout_counter, 3, GFX_COLOR_WHITE);
	GFXUpdate();
}

static void DrawRadarDataWidget(uint16_t x, uint16_t y, uint16_t sensor_id, uint16_t speed1, uint16_t speed2, uint8_t dir, bool online)
{
	char buff[32];

	GFXRect(x, y, x+WIDGET_WIDTH, y+WIDGET_HEIGHT, GFX_COLOR_BLACK);

	//Bottom Lable
	GFXFillRect(x, y+WIDGET_HEIGHT-14, x+WIDGET_WIDTH, y+WIDGET_HEIGHT, GFX_COLOR_BLACK);//background
	GFXSetFont(Arial12);
	sprintf(buff,"Slave#%d", sensor_id);
	GFXWriteStringXY(x+2, y+WIDGET_HEIGHT-12, buff, GFX_COLOR_WHITE);//text

	if(slave_enabled[sensor_id-1])
	{

		//Speed
		if(online)
		{
			GFXSetFont(Arial12);
			GFXWriteStringXY(x+2, y+2, "IN  ", GFX_COLOR_BLACK);
			GFXWriteStringXY(x+2, y+2+14, "OUT ", GFX_COLOR_BLACK);

			GFXWriteIntXY(x+2+25, y+2, speed1, 3, GFX_COLOR_BLACK);
			GFXWriteIntXY(x+2+25, y+2+14, speed2, 3, GFX_COLOR_BLACK);
			/*
			GFXSetFont(Arial34);
			GFXWriteIntXY(x+2, y+2, speed, 3, GFX_COLOR_BLACK);
			GFXSetFont(Arial12);
			*/

			/*
			if(dir==COMING)
			{

				GFXWriteStringXY(x+4,y+WIDGET_HEIGHT-26,"Coming",GFX_COLOR_BLACK);
			}
			else
			{
				GFXWriteStringXY(x+4,y+WIDGET_HEIGHT-26,"Going", GFX_COLOR_BLACK);
			}
			*/

		}
		else
		{
			GFXWriteStringXY(x+2, y+2, "Offline", GFX_COLOR_BLACK);
		}
	}
	else
	{
		//Slave is disabled
		GFXWriteStringXY(x+2, y+12, "Disabled", GFX_COLOR_BLACK);
	}
}

static void DrawCount(uint16_t count)
{
	GFXSetFont(Arial12);
	GFXWriteStringXY(2, 112, "VEHICLE COUNT:", GFX_COLOR_BLACK);

	GFXWriteIntXY(100, 112, count, 2, GFX_COLOR_BLACK);


	if(light_state==STOP)
	{
		GFXWriteStringXY(184, 112, "STOP", GFX_COLOR_BLACK);
	}
	else
	{
		GFXWriteStringXY(184, 112, "GO", GFX_COLOR_BLACK);
	}

	GFXWriteStringXY(128, 112, "SIGNAL:", GFX_COLOR_BLACK);

}


