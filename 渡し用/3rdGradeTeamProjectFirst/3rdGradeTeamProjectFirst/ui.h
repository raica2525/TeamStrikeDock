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

#define MAX_ACTION 4        // アクションの最大数
#define MAX_ACTION_PARAM 8  // アクションの補助値の最大数

#define MAX_ACCESS_NUM 128  // アクセスできるUIの最大数

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
        SET_CUSTOM,
        SET_GAME,
        SET_RESULT,
        SET_MAX
    }SET;

    // 動きの種類
    typedef enum
    {
        ACTION_NONE = 0,    // なし（補助値利用したいときに）
        ACTION_GAUGE,       // ゲージ
        ACTION_SIZE,        // サイズ変更
        ACTION_POS,         // 位置変更
        ACTION_ALPHA,       // 透明度変更
        ACTION_COLOR,       // 色変更
        ACTION_ROT,         // 角度変更
        ACTION_TEX_BREND,   // テクスチャブレンド
        ACTION_LOOP_ANIM,   // ループアニメーション
        ACTION_TEX_PLACE,   // テクスチャの描画場所指定
        ACTION_EMIT_EFFECT, // エフェクト発生
        ACTION_MAX
    }ACTION;

    // アクションの情報
    typedef struct
    {
        int action;                            // アクション
        int nCntTime;                          // 時間をカウント
        float afParam[MAX_ACTION_PARAM];       // 補助値
        float afMemoryParam[MAX_ACTION_PARAM]; // 記憶用補助値
        bool bLock;                            // ロックされているかどうか
        bool bMemoryLock;                      // 記憶用ロック
    }ActionInfo;

    // 動き制限の種類
    typedef enum
    {
        RIMIT_NONE = 0,                // 制限なし
        RIMIT_TO_FRAME,                // 指定フレームまで
        RIMIT_FROM_FRAME,              // 指定フレームから
        RIMIT_TO_VALUE,                // 指定値まで
        RIMIT_REPEAT_FRAME,            // 指定フレームを繰り返す
        RIMIT_REPEAT_VALUE,            // 指定値を繰り返す
        RIMIT_EQUAL_VALUE_FROM_FRAME,  // (指定フレームから)指定値にする
    }RIMIT;

    // 補助値の内訳（サイズ編）
    typedef enum
    {
        PARAM_SIZE_CHANGE_RATE_X = 0,    // X軸変更割合
        PARAM_SIZE_CHANGE_RATE_Y,        // (Y軸変更割合)
        PARAM_SIZE_EQUAL_RATIO,          // 等比変形かどうか
        PARAM_SIZE_RIMIT,                // 制限
        PARAM_SIZE_FRAME,                // フレーム数
        PARAM_SIZE_VALUE_X,               // X指定値
        PARAM_SIZE_VALUE_Y,               // X指定値
    }PARAM_SIZE;

    // 補助値の内訳（位置編）
    typedef enum
    {
        PARAM_POS_CHANGE_RATE_X = 0,     // X軸変更割合
        PARAM_POS_CHANGE_RATE_Y,         // Y軸変更割合
        PARAM_POS_ACCEL,                 // 加速度
        PARAM_POS_RIMIT,                 // 制限
        PARAM_POS_FRAME,                 // フレーム数
        PARAM_POS_VALUE_X,               // X指定値
        PARAM_POS_VALUE_Y,               // X指定値
    }PARAM_POS;

    // 補助値の内訳（透明度編）
    typedef enum
    {
        PARAM_ALPHA_CHANGE_RATE = 0,     // 変更割合
        PARAM_ALPHA_RIMIT,               // 制限
        PARAM_ALPHA_FRAME,               // フレーム数
        PARAM_ALPHA_VALUE,               // 指定値
    }PARAM_ALPHA;

    // 補助値の内訳（色変え編）
    typedef enum
    {
        PARAM_COLOR_CHANGE_RATE_R = 0,   // R変更割合
        PARAM_COLOR_CHANGE_RATE_G,       // G変更割合
        PARAM_COLOR_CHANGE_RATE_B,       // B変更割合
        PARAM_COLOR_RIMIT,               // 制限
        PARAM_COLOR_FRAME,               // フレーム数
        PARAM_COLOR_VALUE_R,             // R指定値
        PARAM_COLOR_VALUE_G,             // G指定値
        PARAM_COLOR_VALUE_B,             // B指定値
    }PARAM_COLOR;

    // 補助値の内訳（回転編）
    typedef enum
    {
        PARAM_ROT_CHANGE_RATE = 0,       // 角度変更割合
        PARAM_ROT_RIMIT,                 // 制限
        PARAM_ROT_FRAME,                 // フレーム数
        PARAM_ROT_VALUE,                 // 指定値
    }PARAM_ROT;

    // 補助値の内訳（テクスチャブレンド編）
    typedef enum
    {
        PARAM_TEX_BREND_TEX_NUMBER = 0,  // テクスチャブレンドに使うテクスチャ番号
        PARAM_TEX_BREND_HOW_TO,          // 合成方法
        PARAM_TEX_BREND_ONE_ROUND_FRAME, // 一周フレーム数
        PARAM_TEX_BREND_INTERVAL_FRAME,  // インターバルフレーム数
        PARAM_TEX_BREND_RIGHT_TO_LEFT,   // 右から左か
        PARAM_TEX_BREND_DIRECT,          // 向き
        PARAM_TEX_BREND_IDX,             // テクスチャブレンドに使うインデックス（1～3）
    }PARAM_TEX_BREND;

    // 補助値の内訳（ループアニメーション編）
    typedef enum
    {
        PARAM_LOOP_ANIM_ONE_ROUND_FRAME = 0, // 一周フレーム数
        PARAM_LOOP_ANIM_INTERVAL_FRAME,      // インターバルフレーム数
        PARAM_LOOP_ANIM_RIGHT_TO_LEFT,       // 右から左か
        PARAM_LOOP_ANIM_DIRECT,              // 向き
    }PARAM_LOOP_ANIM;

    // 補助値の内訳（テクスチャの描画場所指定編）
    typedef enum
    {
        PARAM_TEX_PLACE_PATTERN = 0, // 何等分した
        PARAM_TEX_PLACE_PLACE,       // 何番目か
    }PARAM_TEX_PLACE;

    // 補助値の内訳（エフェクト発生編）
    typedef enum
    {
        PARAM_EMIT_EFFECT_TYPE = 0,            // 角度変更割合
        PARAM_EMIT_EFFECT_RIMIT,               // 制限
        PARAM_EMIT_EFFECT_FRAME,               // フレーム数
        PARAM_EMIT_EFFECT_ADJUST_EMIT_POS_X,   // 発生位置Xをずらす値
        PARAM_EMIT_EFFECT_ADJUST_EMIT_POS_Y,   // 発生位置Yをずらす値
    }PARAM_EMIT_EFFECT;

    CUI(CScene::OBJTYPE objtype);
    ~CUI();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CUI *Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, int nRotAngle, D3DXCOLOR col,
        bool bFrontText = false, bool bUseAddiveSynthesis = false, int nAlphaTestBorder = 0, bool bUseZBuffer = false,
        D3DXVECTOR3 collisionPos = DEFAULT_VECTOR, D3DXVECTOR3 collisionSize = DEFAULT_VECTOR);
    static void Place(SET set);     // 外部ファイルからUIの配置

    /*========================================================
    // ゲッター
    //======================================================*/
    D3DXCOLOR GetCol(void) { return m_col; }
    D3DXVECTOR3 GetCollisionPos(void) { return m_collisionPos; }
    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }
    static CUI* GetAccessUI(int nNum);
    float GetActionParam(int nNumAction, int nNumParam) { return m_aActionInfo[nNumAction].afParam[nNumParam]; }

    /*========================================================
    // セッター
    //======================================================*/
    void SetAccessUI(int nNum);
    void SetDontUse(void) { m_bUse = false; }
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetAlpha(float fAlpha) { m_col.a = fAlpha; }
    void SetActionInfo(int nNum, int action, bool bLock,
        float fParam0 = 0, float fParam1 = 0, float fParam2 = 0, float fParam3 = 0,
        float fParam4 = 0, float fParam5 = 0, float fParam6 = 0, float fParam7 = 0);    // ここでのみ、アクションの補助値周りの設定をできる
    void SetActionLock(int nNum, bool bLock);
    void SetActionReset(int nNum);
    void SetAllActionReset(void);
    void SetFirstPos(void) { SetPosition(m_memoryPos); }    // アクション経由ではなく、最初の位置に戻すとき使う（例:カーソルのリセット）
    void SetDisp(bool bDisp) { m_bDisp = bDisp; }

    /*========================================================
    // アクション
    //======================================================*/
    void PlayAction(int nNum);           // 全てのアクションを実行するもと
    void PlayActionSize(int nNum);       // サイズアクション
    void PlayActionPos(int nNum);        // 位置アクション
    void PlayActionAlpha(int nNum);      // 透明度アクション
    void PlayActionColor(int nNum);      // 色変えアクション
    void PlayActionRot(int nNum);        // 回転アクション
    void PlayActionTexBrend(int nNum);   // テクスチャブレンドアクション
    void PlayActionLoopAnim(int nNum);   // ループアニメーションアクション
    void PlayActionEmitEffect(int nNum); // エフェクト発生アクション

    /*========================================================
    // アクション用の共通処理
    //======================================================*/
    void RimitToValue(const float fChangeRate, float& fCurrentValue, const float fDestValue, bool& bUpdate);
    void RimitRepeatValue(float& fChangeRate, const float fMemoryValue, const float fCurrentValue, const float fDestValue, bool& bUpdate);

