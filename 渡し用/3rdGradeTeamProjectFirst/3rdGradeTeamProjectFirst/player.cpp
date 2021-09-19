//======================================================================================
//
// プレイヤー処理 (player.cpp)
// Author : 後藤慎之助、池田悠希（リザルトやカスタマイズのクリッピング対応）
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
#include "wave.h"
#include "effect3d.h"
#include "ui.h"
#include "effect2d.h"
#include "number_array.h"
#include "text.h"
#include "cliping_musk.h"

//========================================
// マクロ定義
//========================================
#define CUSTOM_ATK_COLOR D3DXCOLOR(0.874f, 0.329f, 0.094f, 1.0f)
#define CUSTOM_DEF_COLOR D3DXCOLOR(0.203f, 0.768f, 0.196f, 1.0f)
#define CUSTOM_SPD_COLOR D3DXCOLOR(0.156f, 0.56f, 0.854f, 1.0f)
#define CUSTOM_WEI_COLOR D3DXCOLOR(1.0f, 0.501f, 0.0f, 1.0f)

#define MAX_ATK 2250.0f
#define MAX_DEF 2000.0f
#define MAX_SPD 2700.0f
#define MAX_WEI 4100.0f

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CPlayer::CPlayer() :CCharacter(OBJTYPE::OBJTYPE_PLAYER)
{
    m_bMannequin = false;
    m_controlInput.bTiltedLeftStick = false;
    m_controlInput.fLeftStickAngle = 0.0f;
    m_controlInput.bTiltedRightStick = false;
    m_controlInput.fRightStickAngle = 0.0f;
    m_controlInput.bTriggerA = false;
    m_controlInput.bPressA = false;
    m_controlInput.bTriggerX = false;
    m_controlInput.bPressX = false;
    m_controlInput.bReleaseX = false;
    m_controlInput.bTriggerY = false;
    m_controlInput.bTriggerB = false;
    m_bUseKeyboard = false;
    m_AIlevel = AI_LEVEL_NONE;
    m_pAI = NULL;
    m_pUI_HP = NULL;
    m_pUI_SP = NULL;
    memset(m_apUI_Stock, 0, sizeof(m_apUI_Stock));
    m_pUI_Playable = NULL;
    m_pUI_Custom_Atk = NULL;
    m_pUI_Custom_Def = NULL;
    m_pUI_Custom_Spd = NULL;
    m_pUI_Custom_Wei = NULL;
    m_pUI_Custom_Ability = NULL;
    m_pText_Custom_Ex_Head = NULL;
    m_pText_Custom_Ex_Up = NULL;
    m_pText_Custom_Ex_Down = NULL;
    m_pText_Custom_Ex_Wep = NULL;
    m_pText_Custom_Sp = NULL;
    m_bDispAbility = false;

    m_nIdxCreate = PLAYER_1;
    m_nIdxControlAndColor = PLAYER_1;
    m_nPoint = 0;
    m_nStock = 0;
    m_startPos = DEFAULT_VECTOR;
    m_startRot = DEFAULT_VECTOR;

    m_move = DEFAULT_VECTOR;
    m_moveOld = DEFAULT_VECTOR;
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

    memset(m_anNumCustomParts, 0, sizeof(m_anNumCustomParts));
    m_fAtk = 0.0f;
    m_fDef = 0.0f;
    m_fSpd = 0.0f;
    m_fWei = 0.0f;
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

    m_collisionSizeAttack = DEFAULT_VECTOR;
    m_collisionSizeDeffence = DEFAULT_VECTOR;

	m_pClipingMusk = NULL;

    //===================================
    // 特殊能力対応周り
    //===================================
    m_bUsedThreeJump = false;
    m_nSwingChargeMax = 0;
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

    // アニメーションを読み込む
    //CCharacter::LoadModelData("./data/ANIMATION/motion_player.txt");  // 座標・親子関係の読み込みと、アニメーションの読み込みを分けた
    CCharacter::SetAnimFilePass("./data/ANIMATION/motion_player.txt");

    // プレイアブルキャラの設定を読み込む
    LoadCustom();

    return S_OK;
}

