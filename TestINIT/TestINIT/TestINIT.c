/*
 * TestINT.c
 *
 * Created: 2020-11-13 15:23:12
 *  Author: tmk20otd
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <avr/interrupt.h>

//Function declaration
int init_LEDs(void);
int init_INTs(void);


ISR (PCINT1_vect, ISR_BLOCK){
					PORTB |= (1<<PB6);
					PORTD |= 0b11000000;
					
}
//int set_LED(int pos, int val);

int main(void)
{
	init_LEDs();
	init_INTs();
	sei();
	
	while(1){
	}
	
}

int init_LEDs(void)
{
		DDRD |= 0b00100000;
		DDRB |= 0b11000000;
		
		return 1;
}

int init_INTs(void){
	DDRC &= ~(0b00011000);
	PCMSK1 |= (1<<PCINT11)|(1<<PCINT12);
	PCICR |= (1<<PCIE1);
	
	return 1;
}




//PCINT11 //12
