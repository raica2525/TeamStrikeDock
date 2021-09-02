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

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EACH_PARTS 32   // 各パーツの最大数

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayer;
class CUI;
class CText;

//=============================================================================
// クラス宣言
//=============================================================================
class CCustom : public CMode
{
public:

    // カスタマイズ画面での選択
    typedef enum
    {
        SELECT_1P_HEAD = 0,
        SELECT_1P_UP,
        SELECT_1P_DOWN,
        SELECT_1P_WEP,
        SELECT_MAX			      // 選択の最大数
    }SELECT;

    CCustom();
    ~CCustom();
    HRESULT Init(void);
    void BindHaveParts(void);
    void Uninit(void);
    void Update(void);

    void SaveCustom(int nNumSaveWho, int nNumSaveWhere, int nNumSaveParts);    // 誰のどこを何のパーツにして保存するか

private:
    CPlayer *m_apPlayer[MAX_PLAYER];    // プレイヤーのポインタ
    int m_anPartsHead[MAX_EACH_PARTS];  // 頭パーツ
    int m_anPartsUp[MAX_EACH_PARTS];    // 上半身パーツ
    int m_anPartsDown[MAX_EACH_PARTS];  // 下半身パーツ
    int m_anPartsWep[MAX_EACH_PARTS];   // 武器パーツ

    // いずれ構造体になる人たち
    CUI *m_pUI_Cursor;                  // カーソル
    int m_nSelectCoolTime;              // 選択のクールタイム
    int m_select;                       // 選択
    int m_nNumSelectHead;               // 頭パーツ選択
    int m_nNumSelectUp;                 // 上半身パーツ選択
    int m_nNumSelectDown;               // 下半身パーツ選択
    int m_nNumSelectWep;                // 武器パーツ選択
    CText *m_pText_Head;                // 頭パーツの名前
    CText *m_pText_Up;                  // 上半身パーツの名前
    CText *m_pText_Down;                // 下半身パーツの名前
    CText *m_pText_Wep;                 // 武器パーツの名前
};

#endif