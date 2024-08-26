#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"
//マクロ定義
#define MAX_BULLET (128)//弾の最大数
#define BULLET_WIDTH (100)					//弾の幅
#define BULLET_HIGHT (100)					//弾の高さ
#define BEEM_DAMAGE (5)					//弾の攻撃力
//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER_NONE = 0,		//プレイヤーの弾なし
	BULLETTYPE_PLAYER_NORMAL,		//プレイヤーの通常弾
	BULLETTYPE_PLAYER_BEAM,			//プレイヤーのビーム弾
	BULLETTYPE_PLAYER_EXPLOSION,	//プレイヤーの爆発弾
	BULLETTYPE_PLAYER_HOMING,		//プレイヤーの追尾弾
	BULLETTYPE_ENEMY,				//敵の弾
	BULLETTYPE_MAX
}BULLETTYPE;
//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fLength,float fAngle, D3DXVECTOR3 rot, BULLETTYPE type);
#endif