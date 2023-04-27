#define IR_THRESHOLD_POT PB7

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

void setup(void);

// Accpets an IR enum and returns 1 if the IR sensor is triggered
typedef enum {
    IR1 = 0,
    IR2 = 1,
    IR3 = 2,
    IR4 = 3,
} IR;
char IR_triggered(IR);

// Accepts a direction and a speed and moves the robot accordingly, speed is from 0 to 255
typedef enum {
    FORWARD = 0,
    BACKWARD = 1,
    LEFT = 2,
    RIGHT = 3,
    STOP = 4,
} DIRECTION;
void move(char direction, int speed);