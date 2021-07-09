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
    m_buttonStateOld.bButtonA = false;
    m_buttonStateOld.bButtonX = false;
    m_buttonStateOld.bButtonB = false;
    m_buttonStateOld.bButtonY = false;
    m_pPlayer = NULL;

    m_core = CORE_THINKING;
    m_nCntActionTime = 0;
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
    // 変数宣言
    bool bCurrentButtonA = false;         // 現在のAボタン
    bool bCurrentButtonX = false;         // 現在のXボタン
    bool bCurrentButtonB = false;         // 現在のBボタン
    bool bCurrentButtonY = false;         // 現在のYボタン

    // コアごとに場合分け
    switch (m_core)
    {
    case CORE_THINKING:
        Thinking();
        DontMove(false);
        break;
    case CORE_ASSAULT:
        AnotherAction();
        RushToBall();
        bCurrentButtonA = JumpBecauseBallUp();
        bCurrentButtonX = DecideAttack(false);
        break;
    case CORE_AVOID:
        AnotherAction();
        RunAwayFromBall();
        bCurrentButtonA = JumpBecauseBallMoveDown();
        bCurrentButtonX = DecideAttack(true);
        break;
    case CORE_WAIT:
        AnotherAction();
        DontMove(true);
        bCurrentButtonX = DecideAttack(true);
        break;
    }

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

    // Bボタントリガー
    if (bCurrentButtonB && !m_buttonStateOld.bButtonB)
    {
        m_pPlayer->GetControlInput()->bTriggerB = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerB = false;
    }

    // Yボタントリガー
    if (bCurrentButtonY && !m_buttonStateOld.bButtonY)
    {
        m_pPlayer->GetControlInput()->bTriggerY = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerY = false;
    }
    //==============================================================

    // 1F前の情報として記憶しておく
    m_buttonStateOld.bButtonA = bCurrentButtonA;
    m_buttonStateOld.bButtonX = bCurrentButtonX;
    m_buttonStateOld.bButtonB = bCurrentButtonB;
    m_buttonStateOld.bButtonY = bCurrentButtonY;
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
    pAI->GetThinkingTime();

    return pAI;
}

//=============================================================================
// 考え中の処理
// Author : 後藤慎之助
//=============================================================================
void CAi::Thinking(void)
{
    // 時間を数える
    m_nCntActionTime--;

    // 一定カウンタで、次のコアを決める
    if (m_nCntActionTime < 0)
    {
        m_nCntActionTime = 0;

        // ランダムに数字を決める
        int nRandNum = GetRandNum(100, 1);

        // 何の攻撃が採用されたか、AIレベルによって決める
        if (nRandNum >= 1 && nRandNum <= CORE_PRIORITY_1)
        {
            switch (m_pPlayer->GetAILevel())
            {
            case CPlayer::AI_LEVEL_1:
                GetAvoidTime();
                break;
            case CPlayer::AI_LEVEL_2:
                GetWaitTime();
                break;
            case CPlayer::AI_LEVEL_3:
                // 現在のプレイヤー人数によって場合分け
                if (CGame::GetNumCurrentDispPlayer() > 2)
                {
                    GetAvoidTime();
                }
                else
                {
                    GetAssaultTime();
                }
                break;
            }
        }
        else if (nRandNum > CORE_PRIORITY_1 && nRandNum <= CORE_PRIORITY_1 + CORE_PRIORITY_2)
        {
            switch (m_pPlayer->GetAILevel())
            {
            case CPlayer::AI_LEVEL_1:
                GetAssaultTime();
                break;
            case CPlayer::AI_LEVEL_2:
                GetAssaultTime();
                break;
            case CPlayer::AI_LEVEL_3:
                GetWaitTime();
                break;
            }
        }
        else if (nRandNum > CORE_PRIORITY_1 + CORE_PRIORITY_2
            && nRandNum <= CORE_PRIORITY_1 + CORE_PRIORITY_2 + CORE_PRIORITY_3)
        {
            switch (m_pPlayer->GetAILevel())
            {
            case CPlayer::AI_LEVEL_1:
                GetWaitTime();
                break;
            case CPlayer::AI_LEVEL_2:
                GetAvoidTime();
                break;
            case CPlayer::AI_LEVEL_3:
                // 現在のプレイヤー人数によって場合分け
                if (CGame::GetNumCurrentDispPlayer() > 2)
                {
                    GetAssaultTime();
                }
                else
                {
                    GetAvoidTime();
                }
                break;
            }
        }
    }
}

//=============================================================================
// 考える以外の行動中
// Author : 後藤慎之助
//=============================================================================
void CAi::AnotherAction(void)
{
    // 時間を数える
    m_nCntActionTime--;

    // 一定カウンタで、一回考える
    if (m_nCntActionTime < 0)
    {
        GetThinkingTime();
    }
}

