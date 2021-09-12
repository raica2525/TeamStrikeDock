//======================================================================================
//
// �v���C���[���� (player.cpp)
// Author : �㓡�T�V��
//
//======================================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "library.h"
#include "camera.h"
#include "character.h"
#include "debug.h"
#include "animation.h"
#include "game.h"
#include "sound.h"
#include "ball.h"
#include "modelData.h"
#include "ai.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CPlayer::CPlayer() :CCharacter(OBJTYPE::OBJTYPE_PLAYER)
{
    m_controlInput.bTiltedLeftStick = false;
    m_controlInput.fLeftStickAngle = 0.0f;
    m_controlInput.bTriggerA = false;
    m_controlInput.bPressA = false;
    m_controlInput.bTriggerX = false;
    m_controlInput.bPressX = false;
    m_controlInput.bReleaseX = false;
    m_controlInput.bTriggerY = false;
    m_controlInput.bTriggerB = false;
    m_AIlevel = AI_LEVEL_NONE;
    m_pAI = NULL;

    m_playable = PLAYABLE_001;
    m_nPoint = 0;
    m_move = DEFAULT_VECTOR;
    m_exFlag = EX_FLAG_NONE;
    m_attackState = ATTACK_STATE_NONE;

    m_nCntAttackTime = 0;
    m_bGround = true;

    m_bDisp = true;
    m_nCntTakeDamageTime = 0;
    m_fLife = 0.0f;
    m_bUsedLuckyGuard = false;
    m_damageState = DAMAGE_STATE_NONE;

    m_nCntLandingTime = 0;
    m_bGroundOld = true;

    m_fAtk = 0.0f;
    m_fDef = 0.0f;
    m_fSpd = 0.0f;
    m_fWei = 0.0f;
    m_nCharSize = CHAR_SIZE_MEDIUM;
    memset(m_afParam, 0, sizeof(m_afParam));
    m_nModelPosDefUp = 0;
    m_nModelPosDefDown = 0;

    m_nCntTurnTime = 0;
    m_nCntPressJump = 0;
    m_bDiveCheck = false;
    m_bDiving = false;
    m_bUsedSecondJump = false;
    m_bSquat = false;
    m_nCntStartGlide = 0;

    m_nCntSwingCharge = 0;
    m_bUseAvoidMultipleHits = false;
    m_bHitAttackThisFrame = false;
    m_nCntStopTime = 0;
    m_nCntAttackAnimTime = 0;

    m_spShot = SP_SHOT_HEAL;
    m_fSpGaugeCurrent = 0.0f;
    m_fSpGaugeMax = 0.0f;

    m_bUsedThreeJump = false;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �p�[�c����ݒ�
    CCharacter::SetPartNum(PARTS_MAX);

    // �v���C�A�u���L�����̐ݒ��ǂݍ���
    LoadCustom();

    // ���W�E�e�q�֌W�̓ǂݍ���
    //CCharacter::LoadModelData("./data/ANIMATION/motion_player.txt");  // �J�X�^�}�C�Y���Ƃ̏����ʒu�ɑΉ�
    CCharacter::SetAnimFilePass("./data/ANIMATION/motion_player.txt");
    CCharacter::LoadModelData(m_nModelPosDefUp, m_nModelPosDefDown);

    CCharacter::Init(pos, size);

    return S_OK;
}

//=============================================================================
// �J�X�^�}�C�Y�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::LoadCustom(void)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �p�[�c���X�g������擾
    CModelData *pModelData = CManager::GetModelData();

    // �ϐ��錾
    int nPartsListType = 0;                // �p�[�c���X�g���̎��
    int nExNumber = 0;                     // ����\�͔ԍ�

    // ��`
    const int PARTS_LIST_HEAD = 0;         // �p�[�c���X�g_��
    const int PARTS_LIST_UP = 1;           // �p�[�c���X�g_�㔼�g
    const int PARTS_LIST_DOWN = 2;         // �p�[�c���X�g_�����g
    const int PARTS_LIST_WEP = 3;          // �p�[�c���X�g_����

    // ���f�����郁���o�ϐ�����񏉊���
    m_fAtk = 0.0f;
    m_fDef = 0.0f;
    m_fSpd = 0.0f;
    m_fWei = 0.0f;
    m_exFlag = EX_FLAG_NONE;
    m_nCharSize = CHAR_SIZE_SMALL;
    memset(m_afParam, 0, sizeof(m_afParam));

    // �J�X�^�}�C�Y�f�[�^�̃t�@�C�����J��
    switch (m_playable)
    {
    case CPlayer::PLAYABLE_001:
        pFile = fopen("data/TXT/custom1.txt", "r");
        break;
    case CPlayer::PLAYABLE_002:
        pFile = fopen("data/TXT/custom2.txt", "r");
        break;
    case CPlayer::PLAYABLE_003:
        pFile = fopen("data/TXT/custom3.txt", "r");
        break;
    case CPlayer::PLAYABLE_004:
        pFile = fopen("data/TXT/custom4.txt", "r");
        break;
    }

    // �J������
    if (pFile != NULL)
    {
        for (int nCntPartsList = 0; nCntPartsList < MAX_PARTS; nCntPartsList++)
        {
            // �ǂݍ���
            fscanf(pFile, "%d", &nPartsListType);

            // ��{4�\��
            m_fAtk += pModelData->GetPartsList(nPartsListType)->fAtk * pModelData->GetPartsRate(nCntPartsList)->fAtkRate;
            m_fDef += pModelData->GetPartsList(nPartsListType)->fDef * pModelData->GetPartsRate(nCntPartsList)->fDefRate;
            m_fSpd += pModelData->GetPartsList(nPartsListType)->fSpd * pModelData->GetPartsRate(nCntPartsList)->fSpdRate;
            m_fWei += pModelData->GetPartsList(nPartsListType)->fWei * pModelData->GetPartsRate(nCntPartsList)->fWeiRate;

            // ����\��
            nExNumber = pModelData->GetPartsList(nPartsListType)->nEx;
            BITON(m_exFlag, 0x001<<nExNumber);

            // �e�p�[�c���X�g����A�ו��̃p�[�c������
            if (nCntPartsList == PARTS_LIST_HEAD)
            {
                BindParts(PARTS_HEAD, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);

                m_afParam[PARAM_SWING_UP] = pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_afParam[PARAM_SWING_DOWN] = pModelData->GetPartsList(nPartsListType)->afParam[2];
                m_afParam[PARAM_SMASH] = pModelData->GetPartsList(nPartsListType)->afParam[3];
                m_afParam[PARAM_SPIKE_RIGHT] = pModelData->GetPartsList(nPartsListType)->afParam[4];
                m_afParam[PARAM_SPIKE_LEFT] = pModelData->GetPartsList(nPartsListType)->afParam[5];
            }
            else if (nCntPartsList == PARTS_LIST_UP)
            {
                BindParts(PARTS_BODY, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                BindParts(PARTS_LHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);

                // ��x�P�p
                //m_nCharSize = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];

                // �㔼�g�̏����ʒu�f�[�^
                m_nModelPosDefUp = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];
            }
            else if (nCntPartsList == PARTS_LIST_DOWN)
            {
                BindParts(PARTS_HIP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                BindParts(PARTS_LFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);

                // �����g�̏����ʒu�f�[�^
                m_nModelPosDefDown = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];
            }
            else if (nCntPartsList == PARTS_LIST_WEP)
            {
                BindParts(PARTS_WEP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);

                // �K�E�Z�ƁA���̃Q�[�W��
                m_spShot = (int)pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_fSpGaugeMax = pModelData->GetPartsList(nPartsListType)->afParam[2];
            }
        }
        // �t�@�C�������
        fclose(pFile);
    }

    // �̗͂�����
    m_fLife = m_fDef;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Uninit(void)
{
    // AI�����������J��
    if (m_pAI != NULL)
    {
        // �������J��
        delete m_pAI;
        m_pAI = NULL;
    }

    CCharacter::Uninit();
}

//=============================================================================
// ���͏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Input(void)
{
    // �R���g���[�����擾�i1P�̏ꍇ�AEnter�őJ�ڂ����Ȃ�L�[�{�[�h����ɕς���j
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(m_playable);

    // �{�^�������擾
    m_controlInput.bTriggerA = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_A);
    m_controlInput.bPressA = pInputJoypad->GetJoypadPress(m_playable, CInputJoypad::BUTTON_A);
    m_controlInput.bTriggerX = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_X);
    m_controlInput.bPressX = pInputJoypad->GetJoypadPress(m_playable, CInputJoypad::BUTTON_X);
    m_controlInput.bReleaseX = pInputJoypad->GetJoypadRelease(m_playable, CInputJoypad::BUTTON_X);
    m_controlInput.bTriggerY = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_Y);
    m_controlInput.bTriggerB = pInputJoypad->GetJoypadTrigger(m_playable, CInputJoypad::BUTTON_B);

    // ���X�e�B�b�N���X���Ă��邩�ǂ���
    if (Controller.lY != 0 || Controller.lX != 0)
    {
        m_controlInput.bTiltedLeftStick = true;

        // �p�x�����߂�
        m_controlInput.fLeftStickAngle = atan2(Controller.lX, Controller.lY*-1);
    }
    else
    {
        m_controlInput.bTiltedLeftStick = false;
    }
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Update(void)
{
    // ���̃t���[�����ɍU���𓖂Ă����ǂ�����������
    m_bHitAttackThisFrame = false;

    // �\�����Ă���Ȃ�
    if (m_bDisp)
    {
        // AI�łȂ��Ȃ�
        if (!m_pAI)
        {
            // ���͏���
            Input();
        }
        else
        {
            // AI�X�V����
            m_pAI->Update();
        }

        // �d�����Ă��Ȃ��Ȃ�
        if(m_nCntStopTime <= 0)
        {
            // �������Ԃ�����Ȃ�A�J�E���^��������
            if (m_nCntTakeDamageTime > 0)
            {
                m_nCntTakeDamageTime--;

                // �����I�����ɏ�Ԃ�߂�
                if (m_nCntTakeDamageTime == 0)
                {
                    // �_�E������̋N���オ��ȊO�́A�_���[�W��Ԃ�����
                    if (m_damageState != DAMAGE_STATE_BIG)
                    {
                        m_damageState = DAMAGE_STATE_NONE;
                    }
                    else
                    {
                        m_damageState = DAMAGE_STATE_STAND_UP;
                        m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_STAND_UP_FRAME;
                    }
                }
            }

            // �U���̃N�[���^�C��
            if (m_nCntAttackTime > 0)
            {
                // �U�����̓J�E���g�_�E��
                m_nCntAttackTime--;

                // �U���I�����Ƀ����o�ϐ���߂�
                if (m_nCntAttackTime == 0)
                {
                    ResetAttack();
                }
            }

            // �A�^�b�N�A�j���[�V�����J�E���^
            if (m_nCntAttackAnimTime > 0)
            {
                // �J�E���g�_�E��
                m_nCntAttackAnimTime--;
            }

            // ���n���Ă���Ȃ�A�F�X�t���O�����Z�b�g
            if (m_bGround)
            {
                ResetOnGround();
            }

            // �s��
            float fSpeed = PLAYER_BASE_MINIMUM_SPEED + (m_fSpd * PLAYER_ADD_BASE_SPEED_RATE); // �Œ�ۏ؂̑��x�ɁA�p�����[�^���𑫂�
            Movement(fSpeed);

            // �A�j���[�V����������
            CCharacter::Update();

            //// ���C�t���Ȃ��Ȃ�����A��\����
            //if (GetLife() <= 0)
            //{
            //    m_bDisp = false;
            //}
        }
        else
        {
            // �d�����Ԃ��J�E���g
            m_nCntStopTime--;

            // �K�E�Z�����i�Q�[�W���^������X�{�^����������j
            if (m_fSpGaugeCurrent >= m_fSpGaugeMax)
            {
                if (m_controlInput.bTriggerX)
                {
                    EmitSpShot();
                }
            }

            // �����Ń{�[�������
            if (m_nCntStopTime == 0)
            {
                // �ʒu���擾
                D3DXVECTOR3 pos = DEFAULT_VECTOR;
                pos = GetPos();

                // �ŏ��̐ڐG�ł͂Ȃ�����false
                AttackUpdate(pos, false);
            }

            // �A�^�b�N�A�j���[�V�����J�E���^�i�ςȃ|�[�Y�Ŏ~�܂�Ȃ����߂̍Œ�ۏ؁j
            if (m_nCntAttackAnimTime > 0)
            {
                // �A�j���[�V�����J�E���g�_�E��
                m_nCntAttackAnimTime--;

                // �U�����Ԃ��J�E���g�_�E��
                m_nCntAttackTime--;

                // �U���I�����Ƀ����o�ϐ���߂��i�O�̂��߁j
                if (m_nCntAttackTime == 0)
                {
                    ResetAttack();
                }

                // �U�����[�V�����ɂ��邩�ǂ���
                AttackMotion();

                // �A�j���[�V�����̍Œ�ۏ�
                CCharacter::Update();
            }
        }
    }
    // �̗͂��Ȃ��Ȃ�
    else
    {
        //// �Q�[�����ʏ�Ȃ�
        //if (CGame::GetState() == CGame::STATE_NORMAL)
        //{
        //    // ���ꂽ���̃G�t�F�N�g
        //    //DeathCrush(GetPos(), GetRot());

        //    // �Q�[���̏�Ԃ��I���ɂ���
        //    CGame::SetState(CGame::STATE_FINISH);
        //}
    }
}

