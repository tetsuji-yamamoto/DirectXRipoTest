//#include "loading.h"
////グローバル変数宣言
//LPDIRECT3DTEXTURE9 g_pTextureLoading = NULL;		//Direct3Dテクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLoading = NULL;	//頂点バッファへのポインタ
//LOADING g_aLoading[MAX_LOADING];					//ローディングの情報
////----------------------------
////ローディング初期化処理
////----------------------------
//void InitLoading(void)
//{
//	int nCntLoadi;
//	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
//	//デバイスの取得
//	pDevice = GetDevice();	
//	//テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice,
//		"data\\TEXTURE\\BulletSet003.png",
//		&g_pTextureLoading);
//	//頂点バッファの生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LOADING, //必要な頂点数
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffLoading,
//		NULL);
//	//頂点情報へのぽいんな
//	VERTEX_2D* pVtx; 
//	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
//	g_pVtxBuffLoading->Lock(0, 0, (void**)&pVtx, 0);
//	//ローディング情報の初期化
//	for (nCntLoadi = 0; nCntLoadi < MAX_LOADING; nCntLoadi++)
//	{
//		//位置を初期化する
//		g_aLoading[nCntLoadi].pos = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
//		//弾タイプの初期化
//		g_aLoading[nCntLoadi].nLoadingType = BULLETTYPE_PLAYER_NONE;
//		//頂点座標の設定
//		pVtx[0].pos.x = g_aLoading[nCntLoadi].pos.x - MAX_LOADING_WIDTH / 2;
//		pVtx[0].pos.y = g_aLoading[nCntLoadi].pos.y - MAX_LOADING_HEIGHT / 2;
//		pVtx[0].pos.z = 0.0f;
//
//		pVtx[1].pos.x = g_aLoading[nCntLoadi].pos.x + MAX_LOADING_WIDTH / 2;;
//		pVtx[1].pos.y = g_aLoading[nCntLoadi].pos.y - MAX_LOADING_HEIGHT / 2;;
//		pVtx[1].pos.z = 0.0f;;
//
//		pVtx[2].pos.x = g_aLoading[nCntLoadi].pos.x - MAX_LOADING_WIDTH / 2;;
//		pVtx[2].pos.y = g_aLoading[nCntLoadi].pos.y + MAX_LOADING_HEIGHT / 2;;
//		pVtx[2].pos.z = 0.0f;;
//
//		pVtx[3].pos.x = g_aLoading[nCntLoadi].pos.x + MAX_LOADING_WIDTH / 2;;
//		pVtx[3].pos.y = g_aLoading[nCntLoadi].pos.y + MAX_LOADING_HEIGHT / 2;;
//		pVtx[3].pos.z = 0.0f;;
//		//rhwの設定
//		pVtx[0].rhw = 1.0f;
//		pVtx[1].rhw = 1.0f;
//		pVtx[2].rhw = 1.0f;
//		pVtx[3].rhw = 1.0f;
//		//頂点カラーの設定
//		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		//テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
//
//		//pVtx += 4;
//	}
//	//頂点バッファをアンロックする
//	g_pVtxBuffLoading->Unlock();
//}
////----------------------------
////ローディング終了処理
////----------------------------
//void UninitLoading(void)
//{
//	//テクスチャの破棄
//	if (g_pTextureLoading != NULL)
//	{
//		g_pTextureLoading->Release();
//		g_pTextureLoading = NULL;
//	}
//	//頂点バッファの破棄
//	if (g_pVtxBuffLoading != NULL)
//	{
//		g_pVtxBuffLoading->Release();
//		g_pVtxBuffLoading = NULL;
//	}
//}
////----------------------------
////ローディング更新処理
////----------------------------
//void UpdateLoading(void)
//{
//	int nCntLoading;
//
//	VERTEX_2D* pVtx;
//	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
//	g_pVtxBuffLoading->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntLoading = 0; nCntLoading < MAX_LOADING; nCntLoading++)
//	{
//		if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_NONE)
//		{//弾なしだったら
//			//テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.5f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.25f);
//			//弾の状態を返す
//			//GetNumLoading(BULLETTYPE_PLAYER_NONE);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_NORMAL)
//		{//通常弾だったら
//			//テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.25f);
//			//弾の状態を返す
//			//GetNumLoading(BULLETTYPE_PLAYER_NORMAL);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_BEAM)
//		{//ビーム弾だったら
//			//テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.25f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.5f);
//			//弾の状態を返す
//			//GetNumLoading(BULLETTYPE_PLAYER_BEAM);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_EXPLOSION)
//		{//爆発弾だったら
//			//テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.25f);
//			//弾の状態を返す
//			//GetNumLoading(BULLETTYPE_PLAYER_EXPLOSION);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_HOMING)
//		{//ホーミング弾だったら
//			//テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.25f);
//			//弾の状態を返す
//			//GetNumLoading(BULLETTYPE_PLAYER_HOMING);
//		}
//		
//		//pVtx += 4;
//	}
//	//頂点バッファをアンロックする
//	g_pVtxBuffLoading->Unlock();
//}
////----------------------------
////ローディング描画処理
////----------------------------
//void DrawLoading(void)
//{
//	int nCntLoading;
//
//	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
//	//デバイスの取得
//	pDevice = GetDevice();
//	//頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, g_pVtxBuffLoading, 0, sizeof(VERTEX_2D));
//	//頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//	//エフェクトの描画
//	for (nCntLoading = 0; nCntLoading < MAX_LOADING; nCntLoading++)
//	{
//		//テクスチャの設定
//		pDevice->SetTexture(0, g_pTextureLoading);
//		//ポリゴン描画
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
//			4 * nCntLoading,							//描画する最初の頂点インデックス
//			2);
//	}
//}
////----------------------------
////ローディング設定処理
////----------------------------
//void SetLoading(BULLETTYPE BulletType)
//{
//	int nCntLoading;
//
//	for (nCntLoading = 0; nCntLoading < MAX_LOADING; nCntLoading++)
//	{
//		g_aLoading[nCntLoading].nLoadingType = BulletType;
//	}
//}
////----------------------------
////ローディング情報の取得
////----------------------------
////int GetLoadingType(int nLoading)
////{
////	return g_aLoading[nLoading].nLoadingType;
////}