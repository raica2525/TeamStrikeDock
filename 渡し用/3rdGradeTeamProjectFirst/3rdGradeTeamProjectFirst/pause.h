//====================================================================
//
// ポーズの処理(オブジェクトを継承していない) (pause.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// マクロ定義
//================================================

#define PAUSE_BG_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))                   // ポーズの背景の位置
#define PAUSE_CONTINUE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 75.0f, 0.0f))     // ポーズのコンティニューの位置
#define PAUSE_QUIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 75.0f, 0.0f))         // ポーズのやめるの位置

#define PAUSE_MENU_SIZE_X 300.0f                                                                // ポーズの各メニューの縦サイズ 
#define PAUSE_MENU_SIZE_Y 80.0f                                                                 // ポーズの各メニューの横サイズ

#define PAUSE_MENU_INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)                       // ポーズの各メニューの非アクティブ色
#define PAUSE_MENU_FADE_RATE 0.01f                                                              // ポーズの点滅割合
#define PAUSE_MENU_FADE_OUT_MIN 0.6f                                                            // ポーズのフェードアウトの最小値
#define PAUSE_MENU_SELECT_COOL_TIME 12                                                          // ポーズの選択のクールタイム

//================================================
// クラス宣言
//================================================

// ポーズクラス
class CPause
{
public:

    // ポーズの種類
    typedef enum
    {
        TYPE_BG = 0,            // 背景
        TYPE_CONTINUE,          // コンティニュー
        TYPE_QUIT,	            // やめる
        TYPE_MAX		        // 種類の最大数
    }TYPE;

    // ポーズの状態
    typedef enum
    {
        STATE_NONE = 0,     // なし
        STATE_PAUSE,        // ポーズ
        STATE_MAX		    // 種類の最大数
    }STATE;

    CPause();
    ~CPause();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    static void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CPause *Create(void);
    void SelectPauseMenu(void);

    static void SetState(STATE state) { m_state = state; }  // 状態を設定

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		   // 頂点バッファへのポインタ
    static STATE m_state;                              // 状態
    TYPE m_typePauseSelect;                            // ポーズで選択中のタイプ
    D3DXCOLOR m_colSelect;                             // 選択中の色
    bool m_bAlphaDown;                                 // 透明度を下げるか上げるか
    int m_nSelectCoolTime;                             // 選択のクールタイム
};

#endif