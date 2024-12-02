/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */

#include "config.h"
#include <stdarg.h>
#if USE_SCREEN_TFT_ILI9341
#include "stm32f1_ili9341.h"
#include "stm32f1xx_hal.h"
#include "stm32f1_spi.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"
/*
 * This library contains the functions needed to use the ILI9341 TFT screen with an STM32F1.
 *
 * The module uses the SPI bus and the pins are imposed (fitting power supply and GND must have been chosen aswell):
 *
 *  CS   : PA12 : Chip select for SPI
 *  RESET: PA9  : Pin used to reset the screen
 *  D/C  : PA8  : Pin used to select Data or Command
 *  SDI  : PA7  : Input from SPI master (MOSI)
 *  SCK  : PA5  : SPI Clock
 *  LED  : +3,3V:
 *  SDO  : PA6  : Output from screen to SPI (MISO)
 *
 *  How to use this library:
 *
 *  - Make sure the USE_SCREEN_TFT_ILI9341 in config.h is correctly defined at 1
 *  - Initialise the library (init function)
 *  - Use the wanted function to draw on the screen
 *
 */


/* Pin definitions */
#define ILI9341_RST_SET()			HAL_GPIO_WritePin(ILI9341_RST_PORT,ILI9341_RST_PIN, 1)
#define ILI9341_RST_RESET()			HAL_GPIO_WritePin(ILI9341_RST_PORT,ILI9341_RST_PIN, 0)
#define ILI9341_CS_SET()			HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, 1)
#define ILI9341_CS_RESET()			HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, 0)
#define ILI9341_WRX_SET()			HAL_GPIO_WritePin(ILI9341_WRX_PORT, ILI9341_WRX_PIN, 1)
#define ILI9341_WRX_RESET()			HAL_GPIO_WritePin(ILI9341_WRX_PORT, ILI9341_WRX_PIN, 0)

/* Private defines */
#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_CMD_MEMORY_READ		0x2E
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7

/* Pin functions */
uint16_t ILI9341_x;
uint16_t ILI9341_y;
ILI931_Options_t ILI9341_Opts;
uint8_t ILI9341_INT_CalledFromPuts = 0;

/* Private functions */
void ILI9341_InitLCD(void);
void ILI9341_SendData(uint8_t data);
void ILI9341_ReadDatas(uint8_t command_to_write, uint8_t * datas, uint8_t nb_to_read);
void ILI9341_SendCommand(uint8_t data);
void ILI9341_Delay(volatile unsigned int delay);
void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);


/*
 * Cette fonction � pour but de vous aider � appr�hender les fonctionnalit�s de ce module logiciel.
 * Cette fonction n'est pas blocante et doit �tre appel�e une fois dans l'init.
 */
