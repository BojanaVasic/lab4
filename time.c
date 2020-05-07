/*
 * time.c
 *
 *  Created on: May 7, 2020
 *      Author: Bojana Vasic
 */


#include <avr/io.h>
#include <avr/interrupt.h>

unsigned long sys_time = 0;

ISR(TIMER0_COMPA_vect) {
	sys_time++;
}

void initTime() {
	TCCR0A = 0x02;

#if F_CPU > 20000000
#error "Frekvencija takta mora biti manja od 20MHz"
#endif

	unsigned long n = F_CPU / 1000;
	unsigned char clksel = 1;
	while (n > 255) {
		clksel++;
		n /= 8;
	}
	TCCROB = clksel;
	OCROA = (unsigned char)(n & 0xff) - 1;
	TIMSKO = 0x02;
	sei();

}

unsigned long millis() {
	unsigned long tmp;
	cli();
	tmp = sys_time;
	sei();
	return tmp;

}

void delay(unsigned long d) {
	unsigned long t0 = millis();
	while (millis() - t0 < d) ;
}
