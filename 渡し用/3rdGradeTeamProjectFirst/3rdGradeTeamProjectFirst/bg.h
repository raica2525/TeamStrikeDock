//====================================================================
//
// �w�i�̏��� (bg.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _BG_H_
#define _BG_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// �O���錾
//================================================

//================================================
// �}�N����`
//================================================

//================================================
// �N���X�錾
//================================================

// �w�i�N���X
class CBg : public CScene3D
{
public:

    // �J���[�t�F�[�Y
    typedef enum
    {
        COLOR_PHASE_R_DOWN = 0,
        COLOR_PHASE_R_UP,
        COLOR_PHASE_G_DOWN,
        COLOR_PHASE_G_UP,
        COLOR_PHASE_B_DOWN,
        COLOR_PHASE_B_UP,
    }COLOR_PHASE;

    CBg();
    ~CBg();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CBg *Create(int nModelType, D3DXVECTOR3 pos);
 
private:
    int m_nModelType;       // �g�����f���̎��
    D3DXCOLOR m_col;        // �F
    COLOR_PHASE m_colPhase; // �J���[�t�F�[�Y
};

#endif