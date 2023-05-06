#pragma once


// returns the value of the given ADC channel
unsigned int get_ADC( unsigned char);


// returns the value of the given digital pin on PORTC
unsigned char digitalRead(unsigned char pin, volatile uint8_t port);


// takes a pin such as PD7, and a value such as 1 or 0
void digitalWrite(unsigned char pin, volatile uint8_t port, unsigned char value);


// returns absolute value of num
int abs(int num);


// delays for the given number of milliseconds
void delay_ms(int ms);


// returns the value of the microsecond counter
int micros(void);
