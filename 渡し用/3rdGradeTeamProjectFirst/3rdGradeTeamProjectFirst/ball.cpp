//===============================================
//
// ボールの処理 (ball.cpp)
// Author : 後藤慎之助、伊藤陽梧（軌跡）
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
#include "effect3d.h"
#include "library.h"
#include "player.h"
#include "wave.h"
#include "effect2d.h"
#include "number_array.h"
#include "outline.h"
#include "input.h"
#include "locus.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CBall::CBall() :CScene3D(CScene::OBJTYPE_BALL)
{
    // アウトラインのポインタはコンストラクタでのみリセット
    m_pOutline = NULL;

    // メンバ変数のリセット
    ResetMenberVariables();
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CBall::~CBall()
{

}

//=============================================================================
// メンバ変数のリセット
// Author : 後藤慎之助
//=============================================================================
void CBall::ResetMenberVariables(void)
{
    m_shootFlag = SHOOT_FLAG_NONE;
    m_moveAngle = BALL_FIRST_MOVE_ANGLE;
    m_fSpeed = BALL_FIRST_SPEED;

    m_nCntCurrentStopTime = 0;
    m_nStopTimeAtShoot = 0;
    m_bUseCollision = false;
    m_bNeverUseCollision = false;
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

    m_bDisp = false;
    m_nCntDispingTime = 0;
    m_nCntNoLineStopTime = 0;

    //===================================
    // 必殺技対応周り
    //===================================
    m_nCntHitWall = 0;
    m_nCntHitWallMax = 0;

    m_bWallThrough = false;
    m_nCntWallThrough = 0;
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

    // アウトラインを設定
    if (!m_pOutline)
    {
        m_pOutline = COutline::Create(pos, D3DXVECTOR3(500.0f, 500.0f, 0.0f), DEFAULT_VECTOR, PLAYER_COLOR_NONE);
    }

    return S_OK;
}

//=============================================================================
// 再設定処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Reset(D3DXVECTOR3 pos)
{
    // 各メンバ変数のリセット
    ResetMenberVariables();

    // 位置を決め、回転とスケールをリセット
    SetPos(pos);
    m_posOld = pos;
    SetRot(DEFAULT_VECTOR);
    SetScale(DEFAULT_VECTOR);
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Uninit(void)
{
    // アウトラインを使用しない
    if (m_pOutline != NULL)
    {
        m_pOutline->Unable();
    }

    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Update(void)
{
    // 死亡の硬直時間をカウント
    if (m_nCntNoLineStopTime > 0)
    {
        m_nCntNoLineStopTime--;
    }

    // 表示しているなら
    if (m_bDisp)
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
            // 位置を取得
            D3DXVECTOR3 pos = GetPos();

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
                pos = HitWall(pos, HIT_WALL_RIGHT);
            }
            else if (pos.x - (BALL_COLLISION_SIZE_SIDE / 2) < -mapLimit.fWidth)
            {
                pos = HitWall(pos, HIT_WALL_LEFT);
            }
            if (pos.y + (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y  > mapLimit.fHeight)
            {
                pos = HitWall(pos, HIT_WALL_UP);
            }
            else if (pos.y - (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y < 0)
            {
                pos = HitWall(pos, HIT_WALL_DOWN);
            }

            // 四つ角の位置を更新
            UpdateCornerPos(pos);

            // 位置を反映(Z軸は固定)
            SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));

#ifdef COLLISION_TEST
            if (m_bUseCollision)
            {
                CDebug::Create(pos + D3DXVECTOR3(0.0f, 0.0f, -50.0f) + BALL_COLLISION_POS_SLIDE, BALL_COLLISION_SIZE, CDebug::TYPE_MOMENT);
            }
#endif // COLLISION_TEST
        }

        // エフェクト発生
        LocusEffect();
        AuraEffect();

        // スケールと向きを変える
        ChangeScaleAndRot();

        // 絶対に当たり判定を使わない偽物のボールなら、終了処理をするかどうか判別
        if (m_bNeverUseCollision)
        {
            if (m_nCntHitWall >= m_nCntHitWallMax)
            {
                CBall::Uninit();
            }
        }
    }
    else
    {
        // 当たり判定を使用しない
        m_bUseCollision = false;

        // ゲーム中なら
        if (CGame::GetState() == CGame::STATE_BUTTLE)
        {
            // ボール配置音
            if (m_nCntDispingTime <= 0)
            {
                CManager::SoundPlay(CSound::LABEL_SE_BALL_DISPING);
            }

            // 出現時間をカウントアップ
            m_nCntDispingTime++;

            // 出現エフェクト
            AppearEffect();

            // 配置時間に達したら、拡大開始
            if (m_nCntDispingTime >= BALL_DISPING_TIME)
            {
                D3DXVECTOR3 scale = GetScale();
                scale.x += BALL_DISPING_SCALE_UP_RATE;
                scale.y += BALL_DISPING_SCALE_UP_RATE;
                scale.z += BALL_DISPING_SCALE_UP_RATE;

                // 大きさが等倍になったら、固定し配置完了
                if (scale.x > 1.0f)
                {
                    m_nCntDispingTime = 0;
                    scale.x = 1.0f;
                    scale.y = 1.0f;
                    scale.z = 1.0f;
                    m_bDisp = true;
                    m_bUseCollision = true;
                    CWave::Create(GetPos(), BALL_WAVE_FIRST_SIZE);
                }

                // スケールを設定
                SetScale(scale);
            }
        }
    }

    // アウトラインを更新
    if (m_pOutline != NULL)
    {
        // 位置を更新
        D3DXVECTOR3 outlinePos = GetPos();
        m_pOutline->SetPos(outlinePos);

        // 色をプレイヤーによって変える
        D3DXCOLOR outlineColor = PLAYER_COLOR_NONE;
        if (m_pPlayer)
        {
            if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
            {
                const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
                if (nWhoColor == PLAYER_1)
                {
                    outlineColor = PLAYER_COLOR_1;
                }
                else if (nWhoColor == PLAYER_2)
                {
                    outlineColor = PLAYER_COLOR_2;
                }
                else if (nWhoColor == PLAYER_3)
                {
                    outlineColor = PLAYER_COLOR_3;
                }
                else if (nWhoColor == PLAYER_4)
                {
                    outlineColor = PLAYER_COLOR_4;
                }
            }
        }

        m_pOutline->SetCol(outlineColor);
    }
}

