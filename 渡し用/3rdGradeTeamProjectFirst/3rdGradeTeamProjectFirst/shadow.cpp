//===============================================
//
// エフェクト3D処理 (shadow.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//========================================
// 静的メンバ変数宣言
//========================================

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CShadow::CShadow(CScene::OBJTYPE objtype) :CBillboard(objtype)
{
    m_fRotAngle = 0.0f;
    m_bUse = true;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ビルボードの初期化
    CBillboard::Init(pos, size);

    //// テクスチャをバインド
    //BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CShadow::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CShadow::Update(void)
{
    // ビルボードを更新
    CBillboard::Update(m_fRotAngle);

    // 使用フラグがfalseなら、消す
    if (!m_bUse)
    {
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CShadow::Draw(void)
{
    // ステンシルバッファを使う
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //// Zバッファ設定 => 有効
    //pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    //// ZBUFFER比較設定変更 => (参照値 <= バッファ値)
    //pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    // ステンシルテスト => 有効
    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

    // ステンシルテストと比較する参照値設定 => 0x01
    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

    // ステンシルテストの値に対してのマスク設定 => 0xff(全て真)
    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

    // ステンシルテストの比較方法設定 => 
    //		この描画での参照値 == ステンシルバッファの参照値なら合格
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

    // ステンシルテストの結果に対しての反映設定
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

    CBillboard::Draw();

    // ステンシルテスト無効
    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
    // メモリの確保
    CShadow *pShadow = NULL;

    // タイプごとに、Zバッファを無効化するエフェクトかどうかを決める
    pShadow = new CShadow(CScene::OBJTYPE_EFFECT3D_STENCIL);

    // メモリを確保出来たら
    if (pShadow != NULL)
    {
        // 初期化
        pShadow->Init(pos, size);

        // ビルボードへ設定を反映
        pShadow->SetRot(rot);
        pShadow->SetCol(col);
    }

    return pShadow;
}