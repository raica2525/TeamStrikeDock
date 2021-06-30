//======================================================================================
//
// プレイヤー処理 (player.cpp)
// Author : 後藤慎之助
//
//======================================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "library.h"
#include "camera.h"
#include "character.h"
#include "debug.h"
#include "animation.h"
#include "game.h"
#include "sound.h"
#include "ball.h"
#include "modelData.h"
#include "ai.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CPlayer::CPlayer() :CCharacter(OBJTYPE::OBJTYPE_PLAYER)
{
    m_controlInput.bTiltedLeftStick = false;
    m_controlInput.fLeftStickAngle = 0.0f;
    m_controlInput.bTriggerA = false;
    m_controlInput.bPressA = false;
    m_controlInput.bTriggerX = false;
    m_controlInput.bPressX = false;
    m_controlInput.bReleaseX = false;
    m_controlInput.bTriggerY = false;
    m_controlInput.bTriggerB = false;
    m_AIlevel = AI_LEVEL_NONE;
    m_pAI = NULL;

    m_playable = PLAYABLE_001;
    m_nPoint = 0;
    m_move = DEFAULT_VECTOR;
    m_exFlag = EX_FLAG_NONE;
    m_attackState = ATTACK_STATE_NONE;

    m_nCntAttackTime = 0;
    m_bGround = true;

    m_bDisp = true;
    m_nCntTakeDamageTime = 0;
    m_fLife = 0.0f;
    m_bUsedLuckyGuard = false;
    m_damageState = DAMAGE_STATE_NONE;

    m_nCntLandingTime = 0;
    m_bGroundOld = true;

    m_fAtk = 0.0f;
    m_fDef = 0.0f;
    m_fSpd = 0.0f;
    m_fWei = 0.0f;
    m_nCharSize = CHAR_SIZE_MEDIUM;
    memset(m_afParam, 0, sizeof(m_afParam));
    m_nModelPosDefUp = 0;
    m_nModelPosDefDown = 0;

    m_nCntTurnTime = 0;
    m_nCntPressJump = 0;
    m_bDiveCheck = false;
    m_bDiving = false;
    m_bUsedSecondJump = false;
    m_bSquat = false;
    m_nCntStartGlide = 0;

    m_nCntSwingCharge = 0;
    m_bUseAvoidMultipleHits = false;
    m_bHitAttackThisFrame = false;
    m_nCntStopTime = 0;
    m_nCntAttackAnimTime = 0;

    m_spShot = SP_SHOT_HEAL;
    m_fSpGaugeCurrent = 0.0f;
    m_fSpGaugeMax = 0.0f;

    m_bUsedThreeJump = false;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // パーツ数を設定
    CCharacter::SetPartNum(PARTS_MAX);

    // プレイアブルキャラの設定を読み込む
    LoadCustom();

    // 座標・親子関係の読み込み
    //CCharacter::LoadModelData("./data/ANIMATION/motion_player.txt");  // カスタマイズごとの初期位置に対応
    CCharacter::SetAnimFilePass("./data/ANIMATION/motion_player.txt");
    CCharacter::LoadModelData(m_nModelPosDefUp, m_nModelPosDefDown);

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// カスタマイズ読み込み処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::LoadCustom(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // パーツリストから情報取得
    CModelData *pModelData = CManager::GetModelData();

    // 変数宣言
    int nPartsListType = 0;                // パーツリスト内の種類
    int nExNumber = 0;                     // 特殊能力番号

    // 定義
    const int PARTS_LIST_HEAD = 0;         // パーツリスト_頭
    const int PARTS_LIST_UP = 1;           // パーツリスト_上半身
    const int PARTS_LIST_DOWN = 2;         // パーツリスト_下半身
    const int PARTS_LIST_WEP = 3;          // パーツリスト_武器

    // 反映させるメンバ変数を一回初期化
    m_fAtk = 0.0f;
    m_fDef = 0.0f;
    m_fSpd = 0.0f;
    m_fWei = 0.0f;
    m_exFlag = EX_FLAG_NONE;
    m_nCharSize = CHAR_SIZE_SMALL;
    memset(m_afParam, 0, sizeof(m_afParam));

    // カスタマイズデータのファイルを開く
    switch (m_playable)
    {
    case CPlayer::PLAYABLE_001:
        pFile = fopen("data/TXT/custom1.txt", "r");
        break;
    case CPlayer::PLAYABLE_002:
        pFile = fopen("data/TXT/custom2.txt", "r");
        break;
    case CPlayer::PLAYABLE_003:
        pFile = fopen("data/TXT/custom3.txt", "r");
        break;
    case CPlayer::PLAYABLE_004:
        pFile = fopen("data/TXT/custom4.txt", "r");
        break;
    }

    // 開けたら
    if (pFile != NULL)
    {
        for (int nCntPartsList = 0; nCntPartsList < MAX_PARTS; nCntPartsList++)
        {
            // 読み込み
            fscanf(pFile, "%d", &nPartsListType);

            // 基本4能力
            m_fAtk += pModelData->GetPartsList(nPartsListType)->fAtk * pModelData->GetPartsRate(nCntPartsList)->fAtkRate;
            m_fDef += pModelData->GetPartsList(nPartsListType)->fDef * pModelData->GetPartsRate(nCntPartsList)->fDefRate;
            m_fSpd += pModelData->GetPartsList(nPartsListType)->fSpd * pModelData->GetPartsRate(nCntPartsList)->fSpdRate;
            m_fWei += pModelData->GetPartsList(nPartsListType)->fWei * pModelData->GetPartsRate(nCntPartsList)->fWeiRate;

            // 特殊能力
            nExNumber = pModelData->GetPartsList(nPartsListType)->nEx;
            BITON(m_exFlag, 0x001<<nExNumber);

            // 各パーツリストから、細部のパーツを決定
            if (nCntPartsList == PARTS_LIST_HEAD)
            {
                BindParts(PARTS_HEAD, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);

                m_afParam[PARAM_SWING_UP] = pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_afParam[PARAM_SWING_DOWN] = pModelData->GetPartsList(nPartsListType)->afParam[2];
                m_afParam[PARAM_SMASH] = pModelData->GetPartsList(nPartsListType)->afParam[3];
                m_afParam[PARAM_SPIKE_RIGHT] = pModelData->GetPartsList(nPartsListType)->afParam[4];
                m_afParam[PARAM_SPIKE_LEFT] = pModelData->GetPartsList(nPartsListType)->afParam[5];
            }
            else if (nCntPartsList == PARTS_LIST_UP)
            {
                BindParts(PARTS_BODY, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                BindParts(PARTS_LHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);

                // 一度撤廃
                //m_nCharSize = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];

                // 上半身の初期位置データ
                m_nModelPosDefUp = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];
            }
            else if (nCntPartsList == PARTS_LIST_DOWN)
            {
                BindParts(PARTS_HIP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                BindParts(PARTS_LFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);

                // 下半身の初期位置データ
                m_nModelPosDefDown = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];
            }
            else if (nCntPartsList == PARTS_LIST_WEP)
            {
                BindParts(PARTS_WEP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);

                // 必殺技と、そのゲージ量
                m_spShot = (int)pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_fSpGaugeMax = pModelData->GetPartsList(nPartsListType)->afParam[2];
            }
        }
        // ファイルを閉じる
        fclose(pFile);
    }

    // 体力を決定
    m_fLife = m_fDef;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Uninit(void)
{
    // AI情報をメモリ開放
    if (m_pAI != NULL)
    {
        // メモリ開放
        delete m_pAI;
        m_pAI = NULL;
    }

    CCharacter::Uninit();
}

//=============================================================================
// 入力処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Input(void)
{
    // コントローラを取得（1Pの場合、Enterで遷移したならキーボード操作に変える）
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(m_playable);

    // ボタン情報を取得
    m_controlInput.bTriggerA = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_A);
    m_controlInput.bPressA = pInputJoypad->GetJoypadPress(m_playable, CInputJoypad::BUTTON_A);
    m_controlInput.bTriggerX = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_X);
    m_controlInput.bPressX = pInputJoypad->GetJoypadPress(m_playable, CInputJoypad::BUTTON_X);
    m_controlInput.bReleaseX = pInputJoypad->GetJoypadRelease(m_playable, CInputJoypad::BUTTON_X);
    m_controlInput.bTriggerY = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_Y);
    m_controlInput.bTriggerB = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_B);

    // 左スティックが傾いているかどうか
    if (Controller.lY != 0 || Controller.lX != 0)
    {
        m_controlInput.bTiltedLeftStick = true;

        // 角度を求める
        m_controlInput.fLeftStickAngle = atan2(Controller.lX, Controller.lY*-1);
    }
    else
    {
        m_controlInput.bTiltedLeftStick = false;
    }
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Update(void)
{
    // このフレーム中に攻撃を当てたかどうかを初期化
    m_bHitAttackThisFrame = false;

    // 表示しているなら
    if (m_bDisp)
    {
        // AIでないなら
        if (!m_pAI)
        {
            // 入力処理
            Input();
        }
        else
        {
            // AI更新処理
            m_pAI->Update();
        }

        // 硬直していないなら
        if(m_nCntStopTime <= 0)
        {
            // 負傷時間があるなら、カウンタを下げる
            if (m_nCntTakeDamageTime > 0)
            {
                m_nCntTakeDamageTime--;

                // 負傷終了時に状態を戻す
                if (m_nCntTakeDamageTime == 0)
                {
                    // ダウンからの起き上がり以外は、ダメージ状態を解除
                    if (m_damageState != DAMAGE_STATE_BIG)
                    {
                        m_damageState = DAMAGE_STATE_NONE;
                    }
                    else
                    {
                        m_damageState = DAMAGE_STATE_STAND_UP;
                        m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_STAND_UP_FRAME;
                    }
                }
            }

            // 攻撃のクールタイム
            if (m_nCntAttackTime > 0)
            {
                // 攻撃中はカウントダウン
                m_nCntAttackTime--;

                // 攻撃終了時にメンバ変数を戻す
                if (m_nCntAttackTime == 0)
                {
                    ResetAttack();
                }
            }

            // アタックアニメーションカウンタ
            if (m_nCntAttackAnimTime > 0)
            {
                // カウントダウン
                m_nCntAttackAnimTime--;
            }

            // 着地しているなら、色々フラグをリセット
            if (m_bGround)
            {
                ResetOnGround();
            }

            // 行動
            float fSpeed = PLAYER_BASE_MINIMUM_SPEED + (m_fSpd * PLAYER_ADD_BASE_SPEED_RATE); // 最低保証の速度に、パラメータ分を足す
            Movement(fSpeed);

            // アニメーションさせる
            CCharacter::Update();

            //// ライフがなくなったら、非表示に
            //if (GetLife() <= 0)
            //{
            //    m_bDisp = false;
            //}
        }
        else
        {
            // 硬直時間をカウント
            m_nCntStopTime--;

            // 必殺技処理（ゲージ満タン時にXボタン押したら）
            if (m_fSpGaugeCurrent >= m_fSpGaugeMax)
            {
                if (m_controlInput.bTriggerX)
                {
                    EmitSpShot();
                }
            }

            // ここでボールを放つ
            if (m_nCntStopTime == 0)
            {
                // 位置を取得
                D3DXVECTOR3 pos = DEFAULT_VECTOR;
                pos = GetPos();

                // 最初の接触ではないためfalse
                AttackUpdate(pos, false);
            }

            // アタックアニメーションカウンタ（変なポーズで止まらないための最低保証）
            if (m_nCntAttackAnimTime > 0)
            {
                // アニメーションカウントダウン
                m_nCntAttackAnimTime--;

                // 攻撃時間もカウントダウン
                m_nCntAttackTime--;

                // 攻撃終了時にメンバ変数を戻す（念のため）
                if (m_nCntAttackTime == 0)
                {
                    ResetAttack();
                }

                // 攻撃モーションにするかどうか
                AttackMotion();

                // アニメーションの最低保証
                CCharacter::Update();
            }
        }
    }
    // 体力がないなら
    else
    {
        //// ゲームが通常なら
        //if (CGame::GetState() == CGame::STATE_NORMAL)
        //{
        //    // やられた時のエフェクト
        //    //DeathCrush(GetPos(), GetRot());

        //    // ゲームの状態を終わりにする
        //    CGame::SetState(CGame::STATE_FINISH);
        //}
    }
}

