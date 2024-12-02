/*
 * stm32f1_spi.c
 *
 *  Created on: 19 mai 2016
 *      Author: Nirgal
 */

#include "stm32f1_spi.h"
#include "stm32f1_gpio.h"
#include "config.h"

void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);

typedef enum
{
	SPI1_ID = 0,
	SPI2_ID,
	SPI_NB
}SPI_ID_e;

static SPI_HandleTypeDef  hSPI[SPI_NB];


/*
 * @brief Initialise le bus SPI
 * @param SPIx indique le SPI utilisé (SPI1 ou SPI2)
 */
void SPI_Init(SPI_TypeDef* SPIx)
{
	assert(SPIx == SPI1 || SPIx == SPI2);

	// SPI_ID = SPI2_ID si SPI2 sinon SPI1_ID
	SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);
	if(id == SPI1_ID)
	{
		//Enables the clocks and pin configuration
		__HAL_RCC_SPI1_CLK_ENABLE();

		#if SPI1_ON_PA5_PA6_PA7
			__HAL_RCC_GPIOA_CLK_ENABLE();
			BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_6, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
		#endif
		#if SPI1_ON_PB3_PB4_PB5
			__HAL_RCC_GPIOB_CLK_ENABLE();
			__HAL_AFIO_REMAP_SPI1_ENABLE();		//SPI1 sur PB3, PB4, PB5
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_4, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
		#endif
	}
	else
	{
		__HAL_RCC_SPI2_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		//SPI2 sur PB10 et PB11.
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_13, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_14, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
	}
	//Configuration of the SPIx module
	hSPI[id].Instance = SPIx;
	hSPI[id].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hSPI[id].Init.CLKPhase = SPI_PHASE_1EDGE;
	hSPI[id].Init.CLKPolarity = SPI_POLARITY_LOW;
	hSPI[id].Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hSPI[id].Init.CRCPolynomial = 0;
	hSPI[id].Init.DataSize = SPI_DATASIZE_8BIT;
	hSPI[id].Init.Direction = SPI_DIRECTION_2LINES;
	hSPI[id].Init.FirstBit = SPI_FIRSTBIT_MSB;
	hSPI[id].Init.Mode = SPI_MODE_MASTER;
	hSPI[id].Init.NSS = SPI_NSS_SOFT;			//Chip select must be manage by software.
	hSPI[id].Init.TIMode = SPI_TIMODE_DISABLE;
	hSPI[id].State = HAL_SPI_STATE_RESET;
	HAL_SPI_Init(&hSPI[id]);
}

/**
@brief Cette fonction permet de modifier "en live" le port de sortie du bus SPI1
@param SPI_remap : si TRUE, le SPI sort sur PB3 PB4 PB5, si FALSE le SPI sort sur PA5 PA6 PA7
@pre	pour utiliser les deux ports dans votre application, il faut avoir mis à 1 les deux defines SPI1_ON_PA5_PA6_PA7 et SPI1_ON_PB3_PB4_PB5
*/
void SPI_config_remap(bool_e SPI_remap)
{
	if(SPI_remap)
		__HAL_AFIO_REMAP_SPI1_ENABLE();		//SPI1 sur PB3, PB4, PB5
	else
		__HAL_AFIO_REMAP_SPI1_DISABLE();	//SPI1 sur PA5, PA6, PA7
}

/*
 * @brief Cette fonction sert à recevoir une donnée sur l'un des bus SPI.
 * @param SPIx est le SPI à lire.
 * @retval cette fonction retourne la donnée lue sur le SPI choisi.
 */
uint8_t SPI_ReadNoRegister(SPI_TypeDef* SPIx)
{
	uint8_t data;
	assert(SPIx == SPI1 || SPIx == SPI2);
	SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);
	data = 0x00;
	HAL_SPI_Receive(&hSPI[id],&data,1,100);
	return data;
}

/*
 * @brief Cette fonction sert à recevoir plusieurs données sur l'un des bus SPI.
 * @param SPIx est le SPI à lire.
 * @param *data est la variable qui va stocker les données lues.
 * @param count est le nombre de données à recevoir.
 */
void SPI_ReadMultiNoRegister(SPI_TypeDef* SPIx, uint8_t* data, uint16_t count)
{
	assert(SPIx == SPI1 || SPIx == SPI2);
	SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);
	HAL_SPI_Receive(&hSPI[id],data,count,100);
}

