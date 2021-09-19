//===============================================
//
// 3D�|���S������ (scene3d.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "scene3d.h"
#include "manager.h"
#include "renderer.h"
#include "modelData.h"

//========================
// �}�N����`
//========================

//========================================
// �ÓI�����o�ϐ��錾
//========================================
bool CScene3D::m_bAdditiveSynthesis = false;

//=============================================================================
// �I�[�o�[���C�h���ꂽ�R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CScene3D::CScene3D(OBJTYPE objType) :CScene(objType)
{
    memset(m_apTexMat, 0, sizeof(m_apTexMat));
    m_pBuffMat = NULL;
    m_pMesh = NULL;
    m_nNumMat = 0;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_scale = DEFAULT_SCALE;

    memset(m_aDiffuse, 0, sizeof(m_aDiffuse));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    m_pos = pos;

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CScene3D::Uninit(void)
{
    Release();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CScene3D::Update(void)
{
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CScene3D::Draw(bool bChangeDeffuse, bool bUseLight, D3DXCOLOR color)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;
    D3DXMATERIAL *pMat;

    // ���Z����
    if (m_bAdditiveSynthesis)
    {
        // �����_�[�X�e�[�g(���Z�����ɂ���)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    // ���C�g�𖳌���
    if (!bUseLight)
    {
        pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    D3DXMatrixIdentity(&m_mtxWorld);

    D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->GetMaterial(&matDef);

    ZeroMemory(&matDef, sizeof(D3DMATERIAL9));

    pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

    for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
    {
        // �e�N�X�`����\���Ă���Ȃ玩�Ȕ���
        if (m_apTexMat[nCntMat])
        {
            pMat[nCntMat].MatD3D.Emissive = color;
        }
        pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        // �g�U����ς��邩�ǂ���
        if (bChangeDeffuse)
        {
            pMat[nCntMat].MatD3D.Diffuse = color;
        }

        pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

        // �ς����l��߂�
        pMat[nCntMat].MatD3D.Diffuse = m_aDiffuse[nCntMat];

        pDevice->SetTexture(0, m_apTexMat[nCntMat]);

        m_pMesh->DrawSubset(nCntMat);
    }

    pDevice->SetMaterial(&matDef);

    // ���C�g��L����
    if (!bUseLight)
    {
        pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    }

    pDevice->SetTexture(0, NULL);

    // ���Z������߂�
    if (m_bAdditiveSynthesis == true)
    {
        // �����_�[�X�e�[�g(���Z������߂�)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // ���Z�����̃t���O��false�ɖ߂�
        m_bAdditiveSynthesis = false;
    }
}

//=============================================================================
// ���f���f�[�^�̊��蓖��
// Author : �㓡�T�V��
//=============================================================================
void CScene3D::BindModelData(const int nNumModelData)
{
    // ���b�V����}�e���A�����󂯎��
    CModelData *pModelData = CManager::GetModelData();
    m_pMesh = pModelData->GetModelData(nNumModelData)->pMesh;
    m_pBuffMat = pModelData->GetModelData(nNumModelData)->pBuffMat;
    m_nNumMat = pModelData->GetModelData(nNumModelData)->nNumMat;

    // �e�}�e���A���̃e�N�X�`�������󂯎��
    D3DXMATERIAL *pMat;
    pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
    for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
    {
        m_apTexMat[nCntMat] = pModelData->GetModelData(nNumModelData)->apTexMat[nCntMat];
        m_aDiffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
    }
}