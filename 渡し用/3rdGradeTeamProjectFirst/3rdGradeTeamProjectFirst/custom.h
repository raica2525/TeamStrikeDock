//=============================================================================
//
// カスタマイズ画面処理 [custom.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _CUSTOM_H_
#define _CUSTOM_H_

//=============================================================================
// ヘッダファイルのインクルード
//=============================================================================
#include "main.h"
#include "mode.h"
#include "ui.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EACH_PARTS 32   // 各パーツの最大数

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayer;
class CText;

//=============================================================================
// クラス宣言
//=============================================================================
class CCustom : public CMode
{
public:

    // カスタマイズ画面での選択(アクセスナンバー)
    typedef enum
    {
        SELECT_1P_HEAD_RIGHT = 0,
        SELECT_1P_HEAD_LEFT,
        SELECT_1P_UP_RIGHT,
        SELECT_1P_UP_LEFT,
        SELECT_1P_DOWN_RIGHT,
        SELECT_1P_DOWN_LEFT,
        SELECT_1P_WEP_RIGHT,
        SELECT_1P_WEP_LEFT,
        SELECT_1P_READY,
        SELECT_2P_HEAD_RIGHT,
        SELECT_2P_HEAD_LEFT,
        SELECT_2P_UP_RIGHT,
        SELECT_2P_UP_LEFT,
        SELECT_2P_DOWN_RIGHT,
        SELECT_2P_DOWN_LEFT,
        SELECT_2P_WEP_RIGHT,
        SELECT_2P_WEP_LEFT,
        SELECT_2P_READY,
        SELECT_3P_HEAD_RIGHT,
        SELECT_3P_HEAD_LEFT,
        SELECT_3P_UP_RIGHT,
        SELECT_3P_UP_LEFT,
        SELECT_3P_DOWN_RIGHT,
        SELECT_3P_DOWN_LEFT,
        SELECT_3P_WEP_RIGHT,
        SELECT_3P_WEP_LEFT,
        SELECT_3P_READY,
        SELECT_4P_HEAD_RIGHT,
        SELECT_4P_HEAD_LEFT,
        SELECT_4P_UP_RIGHT,
        SELECT_4P_UP_LEFT,
        SELECT_4P_DOWN_RIGHT,
        SELECT_4P_DOWN_LEFT,
        SELECT_4P_WEP_RIGHT,
        SELECT_4P_WEP_LEFT,
        SELECT_4P_READY,
        SELECT_1P_CHANGE,
        SELECT_2P_CHANGE,
        SELECT_3P_CHANGE,
        SELECT_4P_CHANGE,
        SELECT_1P_OSUSUME,
        SELECT_2P_OSUSUME,
        SELECT_3P_OSUSUME,
        SELECT_4P_OSUSUME,
        SELECT_1P_DETAIL,
        SELECT_2P_DETAIL,
        SELECT_3P_DETAIL,
        SELECT_4P_DETAIL,
        SELECT_READY_TO_FIGHT,
        SELECT_RETURN,
        SELECT_STOCK,
        SELECT_MAX
    }SELECT;

    // 補助値の内訳（クリック編）
    typedef enum
    {
        PARAM_CLICK_TYPE = 0,   // 種類
        PARAM_CLICK_WHO,        // 誰が
        PARAM_CLICK_WHERE,      // どこの
        PARAM_CLICK_RIGHT,      // 右か
    }PARAM_CLICK;

    // クリックしたものの種類
    typedef enum
    {
        CLICK_TYPE_PARTS = 0,        // パーツ
        CLICK_TYPE_READY,            // 準備完了
        CLICK_TYPE_CHANGE,           // チェンジ
        CLICK_TYPE_OSUSUME,          // オススメ
        CLICK_TYPE_DETAIL,           // 詳細
        CLICK_TYPE_READY_TO_FIGHT,   // 戦いへ
        CLICK_TYPE_RETURN,           // 戻る
        CLICK_TYPE_STOCK,            // ストック
    }CLICK_TYPE;

