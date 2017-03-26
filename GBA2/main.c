#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "images.h"
#include "font.h"


#define NUM_POKE 8

struct CHARACTER {
	int row;
	int col;
	int cd;
	int size;
	int isalive;
};
struct POKEBALL
{
	int row;
	int col;
	int isthere;
};


enum GBAState {
	START,
	PLAY,
	WIN,
	LOSE,
};
int main()
{
	enum GBAState state = START;
	int i;
	int numCaught = 0;
	int numball = 8;
	int row = 15;
	int speed = 1;
	int numEnemy = NUM_POKE;
	int fr = 0;
	int frStart = 0;
	int frWin = 0;
	int frLost = 6;
	int frBall = 0;
	int play = 0;
	int ashLeft = 1;
	int count;
	struct CHARACTER wobbuffet[NUM_POKE];
	struct CHARACTER ash1 = {150, 120, 4, ASH_WIDTH, 1};
	struct POKEBALL ball = {0, 0, 0};
	REG_DISPCTL = MODE4 | BG2_ENABLE;
	loadPalette(IMAGES_PALETTE_SIZE, images_palette);
	// Initialize all objects
	for(i = 0; i < NUM_POKE; i++)
	{
		wobbuffet[i].row = row;
		wobbuffet[i].col = 100 + rand() % 40;
		wobbuffet[i].cd = speed++;
		wobbuffet[i].size = WINGULL_WIDTH;
		wobbuffet[i].isalive = 1;
		row += 15;
	}
	

	// int size = 5;

	
	while(1)
	{
		switch(state) {
			case START:
				drawImage4(0, 0, WOBBUFFET_WIDTH, WOBBUFFET_HEIGHT, wobbuffet_frames[frStart/10]);
				frStart++;
				drawString4(60, 80, "CATCH WOBBUFFET!", 150);
				drawString4(120, 40, "Press START to begin the game.", 100);
				if (frStart/10 >= WOBBUFFET_FRAMES - 12) {
					frStart = 0;
				}
				if (KEY_DOWN_NOW(BUTTON_START))
				{
					numCaught = 0;
					numball = 8;
					numEnemy = NUM_POKE;
					state = PLAY;
					play = 0;
					for (count = 0; count < NUM_POKE; count++) {
						wobbuffet[count].isalive = 1;
					}
				}
				break;
			case PLAY:
				//clearscreen();
				drawImage4(0, 0, BACK_WIDTH, BACK_HEIGHT, back);

				if(KEY_DOWN_NOW(BUTTON_LEFT))
				{
					ash1.col -= 2;;
					ashLeft = 1;
				}
				if(KEY_DOWN_NOW(BUTTON_RIGHT))
				{
					ash1.col += 2;
					ashLeft = 0;
				}
				int l;
				for (l = 0; l < NUM_POKE; l++)
				{
					wobbuffet[l].col += wobbuffet[l].cd;
				}
				if (ball.isthere == 1) {
					drawImage4(ball.row, ball.col, POKEBALL_WIDTH, POKEBALL_HEIGHT, pokeball_frames[frBall]);
					ball.row -= 5;
				}
				frBall++;
				if (frBall >= POKEBALL_FRAMES) {
					frBall = 0;
				}
				if (ball.row < 3) {
					ball.isthere = 0;
				}
				if(KEY_DOWN_NOW(BUTTON_A) && ball.isthere == 0) 
				{
					ball.col = ash1.col;
					ball.row = ash1.row;
					ball.isthere = 1;
					numball--;
				}
				
				for (l = 0; l < NUM_POKE; l++) {
					limitCheck(&wobbuffet[l].col, &wobbuffet[l].cd, MAX_WIDTH, wobbuffet[l].size);
				}
				limitCheck(&ash1.col, &ash1.cd, MAX_WIDTH, ash1.size);
				if (ashLeft == 1) {
					drawImage4(ash1.row, ash1.col, ASH2_WIDTH, ASH2_HEIGHT, ash2);
				} else {
					drawImage4(ash1.row, ash1.col, ASH_WIDTH, ASH_HEIGHT, ash);
				}
				for (l = 0; l < NUM_POKE; l++) 
				{
					if (wobbuffet[l].isalive == 1) {
						drawImage4(wobbuffet[l].row, wobbuffet[l].col, WINGULL_WIDTH, WINGULL_HEIGHT, wingull_frames[fr]);
					}
					if (ball.isthere == 1 && wobbuffet[l].isalive == 1) {
						if (abs(wobbuffet[l].col + (WINGULL_WIDTH / 2) - ball.col + (POKEBALL_WIDTH / 2)) <= POKEBALL_WIDTH / 2) {
							if (wobbuffet[l].row + WINGULL_HEIGHT >= ball.row && wobbuffet[l].row <= ball.row + POKEBALL_HEIGHT) {
								wobbuffet[l].isalive = 0;
								ball.isthere = 0;
								numEnemy--;
								numCaught++;
							}
						}
					}
					if (fr >= WINGULL_FRAMES) {
						fr = 0;
					}
				}
				for (count = 0; count < numball; count++) {
					drawImage4(3, 5 + count * 6, LIFE_WIDTH, LIFE_HEIGHT, life);
				}
				for (count = 0; count < numCaught; count++) {
					drawImage4(3, 230 - count * 8, CAUGHT_WIDTH, CAUGHT_HEIGHT, caught);
				}
				waitForVblank();
				fr++;
				state = PLAY;
				drawString4(10, 3, "BALL", 100);
				drawString4(10, 200, "CAUGHT", 100);
				if(KEY_DOWN_NOW(BUTTON_SELECT))
				{
					state = START;
				}
				else if (numball < 0) {
					if (numCaught > 4) {
						state = WIN;
					} else {
						state = LOSE;
					}
				} 

				break;
			case WIN:
				drawImage4(0, 0, WIN_WIDTH, WIN_HEIGHT, win_frames[frWin]);
				if (KEY_DOWN_NOW(BUTTON_B))
				{
					state = START;
					play = 0;
				} 
				drawString4(70, 82, "You Won!", 100);
				drawString4(100, 35, "Press B to go back to main menu.", 100);
				break;
			case LOSE:
				drawImage4(0, 0, LOST_WIDTH, LOST_HEIGHT, lost_frames[frLost]);

				if (KEY_DOWN_NOW(BUTTON_B))
				{
					state = START;
					play = 0;
				}
				drawString4(70, 78, "You Lost!", 100);
				drawString4(80, 10, "You have to catch more than 5 to win.", 100);
				drawString4(100, 35, "Press B to go back to main menu.", 100);
				break;
		}
		waitForVblank();
		flipPage();

	}
	return 0;
}

		
