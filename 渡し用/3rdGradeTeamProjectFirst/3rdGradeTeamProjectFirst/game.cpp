//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "game.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "input.h"
#include "scene.h"
#include "library.h"
#include "debug.h"
#include "pause.h"
#include "scene2d.h"
#include "camera.h"
#include "ball.h"
#include "bg.h"
#include "wave.h"
#include "effect2d.h"
#include "effect3d.h"
#include "number_array.h"
#include "text.h"

//========================================
// マクロ定義
//========================================

// ゲーム状態の管理フレーム
#define BLOW_MOMENT_FRAME 180          // 一撃の瞬間フレーム数
#define FINISH_WAIT_FRAME 240          // 決着時に、待つフレーム数
#define CREATE_POS_Y_RATE 0.8f         // ボールの発生位置Yの割合
#define FADE_IN_TELOP 30               // テロップのフェードイン開始フレーム
#define FADE_OUT_TELOP 150             // テロップのフェードアウト開始フレーム
#define FADE_IN_FINISH_TELOP 90        // フィニッシュテロップのフェードイン開始フレーム

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CGame::m_bStopObjUpdate = false;

CPlayer *CGame::m_apPlayer[] = {};
int CGame::m_anPlayerRank[] = {};
int CGame::m_anPlayerRankInThisRound[] = {};
CBall *CGame::m_pBall = NULL;
CPause *CGame::m_pPause = NULL;
CEffect2D *CGame::m_pEffect2d_Nega = NULL;
CEffect2D *CGame::m_pEffect2d_Posi = NULL;
CNumberArray *CGame::m_pNumArray_BallSpd = NULL;

CGame::TYPE CGame::m_type = TYPE_TRAINING;
int CGame::m_nNumAllPlayer = 0;
int CGame::m_nNumStock = 3; // 初期のストック数は3個
bool CGame::m_bUseKeyboard = false;
int CGame::m_anMemoryIdxPlayer[] = {};
CPlayer::AI_LEVEL CGame::m_aMemoryAILevel[] = {};
CGame::STATE CGame::m_state = STATE_ROUND_START;
CGame::MAP_LIMIT CGame::m_mapLimit = {};
int CGame::m_nNumDefeatPlayer = 0;
int CGame::m_nWhoWorstPlayer = PLAYER_1;
int CGame::m_nNumDeathPlayer = 0;
CGame::RESERVE_SHOOT CGame::m_aReserveShoot[] = {};

CPlayer *CGame::m_pSpPlayer = NULL;
bool CGame::m_bCurrentSpShot = false;
CText *CGame::m_pSpText = NULL;

//=============================================================================
// ゲームのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CGame::CGame()
{
    m_bStopObjUpdate = false;
    m_bCurrentSpShot = false;
    m_pSpText = NULL;

    memset(m_apPlayer, 0, sizeof(m_apPlayer));
    memset(m_anPlayerRank, 0, sizeof(m_anPlayerRank));
    memset(m_anPlayerRankInThisRound, 0, sizeof(m_anPlayerRankInThisRound));
    m_pBall = NULL;
    m_pPause = NULL;
    m_pEffect2d_Nega = NULL;
    m_pEffect2d_Posi = NULL;
    m_pNumArray_BallSpd = NULL;
    m_pSpPlayer = NULL;

    // 静的メンバ変数を初期化（遷移時に毎回必要なものだけ）
    //m_type = TYPE_TRAINING;
    //m_nNumAllPlayer = 0;
    //m_nNumStock = 0;
    //m_bUseKeyboard = false;
    //memset(m_anMemoryIdxPlayer, 0, sizeof(m_anMemoryIdxPlayer));
    //memset(m_aMemoryAILevel, 0, sizeof(m_aMemoryAILevel));
    m_state = STATE_ROUND_START;
    m_nNumDefeatPlayer = 0;
    m_nWhoWorstPlayer = PLAYER_1;
    m_nNumDeathPlayer = 0;
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        m_aReserveShoot[nCnt].attackCenterPos = DEFAULT_VECTOR;
        m_aReserveShoot[nCnt].moveAngle = DEFAULT_VECTOR;
        m_aReserveShoot[nCnt].fPower = 0.0f;
        m_aReserveShoot[nCnt].bFirstCollision = true;
        m_aReserveShoot[nCnt].flag = CBall::SHOOT_FLAG_NONE;
        m_aReserveShoot[nCnt].bReserved = false;
    }

    // 仮でマップ制限をつけている
    m_mapLimit.fHeight = GAME_LIMIT_HEIGHT;
    m_mapLimit.fWidth = GAME_LIMIT_WIDTH;

    m_nCntGameTime = 0;
    m_bFirestRound = true;
}

