//====================================================================
//
// シーン上の3Dモデル処理 (model.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

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

// 3Dモデルクラス（シーンを継承していない）
class CModel
{
public:
    CModel();
    ~CModel();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(CModel *model, bool bDeath = false);
    void Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bDeath = false);
    void BindModelData(const int nNumModelData);
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    D3DXMATRIX GetWorldMtx(void) { return m_mtxWorld; }

private:
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

    D3DCOLORVALUE m_aDiffuse[MAX_MATERIAL];  // 拡散光
};

#endif