//=============================================================================
// 着地中にリセットする変数たち
// Author : 後藤慎之助
//=============================================================================
void CPlayer::ResetOnGround(void)
{
    m_nCntPressJump = 0;
    m_bDiveCheck = true;
    m_bDiving = false;
    m_bUsedSecondJump = false;
    m_bUsedThreeJump = false;
    m_nCntStartGlide = 0;
}

//=============================================================================
// 攻撃周りでリセットする変数たち
// Author : 後藤慎之助
//=============================================================================
void CPlayer::ResetAttack(void)
{
    m_attackState = ATTACK_STATE_NONE;
    m_nCntSwingCharge = 0;
    m_bUseAvoidMultipleHits = false;
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Draw(void)
{
    // 表示するなら、描画
    if (m_bDisp)
    {
        CCharacter::Draw();
    }
}

//=============================================================================
// インスタンス生成
// Author : 後藤慎之助
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int playable, AI_LEVEL AIlevel)
{
    // メモリ確保
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // 親元の情報を設定
    pPlayer->SetPos(pos);
    pPlayer->SetRot(rot);
    pPlayer->SetTribe(CCharacter::TRIBE_PLAYER);
    pPlayer->SetActiveCollisionSize(PLAYER_COLLISION_SIZE);

    // 読み込む種類の設定
    pPlayer->m_playable = playable;

    // 初期化
    pPlayer->Init(pos, DEFAULT_SCALE);

    // 結びつけるメンバ変数の初期化
    pPlayer->m_AIlevel = AIlevel;
    if (pPlayer->m_AIlevel != AI_LEVEL_NONE)
    {
        pPlayer->m_pAI = CAi::Create(pPlayer);
    }

    // ゲームのプレイヤー人数を増やす
    CGame::SetAddNumCurrentPlayer();

    return pPlayer;
}

