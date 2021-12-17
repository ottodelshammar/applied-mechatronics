/*
 * TestSPEEDMEASUREMENT.c
 *
 * Created: 2020-11-29 11:00:20
 *  Author: tmk20otd
 */ 


#define F_CPU 1000000
#define BAUD 2400
#define BRC ((F_CPU/16/BAUD)-1)
#define ticksArraySize 5
#define charArraySize 4
#define prescaler 8

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Function declaration
void init_LEDs();
void init_INTs();
void init_TIMER();
void init_USART(unsigned int ubrr);
void init_PWM();
unsigned int TIM16_ReadTCNT1( void );
void TIM16_WriteTCNT1( unsigned int i );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void swap(unsigned int *p,unsigned int *q);
void sort(unsigned int a[],unsigned int length);

unsigned char charArray[charArraySize];
unsigned int rpm = 0; //double
unsigned int ticksArray[ticksArraySize]; //double
unsigned int ticks = 0; //double
unsigned int n = 0;
unsigned int pos = 0; 
unsigned int avgTicks = 0; //double
unsigned int median = 0;
unsigned long constant;

int main(void)
{
	init_TIMER();
	init_LEDs();
	init_USART(BRC);
	init_PWM();
	init_INTs();
	sei();
	
	for(int i = 0; i < charArraySize; i++){
		charArray[i]=0;
	}
	
	while(1){

		sort(ticksArray, ticksArraySize);
		
		median = ticksArray[(ticksArraySize + 1)/2 -1];
	
		rpm = constant/median; //avgTicks; // (60*10^6)/(96*8)= 78125// mulitply median*2 for 1 pin.
		
		itoa((int) rpm, charArray, 10); //Cast integer to string
		for(int i = 0; i < charArraySize; i++){ // Print entire buffer
			USART_Transmit(charArray[i]);
		}
		
		USART_Transmit(32); //blank space
		
		_delay_ms(1000);

	}
	
}

ISR (PCINT1_vect){ // PCINT1_vect
	ticks = TIM16_ReadTCNT1();
	TIM16_WriteTCNT1(0);
	
	if (ticks < 390 || ticks > 78125 ) //Corresponds to 200 and 1 rpm// high/low pass filter
	{
		return;
	}
	
	avgTicks += ticks;
	
	n++;
	
	if(n == 3){
	ticksArray[pos] = avgTicks/n;
	avgTicks = 0;
	n = 0;
	pos++;
	}
	
	if(pos == ticksArraySize){ // Reset position count
		pos = 0;
	} //Reset timer
}

void init_LEDs()
{
	DDRD |= 1 << PD5;
	DDRB |= (1<<PB6) | (1<<PB7);
}

void init_INTs(){
	DDRC &= ~(1 << DDC3); //| 1 << DDC4); // Set as inputs
	PCMSK1 |= (1<<PCINT11);//|(1<<PCINT12); // Set interrupt pins
	PCICR |= (1<<PCIE1); //Enable interrupts
	OCR0A = 253; //Set permanent compare value 50PWM = 120 rpm
	constant = 78125*2;
	if(OCR0A > 200){
		DDRC &= ~(1 << DDC4);
		PCMSK1 |= (1<<PCINT12); // Initialize additional l interrupt pin
		constant = 78125;
	}
}

void init_TIMER(){
		TCCR1B = (1 << WGM12); // Waveform generation; CTC = Compare to coutner mode// Bettar than normal mode, because can run withous utlizing the eprocessor?
		TCCR1B |= (1<<CS11); // (CS11 = 1) prescaler 8. // Time to overflow // 0.524288 = T/2, f = 1,
		
// 		OCR1A = 976; // Calculated value for this case//OutputCompare register
// 		TIMSK1 |= (1 << OCIE1A); // Enables global interrupt for on the timer vector
//		TIMSK1 |= (1 << TOIE1); // Enable overflow interrupt
//		TIFR1 |= (1 << TOV1); // TOV1 flag sets when timer overflows
}

void init_PWM()
{
	DDRD |= 1<<DDD6; // Set as output
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1 << WGM00); // Set fast PWM
	TCCR0B |= 1<<CS00; //Set no prescaling
}

void init_USART(unsigned int ubrr){
	UBRR0H = (unsigned int) (ubrr>>8); //Set baudrate
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0); //Enable reciever AND transmitter // RXCIE enables interrupts on the RXC flag
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Set frame format: (UCSZ01)8data // (USBS0 = 1 == 2 stop bits)
}

unsigned int TIM16_ReadTCNT1( void )
{
	unsigned char sreg;
	unsigned int i;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Read TCNT1 into i */
	i = TCNT1;
	/* Restore global interrupt flag */
	SREG = sreg;
	return i;
}

void TIM16_WriteTCNT1( unsigned int i )
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Set TCNT1 to i */
	TCNT1 = i;
	/* Restore global interrupt flag */
	SREG = sreg;
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void swap(unsigned int *p, unsigned int *q) {
	int t;
	
	t=*p;
	*p=*q;
	*q=t;
}

void sort(unsigned int a[],unsigned int length) {
	int i,j;

	for(i = 0;i < length-1;i++) {
		for(j = 0;j < length-i-1;j++) {
			if(a[j] > a[j+1])
			swap(&a[j],&a[j+1]);
		}
	}
}
