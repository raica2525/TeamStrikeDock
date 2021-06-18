//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	model.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _MODEL_H_
#define _MODEL_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene3d.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#define MAX_MATERIAL (16)

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CModel: public CScene3d
{
public:

	typedef struct
	{
		LPD3DXBUFFER pBuffMat;						// マテリアル情報へのポインタ
		DWORD nNumMat;								// マテリアル情報の数
		LPD3DXMESH pMesh;								// メッシュ情報へのポインタ
		LPDIRECT3DTEXTURE9 pTexture[MAX_MATERIAL];				// テクスチャへのポインタ
	}MODELDATA;

	CModel();
	virtual ~CModel();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CModel* Create(CModel* pParentModel, MODELDATA modeldata,D3DXVECTOR3 pos,D3DXVECTOR3 rot, D3DXVECTOR3 size);
	void BindModel(MODELDATA modeldata);
private:
	CModel* m_pParentModel;
	MODELDATA m_ModelData;
};
#endif