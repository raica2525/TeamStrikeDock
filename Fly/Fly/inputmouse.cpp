//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	inputmouse.cpp
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "inputmouse.h"


CInputMouse::CInputMouse()
{
	
}
CInputMouse::~CInputMouse()
{

}
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDeviceInput, NULL)))
	{
		return E_FAIL;
	}

	m_pDeviceInput->SetDataFormat(&c_dfDIMouse2); //ϳ��p���ް��E̫�ϯĐݒ�
	m_pDeviceInput->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
	m_pDeviceInput->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	m_pDeviceInput->Acquire();
	return S_OK;
}

/******************************************************************************
* �֐��� : UninitMouse
*
* ���� : void
* �߂�l : �Ȃ�
* ���� : �I������
*******************************************************************************/
void CInputMouse::Uninit(void)
{

	if (m_pDeviceInput != NULL)
	{
		m_pDeviceInput->Unacquire();
		m_pDeviceInput = NULL;
	}
}


/******************************************************************************
* �֐��� : UpdateMouse
*
* ���� : void
* �߂�l : �Ȃ�
* ���� : �X�V����
*******************************************************************************/
void CInputMouse::Update(void)
{
	for (int nCount = 0; nCount < 4; nCount++)
	{
		m_State.rgbButtons[nCount] = 0;
	}
	m_pDeviceInput->GetDeviceState(sizeof(DIMOUSESTATE2), &m_State);


	m_pDeviceInput->Acquire();

	
}

/******************************************************************************
* �֐��� : GetMouse
* ���� : void
* �߂�l : �Ȃ�
* ���� : �I������
*******************************************************************************/
bool CInputMouse::GetKeyPress(int Mouse)
{
	return (m_State.rgbButtons[Mouse] & 0x80) ? true : false;
}

bool CInputMouse::GetKeyTrigger(int nkey)
{
	/* �O�Ɏ擾�������̃}�E�X�̏�� */
	static bool prevState[sizeof(m_State.rgbButtons) / sizeof(m_State.rgbButtons[0])];
	/* ���̃}�E�X�̏�� */
	bool current = GetKeyPress(nkey);
	/* �O�̏�Ԃ�false�ŁA���̏�Ԃ�true�Ȃ�΁A�N���b�N�����u�ԂƔ��肷�� */
	bool ret = current && !prevState[nkey];
	/* ���̏�Ԃ�ۑ����� */
	prevState[nkey] = current;
	/* ���茋�ʂ�Ԃ� */
	return ret;
}


