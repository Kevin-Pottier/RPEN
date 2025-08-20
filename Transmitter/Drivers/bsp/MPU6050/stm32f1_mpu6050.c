/*
 * stm32f1_mpu6050.c
 *
 *  Created on: 27 avr. 2016
 *      Author: Tilen Majerle (2014) (cf license ci dessous)
 *      		Samuel Poiraud (2016) -> portage STM32F1 et modifications pour les activit√©s p√©dagogiques √† l'ESEO.
 */

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


/*
 * Ce driver fourni pour l'accÈlÈromËtre-gyroscope MPU6050 fonctionne aussi PARTIELLEMENT avec le MPU9250. (fonctionnement non garanti).
 * Brochage proposÈ par dÈfaut :
 *
 *  * Par dÈfaut, les broches utilisÈes sont celles de l'I2C1 :
 *  		- SCL : PB8
 *  		- SDA : PB9
 *  		- Vcc : PA0 par exemple (cf MPU6050_Init) !!! (ce qui permet au pilote de couper l'alimentation et provoquer un reset).
 *  		- GND : 0V
 *
 * Le basculement sur l'I2C2 est possible en redÈfinissant MPU6050_I2C ‡ I2C2 : (PB10 pour SCL et PB11 pour SDA)
 *
 */
#include "config.h"
#if USE_MPU6050
#include "stm32f1xx_hal.h"
#include "stm32f1_mpu6050.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"


/**
 * @brief  Classic use of the MPU6050. Might be taken as an exmaple of how to use it
 * @param  none
 * @retval none
 */
