#pragma once


#define TEST_LED PB7

#define LEFT_MOTOR_ENABLE OCR1A
#define RIGHT_MOTOR_ENABLE OCR1B 

#define LEFT_MOTOR_I1 PD0
#define LEFT_MOTOR_I2 PD1
#define RIGHT_MOTOR_I1 PD2
#define RIGHT_MOTOR_I2 PD3

#define IR_1 PC0
#define IR_2 PC1
#define IR_3 PC2
#define IR_4 PC3

#define US_1_TRIGGER PD4
#define US_1_ECHO PD5
#define US_2_TRIGGER PD6
#define US_2_ECHO PD7
#define US_3_TRIGGER PB6
#define US_3_ECHO PB7

#define THRESHOLD PC5

// value from 0 to 1023
unsigned int _left_motor_speed;
unsigned int _right_motor_speed;
unsigned int _threshold;


// sets pins to input or output and other initializations
void setup(void);


// Accpets an IR enum and returns 1 if the IR sensor is triggered
typedef enum {
    IR1 = IR_1,
    IR2 = IR_2,
    IR3 = IR_3,
    IR4 = IR_4,
} IR;
unsigned char IR_triggered(IR);


// returns the distance in cm from the given ultrasonic sensor
// TODO: do an ADC to make sure its above the threshold instead of just doing a digital read because the threshold pot shares a pin with the echo pin
typedef enum {
    US1 = 0,
    US2 = 1,
    US3 = 2,
} US;
unsigned int get_US_distance(US sensor);


// sets the global variables left_motor_speed and right_motor_speed to the given values
void set_speed(unsigned int left_speed, unsigned int right_speed);


// moves both wheels at the same speed. direction is defined by the DIRECTION enum
typedef enum {
    FORWARD = 0,
    BACKWARD = 1,
    LEFT = 2,
    RIGHT = 3,
    STOP = 4,
} DIRECTION;
void move(DIRECTION dir);


// turns the robot by the given number of degrees. positive values turn the robot counterclockwise
void turn(int degrees);


// moves the robot forward or backward the given distance in cm. negative values move the robot backward
void move_distance(int distance);


// just calls move(STOP) with a delay to allow the robot to stop
void brake(void);


//moves the robot forward or backward at the given speed, with the given turn speed.
//ccwTurnSpeed is from -1023 to 1023, with negative values turning the robot clockwise
// forwardSpeed is from -1023 to 1023, with negative values moving the robot backward
void arc_move(int ccwTurnSpeed, int fowardSpeed);

// turns the led on or off
void led(unsigned char state);

// this function should be called very frequently to check if we're leaving the arena
// checks all the IR sensors, if any are triggered, stop, the turn back into the arena depending on which sensor was triggered.
// if IR1 is triggered turn left by 135 degrees
// if IR2 is triggered turn right by 135 degrees
// if IR3 is triggered turn left by 60 degrees
// if IR4 is triggered turn right by 60 degrees
// returns 1 if one of the IR sensors was triggered, 0 otherwise
unsigned char robot_leaving(void);

// turns the robot if hits wall or gets pushed
void escapeRight(void);
void escapeLeft(void); 

// sets and returns the threshold using the potentiometer
// if the potentiometer is turned all the way to the left, continuously set the threshold for 30 seconds
unsigned char set_threshold(void);


// blink the led
void blink(void);
