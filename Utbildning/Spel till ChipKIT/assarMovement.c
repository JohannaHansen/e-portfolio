#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations of global functions and variables */

void display_assar_jump(int height){
  int x, y;
  display_reset();                // resets all the pixels so the character does not leave a shadow
	for (y = 17; y < 31; y++){			//17 -> 30
		for (x = 0; x < 6; x++){			//0->5 (6 kolumner)
      display_matrix[y-height][x+dist] = assar[y-17][x];    //moves assar upwards
		}
	}
}

void assar_jump(){
  while (height < 12){
    display_assar_jump(height);     // displays all of the part of the game
    display_roof();
    display_ground();
    display_hurdle();
    display_update();

    gameover();
    height+=0.3;        //moves assar upwards
    hurdlemove--;       // moves the hurdle towards the left end of the display

    if (hurdlemove <= 0){
      hurdlemove = 128;
    }
}
  height = 12;
  while  (height > 0){
    display_assar_jump(height);
    display_roof();
    display_ground();
    display_hurdle();
    display_update();

    gameover();
    height-=0.3;          //moves assar downwards
    hurdlemove--;

    if (hurdlemove <= 0){
      hurdlemove = 128;
    }
  }
  height=0;
}

void display_assar_walk(int dist){
  int x, y;
  display_reset();
	for (y = 17; y < 31; y++){			//17 -> 30
		for (x = 0; x < 6; x++){			//0->5 (6 kolumner)
      display_matrix[y][x+dist] = assar[y-17][x];         //moves assar in the direction that that dist changes

		}
	}
}

void assar_forward (){
    if (dist<=121){           //ensures that assar can not leave the right end of the display
      dist++;                 //moves assar forwards
    }
    display_assar_walk(dist);
    display_roof();
    display_ground();
    display_hurdle();
    display_update();
    gameover();
    hurdlemove--;
    if (hurdlemove <= 0){
      hurdlemove = 128;
    }
}

void assar_backward (){
    if (dist>=0){             //ensures that assar can not leave the left end of the display
      dist--;                 //moves assar backwards
    }
    display_assar_walk(dist);
    display_roof();
    display_ground();
    display_hurdle();
    display_update();
    gameover();
    hurdlemove--;
    if (hurdlemove <= 0){
      hurdlemove = 128;
    }
}

void gameover(){
	int i, j;
    for (i=5; i>=0; i--){
      for (j = 0; j <= width; j++) {
        if ((hurdlemove+j)==(dist+i) && (hurdle_height-1)>=height){     //checks if the hitbox of assar is at the same place as a hurdle
          display_gameover();
          display_update();
          delay(1500);
          restart();
        }
      }
    }
}
