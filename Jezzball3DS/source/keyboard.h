#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_ 

#include <3ds.h>
#include "bmp.h"
#include "rect.h"
#include "draw.h"
#include "jezzball.h"

u8 isTouchingRect(touchPosition* touchPos, Rect* rect);
void keyboardInit();
void keyboardDraw();
void writeChar(char c);
u8 keyboardUpdate();
void getKeyboardInputText(char* cbuff);

#endif