//=============================================================================
// ゲームのデストラクタ
// Author : 後藤慎之助
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CGame::Init(void)
{
    // テキスト生成
    m_pSpText = CText::Create(D3DXVECTOR3(640.0f, 500.0f, 0.0f), 100,
        "なし", CText::ALIGN_CENTER, "Reggae One", TEXT_NOT_EXIST_COLOR);

    // 定義
    const float SPLIT_RATE_UNDER_3 = 0.5f;
    const float SPLIT_RATE_ABOVE_2 = 0.333f;
    if (m_type == TYPE_TRAINING)
    {
        m_nNumAllPlayer = 1; // トレーニングは1人固定
        m_nNumStock = 3;     // トレーニングは3ストック固定
    }

    // ステージのモデルを生成
    CBg::Create(34, DEFAULT_VECTOR);    // ステージ1は34
    CBg::Create(83, DEFAULT_VECTOR);    // ステージ1の線は83

    // UIを生成
    CUI::Place(CUI::SET_GAME);

    // ポーズの生成
    m_pPause = CPause::Create();

    // プレイヤーの生成
    D3DXVECTOR3 player1Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player2Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player3Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player4Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    float fSplitXRate = 0.0f;
    switch (m_nNumAllPlayer)
    {
    case 1:
        fSplitXRate = SPLIT_RATE_UNDER_3;
        player1Pos.x *= -fSplitXRate;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, 
            0, m_anMemoryIdxPlayer[0], m_aMemoryAILevel[0], m_bUseKeyboard);
        break;
    case 2:
        fSplitXRate = SPLIT_RATE_UNDER_3;
        player1Pos.x *= -fSplitXRate;
        player2Pos.x *= fSplitXRate;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock,
            0, m_anMemoryIdxPlayer[0], m_aMemoryAILevel[0], m_bUseKeyboard);
        m_apPlayer[1] = CPlayer::CreateInGame(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock,
            1, m_anMemoryIdxPlayer[1], m_aMemoryAILevel[1]);
        break;
    case 3:
        fSplitXRate = SPLIT_RATE_ABOVE_2;
        player1Pos.x *= -fSplitXRate * 2;
        player2Pos.x *= -fSplitXRate;
        player3Pos.x *= fSplitXRate;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock,
            0, m_anMemoryIdxPlayer[0], m_aMemoryAILevel[0], m_bUseKeyboard);
        m_apPlayer[1] = CPlayer::CreateInGame(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock,
            1, m_anMemoryIdxPlayer[1], m_aMemoryAILevel[1]);
        m_apPlayer[2] = CPlayer::CreateInGame(player3Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock,
            2, m_anMemoryIdxPlayer[2], m_aMemoryAILevel[2]);
        break;
    case 4:
        fSplitXRate = SPLIT_RATE_ABOVE_2;
        player1Pos.x *= -fSplitXRate * 2;
        player2Pos.x *= -fSplitXRate;
        player3Pos.x *= fSplitXRate;
        player4Pos.x *= fSplitXRate * 2;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock,
            0, m_anMemoryIdxPlayer[0], m_aMemoryAILevel[0], m_bUseKeyboard);
        m_apPlayer[1] = CPlayer::CreateInGame(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, 
            1, m_anMemoryIdxPlayer[1], m_aMemoryAILevel[1]);
        m_apPlayer[2] = CPlayer::CreateInGame(player3Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock,
            2, m_anMemoryIdxPlayer[2], m_aMemoryAILevel[2]);
        m_apPlayer[3] = CPlayer::CreateInGame(player4Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock,
            3, m_anMemoryIdxPlayer[3], m_aMemoryAILevel[3]);
        break;
    }
    // カメラのロックオン場所を変える
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_GAME);

    // ボールを生成
    m_pBall = CBall::Create(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f), true);

    // 反転合成用のエフェクトを生成
    m_pEffect2d_Nega = CEffect2D::Create(3, DEFAULT_VECTOR);
    m_pEffect2d_Nega->SetUseUpdate(false);
    m_pEffect2d_Posi = CEffect2D::Create(3, DEFAULT_VECTOR);
    m_pEffect2d_Posi->SetUseUpdate(false);

    // ボールスピード表示を生成
    m_pNumArray_BallSpd = CNumberArray::Create(12, D3DXVECTOR3(640.0f, 675.0f, 0.0f), NUMBER_SIZE_X_BALL_SPD, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), (int)BALL_FIRST_SPEED, false);

    // BGMをランダム再生
    int nRand = GetRandNum(2, 0);
    switch (nRand)
    {
    case 0:
        CManager::SoundPlay(CSound::LABEL_BGM_BATTLE00);
        break;
    case 1:
        CManager::SoundPlay(CSound::LABEL_BGM_BATTLE01);
        break;
    case 2:
        CManager::SoundPlay(CSound::LABEL_BGM_BATTLE02);
        break;
    }

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CGame::Uninit(void)
{
    // 全ての音を停止
    CManager::SoundStopAll();

    // ポーズを破棄
    if (m_pPause != NULL)
    {
        m_pPause->Uninit();
        delete m_pPause;
        m_pPause = NULL;
    }
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CGame::Update(void)
{
    // ゲーム状態の管理
    ManageState();

    // 反転合成の終了チェック
    if (m_pEffect2d_Nega && m_pEffect2d_Posi)
    {
        // ポジのほうが一定の大きさに達したら、どちらもサイズを戻す
        if (m_pEffect2d_Posi->GetSize().x >= SCREEN_WIDTH * 2)
        {
            m_pEffect2d_Nega->SetSize(DEFAULT_VECTOR);
            m_pEffect2d_Posi->SetSize(DEFAULT_VECTOR);
            m_pEffect2d_Nega->SetUseUpdate(false);
            m_pEffect2d_Posi->SetUseUpdate(false);
            m_pEffect2d_Nega->SetRotVertex(0.0f);     // 更新を止める代わりに、頂点はここで調整
            m_pEffect2d_Posi->SetRotVertex(0.0f);     // 更新を止める代わりに、頂点はここで調整
        }
    }
}

//=============================================================================
// ゲーム状態の管理
// Author : 後藤慎之助
//=============================================================================
void CGame::ManageState(void)
{
    switch (m_state)
    {
    case STATE_ROUND_START:
        // ラウンド開始
        RoundStart();
        break;

    case STATE_BUTTLE:
        // バトル中
        InButtle();
        break;

    case STATE_BLOW_MOMENT:
        // 一撃の瞬間
        BlowMoment();
        break;

    case STATE_FINISH:
        // 勝敗判定
        JudgmentFinish();
        break;

    case STATE_PAUSE_MENU:
        // ポーズの更新
        m_pPause->Update();
        break;
    }
}

//=============================================================================
// ラウンド開始処理
// Author : 後藤慎之助
//=============================================================================
void CGame::RoundStart(void)
{
    // リスポーン処理
    for (int nCntPlayer = 0; nCntPlayer < m_nNumAllPlayer; nCntPlayer++)
    {
        // 生存していないかつ、ストックが1以上あるなら
        if (!m_apPlayer[nCntPlayer]->GetDisp() && m_apPlayer[nCntPlayer]->GetStock() > 0)
        {
            m_apPlayer[nCntPlayer]->Respawn();
        }

        // 毎ラウンドリセットするステータス
        m_apPlayer[nCntPlayer]->ResetStatusEveryRound();
    }

    // ボールスピード表示をリセット
    m_pNumArray_BallSpd->SetDispNumber((int)BALL_FIRST_SPEED);
    CUI *pBallGaugeR = CUI::GetAccessUI(0);
    CUI *pBallGaugeL = CUI::GetAccessUI(1);
    if (pBallGaugeR)
    {
        pBallGaugeR->SetLeftToRightGauge(BALL_UPDATE_METER_MIN_STOP_FRAME, 0);
    }
    if (pBallGaugeL)
    {
        pBallGaugeL->SetRightToLeftGauge(BALL_UPDATE_METER_MIN_STOP_FRAME, 0);
    }

    // カウンタを加算
    m_nCntGameTime++;

    // 一定フレームで、バトル中に
    if (m_nCntGameTime >= BALL_DISPING_TIME)
    {
        // カウンタリセット
        m_nCntGameTime = 0;

        //// ミッションスタート表示
        //CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_START);

        // バトル中に
        m_state = STATE_BUTTLE;
    }
    else if (m_nCntGameTime == FADE_OUT_TELOP)
    {
        if (!m_bFirestRound)
        {
            // ネクストラウンド
            CUI *pTelopBg = CUI::GetAccessUI(4);
            CUI *pTelop = CUI::GetAccessUI(5);
            if (pTelopBg)
            {
                pTelopBg->SetActionLock(2, false);
            }
            if (pTelop)
            {
                pTelop->SetActionLock(2, false);
            }
        }
        else
        {
            // ファーストラウンドのフラグを切る
            m_bFirestRound = false;

            // バトルスタート
            CUI *pTelopBg = CUI::GetAccessUI(2);
            CUI *pTelop = CUI::GetAccessUI(3);
            if (pTelopBg)
            {
                pTelopBg->SetActionLock(2, false);
            }
            if (pTelop)
            {
                pTelop->SetActionLock(2, false);
            }
        }
    }
    else if (m_nCntGameTime == FADE_IN_TELOP)
    {
        if (!m_bFirestRound)
        {
            // ネクストラウンド
            CUI *pTelopBg = CUI::GetAccessUI(4);
            CUI *pTelop = CUI::GetAccessUI(5);
            if (pTelopBg)
            {
                pTelopBg->SetActionReset(0);
                pTelopBg->SetActionLock(0, false);
                pTelopBg->SetActionReset(2);
            }
            if (pTelop)
            {
                pTelop->SetActionReset(0);
                pTelop->SetActionLock(0, false);
                pTelop->SetActionReset(1);
                pTelop->SetActionLock(1, false);
                pTelop->SetActionReset(2);
            }
        }
        else
        {
            // バトルスタート
            CUI *pTelopBg = CUI::GetAccessUI(2);
            CUI *pTelop = CUI::GetAccessUI(3);
            if (pTelopBg)
            {
                pTelopBg->SetActionReset(0);
                pTelopBg->SetActionLock(0, false);
                pTelopBg->SetActionReset(2);
            }
            if (pTelop)
            {
                pTelop->SetActionReset(0);
                pTelop->SetActionLock(0, false);
                pTelop->SetActionReset(1);
                pTelop->SetActionLock(1, false);
                pTelop->SetActionReset(2);
            }
        }
    }
}

//=============================================================================
// バトル中
// Author : 後藤慎之助
//=============================================================================
void CGame::InButtle(void)
{
    // ボールの予約状況判断（もしフィニッシュになった1Fでも、次のラウンドに予約を持ち越さないように、予約を反映させておく）
    JudgmentShoot();

    switch (m_type)
    {
    case TYPE_ARENA:
        // アリーナモードで1人残ったら
        if (m_nNumDefeatPlayer >= m_nNumAllPlayer - m_nNumDeathPlayer - 1)
        {
            // その残った人を、1位にする
            for (int nCntPlayer = 0; nCntPlayer < m_nNumAllPlayer; nCntPlayer++)
            {
                if (m_apPlayer[nCntPlayer]->GetDisp())
                {
                    m_anPlayerRank[CPlayer::RANK_1] = m_apPlayer[nCntPlayer]->GetIdxControlAndColor();
                    m_anPlayerRankInThisRound[CPlayer::RANK_1] = m_apPlayer[nCntPlayer]->GetIdxControlAndColor();
                }
            }

            // 一撃の瞬間へ
            m_state = STATE_BLOW_MOMENT;

            // 更新を止めておく
            m_bStopObjUpdate = true;

            // 勝者と敗者の注視へ
            CManager::GetCamera()->SetState(CCamera::STATE_FINISH_EACH);

            // このフレームはポーズをさせないため、関数を抜ける
            return;
        }
        break;
    }

    //================================================================================
    // ポーズ処理
    if (!m_bStopObjUpdate)
    {
        const int NO_PAUSE_PLAYER = -1; // 誰もポーズを押していない

        // 変数宣言
        CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();  // キーボード
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();        // コントローラ
        int nNumPausePlayer = NO_PAUSE_PLAYER;  // ポーズを押したプレイヤー

                                                // スタートボタンを押した人を結びつける
        if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            nNumPausePlayer = PLAYER_1;
        }
        else if (pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_START))
        {
            nNumPausePlayer = PLAYER_2;
        }
        else if (pInputJoypad->GetJoypadTrigger(PLAYER_3, CInputJoypad::BUTTON_START))
        {
            nNumPausePlayer = PLAYER_3;
        }
        else if (pInputJoypad->GetJoypadTrigger(PLAYER_4, CInputJoypad::BUTTON_START))
        {
            nNumPausePlayer = PLAYER_4;
        }

        // ポーズするなら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || nNumPausePlayer != NO_PAUSE_PLAYER)
        {
            // SE
            CManager::SoundPlay(CSound::LABEL_SE_INFO);

            // キーボード操作でポーズする際は、1Pのコントローラを使う
            if (nNumPausePlayer == NO_PAUSE_PLAYER)
            {
                nNumPausePlayer = PLAYER_1;
            }

            // ポーズ状態にする
            m_state = STATE_PAUSE_MENU;
            m_pPause->SetPauseMenuSelect(nNumPausePlayer);
            m_bStopObjUpdate = true;

            // カメラも止める
            CManager::GetCamera()->SetState(CCamera::STATE_NONE);
        }
    }
    //================================================================================
}