//=============================================================================
// カスタマイズ読み込み処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::LoadCustom(void)
{
    // ファイルポインタ
    FILE *pFile = NULL;

    // パーツリストから情報取得
    CModelData *pModelData = CManager::GetModelData();

    // 変数宣言
    int nPartsListType = 0;                // パーツリスト内の種類
    int nExNumber = 0;                     // 特殊能力番号
    int nVoiceSetNumber = 0;               // ボイスセットの番号

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
    m_collisionSizeAttack = DEFAULT_VECTOR;
    m_collisionSizeDeffence = DEFAULT_VECTOR;
    memset(m_afParam, 0, sizeof(m_afParam));
    m_nSwingChargeMax = ATTACK_SWING_CHARGE_MAX_FRAME_NORMAL;

    // カスタマイズデータのファイルを開く
    switch (m_nIdxControlAndColor)
    {
    case PLAYER_1:
        pFile = fopen("data/TXT/custom1.txt", "r");
        break;
    case PLAYER_2:
        pFile = fopen("data/TXT/custom2.txt", "r");
        break;
    case PLAYER_3:
        pFile = fopen("data/TXT/custom3.txt", "r");
        break;
    case PLAYER_4:
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
            int customPartsExFlag = 0;  // カスタマイズ画面で使う、パーツ毎の特殊能力
            nExNumber = pModelData->GetPartsList(nPartsListType)->nEx0;
            BITON(m_exFlag, 0x001 << nExNumber);
            BITON(customPartsExFlag, 0x001 << nExNumber);
            nExNumber = pModelData->GetPartsList(nPartsListType)->nEx1;
            BITON(m_exFlag, 0x001 << nExNumber);
            BITON(customPartsExFlag, 0x001 << nExNumber);

            // 各パーツリストから、細部のパーツを決定
            if (nCntPartsList == PARTS_LIST_HEAD)
            {
                // カスタマイズ画面で特殊能力表示を更新
                if (m_pText_Custom_Ex_Head)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Head->SetText(cExName);
                }

                // モデルをバインド
                BindParts(PARTS_HEAD, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);

                // カスタマイズパーツ番号を取得
                m_anNumCustomParts[CUSTOM_PARTS_HEAD] = nPartsListType;

                // スイングの向きを、補助値から補助値へ
                m_afParam[PARAM_SWING_UP] = pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_afParam[PARAM_SWING_DOWN] = pModelData->GetPartsList(nPartsListType)->afParam[2];
                m_afParam[PARAM_SMASH] = pModelData->GetPartsList(nPartsListType)->afParam[3];
                m_afParam[PARAM_SPIKE_RIGHT] = pModelData->GetPartsList(nPartsListType)->afParam[4];
                m_afParam[PARAM_SPIKE_LEFT] = pModelData->GetPartsList(nPartsListType)->afParam[5];

                // ボイスセットの番号を保持
                nVoiceSetNumber = (int)pModelData->GetPartsList(nPartsListType)->afParam[6];

                // 防御当たり判定の更新
                if (m_collisionSizeDeffence.x < pModelData->GetPartsList(nPartsListType)->fWidth)
                {
                    m_collisionSizeDeffence.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                }
                m_collisionSizeDeffence.y += pModelData->GetPartsList(nPartsListType)->fHeight;
            }
            else if (nCntPartsList == PARTS_LIST_UP)
            {
                // カスタマイズ画面で特殊能力表示を更新
                if (m_pText_Custom_Ex_Up)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Up->SetText(cExName);
                }

                // モデルをバインド
                BindParts(PARTS_BODY, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                BindParts(PARTS_LHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);

                // カスタマイズパーツ番号を取得
                m_anNumCustomParts[CUSTOM_PARTS_UP] = nPartsListType;

                // 上半身の初期位置データ
                m_nModelPosDefUp = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];

                // 防御当たり判定の更新
                if (m_collisionSizeDeffence.x < pModelData->GetPartsList(nPartsListType)->fWidth)
                {
                    m_collisionSizeDeffence.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                }
                m_collisionSizeDeffence.y += pModelData->GetPartsList(nPartsListType)->fHeight;
            }
            else if (nCntPartsList == PARTS_LIST_DOWN)
            {
                // カスタマイズ画面で特殊能力表示を更新
                if (m_pText_Custom_Ex_Down)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Down->SetText(cExName);
                }

                // モデルをバインド
                BindParts(PARTS_HIP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                BindParts(PARTS_LFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);

                // カスタマイズパーツ番号を取得
                m_anNumCustomParts[CUSTOM_PARTS_DOWN] = nPartsListType;

                // 下半身の初期位置データ
                m_nModelPosDefDown = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];

                // 防御当たり判定の更新
                if (m_collisionSizeDeffence.x < pModelData->GetPartsList(nPartsListType)->fWidth)
                {
                    m_collisionSizeDeffence.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                }
                m_collisionSizeDeffence.y += pModelData->GetPartsList(nPartsListType)->fHeight;
            }
            else if (nCntPartsList == PARTS_LIST_WEP)
            {
                // カスタマイズ画面で特殊能力表示を更新
                if (m_pText_Custom_Ex_Wep)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Wep->SetText(cExName);
                }

                // モデルをバインド
                BindParts(PARTS_WEP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);

                // カスタマイズパーツ番号を取得
                m_anNumCustomParts[CUSTOM_PARTS_WEP] = nPartsListType;

                // 必殺技と、そのゲージ量
                m_spShot = (int)pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_fSpGaugeMax = pModelData->GetPartsList(nPartsListType)->afParam[2];

                // 必殺技の補助値を補助値へ
                m_afParam[PARAM_5_WEAPON_SP] = pModelData->GetPartsList(nPartsListType)->afParam[5];
                m_afParam[PARAM_6_WEAPON_SP] = pModelData->GetPartsList(nPartsListType)->afParam[6];
                m_afParam[PARAM_7_WEAPON_SP] = pModelData->GetPartsList(nPartsListType)->afParam[7];

                // 攻撃当たり判定の更新
                m_collisionSizeAttack.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                m_collisionSizeAttack.y = pModelData->GetPartsList(nPartsListType)->fHeight;
            }
        }
        // ファイルを閉じる
        fclose(pFile);
    }

    // カスタマイズ画面で必殺技表示を更新
    if (m_pText_Custom_Sp)
    {
        char cSpName[256];
        CustomSpName(cSpName);
        m_pText_Custom_Sp->SetText(cSpName);
    }

    // 体力を決定
    m_fLife = m_fDef;

    // ボイスセットの番号を結びつける
    BindVoiceSet(nVoiceSetNumber);

    // 溜め短縮のフラグがONなら、反映
    if (IS_BITON(m_exFlag, EX_FLAG_FAST_CHARGE))
    {
        m_nSwingChargeMax = ATTACK_SWING_CHARGE_MAX_FRAME_FAST;
    }

    // キャラクターに反映
    CCharacter::LoadModelData(m_nModelPosDefUp, m_nModelPosDefDown);
    CCharacter::Init(CCharacter::GetPos(), DEFAULT_SCALE);
}

//=============================================================================
// ボイスセットを結びつける
// Author : 後藤慎之助
//=============================================================================
void CPlayer::BindVoiceSet(int voiceSet)
{
    switch (voiceSet)
    {
    case VOICE_SET_0:
        break;
    case VOICE_SET_1:
        break;
    case VOICE_SET_2:
        break;
    case VOICE_SET_3:
        break;
    }
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

    // クリッピングの情報を開放
	if (m_pClipingMusk != NULL)
	{
		m_pClipingMusk->Uninit();
		delete m_pClipingMusk;
		m_pClipingMusk = NULL;
	}
	
    CCharacter::Uninit();
}

//=============================================================================
// 入力処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Input(void)
{
    // コントローラorキーボード操作（1Pの場合、Enterで遷移したならキーボード操作に変える）
    if (m_bUseKeyboard)
    {
        // キーボードを取得
        CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

        // ボタン情報を取得
        m_controlInput.bTriggerA = pInputKeyboard->GetKeyboardTrigger(DIK_SPACE);
        m_controlInput.bPressA = pInputKeyboard->GetKeyboardPress(DIK_SPACE);
        m_controlInput.bTriggerX = pInputKeyboard->GetKeyboardTrigger(DIK_RETURN);
        m_controlInput.bPressX = pInputKeyboard->GetKeyboardPress(DIK_RETURN);
        m_controlInput.bReleaseX = pInputKeyboard->GetKeyboardRelease(DIK_RETURN);
        m_controlInput.bTriggerY = pInputKeyboard->GetKeyboardTrigger(DIK_UP);
        m_controlInput.bTriggerB = pInputKeyboard->GetKeyboardTrigger(DIK_RIGHT);

        // 左スティックが傾いているかどうか
        if (pInputKeyboard->GetKeyboardPress(DIK_W) || 
            pInputKeyboard->GetKeyboardPress(DIK_A) ||
            pInputKeyboard->GetKeyboardPress(DIK_S) || 
            pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            m_controlInput.bTiltedLeftStick = true;

            // 角度を求める
            if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(-45.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(-135.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_D))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(-90.0f);
                }
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(45.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(135.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_A))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(90.0f);
                }
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_S))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(180.0f);
                }
            }
        }
        else
        {
            m_controlInput.bTiltedLeftStick = false;
        }
    }
    else
    {
        // コントローラを取得
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
        DIJOYSTATE2 Controller = pInputJoypad->GetController(m_nIdxControlAndColor);

        // ボタン情報を取得
        m_controlInput.bTriggerA = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_A);
        m_controlInput.bPressA = pInputJoypad->GetJoypadPress(m_nIdxControlAndColor, CInputJoypad::BUTTON_A);
        m_controlInput.bTriggerX = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_X);
        m_controlInput.bPressX = pInputJoypad->GetJoypadPress(m_nIdxControlAndColor, CInputJoypad::BUTTON_X);
        m_controlInput.bReleaseX = pInputJoypad->GetJoypadRelease(m_nIdxControlAndColor, CInputJoypad::BUTTON_X);
        m_controlInput.bTriggerY = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_Y);
        m_controlInput.bTriggerB = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_B);

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

        // 右スティックが傾いているかどうか
        if (Controller.lZ != 0 || Controller.lRz != 0)
        {
            m_controlInput.bTiltedRightStick = true;

            // 角度を求める
            m_controlInput.fRightStickAngle = atan2(Controller.lZ, Controller.lRz*-1);
        }
        else
        {
            m_controlInput.bTiltedRightStick = false;
        }
    }
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Update(void)
{
    // マネキンモードでないなら
    if (!m_bMannequin)
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
            if (m_nCntStopTime <= 0)
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
        else
        {
            // やられた時の挙動
            DeadMove();
        }

        // ゲームUIの更新
        UpdateGameUI();
    }
    else
    {
        // マネキンモードの更新
        UpdateMannequin();
    }
}