void ILI9341_demo(void)
{
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
//	ILI9341_DisplayOff();
//	ILI9341_DisplayOn();
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_DrawCircle(20,20,5,ILI9341_COLOR_BLUE);
	ILI9341_DrawLine(20,20,100,20,ILI9341_COLOR_RED);
	ILI9341_DrawLine(20,20,20,100,ILI9341_COLOR_RED);

	#if USE_FONT7x10
		ILI9341_Putc(110,11,'x',&Font_7x10,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
		ILI9341_Putc(15,110,'y',&Font_7x10,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
		ILI9341_Puts(25,200, "chaine 7x10", &Font_7x10, ILI9341_COLOR_BROWN, ILI9341_COLOR_WHITE);
		ILI9341_printf(25, 240, &Font_7x10, ILI9341_COLOR_BROWN, ILI9341_COLOR_WHITE, "%d", 20);
	#endif
	#if USE_FONT11x18
		ILI9341_Puts(25,225, "chaine 11x18", &Font_11x18, ILI9341_COLOR_BROWN, ILI9341_COLOR_WHITE);
	#endif
	#if USE_FONT16x26
		ILI9341_Puts(25,250, "chaine 16x26", &Font_16x26, ILI9341_COLOR_BROWN, ILI9341_COLOR_WHITE);
	#endif
}
/**
 * @brief  Initializes ILI9341 LCD with LTDC peripheral
 *         It also initializes external SDRAM
 * @param  None
 * @retval None
 */
void ILI9341_Init(void)
{
	/* Init WRX pin */
	BSP_GPIO_PinCfg(ILI9341_WRX_PORT,ILI9341_WRX_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_MEDIUM);
	
	/* Init CS pin */
	BSP_GPIO_PinCfg(ILI9341_CS_PORT,ILI9341_CS_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_MEDIUM);
	
	/* Init RST pin */
	BSP_GPIO_PinCfg(ILI9341_RST_PORT,ILI9341_RST_PIN, GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,GPIO_SPEED_FREQ_LOW);


	/* CS high */
	ILI9341_CS_SET();
	
	/* Init SPI */
	SPI_Init(ILI9341_SPI);
	
	/* Init DMA for SPI */
	//SPI_DMA_Init(ILI9341_SPI);
	
	/* Init LCD */
	ILI9341_InitLCD();
	
	/* Set default settings */
	ILI9341_x = ILI9341_y = 0;
	//ILI9341_Rotate(ILI9341_Orientation_Portrait_1);
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);	//Utiliser cette fonction pour modifier l'orientation en paysage
	
	/* Fill with white color */
	ILI9341_Fill(ILI9341_COLOR_WHITE);
}

/**
 * @brief init function for XPT2046 lib
 */
void ILI9341_setConfig(void){
	SPI_setBaudRate(ILI9341_SPI, SPI_BAUDRATEPRESCALER_2);
}


/**
 * @brief  Initializes ILI9341 LCD
 * @param  None
 * @retval None
 */
void ILI9341_InitLCD(void) {
	/* Force reset */
	ILI9341_RST_RESET();
	HAL_Delay(20);
	ILI9341_RST_SET();
	
	/* Delay for RST response */
	HAL_Delay(20);
	
	/* Software reset */
	ILI9341_SendCommand(ILI9341_RESET);
	HAL_Delay(50);
	
	// Power control A
	ILI9341_SendCommand(ILI9341_POWERA);
	ILI9341_SendData(0x39);
	ILI9341_SendData(0x2C);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x34);
	ILI9341_SendData(0x02);

	// Power control B
	ILI9341_SendCommand(ILI9341_POWERB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x30);

	// Driver timing control A
	ILI9341_SendCommand(ILI9341_DTCA);
	ILI9341_SendData(0x85);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x78);

	// Driver timing control B
	ILI9341_SendCommand(ILI9341_DTCB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);

	// Power on sequence control
	ILI9341_SendCommand(ILI9341_POWER_SEQ);
	ILI9341_SendData(0x64);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x12);
	ILI9341_SendData(0x81);

	// Pump ratio control
	ILI9341_SendCommand(ILI9341_PRC);
	ILI9341_SendData(0x20);

	// Power control 1
	ILI9341_SendCommand(ILI9341_POWER1);
	ILI9341_SendData(0x23);

	// Power control 2
	ILI9341_SendCommand(ILI9341_POWER2);
	ILI9341_SendData(0x10);

	// VCOM control 1
	ILI9341_SendCommand(ILI9341_VCOM1);
	ILI9341_SendData(0x3E);
	ILI9341_SendData(0x28);

	// VCOM control 2
	ILI9341_SendCommand(ILI9341_VCOM2);
	ILI9341_SendData(0x86);

	// Memory access control
	ILI9341_SendCommand(ILI9341_MAC);
	ILI9341_SendData(0x48);

	// Pixel format set
	ILI9341_SendCommand(ILI9341_PIXEL_FORMAT);
	ILI9341_SendData(0x55);

	// Frame rate control
	ILI9341_SendCommand(ILI9341_FRC);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x18);

	// Display function control
	ILI9341_SendCommand(ILI9341_DFC);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x82);
	ILI9341_SendData(0x27);

	// 3Gamma function disable
	ILI9341_SendCommand(ILI9341_3GAMMA_EN);
	ILI9341_SendData(0x00);


	ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xEF);

	ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x01);
	ILI9341_SendData(0x3F);

	// Gamma curve selected
	ILI9341_SendCommand(ILI9341_GAMMA);
	ILI9341_SendData(0x01);

	// Set positive gamma
	ILI9341_SendCommand(ILI9341_PGAMMA);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x2B);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x4E);
	ILI9341_SendData(0xF1);
	ILI9341_SendData(0x37);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x10);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x09);
	ILI9341_SendData(0x00);

	// Set negative gama
	ILI9341_SendCommand(ILI9341_NGAMMA);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x14);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x11);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x48);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x36);
	ILI9341_SendData(0x0F);

	ILI9341_SendCommand(ILI9341_SLEEP_OUT);

	HAL_Delay(10);

	ILI9341_SendCommand(ILI9341_DISPLAY_ON);
	ILI9341_SendCommand(ILI9341_GRAM);
}