//=============================================================================
// 考え時間を得る処理
// Author : 後藤慎之助
//=============================================================================
void CAi::GetThinkingTime(void)
{
    // コアを考え中に
    m_core = CORE_THINKING;

    // レベルに応じて考え時間を得る
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(THINKING_TIME_AI_LEVEL_1_MAX, THINKING_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(THINKING_TIME_AI_LEVEL_2_MAX, THINKING_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(THINKING_TIME_AI_LEVEL_3_MAX, THINKING_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// 強襲時間を得る処理
// Author : 後藤慎之助
//=============================================================================
void CAi::GetAssaultTime(void)
{
    // コアを強襲に
    m_core = CORE_ASSAULT;

    // レベルに応じて考え時間を得る
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(ASSAULT_TIME_AI_LEVEL_1_MAX, ASSAULT_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(ASSAULT_TIME_AI_LEVEL_2_MAX, ASSAULT_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(ASSAULT_TIME_AI_LEVEL_3_MAX, ASSAULT_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// 避ける時間を得る処理
// Author : 後藤慎之助
//=============================================================================
void CAi::GetAvoidTime(void)
{
    // コアを避けに
    m_core = CORE_AVOID;

    // レベルに応じて考え時間を得る
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(AVOID_TIME_AI_LEVEL_1_MAX, AVOID_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(AVOID_TIME_AI_LEVEL_2_MAX, AVOID_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(AVOID_TIME_AI_LEVEL_3_MAX, AVOID_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// 待つ時間を得る処理
// Author : 後藤慎之助
//=============================================================================
void CAi::GetWaitTime(void)
{
    // コアを待機中に
    m_core = CORE_WAIT;

    // レベルに応じて考え時間を得る
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(WAIT_TIME_AI_LEVEL_1_MAX, WAIT_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(WAIT_TIME_AI_LEVEL_2_MAX, WAIT_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(WAIT_TIME_AI_LEVEL_3_MAX, WAIT_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// 移動しない処理
// Author : 後藤慎之助
//=============================================================================
void CAi::DontMove(bool bUseTurn)
{
    m_pPlayer->GetControlInput()->bTiltedLeftStick = false;

    // 移動しない程度にボールのほうを振り向くなら
    if (bUseTurn)
    {
        // ボールを取得
        CBall* pBall = CGame::GetBall();

        // ボールのほうを向いていないならそちらを向く
        if (pBall->GetPos().x > m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_LEFT ||
            pBall->GetPos().x < m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_RIGHT)
        {
            float fAngle = GetAngleToTarget3D2D(m_pPlayer->GetPos(), pBall->GetPos());

            m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
            m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;
        }
    }
}

//=============================================================================
// ボールに詰め寄る処理
// Author : 後藤慎之助
//=============================================================================
void CAi::RushToBall(void)
{
    // ボールを取得
    CBall* pBall = CGame::GetBall();
    float fAngle = GetAngleToTarget3D2D(m_pPlayer->GetPos(), pBall->GetPos());

    m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
    m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;
}

//=============================================================================
// ボールから逃げる処理
// Author : 後藤慎之助
//=============================================================================
void CAi::RunAwayFromBall(void)
{
    // ボールを取得
    CBall* pBall = CGame::GetBall();
    float fAngle = GetAngleToTarget3D2D(pBall->GetPos(), m_pPlayer->GetPos());

    // 右か左に角度調整
    if (fAngle > D3DXToRadian(0.0f) && fAngle <= D3DXToRadian(180.0f))
    {
        fAngle = D3DXToRadian(90.0f);
    }
    else
    {
        fAngle = D3DXToRadian(-90.0f);
    }

    m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
    m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;

    // ステージの横幅の約半分まで離れたら、強制的に考える（レベル3は強襲にする）
    float fDistance = GetDistance2D(m_pPlayer->GetPos(), CGame::GetBall()->GetPos());
    if (fDistance >= CGame::GetMapLimit().fWidth - BALL_COLLISION_SIZE_SIDE)
    {
        if (m_pPlayer->GetAILevel() == CPlayer::AI_LEVEL_3)
        {
            GetAssaultTime();
        }
        else
        {
            GetThinkingTime();
        }
    }
}

//=============================================================================
// ボールが上にあるからジャンプする処理
// Author : 後藤慎之助
//=============================================================================
bool CAi::JumpBecauseBallUp(void)
{
    // ジャンプするかどうか
    bool bUseJump = false;

    // ボールを取得
    CBall* pBall = CGame::GetBall();

    // ボールのほうが上にあるなら
    if (pBall->GetPos().y > m_pPlayer->GetPos().y + m_pPlayer->GetCollisionSizeDeffence().y)
    {
        bUseJump = true;
    }

    // 地上にいて、ジャンプボタンを長押ししようとしているなら、長押し解除
    if (m_pPlayer->GetGround() && m_buttonStateOld.bButtonA)
    {
        return false;
    }

    // 空中にいて、滞空が終了したなら、長押し解除
    if (m_pPlayer->GetPressJumpTime() > PLAYER_JUMP_KEEP_FRAME)
    {
        return false;
    }

    return bUseJump;
}

//=============================================================================
// ボールが下に移動してきているからジャンプする処理
// Author : 後藤慎之助
//=============================================================================
bool CAi::JumpBecauseBallMoveDown(void)
{   
    // ジャンプするかどうか
    bool bUseJump = false;

    // ボールを取得
    CBall* pBall = CGame::GetBall();

    // ボールが下に動いてるから
    if (pBall->GetPosOld().y > pBall->GetPos().y)
    {
        bUseJump = true;
    }

    // 地上にいて、ジャンプボタンを長押ししようとしているなら、長押し解除
    if (m_pPlayer->GetGround() && m_buttonStateOld.bButtonA)
    {
        return false;
    }

    // 空中にいて、滞空が終了したなら、長押し解除
    if (m_pPlayer->GetPressJumpTime() > PLAYER_JUMP_KEEP_FRAME)
    {
        return false;
    }

    return bUseJump;
}

//=============================================================================
// 決めきる攻撃をするかどうかの処理
// Author : 後藤慎之助
//=============================================================================
bool CAi::DecideAttack(bool bUseTurn)
{
    // 基本的な攻撃をするかどうか
    bool bUseDecideAttack = false;

    // ボールを取得
    CBall* pBall = CGame::GetBall();

    // ボールが当たり判定を使っていないなら、関数を抜ける
    if (!pBall->GetUseCollision())
    {
        return false;
    }

    // ボールのほうを向いていないなら、関数を抜ける（振り向きで打てる攻撃を使用しないので、若干弱くなる）
    if (!bUseTurn)
    {
        if (pBall->GetPos().x > m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_LEFT ||
            pBall->GetPos().x < m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_RIGHT)
        {
            return false;
        }
    }

    // プレイヤーの中心座標
    D3DXVECTOR3 playerCenterPos = m_pPlayer->GetPos() + D3DXVECTOR3(0.0f, m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);

    // 攻撃をすると判断する距離
    float fAttackDistance = (m_pPlayer->GetCollisionSizeDeffence().x + m_pPlayer->GetCollisionSizeDeffence().y) * 0.5f + (BALL_COLLISION_SIZE_SIDE);
    if (GetDistance2D(playerCenterPos, pBall->GetPos()) <= fAttackDistance)
    {
        bUseDecideAttack = true;

        // チャージ中にボールが近づいたなら、即座に離す
        if (m_pPlayer->GetSwingCharge() > 0)
        {
            bUseDecideAttack = false;
        }

        // 背を向けているならボールの方を向く
        if (bUseTurn)
        {
            RushToBall();

            // 打ち始めで近くのプレイヤーに左右されないため、関数を抜ける
            if (m_pPlayer->GetStopTime() == 0)
            {
                return true;
            }
        }

        // 飛ばすとき、近くのプレイヤーのほうにスティックを倒す
        if (m_pPlayer->GetStopTime() == 1 || m_pPlayer->GetStopTime() == 0)
        {
            GetAttackAngle();
        }
    }

    return bUseDecideAttack;
}

//=============================================================================
// 攻撃時にどの方向にスティックを倒すを倒すか
// Author : 後藤慎之助
//=============================================================================
void CAi::GetAttackAngle(void)
{
    // ランダムに数字を決める
    int nRandNum = GetRandNum(100, 1);
    int nBorderNum = 0;

    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        nBorderNum = ATTACK_TARGET_AI_LEVEL_1;
        break;
    case CPlayer::AI_LEVEL_2:
        nBorderNum = ATTACK_TARGET_AI_LEVEL_2;
        break;
    case CPlayer::AI_LEVEL_3:
        nBorderNum = ATTACK_TARGET_AI_LEVEL_3;
        break;
    }

    // 境界の値の範囲内なら、近くの敵を狙った角度に倒す
    if (nRandNum >= 1 && nRandNum <= nBorderNum)
    {
        m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
        m_pPlayer->GetControlInput()->fLeftStickAngle = CGame::GetAngleToClosestPlayer(m_pPlayer->GetPlayable(), m_pPlayer->GetPos());
    }
    else
    {
        // 範囲外なら、適当にスティックを倒す
        m_pPlayer->GetControlInput()->fLeftStickAngle = float(rand() % 314) / 100 - float(rand() % 314) / 100;
    }
}
