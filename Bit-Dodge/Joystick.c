/*
 * Joystick.c
 *
 * Created: 3/3/2020 3:32:23 PM
 * Author : grant
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart_ATmega1284.h"
#ifdef _SIMULATE_
#endif

void ADC_init(){

	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);

}


unsigned short ReadADC(unsigned char ch){

	ch = ch & 0x07;

	ADMUX = (ADMUX & 0xF8) | ch;

	

	ADCSRA |= (1<<ADSC);


	while(!(ADCSRA & (1<<ADIF)));

	

	//        ADCSRA |= (1<<ADIF);

	

	return(ADC);

}





unsigned char Byte = 0x00;



unsigned short x;



void js(){

	x = ReadADC(0);


	if(x > 950){

		Byte = 0x02;//left



	}
	
	
	

	else if(x < 300 ){

		Byte = 0x01; //right



	}

	else{

		Byte = 0x00;


	}

}


int main(void)
{
	
	DDRB = 0xFF; PORTB = 0x00; // Set B to output and initialize to 0x00
	DDRD = 0xFA; PORTD = 0x05;
	ADC_init();
	initUSART(0);
	while(1){
		js();
		PORTB = Byte;
		if(USART_IsSendReady(0)){
			USART_Send(Byte,0);}

		while(!USART_HasTransmitted(0));

	}
}