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
    static CNumber *Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool bMove, D3DXVECTOR3 centerPos = DEFAULT_VECTOR);

    /*========================================================
    // セッター
    //======================================================*/
    void SetNumber(int nNumber);
    void SetAlpha(float fAlpha) { m_col.a = fAlpha; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetDigitInfo(int nNumDigit, int nThisDigit) { m_nNumDigit = nNumDigit; m_nThisDigit = nThisDigit; }

private:
    int m_nTexType;                         // 使うテクスチャ番号
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
    D3DXVECTOR3				m_pos;	        // 位置
    D3DXVECTOR3             m_size;         // 大きさ
    D3DXCOLOR               m_col;          // 色

    bool m_bMove;                           // 動くかどうか
    int m_nCntTime;                         // 時間をカウント
    int m_nNumDigit;                        // 桁数
    int m_nThisDigit;                       // 何桁目か
    D3DXVECTOR3 m_centerPos;                // 中心位置
};

#endif