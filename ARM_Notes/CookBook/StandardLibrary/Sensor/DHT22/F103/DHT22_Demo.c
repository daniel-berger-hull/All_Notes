/******************************************************************************
  * @title        DHT22_Demo.h
  * @Description  This codes shows how to inteferace with the temperature/Humidity
  *               sensor named DHT22, but invoking the functions of the dht22.h/dht22.c
  *               library
  *               This drive assume that the pin B0 is connected to the sensor, but
  *               witch can be changed by changing the pin value (DHT22_GPIO_PIN)
  *               in this header file.
  *
  ********************************************************************************/

#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <delay.h>
#include <uart.h>
#include <dht22.h>


uint32_t response;
uint16_t humidity,temperature;


int main(void)
{
	UART_Init();
	UART_SendStr("\nSTM32F103RET6 is online.\n\r");

	while (1) {
		DHT22_Init();
		response = DHT22_GetReadings();
		if (response != DHT22_RCV_OK) {
			UART_SendStr("DHT22_GetReadings() error = \n\r");
			UART_SendInt(response); UART_SendChar('\n');
		} else {
			response = DHT22_DecodeReadings();
			UART_SendStr("Parity: Received = "); UART_SendInt(response & 0xff);
			UART_SendStr(" Actual = "); UART_SendInt(response >> 8);
			UART_SendStr("\n\r");
			if ((response & 0xff) != (response >> 8)) {
				UART_SendStr("Wrong data received.\n\r");
			} else {
				temperature = DHT22_GetTemperature();
				humidity = DHT22_GetHumidity();

				UART_SendStr("Humidity: ");
				UART_SendInt(humidity / 10); UART_SendChar('.');
				UART_SendInt(humidity % 10); UART_SendStr("%RH");
				UART_SendStr("\n\r");
				UART_SendStr("Temperature: ");
				if ((temperature & 0x8000) != 0) UART_SendChar('-');
				UART_SendInt((temperature & 0x7fff) / 10); UART_SendChar('.');
				UART_SendInt((temperature & 0x7fff) % 10); UART_SendStr("C");
				UART_SendStr("\n\r");
			}
		}

		Delay_ms(1000);
	}
}
