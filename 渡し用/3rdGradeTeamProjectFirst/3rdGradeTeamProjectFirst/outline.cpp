//===============================================
//
// アウトライン処理 (outline.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "outline.h"
#include "manager.h"
#include "renderer.h"

//========================================
// 静的メンバ変数宣言
//========================================

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
COutline::COutline(CScene::OBJTYPE objtype) :CBillboard(objtype)
{
    m_bUse = true;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
COutline::~COutline()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT COutline::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ビルボードの初期化
    CBillboard::Init(pos, size);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void COutline::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void COutline::Update(void)
{
    // ビルボードを更新
    CBillboard::Update();

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
void COutline::Draw(void)
{
    // ステンシルバッファを使う
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ステンシルテスト => 有効
    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

    // ステンシルテストと比較する参照値設定 => 0x01
    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

    // ステンシルテストの値に対してのマスク設定 => 0xff(全て真)
    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

    // ステンシルテストの比較方法設定 => 
    // この描画での参照値 == ステンシルバッファの参照値なら合格
    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

    // ステンシルテストの結果に対しての反映設定
    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

    // ビルボードの描画
    CBillboard::Draw();

    // ステンシルテスト無効
    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
COutline * COutline::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
    // メモリの確保
    COutline *pOutline = NULL;
    pOutline = new COutline(CScene::OBJTYPE_NONE_DRAW);

    // メモリを確保出来たら
    if (pOutline != NULL)
    {
        // 初期化
        pOutline->Init(pos, size);

        // ビルボードへ設定を反映
        pOutline->SetRot(rot);
        pOutline->SetCol(col);
    }

    return pOutline;
}