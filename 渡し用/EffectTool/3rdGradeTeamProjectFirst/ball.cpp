//===============================================
//
// ボールの処理 (ball.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "ball.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
//#include "effect3d.h"
#include "library.h"
#include "player.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CBall::CBall() :CScene3D(CScene::OBJTYPE_BALL)
{
    m_shootFlag = SHOOT_FLAG_NONE;
    m_moveAngle = BALL_FIRST_MOVE_ANGLE;
    m_fSpeed = BALL_FIRST_SPEED;

    m_nCntStopTime = 0;
    m_bUseCollision = false;
    m_bStartButtle = true;

    m_nWhoShooting = BALL_NOT_ANYONE;
    m_pPlayer = NULL;

    m_bAfterThrow = false;
    m_nCntBuntTime = 0;

    memset(m_aCornerPos, 0, sizeof(m_aCornerPos));
    m_posOld = DEFAULT_VECTOR;

    m_absorbFlag = ABSORB_FLAG_NONE;
    m_nWhoAbsorbing = BALL_NOT_ANYONE;
    m_nCntAbsorbTime = 0;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CBall::~CBall()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CBall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene3D::Init(pos, size);

    // スケールを設定
    SetScale(size);

    // モデルデータをバインド
    BindModelData(0);   // ボールは0番

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Update(void)
{
    // 1F前の位置を記憶
    m_posOld = GetPos();

    // 投げなら
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_THROW))
    {
        Throw();
    }
    else
    {
        // 位置、向き取得
        D3DXVECTOR3 pos = GetPos();
        D3DXVECTOR3 rot = GetRot();

        // 吸収状態でないなら、移動処理
        pos = Move(pos);

        // バトル開始状態なら
        if (m_bStartButtle)
        {
            pos = StartButtleMove(pos);
        }

        // 吸収状態なら
        if (m_nWhoAbsorbing != BALL_NOT_ANYONE)
        {
            pos = Absorb();
        }

        // マップ制限
        CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
        if (pos.x + (BALL_COLLISION_SIZE_SIDE / 2) > mapLimit.fWidth)
        {
            HitWall(HIT_WALL_RIGHT);
            pos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
            m_moveAngle.x *= -1;
        }
        else if (pos.x - (BALL_COLLISION_SIZE_SIDE / 2) < -mapLimit.fWidth)
        {
            HitWall(HIT_WALL_LEFT);
            pos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
            m_moveAngle.x *= -1;
        }
        if (pos.y + BALL_COLLISION_SIZE_SIDE > mapLimit.fHeight)
        {
            HitWall(HIT_WALL_UP);
            pos.y = mapLimit.fHeight - BALL_COLLISION_SIZE_SIDE;
            m_moveAngle.y *= -1;
        }
        else if (pos.y - (BALL_COLLISION_SIZE_SIDE / 2) < 0)
        {
            HitWall(HIT_WALL_DOWN);
            pos.y = (BALL_COLLISION_SIZE_SIDE / 2);
            m_moveAngle.y *= -1;
        }

        // 四つ角の位置を更新
        UpdateCornerPos(pos);

        // 位置、向きを反映
        SetPos(pos);
        SetRot(rot);

#ifdef COLLISION_TEST
        if (m_bUseCollision)
        {
            CDebug::Create(pos + D3DXVECTOR3(0.0f, 0.0f, -50.0f), BALL_COLLISION_SIZE, CDebug::TYPE_MOMENT);
        }
#endif // COLLISION_TEST

        //// ライフが0以下なら、未使用に
        //if (m_nLife <= 0)
        //{
        //    // 終了処理
        //    Uninit();
        //}
    }
}