    // エントリー状態
    typedef enum
    {
        ENTRY_STATUS_WAITING = 0,   // 待機中
        ENTRY_STATUS_PLAYER,        // プレイヤー
        ENTRY_STATUS_CP_LEVEL_1,    // CPよわい
        ENTRY_STATUS_CP_LEVEL_2,    // CPふつう
        ENTRY_STATUS_CP_LEVEL_3,    // CPつよい
    }ENTRY_STATUS;

    // エントリー情報
    typedef struct
    {
        CPlayer *pPlayer;               // プレイヤー
        CUI *pUI_Cursor;                // カーソル
        CUI *pUI_Bg_Wait;               // 待機中の背景
        CUI *pUI_Bg_Select;             // 選択中の背景
        CUI *pUI_Bg_Select_Out_Frame;   // 外側の選択枠（色付き）
        CUI *pUI_Bg_Select_In_Frame;    // 内側の選択枠
        CUI *pUI_Bg_Select_Status;      // 各ステータスの文字
        CUI *pUI_Bg_Ready;              // 準備完了の背景
        int nNumSelectHead;             // 頭パーツ選択
        int nNumSelectUp;               // 上半身パーツ選択
        int nNumSelectDown;             // 下半身パーツ選択
        int nNumSelectWep;              // 武器パーツ選択
        CText *pText_Head;              // 頭パーツの名前表示
        CText *pText_Up;                // 上半身パーツの名前表示
        CText *pText_Down;              // 下半身パーツの名前表示
        CText *pText_Wep;               // 武器パーツの名前表示
        bool bUseCursor;                // カーソルを使っているかどうか
        int nNumSelectUIOld;            // 1F前の選択肢
        bool bReady;                    // 準備完了かどうか
        ENTRY_STATUS status;            // 状態
    }ENTRY_INFO;

    CCustom();
    ~CCustom();
    HRESULT Init(void);
    void BindHaveParts(void);                                                  // 所持しているパーツ情報を結びつける
    void Uninit(void);
    void Update(void);

    /*========================================================
    // カーソル周り
    //======================================================*/
    void MoveCursorByKeyboard(float& fStickAngle, bool& bTiltedStick, float& fTiltedStickValue); // キーボード操作
    void MoveCursor(void);                                                     // カーソル移動
    void RimitMoveCursor(D3DXVECTOR3& cursorPos);                              // カーソル移動の制限
    void CollisionSelect(int nNumWho, D3DXVECTOR3 cursorPos);                  // 選択肢との当たり判定
    void ClickSelect(int nNumWho, CUI* pSelectUI);                             // 選択肢のクリック

    /*========================================================
    // エントリー周り
    //======================================================*/
    void ChangeEntryStatus(int nNumWho, ENTRY_STATUS nextEntryStatus);         // エントリー状態のチェンジ
    void ToggleReady(int nNumWho);                                             // 準備完了のトグル
    void JudgmentReadyToFight(void);                                           // 準備完了できているかの判断

    /*========================================================
    // パーツ選択周り
    //======================================================*/
    void SelectParts(int nNumWho, int nNumWhere, bool bRight);                 // カスタマイズするパーツを選択
    void SaveCustom(int nNumSaveWho, int nNumSaveWhere, int nNumSaveParts);    // 誰のどこを何のパーツにして保存するか

private:
    int m_anMemoryPartsHead[MAX_EACH_PARTS];  // 保持した頭パーツ
    int m_anMemoryPartsUp[MAX_EACH_PARTS];    // 保持した上半身パーツ
    int m_anMemoryPartsDown[MAX_EACH_PARTS];  // 保持した下半身パーツ
    int m_anMemoryPartsWep[MAX_EACH_PARTS];   // 保持した武器パーツ

    ENTRY_INFO m_aEntryInfo[MAX_PLAYER];      // エントリー情報
    
    bool m_bClickReadyToFight;                // ReadyToFightが押されたかどうか
    bool m_bDispReadyToFight;                 // ReadyToFightを表示するかどうか
};

#endif