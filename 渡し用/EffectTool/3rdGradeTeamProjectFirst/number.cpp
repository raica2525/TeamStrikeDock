//===============================================
//
// �����\������(�I�u�W�F�N�g���p�����Ă��Ȃ�)  (number.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//=======================================
// �}�N����`
//=======================================
#define MOVE_RATE 1.0f      // ��֍s���ړ���
#define FADE_RATE 0.025f    // �����Ă�����

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//============================================================
// �����\���̃f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//============================================================
CNumber::CNumber()
{
    m_pVtxBuff = NULL;
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_bMove = false;
}

//============================================================
// �����\���̃f�X�g���N�^
// Author : �㓡�T�V��
//============================================================
CNumber::~CNumber()
{

}

//============================================================
// �����\���̏���������
// Author : �㓡�T�V��
//============================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �ʒu�����т���
    m_pos = pos;

    // �傫�������т���
    m_size = size;

    // ���_�o�b�t�@����
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�i�E���Őݒ肷��j
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), +(m_size.y / 2), 0.0f);

    // rhw�̐ݒ�i�l��1.0�Őݒ肷��j
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // ���_�J���[��ݒ�i0.0f�`1.0f�̒l�Őݒ肷��j
    pVtx[0].col = m_col;
    pVtx[1].col = m_col;
    pVtx[2].col = m_col;
    pVtx[3].col = m_col;

    // �e�N�X�`�����W�̐ݒ�
    // �����ł́A0�ɍ��킹�Ă���
    pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * 0), 0.0f);
    pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * 0), 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * 0), 1.0f);
    pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * 0), 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return S_OK;
}

//============================================================
// �����\���̏I������
// Author : �㓡�T�V��
//============================================================
void CNumber::Uninit(void)
{
    // ���_�o�b�t�@�̔j��
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // �I�u�W�F�N�g�̔h���ł͂Ȃ�
    //// �I�u�W�F�N�g�̔j��
    //Release();
}

//============================================================
// �����\���̍X�V����
// Author : �㓡�T�V��
//============================================================
bool CNumber::Update(void)
{
    // �ϐ��錾
    bool bUse = true;   // �g�p����

                        // �����Ȃ�
    if (m_bMove == true)
    {
        // ���X�ɏ��
        m_pos.y -= MOVE_RATE;

        // ���񂾂񓧖���
        m_col.a -= FADE_RATE;
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), +(m_size.y / 2), 0.0f);

    // ���_�J���[��ݒ�i0.0f�`1.0f�̒l�Őݒ肷��j
    pVtx[0].col = m_col;
    pVtx[1].col = m_col;
    pVtx[2].col = m_col;
    pVtx[3].col = m_col;

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    // ���S�ɓ����ɂȂ�����
    if (m_col.a < 0.0f)
    {
        // �����I������
        Uninit();

        // ���g�p��
        bUse = false;
    }

    return bUse;
}

//=============================================================
// �����\���̕`�揈��
// Author : �㓡�T�V��
//=============================================================
void CNumber::Draw(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    // �e�N�X�`���̐ݒ�
    pDevice->SetTexture(0, m_pTexture);

    // �|���S���̕`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // �e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================
// �����\���̃e�N�X�`����ǂݍ���
// Author : �㓡�T�V��
//=============================================================
HRESULT CNumber::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number.png", &m_pTexture);

    return S_OK;
}

//=============================================================
// �����\���̃e�N�X�`����j��
// Author : �㓡�T�V��
//=============================================================
void CNumber::Unload(void)
{
    // �e�N�X�`���̔j��
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//=============================================================
// �����\���̐���
// Author : �㓡�T�V��
//=============================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool bMove)
{
    CNumber *pNumber = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pNumber = new CNumber;

    // ������
    pNumber->m_col = col;
    pNumber->Init(pos, size);

    // �����o�ϐ������т���
    pNumber->m_bMove = bMove;

    return pNumber;
}

//=============================================================
// �����\���̔ԍ��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CNumber::SetNumber(int nNumber)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 0.0f);
    pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 1.0f);
    pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 1.0f);

    // ���_�J���[��ݒ�i0.0f�`1.0f�̒l�Őݒ肷��j
    pVtx[0].col = m_col;
    pVtx[1].col = m_col;
    pVtx[2].col = m_col;
    pVtx[3].col = m_col;

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}
