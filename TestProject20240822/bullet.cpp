#include "bullet.h"
#include "explosion.h"
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
	D3DXVECTOR3 rotBullet;	//向き
	float fLengthBullet;	//対角線の名が際
	float fAngleBullet;		//対角線の角度
	BULLETTYPE type;		//種類
}Bullet;
//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];//弾の情報
//------------------
//弾の初期化処理
//------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;
	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BeemBlue1.png",
		&g_pTextureBullet);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;

		g_aBullet[nCntBullet].rotBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].fLengthBullet = 0;
		g_aBullet[nCntBullet].fAngleBullet = 0;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, //必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - 100.0f;
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - 25.0f;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + 100.0f;
		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - 100.0f;
		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + 100.0f;
		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + 25.0f;
		pVtx[3].pos.z = 0.0f;
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}
//-------------
//弾の終了処理
//-------------
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//--------------------
//弾の更新処理
//--------------------
void UpdateBullet(void)
{
	int nCntBullet;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			Enemy* pEnemy;//敵の情報へのポインタ
			Player* pPlayer;//プレイヤーの情報へのポインタ
			int nCntEnemy;
			//敵の取得
			pEnemy = GetEnemy();
			pPlayer = GetPlayer();
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{

				if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER_NORMAL)
				{//敵と弾の当たり判定
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - 100 &&
							g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + 100 &&
							g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + 100 &&
							g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - 100)
						{
							//爆発の設定
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
							HitEnemy(nCntEnemy, BEEM_DAMAGE);
							//pEnemy->bUse = false;				//敵を使用していない状態にする
							g_aBullet[nCntBullet].bUse = false;	//弾を使用していない状態にする
						}
					}
					//エフェクトの設定
					SetEffect(g_aBullet[nCntBullet].pos,
						D3DXCOLOR(0.5f,0.5f,0.5f,0.5f),
						40.0f,
						0.5f, 
						BULLETTYPE_PLAYER_NORMAL);
				}
				else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
				{//プレイヤーと敵の弾の当たり判定
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 100 &&
							g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 100 &&
							g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 100 &&
							g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 100)
						{
							//爆発の設定
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
							g_aBullet[nCntBullet].bUse = false;	//弾を使用していない状態にする
							HitPlayer(10);
						}
					}
				}
			}
			//位置を更新
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
			//頂点座標の更新
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rotBullet.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rotBullet.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[0].pos.z = 0.0f;//0.0f;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rotBullet.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rotBullet.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[1].pos.z = 0.0f;//0.0f;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rotBullet.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rotBullet.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[2].pos.z = 0.0f;//0.0f;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rotBullet.z + (0.0f + g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rotBullet.z + (0.0f + g_aBullet[nCntBullet].fAngleBullet)) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[3].pos.z = 0.0f;
			

			g_aBullet[nCntBullet].nLife -= 2;

			if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT)
			{
				g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
			}

			if (g_aBullet[nCntBullet].nLife == 0)//寿命が尽きた
			{
				//爆発の設定
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
				g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
			}
		}
		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}
//--------------------
//弾の描画処理
//--------------------
void DrawBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//弾の描画
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
				//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4*nCntBullet,							//描画する最初の頂点インデックス
				2);
		}
	}
}
//----------------
//たまの設定処理
//----------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLength, float fAngle, D3DXVECTOR3 rot, BULLETTYPE type)
{
	int nCntBullet;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBullet].rotBullet = rot;
			g_aBullet[nCntBullet].fLengthBullet= fLength;
			g_aBullet[nCntBullet].fAngleBullet = fAngle;
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].bUse = true;//使用している状態にする
			//頂点座用の設定
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pos.x + sinf(rot.z + (0.0f - fAngle)) * fLength ;
			pVtx[2].pos.y = pos.y + cosf(rot.z + (0.0f - fAngle)) * fLength ;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pos.x + sinf(rot.z + (0.0f + fAngle)) * fLength ;
			pVtx[3].pos.y = pos.y + cosf(rot.z + (0.0f + fAngle)) * fLength ;
			pVtx[3].pos.z = 0.0f;
			break;
		}
		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}