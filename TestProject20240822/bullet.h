#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"
//�}�N����`
#define MAX_BULLET (128)//�e�̍ő吔
#define BULLET_WIDTH (100)					//�e�̕�
#define BULLET_HIGHT (100)					//�e�̍���
#define BEEM_DAMAGE (5)					//�e�̍U����
//�e�̎��
typedef enum
{
	BULLETTYPE_PLAYER_NONE = 0,		//�v���C���[�̒e�Ȃ�
	BULLETTYPE_PLAYER_NORMAL,		//�v���C���[�̒ʏ�e
	BULLETTYPE_PLAYER_BEAM,			//�v���C���[�̃r�[���e
	BULLETTYPE_PLAYER_EXPLOSION,	//�v���C���[�̔����e
	BULLETTYPE_PLAYER_HOMING,		//�v���C���[�̒ǔ��e
	BULLETTYPE_ENEMY,				//�G�̒e
	BULLETTYPE_MAX
}BULLETTYPE;
//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fLength,float fAngle, D3DXVECTOR3 rot, BULLETTYPE type);
#endif