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

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CBall;
class CScore;
class CPause;
class CUI;

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
        STATE_FINISH,              // 決着中
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
    void JudgmentFinish(void);  // 勝敗判定

    /* セッター*/
    static void SetAddNumAllPlayer(void) { m_nNumAllPlayer++; }                     // 全体プレイヤー人数を増やす
    static void SetAddNumDefeatPlayer(void) { m_nNumDefeatPlayer++; }               // やられたプレイヤー人数を増やす
    static void SetWorstPlayer(int player) { m_nWhoWorstPlayer = player; }          // ワーストのプレイヤーを決める
    static void SetAddNumDeathPlayer(void) { m_nNumDeathPlayer++; }                 // 死んだプレイヤー人数を増やす
    static void SetQuitPause(void) { m_state = STATE_BUTTLE; m_bStopObjUpdate = false; }    // ポーズ状態をやめる

    /* ゲッター*/
    static STATE GetState(void) { return m_state; }                                 // ゲームの状態を取得
    static CPlayer* GetPlayer(const int nNum) { return m_apPlayer[nNum]; }          // プレイヤーを取得
    static CBall* GetBall(void) { return m_pBall; }                                 // ボールを取得
    static CScore *GetScore(void) { return m_pScore; }                              // スコアを取得
    static int GetNumAllPlayer(void) { return m_nNumAllPlayer; }                    // 全体プレイヤー人数を取得
    static int GetNumDefeatPlayer(void) { return m_nNumDefeatPlayer; }              // やられたプレイヤー人数を取得
    static int GetWhoWorstPlayer(void) { return m_nWhoWorstPlayer; }                // ワーストのプレイヤーを取得
    static int GetNumDeathPlayer(void) { return m_nNumDeathPlayer; }                // 死んだプレイヤー人数を取得
    static MAP_LIMIT GetMapLimit(void) { return m_mapLimit; }                       // マップ制限を取得
    static TYPE GetType(void) { return m_type; }                                    // ゲーム種類を取得
    static bool GetStopObjUpdate(void) { return m_bStopObjUpdate; }                 // オブジェクトのアップデートを止めるかどうか
    static int GetNumCurrentDispPlayer(void) { return (m_nNumAllPlayer - m_nNumDeathPlayer - m_nNumDefeatPlayer); } // 現在生きているプレイヤー人数を取得

    /* 便利な関数*/
    static void ReserveShoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, int nWho);
    void JudgmentShoot(void);                                                       // 誰がシュートを打てたかの判定
    static float GetAngleToClosestPlayer(int playable, D3DXVECTOR3 myPos);          // 一番近いプレイヤーへの角度を求める

private:

    static bool m_bStopObjUpdate;                       // オブジェクトのアップデートを止めるかどうか

    static CScore *m_pScore;                            // スコアのポインタ
    static CPlayer *m_apPlayer[MAX_PLAYER];             // プレイヤーのポインタ
    static CBall *m_pBall;                              // ボールのポインタ
    static CPause *m_pPause;                            // ポーズのポインタ

    static TYPE m_type;                                 // ゲーム種類
    static STATE m_state;                               // 状態
    static MAP_LIMIT m_mapLimit;                        // マップ制限
    static int m_nNumAllPlayer;                         // 全体プレイヤー人数
    static int m_nNumDefeatPlayer;                      // やられたプレイヤー人数
    static int m_nWhoWorstPlayer;                       // ワーストのプレイヤー
    static int m_nNumDeathPlayer;                       // 死んだプレイヤー人数
    static RESERVE_SHOOT m_aReserveShoot[MAX_PLAYER];   // シュートの予約

    int m_nCntGameTime;                                 // ゲーム時間のカウンタ
};

#endif