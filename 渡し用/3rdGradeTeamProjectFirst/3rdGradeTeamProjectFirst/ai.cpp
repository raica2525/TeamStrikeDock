//===============================================
//
// AI処理 (ai.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "ai.h"
#include "game.h"
#include "ball.h"
#include "library.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CAi::CAi()
{
    m_pPlayer = NULL;

    m_buttonStateOld.bButtonA = false;
    m_buttonStateOld.bButtonX = false;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CAi::~CAi()
{
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CAi::Update(void)
{
    // 移動
    Move();

    // 変数宣言
    bool bCurrentButtonA = Jump();         // ジャンプ
    bool bCurrentButtonX = BaseAttack();   // 基本的な攻撃をするかどうか

    //==============================================================
    // コントローラの操作に置き換える
    // Aボタントリガー
    if (bCurrentButtonA && !m_buttonStateOld.bButtonA)
    {
        m_pPlayer->GetControlInput()->bTriggerA = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerA = false;
    }

    // Aボタンプレス
    if (bCurrentButtonA && m_buttonStateOld.bButtonA)
    {
        m_pPlayer->GetControlInput()->bPressA = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bPressA = false;
    }

    // Xボタントリガー
    if (bCurrentButtonX && !m_buttonStateOld.bButtonX)
    {
        m_pPlayer->GetControlInput()->bTriggerX = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerX = false;
    }

    // Xボタンプレス
    if (bCurrentButtonX && m_buttonStateOld.bButtonX)
    {
        m_pPlayer->GetControlInput()->bPressX = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bPressX = false;
    }

    // Xボタンリリース
    if (!bCurrentButtonX && m_buttonStateOld.bButtonX)
    {
        m_pPlayer->GetControlInput()->bReleaseX = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bReleaseX = false;
    }
    //==============================================================

    // 1F前の情報として記憶しておく
    m_buttonStateOld.bButtonA = bCurrentButtonA;
    m_buttonStateOld.bButtonX = bCurrentButtonX;
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CAi * CAi::Create(CPlayer *pPlayer)
{
    // インスタンスの生成
    CAi *pAI;
    pAI = new CAi;
    
    pAI->m_pPlayer = pPlayer;

    return pAI;
}

//=============================================================================
// 移動処理
// Author : 後藤慎之助
//=============================================================================
void CAi::Move(void)
{
    // ボールを取得
    CBall* pBall = CGame::GetBall();
    float fAngle = GetAngleToTarget3D2D(m_pPlayer->GetPos(), pBall->GetPos());

    m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
    m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;
}

//=============================================================================
// ジャンプ処理
// Author : 後藤慎之助
//=============================================================================
bool CAi::Jump(void)
{
    // ジャンプするかどうか
    bool bUseJump = false;

    // ボールを取得
    CBall* pBall = CGame::GetBall();

    // ボールのほうが上にあるなら（仮）
    if (pBall->GetPos().y > m_pPlayer->GetPos().y + m_pPlayer->GetActiveCollisionSize().y)
    {
        bUseJump = true;
    }

    return bUseJump;
}

//=============================================================================
// 基本的な攻撃をするかどうかの処理
// Author : 後藤慎之助
//=============================================================================
bool CAi::BaseAttack(void)
{
    // 基本的な攻撃をするかどうか
    bool bUseBaseAttack = false;

    // ボールを取得
    CBall* pBall = CGame::GetBall();

    // プレイヤーの中心座標
    D3DXVECTOR3 playerCenterPos = m_pPlayer->GetPos() + D3DXVECTOR3(0.0f, m_pPlayer->GetActiveCollisionSize().y, 0.0f);

    // 攻撃をすると判断する距離（仮）
    float fAttackDistance = (m_pPlayer->GetActiveCollisionSize().x + m_pPlayer->GetActiveCollisionSize().y) / 2;
    if (GetDistance2D(playerCenterPos, pBall->GetPos()) <= fAttackDistance)
    {
        bUseBaseAttack = true;

        // 飛ばすとき、近くのプレイヤーのほうにスティックを倒す
        if (m_pPlayer->GetStopTime() == 1)
        {
            m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
            m_pPlayer->GetControlInput()->fLeftStickAngle = CGame::GetAngleToClosestPlayer(m_pPlayer->GetPlayable(), m_pPlayer->GetPos());
        }
    }

    return bUseBaseAttack;
}