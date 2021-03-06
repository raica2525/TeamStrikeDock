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
#include "player.h"
#include "camera.h"
#include "ball.h"
#include "bg.h"
#include "wave.h"

//========================================
// マクロ定義
//========================================

// ゲーム状態の管理フレーム
#define FINISH_WAIT_FRAME 240   // 決着時に、待つフレーム数
#define CREATE_POS_Y_RATE 0.8f  // ボールの発生位置Yの割合

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CGame::m_bStopObjUpdate = false;

CPlayer *CGame::m_apPlayer[] = {};
CBall *CGame::m_pBall = NULL;
CScore *CGame::m_pScore = NULL;
CPause *CGame::m_pPause = NULL;

CGame::TYPE CGame::m_type = TYPE_TRAINING;
CGame::STATE CGame::m_state = STATE_ROUND_START;
CGame::MAP_LIMIT CGame::m_mapLimit = {};
int CGame::m_nNumAllPlayer = 0;
int CGame::m_nNumDefeatPlayer = 0;
int CGame::m_nWhoWorstPlayer = PLAYER_1;
int CGame::m_nNumDeathPlayer = 0;
CGame::RESERVE_SHOOT CGame::m_aReserveShoot[] = {};

//=============================================================================
// ゲームのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CGame::CGame()
{
    m_bStopObjUpdate = false;

    memset(m_apPlayer, 0, sizeof(m_apPlayer));
    m_pBall = NULL;
    m_pScore = NULL;
    m_pPause = NULL;

    // 静的メンバ変数を初期化（遷移時に毎回必要なものだけ）
    m_type = TYPE_TRAINING;
    m_state = STATE_ROUND_START;
    m_nNumAllPlayer = 0;
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
    // 仮のゲーム状態決定
    m_type = TYPE_ARENA;
    m_state = STATE_ROUND_START;

    // 定義
    const float SPLIT_RATE_UNDER_3 = 0.5f;
    const float SPLIT_RATE_ABOVE_2 = 0.333f;
    int NUM_PLAYER = 4; // 仮(カスタマイズ画面の情報をもとに決める)
    const int NUM_STOCK = 3; // 仮(カスタマイズ画面の情報をもとに決める)
    if (m_type == TYPE_TRAINING)
    {
        NUM_PLAYER = 1; // トレーニングは1人固定
    }

    // 仮ステージのモデルを生成（カスタマイズ画面の情報を元に決める）
    CBg::Create();

    // 仮UIを生成（カスタマイズ画面の情報を元に決める）
    CUI::Place(CUI::SET_GAME);

    //// スコアの生成
    //m_pScore = CScore::Create(SCORE_POS, SCORE_SIZE);

    // ポーズの生成
    m_pPause = CPause::Create();

    // 仮(8体でも重かったから、何らかの負荷軽減が必要（モデルのポリゴン数削減（2万程度）か、描画周り）)
    //for (int nCnt = 0; nCnt < 16; nCnt++)
    //{
    //    // ポリゴン数を近接頂点をくっつけることでなんとか削減（32体耐え）
    //    float fPosX = 200.0f * nCnt;
    //    CPlayer::Create(D3DXVECTOR3(-1000.0f + fPosX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), CPlayer::PLAYABLE_001);
    //}
    // プレイヤーの生成
    D3DXVECTOR3 player1Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player2Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player3Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player4Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    float fSplitXRate = 0.0f;
    switch (NUM_PLAYER)
    {
    case 1:
        fSplitXRate = SPLIT_RATE_UNDER_3;
        player1Pos.x *= -fSplitXRate;
        m_apPlayer[0] = CPlayer::Create(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_001/*, CPlayer::AI_LEVEL_1*/);
        break;
    case 2:
        fSplitXRate = SPLIT_RATE_UNDER_3;
        player1Pos.x *= -fSplitXRate;
        player2Pos.x *= fSplitXRate;
        m_apPlayer[0] = CPlayer::Create(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_001/*, CPlayer::AI_LEVEL_1*/);
        m_apPlayer[1] = CPlayer::Create(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_002, CPlayer::AI_LEVEL_1);
        break;
    case 3:
        fSplitXRate = SPLIT_RATE_ABOVE_2;
        player1Pos.x *= -fSplitXRate * 2;
        player2Pos.x *= -fSplitXRate;
        player3Pos.x *= fSplitXRate;
        m_apPlayer[0] = CPlayer::Create(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_001/*, CPlayer::AI_LEVEL_1*/);
        m_apPlayer[1] = CPlayer::Create(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_002/*, CPlayer::AI_LEVEL_1*/);
        m_apPlayer[2] = CPlayer::Create(player3Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_003/*, CPlayer::AI_LEVEL_1*/);
        break;
    case 4:
        fSplitXRate = SPLIT_RATE_ABOVE_2;
        player1Pos.x *= -fSplitXRate * 2;
        player2Pos.x *= -fSplitXRate;
        player3Pos.x *= fSplitXRate;
        player4Pos.x *= fSplitXRate * 2;
        m_apPlayer[0] = CPlayer::Create(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_001/*, CPlayer::AI_LEVEL_1*/);
        m_apPlayer[1] = CPlayer::Create(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_002, CPlayer::AI_LEVEL_1);
        m_apPlayer[2] = CPlayer::Create(player3Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_003, CPlayer::AI_LEVEL_2);
        m_apPlayer[3] = CPlayer::Create(player4Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), NUM_STOCK, CPlayer::PLAYABLE_004, CPlayer::AI_LEVEL_3);
        break;
    }

    // カメラのロックオン場所を変える
    CManager::GetCamera()->CCamera::ResetGameCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT);

    // ボールを生成
    m_pBall = CBall::Create(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f), true);
    //m_pBall->SetAbsorb(m_apPlayer[0]);  // 仮に吸収させている

    // ステージの設定
    //SetStage(STAGE_1);

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

