/*
 * config.h
 *
 *  Created on: 31 mars 2016
 *      Author: Nirgal
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "stm32f1xx_hal.h"

//Choix de la cible utilisée.
#define NUCLEO	0
#define BLUEPILL 1

#define MY_BLUEPILL_IS_COUNTERFEIT	BLUEPILL

#define TRACE	0		//permet le débogage sur un simulateur QEMU. Doit rester à 0 par défaut.


//_______________________________________________________
//Configuration des broches utilisées...
//Ces macros permettent d'utiliser dans le code des noms explicites (LED_GREEN, ...)
#if NUCLEO
	#define LED_GREEN_GPIO		GPIOA
	#define LED_GREEN_PIN		GPIO_PIN_5
	#define BLUE_BUTTON_GPIO	GPIOC
	#define BLUE_BUTTON_PIN 	GPIO_PIN_13
	#if BLUEPILL
		#error "Vous ne pouvez pas définir à la fois NUCLEO et BLUEPILL !"
	#endif
#endif
#if BLUEPILL
	#define LED_GREEN_GPIO		GPIOC
	#define LED_GREEN_PIN		GPIO_PIN_13
	#define BLUE_BUTTON_GPIO	GPIOA
	#define BLUE_BUTTON_PIN 	GPIO_PIN_15		//Pas de bouton sur la bluepill, choisissez le port souhaité si vous en reliez un.
#endif

//Choisir les broches pour l'UART1, parmi ces deux possibilités :
#define UART1_ON_PB6_PB7
//#define UART1_ON_PA9_PA10

//Choisir les broches pour l'UART2, parmi ces deux possibilités :
#define UART2_ON_PA2_PA3
//#define UART2_ON_PD5_PD6

//Choisir les broches pour l'UART3, parmi ces deux possibilités :
#define UART3_ON_PB10_PB11
//#define UART3_ON_PD8_PD9


//_______________________________________________________


//Ci dessous, les defines permettant d'activer les modules de la librairie.
//Attention, s'ils sont tous activés, le code compilé est trop volumineux pour rentrer dans la mémoire de 64kio du STM32F103C8T6 la Bluepill.
//Le STM32F103RBT6 de la carte Nucleo dispose quant à lui d'une mémoire de 128kio de flash.

#define USE_BSP_EXTIT			1 //Utilisation du module de gestion des it externe : stm32f1_extit.c/h

#define USE_INTERNAL_FLASH_AS_EEPROM	0

#define USE_BSP_TIMER			1 //Utilisation de stm32f1_timer.c/h


#define USE_ADC					1
	//Ces configurations permettent d'activer les entrées analogiques souhaitées.
	//16 entrées analogiques peuvent être activées maximum.
	//2 entrées analogiques doivent être activées minimum. (Vref est un choix possible si vous utilisez une seule entrée)
	#define USE_AN0			1	//Broche correspondante : PA0
	#define USE_AN1			1	//Broche correspondante : PA1
	#define USE_AN2			0	//Broche correspondante : PA2	//Sur la Nucleo, cette broche n'est pas câblée !
	#define USE_AN3			0	//Broche correspondante : PA3	//Sur la Nucleo, cette broche n'est pas câblée !
	#define USE_AN4			0	//Broche correspondante : PA4
	#define USE_AN5			0	//Broche correspondante : PA5
	#define USE_AN6			0	//Broche correspondante : PA6
	#define USE_AN7			0	//Broche correspondante : PA7
	#define USE_AN8			1	//Broche correspondante : PB0
	#define USE_AN9			0	//Broche correspondante : PB1
	#define USE_AN10		0	//Broche correspondante : PC0	//Sur la Bluepill, cette broche n'est pas câblée !
	#define USE_AN11		1	//Broche correspondante : PC1	//Sur la Bluepill, cette broche n'est pas câblée !
	#define USE_AN12		0	//Broche correspondante : PC2	//Sur la Bluepill, cette broche n'est pas câblée !
	#define USE_AN13		0	//Broche correspondante : PC3	//Sur la Bluepill, cette broche n'est pas câblée !
	#define USE_AN14		0	//Broche correspondante : PC4	//Sur la Bluepill, cette broche n'est pas câblée !
	#define USE_AN15		0	//Broche correspondante : PC5	//Sur la Bluepill, cette broche n'est pas câblée !
	#define USE_AN16		0	//Capteur de température interne
	#define USE_AN17		1	//Vref


#define USE_SCREEN_TFT_ILI9341		1	//Ecran TFT 2.4
	#if USE_SCREEN_TFT_ILI9341
		#define USE_XPT2046			1	//Tactile

		#define USE_FONT11x18		0
		#define USE_FONT7x10		1
		#define USE_FONT16x26		0

		#if BLUEPILL					//Définition des broches CS, WRX et RST pour la BLUEPILL
			#define ILI9341_CS_PORT       GPIOB
			#define ILI9341_CS_PIN        GPIO_PIN_11

			#define ILI9341_WRX_PORT      GPIOB
			#define ILI9341_WRX_PIN       GPIO_PIN_1

			#define ILI9341_RST_PORT      GPIOB
			#define ILI9341_RST_PIN       GPIO_PIN_10
		#endif /* BLUEPILL */
	#endif


