#include "mylib.h"
#include "text.h"

void drawChar(int r, int c, char ch, unsigned short color)
{
	int dr;
	int dc;
	for (dr = 0; dr < 8; dr++)
	{
		for (dc = 0; dc < 6; dc++)
		{
			if (fontdata_6x8[OFFSET(dr, dc ,6) + ch * 48])
			{
				setPixel(r + dr, c + dc, color);
			}
		}
	}
}

void drawString(int r, int c, char *str, unsigned short color)
{
	while (*str)
	{
		drawChar(r, c, *str++, color);
		c += 6;
	}
}