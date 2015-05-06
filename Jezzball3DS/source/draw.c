#include "draw.h"

#include "../textures/verdana14_000000.h"
#include "../textures/verdana14_000000_data.h"

#include "../textures/verdana14_000000_bold.h"
#include "../textures/verdana14_000000_bold_data.h"

#include "../textures/verdana11_000000.h"
#include "../textures/verdana11_000000_data.h"

#include "../textures/verdana10_282828.h"
#include "../textures/verdana10_282828_data.h"

/*BmpFile* verdana14Image = (BmpFile*)verdana14_000000_reg_bmp;
BmpFile* verdana14BoldImage = (BmpFile*)verdana14_000000_bmp;
BmpFile* verdana11Image = (BmpFile*)verdana11_000000_bmp;
BmpFile* verdana10Image = (BmpFile*)verdana10_282828_bmp;*/

sf2d_texture* textures[50];
u8 textureCount = 0;

TextureToDraw texturesToDraw[3][50];
u8 texturesToDrawCount[3];

//u8* frameBuffers[3];
Framebuffer frameBuffers[3];

sf2d_texture* createTexture(unsigned int width, unsigned int height, unsigned char* pixelData)
{
	sf2d_texture* texture = sf2d_create_texture(width, height, GPU_RGBA8, SF2D_PLACE_RAM);
	sf2d_fill_texture_from_RGBA8(texture, pixelData, width, height);
	sf2d_texture_tile32(texture);

	textures[textureCount] = texture;
	textureCount++;
	return texture;
}

sf2d_texture* createEmptyTexture(int width, int height)
{
	sf2d_texture* texture = sf2d_create_texture(width, height, GPU_RGBA8, SF2D_PLACE_RAM);
	textures[textureCount] = texture;
	textureCount++;
	return texture;
}

void drawInit()
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x52, 0x52, 0x52, 0xFF));
	sf2d_set_3D(1);

	Framebuffer topLeftFB;
	topLeftFB.texture = createEmptyTexture(FB_TEXTURE_WIDTH, FB_TEXTURE_HEIGHT); 
	topLeftFB.data = (u8*)calloc(FB_TEXTURE_WIDTH * FB_TEXTURE_HEIGHT * 4 + 1, sizeof(u8));

	u32 i;
	for(i =0; i < FB_TEXTURE_WIDTH * SCREEN_HEIGHT * 4; i++) topLeftFB.data[i] = 0;

	frameBuffers[SCREEN_TOP_LEFT] = topLeftFB;

	Framebuffer topRightFB;
	topRightFB.texture = createEmptyTexture(FB_TEXTURE_WIDTH, FB_TEXTURE_HEIGHT); 
	topRightFB.data = (u8*)calloc(FB_TEXTURE_WIDTH * FB_TEXTURE_HEIGHT * 4 + 1, sizeof(u8));
	
	frameBuffers[SCREEN_TOP_RIGHT] = topRightFB;

	Framebuffer bottomFB;
	bottomFB.texture = createEmptyTexture(FB_TEXTURE_WIDTH, FB_TEXTURE_HEIGHT); 
	bottomFB.data = (u8*)calloc(FB_TEXTURE_WIDTH * FB_TEXTURE_HEIGHT * 4 + 1, sizeof(u8));

	frameBuffers[SCREEN_BOTTOM] = bottomFB;
	
}

/*void updateFramebuffers()
{
	frameBuffers[SCREEN_TOP_LEFT] = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	frameBuffers[SCREEN_TOP_RIGHT] = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	frameBuffers[SCREEN_BOTTOM] = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
}*/

u8* getFramebuffer(u8 screen)
{
	if(screen == SCREEN_BOTTOM)
		return frameBuffers[screen].data;
	return (u8*)frameBuffers[screen].texture->data;
}

void drawPixelBuf(u8* framebuffer, u16 X, u16 Y, u8 R, u8 G, u8 B)
{
	if(framebuffer == NULL) return;

	u32 pos = X * 4 + Y * 4 * FB_TEXTURE_WIDTH;
	/*framebuffer[pos + 0] = RGB & 0xFF;
	framebuffer[pos + 1] = (RGB >> 8) & 0xFF;
	framebuffer[pos + 2] = (RGB >> 16) & 0xFF;*/
	framebuffer[pos + 0] = R;
	framebuffer[pos + 1] = G;
	framebuffer[pos + 2] = B;
	framebuffer[pos + 3] = 0xFF;
}

