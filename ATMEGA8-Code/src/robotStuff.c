#include <avr/io.h>
#include <avr/interrupt.h>
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
    set_speed(0, 0);


    init_millis(F_CPU);
    sei();

    // set the threshold using the potentiometer
    set_threshold();

}

unsigned char IR_triggered(IR ir_sensor)
{
    unsigned int result = get_ADC(ir_sensor);
    return (result > _threshold) ? 1 : 0;
}

// create an array of arrays of unsigned chars
unsigned char US_pins[3][2] = {
    {US_1_TRIGGER, US_1_ECHO},
    {US_2_TRIGGER, US_2_ECHO},
    {US_3_TRIGGER, US_3_ECHO},
};
unsigned int get_US_distance(US sensor)
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

    ccwTurnSpeed = (ccwTurnSpeed > 1023) ? 1023 : ccwTurnSpeed;
    ccwTurnSpeed = (ccwTurnSpeed < -1023) ? -1023 : ccwTurnSpeed;
    forwardSpeed = (forwardSpeed > 1023) ? 1023 : forwardSpeed;
    forwardSpeed = (forwardSpeed < -1023) ? -1023 : forwardSpeed;

    unsigned char direction = (forwardSpeed >= 0) ? FORWARD : BACKWARD;

    // Calculate normalized speed if the sum exceeds 1023
    if (abs(forwardSpeed) + abs(ccwTurnSpeed) > 1023)
    {
        scale = 1023.0f / (abs(forwardSpeed) + abs(ccwTurnSpeed));
        forwardSpeed *= scale;
        ccwTurnSpeed *= scale;
    }

    leftSpeed = forwardSpeed + ccwTurnSpeed / 2;
    rightSpeed = forwardSpeed - ccwTurnSpeed / 2;

    leftSpeed = (leftSpeed > 1023) ? 1023 : (leftSpeed < -1023) ? -1023
                                                             : leftSpeed;
    rightSpeed = (rightSpeed > 1023) ? 1023 : (rightSpeed < -1023) ? -1023
                                                                : rightSpeed;

    set_speed(leftSpeed, rightSpeed);
    move(direction);
}

void led(unsigned char value)
{
    digitalWrite(TEST_LED, &PORTB, value);
}

unsigned char robot_leaving(void)
{
    if (IR_triggered(IR1) || IR_triggered(IR4))
    {
        led(ON);
        brake();
        // set_speed(1023, 1023);
        //TODO: uncomment these to go full speed
        move(LEFT);
        delay_ms(700);
        move(FORWARD);
        delay_ms(1000);
        brake();
        led(OFF);
        return 1;
    }
    if (IR_triggered(IR2) || IR_triggered(IR3))
    {
        led(ON);
        brake();
        // set_speed(1023, 1023);
        //TODO: uncomment these to go full speed
        move(RIGHT);
        delay_ms(700);
        move(FORWARD);
        delay_ms(1000);
        brake();
        led(OFF);
        return 1;
    }
    return 0;
    
    
}

unsigned char set_threshold(void)
{
    // get ADC from the potentiometer
    // scale the result so that fully ccw is MIN_THRESHOLD and fully cw is MAX_THRESHOLD
    _threshold = get_ADC(THRESHOLD) * (MAX_THRESHOLD - MIN_THRESHOLD) / 1023 + MIN_THRESHOLD;

    if (_threshold <= MIN_THRESHOLD + 2)
    {
        // blink the led 3 times
        for (int i = 0; i < 3; i++)
        {
            led(ON);
            delay_ms(200);
            led(OFF);
            delay_ms(200);
        }

        unsigned long start = millis();
        // wait 30 seconds
        while(millis() - start < 30000 ){
            _threshold = get_ADC(THRESHOLD) * (MAX_THRESHOLD - MIN_THRESHOLD) / 1023 + MIN_THRESHOLD;

            if (IR_triggered(IR1) || IR_triggered(IR2) || IR_triggered(IR3) || IR_triggered(IR4))
            {
                led(ON);
            }
            else
            {
                led(OFF);
            }
        }
        led(OFF);

        for (int i = 0; i < 3; i++)
        {
            led(ON);
            delay_ms(200);
            led(OFF);
            delay_ms(200);
        }


    }

    return _threshold;

}