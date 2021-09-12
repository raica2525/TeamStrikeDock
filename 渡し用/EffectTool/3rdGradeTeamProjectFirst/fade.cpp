//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : �㓡�T�V��
//
//=============================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "fade.h"
#include "renderer.h"
#include "manager.h"

//===========================================================
// �}�N����`
//===========================================================
#define FADE_RATE 0.05f;    // �t�F�[�h�̊���

//===========================================================
// �ÓI�����o�ϐ��錾
//===========================================================
D3DXCOLOR CFade::m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);     // ��
CFade::FADE	CFade::m_fade = FADE_NONE;
CManager::MODE	CFade::m_modeNext = CManager::MODE_NONE;
LPDIRECT3DVERTEXBUFFER9 CFade::m_pVtxBuff = NULL;

//===========================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//===========================================================
CFade::CFade()
{

}

//===========================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//===========================================================
CFade::~CFade()
{

}

//===========================================================
// ����������
// Author : �㓡�T�V��
//===========================================================
HRESULT CFade::Init(CManager::MODE mode)
{
    // �X�V������SetMode�ɓ��邽�߁A�t�F�[�h�A�E�g����n�߂�(�N��������t�F�[�h������)
    m_fade = FADE_OUT;

    // �����̃��[�h�������o�ϐ��Ɍ��т���
    m_modeNext = mode;

    // �����_���[���擾
    CRenderer *pRenderer = CManager::GetRenderer();

    // ���_�o�b�t�@�̐���
    pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY
        , FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    VERTEX_2D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // �e���_�̈ʒu��ݒ�
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    // �e���_�ŋ��ʂ�rhw�A�F��ݒ�
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].rhw = 1.0f;
        pVtx[nCount].col = m_col;
    }

    // ���_�f�[�^���A�����b�N
    m_pVtxBuff->Unlock();

    return S_OK;
}

//==============================================================
// �I������
// Author : �㓡�T�V��
//==============================================================
void CFade::Uninit(void)
{
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
}

//==============================================================
// �X�V����
// Author : �㓡�T�V��
//==============================================================
void CFade::Update(void)
{
    switch (m_fade)
    {
    case FADE_OUT:

        // �t�F�[�h�̃|���S�������񂾂񌩂���
        m_col.a += FADE_RATE;

        // ���S�Ƀt�F�[�h�̃|���S�����\�����ꂽ��
        if (m_col.a >= 1.0f)
        {
            // �����x���Œ�
            m_col.a = 1.0f;

            // ���̃��[�h�ɂ���
            CManager::SetMode(m_modeNext);

            // �t�F�[�h�C������
            m_fade = FADE_IN;
        }

        break;

    case FADE_IN:

        // ���̃��[�h�̉�ʂ����񂾂񌩂���
        m_col.a -= FADE_RATE;

        // ���S�Ɏ��̉�ʂ��\�����ꂽ��
        if (m_col.a <= 0.0f)
        {
            // �����x���Œ�
            m_col.a = 0.0f;

            // �t�F�[�h���g�p���Ă��Ȃ���Ԃɂ���
            m_fade = FADE_NONE;
        }

        break;
    }

    // ���_�o�b�t�@�̍X�V
    VERTEX_2D* pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // �����x���e���_�ɔ��f
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = m_col;
    }

    // ���_�f�[�^���A�����b�N
    m_pVtxBuff->Unlock();
}

//=============================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================
void CFade::Draw(void)
{
    // �����_���[���擾
    CRenderer *pRenderer = CManager::GetRenderer();

    // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

    // �|���S���̕`��
    pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================
// �t�F�[�h�A�E�g���n�߂鏈��
// Author : �㓡�T�V��
//=============================================================
void CFade::SetFade(CManager::MODE mode)
{
    // �t�F�[�h�A�E�g��
    m_fade = FADE_OUT;

    // �t�F�[�h�̃|���S���̐F�𓧖���
    m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

    // �󂯎�������[�h�����̃��[�h�ɐݒ�
    m_modeNext = mode;
}

//=============================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================
CFade * CFade::Create(CManager::MODE mode)
{
    // ���������m��
    CFade *pFade = NULL;
    pFade = new CFade;

    // ������
    pFade->Init(mode);

    return pFade;
}