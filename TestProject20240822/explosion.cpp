#include "explosion.h"
#include "main.h"

//マクロ定義
#define MAX_EXPLOSION (128)//爆発の最大数
//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	int nCounterAnim;	//アニメーションカウンター
	int nPatternAnim;	//アニメーションパターンナンバー
	bool bUse;			//使用しているかどうか
}Explosion;
//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];				//爆発の情報
//------------------
//爆発の初期化処理
//------------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;
	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion000.png",
		&g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0, 0.0, 0.0,0.0);
		g_aExplosion[nCntExplosion].bUse = false;
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION, //必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	//爆発の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x - 50.0f;
		pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - 25.0f;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + 50.0f;
		pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - 50.0f;
		pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + 50.0f;
		pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + 25.0f;
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
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
//-------------
//爆発の終了処理
//-------------
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//-------------------
//爆発の更新処理
//--------------------
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている
			//カウンタを進めてパターンナンバーを更新する
			g_aExplosion[nCntExplosion].nCounterAnim++;

			if (g_aExplosion[nCntExplosion].nPatternAnim >= 8)//総パターン数をこえた
			{
				g_aExplosion[nCntExplosion].nPatternAnim = 0;
				g_aExplosion[nCntExplosion].bUse = false;//使用していない状態にする
			}

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 5) == 0)
			{//テクスチャ座標の更新
				g_aExplosion[nCntExplosion].nCounterAnim = 0;
				//パターンを更新
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 10;//パターンナンバーをゼロにする

				pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.125f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.125f) + 0.125, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.125f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.125f) + 0.125, 1.0f);
			}
		}
		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
//--------------------
//爆発の描画処理
//--------------------
void DrawExplosion(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//爆発の描画
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4 * nCntExplosion,						//描画する最初の頂点インデックス
				2);
		}
	}
}
//----------------
//爆発の設定処理
//----------------
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発が使用されていない
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].bUse = true;								//使用している状態にする
			g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//頂点カラーの設定
			//頂点座用の設定
			pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x - 50.0f;
			pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - 50.0f;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + 50.0f;
			pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y - 50.0f;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - 50.0f;
			pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + 50.0f;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + 50.0f;
			pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + 50.0f;
			pVtx[3].pos.z = 0.0f;
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
			break;
		}
		pVtx += 4;//頂点データのポインタを四つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}