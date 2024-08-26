#include "enemy.h"
#include "main.h"
#include "explosion.h"
#include "bullet.h"
#include "scor.h"
//�}�N��
#define NUM_ENEMY (3)//�G�̎��
#define ENEMY_HEIGHT (100)//�G�̍���
#define ENEMY_WIDTH (100)//�G�̕�
//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Enemy g_aEnemy[NUM_ENEMY];								//�G�̏��
int g_nNumEnemy = 0;									//�G�̑���
//-------------------
//�G�̏���������
//-------------------
void InitEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`��4�����̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\kinoko.png",
		&g_apTextureEnemy[ENEMYTYPE_ZERO]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gohan.jpg",
		&g_apTextureEnemy[ENEMYTYPE_ONE]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_ENEMY, //�K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx; //���_���ւ̂ۂ����
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
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
		//���_���W�̐ݒ�
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(600.0f, 100.0f, 0.0f );
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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���l���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
//-------------------
//�G�̏I������
//-------------------
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}
//-------------------
//�G�̍X�V����
//--------------------
void UpdateEnemy(void)
{
	VERTEX_2D* pVtx; //���_���ւ̂ۂ����
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	int nCntEnemy;
	//�g�p����Ă���G���e�𔭎˂��鏈��
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].nBulletCounter++; //�J�E���^�[�����Z

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
				//�G�A�j���[�V����
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_ZERO)
				{
					g_aEnemy[nCntEnemy].nCounterAnim++; //�J�E���^�[�����Z
					if ((g_aEnemy[nCntEnemy].nCounterAnim % 5) == 0)
					{
						g_aEnemy[nCntEnemy].nCounterAnim = 0;
						//�ς�[����X�V
						g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % 20;//�p�^�[���i���o�[���[���ɂ���

						//�e�N�X�`�����W�̐ݒ�
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
					//���_�J���[�̐ݒ�
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
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
//-------------------
//�G�̕`�揈��
//--------------------
void DrawEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_ZERO)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_ZERO]);
			//�G�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				4 * nCntEnemy,							//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);										//�`�悷��v���~�e�B�u�i�G�j��
		}
		else if(g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_ONE)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_ONE]);
			//�G�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				4 * nCntEnemy,							//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);										//�`�悷��v���~�e�B�u�i�G�j��
		}
	}
}
//----------------
//�G�̐ݒ菈��
//----------------
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (nType == ENEMYTYPE_ZERO)
		{
			if (g_aEnemy[nCntEnemy].bUse == false)
			{//�G���g�p����Ă��Ȃ�
				g_aEnemy[nCntEnemy].pos = pos;
				g_aEnemy[nCntEnemy].nType = nType;
				g_aEnemy[nCntEnemy].nLife = 100;
				g_aEnemy[nCntEnemy].nCountState = 0;
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				g_aEnemy[nCntEnemy].bUse = true;//�g�p���Ă����Ԃɂ���
				//���_���p�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				g_nNumEnemy++;//�G�̑����J�E���g�A�b�v
				break;
			}
		}
		else if (nType == ENEMYTYPE_ONE)
		{
			if (g_aEnemy[nCntEnemy].bUse == false)
			{//�G���g�p����Ă��Ȃ�
				g_aEnemy[nCntEnemy].pos = pos;
				g_aEnemy[nCntEnemy].nType = nType;
				g_aEnemy[nCntEnemy].nLife = 100;
				g_aEnemy[nCntEnemy].nCountState = 0;
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				g_aEnemy[nCntEnemy].bUse = true;//�g�p���Ă����Ԃɂ���
				//���_���p�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y + 100.0f, 0.0f);
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				g_nNumEnemy++;//�G�̑����J�E���g�A�b�v
				break;
			}
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���l���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx; //���_���ւ̂ۂ����
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{//�G�̗̑͂��Ȃ��Ȃ���
		//�����̐ݒ�
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		AddScore(200);
		g_aEnemy[nCntEnemy].bUse = false;
		g_nNumEnemy--;//�G�̑����J�E���g�_�E��
	}
	else
	{
		pVtx += 4 * nCntEnemy;

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCountState = 5;				//�_���[�W��Ԃ�ۂ��Ԃ�ݒ�
		//���_�J���[�̐ݒ�,�|���S�����_���[�W�F�ɐݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
//----------------
//�G�̐ݒ菈��
//----------------
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}
//----------------
//�G�̎擾
//----------------
int GetNumEnemy()
{
	return g_nNumEnemy;
}