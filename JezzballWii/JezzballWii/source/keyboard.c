#include "keyboard.h"
#include "../textures/keyboard_data.h"
//#include "keyboard_bmp.h"

//BmpFile* kbImage = (BmpFile*)keyboard_bmp;
//sf2d_texture* kbTexture;

Rect textCursorPos = { 8,11,2,16 }; 
char text[MAX_NAME_LENGTH+1];
u8 charIndex = 0;
u8 lowercase = 1;
u8 blinkTimer = 0;
u8 blink = 0;

Rect key1 = { 3, 70, 24,22 };
Rect key2 = { 28, 70, 24,22 };
Rect key3 = { 52, 70, 24,22 };
Rect key4 = { 77, 70, 24,22 };
Rect key5 = { 102, 70, 24,22 };
Rect key6 = { 127, 70, 24,22 };
Rect key7 = { 152, 70, 24,22 };
Rect key8 = { 177, 70, 24,22 };
Rect key9 = { 203, 70, 24,22 };
Rect key0 = { 227, 70, 24,22 };
Rect keyDash = { 252, 70, 40, 22 };
Rect keyBackspace = { 277, 70, 40, 22 };

Rect keyQ = { 3, 92, 28,22 };
Rect keyW = { 31, 92, 24,22 };
Rect keyE = { 56, 92, 24,22 };
Rect keyR = { 81, 92, 24,22 };
Rect keyT = { 106, 92, 24,22 };
Rect keyY = { 131, 92, 24,22 };
Rect keyU = { 156, 92, 24,22 };
Rect keyI = { 181, 92, 24,22 };
Rect keyO = { 206, 92, 24,22 };
Rect keyP = { 231, 92, 24,22 };

Rect keyA = { 3, 114, 32,22 };
Rect keyS = { 35, 114, 24,22 };
Rect keyD = { 60, 114, 24,22 };
Rect keyF = { 85, 114, 24,22 };
Rect keyG = { 110, 114, 24,22 };
Rect keyH = { 135, 114, 24,22 };
Rect keyJ = { 161, 114, 24,22 };
Rect keyK = { 185, 114, 24,22 };
Rect keyL = { 210, 114, 24,22 };
Rect keySingleQuote = { 235, 114, 24,22 };
Rect keyEqual = { 260, 114, 24,22 };
Rect keySlash = { 285, 114, 32,22 };

Rect keyZ = { 3, 136, 38,22 };
Rect keyX = { 39, 136, 24,22 };
Rect keyC = { 64, 136, 24,22 };
Rect keyV = { 89, 136, 24,22 };
Rect keyB = { 114, 136, 24,22 };
Rect keyN = { 139, 136, 24,22 };
Rect keyM = { 164, 136, 24,22 };
Rect keyComma = { 189, 136, 24,22 };
Rect keyDot = { 214, 136, 24,22 };
Rect keyQMark = { 239, 136, 24,22 };
Rect keyEMark = { 264, 136, 24,22 };
Rect keyAt = { 289, 136, 24,22 };

Rect keyChangeCase = { 3, 159, 54, 22 };
Rect keyShift = { 57, 159, 54, 22 };
Rect keySpace = { 113, 159, 99, 22 };

Rect keyOK = { 160, 218, 160,22 };

void keyboardInit()
{

}

void keyboardDraw()
{

}

void writeChar(char c)
{
    if(charIndex < MAX_NAME_LENGTH)
	{
		textCursorPos.x += getCharWidth(FONTFAMILY_VERDANA14, c);
		text[charIndex] = c;
		charIndex++;
	}
}

u8 keyboardUpdate()
{
	return 0;
}

void getKeyboardInputText(char* cbuff)
{

}