//=============================================================================
// 一撃の瞬間
// Author : 後藤慎之助
//=============================================================================
void CGame::BlowMoment(void)
{
    // 背景と黒線を取得
    CUI *pBg = CUI::GetAccessUI(100);
    CUI *pLine = CUI::GetAccessUI(101);
    // 背景と黒線を見えるように
    if (pBg)
    {
        pBg->SetDisp(true);
    }
    if (pLine)
    {
        pLine->SetDisp(true);
    }

    // カウンタを加算
    m_nCntGameTime++;

    // 一定フレームで、勝敗判定
    if (m_nCntGameTime >= BLOW_MOMENT_FRAME)
    {
        // 背景と黒線を見えないように
        if (pBg)
        {
            pBg->SetDisp(false);
        }
        if (pLine)
        {
            pLine->SetDisp(false);
        }

        // カウンタをリセット
        m_nCntGameTime = 0;

        // カメラのロックオン場所をリセット
        CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_GAME);

        // 更新開始
        m_bStopObjUpdate = false;

        // 勝敗判定へ
        m_state = STATE_FINISH;

        // ボールは消しておく
        m_pBall->SetDispOff();
    }
}

//=============================================================================
// 勝敗判定
// Author : 後藤慎之助
//=============================================================================
void CGame::JudgmentFinish(void)
{
    // カウンタを加算
    m_nCntGameTime++;

    // 一定フレームで、勝敗判定
    if (m_nCntGameTime >= FINISH_WAIT_FRAME)
    {
        // カウンタをリセット
        m_nCntGameTime = 0;

        // 反転合成を、念のため止める（AIつよい同士で戦わせた際、反転合成が戻らないままゲームが進行したため）（←打ち始め1Fに当たったから?）
        if (m_pEffect2d_Nega && m_pEffect2d_Posi)
        {
            m_pEffect2d_Nega->SetSize(DEFAULT_VECTOR);
            m_pEffect2d_Posi->SetSize(DEFAULT_VECTOR);
            m_pEffect2d_Nega->SetUseUpdate(false);
            m_pEffect2d_Posi->SetUseUpdate(false);
            m_pEffect2d_Nega->SetRotVertex(0.0f);     // 更新を止める代わりに、頂点はここで調整
            m_pEffect2d_Posi->SetRotVertex(0.0f);     // 更新を止める代わりに、頂点はここで調整
        }

        // 死んだプレイヤーが全体のプレイヤー-1に達したら
        if (m_nNumDeathPlayer >= m_nNumAllPlayer - 1)
        {
            // リザルトに移行
            CFade::SetFade(CManager::MODE_RESULT);
        }
        else
        {
            // やられたプレイヤー人数をリセット
            m_nNumDefeatPlayer = 0;

            // ボールをリセットし、最も負けた人に吸収させる
            m_pBall->Reset(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f));
            m_pBall->SetAbsorb(m_apPlayer[m_nWhoWorstPlayer]);

            // もう一度ラウンド開始へ
            m_state = STATE_ROUND_START;
        }
    }
    else if (m_nCntGameTime == FADE_IN_FINISH_TELOP)
    {
        // 死んだプレイヤーが全体のプレイヤー-1に達したら
        if (m_nNumDeathPlayer >= m_nNumAllPlayer - 1)
        {
            // SE
            CManager::SoundPlay(CSound::LABEL_SE_FINISH);

            // フィニッシュ
            CUI *pTelopBg = CUI::GetAccessUI(6);
            CUI *pTelop = CUI::GetAccessUI(7);
            if (pTelopBg)
            {
                pTelopBg->SetActionReset(0);
                pTelopBg->SetActionLock(0, false);
                pTelopBg->SetActionReset(2);
            }
            if (pTelop)
            {
                pTelop->SetActionReset(0);
                pTelop->SetActionLock(0, false);
                pTelop->SetActionReset(1);
                pTelop->SetActionLock(1, false);
                pTelop->SetActionReset(2);
            }
        }
    }
}

