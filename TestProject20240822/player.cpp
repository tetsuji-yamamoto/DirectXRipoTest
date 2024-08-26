#include "main.h"
#include "bullet.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "loading.h"
#include "fade.h"
//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;	//Direct3D�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer;	//���_�o�b�t�@�ւ̃|�C���^
Player g_Player;//�v���C���[�̏��
//STICK_ANGLE g_StickAngle;
//-------------------
//�v���C���[�̏���������
//--------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Player.nPatternAnim = 0;						//�J�E���^�[������������
	g_Player.nPatternAnim = 0;						//�p�^�[���i���o�[������������
	g_Player.bDisp = true;
	g_Player.state = PLAYERSTATE_NONE;				//�v���C���[��Ԃ�������
	g_Player.nLife = 50;							//�̗͂̏�����
	g_Player.pos = D3DXVECTOR3(400, 400, 0);		//�ʒu������������
    g_Player.move = D3DXVECTOR3(0 , 0, 0);			//�ړ��ʂ�����������
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������������
	g_Player.junp = 0.0f;							//�W�����v�͂̏�����
	g_Player.gravity = MAX_GRAVITY;					//�d�͂̏�����
	g_Player.PlayerJunp = true;						//�W�����v��Ԃ̏�����
	//�Ίp���m�������Y�o����
	g_Player.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HIGHT * PLAYER_HIGHT)/2;

	//�Ίp���̊p�x���Z�o����
	g_Player.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HIGHT);

		//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\runningman100.png",
		&g_pTexturePlayer);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4, //�K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx; //���_���ւ̂ۂ����

	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
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
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}
//--------------------
//�v���C���[�̏I������
//--------------------
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//-------------------
//�v���C���[�̍X�V����
//--------------------
void UpdatePlayer(void)
{	
	//int nCntLoadi;
	static float fData = 0.1;
	//�X�e�B�b�N��Ԃ̎擾
	XINPUT_STATE* pStick;
	pStick = GetJoySticAngle();
	//�e��Ԃ̎擾
	/*LOADING* pBulletType[MAX_LOADING];
	for (nCntLoadi = 0; nCntLoadi < MAX_LOADING; nCntLoadi++)
	{
		pBulletType[nCntLoadi] = GetLoadingType;
	}*/
		VERTEX_2D* pVtx; //���_���ւ̂ۂ����
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	//�ړ�
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)//A,�\���L�[��
	{//���ړ�
		fData = 0.1;
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)//W,�\���L�[��
		{
			g_Player.move.x += sinf(-D3DX_PI * 0.75) * 0.5;
			g_Player.move.y += cosf(-D3DX_PI * 0.75) * 0.5;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)//S,�\���L�[��
		{
			g_Player.move.x += sinf(-D3DX_PI * 0.25) * 0.5;
			g_Player.move.y += cosf(-D3DX_PI * 0.25) * 0.5;
		}
		else
		{
			g_Player.move.x -= 0.5;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)//D,�\���L�[�E
	{//�E�ړ�
		fData = -0.1;
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)//W,�\���L�[��
		{
			g_Player.move.x += sinf(D3DX_PI * 0.75) * 0.5;
			g_Player.move.y += cosf(D3DX_PI * 0.75) * 0.5;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)//S,�\���L�[��
		{
			g_Player.move.x += sinf(D3DX_PI * 0.25) * 0.5;
			g_Player.move.y += cosf(D3DX_PI * 0.25) * 0.5;
		}
		else
		{
			g_Player.move.x += 0.5;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true  || GetJoypadPress(JOYKEY_UP) == true)//W,�\���L�[��
	{//��ړ�
		g_Player.move.y -= 0.5;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)//S,�\���L�[��
	{//���ړ�
		g_Player.move.y += 0.5;
	}
	else if (GetKeyboardPress(DIK_I) == true)//I
	{//�㎋�_
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
	{//�����_
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
	{//�����_
		g_Player.rot.z = 3.14f;
	}
	else if (GetKeyboardPress(DIK_L) == true)//L
	{//�E���_
		g_Player.rot.z = 0.0f;
	}


	else if (GetJoyStick() == true)
	{
		if (pStick->Gamepad.sThumbLY < -10922)//���ɓ|������
		{//���ړ�
			if (pStick->Gamepad.sThumbLX > 10922)//�E���ɓ|������
			{
				g_Player.move.x += sinf(D3DX_PI * 0.25) * 0.5;
				g_Player.move.y += cosf(D3DX_PI * 0.25) * 0.5;
			}
			else if (pStick->Gamepad.sThumbLX < -10922)//�����ɓ|������
			{
				g_Player.move.x += sinf(-D3DX_PI * 0.25) * 0.5;
				g_Player.move.y += cosf(-D3DX_PI * 0.25) * 0.5;
			}
			else
			{
				g_Player.move.y += 0.5;
			}
		}
		else if (pStick->Gamepad.sThumbLX > 10922)//�E�ɓ|������
		{//�E�ړ�
			g_Player.move.x += 1.5f;
			g_Player.rot.z = 0.0f;
		}
		else if (pStick->Gamepad.sThumbLX < -10922)//���ɓ|������
		{//���ړ�
			g_Player.move.x -= 1.5f;
			g_Player.rot.z = 3.14f;
		}
	}
	//�W�����v
	if (KeyboardTrigger(DIK_C) == true || GetJoypadTrigger(JOYKEY_A) == true && g_Player.PlayerJunp == false)
	{
		g_Player.PlayerJunp = true;
		g_Player.junp = MAX_JUNP;
	}
	//�ˌ�
	if (KeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_X))//�X�y�[�X,x�{�^��
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
	//�����[�h
	if (GetJoyTrigger(JOYKEY_L2) == true)
	{//�ʏ�e
		//SetLoading(BULLETTYPE_PLAYER_NORMAL);
	}


	//�d��
	g_Player.move.y += g_Player.gravity - g_Player.junp;
	g_Player.junp -= g_Player.gravity * 2;//�W�����v�͂�����
	//�ړ��ʂ��X�V(����������)
	g_Player.move.x += (0 - g_Player.move.x) * 0.1;
	g_Player.move.y += (1 - g_Player.move.y) * 0.1;
	//�ʒu���X�V
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	if (g_Player.pos.y >= SCREEN_HEIGHT - (PLAYER_HIGHT / 2))//��ʉ�
	{
		g_Player.pos.y = SCREEN_HEIGHT - (PLAYER_HIGHT / 2);
		g_Player.move.y -= g_Player.move.y;
		g_Player.junp = 0.0f;
		g_Player.PlayerJunp = false;

		if (g_Player.pos.x <= (PLAYER_WIDTH / 2))//��ʍ�
		{
			g_Player.pos.x = (PLAYER_WIDTH / 2);
		}
		else if (g_Player.pos.x >= SCREEN_WIDTH - (PLAYER_WIDTH / 2))//��ʉE
		{
			g_Player.pos.x = SCREEN_WIDTH - (PLAYER_WIDTH / 2);
		}
	}
	else if (g_Player.pos.y <= (PLAYER_HIGHT / 2))//��ʏ�
	{
		g_Player.pos.y = g_Player.OldPos.y;
	}
	else if (g_Player.pos.x <= (PLAYER_WIDTH / 2))//��ʍ�
	{
		g_Player.pos.x = g_Player.OldPos.x;
	}
	else if (g_Player.pos.x >= SCREEN_WIDTH - (PLAYER_WIDTH / 2))//��ʉE
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
			//���_�J���[�̐ݒ�,�ʏ��Ԃɖ߂�
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
			//���[�h�ݒ�(���U���g���)
			SetFade(MODE_RESULT);
			return;
		}
		break;
	}

	g_Player.OldPos = g_Player.pos;
	//���_���W�̍X�V
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

	g_Player.nCounterAnim++; //�J�E���^�[�����Z
	if ((g_Player.nCounterAnim % 5) == 0)
	{
		g_Player.nCounterAnim = 0;
		//�ς�[����X�V
		g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 10;//�p�^�[���i���o�[���[���ɂ���

		pVtx[0].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 - fData) , (g_Player.nPatternAnim/5) * 0.5);
		pVtx[1].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 + fData) , (g_Player.nPatternAnim/5) * 0.5);
		pVtx[2].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 - fData) , (g_Player.nPatternAnim/5) * 0.5 + 0.5);
		pVtx[3].tex = D3DXVECTOR2((g_Player.nPatternAnim * 0.2f) + (0.1 + fData) , (g_Player.nPatternAnim/5) * 0.5 + 0.5);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPlayer->Unlock();
	}

	
}
//--------------------
//�v���C���[�̕`�揈��
//--------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	if (g_Player.bDisp == true)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);
		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,										//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);										//�`�悷��v���~�e�B�u�i�v���C���[�j��
	}
}
//----------------------
//�v���C���[�̃q�b�g����
//----------------------
void HitPlayer(int nDamage)
{
	g_Player.nLife -= nDamage;

	VERTEX_2D* pVtx; //���_���ւ̂ۂ����
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	//�v���C���[�̗̑͂��Ȃ��Ȃ���
	if (g_Player.nLife <= 0)
	{
		//�����̐ݒ�
		g_Player.bDisp = false;
		g_Player.state = PLAYERSTATE_DEATH;
		g_Player.nCounterState = 60;
	}
	else
	{
		g_Player.state = PLAYERSTATE_DAMAGE;
		g_Player.nCounterState = 5;
		//���_�J���[�̐ݒ�,�|���S�����_���[�W�F�ɐݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}
//--------------------
//�v���C���[�̏��
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
//�v���C���[�̎擾
//--------------------
Player* GetPlayer(void)
{
	return &g_Player;
}
