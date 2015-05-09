#include "draw.h"

#include "../textures/verdana14_000000.h"
#include "../textures/verdana14_000000_data.h"

#include "../textures/verdana14_ffffff_bold.h"
#include "../textures/verdana14_ffffff_bold_data.h"

#include "../textures/verdana11_000000.h"
#include "../textures/verdana11_000000_data.h"

#include "../textures/verdana10_282828.h"
#include "../textures/verdana10_282828_data.h"

static u32* xfb = NULL;
u32* dbuffer = NULL;
static GXRModeObj *rmode = NULL;
u32 bufferSize = 0;

u32 rgbToY1cby2cr(u8 r1, u8 g1, u8 b1)
{
	int y1, cb1, cr1, y2, cb2, cr2, cb, cr;
	u8 r2, g2, b2;

	r2 = r1;
	g2 = g1;
	b2 = b1;

	y1 = (299 * r1 + 587 * g1 + 114 * b1) / 1000;
	cb1 = (-16874 * r1 - 33126 * g1 + 50000 * b1 + 12800000) / 100000;
	cr1 = (50000 * r1 - 41869 * g1 - 8131 * b1 + 12800000) / 100000;

	y2 = (299 * r2 + 587 * g2 + 114 * b2) / 1000;
	cb2 = (-16874 * r2 - 33126 * g2 + 50000 * b2 + 12800000) / 100000;
	cr2 = (50000 * r2 - 41869 * g2 - 8131 * b2 + 12800000) / 100000;

	cb = (cb1 + cb2) >> 1;
	cr = (cr1 + cr2) >> 1;

	return ((y1 << 24) | (cb << 16) | (y2 << 8) | cr);
}

void drawInit()
{

	VIDEO_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	bufferSize = rmode->fbWidth*rmode->xfbHeight*2;
	WPAD_SetVRes(0, rmode->fbWidth, rmode->xfbHeight);
	dbuffer = malloc(bufferSize);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();

	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

}

void drawPixel(int x, int y, u8 r, u8 g, u8 b)
{
	dbuffer[(y * 320) + (x >> 1)] = rgbToY1cby2cr(r, g, b);
}

void drawRect(Rect* rect, u8 R, u8 G, u8 B, u8 A)
{
	u32 bufWidth = rmode->fbWidth;
	u32 bufHeight = rmode->xfbHeight;

	u32 x,y;
	for(x = rect->x*DRAWING_SCALE; x < (rect->x*DRAWING_SCALE)+(rect->w*DRAWING_SCALE); x++)
	{
		for(y = rect->y*DRAWING_SCALE; y < (rect->y*DRAWING_SCALE)+(rect->h*DRAWING_SCALE); y++)
		{
			if(x < 0 || y < 0 || x >= bufWidth || y >= bufHeight) continue;

			drawPixel(x,y, R,G,B);
		}
	}
}

void drawImageN(u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest, u8 bgR, u8 bgG, u8 bgB)
{
	u32 bufWidth = rmode->fbWidth;
	u32 bufHeight = rmode->xfbHeight;

	if (imageData == NULL)
		return;

	u32 x, y;

	for (x = 0; x < src->w; x++)
	{
		for (y = 0; y < src->h; y++)
		{
			if ((src->x) + x < 0 || (src->y) + y < 0 || (src->x) + x >= imageWidth || (src->y) + y >= imageHeight) continue;
			if ((dest->x) + x < 0 || (dest->y) + y < 0 || (dest->x) + x >= bufWidth || (dest->y) + y >= bufHeight) continue;

			u32 src_pix = ((src->x) + x) * 4 + ((src->y) + y) * 4 * imageWidth;

			if (imageData[src_pix + 3] > 0)// alpha > 0
			{
				if(imageData[src_pix + 3] < 0xFF) //alpha blend
				{
					drawPixel((dest->x) + x, (dest->y) + y, 
						(((imageData[src_pix + 3] + 1) * imageData[src_pix] + (256 - imageData[src_pix + 3]) * bgR) >> 8), 
						(((imageData[src_pix + 3] + 1) * imageData[src_pix + 1] + (256 - imageData[src_pix + 3]) * bgG) >> 8), 
						(((imageData[src_pix + 3] + 1) * imageData[src_pix + 2] + (256 - imageData[src_pix + 3]) * bgB) >> 8));
				}
				else
					drawPixel((dest->x) + x, (dest->y) + y, imageData[src_pix], imageData[src_pix + 1], imageData[src_pix+2]);
			}
			
		}
	}
}

