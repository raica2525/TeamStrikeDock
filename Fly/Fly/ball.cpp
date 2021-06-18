//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	ball.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "ball.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"


//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//静的メンバ変数宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CModel::MODELDATA CBall::m_modeldata = {};

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CBall::CBall()
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_size = D3DXVECTOR3(1, 1, 1);
	SetObjType(CScene::OBJTYPE_BALL);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CBall::~CBall()
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CBall::Init()
{
	//モデル生成
	m_pModel= CModel::Create(NULL, m_modeldata, m_pos, m_rot, m_size);
	return S_OK;
}

void CBall::Uninit()
{
	//モデル破棄
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
	for (int nCount = 0; nCount < 1/*(読み込んだモデル数分)*/; nCount++)
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