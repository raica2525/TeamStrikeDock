//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	scene3d.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene3d.h"
#include "renderer.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "inputmouse.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デフォルトコンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CScene3d::CScene3d()
{

}


//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CScene3d::~CScene3d()
{
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CScene3d::Init()
{
	return S_OK;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//終了処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CScene3d::Uninit(void)
{
	CScene::Release();
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//セッター関数
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CScene3d::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CScene3d::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

void CScene3d::SetScale(D3DXVECTOR3 scale)
{
	m_scale = scale;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//ゲッター関数
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
D3DXVECTOR3 CScene3d::GetPosition(void)
{
	return m_pos;
}

D3DXVECTOR3 CScene3d::GetRotation(void)
{
	return m_rot;
}

D3DXVECTOR3 CScene3d::GetScale(void)
{
	return m_scale;
}

