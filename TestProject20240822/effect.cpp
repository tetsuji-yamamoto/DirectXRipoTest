#include "effect.h"
#include "explosion.h"
#include "main.h"
#include "enemy.h"
#include "player.h"
//�G�t�F�N�g�̍ő吔
#define MAX_EFFECT (4090) //�G�t�F�N�g�̍ő吔
//�G�t�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	BULLETTYPE type;		//���
	D3DXCOLOR col;			//�F
	float fRadius;			//���_���W�̐ݒ�
	float fAlpha;			//�A���t�@�l�̐ݒ�
	float fData;			//�A���t�@�l�̐ݒ�
	int nLife;				//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Effect;
//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];//�G�t�F�N�g�̏��
//------------------
//�G�t�F�N�g�̏���������
//------------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//�e�N�X�`���̓ǂݍ���
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
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, //�K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//�G�t�F�N�g�̏��̏�����
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
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���l���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
//-------------
//�G�t�F�N�g�̏I������
//-------------
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//-------------------
//�G�t�F�N�g�̍X�V����
//--------------------
void UpdateEffect(void)
{
	int nCntEffect;

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			if (g_aEffect[nCntEffect].type == BULLETTYPE_PLAYER_NORMAL)
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, g_aEffect[nCntEffect].fAlpha);
				//���_���W�̐ݒ�
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
		
		if (g_aEffect[nCntEffect].fRadius <= 0)//�������s����
		{
			g_aEffect[nCntEffect].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���l���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
//--------------------
//�G�t�F�N�g�̕`�揈��
//--------------------
void DrawEffect(void)
{
	int nCntEffect;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//�G�t�F�N�g�̕`��
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
				//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);
			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				4 * nCntEffect,							//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);
		}
	}
	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//----------------
//�G�t�F�N�g�̐ݒ菈��
//----------------
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col,float fRadius,float fAlpha, BULLETTYPE type)
{
	int nCntEffect;

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�
			if (type == BULLETTYPE_PLAYER_NORMAL)
			{
				g_aEffect[nCntEffect].pos = pos;
				g_aEffect[nCntEffect].type = type;
				g_aEffect[nCntEffect].fRadius = fRadius;
				g_aEffect[nCntEffect].fAlpha = fAlpha;
				g_aEffect[nCntEffect].col = col;
				g_aEffect[nCntEffect].fData = 0.05;
				g_aEffect[nCntEffect].nLife = 100;
				g_aEffect[nCntEffect].bUse = true;//�g�p���Ă����Ԃɂ���
				//���_���p�̐ݒ�
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
		pVtx += 4;//���_�f�[�^�̃|�C���^���l���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}