#ifndef _HIGHSCORES_H_
#define _HIGHSCORES_H_

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HIGHSCORES 10

typedef struct
{
	u32 score;
	u8 level;
	char player[12+1];

} Highscore;

s16 isHighscore(s32 highscore); 
void saveHighscore(Highscore highscore); 
void loadHighscores(Highscore* hsBuff, u8* count); 

#endif