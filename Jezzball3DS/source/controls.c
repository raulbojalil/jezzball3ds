#include "controls.h"

u32 kPressed;
u32 kDown;
u32 kReleased;

void updateControls()
{
	hidScanInput();
	kPressed = hidKeysDown();
	kDown = hidKeysHeld();
	kReleased = hidKeysUp();
}

u32 isKeyPressed(u32 key){ 
	return kPressed & key; 
}
u32 isKeyDown(u32 key){ 
	return kDown & key;  
}
u32 isKeyReleased(u32 key){
	return kReleased & key;
}

