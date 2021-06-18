//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	player.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#ifndef _BALL_H_
#define _BALL_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene.h"
#include "model.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

class CBall : public CScene
{
public:
	CBall();
	~CBall();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBall* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	static void Load(void);
	static void Unload(void);
	D3DXVECTOR3 GetPos(void);

private:
	CModel* m_pModel;
	static CModel::MODELDATA m_modeldata;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
};
#endif
