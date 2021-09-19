//====================================================================
//
// AI���� (ai.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _AI_H_
#define _AI_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �l������
#define THINKING_TIME_AI_LEVEL_1_MAX 45
#define THINKING_TIME_AI_LEVEL_1_MIN 30
#define THINKING_TIME_AI_LEVEL_2_MAX 20
#define THINKING_TIME_AI_LEVEL_2_MIN 10
#define THINKING_TIME_AI_LEVEL_3_MAX 5
#define THINKING_TIME_AI_LEVEL_3_MIN 1

// �R�A�̗D�揇��
#define CORE_PRIORITY_1 60
#define CORE_PRIORITY_2 30
#define CORE_PRIORITY_3 10

// ���P����
#define ASSAULT_TIME_AI_LEVEL_1_MAX 60
#define ASSAULT_TIME_AI_LEVEL_1_MIN 30
#define ASSAULT_TIME_AI_LEVEL_2_MAX 150
#define ASSAULT_TIME_AI_LEVEL_2_MIN 90
#define ASSAULT_TIME_AI_LEVEL_3_MAX 180
#define ASSAULT_TIME_AI_LEVEL_3_MIN 120

// �����鎞��
#define AVOID_TIME_AI_LEVEL_1_MAX 30
#define AVOID_TIME_AI_LEVEL_1_MIN 15
#define AVOID_TIME_AI_LEVEL_2_MAX 40
#define AVOID_TIME_AI_LEVEL_2_MIN 25
#define AVOID_TIME_AI_LEVEL_3_MAX 35
#define AVOID_TIME_AI_LEVEL_3_MIN 20

// �҂���
#define WAIT_TIME_AI_LEVEL_1_MAX 30
#define WAIT_TIME_AI_LEVEL_1_MIN 15
#define WAIT_TIME_AI_LEVEL_2_MAX 40
#define WAIT_TIME_AI_LEVEL_2_MIN 25
#define WAIT_TIME_AI_LEVEL_3_MAX 35
#define WAIT_TIME_AI_LEVEL_3_MIN 20

// �U�����ɑ_�����U�����ł��邩�ǂ����i100���̉��j
#define ATTACK_TARGET_AI_LEVEL_1 30
#define ATTACK_TARGET_AI_LEVEL_2 75
#define ATTACK_TARGET_AI_LEVEL_3 95

//================================================
// �N���X�錾
//================================================

// AI�N���X
class CAi
{
public:
    CAi();
    ~CAi();

    // �{�^���̓��͏��
    typedef struct
    {
        bool bButtonA;
        bool bButtonX;
        bool bButtonB;
        bool bButtonY;
    }ButtonState;

    // �S�����Ă��邱��
    typedef enum
    {
        CORE_THINKING = 0, // �l����
        CORE_ASSAULT,      // ���P�i�K���U�߁j
        CORE_AVOID,        // ������i�K�������j
        CORE_WAIT,         // �҂i�K���҂��j
        CORE_MAX
    }CORE;

    void Update(void);						    // �X�V����
    static CAi * Create(CPlayer *pPlayer);		// �C���X�^���X��������
    void Thinking(void);                        // �l����
    void AnotherAction(void);                   // �l����ȊO�̍s����
    void GetThinkingTime(void);                 // �l�����Ԃ𓾂�
    void GetAssaultTime(void);                  // ���P���Ԃ𓾂�
    void GetAvoidTime(void);                    // �����鎞�Ԃ𓾂�
    void GetWaitTime(void);                     // �҂��Ԃ𓾂�
    void DontMove(bool bUseTurn);               // �ړ����Ȃ�
    bool IsFacingBall(void);                    // �{�[���̕��������Ă��邩
    void RushToBall(void);                      // �{�[���ɋl�ߊ��
    void RunAwayFromBall(void);                 // �{�[�����瓦����
    bool JumpBecauseBallUp(void);               // �{�[������ɂ��邩��W�����v����
    bool JumpBecauseBallMoveDown(void);         // �{�[�����������Ă��Ă��邩��W�����v����
    bool DecideAttack(bool bUseTurn);           // ���߂���U�������邩�ǂ���
    void GetAttackAngle(void);                  // �U�����ɂǂ̕����ɃX�e�B�b�N��|����

private:
    CPlayer *m_pPlayer;
    ButtonState m_buttonStateOld;   // 1F�O�̃{�^�����͏��

    CORE m_core;                    // ���݉���S�����Ă��邩
    int m_nCntActionTime;           // �s���̃J�E���^
};

#endif