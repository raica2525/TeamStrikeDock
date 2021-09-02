//====================================================================
//
// ビルボード処理 (billboard.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//================================================
// クラス宣言
//================================================

// ビルボードクラス
class CBillboard : public CScene
{
public:
    CBillboard();
    CBillboard(OBJTYPE objtype);
    virtual ~CBillboard();
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindTexture(const int nNumTexture);

    /*========================================================
    // セッター
    //======================================================*/
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetSize(D3DXVECTOR3 size) { m_size = size; }
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetColorVertex(D3DXCOLOR col);     // 頂点の色を直接設定する（主にCreate時の1Fだけ白くなる現象に、使う）
    void SetTexUV(float x1, float x2, float y1, float y2) { m_fTexX1 = x1; m_fTexX2 = x2; m_fTexY1 = y1; m_fTexY2 = y2; }          // テクスチャ座標を設定
    bool SetUpdateTexLeftToRight(int nMaxPattern);                                // 左から右にテクスチャ座標を更新
    void SetAlphaTestBorder(int nValue) { m_nAlphaTestBorder = nValue; }          // アルファテストの境界値を設定
    void SetRotAngle(float fRotAngle) { m_fRotAngle = fRotAngle; }

    /*========================================================
    // ゲッター
    //======================================================*/
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetSize(void) { return m_size; }
    float GetRotAngle(void) { return m_fRotAngle; }

    static void SetAdditiveSynthesis(void) { m_bAdditiveSynthesis = true; } // 加算合成にする
    static void SetZBufferNone(void) { m_bUseZBuffer = false; }             // Zバッファを無効化

private:
    static bool m_bAdditiveSynthesis;       // 加算合成するかどうか
    static bool m_bUseZBuffer;              // Zバッファを使用するかどうか
    LPDIRECT3DTEXTURE9		m_pTexture;
    D3DXMATRIX			    m_mtxWorld;		// ワールドマトリックス
    LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファー
    D3DXVECTOR3				m_pos;			// 座標
    D3DXVECTOR3				m_size;			// 大きさ
    D3DXVECTOR3				m_rot;			// 回転
    D3DXVECTOR3				m_scale;		// 拡大率
    D3DXCOLOR				m_col;          // 色
    float					m_fTexX1;       // テクスチャ座標X1
    float					m_fTexX2;       // テクスチャ座標X2
    float					m_fTexY1;		// テクスチャ座標Y1
    float					m_fTexY2;		// テクスチャ座標Y2

    int m_nAlphaTestBorder;                 // アルファテストの境界値
    float m_fRotAngle;                      // 回転角度
};

#endif