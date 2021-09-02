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
    static CNumber *Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool bMove, D3DXVECTOR3 centerPos = DEFAULT_VECTOR);

    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    void SetNumber(int nNumber);
    void SetAlpha(float fAlpha) { m_col.a = fAlpha; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetDigitInfo(int nNumDigit, int nThisDigit) { m_nNumDigit = nNumDigit; m_nThisDigit = nThisDigit; }

private:
    int m_nTexType;                         // �g���e�N�X�`���ԍ�
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
    D3DXVECTOR3				m_pos;	        // �ʒu
    D3DXVECTOR3             m_size;         // �傫��
    D3DXCOLOR               m_col;          // �F

    bool m_bMove;                           // �������ǂ���
    int m_nCntTime;                         // ���Ԃ��J�E���g
    int m_nNumDigit;                        // ����
    int m_nThisDigit;                       // �����ڂ�
    D3DXVECTOR3 m_centerPos;                // ���S�ʒu
};

#endif