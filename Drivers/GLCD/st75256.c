#include "stm32g0xx_hal.h"
#include "st75256.h"
#include "st75256_io.h"

uint8_t st75256_display_buffer[ST72256_DISP_WIDTH*ST72256_DISP_HEIGHT/8];

static void __ST75256SendCmd(uint8_t cmd);
static void __ST75256ExtensionCmd(uint8_t extension);
void __ST75256SendCmdNoCS(uint8_t cmd);
void __ST75256SendDataNoCS(uint8_t data);



void ST75256Init()
{
	HAL_Delay(20);
	ST75256IOInit();

	//Reset
	__ST75256Reset();
	HAL_Delay(2);

	__ST75256NoReset();
	HAL_Delay(2);


	__ST75256ExtensionCmd(1);
	__ST75256SendCmd(0x6E);

	//Osc
	__ST75256SendCmd(ST75256_CMD_OSC_ON);
	HAL_Delay(100);

	//Sleepout (wake)
	__ST75256SendCmd(ST75256_CMD_WAKE);

	HAL_Delay(50);

	//Display off
	__ST75256SendCmd(ST75256_CMD_DISP_OFF);

	HAL_Delay(50);

	//Row/Col
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x75);
	__ST75256SendDataNoCS(0x00);
	__ST75256SendDataNoCS(0x4F);
	__ST75256DeSelectChip();

	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x15);
	__ST75256SendDataNoCS(0x00);
	__ST75256SendDataNoCS(255);
	__ST75256DeSelectChip();

	//Disable Auto Read
	__ST75256ExtensionCmd(2);
	//0xd7, 0x9f
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0xD7);
	__ST75256SendDataNoCS(0x9F);
	__ST75256DeSelectChip();
	HAL_Delay(1);


	//Analog Circuit
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x32);
	__ST75256SendDataNoCS(0x00);
	__ST75256SendDataNoCS(0x01);
	__ST75256SendDataNoCS(0x00);
	__ST75256DeSelectChip();
	HAL_Delay(1);

	//Sacn dir
	//0xBC,0x00
	__ST75256ExtensionCmd(1);
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0xBC);
	__ST75256SendDataNoCS(0x02);
	__ST75256DeSelectChip();

	//LSB on Top
	__ST75256SendCmd(0x0C);

	//Display Control
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(ST75256_CMD_DISP_CTRL);
	__ST75256SendDataNoCS(0x00);//clock divide 1
	__ST75256SendDataNoCS(0x7F);//Duty 128
	__ST75256SendDataNoCS(0x20);//todo
	__ST75256DeSelectChip();

	//Internal Power
	__ST75256ExtensionCmd(2);
	__ST75256SendCmd(0x40);

	/* Power control: Regulator, follower & booster on */
	__ST75256ExtensionCmd(1);
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x20);
	__ST75256SendDataNoCS(0x0B);
	__ST75256DeSelectChip();

	//VOP
	__ST75256ExtensionCmd(1);
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x81);
	__ST75256SendDataNoCS(0x29);
	__ST75256SendDataNoCS(0x04);
	__ST75256DeSelectChip();

	HAL_Delay(200);

	//Turn on display
	__ST75256SendCmd(ST75256_CMD_DISP_ON);

	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x5C);
	for(int i=0;i<256*16;i++)
	{
		__ST75256SendDataNoCS(0x00);
	}
	__ST75256DeSelectChip();
}

void __ST75256SendCmd(uint8_t cmd)
{
	__ST75256CmdMode();
	__ST75256SelectChip();
	__ST75256SPIWriteByte(cmd);
	__ST75256DeSelectChip();
}

void __ST75256SendCmdNoCS(uint8_t cmd)
{
	__ST75256CmdMode();
	__ST75256SPIWriteByte(cmd);
}

void __ST75256SendDataNoCS(uint8_t data)
{
	__ST75256DataMode();
	__ST75256SPIWriteByte(data);
}

void __ST75256ExtensionCmd(uint8_t extension)
{
	switch(extension)
	{
		case 1:
			__ST75256SendCmd(0x30);
			break;

		case 2:
			__ST75256SendCmd(0x31);
			break;

		case 3:
			__ST75256SendCmd(0x38);
			break;

		case 4:
			__ST75256SendCmd(0x39);
			break;
	}
}

void ST75256PutPixel(uint16_t x, uint16_t y, uint16_t colour)
{
	uint16_t yyy=y/8;
	uint8_t *temp=&st75256_display_buffer[ST72256_DISP_WIDTH*yyy+x];
	uint16_t yy=y%8;

	if(colour)
	{
		(*temp)|=(1<<yy);
	}
	else
	{
		(*temp)&=(~(1<<yy));
	}

}

void ST75256Update()
{
	uint8_t *ptr=st75256_display_buffer;

	//Set Page and col address
	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x75);//set page address
	__ST75256SendDataNoCS(0);
	__ST75256SendDataNoCS(15);
	__ST75256DeSelectChip();

	__ST75256SendCmdNoCS(0x15);//set column address
	__ST75256SendDataNoCS(0);
	__ST75256SendDataNoCS(255);
	__ST75256DeSelectChip();

	__ST75256SelectChip();
	__ST75256SendCmdNoCS(0x5C);

	for(int i=0;i<256*16;i++)
	{
		__ST75256SendDataNoCS(*ptr);
		ptr++;
	}
	__ST75256DeSelectChip();
}


