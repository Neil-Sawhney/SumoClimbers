#pragma once
#define ON 1
#define OFF 0


// returns the value of the given ADC channel
unsigned int get_ADC( unsigned char);


// returns the value of the given digital pin on PORTC
unsigned char digitalRead(unsigned char pin, volatile uint8_t *port);


// takes a pin such as PD7, and a value such as 1 or 0
void digitalWrite(unsigned char pin, volatile uint8_t *port, unsigned char value);


// returns absolute value of num
int abs(int num);


// delays for the given number of milliseconds
void delay_ms(int ms);


volatile unsigned long timer1_millis;
//NOTE: A unsigned long holds values from 0 to 4,294,967,295 (2^32 - 1). It will roll over to 0 after reaching its maximum value.

void init_millis(unsigned long f_cpu);
unsigned long millis (void);