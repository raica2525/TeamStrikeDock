//====================================================================
//
// UI�̏��� (ui.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _UI_H_
#define _UI_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// �}�N����`
//================================================

#define MAX_ACTION 4        // �A�N�V�����̍ő吔
#define MAX_ACTION_PARAM 8  // �A�N�V�����̕⏕�l�̍ő吔

#define MAX_ACCESS_NUM 128  // �A�N�Z�X�ł���UI�̍ő吔

//================================================
// �N���X�錾
//================================================

// UI�N���X
class CUI : public CScene2D
{
public:

    // �Z�b�g
    typedef enum
    {
        SET_NONE = 0,
        SET_TITLE,
        SET_MANUAL,
        SET_CUSTOM,
        SET_GAME,
        SET_RESULT,
        SET_MENU,
        SET_MAX
    }SET;

    // �����̎��
    typedef enum
    {
        ACTION_NONE = 0,    // �Ȃ��i�⏕�l���p�������Ƃ��Ɂj
        ACTION_GAUGE,       // �Q�[�W
        ACTION_SIZE,        // �T�C�Y�ύX
        ACTION_POS,         // �ʒu�ύX
        ACTION_ALPHA,       // �����x�ύX
        ACTION_COLOR,       // �F�ύX
        ACTION_ROT,         // �p�x�ύX
        ACTION_TEX_BREND,   // �e�N�X�`���u�����h
        ACTION_LOOP_ANIM,   // ���[�v�A�j���[�V����
        ACTION_TEX_PLACE,   // �e�N�X�`���̕`��ꏊ�w��
        ACTION_EMIT_EFFECT, // �G�t�F�N�g����
        ACTION_MAX
    }ACTION;

    // �A�N�V�����̏��
    typedef struct
    {
        int action;                            // �A�N�V����
        int nCntTime;                          // ���Ԃ��J�E���g
        float afParam[MAX_ACTION_PARAM];       // �⏕�l
        float afMemoryParam[MAX_ACTION_PARAM]; // �L���p�⏕�l
        bool bLock;                            // ���b�N����Ă��邩�ǂ���
        bool bMemoryLock;                      // �L���p���b�N
    }ActionInfo;

    // ���������̎��
    typedef enum
    {
        RIMIT_NONE = 0,                // �����Ȃ�
        RIMIT_TO_FRAME,                // �w��t���[���܂�
        RIMIT_FROM_FRAME,              // �w��t���[������
        RIMIT_TO_VALUE,                // �w��l�܂�
        RIMIT_REPEAT_FRAME,            // �w��t���[�����J��Ԃ�
        RIMIT_REPEAT_VALUE,            // �w��l���J��Ԃ�
        RIMIT_EQUAL_VALUE_FROM_FRAME,  // (�w��t���[������)�w��l�ɂ���
    }RIMIT;

    // �⏕�l�̓���i�T�C�Y�ҁj
    typedef enum
    {
        PARAM_SIZE_CHANGE_RATE_X = 0,    // X���ύX����
        PARAM_SIZE_CHANGE_RATE_Y,        // (Y���ύX����)
        PARAM_SIZE_EQUAL_RATIO,          // ����ό`���ǂ���
        PARAM_SIZE_RIMIT,                // ����
        PARAM_SIZE_FRAME,                // �t���[����
        PARAM_SIZE_VALUE_X,               // X�w��l
        PARAM_SIZE_VALUE_Y,               // X�w��l
    }PARAM_SIZE;

    // �⏕�l�̓���i�ʒu�ҁj
    typedef enum
    {
        PARAM_POS_CHANGE_RATE_X = 0,     // X���ύX����
        PARAM_POS_CHANGE_RATE_Y,         // Y���ύX����
        PARAM_POS_ACCEL,                 // �����x
        PARAM_POS_RIMIT,                 // ����
        PARAM_POS_FRAME,                 // �t���[����
        PARAM_POS_VALUE_X,               // X�w��l
        PARAM_POS_VALUE_Y,               // X�w��l
    }PARAM_POS;

    // �⏕�l�̓���i�����x�ҁj
    typedef enum
    {
        PARAM_ALPHA_CHANGE_RATE = 0,     // �ύX����
        PARAM_ALPHA_RIMIT,               // ����
        PARAM_ALPHA_FRAME,               // �t���[����
        PARAM_ALPHA_VALUE,               // �w��l
    }PARAM_ALPHA;