//=============================================================================
// 行動
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Movement(float fSpeed)
{
    // 位置を取得
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    pos = GetPos();

    // 1F前の位置を記憶
    SetPosOld(pos);

    // 横の移動量制御
    if (m_move.x != 0.0f)
    {
        // 制御の割合を、状況によって変える
        float fControlMoveRate = PLAYER_CONTROL_MOVE_ON_GROUND;

        // 負傷していないなら
        if (m_damageState == DAMAGE_STATE_NONE)
        {
            // 空中なら
            if (!m_bGround)
            {
                fControlMoveRate = PLAYER_CONTROL_MOVE_IN_AIR;
            }
        }
        else
        {
            fControlMoveRate = PLAYER_CONTROL_MOVE_TAKE_DAMAGE;
        }

        // しゃがんでいるなら
        if (m_bSquat)
        {
            fControlMoveRate = PLAYER_CONTROL_MOVE_SQUAT;
        }

        // 移動量制御
        m_move.x *= fControlMoveRate;
    }

    // モーションをまずは待機にする
    GetAnimation()->SetAnimation(ANIM_IDLE);

    // しゃがみ状態をリセット
    m_bSquat = false;

    // プレイヤー移動処理
    Control(fSpeed);

    // 移動量と位置を結びつける
    pos += m_move;

    // 地面より上なら
    if (pos.y > 0.0f)
    {
        // 重力
        float fGravity = PLAYER_BASE_MINIMUM_GRAVITY + (m_fWei * PLAYER_ADD_BASE_GRAVITY_RATE);
        if (m_damageState == DAMAGE_STATE_BLOWN)
        {
            // 吹っ飛ばされ中は固定値
            fGravity = PLAYER_TAKE_DAMAGE_BLOWN_GRAVITY;
        }
        m_move.y -= fGravity;

        // 地面にいない
        m_bGround = false;
    }
    else
    {
        // 地面にいるなら、重力をかけないでジャンプ可能、急降下のチェックも復活、二段ジャンプも復活
        pos.y = 0.0f;
        m_move.y = 0.0f;
        m_bGround = true;
    }

    // マップ制限
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    if (pos.x - GetActiveCollisionSize().x < -mapLimit.fWidth)
    {
        pos.x = -mapLimit.fWidth + GetActiveCollisionSize().x;
    }
    if (pos.x + GetActiveCollisionSize().x > mapLimit.fWidth)
    {
        pos.x = mapLimit.fWidth - GetActiveCollisionSize().x;
    }
    if (pos.y + GetActiveCollisionSize().y > mapLimit.fHeight)
    {
        pos.y = mapLimit.fHeight - GetActiveCollisionSize().y;
    }
    
    // ジャンプ
    Jump();

    // 移動モーションの管理
    MoveMotion();

    //====================================================
    // 攻撃処理
    // 攻撃がヒットしていないなら、攻撃の判定等を更新
    if (!m_bUseAvoidMultipleHits)
    {
        AttackUpdate(pos, true); // bFirestCollisionのtrueは、ここが最初の接触になりうるから
    }

    // 攻撃の生成
    AttackGenerator(pos);

    // 攻撃モーションにするかどうか
    AttackMotion();
    //====================================================

    //====================================================
    // 防御処理
    // 負傷中の更新
    DamageUpdate(pos);

    // このフレームに攻撃がヒットしていないかつ、投げ状態以外かつ、負傷していないor起き上がり中に、ボールが当たったかどうか
    if (!m_bHitAttackThisFrame && m_attackState != ATTACK_STATE_THROW)
    {
        if (m_damageState == DAMAGE_STATE_NONE || m_damageState == DAMAGE_STATE_STAND_UP && m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
        {
            CollisionBall(pos);
        }
    }

    // 負傷モーションにするかどうか
    DamageMotion();
    //====================================================

    // 回転制御
    // 地面にいて、負傷していない状態かつ、攻撃状態がなしかチャージなら
    if (m_bGround)
    {
        if (m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_NONE ||
            m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_SWING_CHARGE)
        {
            RotControl();
        }
    }

    // 位置を反映
    SetPos(pos);

    // 1F前、ジャンプできたかどうかを記録
    m_bGroundOld = m_bGround;

#ifdef COLLISION_TEST
    if (!m_bHitAttackThisFrame && m_attackState != ATTACK_STATE_THROW)
    {
        if (m_damageState == DAMAGE_STATE_NONE || m_damageState == DAMAGE_STATE_STAND_UP&& m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
        {
            D3DXVECTOR3 size = GetActiveCollisionSize();
            if (m_bSquat)
            {
                size.y *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
            }
            CDebug::Create(pos, size, CDebug::TYPE_MOMENT);
        }
    }
#endif // COLLISION_TEST
}

//=============================================================================
// 移動系モーションの管理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::MoveMotion(void)
{
    // 着地モーションにするかどうか
    if (!m_bGroundOld && m_bGround)
    {
        // 着地時間を設定
        m_nCntLandingTime = PLAYER_LANDING_TIME;
    }
    if (m_nCntLandingTime > 0)
    {
        // カウントダウン
        m_nCntLandingTime--;

        // モーションを着地にする
        GetAnimation()->SetAnimation(ANIM_LANDING);
    }

    // しゃがみモーションにするかどうか
    if (m_bSquat)
    {
        GetAnimation()->SetAnimation(ANIM_SQUAT);
    }

    // 滑空のカウンタ
    if (m_nCntStartGlide > 0)
    {
        m_nCntStartGlide--;
    }

    // ジャンプモーションにするかどうか
    if (!m_bGround)
    {
        // 滑空アニメーションのカウンタが0以下なら滑空
        if (m_nCntStartGlide <= 0)
        {
            GetAnimation()->SetAnimation(ANIM_GLIDE);
        }
        else
        {
            // どのジャンプにするか
            if (m_bUsedThreeJump)
            {
                GetAnimation()->SetAnimation(ANIM_THIRD_JUMP);
            }
            else
            {
                if (m_bUsedSecondJump)
                {
                    GetAnimation()->SetAnimation(ANIM_SECOND_JUMP);
                }
                else
                {
                    GetAnimation()->SetAnimation(ANIM_JUMP);
                }
            }
        }
    }
}

//=============================================================================
// ボールに当たったかどうか
// Author : 後藤慎之助
//=============================================================================
void CPlayer::CollisionBall(D3DXVECTOR3 playerPos)
{
    // ボールとの当たり判定
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BALL);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BALL); nCntScene++)
    {
        // 中身があるなら
        if (pScene != NULL)
        {
            // 次のシーンを記憶
            CScene*pNextScene = pScene->GetNextScene();

            // ボールにキャスト
            CBall *pBall = (CBall*)pScene;

            // 当たり判定を使用するなら
            if (pBall->GetUseCollision())
            {
                // 位置を取得
                D3DXVECTOR3 ballPos = pBall->GetPos();

                // 自分以外の誰かが撃ったなら
                if (pBall->GetWhoShooting() != m_playable && pBall->GetWhoShooting() != BALL_NOT_ANYONE)
                {
                    // 自分の当たり判定の大きさを決定
                    D3DXVECTOR3 playerSize = GetActiveCollisionSize();

                    // しゃがんでいるなら縦の当たり判定を削る
                    if (m_bSquat)
                    {
                        playerSize.y *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
                    }

                    // 当たっているなら
                    if (IsCollisionToRotationRect(playerPos, playerSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&playerPos, &pBall->GetPos(),
                            &playerSize, &BALL_COLLISION_SIZE))
                    {
                        // ダメージを計算（ラッキーガード発生可能性あり）
                        float fDamage = pBall->GetSpeed() * PLAYER_TAKE_DAMAGE_RATE_FROM_BALL;
                        TakeDamage(fDamage, pBall->GetWhoShooting(), pBall->GetPosOld(), true);
                    }
                }
            }

            // 次のシーンにする
            pScene = pNextScene;
        }
        else
        {
            // 中身がないなら、そこで処理を終える
            break;
        }
    }
}

