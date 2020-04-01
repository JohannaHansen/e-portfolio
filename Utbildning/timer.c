#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations of global functions and variables */


void delay(int time) {

  int i = 0;
  T2CON = 0x030;           //Sets Timer 2:s prescaler to 1:8, i.e. now counts once every 8 ticks
  TMR2 = 0x0;              // Clear the timer register
  PR2 = 10000;             //Sets timer 2 period to 10 000
  IPCSET(2) = 0x01F;       //Sets highest priority and subpriority for timer 2
  IFSCLR(0) = 0x100;       //Clear timer 2:s interrupt flag
  IECSET(0) = 0x0100;      // enable timer 2 is enabled
  T2CONSET = 0x8000;       //Starts timer

/*  Timer should delay next function by "time" ms, so we set prescaler to 1:8
    i.e. 80 000 000 / 8 = 1 000 000
    and period to 10 000 (1 000 000 / 10 000 = 1 000) -- i.e. interrupt flag gets set to 1 a thousand times a second */

  while (i < time) {      //loops as long as we havent had "time" interrupts, i.e. "time" ms
    if (IFS(0) & 0x0100){
      IFSCLR(0) = 0x0100;
      i++;
    }
  }
}