    // �⏕�l�̓���i�F�ς��ҁj
    typedef enum
    {
        PARAM_COLOR_CHANGE_RATE_R = 0,   // R�ύX����
        PARAM_COLOR_CHANGE_RATE_G,       // G�ύX����
        PARAM_COLOR_CHANGE_RATE_B,       // B�ύX����
        PARAM_COLOR_RIMIT,               // ����
        PARAM_COLOR_FRAME,               // �t���[����
        PARAM_COLOR_VALUE_R,             // R�w��l
        PARAM_COLOR_VALUE_G,             // G�w��l
        PARAM_COLOR_VALUE_B,             // B�w��l
    }PARAM_COLOR;

    // �⏕�l�̓���i��]�ҁj
    typedef enum
    {
        PARAM_ROT_CHANGE_RATE = 0,       // �p�x�ύX����
        PARAM_ROT_RIMIT,                 // ����
        PARAM_ROT_FRAME,                 // �t���[����
        PARAM_ROT_VALUE,                 // �w��l
    }PARAM_ROT;

    // �⏕�l�̓���i�e�N�X�`���u�����h�ҁj
    typedef enum
    {
        PARAM_TEX_BREND_TEX_NUMBER = 0,  // �e�N�X�`���u�����h�Ɏg���e�N�X�`���ԍ�
        PARAM_TEX_BREND_HOW_TO,          // �������@
        PARAM_TEX_BREND_ONE_ROUND_FRAME, // ����t���[����
        PARAM_TEX_BREND_INTERVAL_FRAME,  // �C���^�[�o���t���[����
        PARAM_TEX_BREND_RIGHT_TO_LEFT,   // �E���獶��
        PARAM_TEX_BREND_DIRECT,          // ����
        PARAM_TEX_BREND_IDX,             // �e�N�X�`���u�����h�Ɏg���C���f�b�N�X�i1�`3�j
    }PARAM_TEX_BREND;

    // �⏕�l�̓���i���[�v�A�j���[�V�����ҁj
    typedef enum
    {
        PARAM_LOOP_ANIM_ONE_ROUND_FRAME = 0, // ����t���[����
        PARAM_LOOP_ANIM_INTERVAL_FRAME,      // �C���^�[�o���t���[����
        PARAM_LOOP_ANIM_RIGHT_TO_LEFT,       // �E���獶��
        PARAM_LOOP_ANIM_DIRECT,              // ����
    }PARAM_LOOP_ANIM;

    // �⏕�l�̓���i�e�N�X�`���̕`��ꏊ�w��ҁj
    typedef enum
    {
        PARAM_TEX_PLACE_PATTERN = 0, // ����������
        PARAM_TEX_PLACE_PLACE,       // ���Ԗڂ�
    }PARAM_TEX_PLACE;

    // �⏕�l�̓���i�G�t�F�N�g�����ҁj
    typedef enum
    {
        PARAM_EMIT_EFFECT_TYPE = 0,            // �p�x�ύX����
        PARAM_EMIT_EFFECT_RIMIT,               // ����
        PARAM_EMIT_EFFECT_FRAME,               // �t���[����
        PARAM_EMIT_EFFECT_ADJUST_EMIT_POS_X,   // �����ʒuX�����炷�l
        PARAM_EMIT_EFFECT_ADJUST_EMIT_POS_Y,   // �����ʒuY�����炷�l
    }PARAM_EMIT_EFFECT;