//========================================
// シュートの予約
// Author : 後藤慎之助
//========================================
void CGame::ReserveShoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, int nWho)
{
    // 予約情報を更新
    m_aReserveShoot[nWho].attackCenterPos = attackCenterPos;
    m_aReserveShoot[nWho].moveAngle = moveAngle;
    m_aReserveShoot[nWho].fPower = fPower;
    m_aReserveShoot[nWho].bFirstCollision = bFirstCollision;
    m_aReserveShoot[nWho].flag = flag;
    m_aReserveShoot[nWho].bReserved = true;
}

//========================================
// 誰がシュートを撃てたかの判定
// Author : 後藤慎之助
//========================================
void CGame::JudgmentShoot(void)
{
    // 定義
    const int NO_PLAYER = -1; // 誰も打てなかった
    
    // 変数宣言
    int nNumShootingPlayer = NO_PLAYER;  // シュートを打つプレイヤー
    int nNumAbsorbingPlayer = NO_PLAYER; // 吸収するプレイヤー
    bool abSetOff[MAX_PLAYER];           // 相殺するプレイヤー
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        abSetOff[nCnt] = false;
    }

    // 予約状況をチェック
    int nNumReservingPlayer = 0;        // 予約中のプレイヤー人数
    int nNumFirstCollisionPlayer = 0;   // 最初の接触をするプレイヤー
    for (int nCnt = 0; nCnt < m_nNumAllPlayer; nCnt++)
    {
        if (m_aReserveShoot[nCnt].bReserved)
        {
            // 予約中なら、予約を反映
            m_aReserveShoot[nCnt].bReserved = false;
            nNumReservingPlayer++;

            // 最初の接触ではなく、すでに硬直中ならほぼ確実に打てる（吸収だけには負ける）
            if (!m_aReserveShoot[nCnt].bFirstCollision)
            {
                if (IS_BITOFF(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_ABSORB))
                {
                    nNumShootingPlayer = nCnt;
                }
                else
                {
                    nNumAbsorbingPlayer = nCnt;
                }
            }
            else
            {
                // 最初の接触なら、他のプレイヤーと相殺の競争
                abSetOff[nCnt] = true;
                nNumFirstCollisionPlayer = nCnt;
            }
        }
    }

    // 相殺状況をチェック
    if (nNumReservingPlayer >= 2)
    {
        bool bUseWave = false;
        D3DXVECTOR3 setOffPos = m_pBall->GetPos();
        for (int nCnt = 0; nCnt < m_nNumAllPlayer; nCnt++)
        {
            if (abSetOff[nCnt])
            {
                // 相殺のノックバック
                m_apPlayer[nCnt]->TakeDamage(0.0f, nCnt, setOffPos, setOffPos, false, true);

                // 波と音発生
                if (!bUseWave)
                {
                    bUseWave = true;
                    CWave::Create(setOffPos, BALL_WAVE_FIRST_SIZE);
                    CManager::SoundPlay(CSound::LABEL_SE_OFFSET);
                }
            }
        }

        // 相殺に打ち勝った人がいないなら
        if (nNumShootingPlayer == NO_PLAYER && nNumAbsorbingPlayer == NO_PLAYER)
        {
            // ボールを打ち上げる
            m_pBall->Launch(setOffPos);
        }
    }

    // 吸収者がいるなら
    if (nNumAbsorbingPlayer != NO_PLAYER)
    {
        // 打つのを横取り
        nNumShootingPlayer = NO_PLAYER;
        nNumReservingPlayer = 1;
        nNumFirstCollisionPlayer = nNumAbsorbingPlayer;
    }

    // 誰かがシュートを打つなら、シュート
    if (nNumShootingPlayer != NO_PLAYER)
    {
        m_pBall->Shoot(m_aReserveShoot[nNumShootingPlayer].attackCenterPos,
            m_aReserveShoot[nNumShootingPlayer].moveAngle,
            m_aReserveShoot[nNumShootingPlayer].fPower,
            m_aReserveShoot[nNumShootingPlayer].bFirstCollision,
            m_aReserveShoot[nNumShootingPlayer].flag,
            m_apPlayer[nNumShootingPlayer]);
    }
    else
    {
        // 1人のみの予約なら、最初の接触
        if (nNumReservingPlayer == 1)
        {
            // 最初の接触は、硬直時間を結びつける
            m_apPlayer[nNumFirstCollisionPlayer]->SetStopTime(m_pBall->Shoot(m_aReserveShoot[nNumFirstCollisionPlayer].attackCenterPos,
                m_aReserveShoot[nNumFirstCollisionPlayer].moveAngle,
                m_aReserveShoot[nNumFirstCollisionPlayer].fPower,
                m_aReserveShoot[nNumFirstCollisionPlayer].bFirstCollision,
                m_aReserveShoot[nNumFirstCollisionPlayer].flag,
                m_apPlayer[nNumFirstCollisionPlayer]));

            // 必殺ゲージ上昇（バントと吸収は上がらず、キャッチは追加で更に上がる）
            if (IS_BITOFF(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_BUNT) &&
                IS_BITOFF(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_ABSORB))
            {
                if (IS_BITON(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_THROW))
                {
                    m_apPlayer[nNumFirstCollisionPlayer]->GainSpGauge(true);
                }
                else
                {
                    // 通常攻撃のエフェクト
                    switch (m_apPlayer[nNumFirstCollisionPlayer]->GetIdxControlAndColor())
                    {
                    case PLAYER_1:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_1P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_1P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    case PLAYER_2:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_2P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_2P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    case PLAYER_3:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_3P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_3P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    case PLAYER_4:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_4P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_4P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    }

                    m_apPlayer[nNumFirstCollisionPlayer]->GainSpGauge();
                }
            }
        }
    }
}

