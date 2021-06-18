//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	player.cpp
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "player.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "camera.h"
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�}�N����`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

#define FLY_SPEED (3.0f)
#define MAX_ROTATE_SPEED (3.0f)
#define ADD_ROTATE_SPEED (1.0f)
#define MOVE_RATE (1.0f)
#define ROTATE_REDUCE_RATE (0.85f)
#define PITCH_LIMIT (75.0f)
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�ÓI�����o�ϐ��錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CModel::MODELDATA CPlayer::m_modeldata[PLAYER_MODEL_COUNT] = {};
CPlayer* CPlayer::m_apPlayer[MAX_PLAYER] = {};
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�R���X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
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

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�f�X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CPlayer::~CPlayer()
{

}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//����������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
HRESULT CPlayer::Init()
{
	//���f������
	for (int nCount = 0; nCount < 1/*(�ǂݍ��񂾃��f������)*/; nCount++)
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
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
	D3DXMATRIX mtxRot;	//��]�s��
	D3DXMATRIX mtxRotMove;	//��]�s��
	static float angle = 0;
	D3DXQUATERNION quaternion;
	switch (m_state)
	{
	case STATE_STAND:	//�����i�����j���̏����j
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
		
		if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))	//�X�y�[�X�Ŕ�s���[�h�֐؂�ւ�
		{
			m_state = STATE_FLY;
		}
		m_rot.y = CamRot.y;
		break;
	case STATE_FLY:	//��s���̏���
		
		//�p�x�ω��ʂ�ύX
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

		//�������|����
		m_addRot *= ROTATE_REDUCE_RATE;

		//�����ʐ���
		if (m_addRot.x > D3DXToRadian(MAX_ROTATE_SPEED))
		{
			m_addRot.x = D3DXToRadian(MAX_ROTATE_SPEED);
		}
		if (m_addRot.x < D3DXToRadian(-MAX_ROTATE_SPEED))
		{
			m_addRot.x = D3DXToRadian(-MAX_ROTATE_SPEED);
		}
		//�ω��ʂ𑫂�
		m_rot += m_addRot;
		//�㉺�̊p�x����
		if (D3DXToDegree(m_rot.x) < -PITCH_LIMIT)
		{
			m_rot.x = D3DXToRadian(-PITCH_LIMIT);
		}
		if (D3DXToDegree(m_rot.x) > PITCH_LIMIT)
		{
			m_rot.x = D3DXToRadian(PITCH_LIMIT);
		}

		m_rot.y = CamRot.y;
		//�ړ��ʂ��s��ϊ�
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

	for (int nCount = 0; nCount < 1/*(�ǂݍ��񂾃��f������)*/; nCount++)
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