//=============================================================================
// 四つ角の位置を更新
// Author : 後藤慎之助
//=============================================================================
void CBall::UpdateCornerPos(D3DXVECTOR3 pos)
{
    // 真ん中の座標を出す
    D3DXVECTOR3 centerPos = (pos + m_posOld) / 2 + BALL_COLLISION_POS_SLIDE;

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
// エフェクト放出処理
// Author : 後藤慎之助
//=============================================================================
void CBall::AuraEffect(void)
{
    // 現在の位置を取得
    D3DXVECTOR3 posCurrent = GetPos();

    // 現在の位置から1F前の向きを取得
    float fScatterAngle = atan2((posCurrent.x - m_posOld.x), (posCurrent.y - m_posOld.y));

    // エフェクト生成
    CEffect3D* pEffect = CEffect3D::Create(CEffectData::TYPE_BALL_AURA, posCurrent, fScatterAngle);

    // エフェクトが生成できているなら
    if (pEffect)
    {
        // 色を現在ボールを持っている人に合わせて変える
        D3DXCOLOR col = DEFAULT_COLOR;
        D3DXCOLOR colChangeRate = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        if (m_pPlayer)
        {
            if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
            {
                const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
                if (nWhoColor == PLAYER_1)
                {
                    col = D3DXCOLOR(1.0f, 0.99f, 0.0f, 1.0f);
                    colChangeRate.g = -0.05f;
                }
                else if (nWhoColor == PLAYER_2)
                {
                    col = D3DXCOLOR(0.588f, 0.937f, 1.0f, 1.0f);
                    colChangeRate.g = -0.03f;
                    colChangeRate.r = -0.03f;
                }
                else if (nWhoColor == PLAYER_3)
                {
                    col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
                    colChangeRate.r = -0.05f;
                }
                else if (nWhoColor == PLAYER_4)
                {
                    col = D3DXCOLOR(0.933f, 0.478f, 0.02f, 1.0f);
                    colChangeRate.g = 0.01f;
                }
            }
            else
            {
                col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
                colChangeRate.g = -0.05f;
                colChangeRate.r = -0.025f;
            }
        }
        else
        {
            col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
            colChangeRate.g = -0.05f;
            colChangeRate.r = -0.025f;
        }

        // 色と色変化割合を設定
        pEffect->SetCol(col);
        pEffect->SetColChangeRate(colChangeRate);
    }
}

//=============================================================================
// ボール出現のエフェクト処理
// Author : 後藤慎之助
//=============================================================================
void CBall::AppearEffect(void)
{
    // 現在の位置を取得
    D3DXVECTOR3 posCurrent = GetPos();

    // エフェクト生成
    CEffect3D* pEffect = CEffect3D::Emit(CEffectData::TYPE_BALL_APPEAR, posCurrent, posCurrent);

    // エフェクトが生成できているなら
    if (pEffect)
    {
        // 色を現在ボールを持っている人に合わせて変える
        D3DXCOLOR col = DEFAULT_COLOR;
        D3DXCOLOR colChangeRate = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        if (m_pPlayer)
        {
            if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
            {
                const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
                if (nWhoColor == PLAYER_1)
                {
                    col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
                    colChangeRate.g = -0.05f;
                }
                else if (nWhoColor == PLAYER_2)
                {
                    col = D3DXCOLOR(0.588f, 0.937f, 1.0f, 1.0f);
                    colChangeRate.g = -0.03f;
                    colChangeRate.r = -0.03f;
                }
                else if (nWhoColor == PLAYER_3)
                {
                    col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
                    colChangeRate.r = -0.05f;
                }
                else if (nWhoColor == PLAYER_4)
                {
                    col = D3DXCOLOR(0.933f, 0.478f, 0.02f, 1.0f);
                    colChangeRate.g = 0.01f;
                }
            }
            else
            {
                col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
                colChangeRate.g = -0.05f;
                colChangeRate.r = -0.025f;
            }
        }
        else
        {
            col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
            colChangeRate.g = -0.05f;
            colChangeRate.r = -0.025f;
        }

        // 色を設定
        pEffect->SetCol(col);
        pEffect->SetColChangeRate(colChangeRate);
    }
}

//=============================================================================
// ボールの軌跡の処理
// Author : 伊藤陽梧
//=============================================================================
void CBall::LocusEffect(void)
{
    const float LOCUS_SCALE = 50.0f;    // 軌跡の大きさ
    const float SHRINK_SPEED = 1.0f;    // 軌跡の縮む速さ
    const float LOCUS_POS_Z = -1.0f;    // 軌跡の位置Z（他のエフェクトより手前）
    D3DXVECTOR3 pos = GetPos();

    // ボールの進む角度に合わせて出るようにする
    D3DXVECTOR3 posOld1 = D3DXVECTOR3(m_posOld.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(90.0f))),
        m_posOld.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(90.0f))), LOCUS_POS_Z);
    D3DXVECTOR3 posOld2 = D3DXVECTOR3(m_posOld.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(-90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(-90.0f))),
        m_posOld.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(-90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(-90.0f))), LOCUS_POS_Z);
    D3DXVECTOR3 pos1 = D3DXVECTOR3(pos.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(90.0f))),
        pos.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(90.0f))), LOCUS_POS_Z);
    D3DXVECTOR3 pos2 = D3DXVECTOR3(pos.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(-90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(-90.0f))),
        pos.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(-90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(-90.0f))), LOCUS_POS_Z);

    // 色をプレイヤーによって変える
    D3DXCOLOR locusColor = PLAYER_COLOR_NONE;
    if (m_pPlayer)
    {
        if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
        {
            const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
            if (nWhoColor == PLAYER_1)
            {
                locusColor = PLAYER_COLOR_1;
            }
            else if (nWhoColor == PLAYER_2)
            {
                locusColor = PLAYER_COLOR_2;
            }
            else if (nWhoColor == PLAYER_3)
            {
                locusColor = PLAYER_COLOR_3;
            }
            else if (nWhoColor == PLAYER_4)
            {
                locusColor = PLAYER_COLOR_4;
            }
        }
    }

    // 軌跡の生成
    CLocus::Create(pos1, pos2, posOld1, posOld2, locusColor, CLocus::TYPE_SHRINK, SHRINK_SPEED);
}

