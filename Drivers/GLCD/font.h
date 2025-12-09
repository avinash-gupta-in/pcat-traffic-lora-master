#ifndef _FONT_H_
#define _FONT_H_

#include "stdint.h"

//Constants
#define FONT_OFFSET_WIDTH 2
#define FONT_OFFSET_HEIGHT 3
#define FONT_OFFSET_FIRSTCHAR 4
#define FONT_OFFSET_CHARCOUNT 5
#define FONT_OFFSET_WTABLE 6

//Main Selected Font
extern const uint8_t *font;

//Functions
void __GFXInitFont();

uint8_t GFXGetFontHeight();

uint8_t __GFXReadFontData(uint16_t add);

uint8_t __GFXReadPGM(const uint8_t *ptr);

void GFXSetFont(const uint8_t *new_font);

#endif
