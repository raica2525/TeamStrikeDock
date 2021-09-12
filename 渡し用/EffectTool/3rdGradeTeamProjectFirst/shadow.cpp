//===============================================
//
// �G�t�F�N�g3D���� (shadow.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CShadow::CShadow(CScene::OBJTYPE objtype) :CBillboard(objtype)
{
    m_fRotAngle = 0.0f;
    m_bUse = true;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �r���{�[�h�̏�����
    CBillboard::Init(pos, size);

    //// �e�N�X�`�����o�C���h
    //BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CShadow::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CShadow::Update(void)
{
    // �r���{�[�h���X�V
    CBillboard::Update();

    // �g�p�t���O��false�Ȃ�A����
    if (!m_bUse)
    {
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CShadow::Draw(void)
{
    // �X�e���V���o�b�t�@���g��
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //// Z�o�b�t�@�ݒ� => �L��
    //pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    //// ZBUFFER��r�ݒ�ύX => (�Q�ƒl <= �o�b�t�@�l)
    //pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    // �X�e���V���e�X�g => �L��
    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

    // �X�e���V���e�X�g�Ɣ�r����Q�ƒl�ݒ� => 0x01
    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

    // �X�e���V���e�X�g�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

    // �X�e���V���e�X�g�̔�r���@�ݒ� => 
    //		���̕`��ł̎Q�ƒl == �X�e���V���o�b�t�@�̎Q�ƒl�Ȃ獇�i
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

    // �X�e���V���e�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

    CBillboard::Draw();

    // �X�e���V���e�X�g����
    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
    // �������̊m��
    CShadow *pShadow = NULL;

    // �^�C�v���ƂɁAZ�o�b�t�@�𖳌�������G�t�F�N�g���ǂ��������߂�
    pShadow = new CShadow(CScene::OBJTYPE_EFFECT3D_STENCIL);

    // ���������m�ۏo������
    if (pShadow != NULL)
    {
        // ������
        pShadow->Init(pos, size);

        // �r���{�[�h�֐ݒ�𔽉f
        pShadow->SetRot(rot);
        pShadow->SetCol(col);
    }

    return pShadow;
}