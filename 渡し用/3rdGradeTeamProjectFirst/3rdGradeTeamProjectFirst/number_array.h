//====================================================================
//
// �����z�񏈗� (number_array.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _NUMBER_ARRAY_H_
#define _NUMBER_ARRAY_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================
class CNumber;

//================================================
// �}�N����`
//================================================
#define MAX_NUMBER_ARRAY 8  // �����z��̌���

// �e��ʂ̐����̑傫��
#define NUMBER_SIZE_X_BALL_SPD 20.0f
#define NUMBER_SIZE_X_DAMAGE 17.5f

//================================================
// �N���X�錾
//================================================

// �����z��̃N���X
class CNumberArray : public CScene
{
public:
    CNumberArray();
    ~CNumberArray();
    static CNumberArray *Create(int nTexType, D3DXVECTOR3 pos, float fSizeX, D3DXCOLOR col, int nDispNumber, bool bDispUselessDigits, bool bMove = false);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // �������J��
    void Update(void);
    void Draw(void);
    void SetDispNumber(int nDispNumber, bool bMove = false);  // �\�����鐔����ݒ�

private:
    int m_nTexType;                         // �g���e�N�X�`���ԍ�
    CNumber *m_apNumber[MAX_NUMBER_ARRAY];  // �������

    D3DXVECTOR3 m_createPos;                // �����ʒu
    D3DXCOLOR m_createColor;                // �����F
    bool m_bDispUselessDigits;              // �s�v�Ȍ������`�悷�邩�ǂ���
    float m_fSizeX;                         // ���̑傫��
};

#endif