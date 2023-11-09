/**
  ******************************************************************************
  * @file    USB_Example/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main program body
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


#include "main.h"



/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay = 0;

__IO uint8_t DataReady = 0;
__IO uint8_t PrevXferComplete = 1;
uint8_t *Mouse_Buffer;

/* Private function prototypes -----------------------------------------------*/
static uint8_t *USBD_HID_GetPos (void);

int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Configure the USB */
  USB_Config();


  /* Infinite loop */
  while (1)
  {

    /* Wait for data ready */
    while(DataReady !=0x04)
    {}
    DataReady = 0x00;


    /* Get position */
    Mouse_Buffer = USBD_HID_GetPos();
    /* Update the cursor position */
    if((Mouse_Buffer[1] != 0) ||(Mouse_Buffer[2] != 0))
    {
      /* Reset the control token to inform upper layer that a transfer is ongoing */
      PrevXferComplete = 0;

      /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
      USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);

      /* Enable endpoint for transmission */
      SetEPTxValid(ENDP1);
    }
  }
}

/**
  * @brief  Configure the USB.
  * @param  None
  * @retval None
  */
void USB_Config(void)
{
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();

  USB_Init();

  while (bDeviceState != CONFIGURED)
  {}
}

/**
  * @brief  USBD_HID_GetPos
  * @param  None
  * @retval Pointer to report
  */
static uint8_t *USBD_HID_GetPos (void)
{
  static uint8_t HID_Buffer[4] = {0};

  HID_Buffer[1] = 0;
  HID_Buffer[2] = 0;

  HID_Buffer[1] -= CURSOR_STEP;

  return HID_Buffer;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
