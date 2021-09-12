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
#define KEY_MAX 10          // �L�[�Z�b�g�̍ő吔
#define ANIMATION_MAX 24    // ���Ă�A�j���[�V�����̍ő吔(0�Ԗڂ́A�K���ҋ@���[�V�����ɂ���)

//================================================
// �N���X�錾
//================================================

//�A�j���[�V�����N���X
class CAnimation
{
public:
    CAnimation();
    ~CAnimation();

    // �񋓌^�錾(animation.h�ɂ��ǉ����K�v)
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_HUMAN,		// �v���C���[
        TYPE_SPIDER,	// �w�
        TYPE_ANT,		// �a
        TYPE_FORCE,		// �h�q�R
        TYPE_UFO,		// UFO
        TYPE_TOWER,		// �^���[
        TYPE_WYVERN,	// ���C�o�[��
        TYPE_MAX		// �ő�l
    }ANIM_TYPE;

    // �\���̐錾
    typedef struct
    {
        D3DXVECTOR3 pos[KEY_MAX];
        D3DXVECTOR3 rot[KEY_MAX];
    }PARTS_INFO;

    typedef struct
    {
        PARTS_INFO  parts[CHARCTER_PARTS_MAX];
        int			nFrame[KEY_MAX];
        int			nLoop;
        int			nKeyNum;
    }KEY_INFO;

    HRESULT Init(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type);									// ����������
    void Uninit(void);																						// �I������
    void Update(void);																						// �X�V����
    void Draw(void);																						// �`�揈��
    static CAnimation * Create(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type, char* cFilePass);		// �C���X�^���X��������
    void LoadAnimation(ANIM_TYPE type, char* pass);															// �A�j���[�V�����̓ǂݍ��ݏ���
    void SetAnimation(int anim) { m_nAnimation = anim; }													// �A�j���[�V�����ݒ菈��
    int GetAnimation(void) { return m_nAnimation; }															// �C���X�^���X�擾�p

private:
    KEY_INFO				m_keyInfo[TYPE_MAX][ANIMATION_MAX];	// �L�[�A�j���[�V�����f�[�^�ێ��p		
    int				        m_nAnimation;						// ���݂̃A�j���[�V����
    int				        m_nAnimationOld;					// �O��̃A�j���[�V����
    ANIM_TYPE				m_type;								// �A�j���[�V�����̃^�C�v
    D3DXVECTOR3				m_pos[CHARCTER_PARTS_MAX];			// ���W
    D3DXVECTOR3				m_posDest[CHARCTER_PARTS_MAX];		// ���W�ړ���
    D3DXVECTOR3				m_rot[CHARCTER_PARTS_MAX];			// ��]
    D3DXVECTOR3				m_rotDest[CHARCTER_PARTS_MAX];		// ��]�ړ���
    CCharacter				*m_pCharacter;						// �L�����N�^�[�̃C���X�^���X
    int						m_nFrame;							// �t���[����
    int						m_nKey;								// �L�[��
    int						m_nPartsNum;						// ���f���̃p�[�c��
    bool					m_bAdjustment;						// �p�x���C��������
};

#endif