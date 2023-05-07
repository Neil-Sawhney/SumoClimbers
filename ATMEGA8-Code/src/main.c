#include <avr/io.h>
#include <robotStuff.h>
#include <utility.h>


int main()
{
    // initialize the robot
    setup();

    set_speed(255, 255);
    while(1){
        check_leaving();
        move(FORWARD);
     }
    return 0; // the program executed successfully
}
