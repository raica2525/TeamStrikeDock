//===============================================
//
// �V�[�����2D�|���S������ (scene2d.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
bool CScene2D::m_bAdditiveSynthesis = false;
bool CScene2D::m_bAlphaTest = false;
bool CScene2D::m_bNega = false;

//===========================================
// �e���v���[�g�֐�                       
//===========================================

//===========================================
// ����Z
//
// ����   :data0,data1�̓f�[�^
// �߂�l :�v�Z����
//===========================================
template<class T> T Divide(const T data0, const T data1)
{
    // �ϐ��錾
    T answer;   // ����

                // ����
    answer = data0 / data1;

    return answer;
}

//============================================================
// �V�[�����2D�|���S���̃f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//============================================================
CScene2D::CScene2D()
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    m_nCounterAnim = 0;
    m_nPatternAnim = 0;
}

//============================================================
// �V�[�����2D�|���S���̃I�[�o�[���C�h���ꂽ�R���X�g���N�^
// Author : �㓡�T�V��
//============================================================
CScene2D::CScene2D(OBJTYPE objType) :CScene(objType)
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_nCounterAnim = 0;
    m_nPatternAnim = 0;
}

//============================================================
// �V�[�����2D�|���S���̃f�X�g���N�^
// Author : �㓡�T�V��
//============================================================
CScene2D::~CScene2D()
{

}

//============================================================
// �V�[�����2D�|���S���̏���������
// Author : �㓡�T�V��
//============================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �ʒu�����т���
    m_pos = pos;

    // �傫�������т���
    m_size = size;

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^

                            //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

                                                //���_���W�̐ݒ�i�E���Őݒ肷��j
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // rhw�̐ݒ�i�l��1.0�Őݒ肷��j
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // ���_�J���[��ݒ�i0.0f�`1.0f�̒l�Őݒ肷��j
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //�e�N�X�`�����W�̐ݒ�
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return S_OK;
}