void drawImage(u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest)
{
	u32 bufWidth = rmode->fbWidth;
	u32 bufHeight = rmode->xfbHeight;

	if (imageData == NULL)
		return;

	u32 x, y;

	for (x = 0; x < src->w*DRAWING_SCALE; x++)
	{
		for (y = 0; y < src->h*DRAWING_SCALE; y++)
		{
			if ((src->x*DRAWING_SCALE) + x < 0 || (src->y*DRAWING_SCALE) + y < 0 || (src->x*DRAWING_SCALE) + x >= imageWidth || (src->y*DRAWING_SCALE) + y >= imageHeight) continue;
			if ((dest->x*DRAWING_SCALE) + x < 0 || (dest->y*DRAWING_SCALE) + y < 0 || (dest->x*DRAWING_SCALE) + x >= bufWidth || (dest->y*DRAWING_SCALE) + y >= bufHeight) continue;

			u32 src_pix = ((src->x*DRAWING_SCALE) + x) * 4 + ((src->y*DRAWING_SCALE) + y) * 4 * imageWidth;

			if (imageData[src_pix + 3] > 0)// alpha > 0
			{
				if(imageData[src_pix + 3] < 0xFF) //alpha blend
				{
					drawPixel((dest->x*DRAWING_SCALE) + x, (dest->y*DRAWING_SCALE) + y, 
						(((imageData[src_pix + 3] + 1) * imageData[src_pix] + (256 - imageData[src_pix + 3]) * 255) >> 8), 
						(((imageData[src_pix + 3] + 1) * imageData[src_pix + 1] + (256 - imageData[src_pix + 3]) * 255) >> 8), 
						(((imageData[src_pix + 3] + 1) * imageData[src_pix + 2] + (256 - imageData[src_pix + 3]) * 255) >> 8));
				}
				else
					drawPixel((dest->x*DRAWING_SCALE) + x, (dest->y*DRAWING_SCALE) + y, imageData[src_pix], imageData[src_pix + 1], imageData[src_pix+2]);
			}
			
		}
	}
}

u32 getCharWidth(FontFamily fontFamily, char c)
{
	Rect* fontInfo;

	switch(fontFamily)
	{
		case FONTFAMILY_VERDANA14:
			fontInfo = verdana14_000000;
		break;
		case FONTFAMILY_VERDANA14_BOLD:
			fontInfo = verdana14_ffffff_bold;
		break;
		case FONTFAMILY_VERDANA11:
			fontInfo = verdana11_000000;
		break;
		case FONTFAMILY_VERDANA10:
			fontInfo = verdana10_282828;
		break;
	}

	return fontInfo[c].w + CHARACTER_PADDING;
}



void drawString(FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign, u8 bgR, u8 bgG, u8 bgB) 
{
	
	u8 offset = 0;
	Rect* fontInfo;
	u8* fontImage;
	u16 fontWidth;
	u16 fontHeight;

	switch(fontFamily)
	{
		case FONTFAMILY_VERDANA14:
			fontInfo = verdana14_000000;
			fontImage = verdana14_000000_data.pixel_data;
			fontWidth = verdana14_000000_width;
			fontHeight = verdana14_000000_height;
		break;
		case FONTFAMILY_VERDANA14_BOLD:
			fontInfo = verdana14_ffffff_bold;
			fontImage = verdana14_ffffff_bold_data.pixel_data;
			fontWidth = verdana14_ffffff_bold_width;
			fontHeight = verdana14_ffffff_bold_height;
		break;
		case FONTFAMILY_VERDANA11:
			fontInfo = verdana11_000000;
			fontImage = verdana11_000000_data.pixel_data;
			fontWidth = verdana11_000000_width;
			fontHeight = verdana11_000000_height;
			//fontImage = verdana11Image;
		break;
		case FONTFAMILY_VERDANA10:
			fontInfo = verdana10_282828;
			fontImage = verdana10_282828_data.pixel_data;
			fontWidth = verdana10_282828_width;
			fontHeight = verdana10_282828_height;
		break;

	}


	Rect dest = {X,Y,0,0};

	if(textAlign == TEXT_ALIGN_RIGHT || textAlign == TEXT_ALIGN_CENTER)
	{
		u32 strWidth = 0;
		const char *g = str;
		while(*g) { if(*g != '\n' && *g != '\t') strWidth += fontInfo[*g].w + CHARACTER_PADDING; ++g; }

		if(textAlign == TEXT_ALIGN_RIGHT)
			dest.x -= strWidth;
		else 
			dest.x -= strWidth/2;
	}

	const char *s = str;
	while(*s) {
		
		if(*s == '\n') {
		} else if(*s == '\t') {
		} else {

			drawImageN(fontImage, fontWidth,fontHeight, &fontInfo[*s], &dest, bgR, bgG, bgB);
			dest.x += fontInfo[*s].w + CHARACTER_PADDING;
		}
		++s;
	}
}


void draw()
{
	memcpy(xfb, dbuffer, bufferSize);
	memset(dbuffer,128,bufferSize);
	//VIDEO_WaitVSync();
}

void drawFinish()
{
	free(dbuffer);
}