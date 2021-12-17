/*
 * TestLEDS.c
 *
 * Created: 2020-11-06 13:45:57
 *  Author: tmk20otd
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
	DDRD |= 0b00100000;
	DDRB |= 0b11000000;

	
    while(1)
    {
		
			PORTD |= 0b00100000;
			PORTB |= 0b11000000;
			_delay_ms(200);
			PORTD = 0b00000000;
			PORTB = 0b00000000;
			_delay_ms(200);
        //TODO:: Please write your application code 
    }
	
	return 1;
}