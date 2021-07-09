//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	wave.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#ifndef _WAVE_H_
#define _WAVE_H_
#include "main.h"
#include "scene.h"
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�O���錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
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
    LPDIRECT3DVERTEXBUFFER9 m_apVtxBuff[2]; // ���ۂɕ`�悷���ƁA�X�e���V���p��2�K�v
};
#endif