void drawPixel(u8 screen, u16 X, u16 Y, u8 R, u8 G, u8 B)
{
	drawPixelBuf(getFramebuffer(screen), X, Y, R, G, B);
}

void drawPixelLeftEye(u16 X, u16 Y, u8 R, u8 G, u8 B, int d)
{
	drawPixel(SCREEN_TOP_LEFT, X - (d*CONFIG_3D_SLIDERSTATE), Y, R, G, B);
}

void drawPixelRightEye(u16 X, u16 Y, u8 R, u8 G, u8 B, int d)
{
	if(CONFIG_3D_SLIDERSTATE > 0.0f)
		drawPixel(SCREEN_TOP_RIGHT, X + (d*CONFIG_3D_SLIDERSTATE), Y, R, G, B);
}

void drawPixel3D(u16 X, u16 Y, u8 R, u8 G, u8 B, int d) 
{
	drawPixelLeftEye(X, Y, R, G, B, d);
	drawPixelRightEye(X, Y, R, G, B, d);
}

void clearRectBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, Rect* rect, u8 R, u8 G, u8 B, u8 A)
{
	if(framebuffer == NULL) return;

	u32 x,y;
	for(x = rect->x; x < rect->x+rect->w; x++)
	{
		for(y = rect->y; y < rect->y+rect->h; y++)
		{
			if(x < 0 || y < 0 || x >= bufWidth || y >= bufHeight) continue;
			u32 scr_pix = x * 4 + y * 4 * FB_TEXTURE_WIDTH;
			framebuffer[scr_pix+0] = R;
			framebuffer[scr_pix+1] = G;
			framebuffer[scr_pix+2] = B;
			framebuffer[scr_pix+3] = A;
		}
	}
}

void clearRect(u8 screen, Rect* rect, u8 R, u8 G, u8 B, u8 A)
{
	clearRectBuf(getFramebuffer(screen), (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, rect, R, G, B, A);  
}

void drawRectBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, Rect* rect, u8 R, u8 G, u8 B, u8 A)
{
	if(framebuffer == NULL || A == 0) return;

	u32 x,y;
	for(x = rect->x; x < rect->x+rect->w; x++)
	{
		for(y = rect->y; y < rect->y+rect->h; y++)
		{
			if(x < 0 || y < 0 || x >= bufWidth || y >= bufHeight) continue;
			u32 scr_pix = x * 4 + y * 4 * FB_TEXTURE_WIDTH;

			if(A < 0xFF) //Alpha blend
			{
				framebuffer[scr_pix]  = (((A + 1) * R + (256 - A) * framebuffer[scr_pix]) >> 8);
				framebuffer[scr_pix+1] = (((A + 1) * G + (256 - A) * framebuffer[scr_pix+1]) >> 8);
				framebuffer[scr_pix+2] = (((A + 1) * B + (256 - A) * framebuffer[scr_pix+2]) >> 8);
				framebuffer[scr_pix+3] = (framebuffer[scr_pix+3] == 0) ? A : 0xFF;
			}
			else
			{
				framebuffer[scr_pix] = R;
				framebuffer[scr_pix+1] = G;
				framebuffer[scr_pix+2] = B;
				framebuffer[scr_pix+3] = A;
			}
		}
	}

}

