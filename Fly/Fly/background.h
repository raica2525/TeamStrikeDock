//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	background.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "main.h"
#include "scene.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�O���錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CPolygon3d;

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�N���X��`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CBackground : public CScene
{
public:
    CBackground();     //�R���X�g���N�^
    ~CBackground();    //�f�X�g���N�^

    HRESULT Init(void);    //����������
    void Uninit(void);     //�I������
    void Update(void);     //�X�V����
    void Draw(void);       //�`�揈��

    static CBackground* Create(float fDistanceFromCamera);    //�|���S����������
	static void Load(void);
private:
    static LPDIRECT3DTEXTURE9 m_pTexture;    // �e�N�X�`���ւ̃|�C���^
    D3DXVECTOR3 m_pos;                // �|���S���̈ʒu
    D3DXVECTOR3 m_size;
    CPolygon3d* m_pPolygon3d;
	float m_fDistanceFromCamera;
};

#endif
