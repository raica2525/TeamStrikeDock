//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	model.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _SHADOW_H_
#define _SHADOW_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene3d.h"
#include "model.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#define MAX_MATERIAL (16)

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CShadow : public CScene3d
{
public:

	CShadow();
	virtual ~CShadow();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CShadow* Create(CModel *pParentModel, CModel::MODELDATA modeldata, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	void BindModel(CModel::MODELDATA modeldata);
private:
	CModel* m_pParentModel;
	CModel::MODELDATA m_ModelData;
};
#endif