//=============================================================================
// マネキンモードの更新処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::UpdateMannequin(void)
{
    // カスタマイズ画面での待機
    GetAnimation()->SetAnimation(ANIM_CUSTOM_IDLE);

    // 武器を変えたら、武器を見る
    if (m_nCntAttackAnimTime > 0)
    {
        // 武器を見る
        m_nCntAttackAnimTime--;
        GetAnimation()->SetAnimation(ANIM_WEAPON_LOOK);
    }

    // 表示中なら
    if (m_bDisp)
    {
        // 入力処理
        Input();

        // 1Pは、キーボードで右スティックの回転ができる
        if (m_nIdxControlAndColor == PLAYER_1)
        {
            CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
            if (pInputKeyboard->GetKeyboardPress(DIK_UP) ||
                pInputKeyboard->GetKeyboardPress(DIK_LEFT) ||
                pInputKeyboard->GetKeyboardPress(DIK_DOWN) ||
                pInputKeyboard->GetKeyboardPress(DIK_RIGHT))
            {
                m_controlInput.bTiltedRightStick = true;

                // 角度を求める
                if (pInputKeyboard->GetKeyboardPress(DIK_LEFT))
                {
                    if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(-45.0f);
                    }
                    else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(-135.0f);
                    }
                    else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                    }
                    else
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(-90.0f);
                    }
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT))
                {
                    if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(45.0f);
                    }
                    else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(135.0f);
                    }
                    else if (pInputKeyboard->GetKeyboardPress(DIK_LEFT))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                    }
                    else
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(90.0f);
                    }
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                {
                    if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                    }
                    else
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                    }
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                {
                    if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                    }
                    else
                    {
                        m_controlInput.fRightStickAngle = D3DXToRadian(180.0f);
                    }
                }
            }
        }
  
        // 右スティックが倒れているなら向きを変える
        D3DXVECTOR3 rot = GetRot();
        const float ROT_SPEED = 3.0f;
        if (m_controlInput.bTiltedRightStick)
        {
            // 右回転
            if (STICK_RIGHT(m_controlInput.fRightStickAngle))
            {
                rot.y -= D3DXToRadian(ROT_SPEED);
            }
            else if (STICK_LEFT(m_controlInput.fRightStickAngle))
            {
                // 左回転
                rot.y += D3DXToRadian(ROT_SPEED);
            }
        }

        // 回転の制限
        if (rot.y > D3DXToRadian(180.0f) || rot.y < D3DXToRadian(-180.0f))
        {
            rot.y *= -1;
        }

        // 回転の設定
        SetRot(rot);

        // カスタマイズ画面のUIの更新
        m_pUI_Custom_Atk->SetLeftToRightGauge(MAX_ATK, m_fAtk);
        m_pUI_Custom_Def->SetLeftToRightGauge(MAX_DEF, m_fDef);
        m_pUI_Custom_Spd->SetLeftToRightGauge(MAX_SPD, m_fSpd);
        m_pUI_Custom_Wei->SetLeftToRightGauge(MAX_WEI, m_fWei);
        if (m_bDispAbility)
        {
            m_pUI_Custom_Ability->SetAlpha(1.0f);
            m_pText_Custom_Ex_Head->SetColor(TEXT_EXIST_COLOR);
            m_pText_Custom_Ex_Up->SetColor(TEXT_EXIST_COLOR);
            m_pText_Custom_Ex_Down->SetColor(TEXT_EXIST_COLOR);
            m_pText_Custom_Ex_Wep->SetColor(TEXT_EXIST_COLOR);
            m_pText_Custom_Sp->SetColor(TEXT_EXIST_COLOR);
        }
        else
        {
            m_pUI_Custom_Ability->SetAlpha(0.0f);
            m_pText_Custom_Ex_Head->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Ex_Up->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Ex_Down->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Ex_Wep->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Sp->SetColor(TEXT_NOT_EXIST_COLOR);
        }
    }
    else
    {
        // 非表示なら、各表示物も消す
        m_pUI_Custom_Atk->SetLeftToRightGauge(MAX_ATK, 0);
        m_pUI_Custom_Def->SetLeftToRightGauge(MAX_DEF, 0);
        m_pUI_Custom_Spd->SetLeftToRightGauge(MAX_SPD, 0);
        m_pUI_Custom_Wei->SetLeftToRightGauge(MAX_WEI, 0);
        m_pUI_Custom_Ability->SetAlpha(0.0f);
        m_pText_Custom_Ex_Head->SetColor(TEXT_NOT_EXIST_COLOR);
        m_pText_Custom_Ex_Up->SetColor(TEXT_NOT_EXIST_COLOR);
        m_pText_Custom_Ex_Down->SetColor(TEXT_NOT_EXIST_COLOR);
        m_pText_Custom_Ex_Wep->SetColor(TEXT_NOT_EXIST_COLOR);
        m_pText_Custom_Sp->SetColor(TEXT_NOT_EXIST_COLOR);
    }

    // アニメーションさせる
    CCharacter::Update();
}

