//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	renderer.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "main.h"

class CDebug
{
public:
	CDebug();
	~CDebug();

	static HRESULT Init(void);
	static void Uninit(void);
	static void Print(char* fmt, ...);
	static void Draw(void);

private:
			
	static LPD3DXFONT				m_pFont;			// �t�H���g�ւ̃|�C���^
	static char					m_str[512];

};

#endif
