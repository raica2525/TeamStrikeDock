//====================================================================
//
// �A�j���[�V�������� (animation.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "character.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define KEY_MAX 16          // �L�[�Z�b�g�̍ő吔
#define ANIMATION_MAX 32    // ���Ă�A�j���[�V�����̍ő吔(0�Ԗڂ́A�K���ҋ@���[�V�����ɂ���)

//================================================
// �N���X�錾
//================================================

//�A�j���[�V�����N���X
class CAnimation
{
public:
    CAnimation();
    ~CAnimation();

    // �\���̐錾
    typedef struct
    {
        D3DXVECTOR3 aPos[KEY_MAX];
        D3DXVECTOR3 aRot[KEY_MAX];
    }PARTS_INFO;

    typedef struct
    {
        PARTS_INFO  aParts[CHARCTER_PARTS_MAX];
        int			anFrame[KEY_MAX];
        int			nLoop;
        int			nKeyNum;
    }KEY_INFO;

    HRESULT Init(CCharacter *pCharacter, int nPartsNum);									                // ����������
    void Uninit(void);																						// �I������
    void Update(void);																						// �X�V����
    void Draw(void);																						// �`�揈��
    static CAnimation * Create(CCharacter *pCharacter, int nPartsNum, char* cFilePass);		                // �C���X�^���X��������
    void LoadAnimation(char* pass);															                // �A�j���[�V�����̓ǂݍ��ݏ���
    void SetAnimation(int anim) { m_nAnimation = anim; }													// �A�j���[�V�����ݒ菈��
    int GetAnimation(void) { return m_nAnimation; }															// �C���X�^���X�擾�p
    void SetAnimPosReset(int nNum);

private:
    KEY_INFO				m_aKeyInfo[ANIMATION_MAX];	        // �L�[�A�j���[�V�����f�[�^�ێ��p		
    int				        m_nAnimation;						// ���݂̃A�j���[�V����
    int				        m_nAnimationOld;					// �O��̃A�j���[�V����
    D3DXVECTOR3				m_aPos[CHARCTER_PARTS_MAX];			// ���W
    D3DXVECTOR3				m_aPosDest[CHARCTER_PARTS_MAX];		// ���W�ړ���
    D3DXVECTOR3				m_aRot[CHARCTER_PARTS_MAX];			// ��]
    D3DXVECTOR3				m_aRotDest[CHARCTER_PARTS_MAX];		// ��]�ړ���
    CCharacter				*m_pCharacter;						// �L�����N�^�[�̃C���X�^���X
    int						m_nFrame;							// �t���[����
    int						m_nKey;								// �L�[��
    int						m_nPartsNum;						// ���f���̃p�[�c��
    bool					m_bAdjustment;						// �p�x���C��������
};

#endif