//=============================================================================
// ダメージを受ける
// Author : 後藤慎之助
//=============================================================================
void CPlayer::TakeDamage(float fDamage, int nWho, D3DXVECTOR3 damageOldPos, bool bUseLuckyGuard)
{
    // 攻撃状態をリセット
    ResetAttack();

    // ラッキーガードを使う攻撃を受けたら
    D3DXVECTOR3 playerPos = GetPos();       // 位置を取得
    float fKnockbackValue = 0.0f;           // ノックバック量
    bool bUseLuckyGuardThisFrame = false;   // このフレーム中にラッキーガードが成立したかどうか
    if (bUseLuckyGuard)
    {
        // ラッキーガードを使っていないなら
        if (!m_bUsedLuckyGuard)
        {
            // 乱数の結果で、ラッキーガード
            int nRandNum = GetRandNum(PLAYER_LUCKY_GUARD_MAX, 1);
            if (nRandNum <= (int)m_fDef)
            {
                bUseLuckyGuardThisFrame = true;
                m_bUsedLuckyGuard = true;
                fDamage = 1.0f;                                         // 1ダメージは受ける
                fKnockbackValue = PLAYER_KNOCK_BACK_LUCKY_GUARD;        // 専用のノックバック量
                m_nCntStopTime = CGame::GetBall()->Launch(playerPos + D3DXVECTOR3(0.0f, GetActiveCollisionSize().y / 2, 0.0f),
                    BALL_SHOOT_STOP_LUCKY_GUARD_FRAME);   // ボールを打ち上げる
            }
        }
    }

    // ダメージやボールとの位置関係によって、どの負傷状態かを設定
    D3DXVECTOR3 playerRot = GetRot();   // 向きを取得
    float fJudgmentDamage = fDamage;    // 最終的にやられモーションがどうなるかの判断に用いるダメージ

    // 背中向きなら、判定に使うダメージが2倍
    if (playerPos.x < damageOldPos.x)
    {
        if (playerRot.y == PLAYER_ROT_LEFT)
        {
            fJudgmentDamage *= 2.0f;
        }
    }
    else
    {
        if (playerRot.y == PLAYER_ROT_RIGHT)
        {
            fJudgmentDamage *= 2.0f;
        }
    }

    // 負傷状態の判定
    if (fJudgmentDamage < PLAYER_TAKE_DAMAGE_BORDER_DAMAGE)
    {
        m_damageState = DAMAGE_STATE_SMALL;

        // このフレーム中にラッキーガード以外で
        if (!bUseLuckyGuardThisFrame)
        {
            // 小やられ時間
            m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_SMALL_FRAME;

            // 空中では少しのけぞり
            if (!m_bGround)
            {
                fKnockbackValue = PLAYER_KNOCK_BACK_SMALL_IN_AIR;
            }
        }
        else
        {
            // ラッキーガードなら、少し長い時間
            m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_LUCKY_GUARD_FRAME;
        }
    }
    else
    {
        // 吹っ飛ばされ状態は、移動量がなくなって地面につくまで続く
        m_damageState = DAMAGE_STATE_BLOWN;
        fKnockbackValue = PLAYER_KNOCK_BACK_BIG;
    }

    // 攻撃の方を向き、ノックバックさせる
    if (playerPos.x < damageOldPos.x)
    {
        SetRot(D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f));
        m_move.x = -fKnockbackValue;
    }
    else
    {
        SetRot(D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f));
        m_move.x = fKnockbackValue;
    }

    // ダメージを受ける
    m_fLife -= fDamage;

    // 体力がなくなったら
    if (m_fLife <= 0.0f)
    {
        // 非表示に
        m_fLife = 0.0f;
        m_bDisp = false;

        // ボールにやられたなら、ボールを少しだけ止める（ボールでしかラッキーガードは発動しないため代用）
        if (bUseLuckyGuard)
        {
            CGame::GetBall()->SetStopTime(BALL_SHOOT_STOP_DEATH_FRAME);
        }

        // 最後に攻撃した人にポイントが入る
        if (nWho >= PLAYABLE_001 && nWho < CGame::GetNumCurrentPlayer())
        {
            CGame::GetPlayer(nWho)->SetAddPoint();
        }
    }
}

//=============================================================================
// プレイヤーの移動制御
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Control(float fSpeed)
{
    // 向き
    D3DXVECTOR3 rot = GetRot();

    // 地上で攻撃中以外なら（主に地上スイングでは動けない）
    if (!m_bGround || m_attackState == ATTACK_STATE_NONE)
    {
        // 振り向き時間を数える
        if (m_nCntTurnTime > PLAYER_TURN_FRAME)
        {
            m_nCntTurnTime = 0;
        }

        // 負傷していない状態かつ、着地中でもないなら
        if (m_damageState == DAMAGE_STATE_NONE && m_nCntLandingTime <= 0)
        {
            // スティックが倒れているなら移動
            if (m_controlInput.bTiltedLeftStick)
            {
                // 右移動
                if (STICK_RIGHT(m_controlInput.fLeftStickAngle))
                {
                    if (m_nCntTurnTime > 0)
                    {
                        m_nCntTurnTime++;

                        // もし振り向き中にまた振り向いたら、カウントを1から
                        if (rot.y == PLAYER_ROT_LEFT)
                        {
                            m_nCntTurnTime = 1;
                        }
                    }
                    else
                    {
                        if (m_bGround)
                        {
                            // 向いている方向に進む
                            if (rot.y == PLAYER_ROT_RIGHT)
                            {
                                m_move.x = fSpeed;

                                // モーションを歩きにする
                                GetAnimation()->SetAnimation(ANIM_MOVE);
                            }
                            else
                            {
                                // 方向転換開始
                                m_nCntTurnTime++;
                            }
                        }
                        else
                        {
                            // 空中移動
                            m_move.x += fSpeed / PLAYER_TURN_FRAME;
                        }
                    }
                }
                else if (STICK_LEFT(m_controlInput.fLeftStickAngle))
                {
                    if (m_nCntTurnTime > 0)
                    {
                        m_nCntTurnTime++;

                        // もし振り向き中にまた振り向いたら、カウントを1から
                        if (rot.y == PLAYER_ROT_RIGHT)
                        {
                            m_nCntTurnTime = 1;
                        }
                    }
                    else
                    {
                        if (m_bGround)
                        {
                            // 向いている方向に進む
                            if (rot.y == PLAYER_ROT_LEFT)
                            {
                                m_move.x = -fSpeed;

                                // モーションを歩きにする
                                GetAnimation()->SetAnimation(ANIM_MOVE);
                            }
                            else
                            {
                                // 方向転換開始
                                m_nCntTurnTime++;
                            }
                        }
                        else
                        {
                            // 空中移動
                            m_move.x -= fSpeed / PLAYER_TURN_FRAME;
                        }
                    }
                }
                else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
                {
                    // 地上なら、しゃがみ
                    if (m_bGround)
                    {
                        m_bSquat = true;
                    }
                }
            }

            // 最大速度を超えないようにする
            if (m_move.x > fSpeed)
            {
                m_move.x = fSpeed;
            }
            else if (m_move.x < -fSpeed)
            {
                m_move.x = -fSpeed;
            }
        }
    }
}

