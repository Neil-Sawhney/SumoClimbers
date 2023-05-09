#include <avr/io.h>
#include <utility.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>




unsigned int get_ADC( unsigned char channel){

    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    ADMUX = (1<<REFS0);
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1<<ADSC);

    while(ADCSRA & (1<<ADSC));

    return ADC;

}


unsigned char digitalRead(unsigned char pin, volatile uint8_t *port){

    return (*port & (1<<pin)) ? 1 : 0;

}


void digitalWrite(unsigned char pin, volatile uint8_t *port, unsigned char value){

    if(value){

        *port |= (1<<pin);

    }else{

        *port &= ~(1<<pin);

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


ISR(TIMER2_COMP_vect)
{
  timer2_millis++;
}

void init_millis(unsigned long f_cpu)
{
  unsigned long ctc_match_overflow;

  ctc_match_overflow = ((f_cpu / 1000) / 64); //when timer1 is this value, 1ms has passed

  // (Set timer to clear when matching ctc_match_overflow) | (Set clock divisor to 64)
  TCCR2 |= (1 << WGM21);
  TCCR2 &= ~(1 << WGM20);

  // Set clock divisor to 64
  TCCR2 |= (1 << CS22);
  TCCR2 &= ~((1 << CS21) | (1 << CS20));

  // Set compare match value
  OCR2 = ctc_match_overflow;

  // Enable the compare match interrupt
  TIMSK |= (1 << OCIE2);

  //REMEMBER TO ENABLE GLOBAL INTERRUPTS AFTER THIS WITH sei(); !!!
}

unsigned long millis (void)
{
  unsigned long millis_return;

  // Ensure this cannot be disrupted
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    millis_return = timer2_millis;
  }
  return millis_return;
}