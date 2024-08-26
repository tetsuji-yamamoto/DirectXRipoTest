#include "input.h"

//�}�N����`
#define NUM_KEY_MAX (256) //�L�[�̍ő吔
//�O���[�o���ϐ��錾
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aOldState[NUM_KEY_MAX];
XINPUT_STATE g_aOldJoyKeyState;
XINPUT_STATE g_joyKeyState;//�W���C�p�b�h�̃v���X���
bool g_joyKeyFlag[JOYKEY_MAX];
//----------------------
//�L�[�{�[�h�̏���������
//----------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�L�[�{�[�h�j�̂�������
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g�𐶐�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}
//------------------------------
//�L�[�{�[�h�̏I������
//------------------------------
void UninitKeyboard(void)
{
	//���̓f�o�C�X�L�[�{�[�h�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}
//------------------------------
//�L�[�{�[�h�̍X�V����
//------------------------------
void UpdataKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];//�L�[�{�[�h�̓��͏���
	int nCntKey;

	for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		g_aOldState[nCntKey] = g_aKeyState[nCntKey];//�ЂƂO�̃L�[�{�[�h�v���X����ۑ�
	}

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyState[nCntKey] = aKeyState[nCntKey];//���[�ځ[�ǂ̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}
//------------------------------
//�L�[�{�[�h�̃u���X�����擾
//-----------------------------
bool GetKeyboardPress(int nKey)//�����Ă��
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}
bool KeyboardTrigger(int nKey)//�������Ƃ�
{
	bool Trigger = false;
	if (g_aKeyState[nKey] & 0x80 && !(g_aOldState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}
bool KeyboardRelease(int nKey)//��������
{
	bool Trigger = false;
	if (g_aOldState[nKey] & 0x80 && !(g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}
bool KeyboardRepeat(int nKey)//������Ă����
{
	bool Trigger = false;
	if (g_aOldState[nKey] & 0x80 && (g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}
//----------------------
//�W���C�p�b�h�̏���������
//----------------------
HRESULT InitJoypad(void)
{
	//�������̃N���A
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	//Xinput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);
	STICK_ANGLE pStick;
	pStick.nStickLX = 0;
	pStick.nStickLY = 0;
	return S_OK;
}
//----------------------
//�W���C�p�b�h�̏I������
//----------------------
void UninitJoypad(void)
{
	//Xinput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}
//----------------------
//�W���C�p�b�h�̍X�V����
//----------------------
void UpdataJoypad(void)
{
	int nCntJoy;

	XINPUT_STATE joyKeyState;//�W���C�p�b�h�̓��͏���
	//�ЂƂO�̃L�[�{�[�h�v���X����ۑ�
	g_aOldJoyKeyState = g_joyKeyState;

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joyKeyState) == ERROR_SUCCESS)
	{
		g_joyKeyState = joyKeyState;//�W���C�p�b�h�̃v���X����ۑ�
	}
	//Press
	for (nCntJoy = 0; nCntJoy < JOYKEY_MAX; nCntJoy++)
	{
		g_joyKeyFlag[nCntJoy] = (g_joyKeyState.Gamepad.wButtons & (0x01 << nCntJoy)) ? true : false;
	}
}
//----------------------
//�W���C�p�b�h�̃v���X�����擾
//----------------------
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}
bool GetJoypadTrigger(JOYKEY key)//�������Ƃ�
{
	bool joykey = false;
	if (g_joyKeyState.Gamepad.wButtons & (0x01 << key) && !(g_aOldJoyKeyState.Gamepad.wButtons & (0x01 << key)))
	{
		joykey = true;
	}
	return joykey;
}
bool GetJoypadRelease(JOYKEY key)//�������Ƃ�
{
	bool joykey = false;
	if (g_aOldJoyKeyState.Gamepad.wButtons & (0x01 << key) && !(g_joyKeyState.Gamepad.wButtons & (0x01 << key)))
	{
		joykey = true;
	}
	return joykey;
}
bool GetJoypadRepeat(JOYKEY key)//�����Ă����
{
	bool joykey = false;
	if (g_aOldJoyKeyState.Gamepad.wButtons & (0x01 << key) && (g_joyKeyState.Gamepad.wButtons & (0x01 << key)))
	{
		joykey = true;
	}
	return joykey;
}
//------------------------
//L2,R2����
//------------------------
bool GetJoyTrigger(JOYKEY key)
{
	if (key == JOYKEY_L2)
	{
		return (g_joyKeyState.Gamepad.bLeftTrigger) ? true : false;
	}
	else if (key == JOYKEY_R2)
	{
		return (g_joyKeyState.Gamepad.bRightTrigger) ? true : false;
	}
}
bool GetJoyTriggerTrigger(JOYKEY key)//�������Ƃ�
{
	bool JoyStick = false;
	if (key == JOYKEY_L2)
	{	
		if ((g_joyKeyState.Gamepad.bLeftTrigger > 0) && !(g_aOldJoyKeyState.Gamepad.bLeftTrigger > 0))
		{
			JoyStick = true;
		}
	}
	else if (key == JOYKEY_R2)
	{
		if ((g_joyKeyState.Gamepad.bRightTrigger > 0) && !(g_aOldJoyKeyState.Gamepad.bRightTrigger > 0))
		{
			JoyStick = true;
		}
	}
	return JoyStick;
}
bool GetJoyTriggerRelease(JOYKEY key)//�������Ƃ�
{
	bool JoyStick = false;
	if (key == JOYKEY_L2)
	{
		if ((g_aOldJoyKeyState.Gamepad.bLeftTrigger > 0) && !(g_joyKeyState.Gamepad.bLeftTrigger > 0))
		{
			JoyStick = true;
		}
	}
	else if (key == JOYKEY_R2)
	{
		if ((g_aOldJoyKeyState.Gamepad.bRightTrigger > 0) && !(g_joyKeyState.Gamepad.bRightTrigger > 0))
		{
			JoyStick = true;
		}
	}
	return JoyStick;
}
bool GetJoyTriggerRepeat(JOYKEY key)//�����Ă����
{
	bool JoyStick = false;
	if (key == JOYKEY_L2)
	{
		if ((g_aOldJoyKeyState.Gamepad.bLeftTrigger > 0) && (g_joyKeyState.Gamepad.bLeftTrigger > 0))
		{
			JoyStick = true;
		}
	}
	else if (key == JOYKEY_R2)
	{
		if ((g_aOldJoyKeyState.Gamepad.bRightTrigger > 0) && (g_joyKeyState.Gamepad.bRightTrigger > 0))
		{
			JoyStick = true;
		}
	}
	return JoyStick;
}
//------------------------
//�X�e�B�b�N����
//------------------------
bool GetJoyStick()
{
	bool joykey = false;
	//�^�񒆂���Ȃ�������
	if (g_joyKeyState.Gamepad.sThumbLX >= 1000 ||
		g_joyKeyState.Gamepad.sThumbLX <= -1000||
		g_joyKeyState.Gamepad.sThumbLY >= 1000 ||
		g_joyKeyState.Gamepad.sThumbLY <= -1000 )
	{
		joykey = true;
	}
	return joykey;
}
//------------------------
//�X�e�B�b�N���̎擾
//------------------------
XINPUT_STATE* GetJoySticAngle(void)
{
	return &g_joyKeyState;
}