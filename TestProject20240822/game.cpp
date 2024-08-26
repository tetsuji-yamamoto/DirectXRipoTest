#include "Bullet.h"
#include "background.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "scor.h"
#include "effect.h"
#include "loading.h"
//�O���[�o���ϐ��錾
GAMESTATE g_gameState = GAMESTATE_NONE;	//�Q�[�����
int g_nCounterGameState = 0;			//��ԊǗ��J�E���^�[
//----------------
//�Q�[����ʂ̏���������
//----------------
void InitGame(void)
{
	//�w�i�̏���������
	InitBG();
	//�v���C���[�̏���������
	InitPlayer();
	//�G�̏���������
	InitEnemy();
	//�G�o��
	SetEnemy(D3DXVECTOR3(700.0f, 200.0f, 0.0f), ENEMYTYPE_ZERO);
	SetEnemy(D3DXVECTOR3(700.0f, 400.0f, 0.0f), ENEMYTYPE_ONE);
	SetEnemy(D3DXVECTOR3(700.0f, 600.0f, 0.0f), ENEMYTYPE_ONE);
	//���[�f�B���O�̏�����
	//InitLoading();
	//�e�̏���������
	InitBullet();
	//�G�t�F�N�g�̏�����
	InitEffect();
	//�����̏���������
	InitExplosion();
	//�X�R�A�̏�����
	InitScore();
	SetScore(0);

	g_gameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;
}
//----------------
//�Q�[����ʂ̏I������
//----------------
void UninitGame(void)
{
	//�w�i�̏I������
	UninitBG();
	//�v���C���[�̏I������
	UninitPlayer();
	//�G�̏I������
	UninitEnemy();
	//���[�f�B���O�̏I������
	//UninitLoading();
	//�e�̏I������
	UninitBullet();
	//�G�t�F�N�g�̏I������
	UninitEffect();
	//�����̏I������
	UninitExplosion();
	//�X�R�A�̏I������
	UninitScore();
}
//----------------
//�Q�[����ʂ̍X�V����
//----------------
void UpdateGame(void)
{
	Enemy* pEnemy=GetEnemy();
	Player* pPlayer=GetPlayer();
	int nNumEnemy;
	//UpdateTitle();
	//�w�i�X�V����
	UpdateBG();
	//�v���C���[�̍X�V����8
	UpdatePlayer();
	//�G�̍X�V����
	UpdateEnemy();
	//���[�f�B���O�̍X�V����
	//UpdateLoading();
	//�e�̍X�V����
	UpdateBullet();
	//�G�t�F�N�g�̍X�V����
	UpdateEffect();
	//�����̍X�V����
	UpdateExplosion();
	//�X�R�A�̍X�V����
	UpdateScore();

	nNumEnemy = GetNumEnemy();

	if (nNumEnemy <= 0)
	{
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL://�ʏ���

		break;

	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			SetFade(MODE_RESULT);
			g_nCounterGameState = 0;
			g_gameState = GAMESTATE_NONE;//�������Ă��Ȃ���Ԃɐݒ�
		}
		break;
	}
}
//----------------
//�Q�[����ʂ̕`�揈��
//----------------
void DrawGame(void)
{
	//�w�i�̕`��
	DrawBG();
	//�v���C���[�̕`��
	DrawPlayer();
	//�G�l�~�[�̕`��
	DrawEnemy();
	//���[�f�B���O�̕`��
	//DrawLoading();
	//�G�t�F�N�g�̕`��
	DrawEffect();
	//�e�̕`��
	DrawBullet();
	//�����̕`��
	DrawExplosion();
	//�X�R�A�̕`��
	DrawScore();
}
//----------------
//�Q�[����Ԃ̐ݒ�
//----------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;

	g_nCounterGameState = 0;
}
//----------------
//�Q�[����Ԃ̎擾
//----------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}