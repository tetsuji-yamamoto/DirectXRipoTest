#include "effect.h"
#include "explosion.h"
#include "main.h"
#include "enemy.h"
#include "player.h"
//エフェクトの最大数
#define MAX_EFFECT (4090) //エフェクトの最大数
//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	BULLETTYPE type;		//種類
	D3DXCOLOR col;			//色
	float fRadius;			//頂点座標の設定
	float fAlpha;			//アルファ値の設定
	float fData;			//アルファ値の設定
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
}Effect;
//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];//エフェクトの情報
//------------------
//エフェクトの初期化処理
//------------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect001.png",
		&g_pTextureEffect);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 100;
		g_aEffect[nCntEffect].fRadius = 0;
		g_aEffect[nCntEffect].fAlpha = 0;
		g_aEffect[nCntEffect].bUse = false;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, //必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//エフェクトの情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - 100.0f;
		pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - 25.0f;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + 100.0f;
		pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - 100.0f;
		pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + 100.0f;
		pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + 25.0f;
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
	g_pVtxBuffEffect->Unlock();
}
//-------------
//エフェクトの終了処理
//-------------
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//-------------------
//エフェクトの更新処理
//--------------------
void UpdateEffect(void)
{
	int nCntEffect;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
			if (g_aEffect[nCntEffect].type == BULLETTYPE_PLAYER_NORMAL)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				//頂点座標の設定
				pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius / 2;
				pVtx[0].pos.z = 0.0f;
				pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius / 2;
				pVtx[1].pos.z = 0.0f;
				pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius / 2;
				pVtx[2].pos.z = 0.0f;
				pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius / 2;
				pVtx[3].pos.z = 0.0f;
			}
			else if (g_aEffect[nCntEffect].type == BULLETTYPE_ENEMY)
			{

			}

			g_aEffect[nCntEffect].fRadius = g_aEffect[nCntEffect].fRadius - 1.0f;
			g_aEffect[nCntEffect].fAlpha = g_aEffect[nCntEffect].fAlpha - g_aEffect[nCntEffect].fData;
		}
		
		if (g_aEffect[nCntEffect].fRadius <= 0)//寿命が尽きた
		{
			g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
		}
		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
//--------------------
//エフェクトの描画処理
//--------------------
void DrawEffect(void)
{
	int nCntEffect;

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//エフェクトの描画
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
				//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4 * nCntEffect,							//描画する最初の頂点インデックス
				2);
		}
	}
	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//----------------
//エフェクトの設定処理
//----------------
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col,float fRadius,float fAlpha, BULLETTYPE type)
{
	int nCntEffect;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//エフェクトが使用されていない
			if (type == BULLETTYPE_PLAYER_NORMAL)
			{
				g_aEffect[nCntEffect].pos = pos;
				g_aEffect[nCntEffect].type = type;
				g_aEffect[nCntEffect].fRadius = fRadius;
				g_aEffect[nCntEffect].fAlpha = fAlpha;
				g_aEffect[nCntEffect].col = col;
				g_aEffect[nCntEffect].fData = 0.05;
				g_aEffect[nCntEffect].nLife = 100;
				g_aEffect[nCntEffect].bUse = true;//使用している状態にする
				//頂点座用の設定
				pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius / 2;
				pVtx[0].pos.z = 0.0f;
				pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius / 2;
				pVtx[1].pos.z = 0.0f;
				pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius / 2;
				pVtx[2].pos.z = 0.0f;
				pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius / 2;
				pVtx[3].pos.z = 0.0f;
			}
			break;
		}
		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}