//===============================================
//
// �A�E�g���C������ (outline.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "outline.h"
#include "manager.h"
#include "renderer.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
COutline::COutline() :CBillboard(CScene::OBJTYPE_NONE_DRAW)
{
    m_bUse = true;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
COutline::~COutline()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT COutline::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �r���{�[�h�̏�����
    CBillboard::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void COutline::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void COutline::Update(void)
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
void COutline::Draw(void)
{
    // �X�e���V���o�b�t�@���g��
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �X�e���V���e�X�g => �L��
    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

    // �X�e���V���e�X�g�Ɣ�r����Q�ƒl�ݒ� => 0x01
    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

    // �X�e���V���e�X�g�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

    // �X�e���V���e�X�g�̔�r���@�ݒ� => 
    // ���̕`��ł̎Q�ƒl == �X�e���V���o�b�t�@�̎Q�ƒl�Ȃ獇�i
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

    // �X�e���V���e�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

    // �r���{�[�h�̕`��
    CBillboard::Draw();

    // �X�e���V���e�X�g����
    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
COutline * COutline::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
    // �������̊m��
    COutline *pOutline = NULL;
    pOutline = new COutline();

    // ���������m�ۏo������
    if (pOutline != NULL)
    {
        // ������
        pOutline->Init(pos, size);

        // �r���{�[�h�֐ݒ�𔽉f
        pOutline->SetRot(rot);
        pOutline->SetCol(col);
    }

    return pOutline;
}