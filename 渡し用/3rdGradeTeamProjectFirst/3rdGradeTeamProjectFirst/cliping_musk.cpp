//===============================================
//
// �N���b�s���O�}�X�N�̏��� (cliping_musk.cpp)
// Author : �r�c�I��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "cliping_musk.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=======================
// �ÓI�����o�ϐ��錾
//=======================
bool CClipingMusk::m_abUsingReferenceValue[MAX_REFERENCE_VALUE] = {};

//============================================================
// �V�[�����2D�|���S���̃f�t�H���g�R���X�g���N�^
// Author : �r�c�I��
//============================================================
CClipingMusk::CClipingMusk()
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = DEFAULT_VECTOR;
	m_size = DEFAULT_VECTOR;

	m_nAlphaTestBorder = DEFAULT_ALPHATEST_BORDER_2D;

	//�Q�ƒl�ݒ�
	for (int nCount = 0; nCount < MAX_REFERENCE_VALUE; nCount++)
	{
		if (m_abUsingReferenceValue[nCount] == false)
		{
			m_dwReferenceValue = nCount;
			m_abUsingReferenceValue[nCount] = true;
			break;
		}
	}

}

//============================================================
// �V�[�����2D�|���S���̃f�X�g���N�^
// Author : �r�c�I��
//============================================================
CClipingMusk::~CClipingMusk()
{

}

//============================================================
// �V�[�����2D�|���S���̏���������
// Author : �r�c�I��
//============================================================
HRESULT CClipingMusk::Init(void)
{
	// �����_���[����f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

							// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex[0] = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex[0] = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex[0] = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex[0] = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================
// �V�[�����2D�|���S���̏I������
// Author : �r�c�I��
//============================================================
void CClipingMusk::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	m_abUsingReferenceValue[m_dwReferenceValue] = false;
}

//============================================================
// �V�[�����2D�|���S���̍X�V����
// Author : �r�c�I��
//============================================================
void CClipingMusk::Update(void)
{

}

//=============================================================
// �V�[�����2D�|���S���̕`�揈��
// Author : �r�c�I��
//=============================================================
void CClipingMusk::Draw(void)
{
	// �����_���[����f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�A���t�@�e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//�A���t�@�e�X�g��l�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaTestBorder);
	//�A���t�@�e�X�g�̔�r���@�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Z�e�X�g����Ɏ��s����悤��
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	//�X�e���V���̐ݒ�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, m_dwReferenceValue + 1);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	//�A���t�@�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//�X�e���V���̖�����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	//Z�e�X�g�̔�r���@�����Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �e�N�X�`���̐ݒ�̉����@
	pDevice->SetTexture(0, NULL);

}

//=============================================================
// �V�[�����2D�|���S���̃e�N�X�`�������蓖��
// Author : �r�c�I��
//=============================================================
void CClipingMusk::BindTexture(const int nNumTexture)
{
	CTexture *pTexture = CManager::GetTexture();
	m_pTexture = pTexture->GetInfo(nNumTexture)->pTexture;
}

//=============================================================
// �V�[�����2D�|���S���̒��_���W��ݒ�
// Author : �r�c�I��
//=============================================================
void CClipingMusk::SetVertex(void)
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
// Author : �r�c�I��
//=============================================================
void CClipingMusk::SetRotVertex(float fAngle)
{
	// �ϐ��錾
	// �e���_
	D3DXVECTOR3 vertex1 = DEFAULT_VECTOR;
	D3DXVECTOR3 vertex2 = DEFAULT_VECTOR;
	D3DXVECTOR3 vertex3 = DEFAULT_VECTOR;
	D3DXVECTOR3 vertex4 = DEFAULT_VECTOR;

	//==========================================================================================================
	// �摜���A�摜�̒��S�����ɉ�]������
	// ����̒��_
	vertex1.x = -(m_size.x / 2)*cosf(fAngle)
		- (-(m_size.y / 2))*sinf(fAngle);
	vertex1.y = -(m_size.x / 2)*sinf(fAngle)
		+ (-(m_size.y / 2))*cosf(fAngle);

	// �E��̒��_
	vertex2.x = (m_size.x / 2)*cosf(fAngle)
		- (-(m_size.y / 2))*sinf(fAngle);
	vertex2.y = (m_size.x / 2)*sinf(fAngle)
		+ (-(m_size.y / 2))*cosf(fAngle);

	// �����̒��_
	vertex3.x = -(m_size.x / 2)*cosf(fAngle)
		- (m_size.y / 2)*sinf(fAngle);
	vertex3.y = -(m_size.x / 2)*sinf(fAngle)
		+ (m_size.y / 2)*cosf(fAngle);

	// �E���̒��_
	vertex4.x = (m_size.x / 2)*cosf(fAngle)
		- (m_size.y / 2)*sinf(fAngle);
	vertex4.y = (m_size.x / 2)*sinf(fAngle)
		+ (m_size.y / 2)*cosf(fAngle);
	//==========================================================================================================

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx = NULL;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
// Author : �r�c�I��
//=============================================================
void CClipingMusk::SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual)
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
// �N���G�C�g�֐�
// Author : �r�c�I��
//=============================================================
CClipingMusk* CClipingMusk::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumTexture)
{
	CClipingMusk* pClipingMusk = new CClipingMusk;
	pClipingMusk->m_pos = pos;
	pClipingMusk->m_size = size;
	pClipingMusk->BindTexture(nNumTexture);
	pClipingMusk->Init();

	return pClipingMusk;
}

//=============================================================
// �e�N�X�`�����g��Ȃ��ꍇ�̃N���G�C�g�֐�
// Author : �r�c�I��
//=============================================================
CClipingMusk* CClipingMusk::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CClipingMusk* pClipingMusk = new CClipingMusk;
	pClipingMusk->m_pos = pos;
	pClipingMusk->m_size = size;
	pClipingMusk->Init();

	return pClipingMusk;
}