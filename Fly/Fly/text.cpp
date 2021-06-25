//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	text.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "text.h"
#include "manager.h"
#include "renderer.h"
CText::CText()
{
	m_pFont = NULL;
	m_pos = ZERO_VEC;
	m_nSize = 0;
	m_col = 0xffffffff;
	m_align = ALIGN_LEFT;
	m_format = NULL;
	ZeroMemory(m_fontName, sizeof(m_fontName));
	ZeroMemory(m_str, sizeof(m_str));
}
CText::~CText()
{

}
HRESULT CText::Init(void)
{
	//フォント生成
	D3DXCreateFont(CManager::GetRenderer()->GetDevice(),
		m_nSize,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		m_fontName,
		&m_pFont);
	switch (m_align)
	{
	case ALIGN_LEFT:
		m_format = DT_LEFT;
		break;
	case ALIGN_CENTER:
		m_format = DT_CENTER;
		break;
	case ALIGN_RIGHT:
		m_format = DT_RIGHT;
		break;
	default:
		break;
	}
	return S_OK;
}
void CText::Uninit(void)
{
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}
void CText::Update(void)
{

}
void CText::Draw(void)
{
	RECT rect;
	SetRect(&rect, 0, 0, 0, 0);
	m_pFont->DrawText(NULL, m_str, -1, &rect, DT_CALCRECT | m_format, m_col);
	OffsetRect(&rect, m_pos.x, m_pos.y);
	m_pFont->DrawText(NULL, m_str, -1, &rect, m_format, m_col);
}
CText* CText::Create(D3DXVECTOR3 pos, int nSize, char* pStr, ALIGN align, char* pFontName, D3DCOLOR col)
{
	CText* pText = new CText;
	pText->m_pos = pos;
	pText->m_nSize = nSize;
	wsprintf(pText->m_str, pStr);
	pText->m_align = align;
	wsprintf(pText->m_fontName, pFontName);
	pText->m_col = col;
	pText->Init();
	return pText;
}

void CText::Load(void)
{
	AddFontResourceEx("Data\\Fonts\\NikkyouSans-mLKax.ttf", FR_PRIVATE, 0);
}
void CText::Unload(void)
{
	RemoveFontResourceEx("Data\\Fonts\\NikkyouSans-mLKax.ttf", FR_PRIVATE, 0);
}