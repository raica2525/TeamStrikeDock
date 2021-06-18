//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	background.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "background.h"
#include "polygon3d.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"

LPDIRECT3DTEXTURE9 CBackground::m_pTexture = NULL;
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CBackground::CBackground()
{
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	CPolygon3d* m_pPolygon3d;
	float m_fDistanceFromCamera;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CBackground::~CBackground()
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CBackground::Init(void)
{
	CCamera* pCamera = CManager::GetCamera();
	D3DXVECTOR3 CamRot = pCamera->GetRot();
	D3DXVECTOR3 NormalizedCameraVec;
	m_pPolygon3d = CPolygon3d::Create(m_pos, D3DXVECTOR3(-CamRot.x + 90.0f, CamRot.y, CamRot.z), m_size);
	m_pPolygon3d->BindTexture(m_pTexture);
	return S_OK;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//終了処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CBackground::Uninit(void)
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//更新処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CBackground::Update(void)
{
	CCamera* pCamera = CManager::GetCamera();
	D3DXVECTOR3 CamRot = pCamera->GetRot();
	m_pPolygon3d->SetRotation(D3DXVECTOR3(-CamRot.x+90.0f, CamRot.y, CamRot.z));

	D3DXVECTOR3 NormalizedCameraVec;
	D3DXVec3Normalize(&NormalizedCameraVec, &(pCamera->GetPosR() - pCamera->GetPosV()));
	m_pPolygon3d->SetPosition(NormalizedCameraVec * m_fDistanceFromCamera + pCamera->GetPosV());
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//描画処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CBackground::Draw(void)
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クリエイト関数
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CBackground* CBackground::Create(float fDistanceFromCamera)
{
	CBackground* pBackground = NULL;
	pBackground = new CBackground;
	pBackground->m_size = D3DXVECTOR3((fDistanceFromCamera / tanf(CAMERA_FOV/2))* ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)/2, fDistanceFromCamera / tanf(CAMERA_FOV/2)/2, 0);
	CCamera* pCamera = CManager::GetCamera();
	D3DXVECTOR3 NormalizedCameraVec;
	pBackground->m_fDistanceFromCamera = fDistanceFromCamera;
	D3DXVec3Normalize(&NormalizedCameraVec, &(pCamera->GetPosR() - pCamera->GetPosV()));
	pBackground->m_pos = NormalizedCameraVec * fDistanceFromCamera + pCamera->GetPosV();
	pBackground->Init();
	return pBackground;
}

void CBackground::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "town.jpg", &m_pTexture);
}
