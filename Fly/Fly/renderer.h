//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	renderer.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) {return m_pD3DDevice;}
	LPDIRECT3DTEXTURE9 GetScreenTex(void) {return m_pTexture;}
private:
	LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
	LPDIRECT3DTEXTURE9		m_pTexture;
};

#endif
