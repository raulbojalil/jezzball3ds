#include <gccore.h>
#include "controls.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "i18n.h"
#include "bmp.h"
#include "rect.h"
#include "draw.h"
#include "jezzball.h"
#include <ogc/lwp_watchdog.h>

double ellapsedTime = 0;

int main(int argc, char **argv)
{
	WPAD_Init();
	drawInit();
	jezzballInit();

	while(1)
	{
		ellapsedTime = ticks_to_millisecs(gettime());
		updateControls();

		if (isKeyDown(CONTROLS_EXIT)){
			drawFinish();
			exit(0); 
		}

		jezzballUpdate();
		jezzballDraw();

		draw();

		ellapsedTime = ticks_to_millisecs(gettime()) - ellapsedTime; 

		if(ellapsedTime < (1000 / 60))
			usleep((1000 / 60) - ellapsedTime);

	}

	return 0;
}
