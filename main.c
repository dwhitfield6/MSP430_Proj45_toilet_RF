#include <msp430.h>
#include "RF.h"
#include "RF.c"

#define ToiletPos BIT0
#define ToiletCA BIT1
#define LED0 BIT1
#define LED1 BIT2
#define TX BIT3

unsigned char TXToiletMyBathroom[12] = {0,0,0,0,0,0,0,0,0,0,0,1};
unsigned char TXToiletAlexBathroom[12] = {0,0,0,0,0,0,0,0,0,0,0,2};

int timeout =1000;


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	  P2DIR |= (LED1 + LED0 + TX);
	  P2OUT &= ~(LED1 + LED0 + TX);
	  P2OUT |= LED1;
	  P1DIR |= ToiletPos;
	  P1OUT |= ToiletPos;
	  _delay_cycles(5000);
	  CACTL1 = CARSEL + CAREF0 + CAON + CAIES;          // 0.25 Vcc = -comp, on
	  CACTL2 = P2CA4;

	  _delay_cycles(1000);// P1.1/CA1 = +comp

	  CACTL1 |= CAIE;

	  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts

	  P2OUT &= ~LED1;

	while(1)
	{
		_delay_cycles(100);

		if(timeout ==10)
		{
			CACTL1 &= ~(CAIE);

			P2OUT |= LED0;
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			_delay_cycles(10000);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			send_rf(TX, TXToiletAlexBathroom);
			_delay_cycles(20000000);
			P2OUT &= ~LED0;


		}


		 if(timeout == 990)
		 {
				CACTL1 |= (CAIE);
				_delay_cycles(10);
			 _BIS_SR(LPM4_bits + GIE); // Enter LPM3 w/interrupt //lpm4;
		 }

		timeout++;
		if(timeout>1000)
		{
			timeout = 1000;
		}


	}
}


#pragma vector=COMPARATORA_VECTOR
__interrupt void CA(void)
{
	_BIC_SR(LPM4_EXIT); // wake up from low power mode

	_delay_cycles(10);
	CACTL1 &= ~CAIFG;

	timeout =0;

}

