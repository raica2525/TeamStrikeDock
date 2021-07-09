//===============================================
//
// 3Dモデル処理 (scene3d.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "modelData.h"
#include "texture.h"

//========================
// マクロ定義
//========================
#define EMISSIVE_COLOR D3DXCOLOR(0.55f, 0.55f, 0.55f, 0.55f)

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CModel::CModel()
{
    memset(m_apTexMat, 0, sizeof(m_apTexMat));
    m_pBuffMat = NULL;
    m_pMesh = NULL;
    m_nNumMat = 0;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_scale = DEFAULT_SCALE;

    memset(m_aDiffuse, 0, sizeof(m_aDiffuse));
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CModel::Uninit(void)
{

}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CModel::Draw(CModel *model, bool bDeath)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;
    D3DXMATERIAL *pMat;

    // マトリックス計算
    D3DXMatrixIdentity(&m_mtxWorld);

    D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

    // 親とのマトリックス計算
    if (model != NULL)
    {
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &model->m_mtxWorld);
    }

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->GetMaterial(&matDef);

    ZeroMemory(&matDef, sizeof(D3DMATERIAL9));

    pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

    for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
    {
        if (!bDeath)
        {
            // テクスチャを貼っているなら自己発光
            if (m_apTexMat[nCntMat])
            {
                pMat[nCntMat].MatD3D.Emissive = EMISSIVE_COLOR;
            }
            pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

            pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

            pDevice->SetTexture(0, m_apTexMat[nCntMat]);
        }
        else
        {
            // テクスチャを貼っているなら自己発光をやめる
            if (m_apTexMat[nCntMat])
            {
                pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            }
            pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

            pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
        }

        // 変えた値を戻す
        pMat[nCntMat].MatD3D.Diffuse = m_aDiffuse[nCntMat];

        m_pMesh->DrawSubset(nCntMat);
    }

    pDevice->SetMaterial(&matDef);

    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CModel::Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bDeath)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxParent, mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;
    D3DXMATERIAL *pMat;

    // マトリックス計算
    D3DXMatrixIdentity(&m_mtxWorld);

    D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

    // 親とのマトリックス計算
    D3DXMatrixIdentity(&mtxParent);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
    D3DXMatrixMultiply(&mtxParent, &mtxParent, &mtxRot);

    D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
    D3DXMatrixMultiply(&mtxParent, &mtxParent, &mtxTrans);

    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->GetMaterial(&matDef);

    ZeroMemory(&matDef, sizeof(D3DMATERIAL9));

    pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

    for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
    {
        if (!bDeath)
        {
            // テクスチャを貼っているなら自己発光
            if (m_apTexMat[nCntMat])
            {
                pMat[nCntMat].MatD3D.Emissive = EMISSIVE_COLOR;
            }
            pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

            pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

            pDevice->SetTexture(0, m_apTexMat[nCntMat]);
        }
        else
        {
            // テクスチャを貼っているなら自己発光をやめる
            if (m_apTexMat[nCntMat])
            {
                pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            }
            pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

            pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
        }

        // 変えた値を戻す
        pMat[nCntMat].MatD3D.Diffuse = m_aDiffuse[nCntMat];

        m_pMesh->DrawSubset(nCntMat);
    }

    pDevice->SetMaterial(&matDef);

    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// モデルデータの割り当て
// Author : 後藤慎之助
//=============================================================================
void CModel::BindModelData(const int nNumModelData)
{
    // メッシュやマテリアルを受け取る
    CModelData *pModelData = CManager::GetModelData();
    m_pMesh = pModelData->GetModelData(nNumModelData)->pMesh;
    m_pBuffMat = pModelData->GetModelData(nNumModelData)->pBuffMat;
    m_nNumMat = pModelData->GetModelData(nNumModelData)->nNumMat;

    // 各マテリアルのテクスチャ情報を受け取る
    D3DXMATERIAL *pMat;
    pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
    for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
    {
        m_apTexMat[nCntMat] = pModelData->GetModelData(nNumModelData)->apTexMat[nCntMat];
        m_aDiffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
    }
}