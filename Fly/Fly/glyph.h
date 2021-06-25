//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	glyph.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "main.h"
#include "scene2d.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//前方宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CGlyph : public CScene2d
{
public:
	CGlyph();
	~CGlyph();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGlyph* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TCHAR* chara);

	static void Load(void);
	static void Unload(void);

private:
	static LPD3DXFONT m_pFont;
	TCHAR *m_pChar;
	LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif