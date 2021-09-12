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
    m_pPlayer = NULL;

    m_buttonStateOld.bButtonA = false;
    m_buttonStateOld.bButtonX = false;
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
    // �ړ�
    Move();

    // �ϐ��錾
    bool bCurrentButtonA = Jump();         // �W�����v
    bool bCurrentButtonX = BaseAttack();   // ��{�I�ȍU�������邩�ǂ���

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
    //==============================================================

    // 1F�O�̏��Ƃ��ċL�����Ă���
    m_buttonStateOld.bButtonA = bCurrentButtonA;
    m_buttonStateOld.bButtonX = bCurrentButtonX;
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

    return pAI;
}

//=============================================================================
// �ړ�����
// Author : �㓡�T�V��
//=============================================================================
void CAi::Move(void)
{
    // �{�[�����擾
    CBall* pBall = CGame::GetBall();
    float fAngle = GetAngleToTarget3D2D(m_pPlayer->GetPos(), pBall->GetPos());

    m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
    m_pPlayer->GetControlInput()->fLeftStickAngle = fAngle;
}

//=============================================================================
// �W�����v����
// Author : �㓡�T�V��
//=============================================================================
bool CAi::Jump(void)
{
    // �W�����v���邩�ǂ���
    bool bUseJump = false;

    // �{�[�����擾
    CBall* pBall = CGame::GetBall();

    // �{�[���̂ق�����ɂ���Ȃ�i���j
    if (pBall->GetPos().y > m_pPlayer->GetPos().y + m_pPlayer->GetActiveCollisionSize().y)
    {
        bUseJump = true;
    }

    return bUseJump;
}

//=============================================================================
// ��{�I�ȍU�������邩�ǂ����̏���
// Author : �㓡�T�V��
//=============================================================================
bool CAi::BaseAttack(void)
{
    // ��{�I�ȍU�������邩�ǂ���
    bool bUseBaseAttack = false;

    // �{�[�����擾
    CBall* pBall = CGame::GetBall();

    // �v���C���[�̒��S���W
    D3DXVECTOR3 playerCenterPos = m_pPlayer->GetPos() + D3DXVECTOR3(0.0f, m_pPlayer->GetActiveCollisionSize().y, 0.0f);

    // �U��������Ɣ��f���鋗���i���j
    float fAttackDistance = (m_pPlayer->GetActiveCollisionSize().x + m_pPlayer->GetActiveCollisionSize().y) / 2;
    if (GetDistance2D(playerCenterPos, pBall->GetPos()) <= fAttackDistance)
    {
        bUseBaseAttack = true;

        // ��΂��Ƃ��A�߂��̃v���C���[�̂ق��ɃX�e�B�b�N��|��
        if (m_pPlayer->GetStopTime() == 1)
        {
            m_pPlayer->GetControlInput()->bTiltedLeftStick = true;
            m_pPlayer->GetControlInput()->fLeftStickAngle = CGame::GetAngleToClosestPlayer(m_pPlayer->GetPlayable(), m_pPlayer->GetPos());
        }
    }

    return bUseBaseAttack;
}