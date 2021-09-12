//====================================================================
//
// �V�[�����2D�|���S������ (scene2d.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene.h"

//================================================
// �}�N����`
//================================================
#define MAX_ANIM_COUNTER (10000) // �A�j���[�V�����̃J�E���^�𗘗p����ۂ́A�ő�J�E���g

//================================================
// �N���X�錾
//================================================

// 2D�|���S���N���X
class CScene2D : public CScene
{
public:

    // ����
    typedef enum
    {
        DIRECT_VERTICAL,    // �c
        DIRECT_HORIZON,     // ��
        DIRECT_RIGHT_UP,    // �E���オ��
        DIRECT_RIGHT_DOWN,  // �E��������
        DIRECT_MAX          // �����̍ő吔
    }DIRECT;

    CScene2D();
    CScene2D(OBJTYPE objType);
    virtual ~CScene2D();
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindTexture(const int nNumTexture);                                                      // �e�N�X�`�������蓖�Ă�
    void SetVertex(void);                                                                         // ���_���W��ݒ�
    void SetRotVertex(float fAngle);                                                              // ��]���钸�_���W��ݒ�
    void SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual);                          // ��������̒��_���W��ݒ�
    void SetAdditiveSynthesis(void) { m_bAdditiveSynthesis = true; }                              // ���Z�����ɂ���
    void SetAlphaTest(void) { m_bAlphaTest = true; }                                              // ���e�X�g������
    void SetNega(void) { m_bNega = true; }                                                        // ���]����������
    void SetPosition(D3DXVECTOR3 pos);                                                            // �ʒu��ݒ�
    D3DXVECTOR3 GetPosition(void) { return m_pos; }                                               // �ʒu���擾
    void SetSize(D3DXVECTOR3 size);                                                               // �傫����ݒ�
    D3DXVECTOR3 GetSize(void) { return m_size; }                                                  // �傫�����擾
    bool SetAnimation(int nSpeed, int nPattern);                                                  // �A�j���[�V����
    bool SetReverseAnimation(int nSpeed, int nPattern);                                           // �t�����̃A�j���[�V����
    void SetFlowingAnimation(int nSpeed, int nPattern, bool bRightToLeft, DIRECT direct);         // �����A�j���[�V����
    void SetTextureRange(int nRange, int nPattern);                                               // �e�N�X�`���̕`��͈͂�ݒ�
    void SetParagraphAnimation(int nParagraph, int nMaxParagraph, int nSpeed, int nPattern);      // �i���̂���A�j���[�V����
    void SetTexturePlace(int nPlace, int nPattern);                                               // �e�N�X�`���̕`��ꏊ�����߂�
    void SetParagraphTexturePlace(int nPlace, int nParagraph, int nMaxParagraph, int nPattern);   // �i���̂���A�j���[�V��������ꕔ����؂���
    int CountAnimation(int nSpeed, int nPattern);                                                 // �A�j���[�V�����̃J�E���^�𗘗p����
    void ResetCountAnim(void) { m_nCounterAnim = 0; m_nPatternAnim = 0; }                         // �A�j���[�V�����̃J�E���^�����Z�b�g
    void SetColor(D3DXCOLOR col);                                                                 // �F��ς���
    void SetLeftToRightGauge(float fMax, float fNow);                                             // ������E�ɐL�т��Q�[�W

private:
    static bool m_bAdditiveSynthesis;       // ���Z�������邩�ǂ���
    static bool m_bAlphaTest;               // ���e�X�g���邩�ǂ���
    static bool m_bNega;                    // ���]�������邩�ǂ���
    LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
    D3DXVECTOR3				m_pos;	        // �ʒu
    D3DXVECTOR3             m_size;         // �傫��
    int m_nCounterAnim;                     // �A�j���[�V�����J�E���^
    int m_nPatternAnim;                     // �A�j���[�V�����p�^�[��
};

#endif