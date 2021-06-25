//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	text.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#ifndef _TEXT_H_
#define _TEXT_H_

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "main.h"
#include "scene.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�N���X��`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CText : public CScene
{
public:
	typedef enum
	{
		ALIGN_LEFT = 0,
		ALIGN_CENTER,
		ALIGN_RIGHT
	}ALIGN;

	CText();
	virtual ~CText();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CText* Create(D3DXVECTOR3 pos, int nSize, char* pStr, ALIGN align, char* pFontName,D3DCOLOR col);
	static void Load(void);
	static void Unload(void);

private:
	LPD3DXFONT m_pFont;
	D3DXVECTOR3 m_pos;
	int m_nSize;
	D3DCOLOR m_col;
	ALIGN m_align;
	DWORD m_format;
	char m_fontName[124];
	char m_str[512];
};
#endif