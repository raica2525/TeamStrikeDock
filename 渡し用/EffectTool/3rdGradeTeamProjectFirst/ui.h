//====================================================================
//
// UIの処理 (ui.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _UI_H_
#define _UI_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================

// バージョン表記
#define VERSION_POS D3DXVECTOR3(1150.0f, 675.0f, 0.0f)
#define VERSION_SIZE D3DXVECTOR3(150.0f,75.0f,0.0f)           

// 照準(若干下に)
#define LOCK_ON_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 22.5f, 0.0f))
#define LOCK_ON_SIZE (D3DXVECTOR3(60.0f, 60.0f , 0.0f))

// リロード(小さめに)
#define RELOAD_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 70.0f, 0.0f))
#define RELOAD_SIZE (D3DXVECTOR3(180.0f, 60.0f , 0.0f))

// タイトルロゴ
#define TITLE_LOGO_FIRST_POS (D3DXVECTOR3(350.0f, -150.0f, 0.0f))                   // タイトルロゴの最初の位置
#define TITLE_LOGO_TARGET_POS_Y 150.0f                                              // タイトルロゴの目標位置Y 
#define TITLE_LOGO_SIZE (D3DXVECTOR3(500.0f, 250.0f, 0.0f))                         // タイトルロゴのサイズ
#define TITLE_LOGO_FLOW_DOWN_SPEED 2.1f                                             // タイトルロゴの流れ落ちる速度

// PLEASE PRESS START
#define PRESS_START_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f)                 // スタートボタンを押してねの位置
#define PRESS_START_SIZE D3DXVECTOR3(500.0f,200.0f,0.0f)                            // スタートボタンを押してねのサイズ
#define PRESS_START_INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)          // スタートボタンを押してねの非アクティブ色
#define PRESS_START_FADE_RATE 0.01f                                                 // スタートボタンを押してねの点滅割合
#define PRESS_START_FADE_OUT_MIN 0.6f                                               // スタートボタンを押してねのフェードアウトの最小値

// 肉体暴走まで
#define UI_LIMIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT - 45.0f, 0.0f))
#define UI_LIMIT_SIZE (D3DXVECTOR3(400.0f , 65.0f, 0.0f))

// タイトル
#define TITLE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))         // 遊び方の位置
#define TITLE_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))                // 遊び方のサイズ

// 遊び方
#define MANUAL_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))        // 遊び方の位置
#define MANUAL_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))               // 遊び方のサイズ

// リザルト
#define RESULT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))        // リザルトの位置
#define RESULT_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))               // リザルトのサイズ

//================================================================================================================================================
// 齋藤変更箇所

// HPゲージ(枠)
#define UI_STAMINA_POS (D3DXVECTOR3(580.0f - (350.0f / 2), 90.0f, 0.0f))
#define UI_STAMINA_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// ゲージ
#define UI_GAUGE_POS (D3DXVECTOR3(580.0f - (350.0f / 2), 90.0f, 0.0f))
#define UI_GAUGE_SIZE (D3DXVECTOR3(290.0f , 34.0f, 0.0f))

// HPロゴ
#define UI_HP_LOGO_POS (D3DXVECTOR3(250.0f - (170.0f / 2), 100.0f, 0.0f))
#define UI_HP_LOGO_SIZE (D3DXVECTOR3(170.0f , 85.0f, 0.0f))

// 左手フレーム
#define UI_LEFTHAND_FRAME_POS (D3DXVECTOR3(480.0f - (425.0 / 2), 560.0f, 0.0f))
#define UI_LEFTHAND_FRAME_SIZE (D3DXVECTOR3(425.0f ,150.0f, 0.0f))

// 左手ゲージ
#define UI_LEFTHAND_GAUGE_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_GAUGE_SIZE (D3DXVECTOR3(288.0f , 49.5f, 0.0f))

// 左手ゲージ背景
#define UI_LEFTHAND_GAUGE_BG_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_GAUGE_BG_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// 左手ゲージフレーム
#define UI_LEFTHAND_GAUGE_FRAME_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_GAUGE_FRAME_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// 左手仕切り
#define UI_LEFTHAND_PARTITION_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_LOGO_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// 右手ロゴ
#define UI_RIGHTHAND_LOGO_POS (D3DXVECTOR3(900.0f - (75.0 / 2), 590.0f, 0.0f))
#define UI_RIGHTHAND_LOGO_SIZE (D3DXVECTOR3(80.0f , 80.0f, 0.0f))

