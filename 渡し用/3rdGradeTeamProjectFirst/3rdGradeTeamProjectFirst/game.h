//=============================================================================
//
// ゲーム処理 [game.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "mode.h"
#include "player.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBall;
class CPause;
class CUI;
class CEffect2D;
class CNumberArray;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CMode
{
public:

    // ゲームの種類
    typedef enum
    {
        TYPE_TRAINING = 0,     // トレーニング
        TYPE_ARENA,            // 闘技場
        TYPE_MINI,             // ミニゲーム
        TYPE_MAX			   // 状態の最大数
    }TYPE;

    // ゲームの状態（仮）
    typedef enum
    {
        STATE_ROUND_START = 0,     // 開始
        STATE_BUTTLE,              // バトル中
        STATE_BLOW_MOMENT,         // 一撃の瞬間
        STATE_FINISH,              // 決着
        STATE_PAUSE_MENU,	       // ポーズメニュー
        STATE_MAX			       // 状態の最大数
    }STATE;

    // マップ制限
    typedef struct
    {
        float fHeight;
        float fWidth;
    }MAP_LIMIT;

    // シュートの予約
    typedef struct
    {
        D3DXVECTOR3 attackCenterPos;    // 攻撃の中心位置
        D3DXVECTOR3 moveAngle;          // 移動角度
        float fPower;                   // パワー
        bool bFirstCollision;           // 硬直前のファーストヒットか
        int flag;                       // ボールへのシュートフラグ
        bool bReserved;                 // 予約中かどうか
    }RESERVE_SHOOT;

    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

    void ManageState(void);     // ゲーム状態の管理
    void RoundStart(void);      // ラウンド開始
    void InButtle(void);        // バトル中
    void BlowMoment(void);      // 一撃の瞬間
    void JudgmentFinish(void);  // 勝敗判定

    /*========================================================
    // セッター
    //======================================================*/
    static void SetNextGameInDebug(TYPE gameType, int nNumPlayer, int nNumStock, bool bUseKeyboard = false) 
    {
        m_type = gameType;  m_nNumAllPlayer = nNumPlayer; m_nNumStock = nNumStock; m_bUseKeyboard = bUseKeyboard;
    }   // デバッグメニューで、次のゲームを設定
    static void SetNextGameInCustom(TYPE gameType, int nNumPlayer)
    {
        m_type = gameType;  m_nNumAllPlayer = nNumPlayer;
    }   // カスタマイズ画面で、次のゲームを設定
    static void SetIdxPlayer(int nNum, int nIdxPlayer) { m_anMemoryIdxPlayer[nNum] = nIdxPlayer; }  // プレイヤーのインデックス
    static void SetAILevel(int nNum, CPlayer::AI_LEVEL level) { m_aMemoryAILevel[nNum] = level; }   // AIレベル
    static void SetAddNumDefeatPlayer(int nIdxPlayer) { SetPlayerRankInThisRound(nIdxPlayer); m_nNumDefeatPlayer++; }                       // やられたプレイヤー人数を増やす
    static void SetWorstPlayer(int player) { m_nWhoWorstPlayer = player; }                  // ワーストのプレイヤーを決める
    static void SetAddNumDeathPlayer(int nIdxPlayer) { SetPlayerRankInThisRound(nIdxPlayer); m_anPlayerRank[m_nNumAllPlayer - m_nNumDeathPlayer - 1] = nIdxPlayer; m_nNumDeathPlayer++; } // 死んだプレイヤー番号を記憶し、カウント
    static void SetQuitPause(void) { m_state = STATE_BUTTLE; m_bStopObjUpdate = false; }    // ポーズ状態をやめる
    static void SetStock(int nStock) { m_nNumStock = nStock; }
    static void SetUseKeyboard(bool bUseKeyboard) { m_bUseKeyboard = bUseKeyboard; }
    static void SetPlayerRankInThisRound(int nIdxPlayer)
    { 
        int nRank = GetNumCurrentDispPlayer() - 1;
        if (nRank < CPlayer::RANK_1)
        {
            nRank = CPlayer::RANK_2;    // 念のため、不正な配列を参照しないようにする
        }
        m_anPlayerRankInThisRound[nRank] = nIdxPlayer;
    }

