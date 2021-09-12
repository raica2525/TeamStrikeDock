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

// ゲーム状態の管理フレーム
#define COUNT_WAIT_FINISH 300	    // ゲーム終了時に、タイトル画面へ行くまでの時間

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CMode
{
public:

    // ゲームの状態（仮）
    typedef enum
    {
        STATE_ROUND_START = 0,     // 開始
        STATE_BUTTLE,              // バトル中
        STATE_FINISH_ROUND,	       // ラウンド決着時
        STATE_FINISH_GAME,         // ゲーム決着時
        STATE_PAUSE,	           // ポーズ
        STATE_MAX			       // 状態の最大数
    }STATE;

    // マップ制限
    typedef struct
    {
        float fHeight;
        float fWidth;
    }MAP_LIMIT;

    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

    void Judgment(void);                                                            // 勝敗判定
    void ManageState(void);                                                         // ゲーム状態の管理

    void Buttle(void);

    /* セッター*/
    static void SetState(STATE state) { m_state = state; }                          // ゲームの状態を設定
    static void SetAddNumCurrentPlayer(void) { m_nNumCurrentPlayer++; }             // 現在のプレイヤー人数を増やす

    /* ゲッター*/
    static STATE GetState(void) { return m_state; }                                 // ゲームの状態を取得
    static CPlayer* GetPlayer(const int nNum) { return m_apPlayer[nNum]; }          // プレイヤーを取得
    static CBall* GetBall(void) { return m_pBall; }                                 // ボールを取得
    static CScore *GetScore(void) { return m_pScore; }                              // スコアを取得
    static int GetNumCurrentPlayer(void) { return m_nNumCurrentPlayer; }            // 現在のプレイヤー人数を取得
    static MAP_LIMIT GetMapLimit(void) { return m_mapLimit; }                       // マップ制限を取得

    /* 便利な関数*/
    static float GetAngleToClosestPlayer(int playable, D3DXVECTOR3 myPos);          // 一番近いプレイヤーへの角度を求める
    //static D3DXVECTOR3 GetPosToClosestEnemy(D3DXVECTOR3 myPos);                   // 一番近い敵の位置を求める

private:

    static CScore *m_pScore;                            // スコアのポインタ
    static CPlayer *m_apPlayer[MAX_PLAYER];             // プレイヤーのポインタ
    static CBall *m_pBall;                              // ボールのポインタ
    static CPause *m_pPause;                            // ポーズのポインタ

    static STATE m_state;                               // 状態
    static MAP_LIMIT m_mapLimit;                        // マップ制限
    static int m_nNumCurrentPlayer;                     // 現在のプレイヤー人数

    int m_nCntGameTime;                                 // ゲーム時間のカウンタ
};

#endif