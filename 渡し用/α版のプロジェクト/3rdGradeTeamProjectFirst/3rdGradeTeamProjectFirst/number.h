//====================================================================
//
// 数字処理(オブジェクトを継承していない) (number.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// マクロ定義
//================================================

//================================================
// クラス宣言
//================================================

// 数字クラス
class CNumber
{
public:
    CNumber();
    ~CNumber();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    bool Update(void);
    void Draw(void);
    static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool bMove);
    void SetNumber(int nNumber);
    void SetAlpha(float fAlpha) { m_col.a = fAlpha; }

private:
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
    D3DXVECTOR3				m_pos;	        // 位置
    D3DXVECTOR3             m_size;         // 大きさ
    D3DXCOLOR               m_col;          // 色
    bool m_bMove;                           // 動くかどうか    
};

#endif