//=============================================================================
// 四つ角の位置を更新
// Author : 後藤慎之助
//=============================================================================
void CBall::UpdateCornerPos(D3DXVECTOR3 pos)
{
    // 真ん中の座標を出す
    D3DXVECTOR3 centerPos = (pos + m_posOld) / 2;

    // 辺の長さを求める
    float fLengthX = fabsf(pos.x - m_posOld.x);
    float fLengthY = fabsf(pos.y - m_posOld.y);

    // 回転前の四つ角の位置を決める
    if (fLengthX > fLengthY)
    {
        // 回転前の四つ角の位置を出す（横長）
        m_aCornerPos[0] = centerPos + D3DXVECTOR3(-(fLengthX / 2) - (BALL_COLLISION_SIZE_SIDE / 2), (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[1] = centerPos + D3DXVECTOR3(-(fLengthX / 2) - (BALL_COLLISION_SIZE_SIDE / 2), -(BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[2] = centerPos + D3DXVECTOR3((fLengthX / 2) + (BALL_COLLISION_SIZE_SIDE / 2), -(BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[3] = centerPos + D3DXVECTOR3((fLengthX / 2) + (BALL_COLLISION_SIZE_SIDE / 2), (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
    }
    else
    {
        // 回転前の四つ角の位置を出す（縦長）
        m_aCornerPos[0] = centerPos + D3DXVECTOR3(-(BALL_COLLISION_SIZE_SIDE / 2), (fLengthY / 2) + (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[1] = centerPos + D3DXVECTOR3(-(BALL_COLLISION_SIZE_SIDE / 2), -(fLengthY / 2) - (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[2] = centerPos + D3DXVECTOR3((BALL_COLLISION_SIZE_SIDE / 2), -(fLengthY / 2) - (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[3] = centerPos + D3DXVECTOR3((BALL_COLLISION_SIZE_SIDE / 2), (fLengthY / 2) + (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
    }

    // 移動した角度を出す
    float fMoveAngle = atan2((pos.x - m_posOld.x), (pos.y - m_posOld.y));

    // 回転後の四つ角の位置を出す（左上、左下、右下、右上の順）
    m_aCornerPos[0] = caclRotate(m_aCornerPos[0], centerPos, fMoveAngle);
    m_aCornerPos[1] = caclRotate(m_aCornerPos[1], centerPos, fMoveAngle);
    m_aCornerPos[2] = caclRotate(m_aCornerPos[2], centerPos, fMoveAngle);
    m_aCornerPos[3] = caclRotate(m_aCornerPos[3], centerPos, fMoveAngle);
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Draw(void)
{
    CScene3D::Draw();
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CBall * CBall::Create(D3DXVECTOR3 pos, bool bUseCollision)
{
    // ポインタを用意
    CBall *pBall = NULL;

    // メモリ確保
    pBall = new CBall;

    // 初期化
    pBall->Init(pos, DEFAULT_SCALE);

    // メンバ変数を結びつける
    pBall->m_bUseCollision = bUseCollision;

    return pBall;
}

//=============================================================================
// バトル開始時の移動処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CBall::StartButtleMove(D3DXVECTOR3 pos)
{
    // 変数宣言
    D3DXVECTOR3 returnPos = pos;

    // どこで止めるか
    if (pos.y < BALL_START_BUTTLE_STOP_POS_Y)
    {
        pos.y = BALL_START_BUTTLE_STOP_POS_Y;
        m_moveAngle = DEFAULT_VECTOR;
    }

    return returnPos;
}

//=============================================================================
// 移動処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CBall::Move(D3DXVECTOR3 pos)
{
    // 変数宣言
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 moveAngle = m_moveAngle;    // 今回の移動角度
    D3DXVECTOR3 move = DEFAULT_VECTOR;      // 今回の移動量
    float fSpeed = m_fSpeed;                // 今回の速度
    float fGravity = 0.0f;                  // 重力

    // 各特殊処理
    if (m_bAfterThrow)
    {
        // 投げ直後は一定速度以下なら、その速度に
        if (fSpeed < BALL_THROW_FIRST_SPEED)
        {
            fSpeed = BALL_THROW_FIRST_SPEED;
        }
    }
    else if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // バントボールは一定速度かつ重量の影響を受ける
        fSpeed = BALL_BUNT_SPEED;
        m_nCntBuntTime++;
        fGravity = m_nCntBuntTime * BALL_BUNT_GRAVITY;
    }

    // 硬直時間のカウンタ
    if (m_nCntStopTime > 0)
    {
        m_nCntStopTime--;

        // 最後のフレームで、当たり判定を復活させる
        if (m_nCntStopTime == 0)
        {
            m_bUseCollision = true;
        }
    }
    else
    {
        // 移動量を決定
        move = (moveAngle * fSpeed) - D3DXVECTOR3(0.0f, fGravity, 0.0f);

        // バントボールなら、y軸移動に上限がある
        if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
        {
            if (move.y < BALL_BUNT_MAX_FALL_SPEED)
            {
                move.y = BALL_BUNT_MAX_FALL_SPEED;
            }
        }

        // 位置に移動量を加算
        returnPos += move;
    }

    return returnPos;
}

//=============================================================================
// 吸収処理
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CBall::Absorb(void)
{
    // 変数宣言
    D3DXVECTOR3 returnPos = GetPos();   // これより前の処理の位置に左右されない

    // プレイヤーのポインタを持っているなら
    if (m_pPlayer)
    {
        // 移動中なら
        if (IS_BITON(m_absorbFlag, ABSORB_FLAG_MOVING))
        {
            // 目的地へ位置を近づける
            D3DXVECTOR3 targetPos = m_pPlayer->GetPos() + D3DXVECTOR3(m_pPlayer->GetActiveCollisionSize().x / 2 + BALL_ABSORB_POS_X,
                m_pPlayer->GetActiveCollisionSize().y / 2, 0.0f);
            float fAngleToTarget = GetAngleToTarget3D2D(returnPos, targetPos);
            D3DXVECTOR3 move = DEFAULT_VECTOR;
            move.x = sinf(fAngleToTarget) * BALL_ABSORB_SPEED;
            move.y = cosf(fAngleToTarget) * BALL_ABSORB_SPEED;
            returnPos += move;

            // 目的地に近いなら
            float fDistance = GetDistance2D(returnPos, targetPos);
            if (fDistance < BALL_ABSORB_CLOSE_PLAYER)
            {
                // 移動から到着へ
                BITOFF(m_absorbFlag, ABSORB_FLAG_MOVING);
                BITON(m_absorbFlag, ABSORB_FLAG_REACHED);
            }
        }
        else if (IS_BITON(m_absorbFlag, ABSORB_FLAG_REACHED))
        {
            // 到着しているなら
            // 右向きなら
            if (m_pPlayer->GetRot().y == PLAYER_ROT_RIGHT)
            {
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(m_pPlayer->GetActiveCollisionSize().x / 2 + BALL_ABSORB_POS_X,
                    m_pPlayer->GetActiveCollisionSize().y / 2, 0.0f);
            }
            else if (m_pPlayer->GetRot().y == PLAYER_ROT_LEFT)
            {
                // 左向きなら
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(-(m_pPlayer->GetActiveCollisionSize().x / 2 + BALL_ABSORB_POS_X),
                    m_pPlayer->GetActiveCollisionSize().y / 2, 0.0f);
            }
        }
    }

    // 吸収時間のカウントダウン
    if (m_nCntAbsorbTime > 0)
    {
        m_nCntAbsorbTime--;

        // 最後のフレームで打ち上げる
        if (m_nCntAbsorbTime == 0)
        {
            Launch(returnPos);
        }
    }

    return returnPos;
}

//=============================================================================
// 壁に当たった時の共通処理
// Author : 後藤慎之助
//=============================================================================
void CBall::HitWall(HIT_WALL hitWall)
{
    // メンバ変数の調整
    m_bAfterThrow = false;

    // バントボール状態なら
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // 上壁以外なら
        if (hitWall != HIT_WALL_UP)
        {
            // バントボール状態を解除し、最後に打った人のボールにして、縦移動に
            BITOFF(m_shootFlag, SHOOT_FLAG_BUNT);
            m_nCntBuntTime = 0;
            m_moveAngle = BALL_FIRST_MOVE_ANGLE;
            if (m_pPlayer)
            {
                m_nWhoShooting = m_pPlayer->GetPlayable();
            }
        }
        else
        {
            // 上壁なら移動の向きに影響を与えない
            m_moveAngle.y *= -1;
        }
    }
    else
    {
        // バントボール以外なら、壁に当たった時にわずかに硬直する
        m_nCntStopTime = BALL_SHOOT_STOP_REFRECT_FRAME;
    }
}

//=============================================================================
// ゲーム内にとどめる
// Author : 後藤慎之助
//=============================================================================
D3DXVECTOR3 CBall::StayInGame(D3DXVECTOR3 pos)
{
    D3DXVECTOR3 returnPos = pos;

    // マップ制限
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    if (returnPos.x + (BALL_COLLISION_SIZE_SIDE / 2) > mapLimit.fWidth)
    {
        returnPos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
    }
    else if (returnPos.x - (BALL_COLLISION_SIZE_SIDE / 2) < -mapLimit.fWidth)
    {
        returnPos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
    }
    if (returnPos.y + BALL_COLLISION_SIZE_SIDE > mapLimit.fHeight)
    {
        returnPos.y = mapLimit.fHeight - BALL_COLLISION_SIZE_SIDE;
    }
    else if (returnPos.y - (BALL_COLLISION_SIZE_SIDE / 2) < 0)
    {
        returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2);
    }

    return returnPos;
}

//=============================================================================
// 投げの処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Throw(void)
{
    // 位置を取得
    D3DXVECTOR3 pos = GetPos();

    // プレイヤーの情報を持っているなら
    if (m_pPlayer)
    {
        // プレイヤーの左手に固定
        pos = D3DXVECTOR3(m_pPlayer->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._41,
            m_pPlayer->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._42,
            m_pPlayer->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._43);

        // プレイヤーの投げが終わったら、投げ状態を解除
        if (m_pPlayer->GetAttackTime() == ATTACK_THROW_END_FRAME)
        {
            // 投げ直後判定をtrueに
            m_bAfterThrow = true;

            // 投げ状態を解除
            BITOFF(m_shootFlag, SHOOT_FLAG_THROW);

            // 当たり判定を復活させる
            m_bUseCollision = true;

            // 攻撃角度を出す
            float fAttackAngle = 90.0f;

            // プレイヤーの向きが左向きなら、飛ばす方向を逆に
            if (m_pPlayer->GetRot().y == PLAYER_ROT_LEFT)
            {
                fAttackAngle *= -1;
            }

            // スティックの角度によって、攻撃角度を変える
            if (m_pPlayer->GetControlInput()->bTiltedLeftStick)
            {
                // スティックが上か下を向いているなら、真下に
                if (STICK_SPIKE_VERTICAL(m_pPlayer->GetControlInput()->fLeftStickAngle))
                {
                    fAttackAngle = 180.0f;
                }
            }

            // 飛ばす方向を決定
            m_moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
            m_moveAngle.y = cosf(D3DXToRadian(fAttackAngle));
        }
    }

    // 位置を反映
    SetPos(pos);
}

//=============================================================================
// 撃つときの処理
// Author : 後藤慎之助
//=============================================================================
int CBall::Shoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, CPlayer*pPlayer)
{
    // いくつかのメンバ変数を調整
    ResetByShoot();

    // バントボール状態なら
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // バントボール状態を解除し、即打ち
        BITOFF(m_shootFlag, SHOOT_FLAG_BUNT);
        BITON(m_shootFlag, SHOOT_FLAG_QUICK);
    }

    // ファーヒットなら
    if (IS_BITON(flag, SHOOT_FLAG_FAR_HIT))
    {
        // 位置を攻撃発生位置に結びつける
        attackCenterPos = StayInGame(attackCenterPos);
        SetPos(attackCenterPos);
    }

    // プレイヤーの情報を取得し、誰が放ったかを記録
    m_pPlayer = pPlayer;
    m_nWhoShooting = m_pPlayer->GetPlayable();

    // 受け取ったシュートフラグの考慮
    if (IS_BITON(flag, SHOOT_FLAG_THROW))
    {
        // 当たり判定を一回消す
        m_bUseCollision = false;

        // 投げのフラグをオンにする
        BITON(m_shootFlag, SHOOT_FLAG_THROW);

        // 即打ちフラグを解除（打ったことになるため）
        BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);

        // 関数を抜ける
        return 0;
    }
    else if (IS_BITON(flag, SHOOT_FLAG_BUNT))
    {
        // バントのフラグをオンにする
        BITON(m_shootFlag, SHOOT_FLAG_BUNT);

        // 移動角度を受け取る
        m_moveAngle = moveAngle;

        // 誰のボールでもなくなる
        m_nWhoShooting = BALL_NOT_ANYONE;

        // 関数を抜ける
        return 0;
    }
    else if (IS_BITON(flag, SHOOT_FLAG_ABSORB))
    {
        // 吸収の設定
        SetAbsorb(pPlayer);

        // 関数を抜ける
        return 0;
    }
    else
    {
        // 最初の接触なら
        if (bFirstCollision)
        {
            // 当たり判定を一回消す
            m_bUseCollision = false;

            // 硬直時間を決める
            m_nCntStopTime = (int)(m_fSpeed * BALL_SHOOT_STOP_TIME_RATE);

            // 引数が即打ちなら
            if (IS_BITON(flag, SHOOT_FLAG_QUICK))
            {
                BITON(m_shootFlag, SHOOT_FLAG_QUICK);
            }

            // メンバ変数が即打ちなら
            if (IS_BITON(m_shootFlag, SHOOT_FLAG_QUICK))
            {
                // 即打ちフラグを解除し、硬直時間をまず半分に
                BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);
                m_nCntStopTime /= 2;

                // 最大硬直時間を調整
                if (m_nCntStopTime > BALL_QUICK_SHOOT_MAX_STOP_TIME)
                {
                    m_nCntStopTime = BALL_QUICK_SHOOT_MAX_STOP_TIME;
                }
            }

            // 硬直時間の最大
            if (m_nCntStopTime > BALL_SHOOT_MAX_STOP_TIME)
            {
                m_nCntStopTime = BALL_SHOOT_MAX_STOP_TIME;
            }
        }
        else
        {
            // 移動角度と力を結びつける
            m_moveAngle = moveAngle;
            m_fSpeed += fPower;
        }
    }

    // 一定速度未満なら、硬直時間は1
    if (m_fSpeed < BALL_SHOOT_STOP_START_SPEED)
    {
        m_nCntStopTime = 1;
    }

    return m_nCntStopTime;
}

//=============================================================================
// シュートによってリセットする変数
// Author : 後藤慎之助
//=============================================================================
void CBall::ResetByShoot(void)
{
    m_bAfterThrow = false;
    m_nCntBuntTime = 0;
    m_bStartButtle = false;
    m_absorbFlag = ABSORB_FLAG_NONE;
    m_nWhoAbsorbing = BALL_NOT_ANYONE;
    m_nCntAbsorbTime = 0;
}

//=============================================================================
// ラッキーガードや吸収時間切れで打ち上げるときの処理
// Author : 後藤慎之助
//=============================================================================
int CBall::Launch(D3DXVECTOR3 startPos, int nCntStopTime)
{
    // いくつかのメンバ変数を調整
    ResetByShoot();

    // 位置を開始位置に合わせる
    SetPos(startPos);

    // バントのフラグをオンにする
    BITON(m_shootFlag, SHOOT_FLAG_BUNT);

    // 移動角度を真上にする
    m_moveAngle.x = sinf(D3DXToRadian(0.0f));
    m_moveAngle.y = cosf(D3DXToRadian(0.0f));

    // 誰のボールでもなくなる
    m_nWhoShooting = BALL_NOT_ANYONE;

    // 硬直時間を結びつける
    SetStopTime(nCntStopTime);

    return m_nCntStopTime;
}

//=============================================================================
// 硬直時間を決める
// Author : 後藤慎之助
//=============================================================================
void CBall::SetStopTime(const int nCntStopTime)
{
    m_nCntStopTime = nCntStopTime;

    // 硬直しているなら
    if (m_nCntStopTime > 0)
    {
        // 当たり判定を一回消す
        m_bUseCollision = false;
    }
}

//=============================================================================
// 吸収を設定する
// Author : 後藤慎之助
//=============================================================================
void CBall::SetAbsorb(CPlayer *pPlayer)
{
    BITON(m_absorbFlag, ABSORB_FLAG_MOVING);
    m_pPlayer = pPlayer;
    m_nWhoShooting = BALL_NOT_ANYONE;           // プレイヤーへの当たり判定を消すため
    m_nWhoAbsorbing = pPlayer->GetPlayable();   // 次その吸収したプレイヤーだけがボールに触れるようにするため
    m_nCntAbsorbTime = BALL_ABSORB_MAX_TIME;    // 吸収時間のカウントダウンをスタート
}
