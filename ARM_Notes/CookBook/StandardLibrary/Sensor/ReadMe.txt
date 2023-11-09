DHT22:
 The DHT22 is a temperature/Humidity sensor using it own protocol to communicate its data to the MCD
 It is part of a larger familly of sensor, including the DHT11, which is a cheaper version of the DHT22, but with only the temperature.
 Some datasheet and demo program refer to the DHT22 as the AM2302 instead...


DHT22 Pinout:

	  _______               (When the grid part is facing us)
	  |  O  |               From Left Pin to Right 
	------------             1 - Vcc (3.3V to 5.0V)
	|  :  :  : |             2 - Data
	|  :  :  : |             3 - Not connected
	|          |             4 - Gnd
	------------
  	 |  |  |  |              
	 1  2  3  4
 
 
 Important:
 
    The DHT22 have to be used with a pullup resistor, like 10KOhms
    
    
    Vcc ---- 10K --*- Data Pin (#2)