private:
    static CUI* m_apAccessUI[MAX_ACCESS_NUM];   // アクセスできるUI
    int m_nTexType;                             // 使うテクスチャの種類
    ActionInfo m_aActionInfo[MAX_ACTION];       // 動きの状態
    bool m_bUse;                                // 使用するかどうか

    float m_fRotAngle;                          // 回転角度
    D3DXCOLOR m_col;                            // 色
    bool m_bUseAdditiveSynthesis;               // 加算合成にするかどうか

    D3DXVECTOR3 m_memoryPos;                    // 記憶用位置
    D3DXVECTOR3 m_memorySize;                   // 記憶用大きさ
    float m_fMemoryRotAngle;                    // 記憶用角度
    D3DXCOLOR m_memoryCol;                      // 記憶用色

    //int m_nCntAnimTime;                       // アニメーション用カウンタ（effect3dから持ってきた際に削除、scene2dに元々あるため）
    bool m_bOneRoundAnim;                       // アニメーションが一周したかどうか
    int m_nAnimPattern;                         // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）
    int m_nAnimSpeed;                           // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）
    bool m_bRepeat;                             // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）

    D3DXVECTOR3 m_collisionPos;                 // 当たり判定の位置
    D3DXVECTOR3 m_collisionSize;                // 当たり判定の大きさ
    int m_nAccessNum;                           // アクセスナンバー
    bool m_bDisp;                               // 表示するかどうか
};

#endif