// 右手残弾
#define UI_RIGHTHAND_STAMINA_POS (D3DXVECTOR3(1210.0f - (300.0 / 2), 585.0f, 0.0f))
#define UI_RIGHTHAND_STAMINA_SIZE (D3DXVECTOR3(300.0f , 100.0f, 0.0f))

// マップ
#define UI_MINIMAP_POS (D3DXVECTOR3(1200.0f - (350.0f / 2), 150.0f, 0.0f))
#define UI_MINIMAP_SIZE (D3DXVECTOR3(350.0f , 350.0f, 0.0f))

// スコア
#define UI_SCORE_POS (D3DXVECTOR3(480.0f - (300.0 / 2), 530.0f, 0.0f))
#define UI_SCORE_SIZE (D3DXVECTOR3(300.0f ,40.0f, 0.0f))

//================================================================================================================================================================================================

// ミッションロゴの位置とサイズ
#define UI_MISSION_LOGO_POS (D3DXVECTOR3(200.0f, 212.5f, 0.0f))
#define UI_MISSION_LOGO_SIZE (D3DXVECTOR3(160.0f, 40.0f, 0.0f))

// ミッションUIの位置とサイズ
#define UI_MISSION_POS_1 (D3DXVECTOR3(200.0f, 270.0f, 0.0f))
#define UI_MISSION_SIZE_1 (D3DXVECTOR3(320.0f, 80.0f, 0.0f))
#define UI_MISSION_POS_2 (D3DXVECTOR3(200.0f, 320.0f, 0.0f))
#define UI_MISSION_SIZE_2 (D3DXVECTOR3(320.0f, 80.0f, 0.0f))

// 拡大するミッション系
#define UI_EXTEND_MISSION_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 0.0f))  // 位置
#define UI_EXTEND_MISSION_MAX_SIZE_X (600.0f)                                                   // 横の最大サイズ
#define UI_EXTEND_MISSION_MAX_SIZE_Y (300.0f)                                                   // 縦の最大サイズ
#define UI_EXTEND_MISSION_EXTEND_RATE_X (15.0f)                                                 // 横の拡大割合
#define UI_EXTEND_MISSION_EXTEND_RATE_Y (7.5f)                                                  // 縦の拡大割合
#define UI_EXTEND_MISSION_FADE_OUT_COUNT (180)                                                  // フェードが始まるカウンタ
#define UI_EXTEND_MISSION_FADE_OUT_RATE (0.05f)                                                 // フェードアウト割合

//================================================
// クラス宣言
//================================================

// UIクラス
class CUI : public CScene2D
{
public:

    // セット
    typedef enum
    {
        SET_NONE = 0,
        SET_TITLE,
        SET_MANUAL,
        SET_GAME,
        SET_RESULT,
        SET_MAX
    }SET;

    // 動きの種類
    typedef enum
    {
        ACTION_NONE = 0,
        ACTION_TITLE_LOGO,
        ACTION_TITLE_PRESS_ENTER,
        ACTION_GAUGE,
        ACTION_MAX
    }ACTION;

    CUI();
    ~CUI();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CUI *Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, ACTION action);
    static void Place(SET set);

    D3DXVECTOR3 Extend(D3DXVECTOR3 size);           // 拡大するUI

    void SetDontUse(void) { m_bUse = false; }       // 使わないものの破棄

private:
    int m_nTexType;                                    // 使うテクスチャの種類
    ACTION m_action;                                   // 動きの種類
    D3DXVECTOR3 m_move;                                // 移動量
    D3DXCOLOR m_col;                                   // 色
    float m_fAngle;                                    // 角度
    bool m_bFadeOut;                                   // フェードアウトかどうか
    bool m_bBlinking;                                  // 点滅中かどうか
    int m_nCntTime;                                    // 時間をカウント
    bool m_bUse;                                       // 使用するかどうか
};

#endif