//======================================================================================
//
// プレイヤーの必殺技の処理 (player_sp.cpp)
// Author : 後藤慎之助
//
//======================================================================================

//========================
// インクルードファイル
//========================
#include "player.h"
#include "effect3d.h"
#include "ball.h"
#include "game.h"

//=============================================================================
// 必殺技の打ちだし処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::EmitSpShot(void)
{
    // スイング、スマッシュ、スパイクのいずれかなら
    if (m_attackState == ATTACK_STATE_SWING || m_attackState == ATTACK_STATE_SMASH || m_attackState == ATTACK_STATE_SPIKE)
    {
        // 必殺技ごとの打ちだし処理
        switch (m_spShot)
        {
        case SP_SHOT_HEAL:
            SpHeal();
            break;
        case SP_SHOT_DECOY:
            SpDecoy();
            break;
        case SP_SHOT_WALL_THROUGH:
            SpWallThrough();
            break;
        default:

            break;
        }

        // ゲージ全消費
        m_fSpGaugeCurrent = 0.0f;
    }
}

//=============================================================================
// 必殺技:回復の処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SpHeal(void)
{
    // 定義
    const float HEAL_VALUE = m_afParam[PARAM_7_WEAPON_SP]; // 回復割合を補助値から受け取る

    // 回復
    m_fLife += m_fDef * HEAL_VALUE;

    // 回復の上限
    if (m_fLife > m_fDef)
    {
        m_fLife = m_fDef;
    }

    // エフェクト発生
    CEffect3D::Emit(CEffectData::TYPE_HEAL, GetPos(), GetPos(), m_collisionSizeDeffence.x);
}

//=============================================================================
// 必殺技:ぶんしんボールの処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SpDecoy(void)
{
    // 攻撃角度
    float fAttackAngle = D3DXToRadian(180.0f);    // ニュートラルは真下

    // スティックが倒れているならその方向に飛ばす
    if (m_controlInput.bTiltedLeftStick)
    {
        fAttackAngle = m_controlInput.fLeftStickAngle;
    }

    // 移動角度決定
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
    moveAngle.x = sinf(fAttackAngle);
    moveAngle.y = cosf(fAttackAngle);

    // ボールを生成（現在ボール位置から、当たり判定のないものを生成）
    CBall*pBall = CBall::Create(CGame::GetBall()->GetPos(), false);

    // 生成したボールに必要な情報を結びつける
    pBall->SetMoveAngle(moveAngle);
    pBall->SetSpeed(CGame::GetBall()->GetSpeed());
    pBall->SetStartButtle(false);
    pBall->SetWhoShooting(CGame::GetPlayable(this));
    pBall->SetPlayer(this);
    pBall->SetHitWallMax((int)m_afParam[PARAM_7_WEAPON_SP]);    // 補助値から壁に当たったら消える最大数を設定

    // 追加で生成するボール
    if ((int)m_afParam[PARAM_6_WEAPON_SP] > 0)
    {
        for (int nCnt = 0; nCnt < (int)m_afParam[PARAM_6_WEAPON_SP]; nCnt++)
        {
            // 攻撃角度をランダムに決める
            fAttackAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;
            moveAngle.x = sinf(fAttackAngle);
            moveAngle.y = cosf(fAttackAngle);

            // ボールを生成
            pBall = CBall::Create(CGame::GetBall()->GetPos(), false);
            pBall->SetMoveAngle(moveAngle);
            pBall->SetSpeed(CGame::GetBall()->GetSpeed());
            pBall->SetStartButtle(false);
            pBall->SetWhoShooting(CGame::GetPlayable(this));
            pBall->SetPlayer(this);
            pBall->SetHitWallMax((int)m_afParam[PARAM_7_WEAPON_SP]);
        }
    }
}

//=============================================================================
// 必殺技:ウォールスルーの処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SpWallThrough(void)
{
    CGame::GetBall()->SetWallThrough((int)m_afParam[PARAM_7_WEAPON_SP]);
}