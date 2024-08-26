#include "main.h"
#include "bullet.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "loading.h"
#include "fade.h"
//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;	//Direct3Dテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer;	//頂点バッファへのポインタ
Player g_Player;//プレイヤーの情報
//STICK_ANGLE g_StickAngle;
//-------------------
//プレイヤーの初期化処理
//--------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	g_Player.nPatternAnim = 0;						//カウンターを初期化する
	g_Player.nPatternAnim = 0;						//パターンナンバーを初期化する
	g_Player.bDisp = true;
	g_Player.state = PLAYERSTATE_NONE;				//プレイヤー状態を初期化
	g_Player.nLife = 50;							//体力の初期化
	g_Player.pos = D3DXVECTOR3(400, 400, 0);		//位置を初期化する
    g_Player.move = D3DXVECTOR3(0 , 0, 0);			//移動量を初期化する
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向きを初期化する
	g_Player.junp = 0.0f;							//ジャンプ力の初期化
	g_Player.gravity = MAX_GRAVITY;					//重力の初期化
	g_Player.PlayerJunp = true;						//ジャンプ状態の初期化
	//対角線ノ長さを産出する
	g_Player.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HIGHT * PLAYER_HIGHT)/2;

	//対角線の角度を算出する
	g_Player.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HIGHT);

		//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\runningman100.png",
		&g_pTexturePlayer);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4, //必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのぽいんな

	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;
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
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);


	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}
