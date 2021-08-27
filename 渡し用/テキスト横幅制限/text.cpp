//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	text.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "text.h"
#include "manager.h"
#include "renderer.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CText::CText() :CScene(OBJTYPE::OBJTYPE_TEXT)
{
    m_pFont = NULL;
    m_pos = DEFAULT_VECTOR;
    m_nSize = 0;
	m_nLimWidth = 0;
    m_col = 0xffffffff;
    m_align = ALIGN_LEFT;
    m_format = NULL;
    ZeroMemory(m_fontName, sizeof(m_fontName));
    ZeroMemory(m_str, sizeof(m_str));
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CText::~CText()
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CText::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	int nWidth = 0;
	if (m_nLimWidth < strlen(m_str)*m_nSize || m_nLimWidth == 0)
	{
		nWidth = m_nLimWidth / strlen(m_str);
	}
	else
	{
		nWidth = m_nSize;
	}
	//フォント生成
	D3DXCreateFont(CManager::GetRenderer()->GetDevice(),
		m_nSize,
		nWidth,
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

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//終了処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CText::Uninit(void)
{
    if (m_pFont != NULL)
    {
        m_pFont->Release();
        m_pFont = NULL;
    }

    // オブジェクトの破棄
    Release();
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//更新処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CText::Update(void)
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//描画処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CText::Draw(void)
{
    RECT rect;
    SetRect(&rect, 0, 0, 0, 0);
    m_pFont->DrawText(NULL, m_str, -1, &rect, DT_CALCRECT | m_format, m_col);
    OffsetRect(&rect, (int)m_pos.x, (int)m_pos.y);
    m_pFont->DrawText(NULL, m_str, -1, &rect, m_format, m_col);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//生成処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CText* CText::Create(D3DXVECTOR3 pos, int nSize, char* pStr, ALIGN align, char* pFontName, D3DCOLOR col, int nLimWidth)
{
    CText* pText = new CText;
    pText->m_pos = pos;
    pText->m_nSize = nSize;
    wsprintf(pText->m_str, pStr);
    pText->m_align = align;
    wsprintf(pText->m_fontName, pFontName);
    pText->m_col = col;
	pText->m_nLimWidth = nLimWidth;
    pText->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
    return pText;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//フォントファイルの読み込み
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CText::Load(void)
{
    AddFontResourceEx("data\\FONT\\NikkyouSans-mLKax.ttf", FR_PRIVATE, 0);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//フォントデータの破棄
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CText::Unload(void)
{
    RemoveFontResourceEx("data\\FONT\\NikkyouSans-mLKax.ttf", FR_PRIVATE, 0);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//テキスト変更関数
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CText::SetText(char * pStr)
{
    wsprintf(m_str, pStr);
}
