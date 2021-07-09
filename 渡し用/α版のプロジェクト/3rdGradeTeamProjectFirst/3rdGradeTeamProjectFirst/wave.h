//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	wave.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _WAVE_H_
#define _WAVE_H_
#include "main.h"
#include "scene.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//前方宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CWave : public CScene
{
public:
    CWave();
    ~CWave();

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CWave* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    void CreateStencil(int vtxIndex);

    D3DXVECTOR3 m_pos;
    D3DXVECTOR3 m_size;
    LPDIRECT3DTEXTURE9		m_pTexture;
    LPDIRECT3DVERTEXBUFFER9 m_apVtxBuff[2]; // 実際に描画するやつと、ステンシル用で2つ必要
};
#endif