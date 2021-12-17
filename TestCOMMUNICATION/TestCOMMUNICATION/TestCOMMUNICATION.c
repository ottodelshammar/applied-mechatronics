/*
 * TestCOMMUNICATION.c
 *
 * Created: 2020-11-29 10:57:57
 *  Author: tmk20otd
 */ 

#define F_CPU 1000000
#define FOSC 1843200
#define BAUD 2400
#define BRC ((F_CPU/16/BAUD)-1)
#define horst 128
#define messageSize 5

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int initUSART(unsigned int ubrr);
void initLEDs();
void transmitUSART(unsigned char data);
void readMessage(unsigned char message[]);
// void appendSerial(char c);
// void serialWrite(char c[]);

unsigned char data;
unsigned char message[messageSize];
int m[messageSize];
unsigned int messagePos = 0;
int val;

int main(void)
{
	
	initUSART(BRC);
	initLEDs();
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
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char recieveUSART(){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

ISR(USART_RX_vect){
	data = recieveUSART();
	message[messagePos] = data;
	messagePos++;
	if(messagePos == messageSize){
		readMessage(message);
		messagePos = 0;
	}
}

void initLEDs(){
	DDRD |= (1<<PD5);
	DDRB |= (1<<PB6)|(1<<PB7);
}

void readMessage(unsigned char message[]){
	if(message[messageSize-1] == '0'){
		PORTD = 0x0;
		PORTB = 0x0;
	}
	if (message[messageSize-1] == '1'){
		PORTD |= (1<<PD5);
	}
	
	char vect[3] = {message[1], message[2], message[3]};
	
 	val = atoi(vect);
	
	if (val == 123){
		PORTB |= (1<<PB6);
	}
	
	if (val == 0){
		PORTB = 0x0;
	}
	
	
}
