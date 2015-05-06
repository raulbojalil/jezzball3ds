#include "keyboard.h"
#include "../textures/keyboard_data.h"
//#include "keyboard_bmp.h"

//BmpFile* kbImage = (BmpFile*)keyboard_bmp;
sf2d_texture* kbTexture;

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
	kbTexture = createTexture(keyboard_data.width, keyboard_data.height, keyboard_data.pixel_data);
}

u8 isTouchingRect(touchPosition* touchPos, Rect* rect)
{
	return (touchPos->px > rect->x && touchPos->px < (rect->x + rect->w) && touchPos->py > rect->y && touchPos->py < (rect->y + rect->h));
}

void keyboardDraw()
{
	Rect rect = {0,0,320,240};
	Rect overlay = { 0, 25, 280, 25 }; 
	//drawImage(SCREEN_BOTTOM,kbImage, &rect, &rect);
	drawTexture(SCREEN_BOTTOM,kbTexture, &rect, &rect);

	Rect clear = { 0,0, SCREEN_BOTTOM_WIDTH, SCREEN_HEIGHT };
	clearRect(SCREEN_BOTTOM, &clear, 0,0,0,0); 

	drawString(SCREEN_BOTTOM, FONTFAMILY_VERDANA14, 10, 7, text, TEXT_ALIGN_LEFT);
	
	if(blink)
		drawRect(SCREEN_BOTTOM, &textCursorPos, 0,0,0,0xFF);

	/*if(charIndex == 0)
		drawString(SCREEN_BOTTOM, FONTFAMILY_VERDANA14, 10, 10, "Please enter your name", TEXT_ALIGN_LEFT);*/
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

	blinkTimer++;

	if(blinkTimer > 10)
	{
		blinkTimer = 0;
		blink++;
		blink&=1;
	}


	touchPosition touchPos;
	hidTouchRead(&touchPos);

	if(isKeyPressed(KEY_TOUCH))
	{
		if(isTouchingRect(&touchPos, &key1)) { writeChar('1'); }
		else if(isTouchingRect(&touchPos, &key2)) { writeChar('2'); }
		else if(isTouchingRect(&touchPos, &key3)) { writeChar('3'); }
		else if(isTouchingRect(&touchPos, &key4)) { writeChar('4'); }
		else if(isTouchingRect(&touchPos, &key5)) { writeChar('5'); }
		else if(isTouchingRect(&touchPos, &key6)) { writeChar('6'); }
		else if(isTouchingRect(&touchPos, &key7)) { writeChar('7'); }
		else if(isTouchingRect(&touchPos, &key8)) { writeChar('8'); }
		else if(isTouchingRect(&touchPos, &key9)) { writeChar('9'); }
		else if(isTouchingRect(&touchPos, &key0)) { writeChar('0'); }
		else if(isTouchingRect(&touchPos, &keyDash)) { writeChar('-'); }

		else if(isTouchingRect(&touchPos, &keyQ)){ if (lowercase) writeChar('q'); else writeChar('Q'); }
		else if(isTouchingRect(&touchPos, &keyW)){ if (lowercase) writeChar('w'); else writeChar('W'); }
		else if(isTouchingRect(&touchPos, &keyE)){ if (lowercase) writeChar('e'); else writeChar('E'); }
		else if(isTouchingRect(&touchPos, &keyR)){ if (lowercase) writeChar('r'); else writeChar('R'); }
		else if(isTouchingRect(&touchPos, &keyT)){ if (lowercase) writeChar('t'); else writeChar('T'); }
		else if(isTouchingRect(&touchPos, &keyY)){ if (lowercase) writeChar('y'); else writeChar('Y'); }
		else if(isTouchingRect(&touchPos, &keyU)){ if (lowercase) writeChar('u'); else writeChar('U'); }
		else if(isTouchingRect(&touchPos, &keyI)){ if (lowercase) writeChar('i'); else writeChar('I'); }
		else if(isTouchingRect(&touchPos, &keyO)){ if (lowercase) writeChar('o'); else writeChar('O'); }
		else if(isTouchingRect(&touchPos, &keyP)){ if (lowercase) writeChar('p'); else writeChar('P'); }

		else if(isTouchingRect(&touchPos, &keyA)){ if (lowercase) writeChar('a'); else writeChar('A'); }
		else if(isTouchingRect(&touchPos, &keyS)){ if (lowercase) writeChar('s'); else writeChar('S'); }
		else if(isTouchingRect(&touchPos, &keyD)){ if (lowercase) writeChar('d'); else writeChar('D'); }
		else if(isTouchingRect(&touchPos, &keyF)){ if (lowercase) writeChar('f'); else writeChar('F'); }
		else if(isTouchingRect(&touchPos, &keyG)){ if (lowercase) writeChar('g'); else writeChar('G'); }
		else if(isTouchingRect(&touchPos, &keyH)){ if (lowercase) writeChar('h'); else writeChar('H'); }
		else if(isTouchingRect(&touchPos, &keyJ)){ if (lowercase) writeChar('j'); else writeChar('J'); }
		else if(isTouchingRect(&touchPos, &keyK)){ if (lowercase) writeChar('k'); else writeChar('K'); }
		else if(isTouchingRect(&touchPos, &keyL)){ if (lowercase) writeChar('l'); else writeChar('L'); }

		else if(isTouchingRect(&touchPos, &keySingleQuote)){ writeChar('\''); }
		else if(isTouchingRect(&touchPos, &keyEqual)){ writeChar('='); }
		else if(isTouchingRect(&touchPos, &keySlash)){ writeChar('/'); }

		else if(isTouchingRect(&touchPos, &keyZ)){ if (lowercase) writeChar('z'); else writeChar('Z'); }
		else if(isTouchingRect(&touchPos, &keyX)){ if (lowercase) writeChar('x'); else writeChar('X'); }
		else if(isTouchingRect(&touchPos, &keyC)){ if (lowercase) writeChar('c'); else writeChar('C'); }
		else if(isTouchingRect(&touchPos, &keyV)){ if (lowercase) writeChar('v'); else writeChar('V'); }
		else if(isTouchingRect(&touchPos, &keyB)){ if (lowercase) writeChar('b'); else writeChar('B'); }
		else if(isTouchingRect(&touchPos, &keyN)){ if (lowercase) writeChar('n'); else writeChar('N'); }
		else if(isTouchingRect(&touchPos, &keyM)){ if (lowercase) writeChar('m'); else writeChar('M'); }
		
		else if(isTouchingRect(&touchPos, &keyComma)){ writeChar(','); }
		else if(isTouchingRect(&touchPos, &keyDot)){ writeChar('.'); }
		else if(isTouchingRect(&touchPos, &keyQMark)){ writeChar('?'); }
		else if(isTouchingRect(&touchPos, &keyEMark)){ writeChar('!'); }
		else if(isTouchingRect(&touchPos, &keyAt)){ writeChar('@'); }

		else if(isTouchingRect(&touchPos, &keySpace)){ writeChar(' '); }
		
	}

	if(isKeyPressed(CONTROLS_CANCEL) || (isKeyPressed(KEY_TOUCH) && isTouchingRect(&touchPos, &keyBackspace)))
	{
		if(charIndex > 0)
		{
			textCursorPos.x -= getCharWidth(FONTFAMILY_VERDANA14, text[charIndex-1]);
			charIndex--;
			text[charIndex] = ' ';
		}
	}

	if(isKeyPressed(CONTROLS_KBENTER) || (isKeyPressed(KEY_TOUCH) && isTouchingRect(&touchPos, &keyOK)))
	{
		if(charIndex > 0)
			return 1;
	}

	return 0;
}

void getKeyboardInputText(char* cbuff)
{
	u8 i;
	for(i =0; i < charIndex; i++)
	{
		cbuff[i] = text[i];
	}
	cbuff[charIndex] = '\0';
}