//=============================================================================
// ジャンプの処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Jump(void)
{
    // 投げ中以外で
    if (m_attackState != ATTACK_STATE_THROW)
    {
        // Aプレス
        if (m_controlInput.bPressA)
        {
            // 負傷していない状態なら（攻撃中でも距離は伸ばせる）
            if (m_damageState == DAMAGE_STATE_NONE)
            {
                // ジャンプのカウンタが0より大きく、加算カウンタ中なら
                if (m_nCntPressJump > 0 && m_nCntPressJump <= PLAYER_JUMP_ADD_FRAME)
                {
                    // ジャンプのカウンタ加算
                    m_nCntPressJump++;

                    // ジャンプ量加算
                    m_move.y += PLAYER_BASE_MINIMUM_GRAVITY + (m_fWei * PLAYER_ADD_BASE_GRAVITY_RATE);
                }
                else if(m_nCntPressJump > PLAYER_JUMP_ADD_FRAME && m_nCntPressJump <= PLAYER_JUMP_KEEP_FRAME)
                {
                    // 滞空
                    if (m_move.y <= 0.0f)
                    {
                        // ジャンプのカウンタ加算
                        m_nCntPressJump++;

                        m_move.y = 0.0f;
                    }
                }
            }
        }
        else
        {
            // ジャンプのカウンタをリセット
            m_nCntPressJump = 0;
        }

        // Aトリガー（ジャンプ生成のイメージ）
        if (m_controlInput.bTriggerA)
        {
            // 負傷していない状態かつ、攻撃状態がなしかチャージなら
            if (m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_NONE ||
                m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_SWING_CHARGE)
            {
                // 地面にいるなら
                if (m_bGround)
                {
                    // ジャンプの初期量
                    m_move.y = PLAYER_JUMP_FIRST_RATE;

                    // 滑空までのカウンタを設定
                    m_nCntStartGlide = PLAYER_FIRST_JUMP_MOTION_TO_GLIDE;

                    // 地面にいないことに
                    m_bGround = false;

                    // 念のため着地時間をリセット
                    m_nCntLandingTime = 0;

                    // ジャンプのカウンタ加算
                    m_nCntPressJump++;
                }
                else
                {
                    // 多段ジャンプは、チャージ中にできない
                    if (m_nCntSwingCharge <= 0)
                    {
                        // 特殊能力:三段ジャンプ
                        if (IS_BITON(m_exFlag, EX_FLAG_THREE_JUMP))
                        {
                            // 地面にいなく、三段ジャンプをしていなく、二段ジャンプ後なら
                            if (!m_bUsedThreeJump && m_bUsedSecondJump)
                            {
                                // 1Fだけ向きを変えることができる
                                RotControl();

                                // ジャンプの初期量
                                m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                                // 滑空までのカウンタを設定
                                m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                                // 三段ジャンプを使ったフラグをtrueに
                                m_bUsedThreeJump = true;

                                // 念のため着地時間をリセット
                                m_nCntLandingTime = 0;

                                // ジャンプのカウンタ加算
                                m_nCntPressJump++;

                                // 急降下中なら急降下をリセット
                                if (m_bDiving)
                                {
                                    m_bDiving = false;
                                    m_bDiveCheck = true;
                                }
                            }
                        }

                        // 地面にいなく、二段ジャンプをしていないなら
                        if (!m_bUsedSecondJump)
                        {
                            // 1Fだけ向きを変えることができる
                            RotControl();

                            // ジャンプの初期量
                            m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                            // 滑空までのカウンタを設定
                            m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                            // 二段ジャンプを使ったフラグをtrueに
                            m_bUsedSecondJump = true;

                            // 念のため着地時間をリセット
                            m_nCntLandingTime = 0;

                            // ジャンプのカウンタ加算
                            m_nCntPressJump++;

                            // 急降下中なら急降下をリセット
                            if (m_bDiving)
                            {
                                m_bDiving = false;
                                m_bDiveCheck = true;
                            }
                        }
                    }
                }
            }
        }

        // 急降下の判定が有効なら
        if (m_bDiveCheck)
        {
            // 下入力で、急降下の成功/失敗判定
            if (STICK_DOWN(m_controlInput.fLeftStickAngle))
            {
                // ジャンプを押していないなら成功
                if (m_nCntPressJump <= 0)
                {
                    m_bDiving = true;
                }
                else
                {
                    m_bDiveCheck = false;
                }
            }
        }

        // 急降下中なら
        if (m_bDiving)
        {
            m_move.y -= (PLAYER_BASE_MINIMUM_GRAVITY + (m_fWei * PLAYER_ADD_BASE_GRAVITY_RATE))* PLAYER_DIVE_SPEED_RATE;
        }
    }
}

//=============================================================================
// 攻撃更新処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::AttackUpdate(D3DXVECTOR3 pos, bool bFirstCollision)
{
    // 負傷していない状態かつ、攻撃中なら
    if (m_damageState == DAMAGE_STATE_NONE
        && m_attackState != ATTACK_STATE_NONE)
    {
        // 種類によって条件分け
        int nCntAttackEndFrame = 0; // 攻撃が終わるフレーム数
        switch (m_attackState)
        {
            // スイングのチャージ
        case ATTACK_STATE_SWING_CHARGE:
            SwingCharge();
            break;

            // スイング
        case ATTACK_STATE_SWING:
            nCntAttackEndFrame = ATTACK_SWING_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Swing(pos, bFirstCollision);
            }
            break;

            // スマッシュ
        case ATTACK_STATE_SMASH:
            nCntAttackEndFrame = ATTACK_SMASH_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Smash(pos, bFirstCollision);
            }
            break;

            // スパイク
        case ATTACK_STATE_SPIKE:
            nCntAttackEndFrame = ATTACK_SPIKE_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Spike(pos, bFirstCollision);
            }
            break;

            // キャッチの構え
        case ATTACK_STATE_CATCH_READY:
            nCntAttackEndFrame = ATTACK_CATCH_READY_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                CatchReady(pos);
            }
            break;

            // バント
        case ATTACK_STATE_BUNT:
            nCntAttackEndFrame = ATTACK_BUNT_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Bunt(pos);
            }
            break;

            // 吸収
        case ATTACK_STATE_ABSORB:
            nCntAttackEndFrame = ATTACK_ABSORB_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Absorb(pos);
            }
            break;
        }
    }
}

