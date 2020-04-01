/* DISPLAY FUNCTIONS */
char display_matrix[32][128];
void display_update();
void display_reset();
void display_startpage();
void display_ground();
void display_assar();
void display_roof();
void delay(int);
void restart();

/* SCORE-KEEPING */
int score;
int first;
int difficulty;
int diff_set;

/* ASSAR MOVEMENT */
double height;
int dist;         //the distance from the right edge of the display to the left edge of assar
int count;
void assar_jump();
void assar_forward();
void assar_backward();
void gameover();

/* HURDLES */
int hurdlemove;
int hurdle_height;
int width;
int size;
void display_hurdle();

/* BITMAPS */
char start_page[32][128];
char game_over[14][68];
char ground[2][128];
char assar[13][6];
char assar2[13][6];
char hurdle [7][1];

/* FUNCTIONS FROM THE LAB CODE */
void quicksleep(int cyc);
uint8_t spi_send_recv(uint8_t data);
void display_init();
