//=============================================================================
//
// �O�Ղ̏��� [locus.cpp]
// Author : �ɓ��z��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================	
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "locus.h"
#include "texture.h"
#include <D3dx9math.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define LUCOS_SUBALPHA (0.025f) // �����x��������l
#define LOCUS_TEXTURE_NUM (57)  // �O�Ղ̃e�N�X�`���̃i���o�[

//=============================================================================
// �R���X�g���N�^
// Author : �ɓ��z��
//=============================================================================
CLocus::CLocus(CScene::OBJTYPE objtype) : CScene(objtype)
{
    m_pTexture = NULL;
    m_pBuffMat = NULL;
    memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
    m_col = DEFAULT_COLOR;
    m_TypeLucus = TYPE_NONE;
    m_ShrinkSpeed = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
// Author : �ɓ��z��
//=============================================================================
CLocus::~CLocus()
{
}

//=============================================================================
// ��������
// pos1, pos2		: ���݂̃t���[���̍��W
// posOld1, posOld2	: 1�t���[���O�̍��W
// TypeLucus		: �O�Ղ̎��
// ShrinkSpeed		: �O�Ղ̏k�܂鑬�x
// Author : �ɓ��z��
//=============================================================================
CLocus * CLocus::Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2, D3DXCOLOR col, TYPE_LOCUS TypeLucus, float ShrinkSpeed)
{
    // �������m��
    CLocus *pLocus = new CLocus(CScene::OBJTYPE_EFFECT3D);

    // NULL�m�F
    if (pLocus != NULL)
    {
        // ����������
        pLocus->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);

        // ���W�ݒ�
        pLocus->SetPos(pos1, pos2, posOld1, posOld2);

        // ���W�ݒ�
        pLocus->m_col = col;

        // �O�Ղ̎��
        pLocus->m_TypeLucus = TypeLucus;

        // �O�Ղ̏k�߂鑬��
        pLocus->m_ShrinkSpeed = ShrinkSpeed;
    }

    return pLocus;
}

//=============================================================================
// ����������
// Author : �ɓ��z��
//=============================================================================
HRESULT CLocus::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �����_���[�擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    BindTexture(LOCUS_TEXTURE_NUM);

    // ���_�o�b�t�@�̐���
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	//���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
        D3DUSAGE_WRITEONLY,													//���_�o�b�t�@�̎g�p�@
        FVF_VERTEX_3D,														//�g�p���钸�_�t�H�[�}�b�g
        D3DPOOL_MANAGED,													//���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
        &m_pBuffMat,														//���_�o�b�t�@�ւ̃|�C���^
        NULL)))																//NULL�ɐݒ�
    {
        return E_FAIL;
    }

    // ���_����ݒ�
    VERTEX_3D *pVtx = NULL;

    // ���_�o�b�t�@�̃��b�N
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // ���_������
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        // �ꏊ�̐ݒ�
        pVtx[nCount].pos = DEFAULT_VECTOR;

        // �@���̐ݒ�
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        // �J���[�̐ݒ�
        pVtx[nCount].col = m_col;
    }

    // �e�N�X�`�����W�̐ݒ�
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    // ���_�o�b�t�@�̃A�����b�N
    m_pBuffMat->Unlock();

    return S_OK;
}

//=============================================================================
// �I������
// Author : �ɓ��z��
//=============================================================================
void CLocus::Uninit(void)
{
    // ���_�o�b�t�@�̔j��
    if (m_pBuffMat != NULL)
    {
        m_pBuffMat->Release();
        m_pBuffMat = NULL;
    }

    // �j��
    Release();
}

//=============================================================================
// �X�V����
// Author : �ɓ��z��
//=============================================================================
void CLocus::Update(void)
{
    // ���_����ݒ�
    VERTEX_3D *pVtx = NULL;

    // ���_�o�b�t�@�̃��b�N
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // �����x��������
    m_col.a -= LUCOS_SUBALPHA;

    // ���_�o�b�t�@�̃A�����b�N
    m_pBuffMat->Unlock();

    // �ݒ肵����ނɂ���ď�����ς���
    switch (m_TypeLucus)
    {
    case TYPE_SHRINK:
        Shrink();
    default:
        break;
    }

    // �F�̐ݒ�
    SetColor(m_col);

    // ���l��0�����ɂȂ�����I��
    if (m_col.a < 0.0f)
    {
        Uninit();
        return;
    }
}

