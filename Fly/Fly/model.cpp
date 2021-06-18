//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	model.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "model.h"
#include "shadow.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CModel::CModel()
{
	//モデルデータの初期化
	ZeroMemory(&m_ModelData, sizeof(m_ModelData));	
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
// 3Dオブジェクトの初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{

	CScene3d::Init();
	return S_OK;
}

//=============================================================================
// 3Dオブジェクトの終了処理
//=============================================================================
void CModel::Uninit(void)
{
	CScene3d::Uninit();
	Release();
}

//=============================================================================
// 3Dオブジェクトの更新処理
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// 3Dオブジェクトの描画処理
//=============================================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL*pMat;
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//ワールド変換行列の初期化
	D3DXMatrixIdentity(&mtxWorld);
	//回転行列の生成と計算
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRotation().y, GetRotation().x, GetRotation().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//拡大縮小行列の生成と計算
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	//平行移動行列の生成と計算
	D3DXMatrixTranslation(&mtxTrans, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);


	//計算結果を適用
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//マテリアル情報の取得
	pDevice->GetMaterial(&matDef);
	//モデルデータ読み込み失敗時の処理
	if (m_ModelData.pBuffMat == NULL)
	{
		MessageBox(FindWindow(CLASS_NAME, NULL), "モデルデータ読み込み失敗", "警告", MB_ICONWARNING);
		exit(EXIT_FAILURE);
	}
	pMat = (D3DXMATERIAL*)m_ModelData.pBuffMat->GetBufferPointer();

	//マテリアル情報の設定とマテリアルごとに描画
	for (int nCntMat = 0; nCntMat < (int)m_ModelData.nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		pDevice->SetTexture(0, m_ModelData.pTexture[nCntMat]);
		m_ModelData.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetTexture(0, NULL);
	//マテリアル情報のリセット
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// 3Dオブジェクトの生成
//=============================================================================
CModel* CModel::Create(CModel* pParentModel,MODELDATA modeldata,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{

	// メモリの確保
	CModel* pModel;
	pModel = new CModel;
	//各種情報の設定
	pModel->SetPosition(pos);
	pModel->SetRotation(rot);
	pModel->SetScale(size);
	pModel->m_pParentModel = pParentModel;
	pModel->m_ModelData = modeldata;

	CShadow::Create(pModel, modeldata, pos, rot, size);
	// 初期化処理
	pModel->Init();

	return pModel;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//モデルの割当
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CModel::BindModel(MODELDATA modeldata)
{
	m_ModelData = modeldata;
}
