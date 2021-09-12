//====================================================================
//
// �X�R�A���� (score.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

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
#define MAX_SCORE 8                                                         // �X�R�A�̌���
#define MAX_NUMBER 16                                                       // ���ʂɂ�����A�����̍ő吔

#define SCORE_POS (D3DXVECTOR3(530.0f - (300.0 / 2), 528.5f, 0.0f))         // �X�R�A�̈ʒu
#define SCORE_SIZE D3DXVECTOR3(150.0f, 30.0f, 0.0f)                         // �X�R�A�̑傫��

//================================================
// �N���X�錾
//================================================

// �X�R�A�̃N���X
class CScore : public CScene
{
public:
    CScore();
    ~CScore();
    static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // �������J��
    void Update(void);
    void Draw(void);
    void SetScore(const int nDisplayScore, D3DXVECTOR3 pos, D3DXCOLOR col);  // �������o���Ƃ�
    void AddScore(const int nValue);                                         // �X�R�A���Z����Ƃ�
    int GetScore(void) { return m_nScore; }                                  // �X�R�A���擾
    void SetScore(const int nScore);                                         // �X�R�A��ݒ�
    void SetDamage(const int nDisplayDamage, D3DXVECTOR3 pos, D3DXCOLOR col);// �_���[�W��ݒ�

private:
    static CNumber *m_apNumber[MAX_NUMBER];  // �������
    int m_nScore;                            // �X�R�A
};

#endif