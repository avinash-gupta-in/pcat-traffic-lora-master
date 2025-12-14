/*
 * myconsole_glcd.c
 *
 *  Created on: Dec 10, 2025
 *      Author: gmail
 */
#include <string.h>
#include <stm32g0xx_hal.h>
#include "myconsole_glcd.h"
#include <gfx.h>
#include <font.h>

extern uint8_t single5x7[];
char console_data[CONSOLE_HEIGHT][CONSOLE_WIDTH];
uint16_t console_current_col, console_current_line;

static void ConsoleScroll();
static void ConsoleNewline();

void InitConsole()
{
	ClearConsole();
}

void ClearConsole()
{
	for(int y=0;y<CONSOLE_HEIGHT;y++)
	{
		for(int x=0;x<CONSOLE_WIDTH;x++)
		{
			console_data[y][x]='\0';
		}
	}
	console_current_line=0;
	console_current_col=0;
}

void DrawConsole()
{
	GFXClear();
	GFXSetFont(single5x7);
	for(int y=0;y<CONSOLE_HEIGHT;y++)
	{
		GFXWriteStringXY(0, y*8, console_data[y], GFX_COLOR_BLACK);
	}
	GFXUpdate();
}

void ConsoleAppendText(const char *str)
{

	if(console_current_line<CONSOLE_HEIGHT-1)
	{
		strcpy(console_data[console_current_line],str);
		console_current_line++;
	}

	else
	{

		for(int y=1;y<CONSOLE_HEIGHT;y++)
		{
			strcpy(console_data[y-1],console_data[y]);
		}
		strcpy(console_data[CONSOLE_HEIGHT-1],str);
	}
}

/*
 * Move up and make space for a single line at the end
 */
void ConsoleScroll()
{
	for(int y=1;y<CONSOLE_HEIGHT;y++)
	{
		strcpy(console_data[y-1],console_data[y]);
	}
	//Clear last line
	strcpy(console_data[CONSOLE_HEIGHT-1],"");
}

void ConsolePutChar(char ch)
{
	if(ch=='\n')
	{
		ConsoleNewline();
		return;
	}

	console_data[console_current_line][console_current_col]=ch;


	if(console_current_col<CONSOLE_WIDTH-1)
	{
		console_current_col++;
		console_data[console_current_line][console_current_col+1]='\0';
	}
	else
	{
		ConsoleNewline();
	}
}

void ConsoleNewline()
{
	console_current_col=0;
	if(console_current_line<CONSOLE_HEIGHT-1)
	{
		console_current_line++;
	}
	else
	{
		ConsoleScroll();
	}
}
