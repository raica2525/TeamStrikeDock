//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	player.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "player.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "camera.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#define FLY_SPEED (3.0f)
#define MAX_ROTATE_SPEED (3.0f)
#define ADD_ROTATE_SPEED (1.0f)
#define MOVE_RATE (1.0f)
#define ROTATE_REDUCE_RATE (0.85f)
#define PITCH_LIMIT (75.0f)
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//静的メンバ変数宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CModel::MODELDATA CPlayer::m_modeldata[PLAYER_MODEL_COUNT] = {};
CPlayer* CPlayer::m_apPlayer[MAX_PLAYER] = {};
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CPlayer::CPlayer()
{
	m_nLife = START_LIFE;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_addRot = D3DXVECTOR3(0, 0, 0);
	m_size = D3DXVECTOR3(1, 1, 1);
	m_state = STATE_STAND;
	SetObjType(CScene::OBJTYPE_PLAYER);
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apPlayer[nCount] == NULL)
		{
			m_nIndex = nCount;
			m_apPlayer[m_nIndex] = this;
			break;
		}
	}
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CPlayer::~CPlayer()
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CPlayer::Init()
{
	//モデル生成
	for (int nCount = 0; nCount < 1/*(読み込んだモデル数分)*/; nCount++)
	{
		m_apModel[nCount] = CModel::Create(NULL, m_modeldata[nCount], m_pos, m_rot, m_size);
	}
	
	return S_OK;
}

void CPlayer::Uninit()
{
	
}

void CPlayer::Update()
{
	D3DXVECTOR3 CamRot = CManager::GetCamera(m_nIndex)->GetRot();
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = (CInputKeyboard*)CManager::GetInput();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
	D3DXMATRIX mtxRot;	//回転行列
	D3DXMATRIX mtxRotMove;	//回転行列
	static float angle = 0;
	D3DXQUATERNION quaternion;
	switch (m_state)
	{
	case STATE_STAND:	//立ち（歩き）時の処理）
		if (pInputKeyboard->GetKeyPress(DIK_W))
		{
			move.z = MOVE_RATE;
		}
		if (pInputKeyboard->GetKeyPress(DIK_S))
		{
			move.z = -MOVE_RATE;
		}
		if (pInputKeyboard->GetKeyPress(DIK_A))
		{
			move.x = -MOVE_RATE;
		}
		if (pInputKeyboard->GetKeyPress(DIK_D))
		{
			move.x = MOVE_RATE;
		}
		
		if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))	//スペースで飛行モードへ切り替え
		{
			m_state = STATE_FLY;
		}
		m_rot.y = CamRot.y;
		break;
	case STATE_FLY:	//飛行時の処理
		
		//角度変化量を変更
		if (pInputKeyboard->GetKeyPress(DIK_W))
		{
			m_addRot.x = D3DXToRadian(ADD_ROTATE_SPEED);
		}
		if (pInputKeyboard->GetKeyPress(DIK_S))
		{
			m_addRot.x = -D3DXToRadian(ADD_ROTATE_SPEED);
		}
		if (pInputKeyboard->GetKeyPress(DIK_A))
		{
			move.x -= FLY_SPEED;
		}
		if (pInputKeyboard->GetKeyPress(DIK_D))
		{
			move.x += FLY_SPEED;
		}

		//慣性を掛ける
		m_addRot *= ROTATE_REDUCE_RATE;

		//増加量制限
		if (m_addRot.x > D3DXToRadian(MAX_ROTATE_SPEED))
		{
			m_addRot.x = D3DXToRadian(MAX_ROTATE_SPEED);
		}
		if (m_addRot.x < D3DXToRadian(-MAX_ROTATE_SPEED))
		{
			m_addRot.x = D3DXToRadian(-MAX_ROTATE_SPEED);
		}
		//変化量を足す
		m_rot += m_addRot;
		//上下の角度制限
		if (D3DXToDegree(m_rot.x) < -PITCH_LIMIT)
		{
			m_rot.x = D3DXToRadian(-PITCH_LIMIT);
		}
		if (D3DXToDegree(m_rot.x) > PITCH_LIMIT)
		{
			m_rot.x = D3DXToRadian(PITCH_LIMIT);
		}

		m_rot.y = CamRot.y;
		//移動量を行列変換
		move += D3DXVECTOR3(0.0f, 0.0f, FLY_SPEED);


		if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
		{
			m_state = STATE_STAND;
		}
		break;

	}

	D3DXMatrixRotationYawPitchRoll(&mtxRotMove, m_rot.y, m_rot.x, m_rot.z);
	D3DXVec3TransformNormal(&move, &move, &mtxRotMove);
	m_pos += move;

	for (int nCount = 0; nCount < 1/*(読み込んだモデル数分)*/; nCount++)
	{
		D3DXVECTOR3 addRot = D3DXVECTOR3(0, D3DXToRadian(5), 0);
		m_rot += addRot;
		m_apModel[nCount]->SetPosition(m_pos);
		m_apModel[nCount]->SetRotation(m_rot);
	}

}

void CPlayer::Draw()
{

}

CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CPlayer* pPlayer = NULL;
	pPlayer = new CPlayer;
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->m_size = size;
	pPlayer->Init();

	return pPlayer;

}

void CPlayer::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	for (int nCount = 0; nCount < 1; nCount++)
	{
		D3DXLoadMeshFromX("Data\\Models\\pickaxe.x", D3DXMESH_SYSTEMMEM,pDevice , NULL, &m_modeldata[nCount].pBuffMat, NULL, &m_modeldata[nCount].nNumMat, &m_modeldata[nCount].pMesh);
		if (m_modeldata[nCount].pMesh == NULL)
		{

		}
		//for (int nCountMat = 0; nCountMat < (int)m_modeldata[nCount].nNumMat; nCountMat++)
		//{
		//	D3DXCreateTextureFromFile(pDevice, "teemo.jpg", &m_modeldata[nCount].pTexture[nCountMat]);
		//}
	}
}

void CPlayer::Unload()
{
	for (int nCount = 0; nCount < PLAYER_MODEL_COUNT; nCount++)
	{
		if (m_modeldata[nCount].pBuffMat != NULL)
		{
			m_modeldata[nCount].pBuffMat->Release();
		}
		
		if (m_modeldata[nCount].pMesh != NULL)
		{
			m_modeldata[nCount].pMesh->Release();
		}
		
		for (int nCountTex = 0; nCountTex < MAX_MATERIAL; nCountTex++)
		{
			if (m_modeldata[nCount].pTexture[nCountTex] != NULL)
			{
				m_modeldata[nCount].pTexture[nCountTex]->Release();
			}
		}
		
	}
}

D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}
CPlayer* CPlayer::GetPlayer(int nIndex)
{
	return m_apPlayer[nIndex];
}