//=============================================================================
// ���n���Ƀ��Z�b�g����ϐ�����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::ResetOnGround(void)
{
    m_nCntPressJump = 0;
    m_bDiveCheck = true;
    m_bDiving = false;
    m_bUsedSecondJump = false;
    m_bUsedThreeJump = false;
    m_nCntStartGlide = 0;
}

//=============================================================================
// �U������Ń��Z�b�g����ϐ�����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::ResetAttack(void)
{
    m_attackState = ATTACK_STATE_NONE;
    m_nCntSwingCharge = 0;
    m_bUseAvoidMultipleHits = false;
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Draw(void)
{
    // �\������Ȃ�A�`��
    if (m_bDisp)
    {
        CCharacter::Draw();
    }
}

//=============================================================================
// �C���X�^���X����
// Author : �㓡�T�V��
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int playable, AI_LEVEL AIlevel)
{
    // �������m��
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // �e���̏���ݒ�
    pPlayer->SetPos(pos);
    pPlayer->SetRot(rot);
    pPlayer->SetTribe(CCharacter::TRIBE_PLAYER);
    pPlayer->SetActiveCollisionSize(PLAYER_COLLISION_SIZE);

    // �ǂݍ��ގ�ނ̐ݒ�
    pPlayer->m_playable = playable;

    // ������
    pPlayer->Init(pos, DEFAULT_SCALE);

    // ���т��郁���o�ϐ��̏�����
    pPlayer->m_AIlevel = AIlevel;
    if (pPlayer->m_AIlevel != AI_LEVEL_NONE)
    {
        pPlayer->m_pAI = CAi::Create(pPlayer);
    }

    // �Q�[���̃v���C���[�l���𑝂₷
    CGame::SetAddNumCurrentPlayer();

    return pPlayer;
}

