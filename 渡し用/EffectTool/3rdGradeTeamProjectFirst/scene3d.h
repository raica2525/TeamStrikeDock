//====================================================================
//
// シーン上の3Dポリゴン処理 (scene3d.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"
#include "modelData.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//================================================
// クラス宣言
//================================================

// 3Dポリゴンクラス
class CScene3D : public CScene
{
public:
    CScene3D();
    CScene3D(OBJTYPE objType);
    virtual ~CScene3D();
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindModelData(const int nNumModelData);
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    D3DXVECTOR3 GetScale(void) { return m_scale; }

    static void SetAdditiveSynthesis(void) { m_bAdditiveSynthesis = true; }                              // 加算合成にする

private:
    static bool m_bAdditiveSynthesis;       // 加算合成するかどうか

    //======================================
    // Bindで結びつけ、Drawの処理速度を上げる
    LPDIRECT3DTEXTURE9	    m_apTexMat[MAX_MATERIAL];
    LPD3DXMESH				m_pMesh;
    LPD3DXBUFFER			m_pBuffMat;
    DWORD					m_nNumMat;
    //======================================

    D3DXMATRIX			    m_mtxWorld;		// ワールドマトリックス
    D3DXVECTOR3				m_pos;			// 座標
    D3DXVECTOR3				m_rot;			// 回転
    D3DXVECTOR3				m_scale;
};

#endif