//=============================================================================
// プレイヤーが持つUIの更新処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::UpdateGameUI(void)
{
    // HPとSPゲージを設定
    m_pUI_HP->SetLeftToRightGauge(m_fDef, m_fLife);
    m_pUI_SP->SetLeftToRightGauge(m_fSpGaugeMax, m_fSpGaugeCurrent);

    // ストックを表示/非表示
    for (int nCnt = 0; nCnt < PLAYER_MAX_STOCK; nCnt++)
    {
        if (m_apUI_Stock[nCnt])
        {
            if (nCnt < m_nStock)
            {
                m_apUI_Stock[nCnt]->CUI::SetCol(DEFAULT_COLOR);
            }
            else
            {
                m_apUI_Stock[nCnt]->CUI::SetCol(DEFAULT_COLOR_NONE);
            }
        }
    }

    // 即死のHPとボールの速さ関係なら、HPゲージを点滅
    float fDamage = CGame::GetBall()->GetSpeed() * PLAYER_TAKE_DAMAGE_RATE_FROM_BALL;
    D3DXCOLOR col = m_pUI_HP->GetCol();
    if (m_fLife <= fDamage)
    {
        if (col.a == 1.0f)
        {
            m_pUI_HP->SetCol(D3DXCOLOR(col.r, col.g, col.b, 0.0f));
        }
        else
        {
            m_pUI_HP->SetCol(D3DXCOLOR(col.r, col.g, col.b, 1.0f));
        }
    }
    else
    {
        m_pUI_HP->SetCol(D3DXCOLOR(col.r, col.g, col.b, 1.0f));
    }

    // 何Pかの表示の位置更新（しゃがみ、もしくはダウンか起き上がり中なら表示の位置を下げる）
    float fSizeY = m_collisionSizeDeffence.y;
    if (m_bSquat ||
        m_damageState == DAMAGE_STATE_BIG || m_damageState == DAMAGE_STATE_STAND_UP)
    {
        fSizeY *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
    }
    m_pUI_Playable->SetPosTo2D(GetPos() + D3DXVECTOR3(0.0f, fSizeY + 50.0f, 0.0f));
}

//=============================================================================
// やられた時の挙動
// Author : 後藤慎之助
//=============================================================================
void CPlayer::DeadMove(void)
{
    // 硬直していないなら
    if (m_nCntStopTime <= 0)
    {
        // 位置を取得
        D3DXVECTOR3 pos = DEFAULT_VECTOR;
        pos = GetPos();

        // 移動量制御
        m_move.x *= PLAYER_CONTROL_MOVE_DEATH;

        // 移動量と位置を結びつける
        pos += m_move;

        // 重力
        m_move.y -= PLAYER_TAKE_DAMAGE_BLOWN_GRAVITY;

        // 見えないところで止めておく
        if (pos.y < PLAYER_DEATH_STOP_POS_Y)
        {
            pos.y = PLAYER_DEATH_STOP_POS_Y;
        }

        // 位置を反映
        SetPos(pos);

        // 吹っ飛ばされモーション
        GetAnimation()->SetAnimation(ANIM_BLOWN);

        // アニメーション更新
        CCharacter::Update();
    }
    else
    {
        // 硬直時間をカウント
        m_nCntStopTime--;
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
    m_nCntAttackTime = 0;
    m_nCntAttackAnimTime = 0;
    m_attackState = ATTACK_STATE_NONE;
    m_nCntSwingCharge = 0;
    m_bUseAvoidMultipleHits = false;
}

//=============================================================================
// リスポーン処理
// Author : 後藤慎之助
//=============================================================================
void CPlayer::Respawn(void)
{
    // リスポーン
    SetPos(m_startPos);
    SetRot(m_startRot);
    m_bDisp = true;
    m_fLife = m_fDef;

    // 既存のリセット関数
    ResetOnGround();
    ResetAttack();

    // メンバ変数のリセット
    m_move = DEFAULT_VECTOR;
    m_nCntLandingTime = 0;
    m_bGroundOld = false;
    m_nCntTurnTime = 0;
    m_bSquat = false;
    m_nCntStopTime = 0;

    m_nCntTakeDamageTime = 0;
    m_bUsedLuckyGuard = false;
    m_damageState = DAMAGE_STATE_NONE;

    m_fSpGaugeCurrent = 0.0f;
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
		// レンダラーからデバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		if (m_pClipingMusk != NULL)
		{
			m_pClipingMusk->Draw();
			//ステンシルの設定
			pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_STENCILREF, m_pClipingMusk->GetReferenceValue() + 1);
			pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		}
        CCharacter::Draw();
		if (m_pClipingMusk != NULL)
		{
			//ステンシル無効化
			pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		}
    }
    else
    {
        // マネキンモードでないなら
        if (!m_bMannequin)
        {
            // やられた時の描画
            CCharacter::DeathDraw();
        }
    }
}

