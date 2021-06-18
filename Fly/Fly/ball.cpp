//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	ball.cpp
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "ball.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"


//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�ÓI�����o�ϐ��錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CModel::MODELDATA CBall::m_modeldata = {};

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�R���X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CBall::CBall()
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_size = D3DXVECTOR3(1, 1, 1);
	SetObjType(CScene::OBJTYPE_BALL);
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�f�X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CBall::~CBall()
{

}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//����������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
HRESULT CBall::Init()
{
	//���f������
	m_pModel= CModel::Create(NULL, m_modeldata, m_pos, m_rot, m_size);
	return S_OK;
}

void CBall::Uninit()
{
	//���f���j��
}

void CBall::Update()
{
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = (CInputKeyboard*)CManager::GetInput();
	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		m_pos.x--;
	}
	if (pInputKeyboard->GetKeyPress(DIK_S))
	{
		m_pos.x++;
	}
	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		m_pos.z--;
	}
	if (pInputKeyboard->GetKeyPress(DIK_D))
	{
		m_pos.z++;
	}
	if (pInputKeyboard->GetKeyPress(DIK_SPACE))
	{
		m_pos.y++;
	}
	if (pInputKeyboard->GetKeyPress(DIK_LSHIFT))
	{
		m_pos.y--;
	}
	for (int nCount = 0; nCount < 1/*(�ǂݍ��񂾃��f������)*/; nCount++)
	{
		m_pModel->SetPosition(m_pos);
		m_pModel->SetRotation(m_rot);
	}

}

void CBall::Draw()
{

}

CBall* CBall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CBall* pBall = NULL;
	pBall = new CBall;
	pBall->m_pos = pos;
	pBall->m_rot = rot;
	pBall->m_size = size;
	pBall->Init();

	return pBall;

}

void CBall::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXLoadMeshFromX("ball.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_modeldata.pBuffMat, NULL, &m_modeldata.nNumMat, &m_modeldata.pMesh);
		//for (int nCountMat = 0; nCountMat < (int)m_modeldata[nCount].nNumMat; nCountMat++)
		//{
		//D3DXCreateTextureFromFile(pDevice, "teemo.jpg", &m_modeldata[nCount].pTexture[nCountMat]);
		//}
}

void CBall::Unload()
{

	if (m_modeldata.pBuffMat != NULL)
	{
		m_modeldata.pBuffMat->Release();
	}

	if (m_modeldata.pMesh != NULL)
	{
		m_modeldata.pMesh->Release();
	}

	for (int nCountTex = 0; nCountTex < MAX_MATERIAL; nCountTex++)
	{
		if (m_modeldata.pTexture[nCountTex] != NULL)
		{
			m_modeldata.pTexture[nCountTex]->Release();
		}
	}
}

D3DXVECTOR3 CBall::GetPos(void)
{
	return m_pos;
}