#define USE_SCREEN_LCD2X16			0	//Ecran LCD 2x16

#define USE_TFT_PCD8544				0	//Ecran TFT Nokia

//Carte capteur IKS01A1 :
#define USE_SENSOR_HTS221		0//Humidité et température
#define USE_SENSOR_LPS25HB		0//Pression (et température)
#define USE_SENSOR_LSM6DS0		0//Accéléromètre et Gyroscope
#define USE_SENSOR_LIS3MDL		0//Magnetometre

//Autres capteurs :
#define USE_SENSOR_LSM6DS3		0//Accéléromètre et Gyroscope
#define USE_SENSOR_LPS22HB		0//Pression (et température)
#define USE_MLX90614			0	//Capteur de température sans contact
#define USE_MPU6050				1//Accéléromètre et Gyroscope
#define USE_DHT11				0

#define USE_MATRIX_KEYBOARD		0
#define USE_MATRIX_LED			0
#define USE_CAPACITIVE_KEYBOARD	0
#define USE_MATRIX_LED_32_32	0
#define USE_FINGERPRINT			0

#define USE_MCP23S17			0 //Port expander I/O en SPI
#define USE_MCP23017			0 //Port expander I/O en I2C
#define USE_APDS9960			0	//Capteur de gestes, de couleur, de luminosité ambiante
#define USE_BH1750FVI			0	//Capteur de luminosité ambiante
#define USE_BMP180				0	//Capteur de pression
#define USE_MOTOR_DC			0
#define USE_STEPPER_MOTOR		0
#define USE_RTC					0
#define USE_PWM					0
#define USE_ESP8266				0//Module Wifi
#define USE_NFC03A1				0
#define USE_EPAPER				0
#define	USE_GPS					0
#define USE_HCSR04				0
#define USE_HX711				0
#define USE_DAC_MCP4921			0
#define USE_IR_EMITTER			0
#define USE_IR_RECEIVER			0
#define USE_IR_RECEIVER_NEC		0
#define USE_DS18B20				0
#define USE_MIDI				0
#define	USE_VL53L0				0
#if USE_VL53L0
	#define VL53L0X_I2C				I2C1
	#define VL53_NB					1
	//Vous devez définir ci dessous autant de lignes VL_x que le nombre indiqué ci-dessus avec x allant de 0 à VL53_NB-1
	#define VL_0					GPIOA, GPIO_PIN_0
	//#define VL_1					GPIOD, GPIO_PIN_3
	//#define VL_2					GPIOE, GPIO_PIN_10
#endif

#if USE_EPAPER
	#define RST_Pin 			GPIO_PIN_1
	#define RST_GPIO_Port 		GPIOA
	#define DC_Pin 				GPIO_PIN_1
	#define DC_GPIO_Port 		GPIOC
	#define BUSY_Pin 			GPIO_PIN_0
	#define BUSY_GPIO_Port 		GPIOB
	#define SPI_CS_Pin 			GPIO_PIN_4
	#define SPI_CS_GPIO_Port 	GPIOA
	#define EPAPER_SPI			SPI1
#endif

#define USE_SD_CARD				0	//Utilisation d'une carte SD pour lire/écrire des fichiers

#define USE_DIALOG				0	//Module logiciel permettant le dialogue entre plusieurs entités selon unn protocole maison générique.

//Liste des modules utilisant le périphérique I2C
#if USE_MLX90614 || USE_MPU6050	|| USE_APDS9960	 || USE_BH1750FVI || USE_BMP180 || USE_MCP23017 || USE_VL53L0
	#define USE_I2C				1
#endif
#define I2C_TIMEOUT				5	//ms

#define I2C1_ON_PB6_PB7			0

//Liste des modules utilisant le périphérique SPI
#if USE_SCREEN_TFT_ILI9341	|| USE_SD_CARD
	#define USE_SPI				1
	#define SPI1_ON_PA5_PA6_PA7		1
	#define SPI1_ON_PB3_PB4_PB5		0
#endif

#if USE_IR_EMITTER || USE_MOTOR_DC
	#undef USE_PWM
	#define USE_PWM		1
#endif

//Liste des modules utilisant le périphérique timer
#if USE_HCSR04 || USE_ADC || USE_PWM || USE_IR_EMITTER || USE_IR_RECEIVER || USE_VL53L0
	#undef USE_BSP_TIMER
	#define USE_BSP_TIMER		1
#endif



#endif /* CONFIG_H_ */
