//===============================================
//
// 背景の処理 (bg.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "bg.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CBg::CBg() :CScene3D(CScene::OBJTYPE_BG)
{
    m_nModelType = 0;
    m_col = DEFAULT_COLOR;
    m_colPhase = COLOR_PHASE_G_UP;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene3D::Init(pos, size);

    // スケールを設定
    SetScale(size);

    // モデルデータをバインド
    BindModelData(m_nModelType);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CBg::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CBg::Update(void)
{
    // ステージの線なら、色を変える
    if (m_nModelType == 83)
    {
        const float COLOR_CHANGE_RATE = 0.01f;
        switch (m_colPhase)
        {
        case COLOR_PHASE_R_DOWN:
            m_col.r -= COLOR_CHANGE_RATE;
            if (m_col.r <= 0.0f)
            {
                m_col.r = 0.0f;
                m_colPhase = COLOR_PHASE_B_UP;
            }
            break;
        case COLOR_PHASE_G_DOWN:
            m_col.g -= COLOR_CHANGE_RATE;
            if (m_col.g <= 0.0f)
            {
                m_col.g = 0.0f;
                m_colPhase = COLOR_PHASE_R_UP;
            }
            break;
        case COLOR_PHASE_B_DOWN:
            m_col.b -= COLOR_CHANGE_RATE;
            if (m_col.b <= 0.0f)
            {
                m_col.b = 0.0f;
                m_colPhase = COLOR_PHASE_G_UP;
            }
            break;
        case COLOR_PHASE_R_UP:
            m_col.r += COLOR_CHANGE_RATE;
            if (m_col.r >= 1.0f)
            {
                m_col.r = 1.0f;
                m_colPhase = COLOR_PHASE_B_DOWN;
            }
            break;
        case COLOR_PHASE_G_UP:
            m_col.g += COLOR_CHANGE_RATE;
            if (m_col.g >= 1.0f)
            {
                m_col.g = 1.0f;
                m_colPhase = COLOR_PHASE_R_DOWN;
            }
            break;
        case COLOR_PHASE_B_UP:
            m_col.b += COLOR_CHANGE_RATE;
            if (m_col.b >= 1.0f)
            {
                m_col.b = 1.0f;
                m_colPhase = COLOR_PHASE_G_DOWN;
            }
            break;
        }
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CBg::Draw(void)
{
    // ステージの線なら、色を反映させる
    if (m_nModelType == 83)
    {
        CScene3D::Draw(true, true, m_col);
    }
    else
    {
        CScene3D::Draw();
    }
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CBg * CBg::Create(int nModelType, D3DXVECTOR3 pos)
{
    // ポインタを用意
    CBg *pBg = NULL;

    // メモリ確保
    pBg = new CBg;

    // 先に種類を結びつけておく
    pBg->m_nModelType = nModelType;

    // ステージの線の場合、最初の色を変える
    if (nModelType == 83)
    {
        pBg->m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
    }

    // 初期化
    pBg->Init(pos, DEFAULT_SCALE);

    return pBg;
}