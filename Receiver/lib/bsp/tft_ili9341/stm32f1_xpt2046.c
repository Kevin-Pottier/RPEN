/*
 * stm32f1_xpt2046.c
 *
 *  Created on: Nov 17, 2016
 *      Author: louisz
 */
/*
 * This library allows to use the XPT2046 touch part of the TFT screen with any STM32F1 and is independent of ILI9341 library.
 *
 * The module uses the SPI bus and the pins are imposed (fitting power supply and GND must have been chosen aswell):
 *
 *  T_IRQ : PB3 : IRQ pin of the touch screen used to notify when screen is touched or released (currently not used)
 *  T_OUT : PA6 : Output from screen to SPI (MISO)
 *  T_IN  : PA7 : Input from SPI master (MOSI)
 *  T_CS  : PA11: Chip select for SPI
 *  T_CLK : PA5 : SPI Clock
 *  VCC   : 5V  : Power supply
 *  GND   : GND : Ground
 *
 *  How to use the XPT2046 touchscreen library:
 *
 *  - Make sure the USE_XPT2046 in config.h is correctly defined at 1
 *  - Initialise the library (init function)
 *  - Get the data from screen using getCoordinate or getAverageCoordinate (getAverageCoordinate is recommended for better precision)
 *
 */
#include "config.h"
#if USE_XPT2046
#include <stdarg.h>

#include "stm32f1_xpt2046.h"
#include "stm32f1xx_hal.h"
#include "stm32f1_spi.h"
#include "stm32f1_ili9341.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"


// D√©finition du bit de d√©part (S n7)
#define CONTROL_BYTE_START									0b10000000
#define CONTROL_BYTE_NO_START								0b00000000

//Diff√©rents bits de selection des channel (A2-A0 bits 6-4)
// Selection byte pour la temp0 (controle temp√©rature)
#define CONTROL_BYTE_CHANNEL_SELECT_TEMP0					0b00000000
// Other Temp input
#define CONTROL_BYTE_CHANNEL_SELECT_TEMP1					0b01110000
// Selection byte pour Y
#define CONTROL_BYTE_CHANNEL_SELECT_Y						0b00010000
// X
#define CONTROL_BYTE_CHANNEL_SELECT_X						0b01010000
// Cross panel measurements
#define CONTROL_BYTE_CHANNEL_SELECT_Z1						0b00110000
#define CONTROL_BYTE_CHANNEL_SELECT_Z2						0b01000000
// Selection batery monitor input byte
#define CONTROL_BYTE_CHANNEL_SELECT_VBAT					0b00100000
// Aux input
#define CONTROL_BYTE_CHANNEL_SELECT_AUX						0b01100000

//Choix du mode de conversion (bit MODE n3)
#define CONTROL_BYTE_MODE_12_BIT							0b00000000
#define CONTROL_BYTE_MODE_8_BIT								0b00001000

// Choix du mode Single Ended ou differential (bit SER/DFR n2)
#define CONTROL_BYTE_SD_DIFFERENTIAL						0b00000000
#define CONTROL_BYTE_SD_SINGLE_ENDED						0b00000100

// Power Down control mode select (bits PD1 et PD0 n1 et 0)
#define CONTROL_BYTE_POWER_DOWN_MODE_LOW_POWER_IRQ			0b00000000
#define CONTROL_BYTE_POWER_DOWN_MODE_REF_ON_ADC_OFF_NO_IRQ	0b00000001
#define CONTROL_BYTE_POWER_DOWN_MODE_REF_OFF_ADC_ON_IRQ		0b00000010
#define CONTROL_BYTE_POWER_DOWN_MODE_FULL_POWER_NO_IRQ		0b00000011

// Type control byte
typedef Uint8 controlByte_t;



#define XPT2046_CS_SET()			HAL_GPIO_WritePin(PIN_CS_TOUCH,GPIO_PIN_SET)
#define XPT2046_CS_RESET()			HAL_GPIO_WritePin(PIN_CS_TOUCH,GPIO_PIN_RESET)

static Uint16 XPT2046_getReading(controlByte_t controlByte);
static void XPT2046_convertCoordinateScreenMode(Sint16 * pX, Sint16 * pY);


/*
 * Cette fonction blocante a pour but de vous aider ‡ apprÈhender les fonctionnalitÈs de ce module logiciel.
 */
