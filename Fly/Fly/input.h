//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	input.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

#define NUM_KEY_MAX (256)//�L�[�{�[�h�̍ő吔
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:

	static LPDIRECTINPUT8			m_pInput;	//DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8			m_pDeviceInput;

};

#endif