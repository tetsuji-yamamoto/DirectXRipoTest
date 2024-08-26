#include "main.h"
#include "background.h"
//マクロ定義
#define NUM_BG (3)//背景の枚数
//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = {};	//Direct3Dテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;	//頂点バッファへのポインタ
float g_aPosTexU[NUM_BG];						//テクスチャ座標の開始位置(U値)
//-------------------
//背景の初期化処理
//-------------------
void InitBG(void)
{
	int nCntBG;
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg100.png",
		&g_pTextureBG[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_pTextureBG[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg102.png",
		&g_pTextureBG[2]);
	//テクスチャ座標の開始位置(U値)の更新
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aPosTexU[nCntBG] = 0.0f;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG, //必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
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
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}
//-------------------
//背景の終了処理
//--------------------
void UninitBG(void)
{
	int nCntBG;

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの破棄
		if (g_pTextureBG[nCntBG] != NULL)
		{
			g_pTextureBG[nCntBG]->Release();
			g_pTextureBG[nCntBG] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}
//-------------------
//背景の更新処理
//--------------------
void UpdateBG(void)
{
	int nCntBG;

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャ座標の開始位置(U値)の更新
		g_aPosTexU[nCntBG] += 0.0005f * (nCntBG + 1);
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}
//-------------------
//背景の描画処理
//--------------------
void DrawBG(void)
{
	int nCntBG;

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);
		//背景の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			4 * nCntBG,								//描画する最初の頂点インデックス
			2);										//描画するプリミティブ（背景）数
	}
}