//=============================================================================
// 攻撃発生処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::AttackGenerator(D3DXVECTOR3 pos)
{
    // 負傷していない状態かつ、攻撃中でないなら
    if (m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_NONE)
    {
        // 地上にいるなら
        if (m_bGround)
        {
            // 通常攻撃ボタンを押したら、スイングのみ
            if (m_controlInput.bTriggerX)
            {
                // スイングのチャージ状態（当たっているならチャージなしで撃つ）
                m_attackState = ATTACK_STATE_SWING_CHARGE;
                Swing(pos, true);
            }
            else if (m_controlInput.bTriggerY)
            {
                // キャッチの構え
                m_attackState = ATTACK_STATE_CATCH_READY;
                m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                CatchReady(pos);
            }
            else if (m_controlInput.bTriggerB)
            {
                // バント
                m_attackState = ATTACK_STATE_BUNT;
                m_nCntAttackTime = ATTACK_BUNT_WHOLE_FRAME;
                Bunt(pos);
            }
        }
        else
        {
            // 通常攻撃ボタンを押したら
            if (m_controlInput.bTriggerX)
            {
                // 空中にいるなら、スティックの角度によって技が異なる
                if (m_controlInput.bTiltedLeftStick)
                {
                    if (STICK_UP(m_controlInput.fLeftStickAngle))
                    {
                        // 上はスイングになる
                        m_attackState = ATTACK_STATE_SWING_CHARGE;
                        Swing(pos, true);
                    }
                    else if (STICK_RIGHT(m_controlInput.fLeftStickAngle) || STICK_LEFT(m_controlInput.fLeftStickAngle))
                    {
                        // 左右はスマッシュ(この時のみ、向きを空中でも変えれる)
                        m_attackState = ATTACK_STATE_SMASH;
                        m_nCntAttackTime = ATTACK_SMASH_WHOLE_FRAME;
                        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
                        RotControl();
                        Smash(pos, true);
                    }
                    else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
                    {
                        // 下はスパイク
                        m_attackState = ATTACK_STATE_SPIKE;
                        m_nCntAttackTime = ATTACK_SPIKE_WHOLE_FRAME;
                        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
                        Spike(pos, true);
                    }
                }
                else
                {
                    // 倒れていないならスイング
                    m_attackState = ATTACK_STATE_SWING_CHARGE;
                    Swing(pos, true);
                }
            }
            else if (m_controlInput.bTriggerY)
            {
                // キャッチの構え(この時のみ、向きを空中でも変えれる)
                m_attackState = ATTACK_STATE_CATCH_READY;
                m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                RotControl();
                CatchReady(pos);
            }
            else if (m_controlInput.bTriggerB)
            {
                // バント
                m_attackState = ATTACK_STATE_BUNT;
                m_nCntAttackTime = ATTACK_BUNT_WHOLE_FRAME;
                Bunt(pos);
            }
        }
    }
}

//=============================================================================
// スイングのチャージ
// Author : 後藤慎之助
//=============================================================================
void CPlayer::SwingCharge(void)
{
    // スイングボタンを押し続けたら
    if (m_controlInput.bPressX)
    {
        // チャージカウンタアップ
        m_nCntSwingCharge++;
    }

    // スイングボタンから指を離したら
    if (m_controlInput.bReleaseX)
    {
        // スイングのクールタイム等決定
        m_attackState = ATTACK_STATE_SWING;
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }

    // 最大チャージに達したら
    if (m_nCntSwingCharge >= ATTACK_SWING_CHARGE_MAX_FRAME)
    {
        // スイングのクールタイム等決定
        m_attackState = ATTACK_STATE_SWING;
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }
}

