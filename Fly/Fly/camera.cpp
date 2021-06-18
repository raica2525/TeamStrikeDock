//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	camera.cpp
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
#include "main.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "inputkeyboard.h"
#include "debug.h"
#include <stdio.h>

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�}�N����`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#define VEC_FROM_PLAYER D3DXVECTOR3(0.0f, 40.0f, -200.0f)
#define MAX_ROTATE_SPEED (1.5f)
#define PITCH_LIMIT (75.0f)
#define ROTATE_REDUCE_RATE (0.85f)
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�ÓI�����o�ϐ��錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
int CCamera::m_nCntCam = 0;
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�R���X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CCamera::CCamera()
{
	m_nIndex = m_nCntCam++;
}
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�f�X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CCamera::~CCamera()
{
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//����������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
HRESULT CCamera::Init(void)
{
    //�J�����̈ʒu�E���ӓ_�E�������ݒ肷��
    m_posV = D3DXVECTOR3(0.0f, 100.0f, -500.0f);
    m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_addRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    //���_-���ӓ_�̋����̌v�Z
    m_fDistance = D3DXVec3Length(&(m_posV - m_posR));
	
    return S_OK;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�I������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CCamera::Uninit(void)
{
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�X�V����
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CCamera::Update(void)
{
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = (CInputKeyboard*)CManager::GetInput();
	CPlayer* pPlayer = CPlayer::GetPlayer(0);
	D3DXVECTOR3 vecFromPlayer = VEC_FROM_PLAYER;
	D3DXMATRIX mtxRot;
	ZeroMemory(&mtxRot, sizeof(D3DXMATRIX));
	if (pInputKeyboard->GetKeyPress(DIK_Q))
	{
		m_addRot.y = -D3DXToRadian(MAX_ROTATE_SPEED);
	}
	if (pInputKeyboard->GetKeyPress(DIK_E))
	{
		m_addRot.y = D3DXToRadian(MAX_ROTATE_SPEED);
	}

	m_addRot *= ROTATE_REDUCE_RATE;

	m_rot += m_addRot;
	m_rot.x = pPlayer->GetRot().x;
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXVec3TransformNormal(&vecFromPlayer, &vecFromPlayer, &mtxRot);
	m_posV = pPlayer->GetPos() + vecFromPlayer;
	m_posR = pPlayer->GetPos();
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�J������������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CCamera::SetCamera(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();    //�f�o�C�X�ւ̃|�C���^

    //�r���[�}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&m_mtxView);

    //�r���[�}�g���b�N�X�̍쐬
    D3DXMatrixLookAtLH(&m_mtxView,&m_posV,&m_posR, &m_vecU);

    //�r���[�}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    //�v���W�F�N�V�����}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&m_mtxProjection);

    //�v���W�F�N�V�����}�g���b�N�X�̍쐬
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(CAMERA_FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);

    //�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}
D3DXVECTOR3 CCamera::GetPosR()
{
	return m_posR;
}

D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}