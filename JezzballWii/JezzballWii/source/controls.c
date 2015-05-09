#include "controls.h"

u32 kPressed;
u32 kDown;
u32 kReleased;

void updateControls()
{
	WPAD_ScanPads();
	kPressed = WPAD_ButtonsDown(0);
	kDown = WPAD_ButtonsHeld(0);
	kReleased = WPAD_ButtonsUp(0);
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

