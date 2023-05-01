#include <avr/io.h>
#include <util/delay.h>

#include <utility.h>
#include <robotStuff.h>

int main()
{
    setup();
    set_speed(255, 255);
    // unsigned int result;
    while(1)
    {
        simple_move(FORWARD);
        _delay_ms(500);
        simple_move(STOP);
        _delay_ms(500);
        simple_move(BACKWARD);
        _delay_ms(500);
        simple_move(STOP);
        _delay_ms(500);
        simple_move(LEFT);
        _delay_ms(500);
        simple_move(STOP);
        _delay_ms(500);
        simple_move(RIGHT);
        _delay_ms(500);
        simple_move(STOP);
        _delay_ms(500);

    }

    return 0; // the program executed successfully
}
