#ifndef _PLAYER_H_ //マクロ定義されえてなかったら
#define _PLAYER_H_ //二重インクルード防止マクロ定義
//マクロ定義
#define MAX_JUNP (5.0) //ジャンプ力
#define MAX_GRAVITY (0.1) //重力
//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//出現状態
	PLAYERSTATE_NONE,		//通常状態
	PLAYERSTATE_DAMAGE,		//ダメージ状態
	PLAYERSTATE_DEATH,		//死亡状態
	PLAYERSTATE_JUNP,		//ジャンプ状態
	PLAYERSTATE_MAX
}PLAYERSTATE;
//プレイヤー構造体                                 のぞき見は良くないなあ、
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 OldPos;	//前の位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//向き
	PLAYERSTATE state;	//プレイヤーの状態
	float fLength;		//対角線の長さ
	float fAngle;		//対角線の角度
	float junp;			//ジャンプ
	float gravity;		//重力
	int nCounterAnim;	//アニメカウンター
	int nPatternAnim;	//アニメパターン
	int nCounterState;	//状態管理カウンター
	int nLife;			//体力
	bool bDisp;			//表示するかしないか
	bool PlayerJunp;	//ジャンプ状態
}Player;
//プロトタイプ宣言
Player* GetPlayer(void);
void Uninit(void);
void Updata(void);
void Draw(void);
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);
bool PlayerJunp(PLAYERSTATE playerstate);
#endif 
