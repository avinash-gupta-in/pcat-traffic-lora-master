/*

Abstraction Layer for reading program space data
and Font management.

Hardware: AVR MCU

*/
#include <stdint.h>
#include "font.h"

//Main Selected Font
const uint8_t *font;

void __GFXInitFont()
{

}

uint8_t GFXGetFontHeight()
{
	return __GFXReadPGM(font+FONT_OFFSET_HEIGHT);
}

uint8_t __GFXReadFontData(uint16_t add)
{
	return __GFXReadPGM(font+add);
}

uint8_t __GFXReadPGM(const uint8_t *ptr)
{
	
	return *(ptr);
}

void GFXSetFont(const uint8_t *new_font)
{
	font=new_font;
}