//=============================================================================
// �s��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Movement(float fSpeed)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    pos = GetPos();

    // 1F�O�̈ʒu���L��
    SetPosOld(pos);

    // ���̈ړ��ʐ���
    if (m_move.x != 0.0f)
    {
        // ����̊������A�󋵂ɂ���ĕς���
        float fControlMoveRate = PLAYER_CONTROL_MOVE_ON_GROUND;

        // �������Ă��Ȃ��Ȃ�
        if (m_damageState == DAMAGE_STATE_NONE)
        {
            // �󒆂Ȃ�
            if (!m_bGround)
            {
                fControlMoveRate = PLAYER_CONTROL_MOVE_IN_AIR;
            }
        }
        else
        {
            fControlMoveRate = PLAYER_CONTROL_MOVE_TAKE_DAMAGE;
        }

        // ���Ⴊ��ł���Ȃ�
        if (m_bSquat)
        {
            fControlMoveRate = PLAYER_CONTROL_MOVE_SQUAT;
        }

        // �ړ��ʐ���
        m_move.x *= fControlMoveRate;
    }

    // ���[�V�������܂��͑ҋ@�ɂ���
    GetAnimation()->SetAnimation(ANIM_IDLE);

    // ���Ⴊ�ݏ�Ԃ����Z�b�g
    m_bSquat = false;

    // �v���C���[�ړ�����
    Control(fSpeed);

    // �ړ��ʂƈʒu�����т���
    pos += m_move;

    // �n�ʂ���Ȃ�
    if (pos.y > 0.0f)
    {
        // �d��
        float fGravity = PLAYER_BASE_MINIMUM_GRAVITY + (m_fWei * PLAYER_ADD_BASE_GRAVITY_RATE);
        if (m_damageState == DAMAGE_STATE_BLOWN)
        {
            // ������΂��ꒆ�͌Œ�l
            fGravity = PLAYER_TAKE_DAMAGE_BLOWN_GRAVITY;
        }
        m_move.y -= fGravity;

        // �n�ʂɂ��Ȃ�
        m_bGround = false;
    }
    else
    {
        // �n�ʂɂ���Ȃ�A�d�͂������Ȃ��ŃW�����v�\�A�}�~���̃`�F�b�N�������A��i�W�����v������
        pos.y = 0.0f;
        m_move.y = 0.0f;
        m_bGround = true;
    }

    // �}�b�v����
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    if (pos.x - GetActiveCollisionSize().x < -mapLimit.fWidth)
    {
        pos.x = -mapLimit.fWidth + GetActiveCollisionSize().x;
    }
    if (pos.x + GetActiveCollisionSize().x > mapLimit.fWidth)
    {
        pos.x = mapLimit.fWidth - GetActiveCollisionSize().x;
    }
    if (pos.y + GetActiveCollisionSize().y > mapLimit.fHeight)
    {
        pos.y = mapLimit.fHeight - GetActiveCollisionSize().y;
    }
    
    // �W�����v
    Jump();

    // �ړ����[�V�����̊Ǘ�
    MoveMotion();

    //====================================================
    // �U������
    // �U�����q�b�g���Ă��Ȃ��Ȃ�A�U���̔��蓙���X�V
    if (!m_bUseAvoidMultipleHits)
    {
        AttackUpdate(pos, true); // bFirestCollision��true�́A�������ŏ��̐ڐG�ɂȂ肤�邩��
    }

    // �U���̐���
    AttackGenerator(pos);

    // �U�����[�V�����ɂ��邩�ǂ���
    AttackMotion();
    //====================================================

    //====================================================
    // �h�䏈��
    // �������̍X�V
    DamageUpdate(pos);

    // ���̃t���[���ɍU�����q�b�g���Ă��Ȃ����A������ԈȊO���A�������Ă��Ȃ�or�N���オ�蒆�ɁA�{�[���������������ǂ���
    if (!m_bHitAttackThisFrame && m_attackState != ATTACK_STATE_THROW)
    {
        if (m_damageState == DAMAGE_STATE_NONE || m_damageState == DAMAGE_STATE_STAND_UP && m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
        {
            CollisionBall(pos);
        }
    }

    // �������[�V�����ɂ��邩�ǂ���
    DamageMotion();
    //====================================================

    // ��]����
    // �n�ʂɂ��āA�������Ă��Ȃ���Ԃ��A�U����Ԃ��Ȃ����`���[�W�Ȃ�
    if (m_bGround)
    {
        if (m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_NONE ||
            m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_SWING_CHARGE)
        {
            RotControl();
        }
    }

    // �ʒu�𔽉f
    SetPos(pos);

    // 1F�O�A�W�����v�ł������ǂ������L�^
    m_bGroundOld = m_bGround;

#ifdef COLLISION_TEST
    if (!m_bHitAttackThisFrame && m_attackState != ATTACK_STATE_THROW)
    {
        if (m_damageState == DAMAGE_STATE_NONE || m_damageState == DAMAGE_STATE_STAND_UP&& m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
        {
            D3DXVECTOR3 size = GetActiveCollisionSize();
            if (m_bSquat)
            {
                size.y *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
            }
            CDebug::Create(pos, size, CDebug::TYPE_MOMENT);
        }
    }
#endif // COLLISION_TEST
}

//=============================================================================
// �ړ��n���[�V�����̊Ǘ�
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::MoveMotion(void)
{
    // ���n���[�V�����ɂ��邩�ǂ���
    if (!m_bGroundOld && m_bGround)
    {
        // ���n���Ԃ�ݒ�
        m_nCntLandingTime = PLAYER_LANDING_TIME;
    }
    if (m_nCntLandingTime > 0)
    {
        // �J�E���g�_�E��
        m_nCntLandingTime--;

        // ���[�V�����𒅒n�ɂ���
        GetAnimation()->SetAnimation(ANIM_LANDING);
    }

    // ���Ⴊ�݃��[�V�����ɂ��邩�ǂ���
    if (m_bSquat)
    {
        GetAnimation()->SetAnimation(ANIM_SQUAT);
    }

    // ����̃J�E���^
    if (m_nCntStartGlide > 0)
    {
        m_nCntStartGlide--;
    }

    // �W�����v���[�V�����ɂ��邩�ǂ���
    if (!m_bGround)
    {
        // ����A�j���[�V�����̃J�E���^��0�ȉ��Ȃ犊��
        if (m_nCntStartGlide <= 0)
        {
            GetAnimation()->SetAnimation(ANIM_GLIDE);
        }
        else
        {
            // �ǂ̃W�����v�ɂ��邩
            if (m_bUsedThreeJump)
            {
                GetAnimation()->SetAnimation(ANIM_THIRD_JUMP);
            }
            else
            {
                if (m_bUsedSecondJump)
                {
                    GetAnimation()->SetAnimation(ANIM_SECOND_JUMP);
                }
                else
                {
                    GetAnimation()->SetAnimation(ANIM_JUMP);
                }
            }
        }
    }
}

//=============================================================================
// �{�[���ɓ����������ǂ���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::CollisionBall(D3DXVECTOR3 playerPos)
{
    // �{�[���Ƃ̓����蔻��
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BALL);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BALL); nCntScene++)
    {
        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // ���̃V�[�����L��
            CScene*pNextScene = pScene->GetNextScene();

            // �{�[���ɃL���X�g
            CBall *pBall = (CBall*)pScene;

            // �����蔻����g�p����Ȃ�
            if (pBall->GetUseCollision())
            {
                // �ʒu���擾
                D3DXVECTOR3 ballPos = pBall->GetPos();

                // �����ȊO�̒N�����������Ȃ�
                if (pBall->GetWhoShooting() != m_playable && pBall->GetWhoShooting() != BALL_NOT_ANYONE)
                {
                    // �����̓����蔻��̑傫��������
                    D3DXVECTOR3 playerSize = GetActiveCollisionSize();

                    // ���Ⴊ��ł���Ȃ�c�̓����蔻������
                    if (m_bSquat)
                    {
                        playerSize.y *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
                    }

                    // �������Ă���Ȃ�
                    if (IsCollisionToRotationRect(playerPos, playerSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&playerPos, &pBall->GetPos(),
                            &playerSize, &BALL_COLLISION_SIZE))
                    {
                        // �_���[�W���v�Z�i���b�L�[�K�[�h�����\������j
                        float fDamage = pBall->GetSpeed() * PLAYER_TAKE_DAMAGE_RATE_FROM_BALL;
                        TakeDamage(fDamage, pBall->GetWhoShooting(), pBall->GetPosOld(), true);
                    }
                }
            }

            // ���̃V�[���ɂ���
            pScene = pNextScene;
        }
        else
        {
            // ���g���Ȃ��Ȃ�A�����ŏ������I����
            break;
        }
    }
}

