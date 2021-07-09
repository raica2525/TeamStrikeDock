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

// �o�[�W�����\�L
#define VERSION_POS D3DXVECTOR3(1150.0f, 675.0f, 0.0f)
#define VERSION_SIZE D3DXVECTOR3(150.0f,75.0f,0.0f)           

// �Ə�(�኱����)
#define LOCK_ON_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 22.5f, 0.0f))
#define LOCK_ON_SIZE (D3DXVECTOR3(60.0f, 60.0f , 0.0f))

// �����[�h(�����߂�)
#define RELOAD_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 70.0f, 0.0f))
#define RELOAD_SIZE (D3DXVECTOR3(180.0f, 60.0f , 0.0f))

// �^�C�g�����S
#define TITLE_LOGO_FIRST_POS (D3DXVECTOR3(350.0f, -150.0f, 0.0f))                   // �^�C�g�����S�̍ŏ��̈ʒu
#define TITLE_LOGO_TARGET_POS_Y 150.0f                                              // �^�C�g�����S�̖ڕW�ʒuY 
#define TITLE_LOGO_SIZE (D3DXVECTOR3(500.0f, 250.0f, 0.0f))                         // �^�C�g�����S�̃T�C�Y
#define TITLE_LOGO_FLOW_DOWN_SPEED 2.1f                                             // �^�C�g�����S�̗��ꗎ���鑬�x

// PLEASE PRESS START
#define PRESS_START_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f)                 // �X�^�[�g�{�^���������Ă˂̈ʒu
#define PRESS_START_SIZE D3DXVECTOR3(500.0f,200.0f,0.0f)                            // �X�^�[�g�{�^���������Ă˂̃T�C�Y
#define PRESS_START_INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)          // �X�^�[�g�{�^���������Ă˂̔�A�N�e�B�u�F
#define PRESS_START_FADE_RATE 0.01f                                                 // �X�^�[�g�{�^���������Ă˂̓_�Ŋ���
#define PRESS_START_FADE_OUT_MIN 0.6f                                               // �X�^�[�g�{�^���������Ă˂̃t�F�[�h�A�E�g�̍ŏ��l

// ���̖\���܂�
#define UI_LIMIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT - 45.0f, 0.0f))
#define UI_LIMIT_SIZE (D3DXVECTOR3(400.0f , 65.0f, 0.0f))

// �^�C�g��
#define TITLE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))         // �V�ѕ��̈ʒu
#define TITLE_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))                // �V�ѕ��̃T�C�Y

// �V�ѕ�
#define MANUAL_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))        // �V�ѕ��̈ʒu
#define MANUAL_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))               // �V�ѕ��̃T�C�Y

// ���U���g
#define RESULT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))        // ���U���g�̈ʒu
#define RESULT_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))               // ���U���g�̃T�C�Y

//================================================================================================================================================
// �V���ύX�ӏ�

// HP�Q�[�W(�g)
#define UI_STAMINA_POS (D3DXVECTOR3(580.0f - (350.0f / 2), 90.0f, 0.0f))
#define UI_STAMINA_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// �Q�[�W
#define UI_GAUGE_POS (D3DXVECTOR3(580.0f - (350.0f / 2), 90.0f, 0.0f))
#define UI_GAUGE_SIZE (D3DXVECTOR3(290.0f , 34.0f, 0.0f))

// HP���S
#define UI_HP_LOGO_POS (D3DXVECTOR3(250.0f - (170.0f / 2), 100.0f, 0.0f))
#define UI_HP_LOGO_SIZE (D3DXVECTOR3(170.0f , 85.0f, 0.0f))

// ����t���[��
#define UI_LEFTHAND_FRAME_POS (D3DXVECTOR3(480.0f - (425.0 / 2), 560.0f, 0.0f))
#define UI_LEFTHAND_FRAME_SIZE (D3DXVECTOR3(425.0f ,150.0f, 0.0f))

// ����Q�[�W
#define UI_LEFTHAND_GAUGE_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_GAUGE_SIZE (D3DXVECTOR3(288.0f , 49.5f, 0.0f))

// ����Q�[�W�w�i
#define UI_LEFTHAND_GAUGE_BG_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_GAUGE_BG_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// ����Q�[�W�t���[��
#define UI_LEFTHAND_GAUGE_FRAME_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_GAUGE_FRAME_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// ����d�؂�
#define UI_LEFTHAND_PARTITION_POS (D3DXVECTOR3(470.0f - (300.0f / 2), 595.0f, 0.0f))
#define UI_LEFTHAND_LOGO_SIZE (D3DXVECTOR3(300.0f , 50.0f, 0.0f))