/*
 * @brief Cette fonction sert à envoyer une donnée sur l'un des bus SPI.
 * @param SPIx est le SPI sur lequel envoyer la donnée.
 * @param data est la donnée à envoyer.
 */
void SPI_WriteNoRegister(SPI_TypeDef* SPIx, uint8_t data)
{
	assert(SPIx == SPI1 || SPIx == SPI2);
	SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);
	HAL_SPI_Transmit(&hSPI[id],&data,1,100);
}

/*
 * @brief Cette fonction sert à envoyer plusieurs données sur l'un des bus SPI.
 * @param SPIx est le SPI sur lequel envoyer les données.
 * @param *data est la donnée à envoyer.
 * @param count est le nombre de données à envoyer.
 */
void SPI_WriteMultiNoRegister(SPI_TypeDef* SPIx, uint8_t* data, uint16_t count)
{
	assert(SPIx == SPI1 || SPIx == SPI2);
	SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);
	HAL_SPI_Transmit(&hSPI[id],data,count,100);
}


uint8_t SPI_WriteRead(SPI_TypeDef* SPIx, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t readvalue = 0;
  assert(SPIx == SPI1 || SPIx == SPI2);
  SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);
  status = HAL_SPI_TransmitReceive(&hSPI[id], (uint8_t*) &Value, (uint8_t*) &readvalue, 1, 100);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
   // SPIx_Error();
  }
  return readvalue;
}

/**
  * @brief  Write a byte on the SD.
  * @param  DataIn: byte to send.
  * @param  DataOut: byte to read
  * @param  DataLength: length of data
  */
void SPI_WriteReadBuffer(SPI_TypeDef* SPIx, const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  /* Send the byte */
	uint16_t i;
	for(i=0;i<DataLength;i++)
		DataOut[i] = (uint8_t)(SPI_WriteRead(SPIx, DataIn[i]));
}


/*
 * @brief Cette fonction sert à régler la taille d'une donnée.
 * @param SPIx le SPI dont on veut régler la taille des données.
 * @param DataSize est la taille des données :
 * 						TM_SPI_DataSize_8b  pour configurer le SPI en mode 8-bits
 * 						TM_SPI_DataSize_16b pour configurer le SPI en mode 16-bits
 */
TM_SPI_DataSize_t TM_SPI_SetDataSize(SPI_TypeDef* SPIx, TM_SPI_DataSize_t DataSize) {
	TM_SPI_DataSize_t status = (SPIx->CR1 & SPI_CR1_DFF) ? TM_SPI_DataSize_16b : TM_SPI_DataSize_8b;
	SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);

	/* Disable SPI first */
	SPIx->CR1 &= ~SPI_CR1_SPE;

	/* Set proper value */
	if (DataSize == TM_SPI_DataSize_16b) {
		hSPI[id].Init.DataSize = SPI_DATASIZE_16BIT;
		/* Set bit for frame */
		SPIx->CR1 |= SPI_CR1_DFF;
	} else {
		hSPI[id].Init.DataSize = SPI_DATASIZE_8BIT;
		/* Clear bit for frame */
		SPIx->CR1 &= ~SPI_CR1_DFF;
	}

	/* Enable SPI back */
	SPIx->CR1 |= SPI_CR1_SPE;

	/* Return status */
	return status;
}

/*
 * @author louisz (portage spi clubrobot)
 * @brief Permet d'envoyer une commande sur le bus SPI.
 * @param SPIx le SPI dont sur lequel on veut envoyer la commande.
 * @param NewState etat a envoyer comme commande au SPI
 */
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral */
	 SPIx->CR1 |= SPI_CR1_SPE;
  }
  else
  {
    /* Disable the selected SPI peripheral */
	 SPIx->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPE);
  }
}

/*
 * @author louisz (portage spi clubrobot)
 * @brief Permet de régler le baudRate d'un SPI.
 * @param SPIx le SPI dont on veut modifier le baudRate.
 * @param BaudRate choisi, voir SPI_BAUDRATEPRESCALER_x où x vaut 2, 4, 8, 16, 32, 64, 128, 256
 * @pre SPI_Init(SPIx) must be called before
 */
void SPI_setBaudRate(SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler)
{
	SPI_ID_e id = ((SPIx == SPI2)?SPI2_ID:SPI1_ID);
	SPI_Cmd(SPIx, DISABLE);
	hSPI[id].Init.BaudRatePrescaler = SPI_BaudRatePrescaler;
	HAL_SPI_Init(&hSPI[id]);
	SPI_Cmd(SPIx, ENABLE);
}
