#include "Bullet.h"
#include "background.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "scor.h"
#include "effect.h"
#include "loading.h"
//グローバル変数宣言
GAMESTATE g_gameState = GAMESTATE_NONE;	//ゲーム状態
int g_nCounterGameState = 0;			//状態管理カウンター
//----------------
//ゲーム画面の初期化処理
//----------------
void InitGame(void)
{
	//背景の初期化処理
	InitBG();
	//プレイヤーの初期化処理
	InitPlayer();
	//敵の初期化処理
	InitEnemy();
	//敵出現
	SetEnemy(D3DXVECTOR3(700.0f, 200.0f, 0.0f), ENEMYTYPE_ZERO);
	SetEnemy(D3DXVECTOR3(700.0f, 400.0f, 0.0f), ENEMYTYPE_ONE);
	SetEnemy(D3DXVECTOR3(700.0f, 600.0f, 0.0f), ENEMYTYPE_ONE);
	//ローディングの初期化
	//InitLoading();
	//弾の初期化処理
	InitBullet();
	//エフェクトの初期化
	InitEffect();
	//爆発の初期化処理
	InitExplosion();
	//スコアの初期化
	InitScore();
	SetScore(0);

	g_gameState = GAMESTATE_NORMAL;//通常状態に設定
	g_nCounterGameState = 0;
}
//----------------
//ゲーム画面の終了処理
//----------------
void UninitGame(void)
{
	//背景の終了処理
	UninitBG();
	//プレイヤーの終了処理
	UninitPlayer();
	//敵の終了処理
	UninitEnemy();
	//ローディングの終了処理
	//UninitLoading();
	//弾の終了処理
	UninitBullet();
	//エフェクトの終了処理
	UninitEffect();
	//爆発の終了処理
	UninitExplosion();
	//スコアの終了処理
	UninitScore();
}
//----------------
//ゲーム画面の更新処理
//----------------
void UpdateGame(void)
{
	Enemy* pEnemy=GetEnemy();
	Player* pPlayer=GetPlayer();
	int nNumEnemy;
	//UpdateTitle();
	//背景更新処理
	UpdateBG();
	//プレイヤーの更新処理8
	UpdatePlayer();
	//敵の更新処理
	UpdateEnemy();
	//ローディングの更新処理
	//UpdateLoading();
	//弾の更新処理
	UpdateBullet();
	//エフェクトの更新処理
	UpdateEffect();
	//爆発の更新処理
	UpdateExplosion();
	//スコアの更新処理
	UpdateScore();

	nNumEnemy = GetNumEnemy();

	if (nNumEnemy <= 0)
	{
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL://通常状態

		break;

	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			SetFade(MODE_RESULT);
			g_nCounterGameState = 0;
			g_gameState = GAMESTATE_NONE;//何もしていない状態に設定
		}
		break;
	}
}
//----------------
//ゲーム画面の描画処理
//----------------
void DrawGame(void)
{
	//背景の描画
	DrawBG();
	//プレイヤーの描画
	DrawPlayer();
	//エネミーの描画
	DrawEnemy();
	//ローディングの描画
	//DrawLoading();
	//エフェクトの描画
	DrawEffect();
	//弾の描画
	DrawBullet();
	//爆発の描画
	DrawExplosion();
	//スコアの描画
	DrawScore();
}
//----------------
//ゲーム状態の設定
//----------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;

	g_nCounterGameState = 0;
}
//----------------
//ゲーム状態の取得
//----------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}