#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"
//マクロ
#define MAX_ENEMY (3)//てきの最大数
//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//通常状態
	ENEMYSTATE_DAMAGE,		//ダメージ状態
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
//敵構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	ENEMYSTATE state;	//状態
	int nCountState;	//状態管理カウンター
	int nLife;			//体力
	int nType;			//種類
	int nPatternAnim;	//
	int nCounterAnim;
	int nBulletCounter;
	bool bUse;			//使用しているかどうか
}Enemy;
//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy,int nDamage);
Enemy* GetEnemy(void);
int GetNumEnemy();
#endif