//=============================================================================
// �_���[�W���󂯂�
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::TakeDamage(float fDamage, int nWho, D3DXVECTOR3 damageOldPos, bool bUseLuckyGuard)
{
    // �U����Ԃ����Z�b�g
    ResetAttack();

    // ���b�L�[�K�[�h���g���U�����󂯂���
    D3DXVECTOR3 playerPos = GetPos();       // �ʒu���擾
    float fKnockbackValue = 0.0f;           // �m�b�N�o�b�N��
    bool bUseLuckyGuardThisFrame = false;   // ���̃t���[�����Ƀ��b�L�[�K�[�h�������������ǂ���
    if (bUseLuckyGuard)
    {
        // ���b�L�[�K�[�h���g���Ă��Ȃ��Ȃ�
        if (!m_bUsedLuckyGuard)
        {
            // �����̌��ʂŁA���b�L�[�K�[�h
            int nRandNum = GetRandNum(PLAYER_LUCKY_GUARD_MAX, 1);
            if (nRandNum <= (int)m_fDef)
            {
                bUseLuckyGuardThisFrame = true;
                m_bUsedLuckyGuard = true;
                fDamage = 1.0f;                                         // 1�_���[�W�͎󂯂�
                fKnockbackValue = PLAYER_KNOCK_BACK_LUCKY_GUARD;        // ��p�̃m�b�N�o�b�N��
                m_nCntStopTime = CGame::GetBall()->Launch(playerPos + D3DXVECTOR3(0.0f, GetActiveCollisionSize().y / 2, 0.0f),
                    BALL_SHOOT_STOP_LUCKY_GUARD_FRAME);   // �{�[����ł��グ��
            }
        }
    }

    // �_���[�W��{�[���Ƃ̈ʒu�֌W�ɂ���āA�ǂ̕�����Ԃ���ݒ�
    D3DXVECTOR3 playerRot = GetRot();   // �������擾
    float fJudgmentDamage = fDamage;    // �ŏI�I�ɂ��ꃂ�[�V�������ǂ��Ȃ邩�̔��f�ɗp����_���[�W

    // �w�������Ȃ�A����Ɏg���_���[�W��2�{
    if (playerPos.x < damageOldPos.x)
    {
        if (playerRot.y == PLAYER_ROT_LEFT)
        {
            fJudgmentDamage *= 2.0f;
        }
    }
    else
    {
        if (playerRot.y == PLAYER_ROT_RIGHT)
        {
            fJudgmentDamage *= 2.0f;
        }
    }

    // ������Ԃ̔���
    if (fJudgmentDamage < PLAYER_TAKE_DAMAGE_BORDER_DAMAGE)
    {
        m_damageState = DAMAGE_STATE_SMALL;

        // ���̃t���[�����Ƀ��b�L�[�K�[�h�ȊO��
        if (!bUseLuckyGuardThisFrame)
        {
            // �����ꎞ��
            m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_SMALL_FRAME;

            // �󒆂ł͏����̂�����
            if (!m_bGround)
            {
                fKnockbackValue = PLAYER_KNOCK_BACK_SMALL_IN_AIR;
            }
        }
        else
        {
            // ���b�L�[�K�[�h�Ȃ�A������������
            m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_LUCKY_GUARD_FRAME;
        }
    }
    else
    {
        // ������΂����Ԃ́A�ړ��ʂ��Ȃ��Ȃ��Ēn�ʂɂ��܂ő���
        m_damageState = DAMAGE_STATE_BLOWN;
        fKnockbackValue = PLAYER_KNOCK_BACK_BIG;
    }

    // �U���̕��������A�m�b�N�o�b�N������
    if (playerPos.x < damageOldPos.x)
    {
        SetRot(D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f));
        m_move.x = -fKnockbackValue;
    }
    else
    {
        SetRot(D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f));
        m_move.x = fKnockbackValue;
    }

    // �_���[�W���󂯂�
    m_fLife -= fDamage;

    // �̗͂��Ȃ��Ȃ�����
    if (m_fLife <= 0.0f)
    {
        // ��\����
        m_fLife = 0.0f;
        m_bDisp = false;

        // �{�[���ɂ��ꂽ�Ȃ�A�{�[�������������~�߂�i�{�[���ł������b�L�[�K�[�h�͔������Ȃ����ߑ�p�j
        if (bUseLuckyGuard)
        {
            CGame::GetBall()->SetStopTime(BALL_SHOOT_STOP_DEATH_FRAME);
        }

        // �Ō�ɍU�������l�Ƀ|�C���g������
        if (nWho >= PLAYABLE_001 && nWho < CGame::GetNumCurrentPlayer())
        {
            CGame::GetPlayer(nWho)->SetAddPoint();
        }
    }
}

//=============================================================================
// �v���C���[�̈ړ�����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Control(float fSpeed)
{
    // ����
    D3DXVECTOR3 rot = GetRot();

    // �n��ōU�����ȊO�Ȃ�i��ɒn��X�C���O�ł͓����Ȃ��j
    if (!m_bGround || m_attackState == ATTACK_STATE_NONE)
    {
        // �U��������Ԃ𐔂���
        if (m_nCntTurnTime > PLAYER_TURN_FRAME)
        {
            m_nCntTurnTime = 0;
        }

        // �������Ă��Ȃ���Ԃ��A���n���ł��Ȃ��Ȃ�
        if (m_damageState == DAMAGE_STATE_NONE && m_nCntLandingTime <= 0)
        {
            // �X�e�B�b�N���|��Ă���Ȃ�ړ�
            if (m_controlInput.bTiltedLeftStick)
            {
                // �E�ړ�
                if (STICK_RIGHT(m_controlInput.fLeftStickAngle))
                {
                    if (m_nCntTurnTime > 0)
                    {
                        m_nCntTurnTime++;

                        // �����U��������ɂ܂��U���������A�J�E���g��1����
                        if (rot.y == PLAYER_ROT_LEFT)
                        {
                            m_nCntTurnTime = 1;
                        }
                    }
                    else
                    {
                        if (m_bGround)
                        {
                            // �����Ă�������ɐi��
                            if (rot.y == PLAYER_ROT_RIGHT)
                            {
                                m_move.x = fSpeed;

                                // ���[�V����������ɂ���
                                GetAnimation()->SetAnimation(ANIM_MOVE);
                            }
                            else
                            {
                                // �����]���J�n
                                m_nCntTurnTime++;
                            }
                        }
                        else
                        {
                            // �󒆈ړ�
                            m_move.x += fSpeed / PLAYER_TURN_FRAME;
                        }
                    }
                }
                else if (STICK_LEFT(m_controlInput.fLeftStickAngle))
                {
                    if (m_nCntTurnTime > 0)
                    {
                        m_nCntTurnTime++;

                        // �����U��������ɂ܂��U���������A�J�E���g��1����
                        if (rot.y == PLAYER_ROT_RIGHT)
                        {
                            m_nCntTurnTime = 1;
                        }
                    }
                    else
                    {
                        if (m_bGround)
                        {
                            // �����Ă�������ɐi��
                            if (rot.y == PLAYER_ROT_LEFT)
                            {
                                m_move.x = -fSpeed;

                                // ���[�V����������ɂ���
                                GetAnimation()->SetAnimation(ANIM_MOVE);
                            }
                            else
                            {
                                // �����]���J�n
                                m_nCntTurnTime++;
                            }
                        }
                        else
                        {
                            // �󒆈ړ�
                            m_move.x -= fSpeed / PLAYER_TURN_FRAME;
                        }
                    }
                }
                else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
                {
                    // �n��Ȃ�A���Ⴊ��
                    if (m_bGround)
                    {
                        m_bSquat = true;
                    }
                }
            }

            // �ő呬�x�𒴂��Ȃ��悤�ɂ���
            if (m_move.x > fSpeed)
            {
                m_move.x = fSpeed;
            }
            else if (m_move.x < -fSpeed)
            {
                m_move.x = -fSpeed;
            }
        }
    }
}

