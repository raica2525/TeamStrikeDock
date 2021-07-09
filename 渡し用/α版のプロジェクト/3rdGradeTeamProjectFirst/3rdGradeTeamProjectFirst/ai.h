//====================================================================
//
// AI処理 (ai.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _AI_H_
#define _AI_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// 考え時間
#define THINKING_TIME_AI_LEVEL_1_MAX 45
#define THINKING_TIME_AI_LEVEL_1_MIN 30
#define THINKING_TIME_AI_LEVEL_2_MAX 20
#define THINKING_TIME_AI_LEVEL_2_MIN 10
#define THINKING_TIME_AI_LEVEL_3_MAX 5
#define THINKING_TIME_AI_LEVEL_3_MIN 1

// コアの優先順位
#define CORE_PRIORITY_1 60
#define CORE_PRIORITY_2 30
#define CORE_PRIORITY_3 10

// 強襲時間
#define ASSAULT_TIME_AI_LEVEL_1_MAX 60
#define ASSAULT_TIME_AI_LEVEL_1_MIN 30
#define ASSAULT_TIME_AI_LEVEL_2_MAX 180
#define ASSAULT_TIME_AI_LEVEL_2_MIN 120
#define ASSAULT_TIME_AI_LEVEL_3_MAX 300
#define ASSAULT_TIME_AI_LEVEL_3_MIN 180

// 避ける時間
#define AVOID_TIME_AI_LEVEL_1_MAX 30
#define AVOID_TIME_AI_LEVEL_1_MIN 15
#define AVOID_TIME_AI_LEVEL_2_MAX 45
#define AVOID_TIME_AI_LEVEL_2_MIN 30
#define AVOID_TIME_AI_LEVEL_3_MAX 60
#define AVOID_TIME_AI_LEVEL_3_MIN 45

// 待つ時間
#define WAIT_TIME_AI_LEVEL_1_MAX 30
#define WAIT_TIME_AI_LEVEL_1_MIN 15
#define WAIT_TIME_AI_LEVEL_2_MAX 45
#define WAIT_TIME_AI_LEVEL_2_MIN 30
#define WAIT_TIME_AI_LEVEL_3_MAX 60
#define WAIT_TIME_AI_LEVEL_3_MIN 45

// 攻撃時に狙った攻撃ができるかどうか（100分の何）
#define ATTACK_TARGET_AI_LEVEL_1 30
#define ATTACK_TARGET_AI_LEVEL_2 75
#define ATTACK_TARGET_AI_LEVEL_3 95

//================================================
// クラス宣言
//================================================

// AIクラス
class CAi
{
public:
    CAi();
    ~CAi();

    // ボタンの入力情報
    typedef struct
    {
        bool bButtonA;
        bool bButtonX;
        bool bButtonB;
        bool bButtonY;
    }ButtonState;

    // 心がけていること
    typedef enum
    {
        CORE_THINKING = 0, // 考え中
        CORE_ASSAULT,      // 強襲（ガン攻め）
        CORE_AVOID,        // 避ける（ガン逃げ）
        CORE_WAIT,         // 待つ（ガン待ち）
        CORE_MAX
    }CORE;

    void Update(void);						    // 更新処理
    static CAi * Create(CPlayer *pPlayer);		// インスタンス生成処理
    void Thinking(void);                        // 考え中
    void AnotherAction(void);                   // 考える以外の行動中
    void GetThinkingTime(void);                 // 考え時間を得る
    void GetAssaultTime(void);                  // 強襲時間を得る
    void GetAvoidTime(void);                    // 避ける時間を得る
    void GetWaitTime(void);                     // 待つ時間を得る
    void DontMove(bool bUseTurn);               // 移動しない
    void RushToBall(void);                      // ボールに詰め寄る
    void RunAwayFromBall(void);                 // ボールから逃げる
    bool JumpBecauseBallUp(void);               // ボールが上にあるからジャンプする
    bool JumpBecauseBallMoveDown(void);         // ボールが下がってきているからジャンプする
    bool DecideAttack(bool bUseTurn);           // 決めきる攻撃をするかどうか
    void GetAttackAngle(void);                  // 攻撃時にどの方向にスティックを倒すか

private:
    CPlayer *m_pPlayer;
    ButtonState m_buttonStateOld;   // 1F前のボタン入力情報

    CORE m_core;                    // 現在何を心がけているか
    int m_nCntActionTime;           // 行動のカウンタ
};

#endif