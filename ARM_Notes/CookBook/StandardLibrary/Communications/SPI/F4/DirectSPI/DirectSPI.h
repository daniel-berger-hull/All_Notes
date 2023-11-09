/**
  ******************************************************************************
  * @file    SysTick/main.h 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    15-May-2012
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_spi.h"
#include "stm32f4_discovery.h"

#define USE_FULL_ASSERT 1

#define LONG_BLINK  150
#define FAST_BLINK   50

#define     SPI_DataSize_16b   ((uint16_t)0x0800)
#define     SPI_DataSize_8b   ((uint16_t)0x0000)

enum spiSpeed { SPI_SLOW, SPI_MEDIUM, SPI_FAST };

static const uint16_t speeds[] = {
		[SPI_SLOW] = SPI_BaudRatePrescaler_64,
		[SPI_MEDIUM] = SPI_BaudRatePrescaler_8,
		[SPI_FAST] = SPI_BaudRatePrescaler_2};


/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

void spiInit(SPI_TypeDef* SPIx);
int spiReadWrite(SPI_TypeDef* SPIx, uint8_t *rbuf , const uint8_t *tbuf, int cnt, enum spiSpeed speed);
int spiReadWrite16(SPI_TypeDef* SPIx, uint16_t *rbuf, const uint16_t *tbuf, int cnt, enum spiSpeed speed);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
