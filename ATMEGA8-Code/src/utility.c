#include <avr/io.h>
#include <util/delay.h>

#include <utility.h>


unsigned int get_ADC( unsigned char channel){

    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    ADMUX = (1<<REFS0);
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1<<ADSC);

    while(ADCSRA & (1<<ADSC));

    return ADC;

}


unsigned char digitalRead(unsigned char pin, unsigned char port){

    return (port & (1<<pin)) ? 1 : 0;

}


void digitalWrite(unsigned char pin, unsigned char port, unsigned char value){
    
        if(value == 1){
    
            port |= (1<<pin);
    
        }else{
    
            port &= ~(1<<pin);
    
        }
    
    }


void delay_ms(int millisec){

    for(int i = 0; i < millisec; i++){

        _delay_ms(1);

    }
}


int abs(int num){

    return (num < 0) ? -num : num;

}


int micros(void){

    int time = 0;
    time = TCNT1;
    time = time * 4;
    return time;

}
