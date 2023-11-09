/********************************************************************************
 * @file    DirectSPI.c
 * @date    02-June-2017
 ******************************************************************************
 *  This program is a direct use of the SPI peripheral: SPI2 used in this demo
 *  The SPI pins used are: output is on the PB15, while the SPI receive is on
 *
 *	  Chip Select  --> PB12  (Active transfer when CS = O Volt)
 *    Clock        --> PB13
 *    MISO         --> PB14
 *    MOSI         --> PB15
 *
 *  Note: You need to loop back PB14 and PB15 so that data is sent to the MISO
 *         pin (SPI Input pin )
 *******************************************************************************/

#include <DirectSPI.h>



__IO uint32_t TimingDelay = 0;
uint8_t txbuf[4], rxbuf[4];
uint16_t txbuf16[4], rxbuf16[4];


void initGPIO()
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);


	// LED Pins Configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Start with LEDs turned off (so put high value on the pin)
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
}

void blinkLEDSequence(int repetion, int length)
{
	for (int i=0;i<repetion;i++)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
		Delay(length);

		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
		Delay(length);
	}
}


void spiInit(SPI_TypeDef *SPIx){

	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	SPI_StructInit(&SPI_InitStructure);


	// Enable clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);


	// Configure Pins
	// Pin PB13 (SCLK) must be configured as as 50MHz push pull
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	/* Pin PB14 (MISO) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Pin PB15 (MOSI) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	// Configure device
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = speeds[SPI_FAST];


	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx, &SPI_InitStructure);

	SPI_Cmd(SPIx, ENABLE);
}

int spiReadWrite(SPI_TypeDef* SPIx, uint8_t *rbuf , const uint8_t *tbuf, int cnt, enum spiSpeed speed){
	// SPI 8-bit read/write transaction
	int i;
	SPIx->CR1 = (SPIx->CR1 & ~ SPI_BaudRatePrescaler_256 ) |
			speeds[speed];

	for (i = 0; i < cnt; i++){
		if(tbuf) {
			SPI_I2S_SendData(SPIx , *tbuf++);
		} else {
			SPI_I2S_SendData(SPIx , 0xff);
		}
		while( SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE ) == RESET);

		if (rbuf) {
			*rbuf ++ = SPI_I2S_ReceiveData(SPIx);
		} else {
			SPI_I2S_ReceiveData(SPIx);
		}
	}
	return i;
}

int spiReadWrite16(SPI_TypeDef* SPIx, uint16_t *rbuf , const uint16_t *tbuf, int cnt, enum spiSpeed speed){
	// SPI 16-bit read/write transaction
	SPI_DataSizeConfig(SPIx, SPI_DataSize_16b);
	int i;
	SPIx ->CR1 = (SPIx ->CR1 & ~ SPI_BaudRatePrescaler_256 ) |
			speeds [ speed ];
	for (i = 0; i < cnt; i++){
		if (tbuf) {
			SPI_I2S_SendData (SPIx , *tbuf ++);
		} else {
			SPI_I2S_SendData (SPIx , 0xffff);
		}

		while ( SPI_I2S_GetFlagStatus (SPIx , SPI_I2S_FLAG_RXNE ) == RESET);

		if (rbuf) {
			*rbuf ++ = SPI_I2S_ReceiveData (SPIx);
		} else {
			SPI_I2S_ReceiveData (SPIx);
		}
	}
	//SPI_DataSizeConfig(SPIx, SPIDataSize_8b);
	return i;
}

void testSPI()
{
	int i,j;
    for(j = 0; j < 255; j++)
        txbuf[j] = j;


	// 8 bit read/write (?)
   for(i = 0; i < 100; i++) {
       GPIO_WriteBit(GPIOB , GPIO_Pin_12 , 0);
//       spiReadWrite(SPI2 , rxbuf , txbuf , 256, SPI_SLOW );
       spiReadWrite(SPI2 , rxbuf , txbuf , 4, SPI_FAST );

       GPIO_WriteBit (GPIOB , GPIO_Pin_12 , 1);

   }

   //       for(j = 0; j < 4; j++)
   //           if(rxbuf[j] != txbuf [j])
   //               assert_failed (__FILE__ , __LINE__ );


//   for (i = 0; i < 8; i++) {
//       for (j = 0; j < 4; j++)
//           txbuf16 [j] = i*4 + j + (i << 8);
//       GPIO_WriteBit(GPIOB , GPIO_Pin_12 , 0);
//       spiReadWrite16 (SPI2 , rxbuf16 , txbuf16 , 4, SPI_SLOW );
//       GPIO_WriteBit (GPIOB , GPIO_Pin_12 , 1);
////       for (j = 0; j < 4; j++)
////           if ( rxbuf16 [j] != txbuf16 [j])
////               assert_failed (__FILE__ , __LINE__ );
//   }
}



/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Main program
 * @param  None
 * @retval None
 */
int main(void)
{

	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1)
		{}
	}

	initGPIO();
	spiInit(SPI2);

	blinkLEDSequence(4,LONG_BLINK);




	/* Infinite loop */
	while (1)
	{
		for (int rep=0;rep<100;rep++)
			testSPI();
		blinkLEDSequence(1,FAST_BLINK);
	}
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void Delay(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
