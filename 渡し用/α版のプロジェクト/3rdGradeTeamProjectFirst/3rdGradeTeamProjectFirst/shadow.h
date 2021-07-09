//=============================================================================
//
// �e���� [shadow.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define SHADOW_POS_Y 1.0f
#define SHADOW_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.75f)
#define SHADOW_SIZE_RATE 50.0f

//*****************************************************************************
// �e�N���X��`
//*****************************************************************************
class CShadow :public CBillboard
{
public:

    CShadow(CScene::OBJTYPE objtype);
    ~CShadow();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static CShadow * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col);

    void SetRotAngle(float fAngle) { m_fRotAngle = fAngle; }     // ����������
    void Unable(void) { m_bUse = false; }                        // ����

private:
    float m_fRotAngle;  // ��]�̌���
    bool m_bUse;        // �g�p���邩�ǂ���
};
#endif
