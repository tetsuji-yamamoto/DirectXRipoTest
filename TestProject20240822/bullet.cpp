#include "bullet.h"
#include "explosion.h"
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	int nLife;				//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
	D3DXVECTOR3 rotBullet;	//����
	float fLengthBullet;	//�Ίp���̖�����
	float fAngleBullet;		//�Ίp���̊p�x
	BULLETTYPE type;		//���
}Bullet;
//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];//�e�̏��
//------------------
//�e�̏���������
//------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//�e�N�X�`���̓ǂݍ���
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
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, //�K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//�e�̏��̏�����
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
	g_pVtxBuffBullet->Unlock();
}
//-------------
//�e�̏I������
//-------------
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//--------------------
//�e�̍X�V����
//--------------------
void UpdateBullet(void)
{
	int nCntBullet;

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			Enemy* pEnemy;//�G�̏��ւ̃|�C���^
			Player* pPlayer;//�v���C���[�̏��ւ̃|�C���^
			int nCntEnemy;
			//�G�̎擾
			pEnemy = GetEnemy();
			pPlayer = GetPlayer();
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{

				if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER_NORMAL)
				{//�G�ƒe�̓����蔻��
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - 100 &&
							g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + 100 &&
							g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + 100 &&
							g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - 100)
						{
							//�����̐ݒ�
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
							HitEnemy(nCntEnemy, BEEM_DAMAGE);
							//pEnemy->bUse = false;				//�G���g�p���Ă��Ȃ���Ԃɂ���
							g_aBullet[nCntBullet].bUse = false;	//�e���g�p���Ă��Ȃ���Ԃɂ���
						}
					}
					//�G�t�F�N�g�̐ݒ�
					SetEffect(g_aBullet[nCntBullet].pos,
						D3DXCOLOR(0.5f,0.5f,0.5f,0.5f),
						40.0f,
						0.5f, 
						BULLETTYPE_PLAYER_NORMAL);
				}
				else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
				{//�v���C���[�ƓG�̒e�̓����蔻��
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 100 &&
							g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 100 &&
							g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 100 &&
							g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 100)
						{
							//�����̐ݒ�
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
							g_aBullet[nCntBullet].bUse = false;	//�e���g�p���Ă��Ȃ���Ԃɂ���
							HitPlayer(10);
						}
					}
				}
			}
			//�ʒu���X�V
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
			//���_���W�̍X�V
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
				g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}

			if (g_aBullet[nCntBullet].nLife == 0)//�������s����
			{
				//�����̐ݒ�
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
				g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���l���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}
//--------------------
//�e�̕`�揈��
//--------------------
void DrawBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�̕`��
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
				//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);
			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				4*nCntBullet,							//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);
		}
	}
}
//----------------
//���܂̐ݒ菈��
//----------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLength, float fAngle, D3DXVECTOR3 rot, BULLETTYPE type)
{
	int nCntBullet;

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aBullet[nCntBullet].rotBullet = rot;
			g_aBullet[nCntBullet].fLengthBullet= fLength;
			g_aBullet[nCntBullet].fAngleBullet = fAngle;
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].bUse = true;//�g�p���Ă����Ԃɂ���
			//���_���p�̐ݒ�
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
		pVtx += 4;//���_�f�[�^�̃|�C���^���l���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}