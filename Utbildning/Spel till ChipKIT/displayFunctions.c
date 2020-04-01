#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations of global functions and variables */

void *stdout;						// to enable compiling after the inclusion of srand()

void display_hurdle(){
	int x, y, j;
	if (hurdlemove % 128 == 0){
		width = rand() % 10 + difficulty; // randomizes the width of the the hurdle evrytime a new one enters the display, as the game progresses the difficulty increases which makes the hurdles longer
		if(first != 0){ 									// increases the score each time a hurdle leaves the display
			score++;
			diff_set = 0;										//resets diff_set to make sure that the difficulty can increase
		}
		first++;
	}
	for (y=0; y < 7; y++){				//the height of the hurdle is 7 pixels
		for (x=0; x < 1; x++){			//the width of the hurdle bitmap is 2 pixels
			for (j=0; j<width; j++){
			display_matrix[y+24][x+hurdlemove+j] = hurdle[y][x];  //displays a hurdle of the randomized width which moves from the left to the right end of the display
			}
		}
	}
}

void display_assar(){
	int x, y;
	for (y = 17; y < 31; y++){			//Draws Assar at such a level that he's standing on ground
		for (x = 0; x < 6; x++){
			// swiches between the two bitmaps of Assar to create the appearance of him walking
			if (count <5){
      display_matrix[y][x+dist] = assar[y-17][x];
      count++;
    }
    if (count==5){
      display_matrix[y][x+dist] = assar2[y-17][x];
			count=0;
    }
		}
	}
}

void display_roof(){
	int x, y;
	for (y = 0; y < 1; y++){
		for (x = 0; x < 128; x++){
				display_matrix[y][x] = ground[y][x];   //shows a line to mark the top of the display
			}
		}
}

void display_ground(){
	int x, y;
	for (y = 0; y < 32; y++){
		for (x = 0; x < 128; x++){
			if (y == 31){
				display_matrix[y][x] = ground[y-31][x]; 	//shows a line to mark the bottom of the display
			}
		}
	}
}

void display_startpage(){
	int x, y;
	for (y = 0; y < 32; y++){
		for (x = 0; x < 128; x++){
			display_matrix[y][x] = start_page[y][x]; //shows the startpage bitmap on the entire display
		}
	}
}

void display_gameover(){
	int x, y;
	for (y = 9; y < 23; y++){
		for (x = 32; x < 100; x++){
			display_matrix[y][x] = game_over[y-9][x-32]; //places the game over bitmap in the middle of the screen
		}
	}
}

void display_reset() {
	int x, y;
	for (y = 0; y < 32; y++){
		for (x = 0; x < 128; x++){
			display_matrix[y][x] = 0; //turns off all the pixels on the display
		}
	}
}

void display_update() {							//SLIGHTLY REWRITTEN FUNCTION FROM LAB 3
    int col, page, seg;							//CLARIFICATION HERE: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
    int k;													//PAGE 34
    for (page = 0; page < 4; page++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22); // Set page start and end address
        spi_send_recv(page); // Page to use.

        spi_send_recv(0x00); // Set lower nibble of column start address
        spi_send_recv(0x10); // Set higher part of column start address

        DISPLAY_CHANGE_TO_DATA_MODE;

        // Load and send data for each segment
        for (col = 0; col < 128; col++) {
						seg = (display_matrix[page * 8][col]);
            for(k = 1; k < 8; k++){
                seg |= (display_matrix[page * 8 + k][col]) << k;
            }
            spi_send_recv(seg); // Send value to the display
        }
    }
}

void restart (){
	//resets all variables to the starting conditions so that the game is reset
	hurdlemove = 128;
	hurdle_height = 7;
	count = 0;
	score = 0;
	width = 3;
	dist = 30;
	height = 0;
	first = 0;
	difficulty = 3;
	diff_set = 0;
	PORTE = 0x0;

	display_reset();
	display_startpage();
	display_update();
	delay(1000);
}

/* Functions from the lab code, written 2015 by F Lundevall */
#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

void quicksleep(int cyc) {
	int l;
	for(l = cyc; l > 0; l--);
}

//sends information to the display through SPI2
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
/* Power ON sequence:
1. Power ON VDD
2. After VDD become stable, set RES# pin LOW (logic low) for at least 3us (t1) (4) and then HIGH (logic
high).
3. After set RES# pin LOW (logic low), wait for at least 3us (t2). Then Power ON VCC.
(1)
4. After VCC become stable, send command AFh for display ON. SEG/COM will be ON after 100ms
(tAF) */

  DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);			//turns display off
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);			//Enables charge pump
	spi_send_recv(0x14);			//Sets D2 to on for page adressing mode

	spi_send_recv(0xD9);			//sets the pre-charge period
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);			//Sets segment remapping of display
	spi_send_recv(0xC8);			//Sets the display vertically

	spi_send_recv(0xDA);			//Sets the pin configuration of the display
	spi_send_recv(0x20);			//Sets the memory adressing mode

	spi_send_recv(0xAF);			//turns display on
}
