//====================================================================
//
// 描画処理 (renderer.h)
// Author : 後藤慎之助、池田悠希
//
//====================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// クラス宣言
//================================================

// レンダリングクラス
class CRenderer
{
public:
    CRenderer();
    ~CRenderer();
    HRESULT Init(HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);    // オブジェクトのインスタンスがないと、関数が呼べないため持ってくる
    void Draw(void);      // オブジェクトのインスタンスがないと、関数が呼べないため持ってくる
    LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
    LPDIRECT3DTEXTURE9 GetScreenTexture(void) { return m_pScreenTexture; }
#ifdef _DEBUG
    static void SwitchDispFont(void) { m_bDispFont = !m_bDispFont; }
#endif

private:
    LPDIRECT3D9				m_pD3D;			    // Direct3Dオブジェクト
    LPDIRECT3DDEVICE9		m_pD3DDevice;	    // Deviceオブジェクト(描画に必要)

    LPDIRECT3DTEXTURE9		m_pScreenTexture;   // スクリーンのテクスチャ

#ifdef _DEBUG
    void DrawDebugData(void);
    void GameDebugData(void);
    LPD3DXFONT				m_pFont;        // フォントへのポインタ
    static bool             m_bDispFont;    // フォントを表示するかどうか
#endif
};

#endif