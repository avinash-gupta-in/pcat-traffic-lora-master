/*

12 Aug 2010 - Added and tested line algorithm.
12 Aug 2010 - Added and Tested Circle Algorithm
12 Aug 2010 - Added and Tested DrawImage(x,y,image)
14 Aug 2010 - Added Int printing fucntion.
				Tested Positive,Negative int printing
				in both fixed and varrriable field length.
19 Aug 2010 - Added Security Check of Splash Screen Modification.

*/

#include <stdlib.h>

#include "gfx.h"
#include "font.h"
#include "st75256.h"

void GFXInit()
{
	ST75256Init();
}
void GFXUpdate()
{
	ST75256Update();
}

void GFXRawPutPixel(uint16_t x,uint16_t y,uint8_t color)
{
	ST75256PutPixel(x, y, color);
}

void GFXBacklightOn()
{
	ST75256BacklightOn();
}

void GFXHLine(uint16_t x1,uint16_t y1,uint16_t len,uint8_t color)
{
	uint16_t x2=x1+len;

	#ifndef GFX_OPTION_NOCHECK
		if(x1>=GFX_SCREEN_WIDTH)	x1=GFX_SCREEN_WIDTH-1;
		if(y1>=GFX_SCREEN_HEIGHT)	y1=GFX_SCREEN_HEIGHT-1;

		if(x2>GFX_SCREEN_WIDTH)	x1=GFX_SCREEN_WIDTH;
	#endif

	uint16_t _x;
	
	for(_x=x1;_x<x2;_x++)
		GFXRawPutPixel(_x,y1,color);
}

void GFXVLine(uint16_t x1,uint16_t y1,uint16_t len,uint8_t color)
{
	uint16_t y2=y1+len;

	#ifndef GFX_OPTION_NOCHECK
		if(x1>=GFX_SCREEN_WIDTH)	x1=GFX_SCREEN_WIDTH-1;
		if(y1>=GFX_SCREEN_HEIGHT)	y1=GFX_SCREEN_HEIGHT-1;

		if(y2>GFX_SCREEN_HEIGHT)	y1=GFX_SCREEN_HEIGHT;
	#endif

	uint16_t _y;
	for(_y=y1;_y<y2;_y++)
		GFXRawPutPixel(x1,_y,color);
}

void GFXRect(uint16_t x1,uint16_t y1,uint16_t x2, uint16_t y2,uint8_t color)
{
	uint16_t width,height;

	width=x2-x1+1;
	height = y2-y1+1;

	GFXHLine(x1,y1,width,color);
	GFXHLine(x1,y2,width,color);

	GFXVLine(x1,y1,height,color);
	GFXVLine(x2,y1,height,color);
}

void GFXFillRect(uint16_t x1,uint16_t y1,uint16_t x2, uint16_t y2,uint8_t color)
{
	#ifndef GFX_OPTION_NOCHECK
		if(x2<x1)	return;
		if(y2<y1)	return;

		if(x1>=GFX_SCREEN_WIDTH) return;
		if(x2>=GFX_SCREEN_WIDTH) return;

		if(y1>=GFX_SCREEN_HEIGHT) return;
		if(y2>=GFX_SCREEN_HEIGHT) return;
	#endif

	uint16_t x;
	uint16_t y;

	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
		{
			GFXRawPutPixel(x,y,color);
		}
	}
}

void GFXClear()
{
	GFXFillRect(0,0,GFX_SCREEN_WIDTH-1,GFX_SCREEN_HEIGHT-1,GFX_COLOR_WHITE);
}

