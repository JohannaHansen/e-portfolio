#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations of global functions and variables */

int main (void){
  srand(TMR2);          //randomizes an integer based on the timer to seed the rand() function

  /* Setup of output and input signals written 2015 by Axel Isaksson */
	/* Set up peripheral bus clock */
  /* OSCCONbits.PBDIV = 1; */
  OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
  SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;



  /*******************************************************************************************************
  ********************************************************************************************************
  ******************************THE ABOVE IS CODE FROM LAB 3 *********************************************/

  hurdlemove = 128;
  hurdle_height = 7;
  count = 0;
  score = 0;
  width = 3;
  dist = 30;
  first = 0;
  difficulty = 3;
  diff_set = 0;
	display_init();
  display_reset();
  display_startpage();
  display_update();

	while(1){

    int sw4 = (PORTD >> 11) & 0x00000001;
    int btn4 = (PORTD >> 7) & 0x00000001;
    int btn3 = (PORTD >> 6) & 0x00000001;
    int btn2 = (PORTD >> 5) & 0x00000001;
    int btn1 = (PORTF >> 1) & 0x00000001;     //Sets the button and switch value to variables

    if (btn1 == 1){
      restart();        //resets the game if Button 1 is pushed
    }

    if (sw4 == 1){      //game starts when the swich is on, otherwise it is paused
      display_reset();
      display_assar();
      display_ground();
      display_roof();

      display_hurdle();
      gameover();
      hurdlemove--;
      if (hurdlemove == 0){
    		hurdlemove = 128;
    	}
      display_update();

      if (btn4 == 1){
		    assar_jump();
		    display_update();
      }

      if (btn3 == 1 && btn2 == 0){        //you can not walk backwards and forwards at the same time
        assar_backward();
        display_update();
      }

      if (btn2 == 1 && btn3 == 0){
        assar_forward();
        display_update();
      }

      if (score > 0){           //displays the score on the LEDs
        PORTE = score;
      }

      if ((score % 10) == 0 && diff_set == 0){        //after every 10 hurdles cleared, the difficulty increases
        diff_set = 1;                                 //ensures that the difficulty only increases once and not constantly
        difficulty++;
      }
    }
  }

  display_update();
  return 0;
}
