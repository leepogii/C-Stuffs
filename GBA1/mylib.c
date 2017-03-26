#include "mylib.h"
u16* videoBuffer = (u16*) 0x6000000;
// A function to set pixel (r, c) to the color passed in.
void setPixel(int r, int c, u16 color)
{
	videoBuffer[OFFSET(r, c, 240)] = color;
}
// A function to draw a FILLED rectangle starting at (r, c)
void drawRect(int r, int c, int width, int height, u16 color)
{
	int dr;
	int dc;
		for (dr = 0; dr < height; dr++)
		{
			for (dc = 0; dc < width; dc++)
			{
				setPixel(r + dr, c + dc, color);
			}
		}
}
// A function to draw a HOLLOW rectangle starting at (r, c)
// NOTE: It has to run in O(w+h) time.
void drawHollowRect(int r, int c, int width, int height, u16 color)
{
	int dr;
	int dc;
	for (dr = 0; dr < width; dr++)
	{
		setPixel(r + dr, c, color); 
		setPixel(r + dr, c + height, color);
	}
	for (dc = 0; dc < height; dc++)
	{
		setPixel(r, c + dc, color);
		setPixel(r + width, c + dc, color);
	}
}

void drawInSquare(int r, int c, int size, u16 color)
{
	drawRect(r, c, size, size, WHITE);
	drawHollowRect(r - 2, c - 2, size + 4, size + 4, color);
}
/* drawimage3
* A function that will draw an arbitrary sized image
* onto the screen
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	int sx;
	int sy;
	int counter = 0;
	for (sx = 0; sx < height; sx++) {
		for (sy = 0; sy < width; sy++) {
			videoBuffer[(c + sx) * 240 + (r + sy)] = image[counter++];
		}
	}
}
void limitCheck(int *var, int *delta, int limit, int size)
{
	if(*var < 0)
	{
		*var = 0;
		*delta = -*delta;
	}
	if(*var > limit - size)
	{
		*var = limit - size;
		*delta = -*delta;
	}
}

void waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
