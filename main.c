/*
 * main.c
 *
 *  Created on: May 18, 2024
 *      Author: DELL
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;
ISR(TIMER1_COMPA_vect){
	seconds++;
	if(seconds == 60){
		seconds = 0;
		minutes++;
	}
	if(minutes == 60){
		hours++;
		minutes = 0;
	}
	if(hours == 24){
		hours =0;
	}



}

void Timer1_init(void){
	TCNT1 = 0;
	TCCR1A |= (1<<FOC1A) | (1<<FOC1B);
	OCR1A = 977;
	TCCR1B |= (1<<CS12)|(1<<CS10)|(1<<WGM12);
	 TIMSK |=(1<<OCIE1A);


}

ISR(INT0_vect){
	seconds = 0;
	minutes = 0;
	hours = 0;

}
ISR(INT1_vect){
	TCCR1B &= ~(1<<CS10) & ~(1<<CS12); //clear clock to pause countdown
}
ISR(INT2_vect){
	TCCR1B |= (1<<CS10) | (1<<CS12); //resume timer and continue countdown
}

void INTO_init(void){
	DDRD &= ~(1<<PD2);
	PORTD |=(1<<PD2);
	MCUCR |=(1<<ISC00);
	MCUCR &= ~(1<<ISC01);
	GICR |= (1<<INT0);

}
void INT2_init(void){
	DDRB &= ~(1<<PB2);
	PORTB |=(1<<PB2);
	MCUCSR &= ~(1<<ISC2);
	GICR |= (1<<INT2);

}
void INT1_init(void){
	DDRD &= ~(1<<PD3);
	MCUCR |=(1<<ISC10) |(1<<ISC11);
	GICR |= (1<<INT1);

}



int main(void)
{
SREG = SREG |(1<<7);
DDRC |= 0x0F;
PORTC &= 0xF0;

DDRA = 0x3F;
PORTA &= 0x3F;
INTO_init();
INT2_init();
INT1_init();
Timer1_init();
while(1){
	PORTA = (PORTA & 0xC0) | (1<<PIN0);
	PORTC = (PORTC & 0xF0) | (seconds%10);
	_delay_us(200);
	PORTA = (PORTA & 0xC0) | (1<<PIN1);
	PORTC = (PORTC & 0xF0) | (seconds/10);
	_delay_us(200);

	PORTA = (PORTA & 0xC0) | (1<<PIN2);
	PORTC = (PORTC & 0xF0) | (minutes%10);
	_delay_us(200);
	PORTA = (PORTA & 0xC0) | (1<<PIN3);
	PORTC = (PORTC & 0xF0) | (minutes/10);
	_delay_us(200);
	PORTA = (PORTA & 0xC0) | (1<<PIN4);
	PORTC = (PORTC & 0xF0) | (hours%10);
	_delay_us(200);
	PORTA = (PORTA & 0xC0) | (1<<PIN5);
	PORTC = (PORTC & 0xF0) | (hours/10);
	_delay_us(200);
}




}


