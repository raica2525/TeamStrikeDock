//===============================================
//
// �w�i�̏��� (bg.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "bg.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBg::CBg() :CScene3D(CScene::OBJTYPE_BG)
{
    m_nModelType = 0;
    m_col = DEFAULT_COLOR;
    m_colPhase = COLOR_PHASE_G_UP;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene3D::Init(pos, size);

    // �X�P�[����ݒ�
    SetScale(size);

    // ���f���f�[�^���o�C���h
    BindModelData(m_nModelType);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CBg::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CBg::Update(void)
{
    // �X�e�[�W�̐��Ȃ�A�F��ς���
    if (m_nModelType == 83)
    {
        const float COLOR_CHANGE_RATE = 0.01f;
        switch (m_colPhase)
        {
        case COLOR_PHASE_R_DOWN:
            m_col.r -= COLOR_CHANGE_RATE;
            if (m_col.r <= 0.0f)
            {
                m_col.r = 0.0f;
                m_colPhase = COLOR_PHASE_B_UP;
            }
            break;
        case COLOR_PHASE_G_DOWN:
            m_col.g -= COLOR_CHANGE_RATE;
            if (m_col.g <= 0.0f)
            {
                m_col.g = 0.0f;
                m_colPhase = COLOR_PHASE_R_UP;
            }
            break;
        case COLOR_PHASE_B_DOWN:
            m_col.b -= COLOR_CHANGE_RATE;
            if (m_col.b <= 0.0f)
            {
                m_col.b = 0.0f;
                m_colPhase = COLOR_PHASE_G_UP;
            }
            break;
        case COLOR_PHASE_R_UP:
            m_col.r += COLOR_CHANGE_RATE;
            if (m_col.r >= 1.0f)
            {
                m_col.r = 1.0f;
                m_colPhase = COLOR_PHASE_B_DOWN;
            }
            break;
        case COLOR_PHASE_G_UP:
            m_col.g += COLOR_CHANGE_RATE;
            if (m_col.g >= 1.0f)
            {
                m_col.g = 1.0f;
                m_colPhase = COLOR_PHASE_R_DOWN;
            }
            break;
        case COLOR_PHASE_B_UP:
            m_col.b += COLOR_CHANGE_RATE;
            if (m_col.b >= 1.0f)
            {
                m_col.b = 1.0f;
                m_colPhase = COLOR_PHASE_G_DOWN;
            }
            break;
        }
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CBg::Draw(void)
{
    // �X�e�[�W�̐��Ȃ�A�F�𔽉f������
    if (m_nModelType == 83)
    {
        CScene3D::Draw(true, true, m_col);
    }
    else
    {
        CScene3D::Draw();
    }
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CBg * CBg::Create(int nModelType, D3DXVECTOR3 pos)
{
    // �|�C���^��p��
    CBg *pBg = NULL;

    // �������m��
    pBg = new CBg;

    // ��Ɏ�ނ����т��Ă���
    pBg->m_nModelType = nModelType;

    // �X�e�[�W�̐��̏ꍇ�A�ŏ��̐F��ς���
    if (nModelType == 83)
    {
        pBg->m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
    }

    // ������
    pBg->Init(pos, DEFAULT_SCALE);

    return pBg;
}