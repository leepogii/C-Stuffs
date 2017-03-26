#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "text.h"
#include "win.h"
#include "run.h"
#include "tom.h"
#include "cheese.h"
#include "jerry1.h"
#include "tom_face.h"
extern u16* videoBuffer;
// void setPixel(int r, int c, u16 RGB);
// void drawRect(int r, int c, int width, int height, u16 RGB);
// void drawHollowRect(int r, int c, int width, int height, u16 RGB);
// void drawImage3(int r, int c, int width, int height, const u16* image);
// void drawChar(int r, int c, char ch, unsigned short color);
// void drawString(int r, int c, char *str, unsigned short color);
enum GBAState {
	START,
	PLAY,
	WIN,
	LOSE,
};

struct character 
{
	int r;
	int c;
	int oldr;
	int oldc;
	int rdel;
	int cdel;
	int size;
};
int main(void) 
{
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	enum GBAState state = START;
	struct character jerry = {150, 230, 150, 230, 1, 1, JERRY1_WIDTH};
	struct character tom1 = {10, 20, 10, 20, 2, 4, 8};
	struct character tom2 = {50, 80, 50, 80, 3, 2, 10};
	struct character tom3 = {150, 20, 150, 20, 4, 2, 12};
	int play = 0;
	int special = 1;
	while(1)
	{
		switch(state) {
			case START:
				if (play == 0) {
					drawImage3(0, 0, RUN_WIDTH, RUN_HEIGHT, run);
					drawString(45, 75, "TOM & JERRY", YELLOW);
					drawString(120, 35, "Press START Button to start.", RED);
					play = 1;
					special = 1;
				}
				drawHollowRect(35, 70, 25, 80, YELLOW);
				drawHollowRect(33, 68, 29, 84, YELLOW);
				if (KEY_DOWN_NOW(BUTTON_START))
				{
					jerry.r = 150;
					jerry.c = 230;
					tom1.r = 10;
					tom1.c = 20;
					tom2.r = 50;
					tom2.c = 80;
					tom3.r = 150;
					tom3.c = 20;
					drawRect(0, 0, 239, 159, BLACK);
					state = PLAY;
					play = 0;
				}
				break;
			case PLAY:
				if(KEY_DOWN_NOW(BUTTON_UP))
				{
					jerry.r--;
				}
				if(KEY_DOWN_NOW(BUTTON_DOWN))
				{
					jerry.r++;
				}
				if(KEY_DOWN_NOW(BUTTON_LEFT))
				{
					jerry.c--;
				}
				if(KEY_DOWN_NOW(BUTTON_RIGHT))
				{
					jerry.c++;
				}
				tom1.r += tom1.rdel;
				tom1.c += tom1.cdel;
				tom2.r -= tom2.rdel;
				tom2.c += tom2.cdel;
				tom3.r += tom3.rdel;
				tom3.c -= tom3.cdel;
				if(KEY_DOWN_NOW(BUTTON_B) && special == 1) {
					tom1.rdel *= -1;
					tom1.cdel *= -1;
					tom2.rdel *= -1;
					tom2.cdel *= -1;
					tom3.rdel *= -1;
					tom3.cdel *= -1;
				
					special = 0;
				}
				limitCheck(&jerry.r, &jerry.rdel, MAX_HEIGHT, jerry.size);
				limitCheck(&jerry.c, &jerry.cdel, MAX_WIDTH, jerry.size);
				limitCheck(&tom1.r, &tom1.rdel, MAX_HEIGHT, tom1.size);
				limitCheck(&tom1.c, &tom1.cdel, MAX_WIDTH, tom1.size);
				limitCheck(&tom2.r, &tom2.rdel, MAX_HEIGHT, tom2.size);
				limitCheck(&tom2.c, &tom2.cdel, MAX_WIDTH, tom2.size);
				limitCheck(&tom3.r, &tom3.rdel, MAX_HEIGHT, tom3.size);
				limitCheck(&tom3.c, &tom3.cdel, MAX_WIDTH, tom3.size);
				waitForVBlank();
				drawRect(jerry.oldr, jerry.oldc, JERRY1_WIDTH, JERRY1_HEIGHT, BLACK);
				drawImage3(jerry.c, jerry.r, JERRY1_WIDTH, JERRY1_HEIGHT, jerry1);
				drawRect(tom1.oldr, tom1.oldc, tom1.size, tom1.size, BLACK);
				drawImage3(tom1.c, tom1.r, tom1.size, tom1.size, tom_face);
				drawRect(tom2.oldr, tom2.oldc, tom2.size, tom2.size, BLACK);
				drawImage3(tom2.c, tom2.r, tom2.size, tom2.size, tom_face);
				drawRect(tom3.oldr, tom3.oldc, tom3.size, tom3.size, BLACK);
				drawImage3(tom3.c, tom3.r, tom3.size, tom3.size, tom_face);
				drawImage3(0, 0, CHEESE_WIDTH, CHEESE_HEIGHT, cheese);
				drawHollowRect(0, 145, 20, 80, RED);
				drawString(6, 150, "TOM & JERRY", YELLOW);
				drawString(140, 150, "B: Squeak!", PURPLE);
				jerry.oldr = jerry.r;
				jerry.oldc = jerry.c;
				tom1.oldr = tom1.r;
				tom1.oldc = tom1.c;
				tom2.oldr = tom2.r;
				tom2.oldc = tom2.c;
				tom3.oldr = tom3.r;
				tom3.oldc = tom3.c;
				state = PLAY;
				if(KEY_DOWN_NOW(BUTTON_SELECT))
				{
					state = START;
				} else if (jerry.c < CHEESE_WIDTH && jerry.r < CHEESE_HEIGHT) {
					state = WIN;
				} else if (!((jerry.c + jerry.size < tom1.c) || (jerry.c > tom1.c + tom1.size)
					|| ((jerry.r + jerry.size) < tom1.r) || (jerry.r > (tom1.r + tom1.size)))
					|| !((jerry.c + jerry.size < tom2.c) || (jerry.c > tom2.c + tom2.size)
					|| ((jerry.r + jerry.size) < tom2.r) || (jerry.r > (tom2.r + tom2.size)))
					|| !((jerry.c + jerry.size < tom3.c) || (jerry.c > tom3.c + tom3.size)
					|| ((jerry.r + jerry.size) < tom3.r) || (jerry.r > (tom3.r + tom3.size))))
				{
					state = LOSE;
				}
				break;
			case WIN:
				if (play == 0) {
					drawImage3(0, 0, WIN_WIDTH, WIN_HEIGHT, win);
					drawRect(80, 80, 60, 20, RED);
					drawRect(110, 40, 160, 20, RED);
					drawRect(140, 40, 160, 20, BLACK);
					drawString(85, 83, "YOU WON!", YELLOW);
					drawString(115, 45, "Jerry found the cheese!", PURPLE);
					drawString(145, 42, "Press B to go to main menu.", WHITE);
					play = 1;
				}
				if (KEY_DOWN_NOW(BUTTON_B))
				{
					state = START;
					play = 0;
				}
				break;
			case LOSE:
				if (play == 0) {
					drawImage3(0, 0, TOM_WIDTH, TOM_HEIGHT, tom);
					drawRect(80, 80, 60, 20, YELLOW);
					drawRect(110, 40, 160, 20, YELLOW);
					drawRect(140, 40, 160, 20, BLACK);
					drawString(85, 83, "YOU LOST!", RED);
					drawString(115, 45, "You got caught by Tom", RED);
					drawString(145, 42, "Press B to go to main menu.", RED);
					play = 1;
				}
				if (KEY_DOWN_NOW(BUTTON_B))
				{
					state = START;
					play = 0;
				}
				break;
		}

	}
	return 0;
}