    CUI(CScene::OBJTYPE objtype);
    ~CUI();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CUI *Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, int nRotAngle, D3DXCOLOR col,
        bool bFrontText = false, bool bUseAddiveSynthesis = false, int nAlphaTestBorder = 0, bool bUseZBuffer = false,
        D3DXVECTOR3 collisionPos = DEFAULT_VECTOR, D3DXVECTOR3 collisionSize = DEFAULT_VECTOR);
    static void Place(SET set);     // �O���t�@�C������UI�̔z�u

    /*========================================================
    // �Q�b�^�[
    //======================================================*/
    D3DXCOLOR GetCol(void) { return m_col; }
    D3DXVECTOR3 GetCollisionPos(void) { return m_collisionPos; }
    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }
    static CUI* GetAccessUI(int nNum);
    float GetActionParam(int nNumAction, int nNumParam) { return m_aActionInfo[nNumAction].afParam[nNumParam]; }
    D3DXVECTOR3 GetMemorySize(void) { return m_memorySize; }
    bool GetDisp(void) { return m_bDisp; }

    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    void SetAccessUI(int nNum);
    void SetDontUse(void) { m_bUse = false; }
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetAlpha(float fAlpha) { m_col.a = fAlpha; }
    void SetActionInfo(int nNum, int action, bool bLock,
        float fParam0 = 0, float fParam1 = 0, float fParam2 = 0, float fParam3 = 0,
        float fParam4 = 0, float fParam5 = 0, float fParam6 = 0, float fParam7 = 0);    // �����ł̂݁A�A�N�V�����̕⏕�l����̐ݒ���ł���
    void SetActionLock(int nNum, bool bLock);
    void SetActionReset(int nNum);
    void SetAllActionReset(void);
    void SetFirstPos(void) { SetPosition(m_memoryPos); }    // �A�N�V�����o�R�ł͂Ȃ��A�ŏ��̈ʒu�ɖ߂��Ƃ��g���i��:�J�[�\���̃��Z�b�g�j
    void SetDisp(bool bDisp) { m_bDisp = bDisp; }

    /*========================================================
    // �A�N�V����
    //======================================================*/
    void PlayAction(int nNum);           // �S�ẴA�N�V���������s�������
    void PlayActionSize(int nNum);       // �T�C�Y�A�N�V����
    void PlayActionPos(int nNum);        // �ʒu�A�N�V����
    void PlayActionAlpha(int nNum);      // �����x�A�N�V����
    void PlayActionColor(int nNum);      // �F�ς��A�N�V����
    void PlayActionRot(int nNum);        // ��]�A�N�V����
    void PlayActionTexBrend(int nNum);   // �e�N�X�`���u�����h�A�N�V����
    void PlayActionLoopAnim(int nNum);   // ���[�v�A�j���[�V�����A�N�V����
    void PlayActionEmitEffect(int nNum); // �G�t�F�N�g�����A�N�V����

    /*========================================================
    // �A�N�V�����p�̋��ʏ���
    //======================================================*/
    void RimitToValue(const float fChangeRate, float& fCurrentValue, const float fDestValue, bool& bUpdate);
    void RimitRepeatValue(float& fChangeRate, const float fMemoryValue, const float fCurrentValue, const float fDestValue, bool& bUpdate);

private:
    static CUI* m_apAccessUI[MAX_ACCESS_NUM];   // �A�N�Z�X�ł���UI
    int m_nTexType;                             // �g���e�N�X�`���̎��
    ActionInfo m_aActionInfo[MAX_ACTION];       // �����̏��
    bool m_bUse;                                // �g�p���邩�ǂ���

    float m_fRotAngle;                          // ��]�p�x
    D3DXCOLOR m_col;                            // �F
    bool m_bUseAdditiveSynthesis;               // ���Z�����ɂ��邩�ǂ���

    D3DXVECTOR3 m_memoryPos;                    // �L���p�ʒu
    D3DXVECTOR3 m_memorySize;                   // �L���p�傫��
    float m_fMemoryRotAngle;                    // �L���p�p�x
    D3DXCOLOR m_memoryCol;                      // �L���p�F

    //int m_nCntAnimTime;                       // �A�j���[�V�����p�J�E���^�ieffect3d���玝���Ă����ۂɍ폜�Ascene2d�Ɍ��X���邽�߁j
    bool m_bOneRoundAnim;                       // �A�j���[�V����������������ǂ���
    int m_nAnimParagraph;                       // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
    int m_nAnimPattern;                         // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
    int m_nAnimSpeed;                           // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
    bool m_bRepeat;                             // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j

    D3DXVECTOR3 m_collisionPos;                 // �����蔻��̈ʒu
    D3DXVECTOR3 m_collisionSize;                // �����蔻��̑傫��
    int m_nAccessNum;                           // �A�N�Z�X�i���o�[
    bool m_bDisp;                               // �\�����邩�ǂ���
};

#endif