void MPU6050_demo(void)
{
	MPU6050_t MPU6050_Data;
	int32_t gyro_x = 0;
	int32_t gyro_y = 0;
	int32_t gyro_z = 0;
	/* Initialize MPU6050 sensor */
		if (MPU6050_Init(&MPU6050_Data, GPIOA, GPIO_PIN_0, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok) {
			/* Display error to user */
			debug_printf("MPU6050 Error\n");

			/* Infinite loop */
			while (1);
		}

		while (1) {
			/* Read all data from sensor */
			MPU6050_ReadAll(&MPU6050_Data);

			gyro_x += MPU6050_Data.Gyroscope_X;
			gyro_y += MPU6050_Data.Gyroscope_Y;
			gyro_z += MPU6050_Data.Gyroscope_Z;
			debug_printf("AX%4d\t AY%4d\t AZ%4d\t GX%4d\t GY%4d\t GZ%4d\t gx%4ld∞\t gy%4ld∞\t gz%4ld∞\t T%3.1f∞\n",
							MPU6050_Data.Accelerometer_X/410,	//environ en %
							MPU6050_Data.Accelerometer_Y/410,	//environ en %
							MPU6050_Data.Accelerometer_Z/410,	//environ en %
							MPU6050_Data.Gyroscope_X,
							MPU6050_Data.Gyroscope_Y,
							MPU6050_Data.Gyroscope_Z,
							gyro_x/16400,						//environ en ∞
							gyro_y/16400,						//environ en ∞
							gyro_z/16400,						//environ en ∞
							MPU6050_Data.Temperature);

			/* Little delay */
			HAL_Delay(500);
		}
}

/*
 * @brief	Initialise le module MPU6050 en activant son alimentation, puis en configurant les registres internes du MPU6050.
 * @param	GPIOx et GPIO_PIN_x indiquent la broche o√π l'on a reli√© l'alimentation Vcc du MPU6050.
 * 			Indiquer NULL dans GPIOx s'il est aliment√© en direct.
 * 			Cette possibilit√© d'alimentation par la broche permet le reset du module par le microcontr√¥leur.
 * @param	DataStruct : fournir le pointeur vers une structure qui sera √† conserver pour les autres appels des fonctions de ce module logiciel.
 * @param 	DeviceNumber : 					voir MPU6050_Device_t
 * @param	AccelerometerSensitivity : 		voir MPU6050_Accelerometer_t
 * @param	GyroscopeSensitivity :			voir MPU6050_Gyroscope_t
 */
MPU6050_Result_t MPU6050_Init(MPU6050_t* DataStruct, GPIO_TypeDef * GPIOx, uint16_t GPIO_PIN_x, MPU6050_Device_t DeviceNumber, MPU6050_Accelerometer_t AccelerometerSensitivity, MPU6050_Gyroscope_t GyroscopeSensitivity)
{
	uint8_t temp;

	if(GPIOx != NULL)
	{
		BSP_GPIO_PinCfg(GPIOx, GPIO_PIN_x,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
		HAL_GPIO_WritePin(GPIOx,GPIO_PIN_x,GPIO_PIN_SET);
	}
	HAL_Delay(20);
	/* Format I2C address */
	DataStruct->Address = MPU6050_I2C_ADDR | (uint8_t)DeviceNumber;

	/* Initialize I2C */
	I2C_Init(MPU6050_I2C, MPU6050_I2C_CLOCK);

	/* Check if device is connected */
	if (!I2C_IsDeviceConnected(MPU6050_I2C, DataStruct->Address)) {
		/* Return error */
		return MPU6050_Result_DeviceNotConnected;
	}

	/* Check who I am */
	uint8_t i_am;
	I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_WHO_AM_I, &i_am);
	if (i_am != MPU6050_I_AM && i_am != MPU9250_I_AM && i_am != MPU9255_I_AM && i_am != MPU6060_I_AM_STRANGE_MODEL) {
		/* Return error */
		return MPU6050_Result_DeviceInvalid;
	}

	/* Wakeup MPU6050 */
	I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_PWR_MGMT_1, 0x00);

	/* Config accelerometer */
	I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_CONFIG, &temp);
	temp = (temp & 0xE7) | (uint8_t)AccelerometerSensitivity << 3;
	I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_CONFIG, temp);

	/* Config gyroscope */
	I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_CONFIG, &temp);
	temp = (temp & 0xE7) | (uint8_t)GyroscopeSensitivity << 3;
	I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_CONFIG, temp);

	/* Set sensitivities for multiplying gyro and accelerometer data */
	switch (AccelerometerSensitivity) {
		case MPU6050_Accelerometer_2G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_2;
			break;
		case MPU6050_Accelerometer_4G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_4;
			break;
		case MPU6050_Accelerometer_8G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_8;
			break;
		case MPU6050_Accelerometer_16G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_16;
			//no break
		default:
			break;
	}

	switch (GyroscopeSensitivity) {
		case MPU6050_Gyroscope_250s:
			DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_250;
			break;
		case MPU6050_Gyroscope_500s:
			DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_500;
			break;
		case MPU6050_Gyroscope_1000s:
			DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_1000;
			break;
		case MPU6050_Gyroscope_2000s:
			DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_2000;
			// no break
		default:
			break;
	}

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result_t MPU6050_ReadAccelerometer(MPU6050_t* DataStruct) {
	uint8_t data[6];

	/* Read accelerometer data */
	I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_XOUT_H, data, 6);

	/* Format */
	DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result_t MPU6050_ReadGyroscope(MPU6050_t* DataStruct) {
	uint8_t data[6];

	/* Read gyroscope data */
	I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_XOUT_H, data, 6);

	/* Format */
	DataStruct->Gyroscope_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Gyroscope_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Gyroscope_Z = (int16_t)(data[4] << 8 | data[5]);

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result_t MPU6050_ReadTemperature(MPU6050_t* DataStruct) {
	uint8_t data[2];
	int16_t temp;

	/* Read temperature */
	I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_TEMP_OUT_H, data, 2);

	/* Format temperature */
	temp = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result_t MPU6050_ReadAll(MPU6050_t* DataStruct) {
	uint8_t data[14];
	int16_t temp;

	/* Read full raw data, 14bytes */
	I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_XOUT_H, data, 14);

	/* Format accelerometer data */
	DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

	/* Format temperature */
	temp = (int16_t)(data[6] << 8 | data[7]);
	DataStruct->Temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);

	/* Format gyroscope data */
	DataStruct->Gyroscope_X = (int16_t)(data[8] << 8 | data[9]);
	DataStruct->Gyroscope_Y = (int16_t)(data[10] << 8 | data[11]);
	DataStruct->Gyroscope_Z = (int16_t)(data[12] << 8 | data[13]);

	/* Return OK */
	return MPU6050_Result_Ok;
}
#endif
