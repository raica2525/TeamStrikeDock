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
//#include "block.h"
#include "input.h"
#include "scene.h"
#include "library.h"
#include "debug.h"
//#include "score.h"
//#include "item.h"
#include "pause.h"
//#include "enemy.h"
#include "scene2d.h"
//#include "monster.h"
#include "player.h"
#include "camera.h"
#include "ball.h"
#include "bg.h"

//========================================
// マクロ定義
//========================================

// ゲーム状態の管理フレーム
#define START_MISSION 30    // ミッションの開始

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPlayer *CGame::m_apPlayer[] = {};
CBall *CGame::m_pBall = NULL;
CScore *CGame::m_pScore = NULL;
CPause *CGame::m_pPause = NULL;

CGame::STATE CGame::m_state = STATE_BUTTLE;
CGame::MAP_LIMIT CGame::m_mapLimit = {};
int CGame::m_nNumCurrentPlayer = 0;

//=============================================================================
// ゲームのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CGame::CGame()
{
    memset(m_apPlayer, 0, sizeof(m_apPlayer));
    m_pBall = NULL;
    m_pScore = NULL;
    m_pPause = NULL;

    // 静的メンバ変数を初期化（遷移時に毎回必要なものだけ）
    m_state = STATE_BUTTLE; // 仮にバトル中からにしている
    m_nNumCurrentPlayer = 0;

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
    // UIを生成
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
    m_apPlayer[0] = CPlayer::Create(PLAYER_START_POS1, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), CPlayer::PLAYABLE_001);
    m_apPlayer[1] = CPlayer::Create(PLAYER_START_POS2, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), CPlayer::PLAYABLE_002/*, CPlayer::AI_LEVEL_1*/);

    // カメラのロックオン場所を変える
    CManager::GetCamera()->CCamera::ResetGameCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT);

    // ボールを生成
    m_pBall = CBall::Create(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * BALL_START_BUTTLE_CREATE_POS_Y_RATE, 0.0f), true);
    m_pBall->SetAbsorb(m_apPlayer[0]);  // 仮に吸収させている

    // ステージのモデルを生成
    CBg::Create();

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
    // 確保したスコアとプレイヤーと各数字UIのメモリは、CSceneのReleaseAllで開放されている

    // 全ての音を停止
    CSound *pSound = CManager::GetSound();
    pSound->StopAll();

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

////=============================================================================
//// ステージの読み込み
//// Author : 後藤慎之助
////=============================================================================
//void CGame::SetStage(STAGE stage)
//{
//    // 引数をメンバ変数に結びつける
//    m_stage = stage;
//
//    // ミッションクリアのフラグをfalseに戻す
//    m_bMissionClear = false;
//
//    //// プレイヤーの場所を変える
//    //m_pPlayer->SetPos(PLAYER_START_POS_STAGE1);
//
//    // ステージによって、マップの制限やミッションUIを変える
//    CSound *pSound = CManager::GetSound();
//    // BGMを再生
//    pSound->Play(CSound::LABEL_BGM_STAGE_01_NORMAL);
//
//    // 敵の配置読み込み
//    LoadEnemyData(stage);
//
//    // マップの読み込み
//    LoadMapData(stage);
//}

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

    // 音を取得
    CSound *pSound = CManager::GetSound();

    // カメラ取得
    CCamera*pCamera = CManager::GetCamera();

    switch (m_state)
    {
    case STATE_ROUND_START:

        // カウンタを加算
        m_nCntGameTime++;

        // カメラ追従
        pCamera->CCamera::SetState(CCamera::STATE_HORIZON_MOVE);

        // 一定フレームで、ゲーム中に
        if (m_nCntGameTime == START_MISSION)
        {
            // カウンタリセット
            m_nCntGameTime = 0;

            //// ミッションスタート表示
            //CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_START);

            // バトルに
            m_state = STATE_BUTTLE;
        }

        break;

    case STATE_BUTTLE:

        // カメラ追従
        pCamera->CCamera::SetState(CCamera::STATE_HORIZON_MOVE);

        //// バトル中
        //Buttle();

        break;

    //case STATE_FINISH:

    //    // 勝敗判定
    //    Judgment();

    //    break;

    case STATE_PAUSE:

        // ポーズの更新
        m_pPause->Update();

        break;
    }
}

//=============================================================================
// バトル中
// Author : 後藤慎之助
//=============================================================================
void CGame::Buttle(void)
{

}

