#include <avr/io.h>
#include <robotStuff.h>
#include <utility.h>


int main()
{
    // initialize the robot
    setup();

    digitalWrite(TEST_LED, PORTB, 1);

    // set_speed(255, 255);

    // while (1)
    // {
    //     if (IR_triggered(IR_1))
    //     {
               digitalWrite(TEST_LED, PORTB, 1);

    //         brake();
    //         move_distance(-10);
    //         turn(100);
    //     }
    //     else
    //     {
               digitalWrite(TEST_LED, PORTB, 1);
    //         move(FORWARD);
    //     }
    // }
    return 0; // the program executed successfully
}
