/**************************************************************************************************
 *					  			REAL TIME CLOCK LIBRARY(RTC)
 *
 *	See rtc.c for more details on the libraries
 *
 **********************************************************************************************/


#ifndef RTC_LIB_H
#define RTC_LIB_H



void initRTC(void);
void initRTC_GPIO(void);

char* getRTCTime(void);

#endif