/**
 * @brief   Enables display
 * @note    After initialization, LCD is enabled and you don't need to call this function
 * @param   None
 * @retval  None
 */
void ILI9341_DisplayOn(void) {
	ILI9341_SendCommand(ILI9341_DISPLAY_ON);
}

/**
 * @brief   Disables display
 * @param   None
 * @retval  None
 */
void ILI9341_DisplayOff(void) {
	ILI9341_SendCommand(ILI9341_DISPLAY_OFF);
}

/**
 * @brief  Sends chosen Command to the SPI bus
 * @param  None
 * @retval None
 */
void ILI9341_SendCommand(uint8_t data) {
	ILI9341_WRX_RESET();
	ILI9341_CS_RESET();
	SPI_WriteNoRegister(ILI9341_SPI,data);
	ILI9341_CS_SET();
}





/**
 * @brief  Sends chosen Data to the SPI bus
 * @param  None
 * @retval None
 */
void ILI9341_SendData(uint8_t data) {
	//TODO Isnt that redundant
	ILI9341_WRX_SET();
	ILI9341_CS_RESET();
	SPI_WriteNoRegister(ILI9341_SPI, data);
	ILI9341_CS_SET();
}

void ILI9341_ReadDatas(uint8_t command_to_write, uint8_t * datas, uint8_t nb_to_read) {
	ILI9341_CS_RESET();
	ILI9341_WRX_RESET();
	SPI_WriteNoRegister(ILI9341_SPI, command_to_write);
	ILI9341_WRX_SET();
	SPI_ReadMultiNoRegister(ILI9341_SPI, datas, nb_to_read);
	ILI9341_CS_SET();
}

/**
 * @brief  Draws single pixel to LCD
 * @param  x: X position for pixel
 * @param  y: Y position for pixel
 * @param  color: Color of pixel
 * @retval None
 */
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
	ILI9341_SetCursorPosition(x, y, x, y);

	ILI9341_SendCommand(ILI9341_GRAM);
	ILI9341_SendData(color >> 8);
	ILI9341_SendData(color & 0xFF);
}

uint16_t ILI9341_ReadPixel(int16_t x, int16_t y)
{
    uint8_t block[400] = {0};
    ILI9341_SetCursorPosition(x, y, x, y);
    ILI9341_ReadDatas(ILI9341_CMD_MEMORY_READ, block, 400);
    return ((block[1] & 0xF8) << 8u) | ((block[2] & 0xFC) << 3u) | (block[3] >> 3u);
}

/**
 * @brief  Used to select an area where the cursor will draw a color on the screen
 * @param  x1: X coordinate of top left corner of area
 * @param  y1: Y coordinate of top left corner of area
 * @param  x2: X coordinate of bottom right of area
 * @param  y2: Y coordinate of bottom right of area
 * @retval None
 */
void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_SendData(x1 >> 8);
	ILI9341_SendData(x1 & 0xFF);
	ILI9341_SendData(x2 >> 8);
	ILI9341_SendData(x2 & 0xFF);

	ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	ILI9341_SendData(y1 >> 8);
	ILI9341_SendData(y1 & 0xFF);
	ILI9341_SendData(y2 >> 8);
	ILI9341_SendData(y2 & 0xFF);
}

/**
 * @brief  Fills entire LCD with color
 * @param  color: Color to be used in fill
 * @retval None
 */
void ILI9341_Fill(uint16_t color) {
	/* Fill entire screen */
	ILI9341_INT_Fill(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height, color);
}

/**
 * @brief  Fills chosen area with a color
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Color to be used in fill
 * @retval None
 */
void ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t pixels_count;
	uint8_t datas[2];
	datas[1] = HIGHINT(color);
	datas[0] = LOWINT(color);

	/* Set cursor position */
	ILI9341_SetCursorPosition(x0, y0, x1, y1);

	/* Set command for GRAM data */
	ILI9341_SendCommand(ILI9341_GRAM);
	
	/* Calculate pixels count */
	pixels_count = (x1 - x0 + 1) * (y1 - y0 + 1);

	/* Send everything */
	ILI9341_CS_RESET();
	ILI9341_WRX_SET();
	
	/* Go to 16-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_16b);
	
	/* Send first 65535 bytes, SPI MUST BE IN 16-bit MODE */
	//SPI_DMA_SendHalfWord(ILI9341_SPI, color, (pixels_count > 0xFFFF) ? 0xFFFF : pixels_count);
	uint32_t i;
	for(i=0;i<pixels_count;i++)
		SPI_WriteMultiNoRegister(ILI9341_SPI, datas, 1);

	/* Wait till done */
	//while (SPI_DMA_Working(ILI9341_SPI));
	
	/* Check again */
	//if (pixels_count > 0xFFFF) {
		/* Send remaining data */
		//SPI_DMA_SendHalfWord(ILI9341_SPI, color, pixels_count - 0xFFFF);
		/* Wait till done */
		//while (SPI_DMA_Working(ILI9341_SPI));
	//}
	
	ILI9341_CS_SET();

	/* Go back to 8-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_8b);
}

void ILI9341_Delay(volatile unsigned int delay) {
	for (; delay != 0; delay--); 
}

/**
 * @brief  Rotates LCD to specific orientation
 * @param  orientation: LCD orientation. This parameter can be a value of @ref ILI9341_Orientation_t enumeration
 * @retval None
 */
void ILI9341_Rotate(ILI9341_Orientation_t orientation) {
	ILI9341_SendCommand(ILI9341_MAC);

#if (ILI9341_WIDTH == 160)		//TFT 1.44"
		ILI9341_SendData(0x68);
		ILI9341_Opts.width = ILI9341_WIDTH;
		ILI9341_Opts.height = ILI9341_HEIGHT;
		ILI9341_Opts.orientation = ILI9341_Orientation_Portrait_1;
#else
	if (orientation == ILI9341_Orientation_Portrait_1) {
			ILI9341_SendData(0x58);
		} else if (orientation == ILI9341_Orientation_Portrait_2) {
			ILI9341_SendData(0x88);
		} else if (orientation == ILI9341_Orientation_Landscape_1) {
			ILI9341_SendData(0x28);
		} else if (orientation == ILI9341_Orientation_Landscape_2) {
			ILI9341_SendData(0xE8);
		}

	ILI9341_Opts.orientation = orientation;
	if (orientation == ILI9341_Orientation_Portrait_1 || orientation == ILI9341_Orientation_Portrait_2) {
		ILI9341_Opts.width = ILI9341_WIDTH;
		ILI9341_Opts.height = ILI9341_HEIGHT;
	} else {
		ILI9341_Opts.width = ILI9341_HEIGHT;
		ILI9341_Opts.height = ILI9341_WIDTH;
	}
#endif
}

/**
 * @brief  Puts string to LCD
 * @param  x: X position of top left corner of first character in string
 * @param  y: Y position of top left corner of first character in string
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref FontDef_t used font
 * @param  foreground: Color for string
 * @param  background: Color for string background
 * @retval None
 */
void ILI9341_Puts(uint16_t x, uint16_t y, char *str, FontDef_t *font, uint16_t foreground, uint16_t background) {
	uint16_t startX = x;
	
	/* Set X and Y coordinates */
	ILI9341_x = x;
	ILI9341_y = y;
	
	while (*str) {
		/* New line */
		if (*str == '\n') {
			ILI9341_y += font->FontHeight + 1;
			/* if after \n is also \r, than go to the left of the screen */
			if (*(str + 1) == '\r') {
				ILI9341_x = 0;
				str++;
			} else {
				ILI9341_x = startX;
			}
			str++;
			continue;
		} else if (*str == '\r') {
			str++;
			continue;
		}
		if(ILI9341_x > ILI9341_Opts.width - font->FontWidth)
		{  
			//on passe � la ligne suivante, en s'alignant sous le d�but de la premi�re ligne
			ILI9341_y += font->FontHeight + 1;
			ILI9341_x = startX;
		}
		/* Put character to LCD */
		ILI9341_Putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
	}
}

