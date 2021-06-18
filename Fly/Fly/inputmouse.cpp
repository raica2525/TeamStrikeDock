//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	inputmouse.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
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

	m_pDeviceInput->SetDataFormat(&c_dfDIMouse2); //ﾏｳｽ用のﾃﾞｰﾀ・ﾌｫｰﾏｯﾄ設定
	m_pDeviceInput->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
	m_pDeviceInput->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	m_pDeviceInput->Acquire();
	return S_OK;
}

/******************************************************************************
* 関数名 : UninitMouse
*
* 引数 : void
* 戻り値 : なし
* 説明 : 終了処理
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
* 関数名 : UpdateMouse
*
* 引数 : void
* 戻り値 : なし
* 説明 : 更新処理
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
* 関数名 : GetMouse
* 引数 : void
* 戻り値 : なし
* 説明 : 終了処理
*******************************************************************************/
bool CInputMouse::GetKeyPress(int Mouse)
{
	return (m_State.rgbButtons[Mouse] & 0x80) ? true : false;
}

bool CInputMouse::GetKeyTrigger(int nkey)
{
	/* 前に取得した時のマウスの状態 */
	static bool prevState[sizeof(m_State.rgbButtons) / sizeof(m_State.rgbButtons[0])];
	/* 今のマウスの状態 */
	bool current = GetKeyPress(nkey);
	/* 前の状態がfalseで、今の状態がtrueならば、クリックした瞬間と判定する */
	bool ret = current && !prevState[nkey];
	/* 今の状態を保存する */
	prevState[nkey] = current;
	/* 判定結果を返す */
	return ret;
}


