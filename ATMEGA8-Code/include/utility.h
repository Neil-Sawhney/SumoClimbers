#pragma once


// returns the value of the given ADC channel
unsigned int get_ADC( unsigned char);


// returns the value of the given digital pin
unsigned char digitalRead(unsigned char pin, unsigned char port);


// sets the value of the given digital pin
void digitalWrite(unsigned char pin, unsigned char port, unsigned char value);


// delays for millisec milliseconds
void delay_ms(int millisec);


// returns absolute value of num
int abs(int num);


// returns the value of the microsecond counter
int micros(void);
