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

    CBg();
    ~CBg();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CBg *Create(void);
 
private:
  
};

#endif