//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	scene2d.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#ifndef _SCENE2D_H_
#define _SCENE2D_H_
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CScene2d : public CScene
{
public:

	CScene2d(int nPriority = 1);		//コンストラクタ
	virtual ~CScene2d();	//デストラクタ

	 virtual HRESULT Init(void);	//初期化処理
	 virtual void Uninit(void);		//終了処理
	 virtual void Update(void);		//更新処理
	 virtual void Draw(void);		//描画処理

	 void SetPos(D3DXVECTOR3 pos);			//位置設定処理
	 D3DXVECTOR3 GetPos(void);

	 void SetMove(D3DXVECTOR3 move);			//位置設定処理
	 D3DXVECTOR3 GetMove(void);

	 void SetSize(D3DXVECTOR3 size);
	 void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	 void SetTexPos(D3DXVECTOR2* pTexPos);
	 static CScene2d* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nPriority);	//ポリゴン生成処理

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_move;					//ポリゴンの移動量
	D3DXVECTOR3				m_size;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ

};

#endif