//====================================================================
//
// 生成の管理処理 (manager.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CMouse;
class CSound;
class CMode;
class CFade;
class CCamera;
class CLight;
class CTexture;
class CModelData;
class CEffectData;

//================================================
// クラス宣言
//================================================

// マネージャークラス
class CManager
{
public:

    // モードの種類
    typedef enum
    {
        MODE_NONE = 0,
        MODE_TITLE,
        MODE_MANUAL,
        MODE_GAME,
        MODE_RESULT,
        MODE_MAX
    }MODE;

    CManager();
    ~CManager();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CRenderer *GetRenderer(void) { return m_pRenderer; }
    static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
    static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
    static CMouse* GetMouse(void) { return m_pMouse; }
    static CSound *GetSound(void) { return m_pSound; }
    static CCamera *GetCamera(void) { return m_pCamera; }
    static CTexture *GetTexture(void) { return m_pTexture; }
    static CModelData *GetModelData(void) { return m_pModelData; }
    static CEffectData *GetEffectData(void) { return m_pEffectData; }
    static MODE GetMode(void);
    static void SetMode(MODE mode);

    static void SetScore(int nScore) { m_nScore = nScore; }
    static int GetScore(void) { return m_nScore; }

private:
    static CRenderer *m_pRenderer;              // レンダラーのポインタ
    static CInputKeyboard *m_pInputKeyboard;    // キーボードのポインタ
    static CInputJoypad *m_pInputJoypad;        // コントローラーのポインタ
    static CMouse* m_pMouse;                    // マウスのポインタ
    static CSound *m_pSound;                    // サウンドのポインタ
    static CMode *m_pMode;                      // モードのポインタ
    static CFade *m_pFade;                      // フェードのポインタ
    static CCamera *m_pCamera;                  // カメラのポインタ
    static CLight *m_pLight;                    // ライトのポインタ
    static CTexture *m_pTexture;                // テクスチャのポインタ
    static CModelData *m_pModelData;            // モデルデータのポインタ
    static CEffectData *m_pEffectData;          // エフェクトデータ管理へのポインタ

    static int m_nScore;                        // スコア
};

#endif