//=============================================================================
// インスタンス生成
// Author : 後藤慎之助
//=============================================================================
CPlayer * CPlayer::CreateInGame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nStock, int nIdxCreate, int nIdxControlAndColor,
    AI_LEVEL AIlevel, bool bUseKeyboard)
{
    // メモリ確保
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // 親元の情報を設定
    pPlayer->CCharacter::SetPos(pos);
    pPlayer->CCharacter::SetRot(rot);

    // 読み込む種類の設定
    pPlayer->m_nIdxControlAndColor = nIdxControlAndColor;

    // 初期化
    pPlayer->Init(pos, DEFAULT_SCALE);

    // 結びつけるメンバ変数の初期化
    pPlayer->m_startPos = pos;
    pPlayer->m_startRot = rot;
    pPlayer->m_nStock = nStock;
    pPlayer->m_nIdxCreate = nIdxCreate;
    pPlayer->m_AIlevel = AIlevel;
    pPlayer->m_bUseKeyboard = bUseKeyboard;
    if (pPlayer->m_AIlevel != AI_LEVEL_NONE)
    {
        pPlayer->m_pAI = CAi::Create(pPlayer);
    }

    // UIを生成
    D3DXCOLOR col = DEFAULT_COLOR;
    int nTexTypePlayable = 0;
    switch (pPlayer->m_nIdxControlAndColor)
    {
    case PLAYER_1:
        col = PLAYER_COLOR_1;
        nTexTypePlayable = 21;
        break;
    case PLAYER_2:
        col = PLAYER_COLOR_2;
        nTexTypePlayable = 22;
        break;
    case PLAYER_3:
        col = PLAYER_COLOR_3;
        nTexTypePlayable = 23;
        break;
    case PLAYER_4:
        col = PLAYER_COLOR_4;
        nTexTypePlayable = 24;
        break;
    }
    switch (pPlayer->m_AIlevel)
    {
    case AI_LEVEL_1:
        nTexTypePlayable = 25;
        break;
    case AI_LEVEL_2:
        nTexTypePlayable = 27;
        break;
    case AI_LEVEL_3:
        nTexTypePlayable = 28;
        break;
    }
    const float UI_SIZE_X = 240.0f; // ここの値を、UIの大体の横幅に合わせる
    const float SPACE_SIZE = (SCREEN_WIDTH - (UI_SIZE_X * CGame::GetNumAllPlayer())) / (CGame::GetNumAllPlayer() + 1);
    const float FIRST_UI_POS_X = SPACE_SIZE + (UI_SIZE_X / 2);
    const float NEXT_UI_POS_X = UI_SIZE_X + SPACE_SIZE;
    float fDigitPosX = FIRST_UI_POS_X + (NEXT_UI_POS_X * (float)pPlayer->m_nIdxCreate);
    CUI* pUI = CUI::Create(17, D3DXVECTOR3(fDigitPosX, 40.0f, 0.0f), D3DXVECTOR3(240.0f, 30.0f, 0.0f), 0, DEFAULT_COLOR);
    pPlayer->m_pUI_HP = CUI::Create(18, D3DXVECTOR3(fDigitPosX, 40.0f, 0.0f), D3DXVECTOR3(232.0f, 20.4f, 0.0f), 0, col);
    pPlayer->m_pUI_HP->SetActionInfo(0, CUI::ACTION_GAUGE, false); // ゲージに変える
    CUI::Create(17, D3DXVECTOR3(fDigitPosX, 70.0f, 0.0f), D3DXVECTOR3(240.0f, 30.0f, 0.0f), 0, DEFAULT_COLOR);
    pPlayer->m_pUI_SP = CUI::Create(18, D3DXVECTOR3(fDigitPosX, 70.0f, 0.0f), D3DXVECTOR3(232.0f, 20.4f, 0.0f), 0, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
    pPlayer->m_pUI_SP->SetActionInfo(0, CUI::ACTION_GAUGE, false); // ゲージに変える
    for (int nCntStock = 0; nCntStock < pPlayer->m_nStock; nCntStock++)
    {
        // ストックは位置をずらす
        float fStockDigitPosX = -120.0f + 15.0f + (30.0f * nCntStock);  // -ゲージの半分の大きさ+ストックの半分の大きさ+(ストックの大きさ*何番目のストックか)
        pPlayer->m_apUI_Stock[nCntStock] = CUI::Create(19, D3DXVECTOR3(fDigitPosX + fStockDigitPosX, 100.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), 0, DEFAULT_COLOR);
    }
    pPlayer->m_pUI_Playable = CUI::Create(nTexTypePlayable, DEFAULT_VECTOR, D3DXVECTOR3(50.0f, 50.0f, 0.0f), 0, col);

    return pPlayer;
}

//=============================================================================
// カスタマイズ画面での生成
// Author : 後藤慎之助
//=============================================================================
CPlayer * CPlayer::CreateInCustom(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nControlIndex, bool bDisp)
{
    // メモリ確保
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // 親元の情報を設定
    pPlayer->CCharacter::SetPos(pos);
    pPlayer->CCharacter::SetRot(rot);

    // 読み込む種類の設定
    pPlayer->m_nIdxControlAndColor = nControlIndex;

    // UIを生成
    D3DXVECTOR3 startPos = D3DXVECTOR3(205.0f, 293.5f, 0.0f);
    D3DXVECTOR3 clipingPos = DEFAULT_VECTOR;
    const D3DXVECTOR3 gaugeSize = D3DXVECTOR3(210.0f, 22.0f, 0.0f);
    const D3DXVECTOR3 abilitySize = D3DXVECTOR3(297.0f, 155.0f, 0.0f);
    D3DXVECTOR3 abilityPos = D3DXVECTOR3(167.0f, 200.5f, 0.0f);
    switch (pPlayer->m_nIdxControlAndColor)
    {
    case PLAYER_1:
        clipingPos = D3DXVECTOR3(167.0f, 390.0f, 0.0f);
        break;
    case PLAYER_2:
        startPos.x += 315.0f;
        abilityPos.x += 315.0f;
        clipingPos = D3DXVECTOR3(482.0f, 390.0f, 0.0f);
        break;
    case PLAYER_3:
        startPos.x += 315.0f * PLAYER_3;
        abilityPos.x += 315.0f * PLAYER_3;
        clipingPos = D3DXVECTOR3(797.0f, 390.0f, 0.0f);
        break;
    case PLAYER_4:
        startPos.x += 315.0f * PLAYER_4;
        abilityPos.x += 315.0f * PLAYER_4;
        clipingPos = D3DXVECTOR3(1112.0f, 390.0f, 0.0f);
        break;
    }
    pPlayer->m_pUI_Custom_Ability = CUI::Create(73, abilityPos, abilitySize, 0, DEFAULT_COLOR_NONE_ALPHA);
    float fDigitPosY = 0.0f;
    const float DIGIT_UI_VALUE = 31.5f;
    pPlayer->m_pUI_Custom_Def = CUI::Create(18, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_DEF_COLOR);
    pPlayer->m_pUI_Custom_Def->SetActionInfo(0, CUI::ACTION_GAUGE, false); // ゲージに変える
    fDigitPosY += DIGIT_UI_VALUE;

    pPlayer->m_pUI_Custom_Atk = CUI::Create(18, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_ATK_COLOR);
    pPlayer->m_pUI_Custom_Atk->SetActionInfo(0, CUI::ACTION_GAUGE, false); // ゲージに変える
    fDigitPosY += DIGIT_UI_VALUE;

    pPlayer->m_pUI_Custom_Spd = CUI::Create(18, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_SPD_COLOR);
    pPlayer->m_pUI_Custom_Spd->SetActionInfo(0, CUI::ACTION_GAUGE, false); // ゲージに変える
    fDigitPosY += DIGIT_UI_VALUE;

    pPlayer->m_pUI_Custom_Wei = CUI::Create(18, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_WEI_COLOR);
    pPlayer->m_pUI_Custom_Wei->SetActionInfo(0, CUI::ACTION_GAUGE, false); // ゲージに変える

    // テキストを生成
    startPos += D3DXVECTOR3(-105.0f, -155.0f, 0.0f); // UIのステータスバーから、アビリティのテキストへ位置を合わせる
    fDigitPosY = 0.0f;
    const float DIGIT_TEXT_VALUE = 26.0f;
    const int textSize = 20;
    pPlayer->m_pText_Custom_Ex_Head = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "なし",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Ex_Up = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "なし",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Ex_Down = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "なし",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Ex_Wep = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "なし",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Sp = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "なし",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);

    // クリッピングを生成
    pPlayer->ApplyMusk(clipingPos, D3DXVECTOR3(298.0f, 625.0f, 0.0f));

    // 初期化
    pPlayer->Init(pos, DEFAULT_SCALE);

    // 結びつけるメンバ変数の初期化
    pPlayer->m_startPos = pos;
    pPlayer->m_startRot = rot;
    pPlayer->m_bDisp = bDisp;

    // マネキンモードに
    pPlayer->m_bMannequin = true;

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

    // 1F前の移動量を記憶
    m_moveOld = m_move;

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
    if (pos.x - m_collisionSizeDeffence.x < -mapLimit.fWidth)
    {
        pos.x = -mapLimit.fWidth + m_collisionSizeDeffence.x;
    }
    if (pos.x + m_collisionSizeDeffence.x > mapLimit.fWidth)
    {
        pos.x = mapLimit.fWidth - m_collisionSizeDeffence.x;
    }
    if (pos.y + m_collisionSizeDeffence.y > mapLimit.fHeight)
    {
        pos.y = mapLimit.fHeight - m_collisionSizeDeffence.y;
    }
    
    //====================================================
    // 攻撃処理
    // 攻撃がヒットしていないなら、攻撃の判定等を更新
    if (!m_bUseAvoidMultipleHits)
    {
        AttackUpdate(pos, true); // bFirestCollisionのtrueは、ここが最初の接触になりうるから
    }

    // 攻撃の生成
    AttackGenerator(pos);

    // ジャンプ
    Jump();

    // 移動モーションの管理
    MoveMotion();

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
        if (m_damageState == DAMAGE_STATE_NONE || m_damageState == DAMAGE_STATE_STAND_UP && m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
        {
            D3DXVECTOR3 size = m_collisionSizeDeffence;
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
        // 着地の砂煙（当たり判定の分、砂煙が広がる）
        CEffect3D::Emit(CEffectData::TYPE_GROUND_SMOKE, GetPos(), GetPos(), m_collisionSizeDeffence.x);

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
                if (pBall->GetWhoShooting() != m_nIdxCreate && pBall->GetWhoShooting() != BALL_NOT_ANYONE)
                {
                    // 自分の当たり判定の大きさを決定
                    D3DXVECTOR3 playerSize = m_collisionSizeDeffence;

                    // しゃがんでいるか、起き上がり中なら縦の当たり判定を削る
                    if (m_bSquat || 
                        m_damageState == DAMAGE_STATE_STAND_UP && m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
                    {
                        playerSize.y *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
                    }

                    // 当たっているなら
                    if (IsCollisionToRotationRect(playerPos, playerSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&playerPos, &pBall->GetCollisionPos(),
                            &playerSize, &BALL_COLLISION_SIZE))
                    {
                        // ダメージを計算（ラッキーガード発生可能性あり）
                        float fDamage = pBall->GetSpeed() * PLAYER_TAKE_DAMAGE_RATE_FROM_BALL;
                        TakeDamage(fDamage, pBall->GetWhoShooting(), pBall->GetPos(), pBall->GetPosOld(), true);
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
void CPlayer::TakeDamage(float fDamage, int nWho, D3DXVECTOR3 damagePos, D3DXVECTOR3 damageOldPos, bool bUseLuckyGuard, bool bSetOff)
{
    // 生存しているなら
    if (m_bDisp)
    {
        // 攻撃状態をリセット
        ResetAttack();

        // 変数宣言
        D3DXVECTOR3 playerPos = GetPos();       // 位置を取得
        float fKnockbackValue = 0.0f;           // ノックバック量
        bool bUseLuckyGuardThisFrame = false;   // このフレーム中にラッキーガードが成立したかどうか

        // 相殺処理かどうか
        if (bSetOff)
        {
            // ラッキーガードを代用
            bUseLuckyGuardThisFrame = true;
            fKnockbackValue = PLAYER_KNOCK_BACK_LUCKY_GUARD;        // 専用のノックバック量
            m_nCntStopTime = BALL_SHOOT_STOP_LUCKY_GUARD_FRAME;
        }
        else
        {
            // 相殺処理でないなら、ラッキーガードを使う攻撃かどうか
            if (bUseLuckyGuard)
            {
                // ラッキーガードを使っていないなら
                if (!m_bUsedLuckyGuard)
                {
                    // 乱数の結果で、ラッキーガード
                    int nRandNum = GetRandNum(PLAYER_LUCKY_GUARD_MAX, 1);

                    // ファーストヒットガードなら、最初の一撃を必ずガード
                    if (IS_BITON(m_exFlag, EX_FLAG_FIRST_HIT_GUARD))
                    {
                        nRandNum = 0;
                    }

                    // 最大防御よりも乱数の結果が下回ったら、ラッキーガード
                    if (nRandNum <= (int)m_fDef)
                    {
                        // ラッキーガードエフェクト発生
                        CEffect3D::Emit(CEffectData::TYPE_LUCKY_GUARD_FIRST, playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f));
                        CEffect3D::Emit(CEffectData::TYPE_LUCKY_GUARD_SECOND, playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f));
                        CEffect3D::Emit(CEffectData::TYPE_LUCKY_GUARD_SECOND, playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f));

                        bUseLuckyGuardThisFrame = true;
                        m_bUsedLuckyGuard = true;
                        fDamage = 1.0f;                                         // 1ダメージは受ける
                        fKnockbackValue = PLAYER_KNOCK_BACK_LUCKY_GUARD;        // 専用のノックバック量
                        m_nCntStopTime = CGame::GetBall()->Launch(playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            BALL_SHOOT_STOP_LUCKY_GUARD_FRAME);                 // ボールを打ち上げる
                        CWave::Create(playerPos, BALL_WAVE_FIRST_SIZE);         // 波発生
                        CManager::SoundPlay(CSound::LABEL_SE_OFFSET);           // 相殺音
                    }
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
        int nEffectFrame = PLAYER_TAKE_DAMAGE_SMALL_EFFECT_FRAME;   // 振動フレーム
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

            // 大きい振動
            nEffectFrame = PLAYER_TAKE_DAMAGE_BIG_EFFECT_FRAME;
        }

        // ダメージを受ける
        m_fLife -= fDamage;

        // ダメージを表示
        if (fDamage > 0)
        {
            D3DXVECTOR3 dispDamagePos = ConvertScreenPos(playerPos);
            CNumberArray::Create(12, dispDamagePos, NUMBER_SIZE_X_DAMAGE, D3DXCOLOR(0.933f, 0.427f, 0.513f, 1.0f), (int)fDamage, false, true);

            // ラッキーガード以上のダメージで、ヒットエフェクト
            if (fDamage > 1.0f)
            {
                // もしダメージの位置と、1F前のダメージの位置が同じなら、ダメージの位置をプレイヤーの位置にする
                D3DXVECTOR3 hitEffectPos = damagePos;
                if (damagePos == damageOldPos)
                {
                    hitEffectPos = playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f);
                }
                CEffect3D::Emit(CEffectData::TYPE_HIT_BALL, hitEffectPos, damageOldPos);
            }
        }

        // 体力がなくなったら
        if (m_fLife <= 0.0f)
        {
            // 死亡時の振動
            nEffectFrame = PLAYER_TAKE_DAMAGE_DEATH_EFFECT_FRAME;

            // KO音
            CManager::SoundPlay(CSound::LABEL_SE_KO);

            // 死亡表示に
            m_fLife = 0.0f;
            m_fSpGaugeCurrent = 0.0f;
            m_bDisp = false;

            // 死亡用ノックバック
            fKnockbackValue = PLAYER_KNOCK_BACK_DEATH_X;
            m_move.y = PLAYER_KNOCK_BACK_DEATH_Y;

            // ストックを減らす
            m_nStock--;

            // ストックがあるかどうか
            if (m_nStock > 0)
            {
                // やられたプレイヤーがまだ誰もいないなら
                if (CGame::GetNumDefeatPlayer() == 0)
                {
                    // 最下位のプレイヤーにする
                    CGame::SetWorstPlayer(m_nIdxCreate);
                }

                // やられたプレイヤー人数を増やす
                CGame::SetAddNumDefeatPlayer();
            }
            else
            {
                // 死んだプレイヤー人数を増やす
                CGame::SetAddNumDeathPlayer();
            }

            // ボールにやられたなら、ボールも少しだけ止める（ボールでしかラッキーガードは発動しないため代用）
            m_nCntStopTime = PLAYER_DEATH_STOP_FRAME;
            if (bUseLuckyGuard)
            {
                CGame::GetBall()->SetStopTime(PLAYER_DEATH_STOP_FRAME);
            }

            // 最後に攻撃した人にポイントが入る
            CPlayer* pPlayer = CGame::GetPlayer(nWho);
            if (pPlayer)
            {
                pPlayer->SetAddPoint();
            }
        }
        else
        {
            // ラッキーガード以上のダメージで
            if (fDamage > 1.0f)
            {
                // ダメージ音
                CManager::SoundPlay(CSound::LABEL_SE_DAMAGE);
            }
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

        // コントローラの振動
        if (GetUseControllerEffect())
        {
            CManager::GetInputJoypad()->StartEffect(m_nIdxControlAndColor, nEffectFrame);
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
                                // 砂煙の発生
                                if (fabsf(m_moveOld.x) <= PLAYER_SET_WALK_SMOKE_VALUE)
                                {
                                    CEffect3D::Emit(CEffectData::TYPE_WALK_SMOKE_LEFT, GetPos(), GetPos());
                                }
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
                                // 砂煙の発生
                                if (fabsf(m_moveOld.x) <= PLAYER_SET_WALK_SMOKE_VALUE)
                                {
                                    CEffect3D::Emit(CEffectData::TYPE_WALK_SMOKE_RIGHT, GetPos(), GetPos());
                                }
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
                    // 向きによって、砂埃の向きを変える
                    if (GetRot().y == PLAYER_ROT_LEFT)
                    {
                        // ジャンプの砂煙
                        CEffect3D::Emit(CEffectData::TYPE_JUMP_SMOKE_RIGHT, GetPos(), GetPos());
                    }
                    else
                    {
                        // ジャンプの砂煙
                        CEffect3D::Emit(CEffectData::TYPE_JUMP_SMOKE_LEFT, GetPos(), GetPos());
                    }

                    // ジャンプ音
                    CManager::SoundPlay(CSound::LABEL_SE_JUMP);

                    // ジャンプの初期量
                    m_move.y = PLAYER_JUMP_FIRST_RATE;

                    // 滑空までのカウンタを設定
                    m_nCntStartGlide = PLAYER_FIRST_JUMP_MOTION_TO_GLIDE;

                    // 地面にいないことに
                    m_bGround = false;

                    // 念のため着地時間と振り向き時間をリセット
                    m_nCntLandingTime = 0;
                    m_nCntTurnTime = 0;

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
                                // 多段ジャンプの空気の輪と砂煙（当たり判定の半分の大きさ）
                                CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_AIR_RING, GetPos(), GetPos(), m_collisionSizeDeffence.x * 0.5f);
                                CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_SMOKE, GetPos(), GetPos());

                                // ジャンプ音
                                CManager::SoundPlay(CSound::LABEL_SE_JUMP);

                                // 1Fだけ向きを変えることができる
                                RotControl();

                                // ジャンプの初期量
                                m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                                // 滑空までのカウンタを設定
                                m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                                // 三段ジャンプを使ったフラグをtrueに
                                m_bUsedThreeJump = true;

                                // 念のため着地時間と振り向き時間をリセット
                                m_nCntLandingTime = 0;
                                m_nCntTurnTime = 0;

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
                            // 多段ジャンプの空気の輪と砂煙（当たり判定の半分の大きさ）
                            CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_AIR_RING, GetPos(), GetPos(), m_collisionSizeDeffence.x * 0.5f);
                            CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_SMOKE, GetPos(), GetPos());

                            // ジャンプ音
                            CManager::SoundPlay(CSound::LABEL_SE_JUMP);

                            // 1Fだけ向きを変えることができる
                            RotControl();

                            // ジャンプの初期量
                            m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                            // 滑空までのカウンタを設定
                            m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                            // 二段ジャンプを使ったフラグをtrueに
                            m_bUsedSecondJump = true;

                            // 念のため着地時間と振り向き時間をリセット
                            m_nCntLandingTime = 0;
                            m_nCntTurnTime = 0;

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
            // 負傷していない状態なら（チャージ中でも急降下できる）
            if (m_damageState == DAMAGE_STATE_NONE)
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
                // キャッチができるなら
                if (IS_BITOFF(m_exFlag, EX_FLAG_DONT_CATCH))
                {
                    // キャッチの構え
                    m_attackState = ATTACK_STATE_CATCH_READY;
                    m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                    CatchReady(pos);
                }
            }
            else if (m_controlInput.bTriggerB)
            {
                // 素振り音
                CManager::SoundPlay(CSound::LABEL_SE_SWISH);

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
                        // 素振り音
                        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

                        // 左右はスマッシュ(この時のみ、向きを空中でも変えれる)
                        m_attackState = ATTACK_STATE_SMASH;
                        m_nCntAttackTime = ATTACK_SMASH_WHOLE_FRAME;
                        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
                        RotControl();
                        Smash(pos, true);
                    }
                    else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
                    {
                        // 素振り音
                        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

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
                // キャッチができるなら
                if (IS_BITOFF(m_exFlag, EX_FLAG_DONT_CATCH))
                {
                    // キャッチの構え(この時のみ、向きを空中でも変えれる)
                    m_attackState = ATTACK_STATE_CATCH_READY;
                    m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                    RotControl();
                    CatchReady(pos);
                }
            }
            else if (m_controlInput.bTriggerB)
            {
                // 素振り音
                CManager::SoundPlay(CSound::LABEL_SE_SWISH);

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
        // 素振り音
        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

        // スイングのクールタイム等決定
        m_attackState = ATTACK_STATE_SWING;
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }

    // 最大チャージに達したら
    if (m_nCntSwingCharge >= m_nSwingChargeMax)
    {
        // 素振り音
        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

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
    slidePos.x = (m_collisionSizeAttack.x * 0.5f) + (m_collisionSizeDeffence.x * 0.25f);
    attackSize.x = m_collisionSizeAttack.x + (m_collisionSizeDeffence.x * 0.5f);
    attackSize.y = m_collisionSizeDeffence.y;

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
    if (m_nCntSwingCharge >= m_nSwingChargeMax)
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
    if (m_nCntSwingCharge >= m_nSwingChargeMax)
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
    slidePos.x = (m_collisionSizeAttack.x * 0.5f) + (m_collisionSizeDeffence.x * 0.25f);
    attackSize.x = m_collisionSizeAttack.x + (m_collisionSizeDeffence.x * 0.5f);
    slidePos.y = (m_collisionSizeAttack.y * 0.5f) + (m_collisionSizeDeffence.y * 0.25f);
    attackSize.y = m_collisionSizeAttack.y + (m_collisionSizeDeffence.y * 0.5f);

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
    float fMaxChargePower = (ATTACK_SWING_BASE_POWER + ATTACK_SWING_CHARGE_ATTACK_RATE * ATTACK_SWING_CHARGE_MAX_FRAME_NORMAL)
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
    slidePos.y = -((m_collisionSizeAttack.y * 0.5f) + (m_collisionSizeDeffence.y * 0.25f));
    attackSize.y = m_collisionSizeAttack.y + (m_collisionSizeDeffence.y * 0.5f);
    attackSize.x = m_collisionSizeDeffence.x * ATTACK_SPIKE_SIZE_RATE_X;

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

    // 自分の大きさによって変えるもの（キャッチで武器は考慮しない）
    slidePos.x = m_collisionSizeDeffence.x * ATTACK_CATCH_READY_SLIDE_POS_X_RATE;
    attackSize.x = slidePos.x * 2.0f;
    attackSize.y = m_collisionSizeDeffence.y;

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
    if (IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, true, flag))
    {
        // コントローラの振動
        if (GetUseControllerEffect())
        {
            CManager::GetInputJoypad()->StartEffect(m_nIdxControlAndColor, ATTACK_CATCH_READY_EFFECT_FRAME);
        }

        // キャッチ音
        CManager::SoundPlay(CSound::LABEL_SE_CATCH);

        // 当たっていたら即座に投げに
        m_attackState = ATTACK_STATE_THROW;

        // 投げのクールタイム決定
        m_nCntAttackTime = ATTACK_THROW_WHOLE_FRAME;
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
    slidePos.x = (m_collisionSizeAttack.x * 0.5f) + (m_collisionSizeDeffence.x * 0.25f);
    attackSize.x = m_collisionSizeAttack.x + (m_collisionSizeDeffence.x * 0.5f);
    attackSize.y = m_collisionSizeDeffence.y;

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
    IsAttackBall(attackPos, attackSize, moveAngle, 0.0f, true, flag);

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

    // 吸収の大きさは、自分の防御当たり判定の平均に割合をかけたもの
    float fSize = (m_collisionSizeDeffence.x + m_collisionSizeDeffence.y) * 0.5f * ATTACK_ABSORB_SIZE_RATE;
    attackSize.x = fSize;
    attackSize.y = fSize;

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
    IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, false, flag); // 常に最初の接触ではなく、相殺に勝つ

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
                if (pBall->GetWhoAbsorbing() == m_nIdxCreate || pBall->GetWhoAbsorbing() == BALL_NOT_ANYONE)
                {
                    // 当たっているなら
                    if (IsCollisionToRotationRect(attackPos, attackSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&attackPos, &pBall->GetCollisionPos(),
                            &attackSize, &BALL_COLLISION_SIZE))
                    {
                        // バント以外なら
                        if (IS_BITOFF(flag, CBall::SHOOT_FLAG_BUNT))
                        {
                            // ファーヒットならフラグON
                            if (!IsCollisionRectangle3D(&attackPos, &pBall->GetCollisionPos(),
                                &attackSize, &BALL_COLLISION_SIZE))
                            {
                                BITON(flag, CBall::SHOOT_FLAG_FAR_HIT);
                            }
                        }
                        else
                        {
                            // バントは大きさを1.5倍で換算して、ファーヒットならフラグON（連続でバントをした際に、不自然に見えたため）
                            if (!IsCollisionRectangle3D(&attackPos, &pBall->GetCollisionPos(),
                                &(attackSize * 1.5f), &BALL_COLLISION_SIZE))
                            {
                                BITON(flag, CBall::SHOOT_FLAG_FAR_HIT);
                            }
                        }

                        // ボールを飛ばすための予約をする（ここでボールの中心を渡すのは、ファーヒットの時にその位置へ持っていくため）
                        D3DXVECTOR3 attackCenterPos = attackPos + D3DXVECTOR3(0.0f, attackSize.y / 2, 0.0f);
                        CGame::ReserveShoot(attackCenterPos, moveAngle, fPower, bFirstCollision, flag, m_nIdxCreate);

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

//=============================================================================
// 毎ラウンドリセットするステータス
// Author : 後藤慎之助
//=============================================================================
void CPlayer::ResetStatusEveryRound(void)
{
    // 必殺ゲージ
    m_fSpGaugeCurrent = 0.0f;

    // ラッキーガード（ファーストヒットガードのキャラのみリセット）
    if (IS_BITON(m_exFlag, EX_FLAG_FIRST_HIT_GUARD))
    {
        m_bUsedLuckyGuard = false;
    }
}

//=============================================================================
// コントローラの振動を使用するかどうか
// Author : 後藤慎之助
//=============================================================================
bool CPlayer::GetUseControllerEffect(void)
{
    bool bUseControllerEffect = false;

    // AIでないかつ、キーボード操作でないなら、振動を使う
    if (m_AIlevel == AI_LEVEL_NONE && !m_bUseKeyboard)
    {
        bUseControllerEffect = true;
    }

    // いずれオプションで強制OFFできるようにここに書く

    return bUseControllerEffect;
}


//=============================================================================
// クリッピングマスクを適用
// Author : 池田悠希
//=============================================================================
void CPlayer::ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumTexture)
{
    if (!m_pClipingMusk)
    {
        m_pClipingMusk = CClipingMusk::Create(pos, size, nNumTexture);
    }
}

//=============================================================================
// テクスチャなしクリッピングマスクを適用
// Author : 池田悠希
//=============================================================================
void CPlayer::ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    if (!m_pClipingMusk)
    {
        m_pClipingMusk = CClipingMusk::Create(pos, size);
    }
}