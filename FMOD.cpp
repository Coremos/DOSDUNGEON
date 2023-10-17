#include <fmod.h>
#pragma comment(lib,"fmodex_vc.lib")
FMOD_SYSTEM *g_System;
FMOD_SOUND *g_Sound;
FMOD_CHANNEL *g_Channel;

void Init()
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 1, FMOD_INIT_NORMAL, NULL);

	FMOD_System_CreateSound(g_System, "coin.wav", FMOD_DEFAULT, 0, &g_Sound);
	//FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound,0,&g_Channel);
}
void Update(int num)
{
	if (num == 13)
		FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound, 0, &g_Channel);
}
void Release()
{
	FMOD_Sound_Release(g_Sound);
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}