#ifdef _DEBUG
    CDebug::GameCommand();
#endif
}

//=============================================================================
// ゲーム状態の管理
// Author : 後藤慎之助
//=============================================================================
void CGame::ManageState(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

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

        // 全員SPゲージはリセット
        m_apPlayer[nCntPlayer]->SetSpGaugeCurrent(0.0f);
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
            // フィニッシュへ
            m_state = STATE_FINISH;

            // ボールは消しておく
            m_pBall->SetDispOff();
        }
        break;
    }

    // 定義
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
        // キーボード操作でポーズする際は、1Pのコントローラを使う
        if (nNumPausePlayer == NO_PAUSE_PLAYER)
        {
            nNumPausePlayer = PLAYER_1;
        }

        //// ポーズ音
        //pSound->Play(CSound::LABEL_SE_SYSTEM_PAUSE);

        // ポーズ状態にする
        m_state = STATE_PAUSE_MENU;
        m_pPause->SetPauseMenuSelect(nNumPausePlayer);
        m_bStopObjUpdate = true;

        // カメラも止める
        CManager::GetCamera()->SetState(CCamera::STATE_NONE);
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

        // 死んだプレイヤーが全体のプレイヤー-1に達したら
        if (m_nNumDeathPlayer >= m_nNumAllPlayer - 1)
        {
            // フェードしていないなら
            if (CFade::GetFade() == CFade::FADE_NONE)
            {
                // 仮にタイトルに移行
                CFade::SetFade(CManager::MODE_TITLE);
            }
        }
        else
        {
            // やられたプレイヤー人数をリセット
            m_nNumDefeatPlayer = 0;

            // ボールをリセットし、最も負けた人に吸収させる
            m_pBall->Reset(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f));
            m_pBall->SetAbsorb(m_apPlayer[m_nWhoWorstPlayer]);

            //// 最も負けた人をリセット（デバッグログが見づらくなるためコメントアウト）
            //m_nWhoWorstPlayer = 0;

            // 仮にもう一度ステージ開始へ
            m_state = STATE_ROUND_START;
        }
    }
}

//========================================
// シュートの予約
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
                m_apPlayer[nCnt]->TakeDamage(0.0f, nCnt, setOffPos, false, true);

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
                    m_apPlayer[nNumFirstCollisionPlayer]->GainSpGauge();
                }
            }
        }
    }
}

//========================================
// 一番近いプレイヤーへの角度を求める
//========================================
float CGame::GetAngleToClosestPlayer(int playable, D3DXVECTOR3 myPos)
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
        if (playable == nCntPlayer)
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