// �E�胍�S
#define UI_RIGHTHAND_LOGO_POS (D3DXVECTOR3(900.0f - (75.0 / 2), 590.0f, 0.0f))
#define UI_RIGHTHAND_LOGO_SIZE (D3DXVECTOR3(80.0f , 80.0f, 0.0f))

// �E��c�e
#define UI_RIGHTHAND_STAMINA_POS (D3DXVECTOR3(1210.0f - (300.0 / 2), 585.0f, 0.0f))
#define UI_RIGHTHAND_STAMINA_SIZE (D3DXVECTOR3(300.0f , 100.0f, 0.0f))

// �}�b�v
#define UI_MINIMAP_POS (D3DXVECTOR3(1200.0f - (350.0f / 2), 150.0f, 0.0f))
#define UI_MINIMAP_SIZE (D3DXVECTOR3(350.0f , 350.0f, 0.0f))

// �X�R�A
#define UI_SCORE_POS (D3DXVECTOR3(480.0f - (300.0 / 2), 530.0f, 0.0f))
#define UI_SCORE_SIZE (D3DXVECTOR3(300.0f ,40.0f, 0.0f))

//================================================================================================================================================================================================

// �~�b�V�������S�̈ʒu�ƃT�C�Y
#define UI_MISSION_LOGO_POS (D3DXVECTOR3(200.0f, 212.5f, 0.0f))
#define UI_MISSION_LOGO_SIZE (D3DXVECTOR3(160.0f, 40.0f, 0.0f))

// �~�b�V����UI�̈ʒu�ƃT�C�Y
#define UI_MISSION_POS_1 (D3DXVECTOR3(200.0f, 270.0f, 0.0f))
#define UI_MISSION_SIZE_1 (D3DXVECTOR3(320.0f, 80.0f, 0.0f))
#define UI_MISSION_POS_2 (D3DXVECTOR3(200.0f, 320.0f, 0.0f))
#define UI_MISSION_SIZE_2 (D3DXVECTOR3(320.0f, 80.0f, 0.0f))

// �g�傷��~�b�V�����n
#define UI_EXTEND_MISSION_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 0.0f))  // �ʒu
#define UI_EXTEND_MISSION_MAX_SIZE_X (600.0f)                                                   // ���̍ő�T�C�Y
#define UI_EXTEND_MISSION_MAX_SIZE_Y (300.0f)                                                   // �c�̍ő�T�C�Y
#define UI_EXTEND_MISSION_EXTEND_RATE_X (15.0f)                                                 // ���̊g�劄��
#define UI_EXTEND_MISSION_EXTEND_RATE_Y (7.5f)                                                  // �c�̊g�劄��
#define UI_EXTEND_MISSION_FADE_OUT_COUNT (180)                                                  // �t�F�[�h���n�܂�J�E���^
#define UI_EXTEND_MISSION_FADE_OUT_RATE (0.05f)                                                 // �t�F�[�h�A�E�g����

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
        SET_GAME,
        SET_RESULT,
        SET_MAX
    }SET;

    // �����̎��
    typedef enum
    {
        ACTION_NONE = 0,
        ACTION_TITLE_LOGO,
        ACTION_TITLE_PRESS_ENTER,
        ACTION_GAUGE,
        ACTION_MAX
    }ACTION;

    CUI();
    ~CUI();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CUI *Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, ACTION action);
    static void Place(SET set);

    D3DXVECTOR3 Extend(D3DXVECTOR3 size);           // �g�傷��UI

    void SetDontUse(void) { m_bUse = false; }       // �g��Ȃ����̂̔j��

private:
    int m_nTexType;                                    // �g���e�N�X�`���̎��
    ACTION m_action;                                   // �����̎��
    D3DXVECTOR3 m_move;                                // �ړ���
    D3DXCOLOR m_col;                                   // �F
    float m_fAngle;                                    // �p�x
    bool m_bFadeOut;                                   // �t�F�[�h�A�E�g���ǂ���
    bool m_bBlinking;                                  // �_�Œ����ǂ���
    int m_nCntTime;                                    // ���Ԃ��J�E���g
    bool m_bUse;                                       // �g�p���邩�ǂ���
};

#endif