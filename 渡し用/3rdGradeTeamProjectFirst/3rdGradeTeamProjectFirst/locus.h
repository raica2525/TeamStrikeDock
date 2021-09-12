//=============================================================================
//
// �O�Ղ̏��� [locus.h]
// Author : �ɓ��z��
//
//=============================================================================
#ifndef _LOCUS_H_
#define _LOCUS_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "scene3d.h"

//=============================================================================
// �O�ՃN���X
//=============================================================================
class CLocus : public CScene
{
public:
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_SHRINK,	// �O�Ղ��k�߂�
        TYPE_MAX
    }TYPE_LOCUS;

    CLocus(OBJTYPE objtype = OBJTYPE_EFFECT3D);
    ~CLocus();

    static CLocus *Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2, D3DXCOLOR col, TYPE_LOCUS TypeLucus = TYPE_NONE, float ShrinkSpeed = 0.0f);	// ��������

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
    void Uninit(void);									// �I������
    void Update(void);									// �X�V����
    void Draw(void);									// �`�揈��

    void Shrink();	// �O�Ղ��k�߂鏈��

    void SetPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2);	// ���W�ݒ�
    void SetColor(D3DXCOLOR col);																// �F�ݒ�

    void BindTexture(int nNumTexture);	// �e�N�X�`���ݒ�
private:
    LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
    LPDIRECT3DVERTEXBUFFER9 m_pBuffMat;		// ���_�o�b�t�@�ւ̃|�C���^
    D3DXCOLOR m_col;						// ���W
    D3DXMATRIX m_mtxWorld;					// �}�g���b�N�X
    TYPE_LOCUS m_TypeLucus;					// �O�Ղ̎��
    float m_ShrinkSpeed;					// �O�Ղ��k�߂鑬�x
};
#endif