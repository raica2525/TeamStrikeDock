#pragma once
//====================================================================
//
// クリッピングマスク生成(cliping_musk.h)
// Author : 池田悠希
//
//====================================================================
#ifndef _CLIPING_MUSK_H_
#define _CLIPING_MUSK_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "library.h"

//================================================
// マクロ定義
//================================================
#define MAX_REFERENCE_VALUE 0xff

//================================================
// クラス宣言
//================================================
class CClipingMusk
{
public:

	CClipingMusk();
	~CClipingMusk();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(const int nNumTexture);																	// テクスチャを割り当てる
	void SetVertex(void);                                                                                       // 頂点座標を設定
	void SetRotVertex(float fAngle);                                                                            // 回転する頂点座標を設定
	void SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual);                                        // 見かけ上の頂点座標を設定
	static CClipingMusk* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumTexture);
	static CClipingMusk*Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	//========================================================
	// セッター
	//========================================================
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosTo2D(D3DXVECTOR3 pos3D) { m_pos = ConvertScreenPos(pos3D); }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetAlphaTestBorder(int nValue) { m_nAlphaTestBorder = nValue; }

	//========================================================
	// ゲッター
	//========================================================
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	DWORD GetReferenceValue(void) { return m_dwReferenceValue; }
private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		                // 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;	                        // 位置
	D3DXVECTOR3             m_size;                         // 大きさ

	static bool m_abUsingReferenceValue[MAX_REFERENCE_VALUE];	// 参照値を使用しているかどうか
	DWORD m_dwReferenceValue;									// 参照値保存用変数
	int m_nAlphaTestBorder;										// アルファテストの境界値
};

#endif