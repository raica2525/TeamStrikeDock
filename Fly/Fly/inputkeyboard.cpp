//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	inputkeyboard.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "inputkeyboard.h"


CInputKeyboard::CInputKeyboard()
{
	memset(m_aState, 0, sizeof(m_aState));
	memset(m_aStateTrigger, 0, sizeof(m_aStateTrigger));
	memset(m_aStateRelease, 0, sizeof(m_aStateRelease));
}
CInputKeyboard::~CInputKeyboard()
{

}

HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance,hWnd);

	//入力デバイスの作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDeviceInput, NULL))) {
		return E_FAIL;
	}
	//データ・フォーマット設定
	if (FAILED(m_pDeviceInput->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	//強調モード設定
	if (FAILED(m_pDeviceInput->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		return E_FAIL;
	}
	//キーボードへのアクセス権獲得
	m_pDeviceInput->Acquire();

	return S_OK;
}

void CInputKeyboard::Update(void)
{
	BYTE akeyState[NUM_KEY_MAX];//キーの最大数
	int nCntkey;

	//デバイスからデータ取得
	if (SUCCEEDED(m_pDeviceInput->GetDeviceState(sizeof(akeyState), akeyState)))
	{
		for (nCntkey = 0; nCntkey < NUM_KEY_MAX; nCntkey++)
		{
			m_aStateTrigger[nCntkey] = (m_aState[nCntkey] ^ akeyState[nCntkey]) & akeyState[nCntkey];
			m_aStateRelease[nCntkey] = (m_aState[nCntkey] ^ akeyState[nCntkey]) & ~akeyState[nCntkey];
			m_aState[nCntkey] = akeyState[nCntkey];
		}
	}
	else 
	{
		//キーボードのアクセス権取得
		m_pDeviceInput->Acquire();
	}
}

void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
	//入力デバイス開放
	if (m_pDeviceInput != NULL) 
	{
		//キーボードのアクセス権開放
		m_pDeviceInput->Unacquire();
		m_pDeviceInput->Release();
		m_pDeviceInput = NULL;
	}

}

bool CInputKeyboard::GetKeyPress(int nkey)
{
	return (m_aState[nkey] & 0x80) ? true : false;
}
bool CInputKeyboard::GetKeyTrigger(int nkey)
{
	return (m_aStateTrigger[nkey] & 0x80) ? true : false;
}
bool CInputKeyboard::GetKeyRelease(int nkey)
{
	return (m_aStateRelease[nkey] & 0x80) ? true : false;
}
