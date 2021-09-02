//====================================================================
//
// デバッグ周りの処理 (debug.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"
#include "ui.h"
#include "mode.h"

//================================================
// マクロ定義
//================================================

#ifdef _DEBUG
// 当たり判定を生成する
#define COLLISION_TEST
#endif

//================================================
// クラス宣言
//================================================

// デバッククラス
class CDebug : public CScene3D
{
public:

    // デバックの種類
    typedef enum
    {
        TYPE_PERMANENT = 0,	    // 永続
        TYPE_MOMENT,	        // 瞬間
        TYPE_MAX			    // 種類の最大数
    }TYPE;

    CDebug();
    ~CDebug();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);  // 戻り値は呼び出し先で使う必要がないため無し

#ifdef _DEBUG
    static void ReloadUI(CUI::SET set);     // テクスチャの再読み込み
    static void CommonCommand(void);        // 共通のデバッグコマンド
#endif

private:
    int m_nLife;    // 寿命
    TYPE m_type;    // 種類
};

// デバックメニュークラス
class CDebugMenu : public CMode
{
public:

    // デバックメニューの選択
    typedef enum
    {
        SELECT_TRAINING = 0,	  // トレーニング
        SELECT_CUSTOMIZE,	      // カスタマイズ
        SELECT_TITLE,             // タイトル
        SELECT_ARENA_1,           // アリーナ1
        SELECT_ARENA_2,           // アリーナ2
        SELECT_ARENA_3,           // アリーナ3
        SELECT_ARENA_4,           // アリーナ4
        SELECT_ARENA_5,           // アリーナ5
        SELECT_ARENA_6,           // アリーナ6
        SELECT_MAX			      // 選択の最大数
    }SELECT;

    CDebugMenu();
    ~CDebugMenu();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

private:
    CUI *m_pUI_Cursor;      // カーソル
    int m_nSelectCoolTime;  // 選択のクールタイム
    int m_select;           // 選択
};

#endif