#include <avr/io.h>
#include <robotStuff.h>
#include <utility.h>
#include <util/delay.h>


int main()
{
    setup();
    set_speed(255, 255);
    while (1)
    {
        if (IR_triggered(IR_1))
        {
            digitalWritePORTD(PD7, 1);
            simple_move(STOP);
            delay_ms(500);
            simple_move(BACKWARD);
            delay_ms(500);
            simple_move(LEFT);
            delay_ms(500);
        }
        else
        {
            digitalWritePORTD(PD7, 0);
            simple_move(FORWARD);
        }
    }
    return 0; // the program executed successfully
}
