//===============================================
//
// エフェクト3D処理 (effect3d.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "effect3d.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "game.h"
#include "library.h"
#include "sound.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CEffect3D::CEffect3D(CScene::OBJTYPE objtype) :CBillboard(objtype)
{
    m_nTexType = 0;

    m_move = DEFAULT_VECTOR;
    m_fGravity = 0.0f;
    m_fAcceleration = 1.0f;

    m_fRotAngle = 0.0f;
    m_fRotSpeed = 0.0f;

    m_col = DEFAULT_COLOR;
    m_colChangeRate = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
    m_fFadeOutRate = 0.0f;

    m_sizeChangeRate = DEFAULT_VECTOR;

    m_bUseAdditiveSynthesis = false;
    m_bUseZBuffer = true;

    m_nCntAnimTime = 0;
    m_bOneRoundAnim = false;
    m_nAnimPattern = 0;
    m_nAnimSpeed = 0;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ビルボードの初期化
    CBillboard::Init(pos, size);

    // テクスチャをバインド
    BindTexture(m_nTexType);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Update(void)
{
    // 位置と大きさを取得
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 size = GetSize();

    // 位置を更新（重力や加速度も考慮）
    pos += m_move;
    m_move.y -= m_fGravity;
    m_move *= m_fAcceleration;

    // 大きさを更新
    size += m_sizeChangeRate;

    // 色を更新
    m_col += m_colChangeRate;

    // アニメーションを使うなら（フェードアウト周りが変わる）
    if (m_nAnimPattern > 1)
    {
        // 一周していないならテクスチャアニメーションを更新
        if (!m_bOneRoundAnim)
        {
            // カウントアップ
            m_nCntAnimTime++;

            // テクスチャ座標を更新
            if (m_nCntAnimTime == m_nAnimSpeed)
            {
                m_bOneRoundAnim = SetUpdateTexLeftToRight(m_nAnimPattern);

                // カウンタリセット
                m_nCntAnimTime = 0;
            }
        }
        else
        {
            // 一周したらフェードアウト開始
            m_col.a -= m_fFadeOutRate;
        }
    }
    else
    {
        // アニメーションを使わないなら、すぐにフェードアウト開始
        m_col.a -= m_fFadeOutRate;
    }

    // 位置、大きさ、色を反映
    SetPos(pos);
    SetSize(size);
    SetCol(m_col);

    // ビルボードを更新（角度も反映）
    m_fRotAngle += m_fRotSpeed;
    CBillboard::Update(m_fRotAngle);

    // 透明、大きさがないなら終了処理
    if (m_col.a <= 0.0f || size.x < 0.0f || size.y < 0.0f)
    {
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Draw(void)
{
    // 加算合成
    if (m_bUseAdditiveSynthesis)
    {
        CBillboard::SetAdditiveSynthesis();
    }

    // Zバッファを無視するかどうか
    if (!m_bUseZBuffer)
    {
        CBillboard::SetZBufferNone();
    }

    CBillboard::Draw();
}

//=============================================================================
// エフェクト放出処理（一番最初に呼び出す）
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius)
{
    // エフェクトの生成情報を取得
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // 同時発生数分、生成する
    for (int nCnt = 0; nCnt < pCreateInfo->nNumOnce; nCnt++)
    {
        // 生成半径が0以上なら
        if (fEmitRadius > 0.0f)
        {
            // 角度を出す
            float fAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;

            // 放出時の半径をどうするか
            D3DXVECTOR3 effectPos = pos;
            float fFinalEmitRadius = fEmitRadius;   // 円周上
            if (!pCreateInfo->bOnCircleEdge)
            {
                // 円周内
                int nEmitRadiusMax = int(fEmitRadius * EFFECT_FLOATING_POINT);
                fFinalEmitRadius = float(GetRandNum(nEmitRadiusMax, 0)) / EFFECT_FLOATING_POINT;
            }

            // 平面によって位置と飛散角度を場合分け
            float fScatterAngle = 0.0f;
            if (pCreateInfo->plane == CEffectData::PLANE_XY)
            {
                effectPos.x += sinf(fAngle)*fFinalEmitRadius;
                effectPos.y += cosf(fAngle)*fFinalEmitRadius;

                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((effectPos.x - pos.x), (effectPos.y - pos.y));
                }
                else
                {
                    fScatterAngle = atan2((pos.x - effectPos.x), (pos.y - effectPos.y));
                }
            }
            else if (pCreateInfo->plane == CEffectData::PLANE_XZ)
            {
                effectPos.x += -sinf(fAngle)*fFinalEmitRadius;
                effectPos.z += -cosf(fAngle)*fFinalEmitRadius;

                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((effectPos.x - pos.x), (effectPos.z - pos.z));
                }
                else
                {
                    fScatterAngle = atan2((pos.x - effectPos.x), (pos.z - effectPos.z));
                }
            }

            // 生成
            Create(nType, effectPos, fScatterAngle);
        }
        else
        {
            // 平面によって飛散角度を場合分け
            float fScatterAngle = 0.0f;
            if (pCreateInfo->plane == CEffectData::PLANE_XY)
            {
                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((posOld.x - pos.x), (posOld.y - pos.y));
                }
                else
                {
                    fScatterAngle = atan2((pos.x - posOld.x), (pos.y - posOld.y));
                }
            }
            else if (pCreateInfo->plane == CEffectData::PLANE_XZ)
            {
                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((posOld.x - pos.x), (posOld.z - pos.z));
                }
                else
                {
                    fScatterAngle = atan2((pos.x - posOld.x), (pos.z - posOld.z));
                }
            }

            // 生成
            Create(nType, pos, fScatterAngle);
        }
    }
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CEffect3D * CEffect3D::Create(const int nType, D3DXVECTOR3 pos, const float fScatterAngle)
{
    // エフェクトの生成情報を取得
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // メモリの確保
    CEffect3D *pEffect3D = NULL;

    // タイプごとに、Zバッファを無効化するエフェクトかどうかを決める
    if (pCreateInfo->bUseZBuffer)
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D_Z_NONE);
    }
    else
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D);
    }

    // メモリを確保出来たら
    if (pEffect3D != NULL)
    {
        // 種類を結びつけておく
        pEffect3D->m_nTexType = pCreateInfo->nTexType;

        // 初期化（サイズはこのとき決める）
        float fSize = float(GetRandNum(pCreateInfo->nSizeMax, pCreateInfo->nSizeMin)) / EFFECT_FLOATING_POINT;
        pEffect3D->Init(pos, D3DXVECTOR3(fSize, fSize, 0.0f));

        // ビルボードへ設定を反映
        D3DXVECTOR3 rot = DEFAULT_VECTOR;
        if (pCreateInfo->bLayRot)
        {
            rot = LAY_ROT_TOP;
        }
        pEffect3D->SetRot(rot);
        pEffect3D->SetCol(pCreateInfo->col);
        pEffect3D->SetAlphaTestBorder(pCreateInfo->nAlphaTestBorder);

        // テクスチャ情報から、アニメーションの有無を取得
        CTexture *pTexture = CManager::GetTexture();
        pEffect3D->m_nAnimPattern = pTexture->GetInfo(pEffect3D->m_nTexType)->nPattern;
        pEffect3D->m_nAnimSpeed = pTexture->GetInfo(pEffect3D->m_nTexType)->nSpeed;
        if (pEffect3D->m_nAnimPattern > 1)
        {
            pEffect3D->SetTexUV(0.0f, 1.0f / pEffect3D->m_nAnimPattern, 0.0f, 1.0f);
        }

        // メンバ変数に各値を結びつけていく
        pEffect3D->m_fAcceleration = pCreateInfo->fAcceleration;
        pEffect3D->m_fGravity = pCreateInfo->fGravity;

        // 初期の回転角度は、最大回転速度が0より大きいなら変える
        float fRotAngle = 0.0f;
        if (pCreateInfo->nRotSpeedMax > 0)
        {
            fRotAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;
        }
        pEffect3D->m_fRotAngle = fRotAngle;
        pEffect3D->m_fRotSpeed = float(GetRandNum(pCreateInfo->nRotSpeedMax, pCreateInfo->nRotSpeedMin)) / EFFECT_FLOATING_POINT;

        pEffect3D->m_col = pCreateInfo->col;
        pEffect3D->m_colChangeRate = pCreateInfo->colChangeRate;
        pEffect3D->m_fFadeOutRate = pCreateInfo->fFadeOutRate;

        pEffect3D->m_sizeChangeRate = pCreateInfo->sizeChangeRate;

        pEffect3D->m_bUseAdditiveSynthesis = pCreateInfo->bUseAdditiveSynthesis;
        pEffect3D->m_bUseZBuffer = pCreateInfo->bUseZBuffer;

        // 移動量は最後に決定（移動の向きに応じて、初期の回転角度が変わる可能性があるため）
        pEffect3D->MoveCalculation(nType, fScatterAngle);
    }

    return pEffect3D;
}

