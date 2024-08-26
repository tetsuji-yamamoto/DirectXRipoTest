#include "enemy.h"
#include "main.h"
#include "explosion.h"
#include "bullet.h"
#include "scor.h"
//マクロ
#define NUM_ENEMY (3)//敵の種類
#define ENEMY_HEIGHT (100)//敵の高さ
#define ENEMY_WIDTH (100)//敵の幅
//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;			//頂点バッファへのポインタ
Enemy g_aEnemy[NUM_ENEMY];								//敵の情報
int g_nNumEnemy = 0;									//敵の総数
//-------------------
//敵の初期化処理
//-------------------
void InitEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ4枚分の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\kinoko.png",
		&g_apTextureEnemy[ENEMYTYPE_ZERO]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gohan.jpg",
		&g_apTextureEnemy[ENEMYTYPE_ONE]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_ENEMY, //必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].bUse == false;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nLife = 100;
		g_aEnemy[nCntEnemy].nCountState = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nBulletCounter = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		//頂点座標の設定
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(600.0f, 100.0f, 0.0f );
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}
//-------------------
//敵の終了処理
//-------------------
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//テクスチャの破棄
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}
//-------------------
//敵の更新処理
//--------------------
void UpdateEnemy(void)
{
	VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	int nCntEnemy;
	//使用されている敵が弾を発射する処理
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].nBulletCounter++; //カウンターを加算

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				if ((g_aEnemy[nCntEnemy].nBulletCounter % 100) == 0)
				{
					SetBullet(g_aEnemy[nCntEnemy].pos,
						D3DXVECTOR3(sinf(-3.14 + D3DX_PI * 0.5) * 20.0f,
						cosf(-3.14 + D3DX_PI * 0.5) * 20.0f, 0.0f),
						sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HIGHT * BULLET_HIGHT) / 2,
						atan2f(BULLET_HIGHT, BULLET_HIGHT),
						D3DXVECTOR3(0.0f, 0.0f, -3.14f),
						BULLETTYPE_ENEMY);
				}
				//敵アニメーション
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_ZERO)
				{
					g_aEnemy[nCntEnemy].nCounterAnim++; //カウンターを加算
					if ((g_aEnemy[nCntEnemy].nCounterAnim % 5) == 0)
					{
						g_aEnemy[nCntEnemy].nCounterAnim = 0;
						//ぱらーんを更新
						g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % 20;//パターンナンバーをゼロにする

						//テクスチャ座標の設定
						pVtx[0].tex = D3DXVECTOR2(0.2f * g_aEnemy[nCntEnemy].nPatternAnim,			0.25f * (g_aEnemy[nCntEnemy].nPatternAnim / 5));
						pVtx[1].tex = D3DXVECTOR2(0.2f + (0.2f * g_aEnemy[nCntEnemy].nPatternAnim), 0.25f * (g_aEnemy[nCntEnemy].nPatternAnim / 5));
						pVtx[2].tex = D3DXVECTOR2(0.2f * g_aEnemy[nCntEnemy].nPatternAnim,			0.25f + 0.25f * (g_aEnemy[nCntEnemy].nPatternAnim / 5));
						pVtx[3].tex = D3DXVECTOR2(0.2f + (0.2f * g_aEnemy[nCntEnemy].nPatternAnim), 0.25f + 0.25f * (g_aEnemy[nCntEnemy].nPatternAnim / 5));
					}
				}
				break;

			case ENEMYSTATE_DAMAGE:

				g_aEnemy[nCntEnemy].nCountState --;

				if (g_aEnemy[nCntEnemy].nCountState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
					//頂点カラーの設定
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}
//-------------------
//敵の描画処理
//--------------------
void DrawEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_ZERO)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_ZERO]);
			//敵の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4 * nCntEnemy,							//描画する最初の頂点インデックス
				2);										//描画するプリミティブ（敵）数
		}
		else if(g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_ONE)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_ONE]);
			//敵の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4 * nCntEnemy,							//描画する最初の頂点インデックス
				2);										//描画するプリミティブ（敵）数
		}
	}
}
//----------------
//敵の設定処理
//----------------
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (nType == ENEMYTYPE_ZERO)
		{
			if (g_aEnemy[nCntEnemy].bUse == false)
			{//敵が使用されていない
				g_aEnemy[nCntEnemy].pos = pos;
				g_aEnemy[nCntEnemy].nType = nType;
				g_aEnemy[nCntEnemy].nLife = 100;
				g_aEnemy[nCntEnemy].nCountState = 0;
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				g_aEnemy[nCntEnemy].bUse = true;//使用している状態にする
				//頂点座用の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				g_nNumEnemy++;//敵の総数カウントアップ
				break;
			}
		}
		else if (nType == ENEMYTYPE_ONE)
		{
			if (g_aEnemy[nCntEnemy].bUse == false)
			{//敵が使用されていない
				g_aEnemy[nCntEnemy].pos = pos;
				g_aEnemy[nCntEnemy].nType = nType;
				g_aEnemy[nCntEnemy].nLife = 100;
				g_aEnemy[nCntEnemy].nCountState = 0;
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				g_aEnemy[nCntEnemy].bUse = true;//使用している状態にする
				//頂点座用の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				g_nNumEnemy++;//敵の総数カウントアップ
				break;
			}
		}
		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{//敵の体力がなくなった
		//爆発の設定
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		AddScore(200);
		g_aEnemy[nCntEnemy].bUse = false;
		g_nNumEnemy--;//敵の総数カウントダウン
	}
	else
	{
		pVtx += 4 * nCntEnemy;

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCountState = 5;				//ダメージ状態を保つ時間を設定
		//頂点カラーの設定,ポリゴンをダメージ色に設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}
//----------------
//敵の設定処理
//----------------
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}
//----------------
//敵の取得
//----------------
int GetNumEnemy()
{
	return g_nNumEnemy;
}