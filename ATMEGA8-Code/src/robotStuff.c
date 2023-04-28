#include <avr/io.h>
#include <util/delay.h>

#include <robotStuff.h>
#include <utility.h>

void setup(void){
    DDRD |= 0xFF; // set all pins of PORTD as output
    DDRB |= 0xFF; // set all pins of PORTB as output
    PORTB |= (1<<PB1);
    PORTB |= (1<<PB2);

    // start pwm on enable pins
    TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10);
    TCCR1B |= (1<<CS11)|(1<<WGM12);

    // set initial motor speed to 0
    int _left_motor_speed = 0;
    int _right_motor_speed = 0;
    LEFT_MOTOR_ENABLE = _left_motor_speed;
    RIGHT_MOTOR_ENABLE = _right_motor_speed;

}

char IR_triggered(IR ir_sensor){
    unsigned int result = get_ADC(ir_sensor);
    unsigned int IR_THRESHOLD = get_ADC(IR_THRESHOLD_POT);
    return (result > IR_THRESHOLD) ? 1 : 0;
}

void setSpeed(unsigned int left_speed, unsigned int right_speed){
    _left_motor_speed = left_speed;
    _right_motor_speed = right_speed;
}

void simpleMove(char direction){
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

    // pwm for enable pins
    LEFT_MOTOR_ENABLE = _left_motor_speed;
    RIGHT_MOTOR_ENABLE = _right_motor_speed;

}

void arcMove(int ccwTurnSpeed, int forwardSpeed) {
    int leftSpeed, rightSpeed;
    float scale;

    ccwTurnSpeed = (ccwTurnSpeed > 255) ? 255 : ccwTurnSpeed;
    ccwTurnSpeed = (ccwTurnSpeed < -255) ? -255 : ccwTurnSpeed;
    forwardSpeed = (forwardSpeed > 255) ? 255 : forwardSpeed;
    forwardSpeed = (forwardSpeed < -255) ? -255 : forwardSpeed;

    char direction = (forwardSpeed >= 0) ? FORWARD : BACKWARD;

    // Calculate normalized speed if the sum exceeds 255
    if (abs(forwardSpeed) + abs(ccwTurnSpeed) > 255) {
        scale = 255.0f / (abs(forwardSpeed) + abs(ccwTurnSpeed));
        forwardSpeed *= scale;
        ccwTurnSpeed *= scale;
    }

    leftSpeed = forwardSpeed + ccwTurnSpeed / 2;
    rightSpeed = forwardSpeed - ccwTurnSpeed / 2;

    leftSpeed = (leftSpeed > 255) ? 255 : (leftSpeed < -255) ? -255 : leftSpeed;
    rightSpeed = (rightSpeed > 255) ? 255 : (rightSpeed < -255) ? -255 : rightSpeed;

    setSpeed(leftSpeed, rightSpeed);
    simpleMove(direction);
}