//=============================================================================
// スイング
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Swing(D3DXVECTOR3 playerPos, bool bFirstCollision)
{
    // 変数宣言
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ずらす位置
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // 攻撃発生位置
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// 攻撃の大きさ
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // 攻撃の移動角度
    float fAttackAngle = 0.0f;              // 攻撃の角度
    float fFinalPower = 0.0f;               // 最終的な攻撃力

    // 自分の大きさによって変えるもの
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_SWING_SLIDE_POS_SMALL;
        attackSize = ATTACK_SWING_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_SWING_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_SWING_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_SWING_SLIDE_POS_BIG;
        attackSize = ATTACK_SWING_SIZE_BIG;
        break;
    }

    // スティックの角度によって、攻撃角度を変える
    if (m_controlInput.bTiltedLeftStick)
    {
        // 上か下か
        if (STICK_SWING_UP(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = m_afParam[PARAM_SWING_UP];
        }
        else if (STICK_SWING_DOWN(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = m_afParam[PARAM_SWING_DOWN];
        }
        else if (STICK_SWING_HORIZON(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = 90.0f;
        }
    }
    else
    {
        // スティックが倒れていないなら、攻撃角度はニュートラル
        fAttackAngle = 90.0f;
    }

    // プレイヤーの向きが左向きなら、攻撃発生位置と飛ばす方向を逆に
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
        fAttackAngle *= -1;
    }

    // 攻撃発生位置を決める
    attackPos = playerPos + slidePos;

    // 飛ばす方向を決定
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // 攻撃力を考慮（チャージ時間で基礎威力がわずかに上がる）
    fFinalPower = (ATTACK_SWING_BASE_POWER + ATTACK_SWING_CHARGE_ATTACK_RATE * m_nCntSwingCharge) 
        + m_fAtk * ATTACK_SWING_ADD_BASE_POWER_RATE;

    // 最大チャージでスマッシュと同等の威力に変わる
    float fMaxChargePower = 0.0f;
    if (m_nCntSwingCharge >= ATTACK_SWING_CHARGE_MAX_FRAME)
    {
        CBall* pBall = CGame::GetBall();
        fMaxChargePower = ATTACK_SMASH_BASE_POWER + m_fAtk * ATTACK_SMASH_ADD_BASE_POWER_RATE;
        fMaxChargePower *= pBall->GetSpeed() * ATTACK_SMASH_SPEED_RATE;
    }

    // 大きい力のほうを使う
    if (fFinalPower < fMaxChargePower)
    {
        fFinalPower = fMaxChargePower;
    }

    // ボールに当たったかどうか（最大チャージなら即打ち）
    int flag = CBall::SHOOT_FLAG_NONE;
    if (m_nCntSwingCharge >= ATTACK_SWING_CHARGE_MAX_FRAME)
    {
        BITON(flag, CBall::SHOOT_FLAG_QUICK);
    }
    if (IsAttackBall(attackPos, attackSize, moveAngle, fFinalPower, bFirstCollision, flag))
    {
        // 当たっていたら即座にスイングに
        m_attackState = ATTACK_STATE_SWING;

        // スイングのクールタイム決定
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// スマッシュ
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Smash(D3DXVECTOR3 playerPos, bool bFirstCollision)
{
    // 変数宣言
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ずらす位置
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // 攻撃発生位置
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// 攻撃の大きさ
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // 攻撃の移動角度
    float fAttackAngle = 0.0f;              // 攻撃の角度
    float fFinalPower = 0.0f;               // 最終的な攻撃力

    // 自分の大きさによって変えるもの
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_SMASH_SLIDE_POS_SMALL;
        attackSize = ATTACK_SMASH_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_SMASH_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_SMASH_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_SMASH_SLIDE_POS_BIG;
        attackSize = ATTACK_SMASH_SIZE_BIG;
        break;
    }

    // スマッシュの攻撃角度は固定
    fAttackAngle = m_afParam[PARAM_SMASH];

    // プレイヤーの向きが左向きなら、攻撃発生位置と飛ばす方向を逆に
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
        fAttackAngle *= -1;
    }

    // 攻撃発生位置を決める
    attackPos = playerPos + slidePos;

    // 飛ばす方向を決定
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // 攻撃力を考慮（ボールの速さも関係あり）
    CBall* pBall = CGame::GetBall();
    fFinalPower = ATTACK_SMASH_BASE_POWER + m_fAtk * ATTACK_SMASH_ADD_BASE_POWER_RATE;
    fFinalPower *= pBall->GetSpeed() * ATTACK_SMASH_SPEED_RATE;

    // 必ず最大チャージ以上の威力が出る
    float fMaxChargePower = (ATTACK_SWING_BASE_POWER + ATTACK_SWING_CHARGE_ATTACK_RATE * ATTACK_SWING_CHARGE_MAX_FRAME)
        + m_fAtk * ATTACK_SWING_ADD_BASE_POWER_RATE;
    if (fFinalPower < fMaxChargePower)
    {
        fFinalPower = fMaxChargePower;
    }

    // ボールに当たったかどうか
    int flag = CBall::SHOOT_FLAG_NONE;
    IsAttackBall(attackPos, attackSize, moveAngle, fFinalPower, bFirstCollision, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// スパイク
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Spike(D3DXVECTOR3 playerPos, bool bFirstCollision)
{
    // 変数宣言
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ずらす位置
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // 攻撃発生位置
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// 攻撃の大きさ
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // 攻撃の移動角度
    float fAttackAngle = 0.0f;              // 攻撃の角度
    float fFinalPower = 0.0f;               // 最終的な攻撃力

    // 自分の大きさによって変えるもの
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_SPIKE_SLIDE_POS_SMALL;
        attackSize = ATTACK_SPIKE_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_SPIKE_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_SPIKE_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_SPIKE_SLIDE_POS_BIG;
        attackSize = ATTACK_SPIKE_SIZE_BIG;
        break;
    }

    // プレイヤーの向きを考慮
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_RIGHT)
    {
        // スティックの角度によって、攻撃角度を変える
        if (m_controlInput.bTiltedLeftStick)
        {
            if (STICK_SPIKE_RIGHT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = m_afParam[PARAM_SPIKE_RIGHT];
            }
            else if (STICK_SPIKE_LEFT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = m_afParam[PARAM_SPIKE_LEFT];
            }
            else if (STICK_SPIKE_VERTICAL(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = 180.0f;
            }
        }
        else
        {
            // スティックが倒れていないなら、攻撃角度はニュートラル
            fAttackAngle = 180.0f;
        }
    }
    else if (rot.y == PLAYER_ROT_LEFT)
    {
        // スティックの角度によって、攻撃角度を変える
        if (m_controlInput.bTiltedLeftStick)
        {
            if (STICK_SPIKE_RIGHT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = -m_afParam[PARAM_SPIKE_LEFT];
            }
            else if (STICK_SPIKE_LEFT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = -m_afParam[PARAM_SPIKE_RIGHT];
            }
            else if (STICK_SPIKE_VERTICAL(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = 180.0f;
            }
        }
        else
        {
            // スティックが倒れていないなら、攻撃角度はニュートラル
            fAttackAngle = 180.0f;
        }
    }

    // 攻撃発生位置を決める
    attackPos = playerPos + slidePos;

    // 飛ばす方向を決定
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // 攻撃力を考慮
    fFinalPower = ATTACK_SPIKE_BASE_POWER + m_fAtk * ATTACK_SPIKE_ADD_BASE_POWER_RATE;

    // ボールに当たったかどうか
    int flag = CBall::SHOOT_FLAG_NONE;
    IsAttackBall(attackPos, attackSize, moveAngle, fFinalPower, bFirstCollision, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// キャッチの構え
// Author : 後藤慎之助
//=============================================================================
void CPlayer::CatchReady(D3DXVECTOR3 playerPos)
{
    // 変数宣言
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ずらす位置
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // 攻撃発生位置
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// 攻撃の大きさ

    // 自分の大きさによって変えるもの
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_CATCH_READY_SLIDE_POS_SMALL;
        attackSize = ATTACK_CATCH_READY_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_CATCH_READY_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_CATCH_READY_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_CATCH_READY_SLIDE_POS_BIG;
        attackSize = ATTACK_CATCH_READY_SIZE_BIG;
        break;
    }

    // プレイヤーの向きが左向きなら、攻撃発生位置を逆に
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
    }

    // 攻撃発生位置を決める
    attackPos = playerPos + slidePos;

    // ボールに当たったかどうか（投げのフラグをONに）
    int flag = CBall::SHOOT_FLAG_NONE;
    BITON(flag, CBall::SHOOT_FLAG_THROW);
    if (IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, false, flag))
    {
        // 当たっていたら即座に投げに
        m_attackState = ATTACK_STATE_THROW;

        // 投げのクールタイム決定
        m_nCntAttackTime = ATTACK_THROW_WHOLE_FRAME;

        // 必殺ゲージ上昇
        GainSpGauge(true);
    }

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// バント
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Bunt(D3DXVECTOR3 playerPos)
{
    // 変数宣言
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ずらす位置
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // 攻撃発生位置
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// 攻撃の大きさ
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // 攻撃の移動角度
    float fAttackAngle = 0.0f;              // 攻撃の角度

    // 自分の大きさによって変えるもの
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_BUNT_SLIDE_POS_SMALL;
        attackSize = ATTACK_BUNT_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_BUNT_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_BUNT_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_BUNT_SLIDE_POS_BIG;
        attackSize = ATTACK_BUNT_SIZE_BIG;
        break;
    }

    // スティックの角度によって、攻撃角度を変える
    if (m_controlInput.bTiltedLeftStick)
    {
        if (STICK_RIGHT(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = ATTACK_BUNT_ANGLE;
        }
        else if (STICK_LEFT(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = -ATTACK_BUNT_ANGLE;
        }
        else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = 180.0f;
        }
    }

    // プレイヤーの向きが左向きなら、攻撃発生位置と飛ばす方向を逆に
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
    }

    // 攻撃発生位置を決める
    attackPos = playerPos + slidePos;

    // 飛ばす方向を決定
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // ボールに当たったかどうか（バントのフラグをON）
    int flag = CBall::SHOOT_FLAG_NONE;
    BITON(flag, CBall::SHOOT_FLAG_BUNT);
    IsAttackBall(attackPos, attackSize, moveAngle, 0.0f, false, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// 吸収
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Absorb(D3DXVECTOR3 playerPos)
{
    // 変数宣言
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ずらす位置
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // 攻撃発生位置
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// 攻撃の大きさ

    // 自分の大きさによって変えるもの
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_ABSORB_SLIDE_POS_SMALL;
        attackSize = ATTACK_ABSORB_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_ABSORB_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_ABSORB_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_ABSORB_SLIDE_POS_BIG;
        attackSize = ATTACK_ABSORB_SIZE_BIG;
        break;
    }

    // プレイヤーの向きが左向きなら、攻撃発生位置を逆に
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
    }

    // 攻撃発生位置を決める
    attackPos = playerPos + slidePos;

    // ボールに当たったかどうか（吸収のフラグをONに）
    int flag = CBall::SHOOT_FLAG_NONE;
    BITON(flag, CBall::SHOOT_FLAG_ABSORB);
    IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, false, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// ボールに攻撃が当たったかどうかのチェック
// Author : 後藤慎之助
//=============================================================================
bool CPlayer::IsAttackBall(D3DXVECTOR3 attackPos, D3DXVECTOR3 attackSize, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag)
{
    // 当たったかどうか
    bool bHit = false;

    // ボールとの当たり判定
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BALL);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BALL); nCntScene++)
    {
        // 中身があるなら
        if (pScene != NULL)
        {
            // 次のシーンを記憶
            CScene*pNextScene = pScene->GetNextScene();

            // ボールにキャスト
            CBall *pBall = (CBall*)pScene;

            // 当たり判定を使用するかつ、自分以外の誰かが吸収していないなら
            if (pBall->GetUseCollision())
            {
                if (pBall->GetWhoAbsorbing() == m_playable || pBall->GetWhoAbsorbing() == BALL_NOT_ANYONE)
                {
                    // 当たっているなら
                    if (IsCollisionToRotationRect(attackPos, attackSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&attackPos, &pBall->GetPos(),
                            &attackSize, &BALL_COLLISION_SIZE))
                    {
                        // ファーヒットならフラグON
                        if (!IsCollisionRectangle3D(&attackPos, &pBall->GetPos(),
                            &attackSize, &BALL_COLLISION_SIZE))
                        {
                            BITON(flag, CBall::SHOOT_FLAG_FAR_HIT);
                        }

                        // ボールを飛ばす
                        D3DXVECTOR3 attackCenterPos = attackPos + D3DXVECTOR3(0.0f, attackSize.y / 2, 0.0f);
                        if (bFirstCollision)
                        {
                            // 最初の接触は、こちらも硬直時間を受ける
                            m_nCntStopTime = pBall->Shoot(attackCenterPos, moveAngle, fPower, bFirstCollision, flag, this);

                            // 必殺ゲージ上昇
                            GainSpGauge();

                            // 硬直時間がないなら、即座に飛ばす
                            if (m_nCntStopTime <= 0)
                            {
                                pBall->Shoot(attackCenterPos, moveAngle, fPower, true, flag, this);
                            }
                        }
                        else
                        {
                            pBall->Shoot(attackCenterPos, moveAngle, fPower, bFirstCollision, flag, this);
                        }

                        // 当たった
                        bHit = true;

                        // 多段ヒット回避用のフラグ
                        m_bUseAvoidMultipleHits = true;

                        // このフレーム中に攻撃が当たったかどうかのフラグ（主に最終局面で攻撃と防御の当たり判定を同時に発生させたとき用）
                        m_bHitAttackThisFrame = true;
                    }
                }
            }
        }
    }

    return bHit;
}

