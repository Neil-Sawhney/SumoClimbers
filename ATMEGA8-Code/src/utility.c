#include <avr/io.h>
#include <utility.h>
#include <util/delay.h>




unsigned int get_ADC( unsigned char channel){

    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    ADMUX = (1<<REFS0);
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1<<ADSC);

    while(ADCSRA & (1<<ADSC));

    return ADC;

}


unsigned char digitalReadPORTC(unsigned char pin){

    return (PINC & (1<<pin)) ? 1 : 0;

}


void digitalWritePORTD(unsigned char pin, unsigned char value){

    if(value){

        PORTD |= (1<<pin);

    }else{

        PORTD &= ~(1<<pin);

    }

}


int abs(int num){

    return (num < 0) ? -num : num;

}


void delay_ms(int ms){

    for(int i = 0; i < ms; i++){

        _delay_ms(1);

    }

}

int micros(void){

    int time = 0;
    time = TCNT1;
    time = time * 4;
    return time;

}