//=============================================================================
// 勝敗判定
// Author : 後藤慎之助
//=============================================================================
void CGame::Judgment(void)
{
    // カウンタを加算
    m_nCntGameTime++;

    // 最初の1Fで判断して、UIを出す
    if (m_nCntGameTime == 1)
    {
        // 音取得
        CSound *pSound = CManager::GetSound();

        //// ステージのBGMを消す
        //switch (m_stage)
        //{
        //case STAGE_1:
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_01_NORMAL);
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_01_BOSS);
        //    break;
        //case STAGE_2:
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_02_NORMAL);
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_02_BOSS);
        //    break;
        //case STAGE_FINAL:
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_03_NORMAL);
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_03_BOSS);
        //    break;
        //}

        //// ミッションクリアしたなら
        //if (m_bMissionClear == true)
        //{
        //    // クリア音
        //    pSound->Play(CSound::LABEL_SE_CLEAR);

        //    // ミッションクリアのUI
        //    CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_CLEAR);
        //}
        //// ミッション失敗したなら
        //else
        //{
        //    // 失敗音
        //    pSound->Play(CSound::LABEL_SE_FAIL);

        //    // ミッション失敗のUI
        //    CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_FAILD);
        //}
    }

    // 次の画面に行くまでのカウンタ
    if (m_nCntGameTime == COUNT_WAIT_FINISH)
    {
        // カウンタをリセット
        m_nCntGameTime = 0;

        //// 状態を、フェード中に
        //m_state = STATE_IN_FADE;

        //// ミッションに失敗した、または最終ステージを終えたなら
        //if (m_bMissionClear == false || m_stage == STAGE_FINAL)
        //{
        //    // スコアをマネージャに渡す
        //    CManager::SetScore(m_pScore->GetScore());

        //    // リザルトに移行
        //    CFade::SetFade(CManager::MODE_RESULT);
        //}
        //// 次のステージへ
        //else
        //{
        //    // ホワイトフェード
        //    switch (m_stage)
        //    {
        //    case STAGE_1:
        //        CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE1);
        //        break;

        //    case STAGE_2:
        //        CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE2);
        //        break;
        //    }
        //}
    }
}

////=============================================================================
//// マップの読み込み
//// Author : 後藤慎之助
////=============================================================================
//void CGame::LoadMapData(STAGE stage)
//{
//    //// ファイルポイント
//    //FILE *pFile = NULL;
//
//    //// 変数宣言
//    //int  nCntLoad = 0;
//    //char cReedText[128];	// 文字として読み取り用
//    //char cHeadText[256];	//
//    //char cDie[128];
//
//    //// デバッグ用の変数
//    //int nType[256];
//    //int nWidth = 0; // 横
//    //int nDepth = 0; // 奥行
//    //D3DXVECTOR3 pos[256];
//    //D3DXVECTOR3 rot[256];
//
//    //// ステージによって、ファイルを開く
//    //switch (stage)
//    //{
//    //case STAGE_1:
//    //    pFile = fopen("data/TXT/stage1.txt", "r");
//    //    break;
//    //case STAGE_2:
//    //    pFile = fopen("data/TXT/stage2.txt", "r");
//    //    break;
//    //case STAGE_FINAL:
//    //    pFile = fopen("data/TXT/stage3.txt", "r");
//    //    break;
//    //}
//
//    //// 開けた
//    //if (pFile != NULL)
//    //{
//    //    while (strcmp(cHeadText, "MODELSET") != 0)
//    //    {
//    //        fgets(cReedText, sizeof(cReedText), pFile);
//    //        sscanf(cReedText, "%s", &cHeadText);
//
//    //        if (strcmp(cHeadText, "MAP_SIZ") == 0)
//    //        {
//    //            sscanf(cReedText, "%s %s %d %d", &cDie, &cDie, &nWidth, &nDepth);
//    //        }
//    //    }
//
//    //    if (strcmp(cHeadText, "MODELSET") == 0)
//    //    {
//    //        while (strcmp(cHeadText, "END_SCRIPT") != 0)
//    //        {
//    //            fgets(cReedText, sizeof(cReedText), pFile);
//    //            sscanf(cReedText, "%s", &cHeadText);
//
//    //            if (strcmp(cHeadText, "\n") == 0)
//    //            {
//    //            }
//    //            else if (strcmp(cHeadText, "END_MODELSET") != 0)
//    //            {
//    //                if (strcmp(cHeadText, "TYPE") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
//    //                }
//
//    //                if (strcmp(cHeadText, "POS") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
//    //                }
//
//    //                if (strcmp(cHeadText, "ROT") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);
//
//    //                    // ブロックを生成
//    //                    CBlock::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                        D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), nType[nCntLoad]);
//
//    //                    nCntLoad++;
//    //                }
//    //            }
//    //        }
//
//    //    }
//
//    //    // フィールド生成
//    //    CField::Create(stage, nWidth, nDepth);
//
//    //    // ファイル閉
//    //    fclose(pFile);
//    //}
//
//    //// 開けない
//    //else
//    //{
//    //    printf("開けれませんでした\n");
//    //}
//}

