//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	input.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

#define NUM_KEY_MAX (256)//キーボードの最大数
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:

	static LPDIRECTINPUT8			m_pInput;	//DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8			m_pDeviceInput;

};

#endif