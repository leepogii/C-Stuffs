// Rohan Avalani
#include "mylib.h"

u16 *videoBuffer = (u16 *) 0x6000000;

void setPixel4(int row, int col, u8 index) {
	int pix = OFFSET(row, col, 240);
	int srt = pix / 2;
	if (col & 1) {
		videoBuffer[srt] = (videoBuffer[srt] & 0x00FF) | (index << 8);
	} else {
		videoBuffer[srt] = (videoBuffer[srt] & 0xFF00) | (index);
	}
}

void drawImage4(int r, int c, int width, int height, const u16* image) {
	int i;
	for (i = 0; i < height; i++) {
		DMA[3].src = &image[OFFSET(i, 0, width / 2)];
		DMA[3].dst = &videoBuffer[OFFSET(r + i, c, 240)/2];
		DMA[3].cnt = (width/2) | DMA_ON;
	}
}


void drawRect4(int row, int col, int width, int height, u8 index) {
	volatile u16 color = index | (index << 8);
	int i;
	for(i = 0; i < height; i++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + i, col, 240)/2];	
		DMA[3].cnt = (width/2) | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void fillScreen4(u8 index) {
	volatile u16 color = index | (index << 8);
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (19200) | DMA_ON | DMA_SOURCE_FIXED;
}

void flipPage() {
	if(REG_DISPCTL & BUFFER1FLAG) {
		REG_DISPCTL &= ~BUFFER1FLAG;
		videoBuffer = BUFFER1;
	} else {
		REG_DISPCTL |= BUFFER1FLAG;
		videoBuffer = BUFFER0;
	}
}
void limitCheck(int *var, int *delta, int limit, int size) {
	if (*var < 0) {
		*var = 0;
		*delta = -*delta;
	}
	if (*var > limit - size) {
		*var = limit - size;
		*delta = -*delta;
	}
}

void waitForVblank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}

void clearscreen() {
	fillScreen4(0);
}

void pageFlip() {
	if (REG_DISPCTL & BUFFER1FLAG) {
		REG_DISPCTL = REG_DISPCTL & (~BUFFER1FLAG);
		videoBuffer = BUFFER1;
	} else {
		REG_DISPCTL = REG_DISPCTL | BUFFER1FLAG;
		videoBuffer = BUFFER0;
	}
}

void loadPalette(int size, const u16* colors)
{
	int i;
	for(i = 0; i < size; i++)
	{
		PALETTE[i] = colors[i];
	}
}