////=============================================================================
//// エネミーの配置読み込み
//// Author : 後藤慎之助
////=============================================================================
//void CGame::LoadEnemyData(STAGE stage)
//{
//    //// ファイルポイント
//    //FILE *pFile = NULL;
//
//    //// 変数宣言
//    //int  nCntLoad = 0;
//    //char cReedText[128];	// 文字として読み取り用
//    //char cHeadText[256];	//
//    //char cDie[128];
//
//    //// デバッグ用の変数
//    //int nType[256];
//    //D3DXVECTOR3 pos[256];
//    //D3DXVECTOR3 rot[256];
//
//    //// ステージによって、ファイルを開く
//    //switch (stage)
//    //{
//    //case STAGE_1:
//    //    pFile = fopen("data/TXT/enemystage1.txt", "r");
//    //    break;
//    //case STAGE_2:
//    //    pFile = fopen("data/TXT/enemystage2.txt", "r");
//    //    break;
//    //case STAGE_FINAL:
//    //    pFile = fopen("data/TXT/enemystage3.txt", "r");
//    //    break;
//    //}
//
//    //// 開けた
//    //if (pFile != NULL)
//    //{
//    //    while (strcmp(cHeadText, "ENEMYSET") != 0)
//    //    {
//    //        fgets(cReedText, sizeof(cReedText), pFile);
//    //        sscanf(cReedText, "%s", &cHeadText);
//    //    }
//
//    //    if (strcmp(cHeadText, "ENEMYSET") == 0)
//    //    {
//    //        while (strcmp(cHeadText, "END_SCRIPT") != 0)
//    //        {
//    //            fgets(cReedText, sizeof(cReedText), pFile);
//    //            sscanf(cReedText, "%s", &cHeadText);
//
//    //            if (strcmp(cHeadText, "\n") == 0)
//    //            {
//    //            }
//    //            else if (strcmp(cHeadText, "END_ENEMYSET") != 0)
//    //            {
//    //                if (strcmp(cHeadText, "TYPE") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
//    //                }
//
//    //                if (strcmp(cHeadText, "POS") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
//    //                }
//
//    //                if (strcmp(cHeadText, "ROT") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);
//
//    //                    switch (nType[nCntLoad])
//    //                    {
//    //                    case ENEMYSET_WHITE_ANT:
//    //                        // 白アリを生成
//    //                        CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_WHITE);
//    //                        break;
//    //                    case ENEMYSET_BLACK_ANT:
//    //                        // 巨大黒アリを生成
//    //                        CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_BLACK);
//    //                        break;
//    //                    case ENEMYSET_YELLOW_SPIDER:
//    //                        // 黄グモを生成
//    //                        CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_YELLOW);
//    //                        break;
//    //                    case ENEMYSET_BLACK_SPIDER:
//    //                        // 巨大黒グモを生成
//    //                        CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_BLACK);
//    //                        break;
//    //                    case ENEMYSET_UFO:
//    //                        // UFOを生成
//    //                        CUfo::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)));
//    //                        break;
//    //                    }
//
//    //                    nCntLoad++;
//    //                }
//    //            }
//    //        }
//
//    //    }
//    //    // ファイル閉
//    //    fclose(pFile);
//    //}
//
//    //// 開けない
//    //else
//    //{
//    //    printf("開けれませんでした\n");
//    //}
//}

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
    targetPos = D3DXVECTOR3(myPos.x, 5000.0f, 0.0f);

    // 距離が一番近いプレイヤーを決める（自分以外で）
    for (int nCntPlayer = 0; nCntPlayer < m_nNumCurrentPlayer; nCntPlayer++)
    {
        // 自分以外で
        if (playable == nCntPlayer)
        {
            continue;
        }

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

    // 角度を求める
    fAngle = atan2((targetPos.x - myPos.x), (targetPos.y - myPos.y));

    return fAngle;
}
//
////========================================
//// 一番近い敵の位置を求める
////========================================
//D3DXVECTOR3 CGame::GetPosToClosestEnemy(D3DXVECTOR3 myPos)
//{
//    // 変数宣言
//    float fFirstDistance = BONDS_MAX_DISTANCE_TO_CLOSEST_ENEMY;  // 距離
//    D3DXVECTOR3 targetPos = DEFAULT_VECTOR;                      // 対象の位置
//
//                                                                 // 対象の位置を、自分の位置の真上へ一度決めておく
//    targetPos = D3DXVECTOR3(myPos.x, 0.0f, 0.0f);
//
//    // 距離が一番近い敵を決める
//    CScene *pScene = NULL;
//    pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
//    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
//    {
//        // 中身があるなら
//        if (pScene != NULL)
//        {
//            // 次のシーンを記憶
//            CScene*pNextScene = pScene->GetNextScene();
//
//            // エネミーにキャスト
//            CEnemy *pEnemy = (CEnemy*)pScene;
//
//            // 位置を求める
//            D3DXVECTOR3 enemyPos = DEFAULT_VECTOR;
//            enemyPos = pEnemy->GetPosition();
//
//            // 距離を求める
//            float fSecondDistance = sqrtf(
//                powf((myPos.x - enemyPos.x), 2) +
//                powf((myPos.y - enemyPos.y), 2));
//
//            // 距離の比較と、対象の位置を更新
//            if (fFirstDistance > fSecondDistance)
//            {
//                fFirstDistance = fSecondDistance;
//                targetPos = enemyPos;
//            }
//
//            // 次のシーンにする
//            pScene = pNextScene;
//        }
//        else
//        {
//            // 中身がないなら、そこで処理を終える
//            break;
//        }
//    }
//
//    return targetPos;
//}