/*
 *	permet l'affichage de "gros caract�res" ('bigger' fois plus gros que la 'font' fournie)
 *	full_in_bigger d�fini le taux de remplissage des gros pixels g�n�r�s. Testez pour comprendre et appr�cier le style !
 *	@pre : full_in_bigger doit �tre inf�rieur ou �gal � bigger.
 *	exemples :
 *		ILI9341_PutBigs(20, 40, "tst4,3",&Font_16x26, ILI9341_COLOR_BLUE,ILI9341_COLOR_BLACK, 4, 3);
 *	 	ILI9341_PutBigs(20, 40, "test2,2",&Font_16x26, ILI9341_COLOR_BLUE,ILI9341_COLOR_BLACK, 2, 2);
 */
void ILI9341_PutBigs(uint16_t x, uint16_t y, char *str, FontDef_t *font, uint16_t foreground, uint16_t background, uint8_t bigger, uint8_t full_in_bigger) {
	uint16_t startX = x;

	/* Set X and Y coordinates */
	ILI9341_x = x;
	ILI9341_y = y;

	while (*str) {
		/* New line */
		if (*str == '\n') {
			ILI9341_y += font->FontHeight*bigger + bigger;
			/* if after \n is also \r, than go to the left of the screen */
			if (*(str + 1) == '\r') {
				ILI9341_x = 0;
				str++;
			} else {
				ILI9341_x = startX;
			}
			str++;
			continue;
		} else if (*str == '\r') {
			str++;
			continue;
		}
		if(ILI9341_x > ILI9341_Opts.width - font->FontWidth*bigger)
		{
			//on passe � la ligne suivante, en s'alignant sous le d�but de la premi�re ligne
			ILI9341_y += font->FontHeight*bigger + bigger;
			ILI9341_x = startX;
		}
		/* Put character to LCD */
		ILI9341_PutBigc(ILI9341_x, ILI9341_y, *str++, font, foreground, background, bigger, full_in_bigger);
	}
}

/**
 * @brief  Gets width and height of box with text
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref FontDef_t used font
 * @param  *width: Pointer to variable to store width
 * @param  *height: Pointer to variable to store height
 * @retval None
 */
void ILI9341_GetStringSize(char *str, FontDef_t *font, uint16_t *width, uint16_t *height) {
	uint16_t w = 0;
	*height = font->FontHeight;
	while (*str++) {
		w += font->FontWidth;
	}
	*width = w;
}

/**
 * @brief  Puts single character to LCD
 * @param  x: X position of top left corner
 * @param  y: Y position of top left corner
 * @param  c: Character to be displayed
 * @param  *font: Pointer to @ref FontDef_t used font
 * @param  foreground: Color for char
 * @param  background: Color for char background
 * @retval None
 */
void ILI9341_Putc(uint16_t x, uint16_t y, char c, FontDef_t *font, uint16_t foreground, uint16_t background) {
	uint32_t i, b, j;
	/* Set coordinates */
	ILI9341_x = x;
	ILI9341_y = y;
	
	if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.width) {
		/* If at the end of a line of display, go to new line and set x to 0 position */
		ILI9341_y += font->FontHeight;
		ILI9341_x = 0;
	}
	
	/* Draw rectangle for background */
	ILI9341_INT_Fill(ILI9341_x, ILI9341_y, ILI9341_x + font->FontWidth, ILI9341_y + font->FontHeight, background);
	

	/* Draw font data */
	for (i = 0; i < font->FontHeight; i++) {


		if(font->datasize == 1)
		{
			uint8_t * data;
			data = (uint8_t *)font->data;
			b = (uint32_t)(data[(c - 32) * font->FontHeight + i]) << 8;
		}
		else if(font->datasize == 2)
		{
			uint16_t * data;
			data = (uint16_t *)font->data;
			b = data[(c - 32) * font->FontHeight + i];
		}
		else
			b = 0;	//should never happen
		for (j = 0; j < font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
			}
		}
	}
	
	/* Set new pointer */
	ILI9341_x += font->FontWidth;
}


