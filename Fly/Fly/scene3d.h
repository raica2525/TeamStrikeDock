//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	scene3d.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//前方宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CModel;

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//オブジェクトクラス
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CScene3d : public CScene
{
public:
	CScene3d();
	virtual ~CScene3d();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetScale(D3DXVECTOR3 scale);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetRotation(void);
	D3DXVECTOR3 GetScale(void);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポイン,タ
	D3DXVECTOR3	m_pos;								// 位置
	D3DXVECTOR3 m_rot;								// 回転情報
	D3DXVECTOR3 m_scale;							// サイズ
};

#endif