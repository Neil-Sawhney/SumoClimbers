#include <avr/io.h>
#include <robotStuff.h>
#include <utility.h>


int main()
{
    // initialize the robot
    setup();
    set_speed(1023, 1023);
    while (1) {
        check_leaving();
    }

    return 0; // the program executed successfully
}
