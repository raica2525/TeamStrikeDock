//===============================================
//
// 3Dポリゴン処理 (scene3d.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "scene3d.h"
#include "manager.h"
#include "renderer.h"
#include "modelData.h"

//========================
// マクロ定義
//========================
#define EMISSIVE_COLOR D3DXCOLOR(0.25f, 0.25f, 0.25f, 0.25f)

//========================================
// 静的メンバ変数宣言
//========================================
bool CScene3D::m_bAdditiveSynthesis = false;

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CScene3D::CScene3D()
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
// オーバーライドされたコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CScene3D::CScene3D(OBJTYPE objType) :CScene(objType)
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
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    m_pos = pos;

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CScene3D::Uninit(void)
{
    Release();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CScene3D::Update(void)
{
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CScene3D::Draw(bool bChangeColor, D3DXCOLOR color)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;
    D3DXMATERIAL *pMat;

    // 加算合成
    if (m_bAdditiveSynthesis == true)
    {
        // レンダーステート(加算合成にする)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    D3DXMatrixIdentity(&m_mtxWorld);

    D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

    D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
    D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->GetMaterial(&matDef);

    ZeroMemory(&matDef, sizeof(D3DMATERIAL9));

    pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

    for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
    {
        // テクスチャを貼っているなら自己発光
        if (m_apTexMat[nCntMat])
        {
            pMat[nCntMat].MatD3D.Emissive = EMISSIVE_COLOR;
        }
        pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        // 色を変えるなら、拡散光を変える
        if (bChangeColor)
        {
            pMat[nCntMat].MatD3D.Diffuse = color;
        }

        pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

        // 変えた値を戻す
        pMat[nCntMat].MatD3D.Diffuse = m_aDiffuse[nCntMat];

        pDevice->SetTexture(0, m_apTexMat[nCntMat]);

        m_pMesh->DrawSubset(nCntMat);
    }

    pDevice->SetMaterial(&matDef);

    pDevice->SetTexture(0, NULL);

    // 加算合成を戻す
    if (m_bAdditiveSynthesis == true)
    {
        // レンダーステート(加算合成を戻す)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // 加算合成のフラグをfalseに戻す
        m_bAdditiveSynthesis = false;
    }
}

//=============================================================================
// モデルデータの割り当て
// Author : 後藤慎之助
//=============================================================================
void CScene3D::BindModelData(const int nNumModelData)
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