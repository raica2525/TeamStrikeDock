//====================================================================
//
// 背景の処理 (bg.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BG_H_
#define _BG_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// 前方宣言
//================================================

//================================================
// マクロ定義
//================================================

//================================================
// クラス宣言
//================================================

// 背景クラス
class CBg : public CScene3D
{
public:

    // カラーフェーズ
    typedef enum
    {
        COLOR_PHASE_R_DOWN = 0,
        COLOR_PHASE_R_UP,
        COLOR_PHASE_G_DOWN,
        COLOR_PHASE_G_UP,
        COLOR_PHASE_B_DOWN,
        COLOR_PHASE_B_UP,
    }COLOR_PHASE;

    CBg();
    ~CBg();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CBg *Create(int nModelType, D3DXVECTOR3 pos);
 
private:
    int m_nModelType;       // 使うモデルの種類
    D3DXCOLOR m_col;        // 色
    COLOR_PHASE m_colPhase; // カラーフェーズ
};

#endif