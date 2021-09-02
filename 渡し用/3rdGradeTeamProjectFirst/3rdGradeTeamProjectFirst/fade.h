//=============================================================================
//
// フェード処理 [fade.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "manager.h"

//================================================
// クラス宣言
//================================================
class CFade
{
public:

    // フェードの種類
    typedef enum
    {
        FADE_NONE = 0,
        FADE_OUT,
        FADE_IN,
        FADE_MAX
    }FADE;

    CFade();										// コンストラクタ
    ~CFade();										// デストラクタ
    HRESULT Init(CManager::MODE mode);				// 初期化処理
    void Uninit(void);								// 終了処理
    void Update(void);								// 更新処理
    static void Draw(void);							// 描画処理
    static void SetFade(CManager::MODE mode);		// フェードイン処理	
    static CFade *Create(CManager::MODE mode);		// インスタンス生成
    static FADE GetFade(void) { return m_fade; }    // フェードの状態を取得

private:
    static D3DXCOLOR		m_col;			        // 頂点カラー
    static FADE				m_fade;                 // フェードの状態
    static CManager::MODE	m_modeNext;             // 次に設定するモード
    static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファのポインタ
};

#endif