//========================================
// 一番近いプレイヤーへの角度を求める
// Author : 後藤慎之助
//========================================
float CGame::GetAngleToClosestPlayer(int nIdxPlayer, D3DXVECTOR3 myPos)
{
    // 変数宣言
    float fAngle = 0.0f;                        // 返す角度
    float fFirstDistance = 99999.9f;            // 距離
    D3DXVECTOR3 targetPos = DEFAULT_VECTOR;     // 対象の位置

    // 対象の位置を、自分の位置の真上へ一度決めておく
    targetPos = D3DXVECTOR3(myPos.x, fFirstDistance, 0.0f);

    // 距離が一番近いプレイヤーを決める（自分以外で）
    for (int nCntPlayer = 0; nCntPlayer < m_nNumAllPlayer; nCntPlayer++)
    {
        // 自分以外で
        if (nIdxPlayer == nCntPlayer)
        {
            continue;
        }

        // 生存しているなら
        if (m_apPlayer[nCntPlayer]->GetDisp())
        {
            // 他のプレイヤーの位置
            D3DXVECTOR3 otherPlayerPos = m_apPlayer[nCntPlayer]->GetPos();

            // 距離を求める
            float fSecondDistance = sqrtf(
                powf((myPos.x - otherPlayerPos.x), 2) +
                powf((myPos.y - otherPlayerPos.y), 2));

            // 距離の比較と、対象の位置を更新
            if (fFirstDistance > fSecondDistance)
            {
                fFirstDistance = fSecondDistance;
                targetPos = otherPlayerPos;
            }
        }
    }

    // 角度を求める
    fAngle = atan2((targetPos.x - myPos.x), (targetPos.y - myPos.y));

    return fAngle;
}

