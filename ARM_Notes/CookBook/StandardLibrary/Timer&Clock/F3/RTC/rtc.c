/**************************************************************************************************
 *					  			REAL TIME CLOCK LIBRARY(RTC)
 *	Few demo method that show how to use the RTC peripheral present on the STM32F303VG
 *
 **********************************************************************************************/



#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rtc.h"

#include "rtc.h"

uint32_t rtcInterruptCount = 0;
__IO uint8_t showtime[50] = {0};


/**
 *   Only one LED is used by the demo library: The Left Green LED (PE15),
 *   to show that the alarm interrupt is executed correctly in the code
 */
void initRTC_GPIO()
{
	GPIO_InitTypeDef      GPIO_InitStructure;

	//Warning!!! If prevbious GPIO configuration, and clock activation on the GPIO
	//           is called as second type, the previous confirmation will be lost!!!
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	 GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void initRTC(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;

	 /* Enable PWR APB1 Clock */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	  /* Allow access to Backup */
	  PWR_BackupAccessCmd(ENABLE);

	  /* Reset RTC Domain */
	  RCC_BackupResetCmd(ENABLE);
	  RCC_BackupResetCmd(DISABLE);

	  /* Allow access to RTC */
	  PWR_BackupAccessCmd(ENABLE);

	  /* The RTC Clock may varies due to LSI frequency dispersion */
	  /* Enable the LSI OSC */
	  RCC_LSICmd(ENABLE);

	  /* Wait till LSI is ready */
	  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	  {
	  }

	  /* Select the RTC Clock Source */
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	  /* Enable the RTC Clock */
	  RCC_RTCCLKCmd(ENABLE);

	  /* Wait for RTC APB registers synchronisation */
	  RTC_WaitForSynchro();

	  /* RTC prescaler configuration */
	  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	  RTC_InitStructure.RTC_AsynchPrediv = 88;
	  RTC_InitStructure.RTC_SynchPrediv = 470;
	  RTC_Init(&RTC_InitStructure);

	  /* Set the alarm 01h:00min:04s */
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x10;
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x33;
	  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x20;
	  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x24;
	  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	  /* Alarm mask hour, min and second:default Alarm generation each 1s */
	  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
	  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

	  /* Enable RTC Alarm A Interrupt */
	  RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	  /* Enable the alarm */
	  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	  /* Set the date: Wednesday August 15th 2012 */
	  RTC_DateStructure.RTC_Year = 17;
	  RTC_DateStructure.RTC_Month = RTC_Month_March;
	  RTC_DateStructure.RTC_Date = 24;
	  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Friday;
	  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	  /* Set the time to 01h 00mn 00s AM */
	  RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
	  RTC_TimeStructure.RTC_Hours   = 0x10;
	  RTC_TimeStructure.RTC_Minutes = 0x32;
	  RTC_TimeStructure.RTC_Seconds = 0x00;
	  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

	  RTC_ClearFlag(RTC_FLAG_ALRAF);

	  /* RTC Alarm A Interrupt Configuration */
	  /* EXTI configuration */
	  EXTI_ClearITPendingBit(EXTI_Line17);
	  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  /* Enable the RTC Alarm Interrupt */
	  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}

void RTC_Alarm_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_SET);   //Can set any pin from 8 to  15

    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
    rtcInterruptCount++;
  }
}

//void getRTCTime(char *ouput)

char* getRTCTime(void)
{
	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;

	/* Get the RTC current Time  &  Get the RTC current Date */
    RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
    /* Display time Format : hh:mm:ss */
    sprintf((char*)showtime,"%d/%d/%d  %d-%d-%d  Count: %d\n\r",RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, 2000 + RTC_DateStructure.RTC_Year,
   		 	 	 	 	 	 	 	 	 	 	  	         RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds,
														 rtcInterruptCount);
    return showtime;
}

