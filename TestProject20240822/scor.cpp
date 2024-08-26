#include "scor.h"
//マクロ
#define MAX_NUM (8)			//枚数
#define MAX_NUM_HEIGHT (50) //たかさ
#define MAX_NUM_WIDTH (50)	//幅
//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;		//Direct3Dテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;							//スコアの位置
int g_nScore;									//スコアの値
//------------------------
//スコアの初期化処理
//------------------------
void InitScore(void)
{
	int nCnt;
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();
	
	g_posScore = D3DXVECTOR3(100.0f,50.0f,0.0f);	//位置を初期化する
	g_nScore = 0;									//値を初期化する
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number000.png",
		&g_pTextureScore);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM, //必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{	
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f + (MAX_NUM_WIDTH * nCnt), 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(MAX_NUM_WIDTH + (MAX_NUM_WIDTH * nCnt), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f + (MAX_NUM_WIDTH * nCnt), MAX_NUM_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(MAX_NUM_WIDTH + (MAX_NUM_WIDTH * nCnt), MAX_NUM_HEIGHT, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}
//------------------------
//スコアの終了処理
//------------------------
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
//------------------------
//スコアの更新処理
//------------------------
void UpdateScore(void)
{

}
//------------------------
//スコアの描画処理
//------------------------
void DrawScore(void)
{
	int nCnt;
	//必要桁数分の描画
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);
		//背景の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			4 * nCnt,								//描画する最初の頂点インデックス
			2);										//描画するプリミティブ（背景）数
	}
}
//------------------------
//スコアの設定処理
//------------------------
void SetScore(int nScore)
{
	int aPosTexU[8];//格桁の数字を格納
	g_nScore = nScore;
}
//------------------------
//スコアの加算処理
//------------------------
void AddScore(int nValu)
{
	VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	int aPosTexU[8];//格桁の数字を格納
	int nData1 = 10000000, nData2 = 1000000;
	int nCnt;
	g_nScore += nValu;
	//桁ごとに分割する
	for (nCnt = 0; nCnt < 8; nCnt++)
	{
		if (aPosTexU[nCnt] == aPosTexU[0])
		{
			aPosTexU[0] = g_nScore / nData1;
		}
		else
		{
			aPosTexU[nCnt] = (g_nScore % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}