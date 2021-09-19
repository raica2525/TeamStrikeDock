//===============================================
//
// �r���{�[�h���� (billboard.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
bool CBillboard::m_bAdditiveSynthesis = false;
bool CBillboard::m_bUseZBuffer = true;

//=============================================================================
// �I�[�o�[���C�h���ꂽ�R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBillboard::CBillboard(CScene::OBJTYPE objtype) :CScene(objtype)
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_scale = DEFAULT_SCALE;
    m_col = DEFAULT_COLOR;
    m_fTexX1 = 0.0f;
    m_fTexX2 = 1.0f;
    m_fTexY1 = 0.0f;
    m_fTexY2 = 1.0f;
    m_nAlphaTestBorder = DEFAULT_ALPHATEST_BORDER_3D;
    m_fRotAngle = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBillboard::~CBillboard()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �f�o�C�X���擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �ʒu�A�傫����������
    m_pos = pos;
    m_size = size;

    // ���_�o�b�t�@�𐶐�
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    // ���_���̍X�V
    Update();

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CBillboard::Uninit(void)
{
    // ���_�o�b�t�@���J��
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // �V�[���̊J��
    Release();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CBillboard::Update(void)
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
    vertex1.x = -(m_size.x / 2)*cosf(m_fRotAngle)
        - ((m_size.y / 2))*sinf(m_fRotAngle);
    vertex1.y = -(m_size.x / 2)*sinf(m_fRotAngle)
        + ((m_size.y / 2))*cosf(m_fRotAngle);
    vertex1.z = 0.0f;

    //�E��̒��_
    vertex2.x = (m_size.x / 2)*cosf(m_fRotAngle)
        - ((m_size.y / 2))*sinf(m_fRotAngle);
    vertex2.y = (m_size.x / 2)*sinf(m_fRotAngle)
        + ((m_size.y / 2))*cosf(m_fRotAngle);
    vertex2.z = 0.0f;

    //�����̒��_
    vertex3.x = -(m_size.x / 2)*cosf(m_fRotAngle)
        - (-(m_size.y / 2))*sinf(m_fRotAngle);
    vertex3.y = -(m_size.x / 2)*sinf(m_fRotAngle)
        + (-(m_size.y / 2))*cosf(m_fRotAngle);
    vertex3.z = 0.0f;

    //�E���̒��_
    vertex4.x = (m_size.x / 2)*cosf(m_fRotAngle)
        - (-(m_size.y / 2))*sinf(m_fRotAngle);
    vertex4.y = (m_size.x / 2)*sinf(m_fRotAngle)
        + (-(m_size.y / 2))*cosf(m_fRotAngle);
    vertex4.z = 0.0f;
    //==========================================================================================================

    VERTEX_3D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�
    pVtx[0].pos = vertex1;
    pVtx[1].pos = vertex2;
    pVtx[2].pos = vertex3;
    pVtx[3].pos = vertex4;

    // �e���_�̃e�N�X�`�����W���X�V
    pVtx[0].tex = D3DXVECTOR2(m_fTexX1, m_fTexY1);
    pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY1);
    pVtx[2].tex = D3DXVECTOR2(m_fTexX1, m_fTexY2);
    pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);

    // �e���_�̐F�Ɩ@���x�N�g�����X�V
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = m_col;
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    }

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CBillboard::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;

    // Z�o�b�t�@���g�p����t���O��false�Ȃ�
    if (m_bUseZBuffer == false)
    {
        // Z�o�b�t�@�𖳌���
        pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    }

    // ���C�g�𖳌���
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaTestBorder);

    //�A���t�@�e�X�g�̔�r���@�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    // ���Z����
    if (m_bAdditiveSynthesis == true)
    {
        // �����_�[�X�e�[�g(���Z�����ɂ���)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    // ���[���h�}�g���N�X�̏�����
    D3DXMatrixIdentity(&m_mtxWorld);

    // ��]�����߂ĂȂ��Ȃ�A�J�����ɑ΂��ď�ɐ���
    if (m_rot == DEFAULT_VECTOR)
    {
        D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
        pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
        D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);

        mtxRot._41 = 0.0f;
        mtxRot._42 = 0.0f;
        mtxRot._43 = 0.0f;

        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

        D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
    }
    else
    {
        // �����𔽉f
        D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

        // �ʒu�𔽉f
        D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
    }

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

    pDevice->SetFVF(FVF_VERTEX_3D);

    pDevice->SetTexture(0, m_pTexture);

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // ���C�g��L����
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    // �e�N�X�`����NULL��
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

    //�A���t�@�e�X�g�𖳌���
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    // Z�o�b�t�@���g�p����t���O��false�Ȃ�
    if (m_bUseZBuffer == false)
    {
        // Z�o�b�t�@��L����
        pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

        // Z�o�b�t�@�𖳌�������t���O��true�ɖ߂�
        m_bUseZBuffer = true;
    }
}

//=============================================================================
// �e�N�X�`�����o�C���h���鏈��
// Author : �㓡�T�V��
//=============================================================================
void CBillboard::BindTexture(const int nNumTexture)
{
    CTexture *pTexture = CManager::GetTexture();
    m_pTexture = pTexture->GetInfo(nNumTexture)->pTexture;
}

//=============================================================================
// ���_�J���[�𒼐ڐݒ肷�鏈��
// Author : �㓡�T�V��
//=============================================================================
void CBillboard::SetColorVertex(D3DXCOLOR col)
{
    // �����o�ϐ��Ɍ��т���
    m_col = col;

    // ���_���ւ̃|�C���^
    VERTEX_3D *pVtx = NULL;

    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

    // �e���_�̐F���X�V
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = m_col;
    }

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W���X�V���鏈��
// Author : �㓡�T�V��
//=============================================================================
bool CBillboard::SetUpdateTexLeftToRight(int nMaxPattern)
{
    // ����������ǂ���
    bool bOneRound = false;

    // �e�N�X�`�����W���X�V���銄��
    float fUpdateRate = 0.0f;

    // �p�^�[���ɂ���āA�X�V�ʂ�ς���
    fUpdateRate = 1.0f / (float)nMaxPattern;

    // �e�N�X�`����X���W��1.0f���I�[�o�[���Ă���Ȃ�A�߂�
    if (m_fTexX2 >= 1.0f)
    {
        m_fTexX1 = 0.0f;
        m_fTexX2 = fUpdateRate;
    }
    else
    {
        // ���ꂼ��̃e�N�X�`�����WX�ɉ�����
        m_fTexX1 += fUpdateRate;
        m_fTexX2 += fUpdateRate;
    }

    // �e�N�X�`�����W�̉E����1.0f�𒴂�����
    if (m_fTexX2 >= 1.0f)
    {
        // ����̃t���O��true��
        bOneRound = true;
    }

    return bOneRound;
}
