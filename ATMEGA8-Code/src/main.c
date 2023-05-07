#include <avr/io.h>
#include <robotStuff.h>
#include <utility.h>


int main()
{
    // initialize the robot
    setup();
    set_speed(80, 80);

    move(FORWARD);
    delay_ms(1000);
    brake();

    while (1)
    {
    }
    // set_speed(250, 250);
    // while(1){
    //     move(FORWARD); 
    //     delay_ms(1000);
    //     brake();
    //     // if (IR_triggered(IR_1))
    //     // {

    //     //     led(1);
    //     //     brake();
    //     //     move(BACKWARD);
    //     //     delay_ms(180);
    //     //     brake();
    //     // }
    //     // else
    //     // {
    //     //     led(0);
    //     //     // move(FORWARD);
    //     // }
    // }
    return 0; // the program executed successfully
}
