//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
// number.cpp
// Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E



//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

#include "number.h"
#include "renderer.h"
#include "manager.h"

LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�R���X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CNumber::CNumber()
{
	m_nValue = 0;
	m_pos = D3DXVECTOR3(0, 0, 0);			// �|���S���̈ʒu
	m_size = D3DXVECTOR3(0, 0, 0);
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�f�X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CNumber::~CNumber()
{

}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//����������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
HRESULT CNumber::Init(void)
{
	VERTEX_2D*pVtx;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//�Œ�
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�Œ�
		&m_pVtxBuff,//�ϐ����ɂ���ĕω�
		NULL);

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x / 2, -m_size.y / 2, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x / 2, m_size.y / 2, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x / 2, m_size.y / 2, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nValue*0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_nValue +1)*0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nValue*0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_nValue +1)*0.1f, 1.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�(0�`255)
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);//a�͓����x
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);//a�͓����x
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);//a�͓����x
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);//a�͓����x
	m_pVtxBuff->Unlock();


	return S_OK;
}


//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�X�V����
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CNumber::Update(void)
{
	VERTEX_2D*pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_nValue*0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_nValue + 1)*0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nValue*0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_nValue + 1)*0.1f, 1.0f);
	m_pVtxBuff->Unlock();
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�`�揈��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,//�v���e�B�u�̎��
		0,
		NUM_POLYGON//�v���e�B�u�̐�
	);
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�I������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�e�N�X�`���ǂݍ���
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
HRESULT CNumber::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "Resources\\number000.png", &m_pTexture);
	return S_OK;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�e�N�X�`���j��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CNumber::Unload(void)
{
	if (m_pTexture != NULL) {
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�N���G�C�g�֐�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CNumber* CNumber::Create(int nValue, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CNumber* pNumber = NULL;
	pNumber = new CNumber;
	pNumber->SetValue(nValue);
	pNumber->SetPos(pos);
	pNumber->SetSize(size);
	pNumber->Init();
	
	return pNumber;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//���l�ݒ�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CNumber::SetValue(int nValue)
{
	m_nValue = nValue;
}

void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CNumber::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}