//=============================================================================
// �W�����v�̏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Jump(void)
{
    // �������ȊO��
    if (m_attackState != ATTACK_STATE_THROW)
    {
        // A�v���X
        if (m_controlInput.bPressA)
        {
            // �������Ă��Ȃ���ԂȂ�i�U�����ł������͐L�΂���j
            if (m_damageState == DAMAGE_STATE_NONE)
            {
                // �W�����v�̃J�E���^��0���傫���A���Z�J�E���^���Ȃ�
                if (m_nCntPressJump > 0 && m_nCntPressJump <= PLAYER_JUMP_ADD_FRAME)
                {
                    // �W�����v�̃J�E���^���Z
                    m_nCntPressJump++;

                    // �W�����v�ʉ��Z
                    m_move.y += PLAYER_BASE_MINIMUM_GRAVITY + (m_fWei * PLAYER_ADD_BASE_GRAVITY_RATE);
                }
                else if(m_nCntPressJump > PLAYER_JUMP_ADD_FRAME && m_nCntPressJump <= PLAYER_JUMP_KEEP_FRAME)
                {
                    // �؋�
                    if (m_move.y <= 0.0f)
                    {
                        // �W�����v�̃J�E���^���Z
                        m_nCntPressJump++;

                        m_move.y = 0.0f;
                    }
                }
            }
        }
        else
        {
            // �W�����v�̃J�E���^�����Z�b�g
            m_nCntPressJump = 0;
        }

        // A�g���K�[�i�W�����v�����̃C���[�W�j
        if (m_controlInput.bTriggerA)
        {
            // �������Ă��Ȃ���Ԃ��A�U����Ԃ��Ȃ����`���[�W�Ȃ�
            if (m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_NONE ||
                m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_SWING_CHARGE)
            {
                // �n�ʂɂ���Ȃ�
                if (m_bGround)
                {
                    // �W�����v�̏�����
                    m_move.y = PLAYER_JUMP_FIRST_RATE;

                    // ����܂ł̃J�E���^��ݒ�
                    m_nCntStartGlide = PLAYER_FIRST_JUMP_MOTION_TO_GLIDE;

                    // �n�ʂɂ��Ȃ����Ƃ�
                    m_bGround = false;

                    // �O�̂��ߒ��n���Ԃ����Z�b�g
                    m_nCntLandingTime = 0;

                    // �W�����v�̃J�E���^���Z
                    m_nCntPressJump++;
                }
                else
                {
                    // ���i�W�����v�́A�`���[�W���ɂł��Ȃ�
                    if (m_nCntSwingCharge <= 0)
                    {
                        // ����\��:�O�i�W�����v
                        if (IS_BITON(m_exFlag, EX_FLAG_THREE_JUMP))
                        {
                            // �n�ʂɂ��Ȃ��A�O�i�W�����v�����Ă��Ȃ��A��i�W�����v��Ȃ�
                            if (!m_bUsedThreeJump && m_bUsedSecondJump)
                            {
                                // 1F����������ς��邱�Ƃ��ł���
                                RotControl();

                                // �W�����v�̏�����
                                m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                                // ����܂ł̃J�E���^��ݒ�
                                m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                                // �O�i�W�����v���g�����t���O��true��
                                m_bUsedThreeJump = true;

                                // �O�̂��ߒ��n���Ԃ����Z�b�g
                                m_nCntLandingTime = 0;

                                // �W�����v�̃J�E���^���Z
                                m_nCntPressJump++;

                                // �}�~�����Ȃ�}�~�������Z�b�g
                                if (m_bDiving)
                                {
                                    m_bDiving = false;
                                    m_bDiveCheck = true;
                                }
                            }
                        }

                        // �n�ʂɂ��Ȃ��A��i�W�����v�����Ă��Ȃ��Ȃ�
                        if (!m_bUsedSecondJump)
                        {
                            // 1F����������ς��邱�Ƃ��ł���
                            RotControl();

                            // �W�����v�̏�����
                            m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                            // ����܂ł̃J�E���^��ݒ�
                            m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                            // ��i�W�����v���g�����t���O��true��
                            m_bUsedSecondJump = true;

                            // �O�̂��ߒ��n���Ԃ����Z�b�g
                            m_nCntLandingTime = 0;

                            // �W�����v�̃J�E���^���Z
                            m_nCntPressJump++;

                            // �}�~�����Ȃ�}�~�������Z�b�g
                            if (m_bDiving)
                            {
                                m_bDiving = false;
                                m_bDiveCheck = true;
                            }
                        }
                    }
                }
            }
        }

        // �}�~���̔��肪�L���Ȃ�
        if (m_bDiveCheck)
        {
            // �����͂ŁA�}�~���̐���/���s����
            if (STICK_DOWN(m_controlInput.fLeftStickAngle))
            {
                // �W�����v�������Ă��Ȃ��Ȃ琬��
                if (m_nCntPressJump <= 0)
                {
                    m_bDiving = true;
                }
                else
                {
                    m_bDiveCheck = false;
                }
            }
        }

        // �}�~�����Ȃ�
        if (m_bDiving)
        {
            m_move.y -= (PLAYER_BASE_MINIMUM_GRAVITY + (m_fWei * PLAYER_ADD_BASE_GRAVITY_RATE))* PLAYER_DIVE_SPEED_RATE;
        }
    }
}

//=============================================================================
// �U���X�V����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::AttackUpdate(D3DXVECTOR3 pos, bool bFirstCollision)
{
    // �������Ă��Ȃ���Ԃ��A�U�����Ȃ�
    if (m_damageState == DAMAGE_STATE_NONE
        && m_attackState != ATTACK_STATE_NONE)
    {
        // ��ނɂ���ď�������
        int nCntAttackEndFrame = 0; // �U�����I���t���[����
        switch (m_attackState)
        {
            // �X�C���O�̃`���[�W
        case ATTACK_STATE_SWING_CHARGE:
            SwingCharge();
            break;

            // �X�C���O
        case ATTACK_STATE_SWING:
            nCntAttackEndFrame = ATTACK_SWING_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Swing(pos, bFirstCollision);
            }
            break;

            // �X�}�b�V��
        case ATTACK_STATE_SMASH:
            nCntAttackEndFrame = ATTACK_SMASH_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Smash(pos, bFirstCollision);
            }
            break;

            // �X�p�C�N
        case ATTACK_STATE_SPIKE:
            nCntAttackEndFrame = ATTACK_SPIKE_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Spike(pos, bFirstCollision);
            }
            break;

            // �L���b�`�̍\��
        case ATTACK_STATE_CATCH_READY:
            nCntAttackEndFrame = ATTACK_CATCH_READY_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                CatchReady(pos);
            }
            break;

            // �o���g
        case ATTACK_STATE_BUNT:
            nCntAttackEndFrame = ATTACK_BUNT_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Bunt(pos);
            }
            break;

            // �z��
        case ATTACK_STATE_ABSORB:
            nCntAttackEndFrame = ATTACK_ABSORB_END_FRAME;
            if (m_nCntAttackTime > nCntAttackEndFrame)
            {
                Absorb(pos);
            }
            break;
        }
    }
}

