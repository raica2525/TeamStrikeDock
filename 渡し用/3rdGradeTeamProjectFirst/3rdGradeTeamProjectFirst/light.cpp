//===============================================
//
// �������� (light.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "light.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight()
{
    ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CLight::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    m_Light.Type = D3DLIGHT_DIRECTIONAL;
    m_Light.Diffuse.r = 1.0f;
    m_Light.Diffuse.g = 1.0f;
    m_Light.Diffuse.b = 1.0f;
    m_Light.Diffuse.a = 1.0f;
    m_Light.Direction = D3DXVECTOR3(0.2f, -0.8f, 0.4f);     // ���C�g�̕���
    m_Light.Range = 2000.0f;

    pDevice->SetLight(0, &m_Light);
    pDevice->LightEnable(0, TRUE);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CLight::Update(void)
{
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CLight * CLight::Create(void)
{
    CLight *pLight = NULL;
    pLight = new CLight;
    pLight->Init();
    return pLight;
}
