#include <Arduino.h>

void setup()
{
	Serial.begin(9600);
	Serial1.begin(230400);
}

void loop()
{
	if (Serial1.available())
	{
		char data_rcvd = Serial1.read(); // read one byte from serial buffer and save to data_rcvd
		Serial.write(data_rcvd); // write data_rcvd to serial monitor
	}
}