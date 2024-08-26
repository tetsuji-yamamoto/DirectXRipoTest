#ifndef _PLAYER_H_ //�}�N����`���ꂦ�ĂȂ�������
#define _PLAYER_H_ //��d�C���N���[�h�h�~�}�N����`
//�}�N����`
#define MAX_JUNP (5.0) //�W�����v��
#define MAX_GRAVITY (0.1) //�d��
//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//�o�����
	PLAYERSTATE_NONE,		//�ʏ���
	PLAYERSTATE_DAMAGE,		//�_���[�W���
	PLAYERSTATE_DEATH,		//���S���
	PLAYERSTATE_JUNP,		//�W�����v���
	PLAYERSTATE_MAX
}PLAYERSTATE;
//�v���C���[�\����                                 �̂������͗ǂ��Ȃ��Ȃ��A
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 OldPos;	//�O�̈ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//����
	PLAYERSTATE state;	//�v���C���[�̏��
	float fLength;		//�Ίp���̒���
	float fAngle;		//�Ίp���̊p�x
	float junp;			//�W�����v
	float gravity;		//�d��
	int nCounterAnim;	//�A�j���J�E���^�[
	int nPatternAnim;	//�A�j���p�^�[��
	int nCounterState;	//��ԊǗ��J�E���^�[
	int nLife;			//�̗�
	bool bDisp;			//�\�����邩���Ȃ���
	bool PlayerJunp;	//�W�����v���
}Player;
//�v���g�^�C�v�錾
Player* GetPlayer(void);
void Uninit(void);
void Updata(void);
void Draw(void);
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);
bool PlayerJunp(PLAYERSTATE playerstate);
#endif 