//=============================================================================
// スケールと角度の変更処理
// Author : 後藤慎之助
//=============================================================================
void CBall::ChangeScaleAndRot(void)
{
    // スケールと向きを取得
    D3DXVECTOR3 scale = GetScale();
    D3DXVECTOR3 rot = GetRot();

    if (m_nWhoShooting == BALL_NOT_ANYONE || IS_BITON(m_shootFlag, SHOOT_FLAG_THROW) || m_nCntCurrentStopTime > 0)
    {
        // 投げか誰のボールでもない時か止まっているときは通常スケール
        scale.x = 1.0f;
        scale.y = 1.0f;

        // 回転
        rot.x += BALL_ROT_SPEED;
    }
    else
    {
        // ヒットストップが発生するスピード以上なら
        if (m_fSpeed > BALL_SHOOT_BIG_HIT_SPEED)
        {
            // スケールを縦長にする
            scale.x = 0.5f;
            scale.y = 1.5f;

            // 位置から移動角度を取得
            D3DXVECTOR3 pos = GetPos();
            float fMoveX = m_posOld.x - pos.x;
            float fMoveY = m_posOld.y - pos.y;
            float fAngle = atan2((fMoveX), (fMoveY));

            // 移動角度の向きに回転（真横は例外）
            rot.x = sinf(fAngle);
            rot.y = cosf(fAngle);
            if (fabsf(fMoveY) <= BALL_MOVE_Y_DONT_MOVE_BORDER)
            {
                scale.x = 1.5f;
                scale.y = 0.5f;
            }
        }
        else
        {
            // 回転
            rot.x += BALL_ROT_SPEED;
        }
    }

    // 回転制御
    if (rot.x > D3DXToRadian(180.0f))
    {
        rot.x = D3DXToRadian(-180.0f);
    }

    // スケールと向きを反映
    SetScale(scale);
    SetRot(rot);
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CBall::Draw(void)
{
    // 表示しているか、配置中なら描画
    if (m_bDisp || m_nCntDispingTime >= BALL_DISPING_TIME)
    {
        // ステンシルバッファでアウトラインを描画
        if (m_pOutline != NULL)
        {
            // ステンシルバッファを使う
            LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

            // Zテスト有効
            pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
            // Zテストを必ず失敗に
            pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

            // ステンシルテスト有効
            pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
            // ステンシルバッファへ反映する参照値
            pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
            // 参照値マスク
            pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
            // ステンシルテストを必ず成功に
            pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
            // ステンシルテストのテスト設定
            pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
            pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
            pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

            // 通常の描画の前後で、スケールをいじる
            D3DXVECTOR3 memoryScale = GetScale();
            SetScale(GetScale()*BALL_OUTLINE_RATE);
            CScene3D::Draw(false, false);
            SetScale(memoryScale);

            // ステンシルテスト無効
            pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

            // ZBUFFER比較設定変更 => (参照値 <= バッファ値)(戻す)
            pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

            // アウトラインを描画
            m_pOutline->Draw();
        }

        // 通常の描画
        CScene3D::Draw(false, false);
    }
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
    pBall->Init(pos, DEFAULT_VECTOR);

    // メンバ変数を結びつける
    pBall->m_bUseCollision = bUseCollision;

    // 当たり判定を使わないなら、絶対にそれ以降も使わない、かつ配置中の概念はない
    if (!pBall->m_bUseCollision)
    {
        pBall->m_bNeverUseCollision = true;
        pBall->m_bDisp = true;
        pBall->SetScale(DEFAULT_SCALE);
    }

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
        if (m_nCntCurrentStopTime <= 0)
        {
            // 硬直中は、カウンタを加算しない
            m_nCntBuntTime++;
        }
        fSpeed = BALL_BUNT_SPEED;
        fGravity = m_nCntBuntTime * BALL_BUNT_GRAVITY;
    }

    // 硬直時間のカウンタ
    if (m_nCntCurrentStopTime <= 0)
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
    else
    {
        // 集中線を出す硬直時間より上かつ、死亡時のカウントダウンをしていないなら
        if (m_nCntCurrentStopTime > BALL_SHOOT_STOP_REFRECT_FRAME && m_nCntNoLineStopTime <= 0)
        {
            // 集中線
            CEffect3D::Emit(CEffectData::TYPE_SHOOT_ABSORB, pos, pos);
        }

        // 停止時間をカウントダウン
        m_nCntCurrentStopTime--;

        // 当たり判定を使うボールなら
        if (!m_bNeverUseCollision)
        {
            // ボール発射ゲージを更新するシュート時の硬直時間かつ、死亡時のカウントダウンをしていないなら
            if (m_nStopTimeAtShoot > BALL_UPDATE_METER_MIN_STOP_FRAME && m_nCntNoLineStopTime <= 0)
            {
                // ボール発射ゲージを更新
                CGame::SetBallGauge(m_nStopTimeAtShoot, m_nStopTimeAtShoot - m_nCntCurrentStopTime);
            }
            else
            {
                // 放っている最中は、ボール発射ゲージを常に満タンに
                CGame::SetBallGauge(BALL_UPDATE_METER_MIN_STOP_FRAME, BALL_UPDATE_METER_MIN_STOP_FRAME);
            }

            // 最後のフレームで、当たり判定を復活させ、シュート時の硬直時間の記憶をリセット
            if (m_nCntCurrentStopTime == 0)
            {
                m_bUseCollision = true;
                m_nStopTimeAtShoot = 0;
            }
        }
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
            D3DXVECTOR3 targetPos = m_pPlayer->GetPos() + D3DXVECTOR3(0.0f, m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);
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
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(m_pPlayer->GetCollisionSizeDeffence().x * 0.5f + BALL_ABSORB_POS_X,
                    m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);
            }
            else if (m_pPlayer->GetRot().y == PLAYER_ROT_LEFT)
            {
                // 左向きなら
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(-(m_pPlayer->GetCollisionSizeDeffence().x * 0.5f + BALL_ABSORB_POS_X),
                    m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);
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
D3DXVECTOR3 CBall::HitWall(D3DXVECTOR3 pos, HIT_WALL hitWall)
{
    // 移動制限
    D3DXVECTOR3 returnPos = pos;
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    switch (hitWall)
    {
    case HIT_WALL_RIGHT:
        // ウォールスルーするかどうか
        returnPos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.x *= -1;
        }
        else
        {
            // ワープエフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_RIGHT, returnPos);

            m_nCntWallThrough--;
            returnPos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);

            // ワープ先エフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_LEFT, returnPos);

            // ウォールスルーしたなら、前の位置も合わせる
            m_posOld.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
        }
        break;
    case HIT_WALL_LEFT:
        // ウォールスルーするかどうか
        returnPos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.x *= -1;
        }
        else
        {
            // ワープエフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_LEFT, returnPos);

            m_nCntWallThrough--;
            returnPos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);

            // ワープ先エフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_RIGHT, returnPos);

            // ウォールスルーしたなら、前の位置も合わせる
            m_posOld.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
        }
        break;
    case HIT_WALL_UP:
        // ウォールスルーするかどうか
        returnPos.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.y *= -1;
        }
        else
        {
            // ワープエフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_UP, returnPos);

            m_nCntWallThrough--;
            returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;

            // ワープ先エフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_DOWN, returnPos);

            // ウォールスルーしたなら、前の位置も合わせる
            m_posOld.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        }
        break;
    case HIT_WALL_DOWN:
        // ウォールスルーするかどうか
        returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.y *= -1;
        }
        else
        {
            // ワープエフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_DOWN, returnPos);

            m_nCntWallThrough--;
            returnPos.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;

            // ワープ先エフェクト
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_UP, returnPos);

            // ウォールスルーしたなら、前の位置も合わせる
            m_posOld.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        }
        break;
    }

    // メンバ変数の調整
    m_bAfterThrow = false;

    // 壁に当たった回数が関係のある状態なら、カウントアップ
    if (m_bNeverUseCollision)
    {
        m_nCntHitWall++;
    }

    // バントボール状態なら
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // 上壁以外なら
        if (hitWall != HIT_WALL_UP)
        {
            // バントボール状態を解除し、最後に打った人のボールにして、縦移動に
            BITOFF(m_shootFlag, SHOOT_FLAG_BUNT);
            BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);              // 即打ちフラグを解除（打ったことになるため）
            m_nCntBuntTime = 0;
            m_moveAngle = BALL_FIRST_MOVE_ANGLE;
            if (m_pPlayer)
            {
                m_nWhoShooting = m_pPlayer->GetIdxCreate();
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
        // 吸収状態でないなら
        if (m_nWhoAbsorbing == BALL_NOT_ANYONE)
        {
            // バウンド音
            CManager::SoundPlay(CSound::LABEL_SE_BOUND);

            // バントボール以外なら、壁に当たった時にわずかに硬直する
            m_nCntCurrentStopTime = BALL_SHOOT_STOP_REFRECT_FRAME;

            // 壁ヒットエフェクト
            CEffect3D::Emit(CEffectData::TYPE_WALL_HIT_SMOKE, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WALL_HIT_SHOCK, returnPos, m_posOld);
        }
    }

    return returnPos;
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
    if (returnPos.y + (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y > mapLimit.fHeight)
    {
        returnPos.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
    }
    else if (returnPos.y - (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y < 0)
    {
        returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
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
            // 投げ音
            CManager::SoundPlay(CSound::LABEL_SE_THROW);

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
    m_nWhoShooting = m_pPlayer->GetIdxCreate();

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
        // バント音
        CManager::SoundPlay(CSound::LABEL_SE_BUNT);

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

        // 即打ちフラグを解除（打ったことになるため）
        BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);

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
            m_nCntCurrentStopTime = (int)(m_fSpeed * BALL_SHOOT_STOP_TIME_RATE);

            // 引数が即打ちなら
            if (IS_BITON(flag, SHOOT_FLAG_QUICK))
            {
                BITON(m_shootFlag, SHOOT_FLAG_QUICK);
            }

            // メンバ変数が即打ちなら
            bool bQuickShoot = false;
            if (IS_BITON(m_shootFlag, SHOOT_FLAG_QUICK))
            {
                // 即打ちフラグを解除し、硬直時間をまず半分に
                BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);
                m_nCntCurrentStopTime /= 2;
                bQuickShoot = true;

                // 最大硬直時間を調整
                if (m_nCntCurrentStopTime > BALL_QUICK_SHOOT_MAX_STOP_TIME)
                {
                    m_nCntCurrentStopTime = BALL_QUICK_SHOOT_MAX_STOP_TIME;
                }
            }

            // 硬直時間の最大
            if (m_nCntCurrentStopTime > BALL_SHOOT_MAX_STOP_TIME)
            {
                m_nCntCurrentStopTime = BALL_SHOOT_MAX_STOP_TIME;
            }

            // 硬直が発生する速度で、波が発生
            if (m_fSpeed >= BALL_SHOOT_BIG_HIT_SPEED)
            {
                // ウルトラヒットかどうか
                if (m_fSpeed >= BALL_SHOOT_ULTRA_HIT_SPEED)
                {
                    // ウルトラヒットは反転合成（設定で変えれるように）
                    CEffect2D* pNega = CGame::GetNega();
                    if (pNega)
                    {
                        pNega->SetPosTo2D(GetPos());
                        pNega->SetSize(DEFAULT_VECTOR);
                        pNega->SetUseUpdate(true);
                    }
                    CEffect2D* pPosi = CGame::GetPosi();
                    if (pPosi)
                    {
                        pPosi->SetPosTo2D(GetPos());
                        pPosi->SetSize(DEFAULT_VECTOR);
                        pPosi->SetUseUpdate(false);
                        pPosi->SetRotVertex(0.0f);  // 更新を止める代わりに、頂点はここで調整
                    }
                    CManager::SoundPlay(CSound::LABEL_SE_HIT_ULTRA);
                }
                else
                {
                    // 大ヒットは波が発生
                    CWave::Create(GetPos(), BALL_WAVE_FIRST_SIZE);
                    CManager::SoundPlay(CSound::LABEL_SE_HIT_BIG);
                }
            }
            else
            {
                // ヒット音
                CManager::SoundPlay(CSound::LABEL_SE_HIT);
            }

            // 一定速度未満なら、最小硬直時間
            if (m_fSpeed < BALL_SHOOT_BIG_HIT_SPEED)
            {
                m_nCntCurrentStopTime = BALL_SHOOT_STOP_LEAST_FRAME;

                // 早撃ちを考慮
                if (bQuickShoot)
                {
                    m_nCntCurrentStopTime = BALL_SHOOT_STOP_LEAST_FRAME / 2;
                }
            }

            // コントローラの振動
            if (m_pPlayer)
            {
                if (m_pPlayer->GetUseControllerEffect())
                {
                    // 硬直時間の半分揺らす（最低保証あり）
                    int nEffectFrame = m_nCntCurrentStopTime / 2;
                    if (nEffectFrame < BALL_SHOOT_EFFECT_LEAST_FRAME)
                    {
                        nEffectFrame = BALL_SHOOT_EFFECT_LEAST_FRAME;
                    }
                    CManager::GetInputJoypad()->StartEffect(m_pPlayer->GetIdxControlAndColor(), nEffectFrame);
                }
            }

            // ボール発射時の硬直時間を記憶
            m_nStopTimeAtShoot = m_nCntCurrentStopTime;
            if (m_nStopTimeAtShoot > BALL_UPDATE_METER_MIN_STOP_FRAME)
            {
                // ボール発射ゲージを更新
                CGame::SetBallGauge(m_nStopTimeAtShoot, 0);
            }
        }
        else
        {
            // ウルトラヒットなら反転合成を反転していく（元の色に戻していく）（設定で変えれるように）
            if (m_fSpeed >= BALL_SHOOT_ULTRA_HIT_SPEED)
            {
                CEffect2D* pPosi = CGame::GetPosi();
                if (pPosi)
                {
                    pPosi->SetUseUpdate(true);
                }
            }

            // 移動角度と力を結びつける
            m_moveAngle = moveAngle;
            m_fSpeed += fPower;

            // 速度の上限
            if (m_fSpeed > BALL_MAX_SPEED)
            {
                m_fSpeed = BALL_MAX_SPEED;
            }

            // ボールスピード表示を更新
            CGame::GetNumArrayBallSpd()->SetDispNumber((int)m_fSpeed);
        }
    }

    return m_nCntCurrentStopTime;
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
    
    // 必殺技周り
    if (!m_bWallThrough)
    {
        m_nCntWallThrough = 0;
    }
    else
    {
        // 打ち出すときに一度だけ守る感じ
        m_bWallThrough = false;
    }
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

    return m_nCntCurrentStopTime;
}

//=============================================================================
// 硬直時間を決める
// Author : 後藤慎之助
//=============================================================================
void CBall::SetStopTime(const int nCntStopTime)
{
    m_nCntCurrentStopTime = nCntStopTime;

    // 硬直しているなら
    if (m_nCntCurrentStopTime > 0)
    {
        // 当たり判定を一回消す
        m_bUseCollision = false;

        // 集中線を消す
        m_nCntNoLineStopTime = nCntStopTime;
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
    m_nWhoAbsorbing = m_pPlayer->GetIdxCreate();   // 次その吸収したプレイヤーだけがボールに触れるようにするため
    m_nCntAbsorbTime = BALL_ABSORB_MAX_TIME;    // 吸収時間のカウントダウンをスタート
}
