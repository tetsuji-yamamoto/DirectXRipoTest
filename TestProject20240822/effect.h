#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"
#include "bullet.h"
//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, float fAlpha, BULLETTYPE type);
#endif