void drawRect(u8 screen, Rect* rect, u8 R, u8 G, u8 B, u8 A)
{
	drawRectBuf(getFramebuffer(screen), (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, rect, R, G, B, A);  
}

void drawRectLeftEye(Rect* rect, u8 R, u8 G, u8 B, u8 A, int d)
{
	u32 x = rect->x;
	rect->x = x - (d*CONFIG_3D_SLIDERSTATE);
	drawRect(SCREEN_TOP_LEFT, rect, R, G, B, A);
}

void drawRectRightEye(Rect* rect, u8 R, u8 G, u8 B, u8 A, int d)
{
	if(CONFIG_3D_SLIDERSTATE > 0.0f)
	{
		u32 x = rect->x;
		rect->x = x + (d*CONFIG_3D_SLIDERSTATE);
		drawRect(SCREEN_TOP_RIGHT, rect, R, G, B, A);
	}
}

void drawRect3D(Rect* rect, u8 R, u8 G, u8 B, u8 A, int d)
{
	drawRectLeftEye(rect, R, G, B, A, d);
	drawRectRightEye(rect, R, G, B, A, d);
}


void drawBmpBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, BmpFile* pBmp, Rect* src, Rect* dest)
{
	if(framebuffer == NULL)
		return;

	if(pBmp == NULL)
		return drawRectBuf(framebuffer,bufWidth, bufHeight, dest,255,255,255,255);

	u32 img_width = pBmp->info.width;
	u8* img_data = pBmp->image;

	u32 x,y;
	for(x = 0; x < src->w; x++)
	{
		for(y = 0; y < src->h; y++)
		{
			u32 scr_x = dest->x + x;
			u32 scr_y = dest->y + y;
			u32 scr_pix = (scr_x*bufHeight+(bufHeight-1-scr_y))*3;
			if(scr_x < 0 || scr_y < 0 || scr_x >= bufWidth || scr_y >= bufHeight) continue;

			u32 img_x = src->x + x;                                        //0
			//u32 img_y = src->y + (src->h-y-1);        //7 pBmp->info.height-1-src->y-y   9-0-1 
			u32 img_y = pBmp->info.height-1-src->y-y;
			u32 img_pix = (img_x + img_y*pBmp->info.width)*3;

			unsigned char r = img_data[img_pix+0];
			unsigned char g = img_data[img_pix+1];
			unsigned char b = img_data[img_pix+2];

			if(!(r == 255 && g == 0 && b == 255))
			{
				framebuffer[scr_pix+0] = r;
				framebuffer[scr_pix+1] = g;
				framebuffer[scr_pix+2] = b;
			}
		}
	}
}

void drawImageBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest)
{
	if(framebuffer == NULL)
		return;

	if(imageData == NULL)
		return drawRectBuf(framebuffer,bufWidth, bufHeight, dest,255,255,255,255);

	u32 x,y;

	for(x = 0; x < src->w; x++)
	{
		for(y = 0; y < src->h; y++)
		{
			if(src->x + x < 0 || src->y + y < 0 || src->x + x >= imageWidth || src->y + y >= imageHeight) continue;
			if(dest->x + x < 0 || dest->y + y < 0 || dest->x + x >= bufWidth || dest->y + y >= bufHeight) continue;

			u32 src_pix = (src->x + x) * 4 + (src->y + y) * 4 * imageWidth;
			u32 dest_pix = (dest->x + x) * 4 + (dest->y + y) * 4 * FB_TEXTURE_WIDTH;

			if(imageData[src_pix + 3] > 0)// alpha > 0
			{
				if(imageData[src_pix + 3] < 0xFF) //alpha blend
				{
					framebuffer[dest_pix]   = (((imageData[src_pix + 3] + 1) * imageData[src_pix] + (256 - imageData[src_pix + 3]) * framebuffer[dest_pix]) >> 8);
					framebuffer[dest_pix+1] = (((imageData[src_pix + 3] + 1) * imageData[src_pix + 1] + (256 - imageData[src_pix + 3]) * framebuffer[dest_pix+1]) >> 8);
					framebuffer[dest_pix+2] = (((imageData[src_pix + 3] + 1) * imageData[src_pix + 2] + (256 - imageData[src_pix + 3]) * framebuffer[dest_pix+2]) >> 8);
					framebuffer[dest_pix+3] = (framebuffer[dest_pix+3] == 0) ? imageData[src_pix + 3] : 0xFF;
				}
				else
				{
					framebuffer[dest_pix] = imageData[src_pix];
					framebuffer[dest_pix+1] = imageData[src_pix + 1];
					framebuffer[dest_pix+2] = imageData[src_pix + 2];
					framebuffer[dest_pix+3] = 0xFF;
				}
			}
		}
	}
}

