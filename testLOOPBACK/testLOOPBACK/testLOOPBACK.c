/*
 * testLOOPBACK.c
 *
 * Created: 2020-11-26 12:14:47
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
char serialBuffer[horst];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;

int main(void)
{
	
	initUSART(BRC);
	sei();
	
    while(1)
    {	
		
		serialWrite("Med en enkel tullipan på bemärkelsedan \n\r");
		_delay_ms(1000);
		
		
// 		data = recieve_USART();
// 		transmit_USART(data);
	}
}

int initUSART(unsigned int ubrr){
	UBRR0H = (unsigned int) (ubrr>>8); //Set baudrate
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<TXCIE0); //Enable reciever AND transmitter 
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Set frame format: 8data
	return 1;
}

void transmitUSART(unsigned char data){
//	while( !(UCSR0A & (1<<UDRE0))) //Wait empty for transmit buffer// UDRE0 = flag indicated if UDRn is ready to buffer
//	;

	UDR0 = data; //Put data into buffer, sends the data
}


void appendSerial(char c){
	serialBuffer[serialWritePos] = c;
	serialWritePos++;
	
	if(serialWritePos >= horst){
		serialWritePos = 0;
	}
}

void serialWrite(char c[]){
	for(uint8_t i = 0; i < strlen(c); i++){
		appendSerial(c[i]);
	}
	
	if(UCSR0A & (1 << UDRE0)){
		UDR0 = 0;
	}
}

ISR(USART_TX_vect){
	if(serialReadPos != serialWritePos){
		UDR0 = serialBuffer[serialReadPos];
		serialReadPos++;
		if(serialReadPos >= horst){
			serialReadPos++;
		}

	}
}


// 
// unsigned char recieve_USART(void){
// 	while(!(UCSR0A & (1<<RXC0))) // wait for data to be recieved// RXC0 = flag set when unread data in recieve buffer, clear when recieve buffer is empty
// 	;
// 	return UDR0; // get and return recieved data
// }
// 
// void flush_USART(void){
// 	unsigned char dummy;
// 	while(UCSR0A & (1<<RXC0))
// 	dummy = UDR0;
// }

//U2Xn = 0 normal speed mode, = 1 double speed mode
//UDRn Usart I/O data register n. RXB read, WXB write
//UCSRnA control and statur register.
//
