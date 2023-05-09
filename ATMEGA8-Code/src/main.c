#include <avr/io.h>
#include <robotStuff.h>
#include <utility.h>


int main()
{
    // initialize the robot
    setup();

    set_speed(80, 80);
    move(FORWARD);
    while (1) {
        if(robot_leaving()){
            move(FORWARD);
        }
    }

    return 0; // the program executed successfully
}
