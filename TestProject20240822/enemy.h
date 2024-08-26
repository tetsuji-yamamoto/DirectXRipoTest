#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"
//�}�N��
#define MAX_ENEMY (3)//�Ă��̍ő吔
//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//�ʏ���
	ENEMYSTATE_DAMAGE,		//�_���[�W���
	ENEMYSTATE_MAX
}ENEMYSTATE;
enum ENEMYTYPE
{
	ENEMYTYPE_ZERO = 0,
	ENEMYTYPE_ONE,
	ENEMYTYPE_TOWO,
	ENEMYTYPE_THREE,
	ENEMYTYPE_MAX
};
//�G�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	ENEMYSTATE state;	//���
	int nCountState;	//��ԊǗ��J�E���^�[
	int nLife;			//�̗�
	int nType;			//���
	int nPatternAnim;	//
	int nCounterAnim;
	int nBulletCounter;
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Enemy;
//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy,int nDamage);
Enemy* GetEnemy(void);
int GetNumEnemy();
#endif