//=============================================================================
// �U����������
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::AttackGenerator(D3DXVECTOR3 pos)
{
    // �������Ă��Ȃ���Ԃ��A�U�����łȂ��Ȃ�
    if (m_damageState == DAMAGE_STATE_NONE && m_attackState == ATTACK_STATE_NONE)
    {
        // �n��ɂ���Ȃ�
        if (m_bGround)
        {
            // �ʏ�U���{�^������������A�X�C���O�̂�
            if (m_controlInput.bTriggerX)
            {
                // �X�C���O�̃`���[�W��ԁi�������Ă���Ȃ�`���[�W�Ȃ��Ō��j
                m_attackState = ATTACK_STATE_SWING_CHARGE;
                Swing(pos, true);
            }
            else if (m_controlInput.bTriggerY)
            {
                // �L���b�`�̍\��
                m_attackState = ATTACK_STATE_CATCH_READY;
                m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                CatchReady(pos);
            }
            else if (m_controlInput.bTriggerB)
            {
                // �o���g
                m_attackState = ATTACK_STATE_BUNT;
                m_nCntAttackTime = ATTACK_BUNT_WHOLE_FRAME;
                Bunt(pos);
            }
        }
        else
        {
            // �ʏ�U���{�^������������
            if (m_controlInput.bTriggerX)
            {
                // �󒆂ɂ���Ȃ�A�X�e�B�b�N�̊p�x�ɂ���ċZ���قȂ�
                if (m_controlInput.bTiltedLeftStick)
                {
                    if (STICK_UP(m_controlInput.fLeftStickAngle))
                    {
                        // ��̓X�C���O�ɂȂ�
                        m_attackState = ATTACK_STATE_SWING_CHARGE;
                        Swing(pos, true);
                    }
                    else if (STICK_RIGHT(m_controlInput.fLeftStickAngle) || STICK_LEFT(m_controlInput.fLeftStickAngle))
                    {
                        // ���E�̓X�}�b�V��(���̎��̂݁A�������󒆂ł��ς����)
                        m_attackState = ATTACK_STATE_SMASH;
                        m_nCntAttackTime = ATTACK_SMASH_WHOLE_FRAME;
                        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
                        RotControl();
                        Smash(pos, true);
                    }
                    else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
                    {
                        // ���̓X�p�C�N
                        m_attackState = ATTACK_STATE_SPIKE;
                        m_nCntAttackTime = ATTACK_SPIKE_WHOLE_FRAME;
                        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
                        Spike(pos, true);
                    }
                }
                else
                {
                    // �|��Ă��Ȃ��Ȃ�X�C���O
                    m_attackState = ATTACK_STATE_SWING_CHARGE;
                    Swing(pos, true);
                }
            }
            else if (m_controlInput.bTriggerY)
            {
                // �L���b�`�̍\��(���̎��̂݁A�������󒆂ł��ς����)
                m_attackState = ATTACK_STATE_CATCH_READY;
                m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                RotControl();
                CatchReady(pos);
            }
            else if (m_controlInput.bTriggerB)
            {
                // �o���g
                m_attackState = ATTACK_STATE_BUNT;
                m_nCntAttackTime = ATTACK_BUNT_WHOLE_FRAME;
                Bunt(pos);
            }
        }
    }
}

//=============================================================================
// �X�C���O�̃`���[�W
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SwingCharge(void)
{
    // �X�C���O�{�^����������������
    if (m_controlInput.bPressX)
    {
        // �`���[�W�J�E���^�A�b�v
        m_nCntSwingCharge++;
    }

    // �X�C���O�{�^������w�𗣂�����
    if (m_controlInput.bReleaseX)
    {
        // �X�C���O�̃N�[���^�C��������
        m_attackState = ATTACK_STATE_SWING;
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }

    // �ő�`���[�W�ɒB������
    if (m_nCntSwingCharge >= ATTACK_SWING_CHARGE_MAX_FRAME)
    {
        // �X�C���O�̃N�[���^�C��������
        m_attackState = ATTACK_STATE_SWING;
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }
}

