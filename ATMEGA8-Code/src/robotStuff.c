#include <avr/io.h>
#include <utility.h>
#include <robotStuff.h>
#include <util/delay.h>

void setup(void)
{
    DDRD |= 0xFF; // set all pins of PORTD as output
    DDRB |= 0xFF; // set all pins of PORTB as output

    // start pwm on enable pins
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
    TCCR1A |= (1 << WGM11) | (1 << WGM10);
    TCCR1B |= (1 << CS11) | (1 << WGM12);

    // set initial motor speed to 0
    _left_motor_speed = 0;
    _right_motor_speed = 0;
    LEFT_MOTOR_ENABLE = _left_motor_speed;
    RIGHT_MOTOR_ENABLE = _right_motor_speed;
}

unsigned char IR_triggered(IR ir_sensor)
{
    unsigned int result = get_ADC(ir_sensor);
    return (result > IR_THRESHOLD) ? 1 : 0;
}

// create an array of arrays of unsigned chars
unsigned char US_pins[3][2] = {
    {US_1_TRIGGER, US_1_ECHO},
    {US_2_TRIGGER, US_2_ECHO},
    {US_3_TRIGGER, US_3_ECHO},
};
unsigned int get_distance(US sensor)
{
    unsigned char trigger = US_pins[sensor][0];
    unsigned char echo = US_pins[sensor][1];
    unsigned int distance = 0;
    unsigned int time = 0;

    // send a 10us pulse to the trigger pin
    digitalWrite(trigger, &PORTD, 0xFF);
    _delay_us(10);
    digitalWrite(trigger, &PORTD, 0x00);

    // wait for the echo pin to go high
    while (!digitalRead(echo, &PORTD))
        ;

    // get the time
    int currentTime_us = micros();
    while (digitalRead(echo, &PORTD))
        ;
    time = micros() - currentTime_us;

    // convert time to distance
    distance = time / 58; // cm
    return distance;
}

void set_speed(unsigned int left_speed, unsigned int right_speed)
{
    _left_motor_speed = left_speed;
    _right_motor_speed = right_speed;
}

void move(DIRECTION dir)
{
    // pwm for enable pins
    LEFT_MOTOR_ENABLE = _left_motor_speed;
    RIGHT_MOTOR_ENABLE = _right_motor_speed;

    switch (dir)
    {
    case BACKWARD:
        digitalWrite(LEFT_MOTOR_I1, &PORTD, 0xFF);
        digitalWrite(LEFT_MOTOR_I2, &PORTD, 0x00);
        digitalWrite(RIGHT_MOTOR_I1, &PORTD, 0xFF);
        digitalWrite(RIGHT_MOTOR_I2, &PORTD, 0x00);
        break;
    case FORWARD:
        digitalWrite(LEFT_MOTOR_I1, &PORTD, 0x00);
        digitalWrite(LEFT_MOTOR_I2, &PORTD, 0xFF);
        digitalWrite(RIGHT_MOTOR_I1, &PORTD, 0x00);
        digitalWrite(RIGHT_MOTOR_I2, &PORTD, 0xFF);
        break;
    case RIGHT:
        digitalWrite(LEFT_MOTOR_I1, &PORTD, 0xFF);
        digitalWrite(LEFT_MOTOR_I2, &PORTD, 0x00);
        digitalWrite(RIGHT_MOTOR_I1, &PORTD, 0x00);
        digitalWrite(RIGHT_MOTOR_I2, &PORTD, 0xFF);
        break;
    case LEFT:
        digitalWrite(LEFT_MOTOR_I1, &PORTD, 0x00);
        digitalWrite(LEFT_MOTOR_I2, &PORTD, 0xFF);
        digitalWrite(RIGHT_MOTOR_I1, &PORTD, 0xFF);
        digitalWrite(RIGHT_MOTOR_I2, &PORTD, 0x00);
        break;
    case STOP:
        digitalWrite(LEFT_MOTOR_I1, &PORTD, 0x00);
        digitalWrite(LEFT_MOTOR_I2, &PORTD, 0x00);
        digitalWrite(RIGHT_MOTOR_I1, &PORTD, 0x00);
        digitalWrite(RIGHT_MOTOR_I2, &PORTD, 0x00);
        break;
    default:
        break;
    }
}

void turn(int degrees)
{

    int secondsToTurn180 = 2;
    int secondsToTurn = secondsToTurn180 / 180 * abs(degrees);

    if (degrees > 0)
    {
        move(LEFT);
        delay_ms(secondsToTurn * 1000);
        move(STOP);
    }
    else
    {
        move(RIGHT);
        delay_ms(secondsToTurn * 1000);
        move(STOP);
    }
}

void move_distance(int distance)
{
    int secondsToMove100cm = 2;
    int secondsToMove = secondsToMove100cm / 100 * abs(distance);

    if (distance > 0)
    {
        move(FORWARD);
        delay_ms(secondsToMove * 1000);
        brake();
    }
    else
    {
        move(BACKWARD);
        delay_ms(secondsToMove * 1000);
        brake();
    }
}

void brake(void)
{
    move(STOP);
    delay_ms(100);
}

void arc_move(int ccwTurnSpeed, int forwardSpeed)
{
    int leftSpeed, rightSpeed;
    float scale;

    ccwTurnSpeed = (ccwTurnSpeed > 255) ? 255 : ccwTurnSpeed;
    ccwTurnSpeed = (ccwTurnSpeed < -255) ? -255 : ccwTurnSpeed;
    forwardSpeed = (forwardSpeed > 255) ? 255 : forwardSpeed;
    forwardSpeed = (forwardSpeed < -255) ? -255 : forwardSpeed;

    unsigned char direction = (forwardSpeed >= 0) ? FORWARD : BACKWARD;

    // Calculate normalized speed if the sum exceeds 255
    if (abs(forwardSpeed) + abs(ccwTurnSpeed) > 255)
    {
        scale = 255.0f / (abs(forwardSpeed) + abs(ccwTurnSpeed));
        forwardSpeed *= scale;
        ccwTurnSpeed *= scale;
    }

    leftSpeed = forwardSpeed + ccwTurnSpeed / 2;
    rightSpeed = forwardSpeed - ccwTurnSpeed / 2;

    leftSpeed = (leftSpeed > 255) ? 255 : (leftSpeed < -255) ? -255
                                                             : leftSpeed;
    rightSpeed = (rightSpeed > 255) ? 255 : (rightSpeed < -255) ? -255
                                                                : rightSpeed;

    set_speed(leftSpeed, rightSpeed);
    move(direction);
}

void led(unsigned char value)
{
    digitalWrite(TEST_LED, &PORTB, value);
}

unsigned char check_leaving(void)
{
    if (IR_triggered(IR1))
    {
        led(1);
        brake();
        move(BACKWARD);
        delay_ms(700);
        brake();
        move(LEFT);
        delay_ms(700);
        brake();
        return 1;
    }
    if (IR_triggered(IR2))
    {
        led(1);
        brake();
        move(BACKWARD);
        delay_ms(700);
        brake();
        move(RIGHT);
        delay_ms(700);
        brake();
        return 1;
    }
    if (IR_triggered(IR3))
    {
        led(1);
        set_speed(255, 255);
        move(RIGHT);
        delay_ms(700);
        return 1;
    }
    if (IR_triggered(IR4))
    {
        led(1);
        set_speed(255, 255);
        move(LEFT);
        delay_ms(700);
        return 1;
    }
    led(0);
    return 0;
    
    
}