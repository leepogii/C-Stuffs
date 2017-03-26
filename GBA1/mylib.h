typedef unsigned short u16;
#define OFFSET(r, c, numcols) 	((r) * (numcols) + (c))
#define REG_DISPCNT				*(u16*) 0x4000000
#define RGB(r, g, b)			((b) << 10 | (g) << 5 | (r))
#define BG2_ENABLE				(1 << 10)
#define MODE3					3
#define SCANLINECOUNTER 		*(volatile u16*) 0x4000006
#define MAX_WIDTH		239
#define MAX_HEIGHT		159

//Colors
#define RGB(r, g, b)	((b) << 10 | (g) << 5 | (r))
#define BLACK 			RGB(0 , 0 , 0 )
#define WHITE  			RGB(31, 31, 31)
#define RED    			RGB(31, 0 , 0 )
#define GREEN  			RGB(0 , 31, 0 )
#define BLUE   			RGB(0 , 0 , 31)
#define YELLOW  		RGB(31, 31, 0 )
#define MAGENTA 		RGB(31, 0 , 31)
#define CYAN    		RGB(0 , 31, 31)
#define ORANGE  		RGB(31, 15, 0 )
#define BROWN   		RGB(18, 9 , 0 )
#define PURPLE  		RGB(15, 0 , 15)
#define TEAL    		RGB(0 , 15, 15)
#define MAROON  		RGB(15, 0 , 0 )
#define GREY    		RGB(15, 15, 15)
#define PINK    		RGB(31, 18, 19)

//Buttons
#define BUTTONS       	*(volatile u16*) 0x4000130
#define BUTTON_A      	(1 << 0)
#define BUTTON_B     	(1 << 1)
#define BUTTON_SELECT	(1 << 2)
#define BUTTON_START  	(1 << 3)
#define BUTTON_RIGHT  	(1 << 4)
#define BUTTON_LEFT   	(1 << 5)
#define BUTTON_UP     	(1 << 6)
#define BUTTON_DOWN   	(1 << 7)
#define BUTTON_R      	(1 << 8)
#define BUTTON_L      	(1 << 9)
#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

void setPixel(int r, int c, u16 RGB);
void drawRect(int r, int c, int width, int height, u16 RGB);
void drawHollowRect(int r, int c, int width, int height, u16 RGB);
void drawInSquare(int r, int c, int size, u16 color);
void drawImage3(int r, int c, int width, int height, const u16* image);
void limitCheck(int *var, int *delta, int limit, int size);
void waitForVBlank();