//=============================================================================
// �X�C���O
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Swing(D3DXVECTOR3 playerPos, bool bFirstCollision)
{
    // �ϐ��錾
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ���炷�ʒu
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // �U�������ʒu
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// �U���̑傫��
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // �U���̈ړ��p�x
    float fAttackAngle = 0.0f;              // �U���̊p�x
    float fFinalPower = 0.0f;               // �ŏI�I�ȍU����

    // �����̑傫���ɂ���ĕς������
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_SWING_SLIDE_POS_SMALL;
        attackSize = ATTACK_SWING_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_SWING_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_SWING_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_SWING_SLIDE_POS_BIG;
        attackSize = ATTACK_SWING_SIZE_BIG;
        break;
    }

    // �X�e�B�b�N�̊p�x�ɂ���āA�U���p�x��ς���
    if (m_controlInput.bTiltedLeftStick)
    {
        // �ォ����
        if (STICK_SWING_UP(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = m_afParam[PARAM_SWING_UP];
        }
        else if (STICK_SWING_DOWN(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = m_afParam[PARAM_SWING_DOWN];
        }
        else if (STICK_SWING_HORIZON(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = 90.0f;
        }
    }
    else
    {
        // �X�e�B�b�N���|��Ă��Ȃ��Ȃ�A�U���p�x�̓j���[�g����
        fAttackAngle = 90.0f;
    }

    // �v���C���[�̌������������Ȃ�A�U�������ʒu�Ɣ�΂��������t��
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
        fAttackAngle *= -1;
    }

    // �U�������ʒu�����߂�
    attackPos = playerPos + slidePos;

    // ��΂�����������
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // �U���͂��l���i�`���[�W���ԂŊ�b�З͂��킸���ɏオ��j
    fFinalPower = (ATTACK_SWING_BASE_POWER + ATTACK_SWING_CHARGE_ATTACK_RATE * m_nCntSwingCharge) 
        + m_fAtk * ATTACK_SWING_ADD_BASE_POWER_RATE;

    // �ő�`���[�W�ŃX�}�b�V���Ɠ����̈З͂ɕς��
    float fMaxChargePower = 0.0f;
    if (m_nCntSwingCharge >= ATTACK_SWING_CHARGE_MAX_FRAME)
    {
        CBall* pBall = CGame::GetBall();
        fMaxChargePower = ATTACK_SMASH_BASE_POWER + m_fAtk * ATTACK_SMASH_ADD_BASE_POWER_RATE;
        fMaxChargePower *= pBall->GetSpeed() * ATTACK_SMASH_SPEED_RATE;
    }

    // �傫���͂̂ق����g��
    if (fFinalPower < fMaxChargePower)
    {
        fFinalPower = fMaxChargePower;
    }

    // �{�[���ɓ����������ǂ����i�ő�`���[�W�Ȃ瑦�ł��j
    int flag = CBall::SHOOT_FLAG_NONE;
    if (m_nCntSwingCharge >= ATTACK_SWING_CHARGE_MAX_FRAME)
    {
        BITON(flag, CBall::SHOOT_FLAG_QUICK);
    }
    if (IsAttackBall(attackPos, attackSize, moveAngle, fFinalPower, bFirstCollision, flag))
    {
        // �������Ă����瑦���ɃX�C���O��
        m_attackState = ATTACK_STATE_SWING;

        // �X�C���O�̃N�[���^�C������
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �X�}�b�V��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Smash(D3DXVECTOR3 playerPos, bool bFirstCollision)
{
    // �ϐ��錾
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ���炷�ʒu
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // �U�������ʒu
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// �U���̑傫��
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // �U���̈ړ��p�x
    float fAttackAngle = 0.0f;              // �U���̊p�x
    float fFinalPower = 0.0f;               // �ŏI�I�ȍU����

    // �����̑傫���ɂ���ĕς������
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_SMASH_SLIDE_POS_SMALL;
        attackSize = ATTACK_SMASH_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_SMASH_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_SMASH_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_SMASH_SLIDE_POS_BIG;
        attackSize = ATTACK_SMASH_SIZE_BIG;
        break;
    }

    // �X�}�b�V���̍U���p�x�͌Œ�
    fAttackAngle = m_afParam[PARAM_SMASH];

    // �v���C���[�̌������������Ȃ�A�U�������ʒu�Ɣ�΂��������t��
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
        fAttackAngle *= -1;
    }

    // �U�������ʒu�����߂�
    attackPos = playerPos + slidePos;

    // ��΂�����������
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // �U���͂��l���i�{�[���̑������֌W����j
    CBall* pBall = CGame::GetBall();
    fFinalPower = ATTACK_SMASH_BASE_POWER + m_fAtk * ATTACK_SMASH_ADD_BASE_POWER_RATE;
    fFinalPower *= pBall->GetSpeed() * ATTACK_SMASH_SPEED_RATE;

    // �K���ő�`���[�W�ȏ�̈З͂��o��
    float fMaxChargePower = (ATTACK_SWING_BASE_POWER + ATTACK_SWING_CHARGE_ATTACK_RATE * ATTACK_SWING_CHARGE_MAX_FRAME)
        + m_fAtk * ATTACK_SWING_ADD_BASE_POWER_RATE;
    if (fFinalPower < fMaxChargePower)
    {
        fFinalPower = fMaxChargePower;
    }

    // �{�[���ɓ����������ǂ���
    int flag = CBall::SHOOT_FLAG_NONE;
    IsAttackBall(attackPos, attackSize, moveAngle, fFinalPower, bFirstCollision, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �X�p�C�N
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Spike(D3DXVECTOR3 playerPos, bool bFirstCollision)
{
    // �ϐ��錾
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ���炷�ʒu
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // �U�������ʒu
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// �U���̑傫��
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // �U���̈ړ��p�x
    float fAttackAngle = 0.0f;              // �U���̊p�x
    float fFinalPower = 0.0f;               // �ŏI�I�ȍU����

    // �����̑傫���ɂ���ĕς������
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_SPIKE_SLIDE_POS_SMALL;
        attackSize = ATTACK_SPIKE_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_SPIKE_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_SPIKE_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_SPIKE_SLIDE_POS_BIG;
        attackSize = ATTACK_SPIKE_SIZE_BIG;
        break;
    }

    // �v���C���[�̌������l��
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_RIGHT)
    {
        // �X�e�B�b�N�̊p�x�ɂ���āA�U���p�x��ς���
        if (m_controlInput.bTiltedLeftStick)
        {
            if (STICK_SPIKE_RIGHT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = m_afParam[PARAM_SPIKE_RIGHT];
            }
            else if (STICK_SPIKE_LEFT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = m_afParam[PARAM_SPIKE_LEFT];
            }
            else if (STICK_SPIKE_VERTICAL(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = 180.0f;
            }
        }
        else
        {
            // �X�e�B�b�N���|��Ă��Ȃ��Ȃ�A�U���p�x�̓j���[�g����
            fAttackAngle = 180.0f;
        }
    }
    else if (rot.y == PLAYER_ROT_LEFT)
    {
        // �X�e�B�b�N�̊p�x�ɂ���āA�U���p�x��ς���
        if (m_controlInput.bTiltedLeftStick)
        {
            if (STICK_SPIKE_RIGHT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = -m_afParam[PARAM_SPIKE_LEFT];
            }
            else if (STICK_SPIKE_LEFT(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = -m_afParam[PARAM_SPIKE_RIGHT];
            }
            else if (STICK_SPIKE_VERTICAL(m_controlInput.fLeftStickAngle))
            {
                fAttackAngle = 180.0f;
            }
        }
        else
        {
            // �X�e�B�b�N���|��Ă��Ȃ��Ȃ�A�U���p�x�̓j���[�g����
            fAttackAngle = 180.0f;
        }
    }

    // �U�������ʒu�����߂�
    attackPos = playerPos + slidePos;

    // ��΂�����������
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // �U���͂��l��
    fFinalPower = ATTACK_SPIKE_BASE_POWER + m_fAtk * ATTACK_SPIKE_ADD_BASE_POWER_RATE;

    // �{�[���ɓ����������ǂ���
    int flag = CBall::SHOOT_FLAG_NONE;
    IsAttackBall(attackPos, attackSize, moveAngle, fFinalPower, bFirstCollision, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �L���b�`�̍\��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::CatchReady(D3DXVECTOR3 playerPos)
{
    // �ϐ��錾
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ���炷�ʒu
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // �U�������ʒu
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// �U���̑傫��

    // �����̑傫���ɂ���ĕς������
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_CATCH_READY_SLIDE_POS_SMALL;
        attackSize = ATTACK_CATCH_READY_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_CATCH_READY_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_CATCH_READY_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_CATCH_READY_SLIDE_POS_BIG;
        attackSize = ATTACK_CATCH_READY_SIZE_BIG;
        break;
    }

    // �v���C���[�̌������������Ȃ�A�U�������ʒu���t��
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
    }

    // �U�������ʒu�����߂�
    attackPos = playerPos + slidePos;

    // �{�[���ɓ����������ǂ����i�����̃t���O��ON�Ɂj
    int flag = CBall::SHOOT_FLAG_NONE;
    BITON(flag, CBall::SHOOT_FLAG_THROW);
    if (IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, false, flag))
    {
        // �������Ă����瑦���ɓ�����
        m_attackState = ATTACK_STATE_THROW;

        // �����̃N�[���^�C������
        m_nCntAttackTime = ATTACK_THROW_WHOLE_FRAME;

        // �K�E�Q�[�W�㏸
        GainSpGauge(true);
    }

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �o���g
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Bunt(D3DXVECTOR3 playerPos)
{
    // �ϐ��錾
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ���炷�ʒu
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // �U�������ʒu
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// �U���̑傫��
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR; // �U���̈ړ��p�x
    float fAttackAngle = 0.0f;              // �U���̊p�x

    // �����̑傫���ɂ���ĕς������
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_BUNT_SLIDE_POS_SMALL;
        attackSize = ATTACK_BUNT_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_BUNT_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_BUNT_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_BUNT_SLIDE_POS_BIG;
        attackSize = ATTACK_BUNT_SIZE_BIG;
        break;
    }

    // �X�e�B�b�N�̊p�x�ɂ���āA�U���p�x��ς���
    if (m_controlInput.bTiltedLeftStick)
    {
        if (STICK_RIGHT(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = ATTACK_BUNT_ANGLE;
        }
        else if (STICK_LEFT(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = -ATTACK_BUNT_ANGLE;
        }
        else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
        {
            fAttackAngle = 180.0f;
        }
    }

    // �v���C���[�̌������������Ȃ�A�U�������ʒu�Ɣ�΂��������t��
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
    }

    // �U�������ʒu�����߂�
    attackPos = playerPos + slidePos;

    // ��΂�����������
    moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
    moveAngle.y = cosf(D3DXToRadian(fAttackAngle));

    // �{�[���ɓ����������ǂ����i�o���g�̃t���O��ON�j
    int flag = CBall::SHOOT_FLAG_NONE;
    BITON(flag, CBall::SHOOT_FLAG_BUNT);
    IsAttackBall(attackPos, attackSize, moveAngle, 0.0f, false, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �z��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Absorb(D3DXVECTOR3 playerPos)
{
    // �ϐ��錾
    D3DXVECTOR3 slidePos = DEFAULT_VECTOR;  // ���炷�ʒu
    D3DXVECTOR3 attackPos = DEFAULT_VECTOR; // �U�������ʒu
    D3DXVECTOR3 attackSize = DEFAULT_VECTOR;// �U���̑傫��

    // �����̑傫���ɂ���ĕς������
    switch (m_nCharSize)
    {
    case CHAR_SIZE_SMALL:
        slidePos = ATTACK_ABSORB_SLIDE_POS_SMALL;
        attackSize = ATTACK_ABSORB_SIZE_SMALL;
        break;
    case CHAR_SIZE_MEDIUM:
        slidePos = ATTACK_ABSORB_SLIDE_POS_MEDIUM;
        attackSize = ATTACK_ABSORB_SIZE_MEDIUM;
        break;
    case CHAR_SIZE_BIG:
        slidePos = ATTACK_ABSORB_SLIDE_POS_BIG;
        attackSize = ATTACK_ABSORB_SIZE_BIG;
        break;
    }

    // �v���C���[�̌������������Ȃ�A�U�������ʒu���t��
    D3DXVECTOR3 rot = GetRot();
    if (rot.y == PLAYER_ROT_LEFT)
    {
        slidePos.x *= -1;
    }

    // �U�������ʒu�����߂�
    attackPos = playerPos + slidePos;

    // �{�[���ɓ����������ǂ����i�z���̃t���O��ON�Ɂj
    int flag = CBall::SHOOT_FLAG_NONE;
    BITON(flag, CBall::SHOOT_FLAG_ABSORB);
    IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, false, flag);

#ifdef COLLISION_TEST
    CDebug::Create(attackPos, attackSize, CDebug::TYPE_MOMENT);
#endif // COLLISION_TEST
}

//=============================================================================
// �{�[���ɍU���������������ǂ����̃`�F�b�N
// Author : �㓡�T�V��
//=============================================================================
bool CPlayer::IsAttackBall(D3DXVECTOR3 attackPos, D3DXVECTOR3 attackSize, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag)
{
    // �����������ǂ���
    bool bHit = false;

    // �{�[���Ƃ̓����蔻��
    CScene *pScene = CScene::GetTopScene(CScene::OBJTYPE_BALL);
    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_BALL); nCntScene++)
    {
        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // ���̃V�[�����L��
            CScene*pNextScene = pScene->GetNextScene();

            // �{�[���ɃL���X�g
            CBall *pBall = (CBall*)pScene;

            // �����蔻����g�p���邩�A�����ȊO�̒N�����z�����Ă��Ȃ��Ȃ�
            if (pBall->GetUseCollision())
            {
                if (pBall->GetWhoAbsorbing() == m_playable || pBall->GetWhoAbsorbing() == BALL_NOT_ANYONE)
                {
                    // �������Ă���Ȃ�
                    if (IsCollisionToRotationRect(attackPos, attackSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&attackPos, &pBall->GetPos(),
                            &attackSize, &BALL_COLLISION_SIZE))
                    {
                        // �t�@�[�q�b�g�Ȃ�t���OON
                        if (!IsCollisionRectangle3D(&attackPos, &pBall->GetPos(),
                            &attackSize, &BALL_COLLISION_SIZE))
                        {
                            BITON(flag, CBall::SHOOT_FLAG_FAR_HIT);
                        }

                        // �{�[�����΂�
                        D3DXVECTOR3 attackCenterPos = attackPos + D3DXVECTOR3(0.0f, attackSize.y / 2, 0.0f);
                        if (bFirstCollision)
                        {
                            // �ŏ��̐ڐG�́A��������d�����Ԃ��󂯂�
                            m_nCntStopTime = pBall->Shoot(attackCenterPos, moveAngle, fPower, bFirstCollision, flag, this);

                            // �K�E�Q�[�W�㏸
                            GainSpGauge();

                            // �d�����Ԃ��Ȃ��Ȃ�A�����ɔ�΂�
                            if (m_nCntStopTime <= 0)
                            {
                                pBall->Shoot(attackCenterPos, moveAngle, fPower, true, flag, this);
                            }
                        }
                        else
                        {
                            pBall->Shoot(attackCenterPos, moveAngle, fPower, bFirstCollision, flag, this);
                        }

                        // ��������
                        bHit = true;

                        // ���i�q�b�g���p�̃t���O
                        m_bUseAvoidMultipleHits = true;

                        // ���̃t���[�����ɍU���������������ǂ����̃t���O�i��ɍŏI�ǖʂōU���Ɩh��̓����蔻��𓯎��ɔ����������Ƃ��p�j
                        m_bHitAttackThisFrame = true;
                    }
                }
            }
        }
    }

    return bHit;
}

//=============================================================================
// �����̐���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::RotControl()
{
    // �ݒ肷���]
    D3DXVECTOR3 rot = GetRot();

    // �X�e�B�b�N���|��Ă���Ȃ������ς���
    if (m_controlInput.bTiltedLeftStick)
    {
        if (!m_bSquat)
        {
            // �X�e�B�b�N�̊p�x����A������ς���
            if (STICK_RIGHT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_RIGHT;
            }
            else if (STICK_LEFT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_LEFT;
            }
        }
        else
        {
            // ���Ⴊ�݂Ȃ��������ς���ꍇ�A�X�e�B�b�N�͉����
            if (STICK_SQUAT_RIGHT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_RIGHT;
            }
            else if (STICK_SQUAT_LEFT(m_controlInput.fLeftStickAngle))
            {
                rot.y = PLAYER_ROT_LEFT;
            }
        }
    }

    // ��]�̐ݒ�
    SetRot(rot);
}

//=============================================================================
// �U�����[�V�����̊Ǘ�
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::AttackMotion(void)
{
    // �U����Ԃɉ����āA���[�V���������߂�
    switch (m_attackState)
    {
    case ATTACK_STATE_SWING_CHARGE:
        GetAnimation()->SetAnimation(ANIM_SWING_CHARGE);
        break;
    case ATTACK_STATE_SWING:
        GetAnimation()->SetAnimation(ANIM_SWING);
        break;
    case ATTACK_STATE_SMASH:
        GetAnimation()->SetAnimation(ANIM_SMASH);
        break;
    case ATTACK_STATE_SPIKE:
        GetAnimation()->SetAnimation(ANIM_SPIKE);
        break;
    case ATTACK_STATE_BUNT:
        GetAnimation()->SetAnimation(ANIM_BUNT);
        break;
    case ATTACK_STATE_CATCH_READY:
        GetAnimation()->SetAnimation(ANIM_CATCH_READY);
        break;
    case ATTACK_STATE_THROW:
        GetAnimation()->SetAnimation(ANIM_THROW);
        m_move *= ATTACK_THROW_SLOW_SPEED_RATE; // �����Ă�Ƃ��̓X���[�Ȉړ�
        break;
    case ATTACK_STATE_ABSORB:
        GetAnimation()->SetAnimation(ANIM_ABSORB);
        break;
    }
}

//=============================================================================
// �����̍X�V����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::DamageUpdate(D3DXVECTOR3 pos)
{
    switch (m_damageState)
    {
    case DAMAGE_STATE_BLOWN:
        // ������΂��ꂩ��_�E���ւ̑J��
        if (m_bGround)
        {
            // �n�ʂɂ��Ă��A���x�������Ă���Ȃ�
            if (fabsf(m_move.x) < PLAYER_KNOCK_BACK_STOP_BORDER)
            {
                m_move.x = 0.0f;
                m_damageState = DAMAGE_STATE_BIG;
                m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_BIG_FRAME;
            }
        }
        else
        {
            // �󒆂̑��x���Œ���ێ�����
            if (fabsf(m_move.x) < PLAYER_KNOCK_BACK_STOP_BORDER)
            {
                if (m_move.x > 0.0f)
                {
                    m_move.x = PLAYER_KNOCK_BACK_STOP_BORDER;
                }
                else
                {
                    m_move.x = -PLAYER_KNOCK_BACK_STOP_BORDER;
                }
            }
        }
        break;
    case DAMAGE_STATE_BIG:
        // �N���オ��\���ԂȂ�
        if (m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_START_QUICK_STAND_UP)
        {
            // ���N���オ��or�z���N���オ��A�s�[��
            if (m_controlInput.bTiltedLeftStick)
            {
                m_damageState = DAMAGE_STATE_STAND_UP;
                m_nCntTakeDamageTime = PLAYER_TAKE_DAMAGE_STAND_UP_FRAME;
            }
            else if (m_controlInput.bTriggerX)
            {
                // �_���[�W��Ԃ̃��Z�b�g
                m_damageState = DAMAGE_STATE_NONE;
                m_nCntTakeDamageTime = 0;

                // �z��
                m_attackState = ATTACK_STATE_ABSORB;
                m_nCntAttackTime = ATTACK_ABSORB_WHOLE_FRAME;
                Absorb(pos);
            }
        }
        break;
    }
}

//=============================================================================
// �������[�V�����̊Ǘ�
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::DamageMotion(void)
{
    // ������Ԃɉ����āA���[�V���������߂�
    switch (m_damageState)
    {
    case DAMAGE_STATE_SMALL:
        GetAnimation()->SetAnimation(ANIM_DAMAGE_SMALL);
        break;
    case DAMAGE_STATE_BIG:
        GetAnimation()->SetAnimation(ANIM_DAMAGE_BIG);
        break;
    case DAMAGE_STATE_STAND_UP:
        GetAnimation()->SetAnimation(ANIM_STAND_UP);
        break;
    case DAMAGE_STATE_BLOWN:
        GetAnimation()->SetAnimation(ANIM_BLOWN);
        break;
    }
}

//=============================================================================
// �K�E�Q�[�W�㏸����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::GainSpGauge(bool bExAdd)
{
    // �K�E�Z�Q�[�W�㏸�ʂ��o��
    float fGainSpGauge = ATTACK_SP_GAUGE_BASE_GAIN + m_fAtk * ATTACK_SP_GAUGE_ADD_BASE_GAIN_RATE;

    // �L���b�`�Ȃǂ̍X�ɔ{���������Ȃ�s���Ȃ�
    if (bExAdd)
    {
        fGainSpGauge *= ATTACK_SP_GAUGE_EX_ADD_RATE;
    }

    // ���݂̃Q�[�W�ɑ���
    m_fSpGaugeCurrent += fGainSpGauge;

    // �ő�l������Ȃ��悤�ɂ���
    if (m_fSpGaugeCurrent > m_fSpGaugeMax)
    {
        m_fSpGaugeCurrent = m_fSpGaugeMax;
    }
}