void XPT2046_demo(void)
{
	ILI9341_Init();	//initialisation de l'Ècran TFT
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_DrawCircle(20,20,5,ILI9341_COLOR_BLUE);
	ILI9341_DrawLine(20,20,100,20,ILI9341_COLOR_RED);
	ILI9341_DrawLine(20,20,20,100,ILI9341_COLOR_RED);

	#if USE_FONT7x10
		ILI9341_Putc(110,11,'x',&Font_7x10,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
		ILI9341_Putc(15,110,'y',&Font_7x10,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	#endif

	XPT2046_init();	//initialisation du tactile

	while(1)
	{
		static int16_t static_x,static_y;
		int16_t x, y;

		if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
		{
			ILI9341_DrawCircle(static_x,static_y,15,ILI9341_COLOR_WHITE);
			ILI9341_DrawCircle(x,y,15,ILI9341_COLOR_BLUE);
			static_x = x;
			static_y = y;
		}
	}
}




/**
 * @brief init function for XPT2046 lib
 */
void XPT2046_init(void){

	// Initialise SPI
	SPI_Init(XPT2046_SPI);
	BSP_GPIO_PinCfg(PIN_CS_TOUCH,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(PIN_IRQ_TOUCH,GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
	XPT2046_CS_SET();

	XPT2046_getReading(CONTROL_BYTE_START
					   | CONTROL_BYTE_CHANNEL_SELECT_X
					   | CONTROL_BYTE_MODE_12_BIT
					   | CONTROL_BYTE_SD_DIFFERENTIAL
					   | CONTROL_BYTE_POWER_DOWN_MODE_LOW_POWER_IRQ);
}

/*
 * @brief Reconfigure SPI speed (currently not working)
 */
void XPT2046_setConfig(void){
	SPI_setBaudRate(XPT2046_SPI, SPI_BAUDRATEPRESCALER_256);
}

/*
 * @brief function to get coordinate on a chosen coordinate mode.
 * @param *pX : pointer to get the X coordinate
 * @param *pY : pointer to get the Y coordinate
 * @param coordinateMode : either XPT2046_COORDINATE_RAW or XPT2046_COORDINATE_SCREEN_RELATIVE
 * @return a boolean that shows whether or not the screen has been touched (and the coordinate returned is valid)
 */
bool_e XPT2046_getCoordinates(Sint16 * pX, Sint16 * pY, XPT2046_coordinateMode_e coordinateMode){
	Uint8 i, j;
	Sint16 allX[7] , allY[7];
	bool_e ret;
	for (i=0; i < 7 ; i++){

		allY[i] = (Sint16)XPT2046_getReading(CONTROL_BYTE_START
										| CONTROL_BYTE_CHANNEL_SELECT_Y
										| CONTROL_BYTE_MODE_12_BIT
										| CONTROL_BYTE_SD_DIFFERENTIAL
										| CONTROL_BYTE_POWER_DOWN_MODE_LOW_POWER_IRQ);

		allX[i] = (Sint16)XPT2046_getReading(CONTROL_BYTE_START
										| CONTROL_BYTE_CHANNEL_SELECT_X
										| CONTROL_BYTE_MODE_12_BIT
										| CONTROL_BYTE_SD_DIFFERENTIAL
										| CONTROL_BYTE_POWER_DOWN_MODE_LOW_POWER_IRQ);
	}

	for (i=0; i < 4 ; i++){
		for (j=i; j < 7 ; j++) {
			Sint16 temp = allX[i];
			if(temp > allX[j]){
				allX[i] = allX[j];
				allX[j] = temp;
			}
			temp = allY[i];
			if(temp > allY[j]){
				allY[i] = allY[j];
				allY[j] = temp;
			}
		}
	}

#ifdef XPT2046_USE_PIN_IRQ_TO_CHECK_TOUCH
	if(!HAL_GPIO_ReadPin(PIN_IRQ_TOUCH))
		ret = TRUE;
	else
		ret =  FALSE;
#else
	if(allX[3] >= 4000 || allY[3] >= 4000 || allX[3] < 100 || allY[3] < 100)
		ret =  FALSE;
	else
		ret =  TRUE;
#endif

	if(coordinateMode == XPT2046_COORDINATE_SCREEN_RELATIVE)
		XPT2046_convertCoordinateScreenMode(&(allX[3]), &(allY[3]));

	*pX = allX[3];
	*pY = allY[3];

	return ret;
}

/*
 * @brief function to get average coordinate on a chosen coordinate mode on a chosen number of values.
 * @param *pX : pointer to get the X coordinate
 * @param *pY : pointer to get the Y coordinate
 * @param nSamples : number of samples used for each returned values
 * @param coordinateMode : either XPT2046_COORDINATE_RAW or XPT2046_COORDINATE_SCREEN_RELATIVE
 * @return a boolean that shows whether or not the screen has been touched (and the coordinate returned is valid)
 */
bool_e XPT2046_getAverageCoordinates(Sint16 * pX, Sint16 * pY, Uint8 nSamples, XPT2046_coordinateMode_e coordinateMode){
	Uint8 i = 0;
	Sint32 xAcc = 0 , yAcc = 0;
	Sint16 x , y;
	for(i=0; i<nSamples; i++)
	{
		if(XPT2046_getCoordinates(&x , &y, coordinateMode))
		{
			xAcc += x;
			yAcc += y;
		}
		else
			return FALSE;
	}

	if(i < nSamples)
		return FALSE;

	*pX = (Sint16)(xAcc / i);
	*pY = (Sint16)(yAcc / i);
	ILI931_Options_t screenOption = ILI9341_getOptions();
	if(screenOption.orientation == ILI9341_Orientation_Portrait_1 || screenOption.orientation == ILI9341_Orientation_Portrait_2)
	{
		if(*pX > 0 && *pX < 239 && *pY > 0 && *pY < 319)
			return TRUE;
	}
	else
	{
		if(*pX > 0 && *pX < 319 && *pY > 0 && *pY < 239)
			return TRUE;
	}
	return FALSE;
}


#define NB_POINTS_FOR_MEDIAN		8
/*
 * @brief function to get median coordinate on a chosen coordinate mode on a defined number of values.
 * @param *pX : pointer to get the X coordinate
 * @param *pY : pointer to get the Y coordinate
 * @param coordinateMode : either XPT2046_COORDINATE_RAW or XPT2046_COORDINATE_SCREEN_RELATIVE
 * @return a boolean that shows whether or not the screen has been touched (and the coordinate returned is valid)
 */
bool_e XPT2046_getMedianCoordinates(Sint16 * pX, Sint16 * pY, XPT2046_coordinateMode_e coordinateMode){
	Uint8 n = 0, i, j;
	typedef struct
	{
		Sint16 x;
		Sint16 y;
	}point_t;
	point_t tab[NB_POINTS_FOR_MEDIAN];
	point_t current;
	Sint16 index;

	index = 0;
	XPT2046_getCoordinates(&tab[0].x , &tab[0].y, coordinateMode);	//on place le premier point dans la premiËre case du tableau.


	for(n=1; n<NB_POINTS_FOR_MEDIAN; n++)
	{
		if(XPT2046_getCoordinates(&current.x , &current.y, coordinateMode))	//rÈcup d'un point
		{
			for(i=0; i<index; i++)		//parcours des valeurs plus faibles
			{
				if(current.x < tab[i].x)
					break;
			}
			for(j=(uint8_t)index; j>i; j--)	//dÈplacement des valeurs plus grandes de 1 case
			{
				tab[j] = tab[j-1];
			}
			tab[i] = current;	//Ècriture de la nouvelle valeur ‡ sa position

			index++;
		}
		else
			return FALSE;
	}

	*pX = tab[index/2].x;
	*pY = tab[index/2].y;
	ILI931_Options_t screenOption = ILI9341_getOptions();
	if(screenOption.orientation == ILI9341_Orientation_Portrait_1 || screenOption.orientation == ILI9341_Orientation_Portrait_2)
	{
		if(*pX > 0 && *pX < 239 && *pY > 0 && *pY < 319)
			return TRUE;
	}
	else
	{
		if(*pX > 0 && *pX < 319 && *pY > 0 && *pY < 239)
			return TRUE;
	}
	return FALSE;
}


/*
 * @brief private function used to read the SPI
 * @param controlByte controlByte used to read
 * @return data read from screen touch
 */
static Uint16 XPT2046_getReading(controlByte_t controlByte){

	Uint16 ret;

	XPT2046_CS_RESET();
	SPI_WriteNoRegister(XPT2046_SPI,controlByte);

	ret = (Uint16)((Uint16)(SPI_ReadNoRegister(XPT2046_SPI)) << 5);
	ret |= (Uint16)(SPI_ReadNoRegister(XPT2046_SPI) >> (Uint16)(3));

	XPT2046_CS_SET();

	return ret;
}

/*
 * @brief private function used to convert coordinates from Raw to ScreenMode
 * @param *pX X coordinate of the data
 * @param *pY Y coordinate of the data
 */
static void XPT2046_convertCoordinateScreenMode(Sint16 * pX, Sint16 * pY){
	ILI931_Options_t screenOption = ILI9341_getOptions();
	Sint32 tempX, tempY;
	tempX = (Sint32)*pX;
	tempY = (Sint32)*pY;

	switch(screenOption.orientation){
	case ILI9341_Orientation_Portrait_1 :
		*pX = (Sint16)((4096 - tempX) * (Sint32)screenOption.width / 4096);
		*pY = (Sint16)((4096 - tempY) * (Sint32)screenOption.height / 4096);
		break;

	case ILI9341_Orientation_Portrait_2 :
		*pX = (Sint16)(tempX * (Sint32)screenOption.width / 4096);
		*pY = (Sint16)(tempY * (Sint32)screenOption.height / 4096);
		break;

	case ILI9341_Orientation_Landscape_1 :
		*pX = (Sint16)((4096 - tempY) * (Sint32)screenOption.width / 4096);
		*pY = (Sint16)(tempX * (Sint32)screenOption.height / 4096);
		break;

	case ILI9341_Orientation_Landscape_2 :
		*pX = (Sint16)((tempY) * (Sint32)screenOption.width / 4096);
		*pY = (Sint16)((4096 - tempX) * (Sint32)screenOption.height / 4096);
		break;
	}
}

#endif

