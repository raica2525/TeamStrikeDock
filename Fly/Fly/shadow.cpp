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
#include "shadow.h"
#include "model.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CShadow::CShadow()
{
	//モデルデータの初期化
	ZeroMemory(&m_ModelData, sizeof(m_ModelData));
}

//=============================================================================
// デストラクタ
//=============================================================================
CShadow::~CShadow()
{
}

//=============================================================================
// 3Dオブジェクトの初期化処理
//=============================================================================
HRESULT CShadow::Init(void)
{

	CScene3d::Init();
	return S_OK;
}

//=============================================================================
// 3Dオブジェクトの終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3d::Uninit();
	Release();
}

//=============================================================================
// 3Dオブジェクトの更新処理
//=============================================================================
void CShadow::Update(void)
{
	D3DXVECTOR3 pos = m_pParentModel->GetPosition();
	D3DXVECTOR3 rot = m_pParentModel->GetRotation();
	D3DXVECTOR3 size = m_pParentModel->GetScale();
	pos.y = 0;
	rot.x *= -1;
	SetPosition(pos);
	SetRotation(rot);
	SetScale(size);
}

//=============================================================================
// 3Dオブジェクトの描画処理
//=============================================================================
void CShadow::Draw(void)
{
	if (CManager::GetUseShadow())
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
		D3DMATERIAL9 matDef;
		D3DXMATERIAL*pMat;

		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

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
			if (m_ModelData.pTexture[nCntMat] != NULL)
			{
				pDevice->SetTexture(0, m_ModelData.pTexture[nCntMat]);
			}
			m_ModelData.pMesh->DrawSubset(nCntMat);
		}
		//マテリアル情報のリセット
		pDevice->SetMaterial(&matDef);

		// Zバッファ設定 => 有効
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		// ZBUFFER比較設定変更 => (参照値 <= バッファ値)
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}


}

//=============================================================================
// 3Dオブジェクトの生成
//=============================================================================
CShadow* CShadow::Create(CModel *pParentModel, CModel::MODELDATA modeldata, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	// メモリの確保
	CShadow* pShadow;
	pShadow = new CShadow;
	//各種情報の設定
	pShadow->SetPosition(pos);
	pShadow->SetRotation(rot);
	pShadow->SetScale(size);
	pShadow->m_pParentModel = pParentModel;
	pShadow->m_ModelData = modeldata;
	// 初期化処理
	pShadow->Init();

	return pShadow;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//モデルの割当
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CShadow::BindModel(CModel::MODELDATA modeldata)
{
	m_ModelData = modeldata;
}
