//#include "loading.h"
////�O���[�o���ϐ��錾
//LPDIRECT3DTEXTURE9 g_pTextureLoading = NULL;		//Direct3D�e�N�X�`���ւ̃|�C���^
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLoading = NULL;	//���_�o�b�t�@�ւ̃|�C���^
//LOADING g_aLoading[MAX_LOADING];					//���[�f�B���O�̏��
////----------------------------
////���[�f�B���O����������
////----------------------------
//void InitLoading(void)
//{
//	int nCntLoadi;
//	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
//	//�f�o�C�X�̎擾
//	pDevice = GetDevice();	
//	//�e�N�X�`���̓ǂݍ���
//	D3DXCreateTextureFromFile(pDevice,
//		"data\\TEXTURE\\BulletSet003.png",
//		&g_pTextureLoading);
//	//���_�o�b�t�@�̐���
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LOADING, //�K�v�Ȓ��_��
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffLoading,
//		NULL);
//	//���_���ւ̂ۂ����
//	VERTEX_2D* pVtx; 
//	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
//	g_pVtxBuffLoading->Lock(0, 0, (void**)&pVtx, 0);
//	//���[�f�B���O���̏�����
//	for (nCntLoadi = 0; nCntLoadi < MAX_LOADING; nCntLoadi++)
//	{
//		//�ʒu������������
//		g_aLoading[nCntLoadi].pos = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
//		//�e�^�C�v�̏�����
//		g_aLoading[nCntLoadi].nLoadingType = BULLETTYPE_PLAYER_NONE;
//		//���_���W�̐ݒ�
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
//		//rhw�̐ݒ�
//		pVtx[0].rhw = 1.0f;
//		pVtx[1].rhw = 1.0f;
//		pVtx[2].rhw = 1.0f;
//		pVtx[3].rhw = 1.0f;
//		//���_�J���[�̐ݒ�
//		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		//�e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
//
//		//pVtx += 4;
//	}
//	//���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffLoading->Unlock();
//}
////----------------------------
////���[�f�B���O�I������
////----------------------------
//void UninitLoading(void)
//{
//	//�e�N�X�`���̔j��
//	if (g_pTextureLoading != NULL)
//	{
//		g_pTextureLoading->Release();
//		g_pTextureLoading = NULL;
//	}
//	//���_�o�b�t�@�̔j��
//	if (g_pVtxBuffLoading != NULL)
//	{
//		g_pVtxBuffLoading->Release();
//		g_pVtxBuffLoading = NULL;
//	}
//}
////----------------------------
////���[�f�B���O�X�V����
////----------------------------
//void UpdateLoading(void)
//{
//	int nCntLoading;
//
//	VERTEX_2D* pVtx;
//	//���_�o�b�t�@�����b�N���A���傤�Ă���ւ̃|�C���^���擾
//	g_pVtxBuffLoading->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntLoading = 0; nCntLoading < MAX_LOADING; nCntLoading++)
//	{
//		if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_NONE)
//		{//�e�Ȃ���������
//			//�e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.5f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.25f);
//			//�e�̏�Ԃ�Ԃ�
//			//GetNumLoading(BULLETTYPE_PLAYER_NONE);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_NORMAL)
//		{//�ʏ�e��������
//			//�e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.25f);
//			//�e�̏�Ԃ�Ԃ�
//			//GetNumLoading(BULLETTYPE_PLAYER_NORMAL);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_BEAM)
//		{//�r�[���e��������
//			//�e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.25f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.5f);
//			//�e�̏�Ԃ�Ԃ�
//			//GetNumLoading(BULLETTYPE_PLAYER_BEAM);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_EXPLOSION)
//		{//�����e��������
//			//�e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.25f);
//			//�e�̏�Ԃ�Ԃ�
//			//GetNumLoading(BULLETTYPE_PLAYER_EXPLOSION);
//		}
//		else if (g_aLoading[nCntLoading].nLoadingType == BULLETTYPE_PLAYER_HOMING)
//		{//�z�[�~���O�e��������
//			//�e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.25f);
//			//�e�̏�Ԃ�Ԃ�
//			//GetNumLoading(BULLETTYPE_PLAYER_HOMING);
//		}
//		
//		//pVtx += 4;
//	}
//	//���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffLoading->Unlock();
//}
////----------------------------
////���[�f�B���O�`�揈��
////----------------------------
//void DrawLoading(void)
//{
//	int nCntLoading;
//
//	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
//	//�f�o�C�X�̎擾
//	pDevice = GetDevice();
//	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(0, g_pVtxBuffLoading, 0, sizeof(VERTEX_2D));
//	//���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//	//�G�t�F�N�g�̕`��
//	for (nCntLoading = 0; nCntLoading < MAX_LOADING; nCntLoading++)
//	{
//		//�e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, g_pTextureLoading);
//		//�|���S���`��
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
//			4 * nCntLoading,							//�`�悷��ŏ��̒��_�C���f�b�N�X
//			2);
//	}
//}
////----------------------------
////���[�f�B���O�ݒ菈��
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
////���[�f�B���O���̎擾
////----------------------------
////int GetLoadingType(int nLoading)
////{
////	return g_aLoading[nLoading].nLoadingType;
////}