//====================================================================
//
// シーン上の2Dポリゴン処理 (scene2d.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"
#include "library.h"

//================================================
// マクロ定義
//================================================
#define MAX_ANIM_COUNTER (10000)        // アニメーションのカウンタを利用する際の、最大カウント

//================================================
// クラス宣言
//================================================

// 2Dポリゴンクラス
class CScene2D : public CScene
{
public:

    // 向き
    typedef enum
    {
        DIRECT_VERTICAL,    // 縦
        DIRECT_HORIZON,     // 横
        DIRECT_RIGHT_UP,    // 右肩上がり
        DIRECT_RIGHT_DOWN,  // 右肩下がり
        DIRECT_MAX          // 向きの最大数
    }DIRECT;

    CScene2D();
    CScene2D(OBJTYPE objType);
    virtual ~CScene2D();
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindTexture(const int nNumTexture);                                                      // テクスチャを割り当てる
    void SetVertex(void);                                                                         // 頂点座標を設定
    void SetRotVertex(float fAngle);                                                              // 回転する頂点座標を設定
    void SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual);                          // 見かけ上の頂点座標を設定
    bool SetAnimation(int nSpeed, int nPattern);                                                  // アニメーション
    bool SetReverseAnimation(int nSpeed, int nPattern);                                           // 逆向きのアニメーション
    void SetFlowingAnimation(int nSpeed, int nPattern, bool bRightToLeft, DIRECT direct);         // 流れるアニメーション
    void SetTextureRange(int nRange, int nPattern);                                               // テクスチャの描画範囲を設定
    void SetParagraphAnimation(int nParagraph, int nMaxParagraph, int nSpeed, int nPattern);      // 段落のあるアニメーション
    void SetTexturePlace(int nPlace, int nPattern);                                               // テクスチャの描画場所を決める
    void SetParagraphTexturePlace(int nPlace, int nParagraph, int nMaxParagraph, int nPattern);   // 段落のあるアニメーションから一部分を切り取る
    int CountAnimation(int nSpeed, int nPattern);                                                 // アニメーションのカウンタを利用する
    void ResetCountAnim(void) { m_nCounterAnim = 0; m_nPatternAnim = 0; }                         // アニメーションのカウンタをリセット
    void SetColor(D3DXCOLOR col);                                                                 // 色を変える
    void SetLeftToRightGauge(float fMax, float fNow);                                             // 左から右に伸びたゲージ

    /*========================================================
    // セッター
    //======================================================*/
    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetPosTo2D(D3DXVECTOR3 pos3D) { m_pos = ConvertScreenPos(pos3D); }
    void SetSize(D3DXVECTOR3 size) { m_size = size; }
    void SetAlphaTestBorder(int nValue) { m_nAlphaTestBorder = nValue; }
    void SetAdditiveSynthesis(void) { m_bAdditiveSynthesis = true; }
    void SetAlphaTest(void) { m_bAlphaTest = true; }
    void SetNega(void) { m_bNega = true; }

    /*========================================================
    // ゲッター
    //======================================================*/
    D3DXVECTOR3 GetPosition(void) { return m_pos; }
    D3DXVECTOR3 GetSize(void) { return m_size; }

private:
    static bool m_bAdditiveSynthesis;       // 加算合成するかどうか
    static bool m_bAlphaTest;               // αテストするかどうか
    static bool m_bNega;                    // 反転合成するかどうか
    LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
    D3DXVECTOR3				m_pos;	        // 位置
    D3DXVECTOR3             m_size;         // 大きさ
    int m_nCounterAnim;                     // アニメーションカウンタ
    int m_nPatternAnim;                     // アニメーションパターン

    int m_nAlphaTestBorder;                 // アルファテストの境界値
};

#endif