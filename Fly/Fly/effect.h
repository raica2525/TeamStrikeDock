//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	effect.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "main.h"
#include "scene.h"
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�O���錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CEffect : public CScene
{
public:
	CEffect();
	~CEffect();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool bNega);

	static void Load(void);
	static void Unload(void);

private:
	void CreateStencil(int vtxIndex);

	static LPDIRECT3DTEXTURE9 m_apTexture[2];
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	bool m_bNega;
	LPDIRECT3DVERTEXBUFFER9 m_apVtxBuff[2];
};