#include <avr/io.h>
#include <util/delay.h>

#include <robotStuff.h>
#include <utility.h>

void setup(void){
    DDRD |= 0xFF; // set all pins of PORTD as output
    DDRB |= 0xFF; // set all pins of PORTB as output
    // TCCR2 = 0b00100000; // force output compare
}

char IR_triggered(IR ir_sensor){
    unsigned int result = get_ADC(ir_sensor);
    return (result > IR_THRESHOLD) ? 1 : 0;
}

void move(char direction, int speed){

    PORTB |= (1<<PB1);
    PORTB |= (1<<PB2);
    // LEFT_MOTOR_ENABLE = speed;
    // RIGHT_MOTOR_ENABLE = speed; 

    switch(direction){
        case FORWARD:
            PORTD |= (1<<LEFT_MOTOR_I1);
            PORTD &= ~(1<<LEFT_MOTOR_I2);
            PORTD |= (1<<RIGHT_MOTOR_I1);
            PORTD &= ~(1<<RIGHT_MOTOR_I2);
            break;
        case BACKWARD:
            PORTD &= ~(1<<LEFT_MOTOR_I1);
            PORTD |= (1<<LEFT_MOTOR_I2);
            PORTD &= ~(1<<RIGHT_MOTOR_I1);
            PORTD |= (1<<RIGHT_MOTOR_I2);
            break;
        case LEFT:
            PORTD &= ~(1<<LEFT_MOTOR_I1);
            PORTD |= (1<<LEFT_MOTOR_I2);
            PORTD |= (1<<RIGHT_MOTOR_I1);
            PORTD &= ~(1<<RIGHT_MOTOR_I2);
            break;
        case RIGHT:
            PORTD |= (1<<LEFT_MOTOR_I1);
            PORTD &= ~(1<<LEFT_MOTOR_I2);
            PORTD &= ~(1<<RIGHT_MOTOR_I1);
            PORTD |= (1<<RIGHT_MOTOR_I2);
            break;
        case STOP:
            PORTD |= (1<<LEFT_MOTOR_I1);
            PORTD |= (1<<LEFT_MOTOR_I2);
            PORTD |= (1<<RIGHT_MOTOR_I1);
            PORTD |= (1<<RIGHT_MOTOR_I2);
            break;
    }

}