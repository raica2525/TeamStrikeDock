//===============================================
//
// AI���� (ai.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "ai.h"
#include "game.h"
#include "ball.h"
#include "library.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CAi::CAi()
{
    m_buttonStateOld.bButtonA = false;
    m_buttonStateOld.bButtonX = false;
    m_buttonStateOld.bButtonB = false;
    m_buttonStateOld.bButtonY = false;
    m_pPlayer = NULL;

    m_core = CORE_THINKING;
    m_nCntActionTime = 0;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CAi::~CAi()
{
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CAi::Update(void)
{
    // �ϐ��錾
    bool bCurrentButtonA = false;         // ���݂�A�{�^��
    bool bCurrentButtonX = false;         // ���݂�X�{�^��
    bool bCurrentButtonB = false;         // ���݂�B�{�^��
    bool bCurrentButtonY = false;         // ���݂�Y�{�^��

    // �R�A���Ƃɏꍇ����
    switch (m_core)
    {
    case CORE_THINKING:
        Thinking();
        DontMove(false);
        break;
    case CORE_ASSAULT:
        AnotherAction();
        RushToBall();
        bCurrentButtonA = JumpBecauseBallUp();
        bCurrentButtonX = DecideAttack(false);
        break;
    case CORE_AVOID:
        AnotherAction();
        RunAwayFromBall();
        bCurrentButtonA = JumpBecauseBallMoveDown();
        bCurrentButtonX = DecideAttack(true);
        break;
    case CORE_WAIT:
        AnotherAction();
        DontMove(true);
        bCurrentButtonX = DecideAttack(true);
        break;
    }

    //==============================================================
    // �R���g���[���̑���ɒu��������
    // A�{�^���g���K�[
    if (bCurrentButtonA && !m_buttonStateOld.bButtonA)
    {
        m_pPlayer->GetControlInput()->bTriggerA = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerA = false;
    }

    // A�{�^���v���X
    if (bCurrentButtonA && m_buttonStateOld.bButtonA)
    {
        m_pPlayer->GetControlInput()->bPressA = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bPressA = false;
    }

    // X�{�^���g���K�[
    if (bCurrentButtonX && !m_buttonStateOld.bButtonX)
    {
        m_pPlayer->GetControlInput()->bTriggerX = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerX = false;
    }

    // X�{�^���v���X
    if (bCurrentButtonX && m_buttonStateOld.bButtonX)
    {
        m_pPlayer->GetControlInput()->bPressX = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bPressX = false;
    }

    // X�{�^�������[�X
    if (!bCurrentButtonX && m_buttonStateOld.bButtonX)
    {
        m_pPlayer->GetControlInput()->bReleaseX = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bReleaseX = false;
    }

    // B�{�^���g���K�[
    if (bCurrentButtonB && !m_buttonStateOld.bButtonB)
    {
        m_pPlayer->GetControlInput()->bTriggerB = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerB = false;
    }

    // Y�{�^���g���K�[
    if (bCurrentButtonY && !m_buttonStateOld.bButtonY)
    {
        m_pPlayer->GetControlInput()->bTriggerY = true;
    }
    else
    {
        m_pPlayer->GetControlInput()->bTriggerY = false;
    }
    //==============================================================

    // 1F�O�̏��Ƃ��ċL�����Ă���
    m_buttonStateOld.bButtonA = bCurrentButtonA;
    m_buttonStateOld.bButtonX = bCurrentButtonX;
    m_buttonStateOld.bButtonB = bCurrentButtonB;
    m_buttonStateOld.bButtonY = bCurrentButtonY;
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CAi * CAi::Create(CPlayer *pPlayer)
{
    // �C���X�^���X�̐���
    CAi *pAI;
    pAI = new CAi;
    
    pAI->m_pPlayer = pPlayer;
    pAI->GetThinkingTime();

    return pAI;
}

//=============================================================================
// �l�����̏���
// Author : �㓡�T�V��
//=============================================================================
void CAi::Thinking(void)
{
    // ���Ԃ𐔂���
    m_nCntActionTime--;

    // ���J�E���^�ŁA���̃R�A�����߂�
    if (m_nCntActionTime < 0)
    {
        m_nCntActionTime = 0;

        // �����_���ɐ��������߂�
        int nRandNum = GetRandNum(100, 1);

        // ���̍U�����̗p���ꂽ���AAI���x���ɂ���Č��߂�
        if (nRandNum >= 1 && nRandNum <= CORE_PRIORITY_1)
        {
            switch (m_pPlayer->GetAILevel())
            {
            case CPlayer::AI_LEVEL_1:
                GetAvoidTime();
                break;
            case CPlayer::AI_LEVEL_2:
                GetWaitTime();
                break;
            case CPlayer::AI_LEVEL_3:
                // ���݂̃v���C���[�l���ɂ���ďꍇ����
                if (CGame::GetNumCurrentDispPlayer() > 2)
                {
                    GetAvoidTime();
                }
                else
                {
                    GetAssaultTime();
                }
                break;
            }
        }
        else if (nRandNum > CORE_PRIORITY_1 && nRandNum <= CORE_PRIORITY_1 + CORE_PRIORITY_2)
        {
            switch (m_pPlayer->GetAILevel())
            {
            case CPlayer::AI_LEVEL_1:
                GetAssaultTime();
                break;
            case CPlayer::AI_LEVEL_2:
                GetAssaultTime();
                break;
            case CPlayer::AI_LEVEL_3:
                GetWaitTime();
                break;
            }
        }
        else if (nRandNum > CORE_PRIORITY_1 + CORE_PRIORITY_2
            && nRandNum <= CORE_PRIORITY_1 + CORE_PRIORITY_2 + CORE_PRIORITY_3)
        {
            switch (m_pPlayer->GetAILevel())
            {
            case CPlayer::AI_LEVEL_1:
                GetWaitTime();
                break;
            case CPlayer::AI_LEVEL_2:
                GetAvoidTime();
                break;
            case CPlayer::AI_LEVEL_3:
                // ���݂̃v���C���[�l���ɂ���ďꍇ����
                if (CGame::GetNumCurrentDispPlayer() > 2)
                {
                    GetAssaultTime();
                }
                else
                {
                    GetAvoidTime();
                }
                break;
            }
        }
    }
}

//=============================================================================
// �l����ȊO�̍s����
// Author : �㓡�T�V��
//=============================================================================
void CAi::AnotherAction(void)
{
    // ���Ԃ𐔂���
    m_nCntActionTime--;

    // ���J�E���^�ŁA���l����
    if (m_nCntActionTime < 0)
    {
        GetThinkingTime();
    }
}

//=============================================================================
// �l�����Ԃ𓾂鏈��
// Author : �㓡�T�V��
//=============================================================================
void CAi::GetThinkingTime(void)
{
    // �R�A���l������
    m_core = CORE_THINKING;

    // ���x���ɉ����čl�����Ԃ𓾂�
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(THINKING_TIME_AI_LEVEL_1_MAX, THINKING_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(THINKING_TIME_AI_LEVEL_2_MAX, THINKING_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(THINKING_TIME_AI_LEVEL_3_MAX, THINKING_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// ���P���Ԃ𓾂鏈��
// Author : �㓡�T�V��
//=============================================================================
void CAi::GetAssaultTime(void)
{
    // �R�A�����P��
    m_core = CORE_ASSAULT;

    // ���x���ɉ����čl�����Ԃ𓾂�
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(ASSAULT_TIME_AI_LEVEL_1_MAX, ASSAULT_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(ASSAULT_TIME_AI_LEVEL_2_MAX, ASSAULT_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(ASSAULT_TIME_AI_LEVEL_3_MAX, ASSAULT_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// �����鎞�Ԃ𓾂鏈��
// Author : �㓡�T�V��
//=============================================================================
void CAi::GetAvoidTime(void)
{
    // �R�A�������
    m_core = CORE_AVOID;

    // ���x���ɉ����čl�����Ԃ𓾂�
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(AVOID_TIME_AI_LEVEL_1_MAX, AVOID_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(AVOID_TIME_AI_LEVEL_2_MAX, AVOID_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(AVOID_TIME_AI_LEVEL_3_MAX, AVOID_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// �҂��Ԃ𓾂鏈��
// Author : �㓡�T�V��
//=============================================================================
void CAi::GetWaitTime(void)
{
    // �R�A��ҋ@����
    m_core = CORE_WAIT;

    // ���x���ɉ����čl�����Ԃ𓾂�
    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        m_nCntActionTime = GetRandNum(WAIT_TIME_AI_LEVEL_1_MAX, WAIT_TIME_AI_LEVEL_1_MIN);
        break;
    case CPlayer::AI_LEVEL_2:
        m_nCntActionTime = GetRandNum(WAIT_TIME_AI_LEVEL_2_MAX, WAIT_TIME_AI_LEVEL_2_MIN);
        break;
    case CPlayer::AI_LEVEL_3:
        m_nCntActionTime = GetRandNum(WAIT_TIME_AI_LEVEL_3_MAX, WAIT_TIME_AI_LEVEL_3_MIN);
        break;
    }
}

//=============================================================================
// �ړ����Ȃ�����
// Author : �㓡�T�V��
//=============================================================================
void CAi::DontMove(bool bUseTurn)
{
    m_pPlayer->GetControlInput()->bTiltedLeftStick = false;

    // �ړ����Ȃ����x�Ƀ{�[���̂ق���U������Ȃ�
    if (bUseTurn)
    {
        // �{�[�����擾
        CBall* pBall = CGame::GetBall();

        // �{�[���̂ق��������Ă��Ȃ��Ȃ炻���������
        if (pBall->GetPos().x > m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_LEFT ||
            pBall->GetPos().x < m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_RIGHT)
        {
            float fAngle = GetAngleToTarget3D2D(m_pPlayer->GetPos(), pBall->GetPos());

            m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
            m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;
        }
    }
}

//=============================================================================
// �{�[���ɋl�ߊ�鏈��
// Author : �㓡�T�V��
//=============================================================================
void CAi::RushToBall(void)
{
    // �{�[�����擾
    CBall* pBall = CGame::GetBall();
    float fAngle = GetAngleToTarget3D2D(m_pPlayer->GetPos(), pBall->GetPos());

    m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
    m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;
}

//=============================================================================
// �{�[�����瓦���鏈��
// Author : �㓡�T�V��
//=============================================================================
void CAi::RunAwayFromBall(void)
{
    // �{�[�����擾
    CBall* pBall = CGame::GetBall();
    float fAngle = GetAngleToTarget3D2D(pBall->GetPos(), m_pPlayer->GetPos());

    // �E�����Ɋp�x����
    if (fAngle > D3DXToRadian(0.0f) && fAngle <= D3DXToRadian(180.0f))
    {
        fAngle = D3DXToRadian(90.0f);
    }
    else
    {
        fAngle = D3DXToRadian(-90.0f);
    }

    m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
    m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;

    // �X�e�[�W�̉����̖񔼕��܂ŗ��ꂽ��A�����I�ɍl����i���x��3�͋��P�ɂ���j
    float fDistance = GetDistance2D(m_pPlayer->GetPos(), CGame::GetBall()->GetPos());
    if (fDistance >= CGame::GetMapLimit().fWidth - BALL_COLLISION_SIZE_SIDE)
    {
        if (m_pPlayer->GetAILevel() == CPlayer::AI_LEVEL_3)
        {
            GetAssaultTime();
        }
        else
        {
            GetThinkingTime();
        }
    }
}

//=============================================================================
// �{�[������ɂ��邩��W�����v���鏈��
// Author : �㓡�T�V��
//=============================================================================
bool CAi::JumpBecauseBallUp(void)
{
    // �W�����v���邩�ǂ���
    bool bUseJump = false;

    // �{�[�����擾
    CBall* pBall = CGame::GetBall();

    // �{�[���̂ق�����ɂ���Ȃ�
    if (pBall->GetPos().y > m_pPlayer->GetPos().y + m_pPlayer->GetCollisionSizeDeffence().y)
    {
        bUseJump = true;
    }

    // �n��ɂ��āA�W�����v�{�^���𒷉������悤�Ƃ��Ă���Ȃ�A����������
    if (m_pPlayer->GetGround() && m_buttonStateOld.bButtonA)
    {
        return false;
    }

    // �󒆂ɂ��āA�؋󂪏I�������Ȃ�A����������
    if (m_pPlayer->GetPressJumpTime() > PLAYER_JUMP_KEEP_FRAME)
    {
        return false;
    }

    return bUseJump;
}

//=============================================================================
// �{�[�������Ɉړ����Ă��Ă��邩��W�����v���鏈��
// Author : �㓡�T�V��
//=============================================================================
bool CAi::JumpBecauseBallMoveDown(void)
{   
    // �W�����v���邩�ǂ���
    bool bUseJump = false;

    // �{�[�����擾
    CBall* pBall = CGame::GetBall();

    // �{�[�������ɓ����Ă邩��
    if (pBall->GetPosOld().y > pBall->GetPos().y)
    {
        bUseJump = true;
    }

    // �n��ɂ��āA�W�����v�{�^���𒷉������悤�Ƃ��Ă���Ȃ�A����������
    if (m_pPlayer->GetGround() && m_buttonStateOld.bButtonA)
    {
        return false;
    }

    // �󒆂ɂ��āA�؋󂪏I�������Ȃ�A����������
    if (m_pPlayer->GetPressJumpTime() > PLAYER_JUMP_KEEP_FRAME)
    {
        return false;
    }

    return bUseJump;
}

//=============================================================================
// ���߂���U�������邩�ǂ����̏���
// Author : �㓡�T�V��
//=============================================================================
bool CAi::DecideAttack(bool bUseTurn)
{
    // ��{�I�ȍU�������邩�ǂ���
    bool bUseDecideAttack = false;

    // �{�[�����擾
    CBall* pBall = CGame::GetBall();

    // �{�[���������蔻����g���Ă��Ȃ��Ȃ�A�֐��𔲂���
    if (!pBall->GetUseCollision())
    {
        return false;
    }

    // �{�[���̂ق��������Ă��Ȃ��Ȃ�A�֐��𔲂���i�U������őłĂ�U�����g�p���Ȃ��̂ŁA�኱�キ�Ȃ�j
    if (!bUseTurn)
    {
        if (pBall->GetPos().x > m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_LEFT ||
            pBall->GetPos().x < m_pPlayer->GetPos().x && m_pPlayer->GetRot().y == PLAYER_ROT_RIGHT)
        {
            return false;
        }
    }

    // �v���C���[�̒��S���W
    D3DXVECTOR3 playerCenterPos = m_pPlayer->GetPos() + D3DXVECTOR3(0.0f, m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);

    // �U��������Ɣ��f���鋗��
    float fAttackDistance = (m_pPlayer->GetCollisionSizeDeffence().x + m_pPlayer->GetCollisionSizeDeffence().y) * 0.5f + (BALL_COLLISION_SIZE_SIDE);
    if (GetDistance2D(playerCenterPos, pBall->GetPos()) <= fAttackDistance)
    {
        bUseDecideAttack = true;

        // �`���[�W���Ƀ{�[�����߂Â����Ȃ�A�����ɗ���
        if (m_pPlayer->GetSwingCharge() > 0)
        {
            bUseDecideAttack = false;
        }

        // �w�������Ă���Ȃ�{�[���̕�������
        if (bUseTurn)
        {
            RushToBall();

            // �ł��n�߂ŋ߂��̃v���C���[�ɍ��E����Ȃ����߁A�֐��𔲂���
            if (m_pPlayer->GetStopTime() == 0)
            {
                return true;
            }
        }

        // ��΂��Ƃ��A�߂��̃v���C���[�̂ق��ɃX�e�B�b�N��|��
        if (m_pPlayer->GetStopTime() == 1 || m_pPlayer->GetStopTime() == 0)
        {
            GetAttackAngle();
        }
    }

    return bUseDecideAttack;
}

//=============================================================================
// �U�����ɂǂ̕����ɃX�e�B�b�N��|����|����
// Author : �㓡�T�V��
//=============================================================================
void CAi::GetAttackAngle(void)
{
    // �����_���ɐ��������߂�
    int nRandNum = GetRandNum(100, 1);
    int nBorderNum = 0;

    switch (m_pPlayer->GetAILevel())
    {
    case CPlayer::AI_LEVEL_1:
        nBorderNum = ATTACK_TARGET_AI_LEVEL_1;
        break;
    case CPlayer::AI_LEVEL_2:
        nBorderNum = ATTACK_TARGET_AI_LEVEL_2;
        break;
    case CPlayer::AI_LEVEL_3:
        nBorderNum = ATTACK_TARGET_AI_LEVEL_3;
        break;
    }

    // ���E�̒l�͈͓̔��Ȃ�A�߂��̓G��_�����p�x�ɓ|��
    if (nRandNum >= 1 && nRandNum <= nBorderNum)
    {
        m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
        m_pPlayer->GetControlInput()->fLeftStickAngle = CGame::GetAngleToClosestPlayer(m_pPlayer->GetPlayable(), m_pPlayer->GetPos());
    }
    else
    {
        // �͈͊O�Ȃ�A�K���ɃX�e�B�b�N��|��
        m_pPlayer->GetControlInput()->fLeftStickAngle = float(rand() % 314) / 100 - float(rand() % 314) / 100;
    }
}
