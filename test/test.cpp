// Included just to get Intellisense to shut up
#ifndef __AVR_ATmega4809__
#define __AVR_ATmega4809__
#endif

#ifndef F_CPU
#define F_CPU 20000000UL // 20MHz unsigned long
#endif

// #define __DELAY_BACKWARD_COMPATIBLE__ // solves problem with _delay_ms()

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/* =============================================================================
   =====   FUNCTIONS                                                       =====
   ===========================================================================*/


   /* =============================================================================
	  =====   MAIN                                                            =====
	  ===========================================================================*/
int main(void) {
	/***************************************************************************
	******   SETUP			 											   *****
	***************************************************************************/

	// Unlock protected registers
	CCP = CCP_IOREG_gc;       // Write the Configuration Change Protection signature

	// Disable clock prescaler - set prescaler to 1 (no division)
	CLKCTRL.MCLKCTRLB = 0;   // MCLKCTRLB register with prescaler bits = 0 means no division

	// We're using the 40-pin DIP version of the 4809, so need to configure
	// the non-existent pins, PB0..5, PC6, PC7.
	PORTB.PIN0CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN1CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN3CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN5CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN6CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN7CTRL = PORT_PULLUPEN_bm;

	for (uint8_t i = 0; i < 8; i++) {
		PORTA.DIRSET = (1 << i); // set port A to outputs
		PORTA.OUTSET = (1 << i); // set port A pins high
		PORTD.DIRCLR = (1 << i); // set port D to inputs
		_delay_ms(250);
	}

	_delay_ms(500);

	for (uint8_t i = 0; i < 8; i++) {
		PORTA.OUTCLR = (1 << i);
		_delay_ms(250);
	}

	/***************************************************************************
	*****   MAIN LOOP                                                      *****
	***************************************************************************/

	while (1) {

		for (uint8_t i = 0; i < 8; i++) {
			// The != operator returns true if the two expressions are
			// not equal. So in this expression:
			// - We get the value of PORTD.IN, which will be the total value
			// for all the pins - ie, it could be anywhere from 0 to 255.
			// We AND this with the bit we're interested in. If set, this will
			// return the value of that bit - ie, 1, 2, 4, 8 etc.
			// Using != 0 tests whether the result is not equal to 0.
			// If it's anything BUT 0, this returns true. If it's 0, it
			// returns false.
			bool bitset = (PORTD.IN & (1 << i)) != 0;
			if (bitset) {
				PORTA.OUTSET = (1 << i);
			} else {
				PORTA.OUTCLR = (1 << i);
			}
		}

	}

}
