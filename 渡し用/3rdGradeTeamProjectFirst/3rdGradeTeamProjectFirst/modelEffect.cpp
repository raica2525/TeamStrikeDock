//===============================================
//
// モデルエフェクトの処理 (modelEffect.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "modelEffect.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CModelEffect::CModelEffect() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nModelType = 0;
    m_col = SCENE3D_EMISSIVE_COLOR;
    m_colChangeRate = DEFAULT_COLOR_NONE;
    m_bUseLight = true;

    m_bUseAdditiveSynthesis = false;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CModelEffect::~CModelEffect()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CModelEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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
void CModelEffect::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::Update(void)
{
    m_col += m_colChangeRate;

    if (m_col.a < 0.0f)
    {
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CModelEffect::Draw(void)
{
    if (m_bUseAdditiveSynthesis)
    {
        CScene3D::SetAdditiveSynthesis();
    }

    CScene3D::Draw(true, m_bUseLight, m_col);
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CModelEffect * CModelEffect::Create(int nModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, D3DXCOLOR colChangeRate, bool bUseLight)
{
    // ポインタを用意
    CModelEffect *pModelEffect = NULL;

    // メモリ確保
    pModelEffect = new CModelEffect;

    // 先に結びつけておく
    pModelEffect->m_nModelType = nModelType;
    pModelEffect->SetRot(rot);

    // 初期化
    pModelEffect->Init(pos, DEFAULT_SCALE);

    // 引数を結びつけておく
    pModelEffect->m_col = col;
    pModelEffect->m_colChangeRate = colChangeRate;
    pModelEffect->m_bUseLight = bUseLight;

    return pModelEffect;
}