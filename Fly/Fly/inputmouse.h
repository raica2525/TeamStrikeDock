//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	inputkeyboard.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_
#include "main.h"
#include "input.h"
class CInputMouse :public CInput
{
public:
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyPress(int nkey);
	bool GetKeyTrigger(int nkey);
private:
	DIMOUSESTATE  m_State;
};

#endif