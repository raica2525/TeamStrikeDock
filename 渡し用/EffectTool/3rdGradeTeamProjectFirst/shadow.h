//=============================================================================
//
// 影処理 [shadow.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define SHADOW_POS_Y 1.0f
#define SHADOW_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.75f)
#define SHADOW_SIZE_RATE 50.0f

//*****************************************************************************
// 影クラス定義
//*****************************************************************************
class CShadow :public CBillboard
{
public:

    CShadow(CScene::OBJTYPE objtype);
    ~CShadow();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static CShadow * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col);

    void SetRotAngle(float fAngle) { m_fRotAngle = fAngle; }     // 向きを決定
    void Unable(void) { m_bUse = false; }                        // 消す

private:
    float m_fRotAngle;  // 回転の向き
    bool m_bUse;        // 使用するかどうか
};
#endif
