//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	camera.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
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

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#define VEC_FROM_PLAYER D3DXVECTOR3(0.0f, 40.0f, -200.0f)
#define MAX_ROTATE_SPEED (1.5f)
#define PITCH_LIMIT (75.0f)
#define ROTATE_REDUCE_RATE (0.85f)
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//静的メンバ変数宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
int CCamera::m_nCntCam = 0;
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CCamera::CCamera()
{
	m_nIndex = m_nCntCam++;
}
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CCamera::~CCamera()
{
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CCamera::Init(void)
{
    //カメラの位置・注意点・上方向を設定する
    m_posV = D3DXVECTOR3(0.0f, 100.0f, -500.0f);
    m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_addRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    //視点-注意点の距離の計算
    m_fDistance = D3DXVec3Length(&(m_posV - m_posR));
	
    return S_OK;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//終了処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CCamera::Uninit(void)
{
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//更新処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
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

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//カメラ生成処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CCamera::SetCamera(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();    //デバイスへのポインタ

    //ビューマトリックスの初期化
    D3DXMatrixIdentity(&m_mtxView);

    //ビューマトリックスの作成
    D3DXMatrixLookAtLH(&m_mtxView,&m_posV,&m_posR, &m_vecU);

    //ビューマトリックスの設定
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    //プロジェクションマトリックスの初期化
    D3DXMatrixIdentity(&m_mtxProjection);

    //プロジェクションマトリックスの作成
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(CAMERA_FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);

    //プロジェクションマトリックスの設定
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