void drawBmp(u8 screen, BmpFile* pBmp, Rect* src, Rect* dest)
{
	drawBmpBuf(getFramebuffer(screen), (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, pBmp, src, dest);
}

void drawImage(u8 screen, u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest)
{
	drawImageBuf(getFramebuffer(screen), (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, imageData, imageWidth, imageHeight, src, dest);
}

void drawBmpLeftEye(BmpFile* pBmp, Rect* src, Rect* dest, int d)
{
	u32 x = dest->x;
	dest->x = x - (d*CONFIG_3D_SLIDERSTATE);
	drawBmp(SCREEN_TOP_LEFT, pBmp, src, dest);
}

void drawImageLeftEye(u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest, int d)
{
	u32 x = dest->x;
	dest->x = x - (d*CONFIG_3D_SLIDERSTATE);
	drawImage(SCREEN_TOP_LEFT, imageData, imageWidth, imageHeight, src, dest);
}

void drawBmpRightEye(BmpFile* pBmp, Rect* src, Rect* dest, int d)
{
	if(CONFIG_3D_SLIDERSTATE > 0.0f)
	{
		u32 x = dest->x;
		dest->x = x + (d*CONFIG_3D_SLIDERSTATE);
		drawBmp(SCREEN_TOP_RIGHT, pBmp, src, dest);
	}
}

void drawImageRightEye(u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest, int d)
{
	if(CONFIG_3D_SLIDERSTATE > 0.0f)
	{
		u32 x = dest->x;
		dest->x = x + (d*CONFIG_3D_SLIDERSTATE);
		drawImage(SCREEN_TOP_RIGHT, imageData, imageWidth, imageHeight, src, dest);
	}
}

void drawBmp3D(BmpFile* pBmp, Rect* src, Rect* dest, int d)
{
	drawBmpLeftEye(pBmp,src,dest,d);
	drawBmpRightEye(pBmp,src,dest,d);
}

void drawImage3D(u8* imageData, u32 imageWidth, u32 imageHeight, Rect* src, Rect* dest, int d)
{
	drawImageLeftEye(imageData, imageWidth, imageHeight, src,dest,d);
	drawImageRightEye(imageData, imageWidth, imageHeight, src,dest,d);
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
			fontInfo = verdana14_000000_bold;
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

void drawStringBuf(u8 *framebuffer, FontFamily fontFamily, u32 bufWidth, u32 bufHeight, u16 X, u16 Y, const char *str, TextAlign textAlign) {
	
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
			fontInfo = verdana14_000000_bold;
			fontImage = verdana14_000000_bold_data.pixel_data;
			fontWidth = verdana14_000000_bold_width;
			fontHeight = verdana14_000000_bold_height;
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

	if(framebuffer == NULL || str == NULL) return;

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

			drawImageBuf(framebuffer, bufWidth, bufHeight, fontImage, fontWidth,fontHeight, &fontInfo[*s], &dest);
			dest.x += fontInfo[*s].w + CHARACTER_PADDING;
		}
		++s;
	}
}

void drawString(u8 screen, FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign) 
{
	drawStringBuf(getFramebuffer(screen), fontFamily, (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, X, Y, str, textAlign);
}

void drawStringLeftEye(FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign, int d)
{
	drawString(SCREEN_TOP_LEFT, fontFamily, X-(d*CONFIG_3D_SLIDERSTATE), Y, str, textAlign);
}

void drawStringRightEye(FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign, int d)
{
	if(CONFIG_3D_SLIDERSTATE > 0.0f)
		drawString(SCREEN_TOP_RIGHT, fontFamily, X+(d*CONFIG_3D_SLIDERSTATE), Y, str, textAlign);
}

void drawString3D(FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign, int d) 
{
	drawStringLeftEye(fontFamily, X, Y, str, textAlign, d);
	drawStringRightEye(fontFamily, X, Y, str, textAlign, d);
}

void drawTexture(u8 screen, sf2d_texture* texture, Rect* src, Rect* dest)
{
	TextureToDraw ttd;
	ttd.texture = texture;
	Rect s = { src->x, src->y, src->w, src->h };
	ttd.src = s;
	Rect d = { dest->x, dest->y, dest->w, dest->h };
	ttd.dest = d;
	texturesToDraw[screen][texturesToDrawCount[screen]] = ttd;
	texturesToDrawCount[screen]++;
}

void drawTextureLeftEye(sf2d_texture* texture, Rect* src, Rect* dest, int d)
{
	u32 x = dest->x;
	dest->x = x - (d*CONFIG_3D_SLIDERSTATE);
	drawTexture(SCREEN_TOP_LEFT, texture, src, dest);
}

void drawTextureRightEye(sf2d_texture* texture, Rect* src, Rect* dest, int d)
{
	if(CONFIG_3D_SLIDERSTATE > 0.0f)
	{
		u32 x = dest->x;
		dest->x = x + (d*CONFIG_3D_SLIDERSTATE);
		drawTexture(SCREEN_TOP_RIGHT, texture, src, dest);
	}
}

void drawTexture3D(sf2d_texture* texture, Rect* src, Rect* dest, int d)
{
	drawTextureLeftEye(texture, src, dest, d);
	drawTextureRightEye(texture, src, dest, d);
}

void drawFinish()
{
	u8 i;

	for(i =0; i< 3; i++)
	{
		free(frameBuffers[i].data);
	}
	for(i =0; i < textureCount; i++)
	{
		sf2d_free_texture(textures[i]);
	}

	sf2d_fini();
}

void draw()
{
	u32 i = 0;

	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	//sf2d_fill_texture_from_RGBA8(frameBuffers[SCREEN_TOP_LEFT].texture, frameBuffers[SCREEN_TOP_LEFT].data, FB_TEXTURE_WIDTH,FB_TEXTURE_HEIGHT);
	sf2d_texture_tile32(frameBuffers[SCREEN_TOP_LEFT].texture);
	
	for(i =0; i < texturesToDrawCount[SCREEN_TOP_LEFT]; i++)
	{
		sf2d_draw_texture_part(texturesToDraw[SCREEN_TOP_LEFT][i].texture, 
			texturesToDraw[SCREEN_TOP_LEFT][i].dest.x, 
			texturesToDraw[SCREEN_TOP_LEFT][i].dest.y, 
			texturesToDraw[SCREEN_TOP_LEFT][i].src.x, 
			texturesToDraw[SCREEN_TOP_LEFT][i].src.y, 
			texturesToDraw[SCREEN_TOP_LEFT][i].src.w, 
			texturesToDraw[SCREEN_TOP_LEFT][i].src.h);
	}
	texturesToDrawCount[SCREEN_TOP_LEFT] = 0;

	sf2d_draw_texture(frameBuffers[SCREEN_TOP_LEFT].texture, 0,0);
	sf2d_end_frame();

	if(CONFIG_3D_SLIDERSTATE > 0.0f)
	{
		sf2d_start_frame(GFX_TOP, GFX_RIGHT);
		//sf2d_fill_texture_from_RGBA8(frameBuffers[SCREEN_TOP_RIGHT].texture, frameBuffers[SCREEN_TOP_RIGHT].data, FB_TEXTURE_WIDTH,FB_TEXTURE_HEIGHT);
		sf2d_texture_tile32(frameBuffers[SCREEN_TOP_RIGHT].texture);
		

		for(i =0; i < texturesToDrawCount[SCREEN_TOP_RIGHT]; i++)
		{
			sf2d_draw_texture_part(texturesToDraw[SCREEN_TOP_RIGHT][i].texture, 
				texturesToDraw[SCREEN_TOP_RIGHT][i].dest.x, 
				texturesToDraw[SCREEN_TOP_RIGHT][i].dest.y, 
				texturesToDraw[SCREEN_TOP_RIGHT][i].src.x, 
				texturesToDraw[SCREEN_TOP_RIGHT][i].src.y, 
				texturesToDraw[SCREEN_TOP_RIGHT][i].src.w, 
				texturesToDraw[SCREEN_TOP_RIGHT][i].src.h);
		}
		texturesToDrawCount[SCREEN_TOP_RIGHT] = 0;

		sf2d_draw_texture(frameBuffers[SCREEN_TOP_RIGHT].texture, 0,0);
		sf2d_end_frame();
	}

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
	sf2d_fill_texture_from_RGBA8(frameBuffers[SCREEN_BOTTOM].texture, frameBuffers[SCREEN_BOTTOM].data, FB_TEXTURE_WIDTH,FB_TEXTURE_HEIGHT);
	sf2d_texture_tile32(frameBuffers[SCREEN_BOTTOM].texture);
	

	for(i =0; i < texturesToDrawCount[SCREEN_BOTTOM]; i++)
	{
		sf2d_draw_texture_part(texturesToDraw[SCREEN_BOTTOM][i].texture, 
			texturesToDraw[SCREEN_BOTTOM][i].dest.x, 
			texturesToDraw[SCREEN_BOTTOM][i].dest.y, 
			texturesToDraw[SCREEN_BOTTOM][i].src.x, 
			texturesToDraw[SCREEN_BOTTOM][i].src.y, 
			texturesToDraw[SCREEN_BOTTOM][i].src.w, 
			texturesToDraw[SCREEN_BOTTOM][i].src.h);
	}
	texturesToDrawCount[SCREEN_BOTTOM] = 0;

	sf2d_draw_texture(frameBuffers[SCREEN_BOTTOM].texture, 0, 0);
	sf2d_end_frame();

	sf2d_swapbuffers();

	memset(getFramebuffer(SCREEN_TOP_LEFT), 0, sizeof(u8)*FB_TEXTURE_WIDTH * SCREEN_HEIGHT * 4);

	if(CONFIG_3D_SLIDERSTATE > 0.0f)
		memset(getFramebuffer(SCREEN_TOP_RIGHT), 0, sizeof(u8)*FB_TEXTURE_WIDTH * SCREEN_HEIGHT * 4);

}