/*
 * bigger : amplification de la taille du caract�re
 * full_in_bigger : sorte de "pourcentage de remplissage" : mettre la m�me valeur que bigger... ou moins (essayez pour voir si �a vous plait)
 */
void ILI9341_PutBigc(uint16_t x, uint16_t y, char c, FontDef_t *font, uint16_t foreground, uint16_t background, uint8_t bigger, uint8_t full_in_bigger) {
	uint32_t i, b, j;
	/* Set coordinates */
	ILI9341_x = x;
	ILI9341_y = y;

	if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.width) {
		/* If at the end of a line of display, go to new line and set x to 0 position */
		ILI9341_y += font->FontHeight;
		ILI9341_x = 0;
	}

	/* Draw rectangle for background */
	ILI9341_INT_Fill(ILI9341_x, ILI9341_y, ILI9341_x + bigger*font->FontWidth, ILI9341_y + bigger*font->FontHeight, background);


	/* Draw font data */
	for (i = 0; i < font->FontHeight; i++) {


		if(font->datasize == 1)
		{
			uint8_t * data;
			data = (uint8_t *)font->data;
			b = (uint32_t)(data[(c - 32) * font->FontHeight + i]) << 8;
		}
		else if(font->datasize == 2)
		{
			uint16_t * data;
			data = (uint16_t *)font->data;
			b = data[(c - 32) * font->FontHeight + i];
		}
		else
			b = 0;	//should never happen

		for (j = 0; j < font->FontWidth; j++)
		{
			if ((b << j) & 0x8000)
			{
				uint32_t k, l;

				for(k=0;k<full_in_bigger;k++)
					for(l=0;l<full_in_bigger;l++)
						ILI9341_DrawPixel(ILI9341_x + bigger*j+l, (ILI9341_y + bigger*i+k), foreground);
			}
		}
	}

	/* Set new pointer */
	ILI9341_x += font->FontWidth*bigger;
}


/**
 * @brief  Draws line to LCD
 * @param  x0: X coordinate of starting point
 * @param  y0: Y coordinate of starting point
 * @param  x1: X coordinate of ending point
 * @param  y1: Y coordinate of ending point
 * @param  color: Line color
 * @retval None
 */
void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    //WindowMax();
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;


    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0) {        /* vertical line */
        if (y1 > y0) ILI9341_INT_Fill(x0, y0, x0, y1, color);
        else ILI9341_INT_Fill(x0, y1, x0, y0, color);
        return;
    }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0) {        /* horizontal line */
        if (x1 > x0) ILI9341_INT_Fill(x0, y0, x1, y0, color);
        else  ILI9341_INT_Fill(x1, y0, x0, y0, color);
        return;
    }

    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

        	ILI9341_DrawPixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        ILI9341_DrawPixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
        	ILI9341_DrawPixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        ILI9341_DrawPixel(x0, y0, color);
    }
    return;
}

/**
 * @brief  Draws rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	ILI9341_DrawLine(x0, y0, x1, y0, color); //Top
	ILI9341_DrawLine(x0, y0, x0, y1, color);	//Left
	ILI9341_DrawLine(x1, y0, x1, y1, color);	//Right
	ILI9341_DrawLine(x0, y1, x1, y1, color);	//Bottom
}

/**
 * @brief  Draws filled rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t tmp;
	
	/* Check correction */
	if (x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}
	
	/* Fill rectangle */
	ILI9341_INT_Fill(x0, y0, x1, y1, color);
	
	/* CS HIGH back */
	ILI9341_CS_SET();
}

/**
 * @brief  Draws circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawPixel(x0 + x, y0 + y, color);
        ILI9341_DrawPixel(x0 - x, y0 + y, color);
        ILI9341_DrawPixel(x0 + x, y0 - y, color);
        ILI9341_DrawPixel(x0 - x, y0 - y, color);

        ILI9341_DrawPixel(x0 + y, y0 + x, color);
        ILI9341_DrawPixel(x0 - y, y0 + x, color);
        ILI9341_DrawPixel(x0 + y, y0 - x, color);
        ILI9341_DrawPixel(x0 - y, y0 - x, color);
    }
}

/**
 * @brief  Draws filled circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);
    ILI9341_DrawLine(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        ILI9341_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

        ILI9341_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
        ILI9341_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
}

/**
 * @brief  Returns ILI9341 options
 * @retval ILI9341_Opts : options
 */

