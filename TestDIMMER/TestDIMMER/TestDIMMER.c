/*
 * TestDIMMER.c
 *
 * Created: 2020-11-18 11:11:08
 *  Author: tmk20otd
 */ 

#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Function declaration
int init_LEDs(void);
int init_INTs(void);
int init_PWM(void);
int update_PWM(int value);

uint8_t pin27;
uint8_t pin28;
uint8_t AB = 0x0;
uint8_t ABnew = 0x0;
int n = 0;


int main(void)
{
	init_INTs();
	init_PWM();
	init_LEDs();
	sei();

		
	
	while(1){
		
		update_PWM(n*10);
		
		if(n > 200){
			PORTD |= (1<<PD5);
		}
		
		if(n < 10 ){
			PORTD &=  ~(1<<PD5);
		}
	}
	
	
}

ISR (PCINT1_vect){ //ISR_BLOCK <<--- if needed?

	ABnew = ((PINC & (0b00011000)) >> 3) ;
	
	switch (ABnew)
	{
		case 0:
		if(AB == 2){
			PORTB &= ~(1<<PB7);
			PORTB |= (1<<PB6);
			n++;
		}else{ // AB==1
			PORTB |= (1<<PB7);
			PORTB &= ~(1<<PB6);
			n--;
		}
		break;
		
		case 1:
		if(AB == 0){
			PORTB &= ~(1<<PB7);
			PORTB |= (1<<PB6);
			n++;
		}else{ // AB==3
			PORTB |= (1<<PB7);
			PORTB &= ~(1<<PB6);
			n--;
		}
		break;
		
		case 2:
		if(AB == 3){
			PORTB &= ~(1<<PB7);
			PORTB |= (1<<PB6);
			n++;
		}else{ // AB==0
			PORTB |= (1<<PB7);
			PORTB &= ~(1<<PB6);
			n--;
		}
		break;
		
		case 3:
		if(AB == 1){
			PORTB &= ~(1<<PB7);
			PORTB |= (1<<PB6);
			n++;
		}else{ // AB==2
			PORTB |= (1<<PB7);
			PORTB &= ~(1<<PB6);
			n--;
		}
		break;
	}
	
	if (n < 0){
		n = 0;
	}
	
	if (n > 255){
		n = 255;
	}
	AB = ABnew;

}
//int set_LED(int pos, int val)

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

int init_PWM(void)
{
	DDRD |= 1<<DDD6; // This works
	TCCR0A |= 0b10000011;
	TCCR0B |= 0x01;
	return 1;
}

int update_PWM(int value)
{
	OCR0A = value;
	return value;
}




//PCINT11 //12
