#ifndef _DRAW_H_
#define _DRAW_H_ 

#include <3ds.h>
#include "rect.h"
#include "bmp.h"
#include <stdlib.h>
#include <sf2d.h>

#define SCREEN_HEIGHT 240
#define SCREEN_TOP_WIDTH 400
#define SCREEN_BOTTOM_WIDTH 320
#define SCREEN_TOP_LEFT 0
#define SCREEN_TOP_RIGHT 1
#define SCREEN_BOTTOM 2

#define FB_TEXTURE_WIDTH 512
#define FB_TEXTURE_HEIGHT 256

#define CONFIG_3D_SLIDERSTATE (*(float*)0x1FF81080)
#define CHARACTER_PADDING 1

typedef struct {
	sf2d_texture* texture;
	Rect src;
	Rect dest;
} TextureToDraw;

typedef enum {
	TEXT_ALIGN_LEFT        = 0,
	TEXT_ALIGN_CENTER      = 1,
	TEXT_ALIGN_RIGHT       = 2
} TextAlign;

typedef enum {
	FONTFAMILY_VERDANA14		      = 0,
	FONTFAMILY_VERDANA14_BOLD         = 1,
	FONTFAMILY_VERDANA11              = 2,
	FONTFAMILY_VERDANA10			  = 3,
} FontFamily;

typedef struct {
	u8* data;
	sf2d_texture* texture;
} Framebuffer;

//void updateFramebuffers();
u8* getFramebuffer(u8 screen);

sf2d_texture* createEmptyTexture(int width, int height);
sf2d_texture* createTexture(unsigned int width, unsigned int height, unsigned char* pixelData);

void drawInit();
void drawFinish();

void drawPixelBuf(u8* framebuffer, u16 X, u16 Y, u8 R, u8 G, u8 B);
void drawPixel(u8 screen, u16 X, u16 Y, u8 R, u8 G, u8 B);
void drawPixel3D(u16 X, u16 Y, u8 R, u8 G, u8 B, int d);

void clearRectBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, Rect* rect, u8 R, u8 G, u8 B, u8 A);
void clearRect(u8 screen, Rect* rect, u8 R, u8 G, u8 B, u8 A);

void drawRectBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, Rect* rect, u8 R, u8 G, u8 B, u8 A);
void drawRect(u8 screen, Rect* rect, u8 R, u8 G, u8 B, u8 A);
void drawRect3D(Rect* rect, u8 R, u8 G, u8 B, u8 A, int d);

void drawTexture(u8 screen, sf2d_texture* texture, Rect* src, Rect* dest);
void drawTexture3D(sf2d_texture* texture, Rect* src, Rect* dest, int d);

void drawBmpBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, BmpFile* pBmp, Rect* src, Rect* dest);
void drawImageBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest);
void drawBmp(u8 screen, BmpFile* pBmp, Rect* src, Rect* dest);
void drawImage(u8 screen, u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest);
void drawBmp3D(BmpFile* pBmp, Rect* src, Rect* dest, int d);
void drawImage3D(u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest, int d);

u32 getCharWidth(FontFamily fontFamily, char c);

void drawStringBuf(u8 *framebuffer, FontFamily fontFamily, u32 bufWidth, u32 bufHeight, u16 X, u16 Y, const char *str, TextAlign textAlign);
void drawString(u8 screen, FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign);
void drawString3D(FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign, int d);

void draw();

#endif
