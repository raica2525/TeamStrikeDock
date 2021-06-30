//====================================================================
//
// ��������(�I�u�W�F�N�g���p�����Ă��Ȃ�) (number.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �}�N����`
//================================================

//================================================
// �N���X�錾
//================================================

// �����N���X
class CNumber
{
public:
    CNumber();
    ~CNumber();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    bool Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool bMove);
    void SetNumber(int nNumber);
    void SetAlpha(float fAlpha) { m_col.a = fAlpha; }

private:
    static LPDIRECT3DTEXTURE9   m_pTexture;	// �e�N�X�`���ւ̃|�C���^
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
    D3DXVECTOR3				m_pos;	        // �ʒu
    D3DXVECTOR3             m_size;         // �傫��
    D3DXCOLOR               m_col;          // �F
    bool m_bMove;                           // �������ǂ���    
};

#endif