//============================================================
// �V�[�����2D�|���S���̏I������
// Author : �㓡�T�V��
//============================================================
void CScene2D::Uninit(void)
{
    // ���_�o�b�t�@�̔j��
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//============================================================
// �V�[�����2D�|���S���̍X�V����
// Author : �㓡�T�V��
//============================================================
void CScene2D::Update(void)
{

}

//=============================================================
// �V�[�����2D�|���S���̕`�揈��
// Author : �㓡�T�V��
//=============================================================
void CScene2D::Draw(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    // �A���t�@�e�X�g
    if (m_bAlphaTest == true)
    {
        //�A���t�@�e�X�g��L����
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        //�A���t�@�e�X�g��l�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
        //�A���t�@�e�X�g�̔�r���@�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    }

    // ���Z����
    if (m_bAdditiveSynthesis == true)
    {
        // �����_�[�X�e�[�g(���Z�����ɂ���)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    // ���]����
    if (m_bNega == true)
    {
        // ���]�����ōl���ł��Ȃ����l���A�A���t�@�e�X�g�Ő��䂷��
        // �A���t�@�e�X�g��L����
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        // �A���t�@�e�X�g��l�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
        // �A���t�@�e�X�g�̔�r���@�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

        // ���]����
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
    }

    // �e�N�X�`���̐ݒ�
    pDevice->SetTexture(0, m_pTexture);

    // �|���S���̕`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // ���]������߂�
    if (m_bNega == true)
    {
        // �A���t�@�e�X�g�𖳌���
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

        // ���]������߂�
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        m_bNega = false;
    }

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

    // �A���t�@�e�X�g��߂�
    if (m_bAlphaTest == true)
    {
        //�A���t�@�e�X�g�𖳌���
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

        // �A���t�@�e�X�g�̃t���O��false�ɖ߂�
        m_bAlphaTest = false;
    }

    // �e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================
// �V�[�����2D�|���S���̃e�N�X�`�������蓖��
// Author : �㓡�T�V��
//=============================================================
void CScene2D::BindTexture(const int nNumTexture)
{
    CTexture *pTexture = CManager::GetTexture();
    m_pTexture = pTexture->GetInfo(nNumTexture)->pTexture;
}

//=============================================================
// �V�[�����2D�|���S���̒��_���W��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetVertex(void)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), +(m_size.y / 2), 0.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �V�[�����2D�|���S���̉�]���钸�_���W��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetRotVertex(float fAngle)
{
    // �ϐ��錾
    // �e���_
    D3DXVECTOR3 vertex1;
    D3DXVECTOR3 vertex2;
    D3DXVECTOR3 vertex3;
    D3DXVECTOR3 vertex4;

    //==========================================================================================================
    //�摜���A�摜�̒��S�����ɉ�]������
    //����̒��_
    vertex1.x = -(m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex1.y = -(m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);
    vertex1.z = 0.0f;

    //�E��̒��_
    vertex2.x = (m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex2.y = (m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);
    vertex2.z = 0.0f;

    //�����̒��_
    vertex3.x = -(m_size.x / 2)*cosf(fAngle)
        - (m_size.y / 2)*sinf(fAngle);
    vertex3.y = -(m_size.x / 2)*sinf(fAngle)
        + (m_size.y / 2)*cosf(fAngle);
    vertex3.z = 0.0f;

    //�E���̒��_
    vertex4.x = (m_size.x / 2)*cosf(fAngle)
        - (m_size.y / 2)*sinf(fAngle);
    vertex4.y = (m_size.x / 2)*sinf(fAngle)
        + (m_size.y / 2)*cosf(fAngle);
    vertex4.z = 0.0f;
    //==========================================================================================================

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�
    pVtx[0].pos = m_pos + vertex1;
    pVtx[1].pos = m_pos + vertex2;
    pVtx[2].pos = m_pos + vertex3;
    pVtx[3].pos = m_pos + vertex4;

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �V�[�����2D�|���S���̌�������̒��_���W��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�
    pVtx[0].pos = posVisual + D3DXVECTOR3(-(sizeVisual.x / 2), -(sizeVisual.y / 2), 0.0f);
    pVtx[1].pos = posVisual + D3DXVECTOR3(+(sizeVisual.x / 2), -(sizeVisual.y / 2), 0.0f);
    pVtx[2].pos = posVisual + D3DXVECTOR3(-(sizeVisual.x / 2), +(sizeVisual.y / 2), 0.0f);
    pVtx[3].pos = posVisual + D3DXVECTOR3(+(sizeVisual.x / 2), +(sizeVisual.y / 2), 0.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �V�[�����2D�|���S���̈ʒu��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
    m_pos = pos;
}

//=============================================================
// �V�[�����2D�|���S���̑傫����ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetSize(D3DXVECTOR3 size)
{
    m_size = size;
}

//=============================================================
// �V�[�����2D�|���S���̃A�j���[�V������ݒ�
// Author : �㓡�T�V��
//=============================================================
bool CScene2D::SetAnimation(int nSpeed, int nPattern)
{
    // �ϐ��錾
    bool bOneRound = false;   // �A�j���[�V����������������ǂ���

                              // �A�j���[�V����
    m_nCounterAnim++;	//�J�E���^���Z
    if (m_nCounterAnim == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_nCounterAnim = 0;  // �J�E���^��0�ɖ߂�

                             // �A�j���[�V���������������
        if ((m_nPatternAnim + 1) % nPattern == 0)
        {
            // ����̃t���O��true��
            bOneRound = true;
        }

        // �A�j���[�V������؂�ւ���
        m_nPatternAnim = (m_nPatternAnim + 1) % nPattern;  // ����
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

                                   // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + fEqualDivision, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + fEqualDivision, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// �V�[�����2D�|���S���̋t�����̃A�j���[�V������ݒ�
// Author : �㓡�T�V��
//=============================================================
bool CScene2D::SetReverseAnimation(int nSpeed, int nPattern)
{
    // �ϐ��錾
    bool bOneRound = false;   // �A�j���[�V����������������ǂ���

                              // �A�j���[�V����
    m_nCounterAnim++;	//�J�E���^���Z
    if (m_nCounterAnim == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_nCounterAnim = 0;  // �J�E���^��0�ɖ߂�

                             // �A�j���[�V���������������
        if ((m_nPatternAnim + 1) % nPattern == 0)
        {
            // ����̃t���O��true��
            bOneRound = true;
        }

        // �A�j���[�V������؂�ւ���
        m_nPatternAnim = (m_nPatternAnim + 1) % nPattern;  // ����
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

                                   // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + fEqualDivision, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + fEqualDivision, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// �V�[�����2D�|���S���̃A�j���[�V������ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetFlowingAnimation(int nSpeed, int nPattern, bool bRightToLeft, DIRECT direct)
{
    // �A�j���[�V����
    m_nCounterAnim++;	//�J�E���^���Z
    if (m_nCounterAnim == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_nCounterAnim = 0;  // �J�E���^��0�ɖ߂�

        if (bRightToLeft)
        {
            // �A�j���[�V������؂�ւ���
            m_nPatternAnim = (m_nPatternAnim + 1) % nPattern;  // ����
        }
        else
        {
            // �A�j���[�V������؂�ւ���
            m_nPatternAnim = (m_nPatternAnim - 1) % nPattern;  // ����
        }
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

    // �e�N�X�`�����W�̐ݒ�
    // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

    // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    switch (direct)
    {
    case CScene2D::DIRECT_VERTICAL:
        // �c
        pVtx[0].tex = D3DXVECTOR2(0.0f, m_nPatternAnim * fEqualDivision);
        pVtx[1].tex = D3DXVECTOR2(1.0f, m_nPatternAnim * fEqualDivision);
        pVtx[2].tex = D3DXVECTOR2(0.0f, m_nPatternAnim * fEqualDivision + 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, m_nPatternAnim * fEqualDivision + 1.0f);
        break;

    case CScene2D::DIRECT_HORIZON:
        // ��
        pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, 1.0f);
        break;

    case CScene2D::DIRECT_RIGHT_UP:
        // �E���オ��
        pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, m_nPatternAnim * (fEqualDivision * (-1)));
        pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, m_nPatternAnim * (fEqualDivision * (-1)));
        pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, m_nPatternAnim * (fEqualDivision * (-1)) + 1.0f);
        pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, m_nPatternAnim * (fEqualDivision * (-1)) + 1.0f);

        break;

    case CScene2D::DIRECT_RIGHT_DOWN:
        // �E��������
        pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, m_nPatternAnim * fEqualDivision);
        pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, m_nPatternAnim * fEqualDivision);
        pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, m_nPatternAnim * fEqualDivision + 1.0f);
        pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, m_nPatternAnim * fEqualDivision + 1.0f);
        break;
    }

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �e�N�X�`���̕`��͈͂̐ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetTextureRange(int nRange, int nPattern)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

                                   // ���������邩�v�Z
    fEqualDivision = Divide((float)nRange, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(fEqualDivision, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(fEqualDivision, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �i���̂���A�j���[�V�����̐ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetParagraphAnimation(int nParagraph, int nMaxParagraph, int nSpeed, int nPattern)
{
    // �A�j���[�V����
    m_nCounterAnim++;	//�J�E���^���Z
    if (m_nCounterAnim == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_nCounterAnim = 0;  // �J�E���^��0�ɖ߂�

                             // �A�j���[�V������؂�ւ���
        m_nPatternAnim = (m_nPatternAnim + 1) % nPattern;  // ����
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivisionX = 0.0f;   // �e�N�X�`���𓙕�����i�c�j
    float fEqualDivisionY = 0.0f;   // �e�N�X�`���𓙕�����i���j

                                    // ���������邩�v�Z
    fEqualDivisionX = Divide(1.0f, (float)nPattern);
    fEqualDivisionY = Divide(1.0f, (float)nMaxParagraph);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivisionX, fEqualDivisionY * (nParagraph - 1));
    pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivisionX + fEqualDivisionX, fEqualDivisionY * (nParagraph - 1));
    pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivisionX, fEqualDivisionY * nParagraph);
    pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivisionX + fEqualDivisionX, fEqualDivisionY * nParagraph);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �e�N�X�`���̕`��ꏊ�����߂�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetTexturePlace(int nPlace, int nPattern)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

                                   // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex = D3DXVECTOR2((fEqualDivision * (nPlace - 1)), 0.0f);
    pVtx[1].tex = D3DXVECTOR2(fEqualDivision + (fEqualDivision * (nPlace - 1)), 0.0f);
    pVtx[2].tex = D3DXVECTOR2((fEqualDivision * (nPlace - 1)), 1.0f);
    pVtx[3].tex = D3DXVECTOR2(fEqualDivision + (fEqualDivision * (nPlace - 1)), 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �i��������e�N�X�`���̕`��ꏊ�����߂�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetParagraphTexturePlace(int nPlace, int nParagraph, int nMaxParagraph, int nPattern)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivisionX = 0.0f;   // �e�N�X�`���𓙕�����i�c�j
    float fEqualDivisionY = 0.0f;   // �e�N�X�`���𓙕�����i���j

                                    // ���������邩�v�Z
    fEqualDivisionX = Divide(1.0f, (float)nPattern);
    fEqualDivisionY = Divide(1.0f, (float)nMaxParagraph);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex = D3DXVECTOR2((fEqualDivisionX * (nPlace - 1)), fEqualDivisionY * (nParagraph - 1));
    pVtx[1].tex = D3DXVECTOR2(fEqualDivisionX + (fEqualDivisionX * (nPlace - 1)), fEqualDivisionY * (nParagraph - 1));
    pVtx[2].tex = D3DXVECTOR2((fEqualDivisionX * (nPlace - 1)), fEqualDivisionY * nParagraph);
    pVtx[3].tex = D3DXVECTOR2(fEqualDivisionX + (fEqualDivisionX * (nPlace - 1)), fEqualDivisionY * nParagraph);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �A�j���[�V�����̃J�E���^�𗘗p����Ƃ��Ɏg��
// Author : �㓡�T�V��
//=============================================================
int CScene2D::CountAnimation(int nSpeed, int nPattern)
{
    // �A�j���[�V����
    m_nCounterAnim++;	//�J�E���^���Z
    if (m_nCounterAnim == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_nCounterAnim = 0;  // �J�E���^��0�ɖ߂�

                             // �A�j���[�V������؂�ւ���
        m_nPatternAnim = (m_nPatternAnim + 1) % nPattern;  // ����
    }

    // �A�j���[�V�����̃p�^�[���̃J�E���^��Ԃ�
    return m_nPatternAnim;
}

//=============================================================
// �V�[�����2D�|���S���̐F��ς���
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // ���_�J���[��ݒ�i0.0f�`1.0f�̒l�Őݒ肷��j
    pVtx[0].col = col;
    pVtx[1].col = col;
    pVtx[2].col = col;
    pVtx[3].col = col;

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �����Q�[�W�̃T�C�Y��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetLeftToRightGauge(float fMax, float fNow)
{
    // �d��
    float fWeight = fNow / fMax;

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // ���_���W�̐ݒ�(���ɑ����Ă���A�E�ɐL�΂��C���[�W)
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2) + (m_size.x * fWeight), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2) + (m_size.x * fWeight), +(m_size.y / 2), 0.0f);

    // �e�N�X�`�����W���X�V
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(fWeight, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(fWeight, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}