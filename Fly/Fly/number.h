//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
// number.h
// Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#ifndef _NUMBER_H_
#define _NUMBER_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#include "main.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

class CNumber
{
public:
	CNumber();
	~CNumber();

	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	static HRESULT Load(void);
	static void Unload(void);

	static CNumber* Create(int nValue, D3DXVECTOR3 pos,D3DXVECTOR3 size);

	void SetValue(int nValue);
	void SetPos(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR3 size);
private:
	int m_nValue;
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
};

#endif