//=============================================================================
// 移動量を算出する
// Author : 後藤慎之助
//=============================================================================
void CEffect3D::MoveCalculation(const int nType, const float fScatterAngle)
{
    // エフェクトの生成情報を取得
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // 速さを出す
    float fSpeed = float(GetRandNum(pCreateInfo->nSpeedMax, pCreateInfo->nSpeedMin)) / EFFECT_FLOATING_POINT;
    float fAuxiliaryAxisSpeed = float(GetRandNum(pCreateInfo->nAuxiliaryAxisSpeedMax, pCreateInfo->nAuxiliaryAxisSpeedMin)) / EFFECT_FLOATING_POINT;

    // 移動の角度を決める（飛散幅＋飛散角度）
    float fScatterWidth = 0;
    if (pCreateInfo->nScatterWidth > 0)
    {
        fScatterWidth = float(rand() % pCreateInfo->nScatterWidth) / EFFECT_FLOATING_POINT - float(rand() % pCreateInfo->nScatterWidth) / EFFECT_FLOATING_POINT;
    }
    float fMoveAngle = fScatterAngle + fScatterWidth;

    // 初期の回転を移動の向きに合わせるかどうか
    if (pCreateInfo->bRotEqualMoveAngle)
    {
        m_fRotAngle = fMoveAngle;
    }

    // 平面によって移動量の向きを場合分け
    if (pCreateInfo->plane == CEffectData::PLANE_XY)
    {
        m_move = D3DXVECTOR3(sinf(fMoveAngle) * fSpeed, cosf(fMoveAngle)* fSpeed, fAuxiliaryAxisSpeed);
    }
    else if (pCreateInfo->plane == CEffectData::PLANE_XZ)
    {
        m_move = D3DXVECTOR3(-sinf(fMoveAngle) * fSpeed, fAuxiliaryAxisSpeed, -cosf(fMoveAngle)* fSpeed);
    }
}