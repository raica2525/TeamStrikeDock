//===============================================
//
// エフェクト2D処理 (effect2d.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "effect2d.h"
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
CEffect2D::CEffect2D(CScene::OBJTYPE objtype) :CScene2D(objtype)
{
    m_nTexType = 0;

    m_move = DEFAULT_VECTOR;
    m_fAcceleration = 1.0f;
    m_fGravity = 0.0f;
    m_fGravityLimit = 0.0f;

    m_fRotAngle = 0.0f;
    m_fRotSpeed = 0.0f;

    m_col = DEFAULT_COLOR;
    m_colChangeRate = DEFAULT_COLOR_NONE;
    m_fFadeOutRate = 0.0f;
    m_nCntFadeOutLock = 0;

    m_sizeChangeRate = DEFAULT_VECTOR;
    m_nCntSizeChangeLock = 0;
    m_bSizeChangeStartOrFinish = false;

    m_bUseAdditiveSynthesis = false;

    m_bOneRoundAnim = false;
    m_nAnimPattern = 0;
    m_nAnimSpeed = 0;
    m_bRepeat = false;

    m_bUseUpdate = true;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CEffect2D::~CEffect2D()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CEffect2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャをバインド
    BindTexture(m_nTexType);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CEffect2D::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CEffect2D::Update(void)
{
    // 更新処理をするなら
    if (m_bUseUpdate)
    {
        // 位置と大きさを取得
        D3DXVECTOR3 pos = GetPosition();
        D3DXVECTOR3 size = GetSize();

        // 位置を更新（重力や加速度も考慮）
        pos += m_move;
        m_move *= m_fAcceleration;
        m_move.y += m_fGravity; // 注意（3Dと逆）
        if (m_fGravityLimit > 0.0f)
        {
            // 重力制限（3Dと逆）
            if (m_move.y > m_fGravityLimit)
            {
                m_move.y = m_fGravityLimit;
            }
        }

        // 大きさをロックするカウンタ
        if (m_nCntSizeChangeLock > 0)
        {
            m_nCntSizeChangeLock--;
        }

        // 大きさを、開始と終了に場合分けしながら更新
        if (!m_bSizeChangeStartOrFinish)
        {
            // 開始かつ、カウンタがないなら大きさ更新
            if (m_nCntSizeChangeLock <= 0)
            {
                size += m_sizeChangeRate;
            }
        }
        else
        {
            // 終了なら、カウンタがなくなるまで大きさ更新
            if (m_nCntSizeChangeLock > 0)
            {
                size += m_sizeChangeRate;
            }
        }

        // 色を更新
        m_col += m_colChangeRate;

        // α値をロックするカウンタ
        if (m_nCntFadeOutLock > 0)
        {
            m_nCntFadeOutLock--;
        }

        // アニメーションを使うなら（フェードアウト周りが変わる）
        if (m_nAnimPattern > 1)
        {
            // 一周していないならテクスチャアニメーションを更新
            if (!m_bOneRoundAnim)
            {
                // リピートするなら、一周のフラグと結びつけない
                if (m_bRepeat)
                {
                    SetAnimation(m_nAnimSpeed, m_nAnimPattern);
                }
                else
                {
                    m_bOneRoundAnim = SetAnimation(m_nAnimSpeed, m_nAnimPattern);
                }
            }
            else
            {
                // α値のロックが解かれたら
                if (m_nCntFadeOutLock <= 0)
                {
                    // 一周したらフェードアウト開始
                    m_col.a -= m_fFadeOutRate;
                }
            }
        }
        else
        {
            // α値のロックが解かれたら
            if (m_nCntFadeOutLock <= 0)
            {
                // アニメーションを使わないならフェードアウト開始
                m_col.a -= m_fFadeOutRate;
            }
        }

        // 大きさの制限（effect3dから持ってきた際に、反転合成用で追加）
        if (size.x > SCREEN_WIDTH * 5)
        {
            size.x = SCREEN_WIDTH * 5;
        }

        if (size.y > SCREEN_HEIGHT * 5)
        {
            size.y = SCREEN_HEIGHT * 5;
        }

        // 位置、大きさ、色を反映
        CScene2D::SetPosition(pos);
        CScene2D::SetSize(size);
        CScene2D::SetColor(m_col);

        // 各頂点を更新（角度も反映）
        m_fRotAngle += m_fRotSpeed;
        if (m_fRotAngle > D3DXToRadian(180.0f) || m_fRotAngle < D3DXToRadian(-180.0f))
        {
            m_fRotAngle *= -1;
        }
        CScene2D::SetRotVertex(m_fRotAngle);

        // 透明、大きさがないなら終了処理
        if (m_col.a <= 0.0f || size.x < 0.0f || size.y < 0.0f)
        {
            Uninit();
        }
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CEffect2D::Draw(void)
{
    // 加算合成
    if (m_bUseAdditiveSynthesis)
    {
        CScene2D::SetAdditiveSynthesis();
    }

    // 反転合成
    if (m_nTexType == 20)
    {
        SetNega();
    }

    CScene2D::Draw();
}

//=============================================================================
// エフェクト放出処理（一番最初に呼び出す）
// Author : 後藤慎之助
//=============================================================================
CEffect2D* CEffect2D::Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius)
{
    // 生成するエフェクトの情報
    CEffect2D *pEffect2D = NULL;

    // エフェクトの生成情報を取得
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // 放出半径が指定されているエフェクトなら、塗り替える
    if (pCreateInfo->fEmitRadius > 0)
    {
        fEmitRadius = pCreateInfo->fEmitRadius;
    }

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

                // XZとは逆
                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((pos.x - effectPos.x), (pos.y - effectPos.y));
                }
                else
                {
                    fScatterAngle = atan2((effectPos.x - pos.x), (effectPos.y - pos.y));
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
            if (!pEffect2D)
            {
                pEffect2D = Create(nType, effectPos, fScatterAngle);
            }
            else
            {
                Create(nType, effectPos, fScatterAngle);
            }
        }
        else
        {
            // 平面によって飛散角度を場合分け
            float fScatterAngle = 0.0f;
            if (pCreateInfo->plane == CEffectData::PLANE_XY)
            {
                // XZとは逆
                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((pos.x - posOld.x), (pos.y - posOld.y));
                }
                else
                {
                    fScatterAngle = atan2((posOld.x - pos.x), (posOld.y - pos.y));
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
            if (!pEffect2D)
            {
                pEffect2D = Create(nType, pos, fScatterAngle);
            }
            else
            {
                Create(nType, pos, fScatterAngle);
            }
        }
    }

    // 最初に生成した一つを返す
    return pEffect2D;
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CEffect2D* CEffect2D::Create(const int nType, D3DXVECTOR3 pos, float fScatterAngle, const bool bIn3D)
{
    // エフェクトの生成情報を取得
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // メモリの確保
    CEffect2D *pEffect2D = NULL;

    // タイプごとに、UIの手前か後ろのエフェクトかどうかを決める
    if (pCreateInfo->bUseZBuffer)
    {
        pEffect2D = new CEffect2D(CScene::OBJTYPE_EFFECT2D_BACK_UI);
    }
    else
    {
        pEffect2D = new CEffect2D(CScene::OBJTYPE_EFFECT2D_FRONT_UI);
    }

    // 飛散角度が既に設定されているエフェクトなら、置き換える
    if (pCreateInfo->nScatterAngle > 0)
    {
        fScatterAngle = ((float)pCreateInfo->nScatterAngle / EFFECT_FLOATING_POINT) * 2;
    }

    // メモリを確保出来たら
    if (pEffect2D != NULL)
    {
        // 設定を反映
        //pEffect2D->CScene2D::SetColor(pCreateInfo->col);（effect3dから持ってきた際に、effect2dでは生成1F後に見えるようになるため、削除）
        pEffect2D->CScene2D::SetAlphaTestBorder(pCreateInfo->nAlphaTestBorder);

        // テクスチャ情報から、アニメーションの有無を取得
        CTexture *pTexture = CManager::GetTexture();
        pEffect2D->m_nAnimPattern = pTexture->GetInfo(pCreateInfo->nTexType)->nPattern;
        pEffect2D->m_nAnimSpeed = pTexture->GetInfo(pCreateInfo->nTexType)->nSpeed;
        pEffect2D->m_bRepeat = pTexture->GetInfo(pCreateInfo->nTexType)->bRepeat;

        // 初期の回転角度は、最大回転速度が0より大きいなら変える
        float fRotAngle = 0.0f;
        if (pCreateInfo->nRotSpeedMax > 0)
        {
            fRotAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;
            pEffect2D->m_fRotSpeed = D3DXToRadian(float(GetRandNum(pCreateInfo->nRotSpeedMax, pCreateInfo->nRotSpeedMin)) / EFFECT_FLOATING_POINT
                - float(GetRandNum(pCreateInfo->nRotSpeedMax, pCreateInfo->nRotSpeedMin)) / EFFECT_FLOATING_POINT);
        }
        pEffect2D->m_fRotAngle = fRotAngle;

        // 種類を結びつけておく
        pEffect2D->m_nTexType = pCreateInfo->nTexType;

        // 位置とサイズを初期化
        if (bIn3D)
        {
            pos = ConvertScreenPos(pos);    // 3D上なら、スクリーン座標に変換
        }
        float fSize = 0.0f;
        if (pCreateInfo->nSizeMax > 0)
        {
            fSize = float(GetRandNum(pCreateInfo->nSizeMax, pCreateInfo->nSizeMin)) / EFFECT_FLOATING_POINT;
        }
        pEffect2D->Init(pos, D3DXVECTOR3(fSize, fSize, 0.0f));

        // メンバ変数に各値を結びつけていく
        pEffect2D->m_fAcceleration = pCreateInfo->fAcceleration;
        pEffect2D->m_fGravity = pCreateInfo->fGravity;
        pEffect2D->m_fGravityLimit = pCreateInfo->fGravityLimit;

        pEffect2D->m_col = pCreateInfo->col;
        pEffect2D->m_colChangeRate = pCreateInfo->colChangeRate;
        pEffect2D->m_fFadeOutRate = pCreateInfo->fFadeOutRate;
        pEffect2D->m_nCntFadeOutLock = pCreateInfo->nCntFadeOutLock;

        pEffect2D->m_sizeChangeRate = pCreateInfo->sizeChangeRate;
        pEffect2D->m_nCntSizeChangeLock = pCreateInfo->nCntSizeChangeLock;
        pEffect2D->m_bSizeChangeStartOrFinish = pCreateInfo->bSizeChangeStartOrFinish;

        pEffect2D->m_bUseAdditiveSynthesis = pCreateInfo->bUseAdditiveSynthesis;

        // 移動量は最後に決定（移動の向きに応じて、初期の回転角度が変わる可能性があるため）
        pEffect2D->MoveCalculation(nType, fScatterAngle);
    }

    return pEffect2D;
}

//=============================================================================
// 移動量を算出する
// Author : 後藤慎之助
//=============================================================================
void CEffect2D::MoveCalculation(const int nType, const float fScatterAngle)
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
        m_fRotAngle = -fMoveAngle;
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