//--------------------
//プレイヤーの終了処理
//--------------------
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//-------------------
//プレイヤーの更新処理
//--------------------
void UpdatePlayer(void)
{	
	//int nCntLoadi;
	static float fData = 0.1;
	//スティック状態の取得
	XINPUT_STATE* pStick;
	pStick = GetJoySticAngle();
	//弾状態の取得
	/*LOADING* pBulletType[MAX_LOADING];
	for (nCntLoadi = 0; nCntLoadi < MAX_LOADING; nCntLoadi++)
	{
		pBulletType[nCntLoadi] = GetLoadingType;
	}*/
		VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	//移動
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)//A,十字キー左
	{//左移動
		fData = 0.1;
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)//W,十字キー上
		{
			g_Player.move.x += sinf(-D3DX_PI * 0.75) * 0.5;
			g_Player.move.y += cosf(-D3DX_PI * 0.75) * 0.5;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)//S,十字キー下
		{
			g_Player.move.x += sinf(-D3DX_PI * 0.25) * 0.5;
			g_Player.move.y += cosf(-D3DX_PI * 0.25) * 0.5;
		}
		else
		{
			g_Player.move.x -= 0.5;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)//D,十字キー右
	{//右移動
		fData = -0.1;
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)//W,十字キー上
		{
			g_Player.move.x += sinf(D3DX_PI * 0.75) * 0.5;
			g_Player.move.y += cosf(D3DX_PI * 0.75) * 0.5;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)//S,十字キー下
		{
			g_Player.move.x += sinf(D3DX_PI * 0.25) * 0.5;
			g_Player.move.y += cosf(D3DX_PI * 0.25) * 0.5;
		}
		else
		{
			g_Player.move.x += 0.5;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true  || GetJoypadPress(JOYKEY_UP) == true)//W,十字キー上
	{//上移動
		g_Player.move.y -= 0.5;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)//S,十字キー下
	{//下移動
		g_Player.move.y += 0.5;
	}
	else if (GetKeyboardPress(DIK_I) == true)//I
	{//上視点
		if (GetKeyboardPress(DIK_J) == true)//J
		{
			g_Player.rot.z = 2.355f;
		}
		else if (GetKeyboardPress(DIK_L) == true)//L
		{
			g_Player.rot.z = 0.785f;
		}
		else
		{
			g_Player.rot.z = 1.57f;
		}
	}
	else if (GetKeyboardPress(DIK_K) == true)//K
	{//下視点
		if (GetKeyboardPress(DIK_J) == true)//J
		{
			g_Player.rot.z = -2.355f;
		}
		else if (GetKeyboardPress(DIK_L) == true)//L
		{
			g_Player.rot.z = -0.785f;
		}
		else
		{
			g_Player.rot.z = -1.57f;
		}
	}
	else if (GetKeyboardPress(DIK_J) == true)//J
	{//左視点
		g_Player.rot.z = 3.14f;
	}
	else if (GetKeyboardPress(DIK_L) == true)//L
	{//右視点
		g_Player.rot.z = 0.0f;
	}


	else if (GetJoyStick() == true)
	{
		if (pStick->Gamepad.sThumbLY < -10922)//下に倒したら
		{//下移動
			if (pStick->Gamepad.sThumbLX > 10922)//右下に倒したら
			{
				g_Player.move.x += sinf(D3DX_PI * 0.25) * 0.5;
				g_Player.move.y += cosf(D3DX_PI * 0.25) * 0.5;
			}
			else if (pStick->Gamepad.sThumbLX < -10922)//左下に倒したら
			{
				g_Player.move.x += sinf(-D3DX_PI * 0.25) * 0.5;
				g_Player.move.y += cosf(-D3DX_PI * 0.25) * 0.5;
			}
			else
			{
				g_Player.move.y += 0.5;
			}
		}
		else if (pStick->Gamepad.sThumbLX > 10922)//右に倒したら
		{//右移動
			g_Player.move.x += 1.5f;
			g_Player.rot.z = 0.0f;
		}
		else if (pStick->Gamepad.sThumbLX < -10922)//左に倒したら
		{//左移動
			g_Player.move.x -= 1.5f;
			g_Player.rot.z = 3.14f;
		}
	}
	//ジャンプ
	if (KeyboardTrigger(DIK_C) == true || GetJoypadTrigger(JOYKEY_A) == true && g_Player.PlayerJunp == false)
	{
		g_Player.PlayerJunp = true;
		g_Player.junp = MAX_JUNP;
	}
	//射撃
	if (KeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_X))//スペース,xボタン
	{
		/*if (GetLoadingType() = )
		{

		}*/
		SetBullet(g_Player.pos,
				D3DXVECTOR3(sinf(g_Player.rot.z + D3DX_PI * 0.5)*20.0f,
				cosf(g_Player.rot.z + D3DX_PI * 0.5) * 20.0f,0.0f),
				sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HIGHT * BULLET_HIGHT) / 2,
				atan2f(BULLET_WIDTH, BULLET_HIGHT),
				g_Player.rot,
			BULLETTYPE_PLAYER_NORMAL);
	}
	//リロード
	if (GetJoyTrigger(JOYKEY_L2) == true)
	{//通常弾
		//SetLoading(BULLETTYPE_PLAYER_NORMAL);
	}


	//重力
	g_Player.move.y += g_Player.gravity - g_Player.junp;
	g_Player.junp -= g_Player.gravity * 2;//ジャンプ力を減衰
	//移動量を更新(減衰させる)
	g_Player.move.x += (0 - g_Player.move.x) * 0.1;
	g_Player.move.y += (1 - g_Player.move.y) * 0.1;
	//位置を更新
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	if (g_Player.pos.y >= SCREEN_HEIGHT - (PLAYER_HIGHT / 2))//画面下
	{
		g_Player.pos.y = SCREEN_HEIGHT - (PLAYER_HIGHT / 2);
		g_Player.move.y -= g_Player.move.y;
		g_Player.junp = 0.0f;
		g_Player.PlayerJunp = false;

		if (g_Player.pos.x <= (PLAYER_WIDTH / 2))//画面左
		{
			g_Player.pos.x = (PLAYER_WIDTH / 2);
		}
		else if (g_Player.pos.x >= SCREEN_WIDTH - (PLAYER_WIDTH / 2))//画面右
		{
			g_Player.pos.x = SCREEN_WIDTH - (PLAYER_WIDTH / 2);
		}
	}
	else if (g_Player.pos.y <= (PLAYER_HIGHT / 2))//画面上
	{
		g_Player.pos.y = g_Player.OldPos.y;
	}
	else if (g_Player.pos.x <= (PLAYER_WIDTH / 2))//画面左
	{
		g_Player.pos.x = g_Player.OldPos.x;
	}
	else if (g_Player.pos.x >= SCREEN_WIDTH - (PLAYER_WIDTH / 2))//画面右
	{
		g_Player.pos.x = g_Player.OldPos.x;
	}
	
	switch (g_Player.state)
	{
	case PLAYERSTATE_NONE:
		break;

	case PLAYERSTATE_DAMAGE:
		g_Player.nCounterState--;

		if (g_Player.nCounterState <= 0)
		{
			g_Player.state = PLAYERSTATE_NONE;
			//頂点カラーの設定,通常状態に戻す
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case PLAYERSTATE_DEATH:
		g_Player.nCounterState--;

		if (g_Player.nCounterState <= 0)
		{
			//モード設定(リザルト画面)
			SetFade(MODE_RESULT);
			return;
		}
		break;
	}

	g_Player.OldPos = g_Player.pos;
	//頂点座標の更新
	pVtx[0].pos.x = g_Player.pos.x - PLAYER_WIDTH / 2;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_HIGHT / 2;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + PLAYER_WIDTH / 2;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_HIGHT / 2;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x - PLAYER_WIDTH / 2;
	pVtx[2].pos.y = g_Player.pos.y + PLAYER_HIGHT / 2;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + PLAYER_WIDTH / 2;
	pVtx[3].pos.y = g_Player.pos.y + PLAYER_HIGHT / 2;
	pVtx[3].pos.z = 0.0f;

	g_Player.nCounterAnim++; //カウンターを加算
	if ((g_Player.nCounterAnim % 5) == 0)
	{
		g_Player.nCounterAnim = 0;
		//ぱらーんを更新
		g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 10;//パターンナンバーをゼロにする

		pVtx[0].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 - fData) , (g_Player.nPatternAnim/5) * 0.5);
		pVtx[1].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 + fData) , (g_Player.nPatternAnim/5) * 0.5);
		pVtx[2].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 - fData) , (g_Player.nPatternAnim/5) * 0.5 + 0.5);
		pVtx[3].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 + fData) , (g_Player.nPatternAnim/5) * 0.5 + 0.5);

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();
	}

	
}
//--------------------
//プレイヤーの描画処理
//--------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();
	if (g_Player.bDisp == true)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);
		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,										//描画する最初の頂点インデックス
			2);										//描画するプリミティブ（プレイヤー）数
	}
}
//----------------------
//プレイヤーのヒット処理
//----------------------
void HitPlayer(int nDamage)
{
	g_Player.nLife -= nDamage;

	VERTEX_2D* pVtx; //頂点情報へのぽいんな
	//頂点バッファをロックし、ちょうてん情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	//プレイヤーの体力がなくなった
	if (g_Player.nLife <= 0)
	{
		//爆発の設定
		g_Player.bDisp = false;
		g_Player.state = PLAYERSTATE_DEATH;
		g_Player.nCounterState = 60;
	}
	else
	{
		g_Player.state = PLAYERSTATE_DAMAGE;
		g_Player.nCounterState = 5;
		//頂点カラーの設定,ポリゴンをダメージ色に設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}
//--------------------
//プレイヤーの状態
//--------------------
bool PlayerJunp(PLAYERSTATE playerstate)
{
	bool junp = false;
	if (playerstate == PLAYERSTATE_JUNP)
	{
		return true;
	}
}
//--------------------
//プレイヤーの取得
//--------------------
Player* GetPlayer(void)
{
	return &g_Player;
}
