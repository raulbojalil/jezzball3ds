#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "i18n.h"
#include "bmp.h"
#include "rect.h"
#include "draw.h"
#include "jezzball.h"
#include "controls.h"
#include <sf2d.h>

int main(int argc, char **argv)
{
	drawInit();

	initCfgu();
	u8 language;
	CFGU_GetSystemLanguage(&language);
	setLanguage(language);

	jezzballInit();

	while (aptMainLoop())
	{
		updateControls();
		
		if (isKeyDown(CONTROLS_EXIT)) break; // break in order to return to hbmenu

		jezzballUpdate();
		jezzballDraw();

		draw();
	}

	exitCfgu();
	drawFinish();
	return 0;
}
