/*
 * TestPWM.c
 *
 * Created: 2020-11-06 14:27:05
 *  Author: tmk20otd
 */ 


#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

 
int initPWM(void);
int updatePWM(int value);

int main(void)
{
	
	int duty = 100;
	initPWM();
	updatePWM(duty);
	
    while(1)
    {
// 		_delay_ms(250);
// 		duty+=10;
// 		if(duty > 255)
// 		{
// 			duty = 10;
// 		}
// 		
// 		updatePWM(duty);
		
        //TODO:: Please write your application code 
    }
}

int initPWM(void)
{
	DDRD |= 1<<DDD6;
	TCCR0A |= 0b10000011; // 0b10110011
	TCCR0B |= 0x01;
	return 1;
}

int updatePWM(int value)
{
OCR0A = value;
//OCR0B = value;	
return value;
}