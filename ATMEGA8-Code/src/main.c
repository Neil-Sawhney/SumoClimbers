#include <avr/io.h>
#include <util/delay.h>

#include <utility.h>
#include <robotStuff.h>

int main()
{
    setup();

    // unsigned int result;
    while(1)
    {

        int speed = 0xFF;
        move(FORWARD, speed);
        delay_ms(50);
        move(STOP, speed);
        delay_ms(50);
        move(BACKWARD, speed);
        delay_ms(50);
        move(STOP, speed);
        delay_ms(50);
        move(LEFT, speed);
        delay_ms(50);
        move(STOP, speed);
        delay_ms(50);
        move(RIGHT, speed);
        delay_ms(50);
        move(STOP, speed);
        delay_ms(50);

    }

    return 0; // the program executed successfully
}