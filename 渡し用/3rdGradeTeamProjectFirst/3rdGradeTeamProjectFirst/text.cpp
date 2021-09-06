//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	text.cpp
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "text.h"
#include "manager.h"
#include "renderer.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�R���X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
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

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�f�X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CText::~CText()
{

}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//����������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
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
    //�t�H���g����
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

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�I������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CText::Uninit(void)
{
    if (m_pFont != NULL)
    {
        m_pFont->Release();
        m_pFont = NULL;
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�X�V����
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CText::Update(void)
{

}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�`�揈��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CText::Draw(void)
{
    RECT rect;
    SetRect(&rect, 0, 0, 0, 0);
    m_pFont->DrawText(NULL, m_str, -1, &rect, DT_CALCRECT | m_format, m_col);
    OffsetRect(&rect, (int)m_pos.x, (int)m_pos.y);
    m_pFont->DrawText(NULL, m_str, -1, &rect, m_format, m_col);
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//��������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
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

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�t�H���g�t�@�C���̓ǂݍ���
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CText::Load(void)
{
    AddFontResourceEx("data\\FONT\\ReggaeOne-Regular.ttf", FR_PRIVATE, 0);
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�t�H���g�f�[�^�̔j��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CText::Unload(void)
{
    RemoveFontResourceEx("data\\FONT\\ReggaeOne-Regular.ttf", FR_PRIVATE, 0);
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�e�L�X�g�ύX�֐�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CText::SetText(char * pStr)
{
    wsprintf(m_str, pStr);
}