ILI931_Options_t ILI9341_getOptions(void){
	return ILI9341_Opts;
}

/**
 * @brief  Puts formated string to LCD
 * @param  x: X position of top left corner of first character in string
 * @param  y: Y position of top left corner of first character in string
 * @param  *font: Pointer to @ref FontDef_t used font
 * @param  foreground: Color for string
 * @param  background: Color for string background
 * @param  format : Formated string like printf
 * @retval None
 */
void ILI9341_printf(int16_t x, int16_t y, FontDef_t *font, int16_t foreground, int16_t background, const char *format, ...)
{
	char buffer[256];

	va_list args_list;
	va_start(args_list, format);
	vsnprintf(buffer, 256, format, args_list);
	va_end(args_list);

	ILI9341_Puts(x, y, buffer, font, foreground, background);
}

/**
 * @brief  Put Image on LCD
 * @param  x0: X coordinate of starting point
 * @param  y0: Y coordinate of starting point
 * @param  width: width of image
 * @param  height: height of image
 * @param  img: Pointeur sur le tableau des pixels de l'image en BMP 565
 * @param  size: Nombre d'�l�ment dans le tableau
 * @retval None
 *
 * Astuce pour g�n�rer l'image :
 *  https://javl.github.io/image2cpp/
			Fournir une image en bmp

			Code output format : Plan bytes
			Draw mode : 2 bytes per pixel 565

			attention � la taille des images (la m�moire du microcontr�leur est vite remplie !!)
 */
void ILI9341_putImage(int16_t x0, int16_t y0, int16_t width, int16_t height, const int16_t *img, int32_t size){
	ILI9341_SetCursorPosition(x0, y0, x0 + width-1, y0 + height-1);

	uint8_t datas[2];

	/* Set command for GRAM data */
	ILI9341_SendCommand(ILI9341_GRAM);

	/* Send everything */
	ILI9341_CS_RESET();
	ILI9341_WRX_SET();

	/* Go to 16-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_16b);

#ifndef LCD_DMA
	int32_t i;
	for(i=0; i < size; i++){
		datas[1] = HIGHINT(img[i]);
		datas[0] = LOWINT(img[i]);
		SPI_WriteMultiNoRegister(ILI9341_SPI, datas, 1);
		//SPI_WriteNoRegister(ILI9341_SPI,0x07E0);
		//SPI_Write(img[i]);
	}
#else
	SPI2_DMA_send16BitArray((Uint16 *)img, size);
#endif


	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_8b);
	ILI9341_CS_SET();
}

#ifndef LCD_DMA
//size : nombre de pixel (1 pixel = 1 bit !)
//img : tableau contenant une image en monochrome, 1 octet = 8 pixels ! (bit de poids fort = premier pixel)
//    tout bit � 1 est du blanc. tout bit � 0 est du noir.
//La taille (width et height) doit �tre multiple de 8 !
void ILI9341_putImage_monochrome(uint16_t color_front, uint16_t color_background, int16_t x0, int16_t y0, int16_t width, int16_t height, const uint8_t *img, int32_t size)
{
	ILI9341_SetCursorPosition(x0, y0, x0 + width-1, y0 + height-1);

	uint8_t datas_front[2];
	uint8_t datas_background[2];
	datas_front[1] = HIGHINT(color_front);
	datas_front[0] = LOWINT(color_front);
	datas_background[1] = HIGHINT(color_background);
	datas_background[0] = LOWINT(color_background);
	uint8_t * datas;
	/* Set command for GRAM data */
	ILI9341_SendCommand(ILI9341_GRAM);

	/* Send everything */
	ILI9341_CS_RESET();
	ILI9341_WRX_SET();

	/* Go to 16-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_16b);


	int32_t i;
	for(i=0; i < size; i++){
		if((img[i/8]>>(7-i%8))&1)	//Hooo, la belle ligne. Prenez 5mn pour comprendre ce qui est fait ici ^^
			datas = datas_background;
		else
			datas = datas_front;

		SPI_WriteMultiNoRegister(ILI9341_SPI, datas, 1);
	}


	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_8b);
}
#endif	//ndef LCD_DMA

#endif