//========================================
// 一番近いプレイヤーの位置を求める
// Author : 後藤慎之助
//========================================
D3DXVECTOR3 CGame::GetPosToClosestPlayer(int nIdxPlayer, D3DXVECTOR3 myPos)
{
    // 変数宣言
    float fFirstDistance = 99999.9f;            // 距離
    D3DXVECTOR3 targetPos = DEFAULT_VECTOR;     // 対象の位置

    // 対象の位置を、自分の位置の真上へ一度決めておく
    targetPos = D3DXVECTOR3(myPos.x, fFirstDistance, 0.0f);

    // 距離が一番近いプレイヤーを決める（自分以外で）
    for (int nCntPlayer = 0; nCntPlayer < m_nNumAllPlayer; nCntPlayer++)
    {
        // 自分以外で
        if (nIdxPlayer == nCntPlayer)
        {
            continue;
        }

        // 生存しているなら
        if (m_apPlayer[nCntPlayer]->GetDisp())
        {
            // 他のプレイヤーの位置
            D3DXVECTOR3 otherPlayerPos = m_apPlayer[nCntPlayer]->GetPos();

            // 距離を求める
            float fSecondDistance = sqrtf(
                powf((myPos.x - otherPlayerPos.x), 2) +
                powf((myPos.y - otherPlayerPos.y), 2));

            // 距離の比較と、対象の位置を更新
            if (fFirstDistance > fSecondDistance)
            {
                fFirstDistance = fSecondDistance;
                targetPos = otherPlayerPos;
            }
        }
    }

    return targetPos;
}

//========================================
// ボール発射ゲージの設定
// Author : 後藤慎之助
//========================================
void CGame::SetBallGauge(int nMax, int nNow)
{
    // UIを取得
    CUI *pBallGaugeR = CUI::GetAccessUI(0);
    CUI *pBallGaugeL = CUI::GetAccessUI(1);

    // 左右のゲージを設定
    if (pBallGaugeR)
    {
        pBallGaugeR->SetLeftToRightGauge((float)nMax, (float)nNow);
    }
    if (pBallGaugeL)
    {
        pBallGaugeL->SetRightToLeftGauge((float)nMax, (float)nNow);
    }
}
