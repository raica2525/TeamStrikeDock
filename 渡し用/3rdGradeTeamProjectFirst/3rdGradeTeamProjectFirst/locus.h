//=============================================================================
//
// 軌跡の処理 [locus.h]
// Author : 伊藤陽梧
//
//=============================================================================
#ifndef _LOCUS_H_
#define _LOCUS_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene3d.h"

//=============================================================================
// 軌跡クラス
//=============================================================================
class CLocus : public CScene
{
public:
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_SHRINK,	// 軌跡を縮める
        TYPE_MAX
    }TYPE_LOCUS;

    CLocus(OBJTYPE objtype = OBJTYPE_EFFECT3D);
    ~CLocus();

    static CLocus *Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2, D3DXCOLOR col, TYPE_LOCUS TypeLucus = TYPE_NONE, float ShrinkSpeed = 0.0f);	// 生成処理

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
    void Uninit(void);									// 終了処理
    void Update(void);									// 更新処理
    void Draw(void);									// 描画処理

    void Shrink();	// 軌跡を縮める処理

    void SetPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2);	// 座標設定
    void SetColor(D3DXCOLOR col);																// 色設定

    void BindTexture(int nNumTexture);	// テクスチャ設定
private:
    LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
    LPDIRECT3DVERTEXBUFFER9 m_pBuffMat;		// 頂点バッファへのポインタ
    D3DXCOLOR m_col;						// 座標
    D3DXMATRIX m_mtxWorld;					// マトリックス
    TYPE_LOCUS m_TypeLucus;					// 軌跡の種類
    float m_ShrinkSpeed;					// 軌跡を縮める速度
};
#endif