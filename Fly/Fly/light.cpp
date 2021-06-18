//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	light.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

//インクルードファイル
#include "main.h"
#include "light.h"
#include "renderer.h"
#include "manager.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CLight::CLight()
{
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CLight::~CLight()
{
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスへのポインタ

	D3DXVECTOR3 vecDir;//ライトの方向ベクトル
					   //ライトをクリアする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	//ライトの種類を設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	//ライトの拡散光を設定
	m_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(1.0f, -2.0f, 1.0f);
	D3DXVec3Normalize(&vecDir, &vecDir);//正規化する（大きさ1のベクトルにする必要がある）
	m_light.Direction = vecDir;
	//ライトを設定する
	pDevice->SetLight(0, &m_light);
	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//終了処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CLight::Uninit(void)
{
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//更新処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CLight::Update(void)
{
}
