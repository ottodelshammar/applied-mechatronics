/*
 * TestLOOPBACK2.c
 *
 * Created: 2020-11-28 13:18:25
 *  Author: tmk20otd
 */ 


#define F_CPU 1000000
#define FOSC 1843200
#define BAUD 2400
#define BRC ((F_CPU/16/BAUD)-1)
#define horst 128

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int initUSART(unsigned int ubrr);
void transmitUSART(unsigned char data);
void appendSerial(char c);
void serialWrite(char c[]);

unsigned char data;

int main(void)
{
	
	initUSART(BRC);
	sei();
	
	while(1)
	{
		
	}
}

int initUSART(unsigned int ubrr){
	UBRR0H = (unsigned int) (ubrr>>8); //Set baudrate
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0); //Enable reciever AND transmitter // RXCIE enables interrupts on the RXC flag
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00) | (1<<USBS0); // Set frame format: (UCSZ01)8data // (USBS0 = 1 == 2 stop bits)
	return 1;
}

void transmitUSART(unsigned char data){

	UDR0 = data; //Put data into buffer, sends the data
}

unsigned char recieveUSART(){
	data = UDR0;
	return data;
}

ISR(USART_RX_vect){
	data = recieveUSART();
	transmitUSART(data);
}
