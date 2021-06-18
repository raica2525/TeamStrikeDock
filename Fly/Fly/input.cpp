#include "input.h"


LPDIRECTINPUT8			CInput::m_pInput = NULL;

CInput::CInput()
{

}
CInput::~CInput()
{

}

HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//Directlnput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL))) 
	{
		return E_FAIL;
	}
	return S_OK;
}

void CInput::Uninit(void)
{
	//Directlnput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL) {
		m_pInput->Release();
		m_pInput = NULL;
	}
}