//=============================================================================
// �`�揈��
// Author : �ɓ��z��
//=============================================================================
void CLocus::Draw(void)
{
    // �����_���[�擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���C�g����
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    // �A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    // �A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

    // �A���t�@�e�X�g�̔�r���@�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    // �}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&m_mtxWorld);

    // ���[���h�}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    // ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, m_pBuffMat, 0, sizeof(VERTEX_3D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_3D);

    // �Z�b�g�e�N�X�`��
    pDevice->SetTexture(0, m_pTexture);

    // �|���S���`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // �e�N�X�`���̏�����
    pDevice->SetTexture(0, NULL);

    // ���C�g�L��
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    // �A���t�@�e�X�g�𖳌���
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// �O�Ղ��k�߂鏈��
// Author : �ɓ��z��
//=============================================================================
void CLocus::Shrink()
{
    // ���_����ݒ�
    VERTEX_3D *pVtx = NULL;

    // ���_�o�b�t�@�̃��b�N
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // ���E�̒��_���߂Â��邽�߂̊p�x���v�Z����
    float fAngle1 = atan2f(pVtx[2].pos.y - pVtx[0].pos.y, pVtx[2].pos.x - pVtx[0].pos.x);
    float fAngle2 = atan2f(pVtx[0].pos.y - pVtx[2].pos.y, pVtx[0].pos.x - pVtx[2].pos.x);
    float fAngle3 = atan2f(pVtx[3].pos.y - pVtx[1].pos.y, pVtx[3].pos.x - pVtx[1].pos.x);
    float fAngle4 = atan2f(pVtx[1].pos.y - pVtx[3].pos.y, pVtx[1].pos.x - pVtx[3].pos.x);

    // �k�߂�v�Z�����ĉ��Z
    pVtx[0].pos.x += cosf(fAngle1) * m_ShrinkSpeed;
    pVtx[0].pos.y += sinf(fAngle1) * m_ShrinkSpeed;

    pVtx[1].pos.x += cosf(fAngle3) * m_ShrinkSpeed;
    pVtx[1].pos.y += sinf(fAngle3) * m_ShrinkSpeed;

    pVtx[2].pos.x += cosf(fAngle2) * m_ShrinkSpeed;
    pVtx[2].pos.y += sinf(fAngle2) * m_ShrinkSpeed;

    pVtx[3].pos.x += cosf(fAngle4) * m_ShrinkSpeed;
    pVtx[3].pos.y += sinf(fAngle4) * m_ShrinkSpeed;

    //���_�o�b�t�@�̃A�����b�N
    m_pBuffMat->Unlock();
}

//=============================================================================
// ���W�ݒ�
// pos1, pos2		: ���݂̍��W
// posOld1, posOld2	: 1�t���[���O�̍��W
// Author : �ɓ��z��
//=============================================================================
void CLocus::SetPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2)
{
    // ���_����ݒ�
    VERTEX_3D *pVtx = NULL;

    // ���_�o�b�t�@�̃��b�N
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // �ꏊ�̐ݒ�
    pVtx[0].pos = posOld1;
    pVtx[1].pos = pos1;
    pVtx[2].pos = posOld2;
    pVtx[3].pos = pos2;

    //���_�o�b�t�@�̃A�����b�N
    m_pBuffMat->Unlock();
}

//=============================================================================
// �F�̐ݒ�
// col	: �F�̏��
// Author : �ɓ��z��
//=============================================================================
void CLocus::SetColor(D3DXCOLOR col)
{
    // ���_����ݒ�
    VERTEX_3D *pVtx = NULL;

    // ���_�o�b�t�@�̃��b�N
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // �e���_�̐F���X�V
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = col;
    }

    //���_�o�b�t�@�̃A�����b�N
    m_pBuffMat->Unlock();
}

//=============================================================================
// �e�N�X�`���󂯓n��
// Author : �ɓ��z��
//=============================================================================
void CLocus::BindTexture(int nNumTexture)
{
    CTexture *pTexture = CManager::GetTexture();
    m_pTexture = pTexture->GetInfo(nNumTexture)->pTexture;
}