    /*========================================================
    // ゲッター
    //======================================================*/
    static STATE GetState(void) { return m_state; }                                 // ゲームの状態を取得
    static CPlayer* GetPlayer(const int nNum) { return m_apPlayer[nNum]; }          // プレイヤーを取得
    static CBall* GetBall(void) { return m_pBall; }                                 // ボールを取得
    static int GetNumAllPlayer(void) { return m_nNumAllPlayer; }                    // 全体プレイヤー人数を取得
    static int GetNumDefeatPlayer(void) { return m_nNumDefeatPlayer; }              // やられたプレイヤー人数を取得
    static int GetWhoWorstPlayer(void) { return m_nWhoWorstPlayer; }                // ワーストのプレイヤーを取得
    static int GetNumDeathPlayer(void) { return m_nNumDeathPlayer; }                // 死んだプレイヤー人数を取得
    static MAP_LIMIT GetMapLimit(void) { return m_mapLimit; }                       // マップ制限を取得
    static TYPE GetType(void) { return m_type; }                                    // ゲーム種類を取得
    static bool GetStopObjUpdate(void) { return m_bStopObjUpdate; }                 // オブジェクトのアップデートを止めるかどうか
    static int GetNumCurrentDispPlayer(void) { return (m_nNumAllPlayer - m_nNumDeathPlayer - m_nNumDefeatPlayer); } // 現在生きているプレイヤー人数を取得
    static CEffect2D* GetNega(void) { return m_pEffect2d_Nega; }                    // 反転合成のエフェクトを取得
    static CEffect2D* GetPosi(void) { return m_pEffect2d_Posi; }                    // 反転合成を戻すためのエフェクトを取得
    static CNumberArray* GetNumArrayBallSpd(void) { return m_pNumArray_BallSpd; }   // ボールスピード表示を取得
    static bool GetUseKeyboard(void) { return m_bUseKeyboard; }                     // キーボードを使うかどうか取得
    static int GetStock(void) { return m_nNumStock; }                               // ストック数を取得
    static int GetPlayerRank(int nIdx) { return m_anPlayerRank[nIdx]; }             // プレイヤーの順位を取得
    static int GetPlayerRankInThisRound(int nIdx) { return m_anPlayerRankInThisRound[nIdx]; }  // 現ラウンドのプレイヤーの順位を取得

    /*========================================================
    // 便利な関数
    //======================================================*/
    static void ReserveShoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, int nWho);
    void JudgmentShoot(void);                                                       // 誰がシュートを打てたかの判定
    static float GetAngleToClosestPlayer(int nIdxPlayer, D3DXVECTOR3 myPos);        // 一番近いプレイヤーへの角度を求める
    static D3DXVECTOR3  GetPosToClosestPlayer(int nIdxPlayer, D3DXVECTOR3 myPos);   // 一番近いプレイヤーの位置を求める
    static void SetBallGauge(int nMax, int nNow);                                   // ボール発射ゲージ

private:

    static bool m_bStopObjUpdate;                          // オブジェクトのアップデートを止めるかどうか
    static CPlayer *m_apPlayer[MAX_PLAYER];                // プレイヤーのポインタ
    static int m_anPlayerRank[MAX_PLAYER];                 // プレイヤーのランキング（配列の若い順から1位、2位、、、）
    static int m_anPlayerRankInThisRound[MAX_PLAYER];      // このラウンド内のプレイヤーのランキング（配列の若い順から1位、2位、、、）
    static CBall *m_pBall;                                 // ボールのポインタ
    static CPause *m_pPause;                               // ポーズのポインタ
    static CEffect2D *m_pEffect2d_Nega;                    // 反転合成へのポインタ
    static CEffect2D *m_pEffect2d_Posi;                    // 反転合成を戻すためのポインタ
    static CNumberArray *m_pNumArray_BallSpd;              // ボールスピード表示へのポインタ

    static TYPE m_type;                                    // ゲーム種類
    static int m_nNumAllPlayer;                            // 全体プレイヤー人数
    static int m_nNumStock;                                // ストック数
    static bool m_bUseKeyboard;                            // キーボードを使うかどうか
    static int m_anMemoryIdxPlayer[MAX_PLAYER];            // プレイヤーのインデックスを記憶
    static CPlayer::AI_LEVEL m_aMemoryAILevel[MAX_PLAYER]; // AIレベルを記憶
    static STATE m_state;                                  // 状態
    static MAP_LIMIT m_mapLimit;                           // マップ制限
    static int m_nNumDefeatPlayer;                         // やられたプレイヤー人数
    static int m_nWhoWorstPlayer;                          // ワーストのプレイヤー
    static int m_nNumDeathPlayer;                          // 死んだプレイヤー人数
    static RESERVE_SHOOT m_aReserveShoot[MAX_PLAYER];      // シュートの予約

    int m_nCntGameTime;                                    // ゲーム時間のカウンタ
};

#endif