//=============================================================================
// 向きの制御
// Author : 後藤慎之助
//=============================================================================
void CPlayer::RotControl()
{
    // 設定する回転
    D3DXVECTOR3 rot = GetRot();

    // スティックが倒れているなら向きを変える
    if (m_controlInput.bTiltedLeftStick)
    {
        if (!m_bSquat)
        {
            // スティックの角度から、向きを変える
            if (STICK_RIGHT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_RIGHT;
            }
            else if (STICK_LEFT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_LEFT;
            }
        }
        else
        {
            // しゃがみながら向きを変える場合、スティックは下寄り
            if (STICK_SQUAT_RIGHT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_RIGHT;
            }
            else if (STICK_SQUAT_LEFT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_LEFT;
            }
        }
    }

    // 回転の設定
    SetRot(rot);
}

//=============================================================================
// 攻撃モーションの管理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::AttackMotion(void)
{
    // 攻撃状態に応じて、モーションを決める
    switch (m_attackState)
    {
    case ATTACK_STATE_SWING_CHARGE:
        GetAnimation()->SetAnimation(ANIM_SWING_CHARGE);
        break;
    case ATTACK_STATE_SWING:
        GetAnimation()->SetAnimation(ANIM_SWING);
        break;
    case ATTACK_STATE_SMASH:
        GetAnimation()->SetAnimation(ANIM_SMASH);
        break;
    case ATTACK_STATE_SPIKE:
        GetAnimation()->SetAnimation(ANIM_SPIKE);
        break;
    case ATTACK_STATE_BUNT:
        GetAnimation()->SetAnimation(ANIM_BUNT);
        break;
    case ATTACK_STATE_CATCH_READY:
        GetAnimation()->SetAnimation(ANIM_CATCH_READY);
        break;
    case ATTACK_STATE_THROW:
        GetAnimation()->SetAnimation(ANIM_THROW);
        m_move *= ATTACK_THROW_SLOW_SPEED_RATE; // 投げてるときはスローな移動
        break;
    case ATTACK_STATE_ABSORB:
        GetAnimation()->SetAnimation(ANIM_ABSORB);
        break;
    }
}

//=============================================================================
// 負傷の更新処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::DamageUpdate(D3DXVECTOR3 pos)
{
    switch (m_damageState)
    {
    case DAMAGE_STATE_BLOWN:
        // 吹っ飛ばされからダウンへの遷移
        if (m_bGround)
        {
            // 地面にいてかつ、速度が落ちているなら
            if (fabsf(m_move.x) < PLAYER_KNOCK_BACK_STOP_BORDER)
            {
                m_move.x = 0.0f;
                m_damageState = DAMAGE_STATE_BIG;
                m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_BIG_FRAME;
            }
        }
        else
        {
            // 空中の速度を最低限保持する
            if (fabsf(m_move.x) < PLAYER_KNOCK_BACK_STOP_BORDER)
            {
                if (m_move.x > 0.0f)
                {
                    m_move.x = PLAYER_KNOCK_BACK_STOP_BORDER;
                }
                else
                {
                    m_move.x = -PLAYER_KNOCK_BACK_STOP_BORDER;
                }
            }
        }
        break;
    case DAMAGE_STATE_BIG:
        // 起き上がり可能時間なら
        if (m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_START_QUICK_STAND_UP)
        {
            // 即起き上がりor吸収起き上がりアピール
            if (m_controlInput.bTiltedLeftStick)
            {
                m_damageState = DAMAGE_STATE_STAND_UP;
                m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_STAND_UP_FRAME;
            }
            else if (m_controlInput.bTriggerX)
            {
                // ダメージ状態のリセット
                m_damageState = DAMAGE_STATE_NONE;
                m_nCntTakeDamageTime = 0;

                // 吸収
                m_attackState = ATTACK_STATE_ABSORB;
                m_nCntAttackTime = ATTACK_ABSORB_WHOLE_FRAME;
                Absorb(pos);
            }
        }
        break;
    }
}

//=============================================================================
// 負傷モーションの管理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::DamageMotion(void)
{
    // 負傷状態に応じて、モーションを決める
    switch (m_damageState)
    {
    case DAMAGE_STATE_SMALL:
        GetAnimation()->SetAnimation(ANIM_DAMAGE_SMALL);
        break;
    case DAMAGE_STATE_BIG:
        GetAnimation()->SetAnimation(ANIM_DAMAGE_BIG);
        break;
    case DAMAGE_STATE_STAND_UP:
        GetAnimation()->SetAnimation(ANIM_STAND_UP);
        break;
    case DAMAGE_STATE_BLOWN:
        GetAnimation()->SetAnimation(ANIM_BLOWN);
        break;
    }
}

//=============================================================================
// 必殺ゲージ上昇処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::GainSpGauge(bool bExAdd)
{
    // 必殺技ゲージ上昇量を出す
    float fGainSpGauge = ATTACK_SP_GAUGE_BASE_GAIN + m_fAtk * ATTACK_SP_GAUGE_ADD_BASE_GAIN_RATE;

    // キャッチなどの更に倍率が高くなる行動なら
    if (bExAdd)
    {
        fGainSpGauge *= ATTACK_SP_GAUGE_EX_ADD_RATE;
    }

    // 現在のゲージに足す
    m_fSpGaugeCurrent += fGainSpGauge;

    // 最大値を上回らないようにする
    if (m_fSpGaugeCurrent > m_fSpGaugeMax)
    {
        m_fSpGaugeCurrent = m_fSpGaugeMax;
    }
}
