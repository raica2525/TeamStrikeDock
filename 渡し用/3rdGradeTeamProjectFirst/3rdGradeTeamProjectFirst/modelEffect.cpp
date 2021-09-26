//===============================================
//
// ���f���G�t�F�N�g�̏��� (modelEffect.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "modelEffect.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CModelEffect::CModelEffect() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nModelType = 0;
    m_col = SCENE3D_EMISSIVE_COLOR;
    m_colChangeRate = DEFAULT_COLOR_NONE;
    m_bUseLight = true;

    m_bUseAdditiveSynthesis = false;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CModelEffect::~CModelEffect()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CModelEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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
void CModelEffect::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::Update(void)
{
    m_col += m_colChangeRate;

    if (m_col.a < 0.0f)
    {
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CModelEffect::Draw(void)
{
    if (m_bUseAdditiveSynthesis)
    {
        CScene3D::SetAdditiveSynthesis();
    }

    CScene3D::Draw(true, m_bUseLight, m_col);
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CModelEffect * CModelEffect::Create(int nModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, D3DXCOLOR colChangeRate, bool bUseLight)
{
    // �|�C���^��p��
    CModelEffect *pModelEffect = NULL;

    // �������m��
    pModelEffect = new CModelEffect;

    // ��Ɍ��т��Ă���
    pModelEffect->m_nModelType = nModelType;
    pModelEffect->SetRot(rot);

    // ������
    pModelEffect->Init(pos, DEFAULT_SCALE);

    // ���������т��Ă���
    pModelEffect->m_col = col;
    pModelEffect->m_colChangeRate = colChangeRate;
    pModelEffect->m_bUseLight = bUseLight;

    return pModelEffect;
}