//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	inputkeyboard.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_
#include "main.h"
#include "input.h"
#define NUM_KEY_MAX (256)//�L�[�{�[�h�̍ő吔
class CInputKeyboard:public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyPress(int nkey);
	bool GetKeyTrigger(int nkey);
	bool GetKeyRelease(int nkey);
private:
	BYTE m_aState[NUM_KEY_MAX];
	BYTE m_aStateTrigger[NUM_KEY_MAX];
	BYTE m_aStateRelease[NUM_KEY_MAX];

};

#endif