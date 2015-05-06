#include "highscores.h"

s16 isHighscore(s32 highscore)  
{
	Highscore highscores[50];
	u8 highscoreCount = 0;
    u8 count = 0;
	loadHighscores(highscores, &highscoreCount);
	u8 i=0;

	for(i=0; i < highscoreCount; i++)
	{
		if(highscore > highscores[i].score)
			return i;
	}

	return -1;
}

void saveHighscore(Highscore highscore)
{
	Highscore highscores[50];
	u8 highscoreCount = 0;
	loadHighscores(highscores, &highscoreCount);

	u8 i;
	s16 j;

	for(i = 0; i < highscoreCount; i++)
	{
		if(highscore.score > highscores[i].score)
		{
			for(j = highscoreCount-2; j >= i; j--)
			{
				highscores[j+1] = highscores[j];
			}

			highscores[i] = highscore;
			break;
		}
	}

	FILE *file = fopen("jezzball3ds.sav","w");
	if (file == NULL) return;
	fwrite ((void*)highscores, sizeof(Highscore), MAX_HIGHSCORES, file);
	fclose(file);
}

void loadHighscores(Highscore* hsBuff, u8* count){

	*count = MAX_HIGHSCORES;

	u8 i= 0;
	for(i =0; i < MAX_HIGHSCORES; i++)
	{
		Highscore score = { 0,1, "------" };
		hsBuff[i] = score;
	}
	
	FILE *file = fopen("jezzball3ds.sav","rb");
	if (file == NULL) return;

	fseek(file,0,SEEK_END);
	u32 size = ftell(file);
	fseek(file,0,SEEK_SET);

	Highscore* fileBuff;
	fileBuff=(Highscore*)malloc(size);
	fread(fileBuff,1,size,file);
	fclose(file);

	if(!fileBuff) return;
	
	u8 countInFile = size / sizeof(Highscore);
	
	for(i=0; i < MAX_HIGHSCORES; i++)
	{
		if(i < countInFile)
			hsBuff[i] = fileBuff[i];
	}

	free(fileBuff);

}