int8_t GFXPutCharXY(uint16_t x, uint16_t y,char c,uint8_t color)
{

	uint8_t width = 0;
	uint8_t height = __GFXReadFontData(FONT_OFFSET_HEIGHT);
	uint8_t bytes = (height+7)/8;
	
	uint8_t firstChar = __GFXReadFontData(FONT_OFFSET_FIRSTCHAR);
	uint8_t charCount = __GFXReadFontData(FONT_OFFSET_CHARCOUNT);
	
	int16_t index = 0;
	
	if(c < firstChar || c >= (firstChar+charCount)) {
		return -1;//Error
	}
	
	c-= firstChar;
	
	// read width data, to get the index
	for(uint8_t i=0; i<c; i++) {
		index += __GFXReadFontData(FONT_OFFSET_WTABLE+i);
	}
	index =index*bytes+charCount+FONT_OFFSET_WTABLE;
	width = __GFXReadFontData(FONT_OFFSET_WTABLE+c);


	//Draw
	uint16_t _x,_y,fx,fy,b;

	uint16_t address;
	
	_y=y;


	uint8_t shift_val;
	shift_val=(bytes*8)-height;

	bytes--;
	
	for(b=0;b<bytes;b++)
	{
		uint8_t mask=0b00000001;
		for(fy=0;fy<8;fy++)
		{

			if((_y+fy)>(GFX_SCREEN_HEIGHT-1))
				break;

			address=(index+b*width);

			for(_x=x,fx=0;fx<width;fx++,_x++)
			{
				if(_x>(GFX_SCREEN_WIDTH-1))	break;

				uint8_t data=__GFXReadFontData(address);

				uint8_t bit= (data & mask);

				if(bit)
					GFXRawPutPixel(_x,_y+fy,color);
				//else
					//GFXRawPutPixel(_x,_y+fy,color_invert);

				address++;

			}
			mask=mask<<1;
			
		}
		_y+=8;
	}

	//Last Byte May require shifting so draw it separtly

		uint8_t mask=0b00000001<<shift_val;
		for(fy=0;fy<(8-shift_val);fy++)
		{
			if((_y+fy)>(GFX_SCREEN_HEIGHT-1))
				break;
			
			address=(index+b*width);

			for(_x=x,fx=0;fx<width;fx++,_x++)
			{
				if(_x>(GFX_SCREEN_WIDTH-1))	break;

				uint8_t data=__GFXReadFontData(address);

				uint8_t bit= (data & mask);

				if(bit)
					GFXRawPutPixel(_x,_y+fy,color);
				//else
					//GFXRawPutPixel(_x,_y+fy,color_invert);

				address++;

			}
			mask=mask<<1;
			
		}
	return 1;	
}

int8_t GFXGetCharWidth(char c)
{

	uint8_t firstChar = __GFXReadFontData(FONT_OFFSET_FIRSTCHAR);
	uint8_t charCount = __GFXReadFontData(FONT_OFFSET_CHARCOUNT);
	
	
	if(c < firstChar || c >= (firstChar+charCount)) {
		return -1;//Error
	}
	
	c-= firstChar;
	
	return __GFXReadFontData(FONT_OFFSET_WTABLE+c);
}

int8_t GFXGetCharHeight()
{
	return __GFXReadFontData(FONT_OFFSET_HEIGHT);
}

int16_t GFXGetStringWidth(const char *string)
{
	int16_t w,r;

	w=0;
	while(*string!='\0')
	{
		r=GFXGetCharWidth(*string);
		if(r==-1) return -1;

		w+=r;
		w++;//Extra Spacing Between Chars

		string++;
	}

	return w;
}

int8_t GFXWriteStringXY(uint16_t x,uint16_t y,const char *string,uint8_t color)
{
	int8_t w;
	while(*string!='\0')
	{
		if(GFXPutCharXY(x,y,*string,color)==-1)
			return -1;
		
		w=GFXGetCharWidth(*string);
		if(w==-1)
			return -1;

		x+=w;
		x++; //Blank Line after each char
		string++;
	}
	return 1;
}

int8_t GFXCenterText(const char *string,uint8_t color)
{
	uint16_t x,y;
	x=GFXGetStringWidth(string);

	if(x==-1) return -1;

	x=(GFX_SCREEN_WIDTH/2)-(x/2);

	if(x<0 || x>(GFX_SCREEN_WIDTH-1))
		return -1;

	y=GFXGetCharHeight();

	y=((GFX_SCREEN_HEIGHT/2)-(y/2));

	if(y<0 || y>(GFX_SCREEN_HEIGHT-1))
		return -1;

	return GFXWriteStringXY(x,y,string,color);
}

int8_t GFXHCenterText(int16_t y, const char *string, uint8_t color)
{
	uint16_t x;
	x=GFXGetStringWidth(string);

	if(x==-1) return -1;

	x=(GFX_SCREEN_WIDTH/2)-(x/2);

	if(x<0 || x>(GFX_SCREEN_WIDTH-1))
		return -1;

	return GFXWriteStringXY(x,y,string,color);
}


void GFXLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color)
{
	uint8_t x,y;	//Current POS
	int8_t xinc,yinc;

	if(x1<=x2)
		xinc=1;
	else
		xinc=-1;

	if(y1<=y2)
		yinc=1;
	else
		yinc=-1;

	//x=x1;
	//y=y1;

	int16_t deltax,deltay;

	deltax=abs(x2-x1);
	deltay=abs(y2-y1);

	

	int16_t d;

	if(deltax>=deltay)
	{
		int16_t v2dy=deltay<<1;
		int16_t v2dydx=(deltay-deltax)<<1;
		d=(2*deltay)-deltax;

		for(x=x1,y=y1;x!=x2;x+=xinc)
		{

			if(x>=GFX_SCREEN_WIDTH || y>=GFX_SCREEN_HEIGHT)
				break;
			else
				GFXRawPutPixel(x,y,color);
			if(d<0)
				d+=v2dy;
			else
			{
				d+=v2dydx;
				y+=yinc;
			}
		}
		if(x<GFX_SCREEN_WIDTH && y<GFX_SCREEN_HEIGHT)
			GFXRawPutPixel(x,y,color);		
		
	}
	else
	{
		int16_t v2dx=deltax<<1;
		int16_t v2dxdy=(deltax-deltay)<<1;
		d=(2*deltax)-deltay;

		for(y=y1,x=x1;y!=y2;y+=yinc)
		{

			if(x>=GFX_SCREEN_WIDTH || y>=GFX_SCREEN_HEIGHT)
				break;
			else
				GFXRawPutPixel(x,y,color);
			if(d<0)
				d+=v2dx;
			else
			{
				d+=v2dxdy;
				x+=xinc;
			}
		}
		if(x<GFX_SCREEN_WIDTH && y<GFX_SCREEN_HEIGHT)
			GFXRawPutPixel(x,y,color);	
	}

}

void GFXCircle(uint8_t cx,uint8_t cy,uint8_t rad,uint8_t color)
{
	#ifndef GFX_OPTION_NOCHECK

		if(((cx+rad)>=GFX_SCREEN_WIDTH) || (((int)cx-rad)<0) || ((cy+rad)>=GFX_SCREEN_HEIGHT) || (((int)cy-rad)<0)) return;
	
	#endif
	
	int16_t d;
	d=3-(rad<<1);

	uint8_t x,y;	//Current POS

	x=0;
	y=rad;

	

	while(1)
	{
		GFXRawPutPixel(cx+x,cy+y,color);
		GFXRawPutPixel(cx+x,cy-y,color);
		GFXRawPutPixel(cx-x,cy+y,color);
		GFXRawPutPixel(cx-x,cy-y,color);

		GFXRawPutPixel(cx+y,cy+x,color);
		GFXRawPutPixel(cx+y,cy-x,color);
		GFXRawPutPixel(cx-y,cy+x,color);
		GFXRawPutPixel(cx-y,cy-x,color);

		if(d<0)
			d=d+(x<<2)+6;
		else
		{
			d=d+((x-y)<<2)+10;
			y=y-1;
		}
		

		if(x>=y) break;

		x+=1;


	}

}
/*
void GFXDrawImage(uint8_t x,uint8_t y,const uint8_t *img)
{
	uint8_t xx,yy;
	uint8_t h,w;
	
	
	

	w=pgm_read_byte(img);
	img++;
	h=pgm_read_byte(img);
	img++;

	#ifndef GFX_OPTION_NOCHECK

		if((x+w)>GFX_SCREEN_WIDTH) return;
		if((y+h)>GFX_SCREEN_HEIGHT) return;
	
	#endif

	for(yy=0;yy<h;yy++)
	{
		for(xx=0;xx<w;xx++)
		{	
			if(pgm_read_byte(img+((yy/8)*w)+xx) & (1<<(yy%8)))
				GFXRawPutPixel(x+xx,y+yy,GFX_COLOR_BLACK);
			else
				GFXRawPutPixel(x+xx,y+yy,GFX_COLOR_WHITE);
		}
	
	}
}*/

void GFXWriteIntXY(uint8_t x,uint8_t y,int16_t val,int8_t feild_length,uint8_t color)
{
	char str[7]={0,0,0,0,0,0,0};

	if(val<0) 
	{
		str[6]='-';
		val=-val;
	}
	else
		str[6]=' ';

	int i=5,j=0;
	
	while(val)
	{
		str[i]=val%10;
		val=val/10;
		i--;
	}
	if(feild_length==-1)
		while(str[j]==0) j++;
	else
		j=6-feild_length;

	//The Sign
	GFXPutCharXY(x,y,str[6],color);
	x+=GFXGetCharWidth(str[6]);
	x++;
	
	for(i=j;i<6;i++)
	{
		GFXPutCharXY(x,y,'0'+str[i],color);
		x+=GFXGetCharWidth('0'+str[i]);
		x++;
	}
}
