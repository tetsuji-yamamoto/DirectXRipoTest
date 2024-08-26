#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
typedef enum
{
	GAMESTATE_NONE = 0,	//�������Ă��Ȃ����
	GAMESTATE_NORMAL,	//�ʏ���
	GAMESTATE_END,		//�I�����
	GAMESTATE_MAX
}GAMESTATE;
//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
#endif