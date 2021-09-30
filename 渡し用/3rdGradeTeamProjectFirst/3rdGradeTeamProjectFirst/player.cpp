//======================================================================================
//
// �v���C���[���� (player.cpp)
// Author : �㓡�T�V���A�r�c�I��i���U���g��J�X�^�}�C�Y�̃N���b�s���O�}�X�N�Ή��j
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
#include "wave.h"
#include "effect3d.h"
#include "ui.h"
#include "effect2d.h"
#include "number_array.h"
#include "text.h"
#include "cliping_musk.h"
#include "modelEffect.h"
#include "number_array.h"

//========================================
// �}�N����`
//========================================
#define CUSTOM_ATK_COLOR D3DXCOLOR(0.874f, 0.329f, 0.094f, 1.0f)
#define CUSTOM_DEF_COLOR D3DXCOLOR(0.203f, 0.768f, 0.196f, 1.0f)
#define CUSTOM_SPD_COLOR D3DXCOLOR(0.156f, 0.56f, 0.854f, 1.0f)
#define CUSTOM_WEI_COLOR D3DXCOLOR(1.0f, 0.501f, 0.0f, 1.0f)

#define MAX_ATK 2250.0f
#define MAX_DEF 2000.0f
#define MAX_SPD 2700.0f
#define MAX_WEI 4100.0f

#define SHADOW_POS_Y 20.0f

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CPlayer::CPlayer() :CCharacter(OBJTYPE::OBJTYPE_PLAYER)
{
    m_bMannequin = false;
    m_controlInput.bTiltedLeftStick = false;
    m_controlInput.fLeftStickAngle = 0.0f;
    m_controlInput.bTiltedRightStick = false;
    m_controlInput.fRightStickAngle = 0.0f;
    m_controlInput.bTriggerA = false;
    m_controlInput.bPressA = false;
    m_controlInput.bTriggerX = false;
    m_controlInput.bPressX = false;
    m_controlInput.bReleaseX = false;
    m_controlInput.bTriggerY = false;
    m_controlInput.bTriggerB = false;
    m_bUseKeyboard = false;
    m_AIlevel = AI_LEVEL_NONE;
    m_pAI = NULL;
    m_pUI_HP = NULL;
    m_pUI_HP_red = NULL;
    m_pUI_SP = NULL;
    memset(m_apUI_Stock, 0, sizeof(m_apUI_Stock));
    m_pUI_Playable = NULL;
    m_pNumArray_SP = NULL;
    m_pEffect3d_Shadow = NULL;
    m_pUI_Custom_Atk = NULL;
    m_pUI_Custom_Def = NULL;
    m_pUI_Custom_Spd = NULL;
    m_pUI_Custom_Wei = NULL;
    m_pUI_Custom_Ability = NULL;
    m_pText_Custom_Ex_Head = NULL;
    m_pText_Custom_Ex_Up = NULL;
    m_pText_Custom_Ex_Down = NULL;
    m_pText_Custom_Ex_Wep = NULL;
    m_pText_Custom_Sp = NULL;
    m_bDispAbility = false;

    m_nIdxCreate = PLAYER_1;
    m_nIdxControlAndColor = PLAYER_1;
    m_nPoint = 0;
    m_nStock = 0;
    m_startPos = DEFAULT_VECTOR;
    m_startRot = DEFAULT_VECTOR;

    m_move = DEFAULT_VECTOR;
    m_moveOld = DEFAULT_VECTOR;
    m_exFlag = EX_FLAG_NONE;
    m_attackState = ATTACK_STATE_NONE;

    m_nCntAttackTime = 0;
    m_bGround = true;

    m_bDisp = true;
    m_nCntTakeDamageTime = 0;
    m_fLife = 0.0f;
    m_fLife_red = 0.0f;
    m_bUsedLuckyGuard = false;
    m_damageState = DAMAGE_STATE_NONE;

    m_nCntLandingTime = 0;
    m_bGroundOld = true;

    memset(m_anNumCustomParts, 0, sizeof(m_anNumCustomParts));
    m_fAtk = 0.0f;
    m_fDef = 0.0f;
    m_fSpd = 0.0f;
    m_fWei = 0.0f;
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

    m_collisionSizeAttack = DEFAULT_VECTOR;
    m_collisionSizeDeffence = DEFAULT_VECTOR;

	m_pClipingMusk = NULL;
    m_nNumWep = 0;
    m_nCntStopRedLifeTime = 0;
    m_rank = RANK_1;
    m_hipPosOld = DEFAULT_VECTOR;

    //===================================
    // ����\�͑Ή�����
    //===================================
    m_bUsedThreeJump = false;
    m_nSwingChargeMax = 0;
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

    // �A�j���[�V������ǂݍ���
    //CCharacter::LoadModelData("./data/ANIMATION/motion_player.txt");  // ���W�E�e�q�֌W�̓ǂݍ��݂ƁA�A�j���[�V�����̓ǂݍ��݂𕪂���
    CCharacter::SetAnimFilePass("./data/ANIMATION/motion_player.txt");

    // �v���C�A�u���L�����̐ݒ��ǂݍ���
    LoadCustom();

    return S_OK;
}

//=============================================================================
// �J�X�^�}�C�Y�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::LoadCustom(void)
{
    // �t�@�C���|�C���^
    FILE *pFile = NULL;

    // �p�[�c���X�g������擾
    CModelData *pModelData = CManager::GetModelData();

    // �ϐ��錾
    int nPartsListType = 0;                // �p�[�c���X�g���̎��
    int nExNumber = 0;                     // ����\�͔ԍ�
    int nVoiceSetNumber = 0;               // �{�C�X�Z�b�g�̔ԍ�

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
    m_collisionSizeAttack = DEFAULT_VECTOR;
    m_collisionSizeDeffence = DEFAULT_VECTOR;
    memset(m_afParam, 0, sizeof(m_afParam));
    m_nSwingChargeMax = ATTACK_SWING_CHARGE_MAX_FRAME_NORMAL;

    // �J�X�^�}�C�Y�f�[�^�̃t�@�C�����J��
    switch (m_nIdxControlAndColor)
    {
    case PLAYER_1:
        pFile = fopen("data/TXT/custom1.txt", "r");
        break;
    case PLAYER_2:
        pFile = fopen("data/TXT/custom2.txt", "r");
        break;
    case PLAYER_3:
        pFile = fopen("data/TXT/custom3.txt", "r");
        break;
    case PLAYER_4:
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
            int customPartsExFlag = 0;  // �J�X�^�}�C�Y��ʂŎg���A�p�[�c���̓���\��
            nExNumber = pModelData->GetPartsList(nPartsListType)->nEx0;
            BITON(m_exFlag, 0x001 << nExNumber);
            BITON(customPartsExFlag, 0x001 << nExNumber);
            nExNumber = pModelData->GetPartsList(nPartsListType)->nEx1;
            BITON(m_exFlag, 0x001 << nExNumber);
            BITON(customPartsExFlag, 0x001 << nExNumber);

            // �e�p�[�c���X�g����A�ו��̃p�[�c������
            if (nCntPartsList == PARTS_LIST_HEAD)
            {
                // �J�X�^�}�C�Y��ʂœ���\�͕\�����X�V
                if (m_pText_Custom_Ex_Head)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Head->SetText(cExName);
                }

                // ���f�����o�C���h
                BindParts(PARTS_HEAD, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);

                // �J�X�^�}�C�Y�p�[�c�ԍ����擾
                m_anNumCustomParts[CUSTOM_PARTS_HEAD] = nPartsListType;

                // �X�C���O�̌������A�⏕�l����⏕�l��
                m_afParam[PARAM_SWING_UP] = pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_afParam[PARAM_SWING_DOWN] = pModelData->GetPartsList(nPartsListType)->afParam[2];
                m_afParam[PARAM_SMASH] = pModelData->GetPartsList(nPartsListType)->afParam[3];
                m_afParam[PARAM_SPIKE_RIGHT] = pModelData->GetPartsList(nPartsListType)->afParam[4];
                m_afParam[PARAM_SPIKE_LEFT] = pModelData->GetPartsList(nPartsListType)->afParam[5];

                // �{�C�X�Z�b�g�̔ԍ���ێ�
                nVoiceSetNumber = (int)pModelData->GetPartsList(nPartsListType)->afParam[6];

                // �h�䓖���蔻��̍X�V
                if (m_collisionSizeDeffence.x < pModelData->GetPartsList(nPartsListType)->fWidth)
                {
                    m_collisionSizeDeffence.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                }
                m_collisionSizeDeffence.y += pModelData->GetPartsList(nPartsListType)->fHeight;
            }
            else if (nCntPartsList == PARTS_LIST_UP)
            {
                // �J�X�^�}�C�Y��ʂœ���\�͕\�����X�V
                if (m_pText_Custom_Ex_Up)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Up->SetText(cExName);
                }

                // ���f�����o�C���h
                BindParts(PARTS_BODY, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LSHOULDER, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LARM, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                // ����ɕ���������Ă���p�[�c�́A�J�X�^�}�C�Y��ʂł̗�O����
                if (CManager::GetMode() == CManager::MODE_RESULT)
                {
                    switch ((int)pModelData->GetPartsList(nPartsListType)->afParam[6])
                    {
                    case 51:
                        BindParts(PARTS_LHAND, 43);
                        break;
                    case 75:
                        BindParts(PARTS_LHAND, 84);
                        break;
                    case 93:
                        BindParts(PARTS_LHAND, 117);
                        break;
                    default:
                        BindParts(PARTS_LHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);
                        break;
                    }
                }
                else
                {
                    BindParts(PARTS_LHAND, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);
                }

                // �J�X�^�}�C�Y�p�[�c�ԍ����擾
                m_anNumCustomParts[CUSTOM_PARTS_UP] = nPartsListType;

                // �㔼�g�̏����ʒu�f�[�^
                m_nModelPosDefUp = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];

                // �h�䓖���蔻��̍X�V
                if (m_collisionSizeDeffence.x < pModelData->GetPartsList(nPartsListType)->fWidth)
                {
                    m_collisionSizeDeffence.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                }
                m_collisionSizeDeffence.y += pModelData->GetPartsList(nPartsListType)->fHeight;
            }
            else if (nCntPartsList == PARTS_LIST_DOWN)
            {
                // �J�X�^�}�C�Y��ʂœ���\�͕\�����X�V
                if (m_pText_Custom_Ex_Down)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Down->SetText(cExName);
                }

                // ���f�����o�C���h
                BindParts(PARTS_HIP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                BindParts(PARTS_RTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[1]);
                BindParts(PARTS_RLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[2]);
                BindParts(PARTS_RFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[3]);
                BindParts(PARTS_LTHIGH, (int)pModelData->GetPartsList(nPartsListType)->afParam[4]);
                BindParts(PARTS_LLEG, (int)pModelData->GetPartsList(nPartsListType)->afParam[5]);
                BindParts(PARTS_LFOOT, (int)pModelData->GetPartsList(nPartsListType)->afParam[6]);

                // �J�X�^�}�C�Y�p�[�c�ԍ����擾
                m_anNumCustomParts[CUSTOM_PARTS_DOWN] = nPartsListType;

                // �����g�̏����ʒu�f�[�^
                m_nModelPosDefDown = (int)pModelData->GetPartsList(nPartsListType)->afParam[7];

                // �h�䓖���蔻��̍X�V
                if (m_collisionSizeDeffence.x < pModelData->GetPartsList(nPartsListType)->fWidth)
                {
                    m_collisionSizeDeffence.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                }
                m_collisionSizeDeffence.y += pModelData->GetPartsList(nPartsListType)->fHeight;
            }
            else if (nCntPartsList == PARTS_LIST_WEP)
            {
                // �J�X�^�}�C�Y��ʂœ���\�͕\�����X�V
                if (m_pText_Custom_Ex_Wep)
                {
                    char cExName[256];
                    CustomExName(cExName, customPartsExFlag);
                    m_pText_Custom_Ex_Wep->SetText(cExName);
                }

                // ���f�����o�C���h
                BindParts(PARTS_WEP, (int)pModelData->GetPartsList(nPartsListType)->afParam[0]);
                m_nNumWep = (int)pModelData->GetPartsList(nPartsListType)->afParam[0];

                // �J�X�^�}�C�Y�p�[�c�ԍ����擾
                m_anNumCustomParts[CUSTOM_PARTS_WEP] = nPartsListType;

                // �K�E�Z�ƁA���̃Q�[�W��
                m_spShot = (int)pModelData->GetPartsList(nPartsListType)->afParam[1];
                m_fSpGaugeMax = pModelData->GetPartsList(nPartsListType)->afParam[2];

                // �K�E�Z�̕⏕�l��⏕�l��
                m_afParam[PARAM_5_WEAPON_SP] = pModelData->GetPartsList(nPartsListType)->afParam[5];
                m_afParam[PARAM_6_WEAPON_SP] = pModelData->GetPartsList(nPartsListType)->afParam[6];
                m_afParam[PARAM_7_WEAPON_SP] = pModelData->GetPartsList(nPartsListType)->afParam[7];

                // �U�������蔻��̍X�V
                m_collisionSizeAttack.x = pModelData->GetPartsList(nPartsListType)->fWidth;
                m_collisionSizeAttack.y = pModelData->GetPartsList(nPartsListType)->fHeight;
            }
        }
        // �t�@�C�������
        fclose(pFile);
    }

    // �J�X�^�}�C�Y��ʂŕK�E�Z�\�����X�V
    if (m_pText_Custom_Sp)
    {
        char cSpName[256];
        CustomSpName(cSpName);
        m_pText_Custom_Sp->SetText(cSpName);
    }

    // �̗͂�����
    m_fLife = m_fDef;
    m_fLife_red = m_fDef;

    // �{�C�X�Z�b�g�̔ԍ������т���
    BindVoiceSet(nVoiceSetNumber);

    // ���ߒZ�k�̃t���O��ON�Ȃ�A���f
    if (IS_BITON(m_exFlag, EX_FLAG_FAST_CHARGE))
    {
        m_nSwingChargeMax = ATTACK_SWING_CHARGE_MAX_FRAME_FAST;
    }

    // �L�����N�^�[�ɔ��f
    CCharacter::LoadModelData(m_nModelPosDefUp, m_nModelPosDefDown);
    CCharacter::Init(CCharacter::GetPos(), DEFAULT_SCALE);
}

//=============================================================================
// �{�C�X�Z�b�g�����т���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::BindVoiceSet(int voiceSet)
{
    switch (voiceSet)
    {
    case VOICE_SET_0:
        break;
    case VOICE_SET_1:
        break;
    case VOICE_SET_2:
        break;
    case VOICE_SET_3:
        break;
    }
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

    // �N���b�s���O�̏����J��
	if (m_pClipingMusk != NULL)
	{
		m_pClipingMusk->Uninit();
		delete m_pClipingMusk;
		m_pClipingMusk = NULL;
	}
	
    CCharacter::Uninit();
}

//=============================================================================
// ���͏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Input(void)
{
    // �R���g���[��or�L�[�{�[�h����i1P�̏ꍇ�AEnter�őJ�ڂ����Ȃ�L�[�{�[�h����ɕς���j
    if (m_bUseKeyboard)
    {
        // �L�[�{�[�h���擾
        CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

        // �{�^�������擾
        m_controlInput.bTriggerA = pInputKeyboard->GetKeyboardTrigger(DIK_SPACE);
        m_controlInput.bPressA = pInputKeyboard->GetKeyboardPress(DIK_SPACE);
        m_controlInput.bTriggerX = pInputKeyboard->GetKeyboardTrigger(DIK_RETURN);
        m_controlInput.bPressX = pInputKeyboard->GetKeyboardPress(DIK_RETURN);
        m_controlInput.bReleaseX = pInputKeyboard->GetKeyboardRelease(DIK_RETURN);
        m_controlInput.bTriggerY = pInputKeyboard->GetKeyboardTrigger(DIK_UP);
        m_controlInput.bTriggerB = pInputKeyboard->GetKeyboardTrigger(DIK_RIGHT);

        // ���X�e�B�b�N���X���Ă��邩�ǂ���
        if (pInputKeyboard->GetKeyboardPress(DIK_W) || 
            pInputKeyboard->GetKeyboardPress(DIK_A) ||
            pInputKeyboard->GetKeyboardPress(DIK_S) || 
            pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            m_controlInput.bTiltedLeftStick = true;

            // �p�x�����߂�
            if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(-45.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(-135.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_D))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(-90.0f);
                }
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(45.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(135.0f);
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_A))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(90.0f);
                }
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_S))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(0.0f);
                }
                else
                {
                    m_controlInput.fLeftStickAngle = D3DXToRadian(180.0f);
                }
            }
        }
        else
        {
            m_controlInput.bTiltedLeftStick = false;
        }
    }
    else
    {
        // �R���g���[�����擾
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
        DIJOYSTATE2 Controller = pInputJoypad->GetController(m_nIdxControlAndColor);

        // �{�^�������擾
        m_controlInput.bTriggerA = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_A);
        m_controlInput.bPressA = pInputJoypad->GetJoypadPress(m_nIdxControlAndColor, CInputJoypad::BUTTON_A);
        m_controlInput.bTriggerX = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_X);
        m_controlInput.bPressX = pInputJoypad->GetJoypadPress(m_nIdxControlAndColor, CInputJoypad::BUTTON_X);
        m_controlInput.bReleaseX = pInputJoypad->GetJoypadRelease(m_nIdxControlAndColor, CInputJoypad::BUTTON_X);
        m_controlInput.bTriggerY = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_Y);
        m_controlInput.bTriggerB = pInputJoypad->GetJoypadTrigger(m_nIdxControlAndColor, CInputJoypad::BUTTON_B);

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

        // �E�X�e�B�b�N���X���Ă��邩�ǂ���
        if (Controller.lZ != 0 || Controller.lRz != 0)
        {
            m_controlInput.bTiltedRightStick = true;

            // �p�x�����߂�
            m_controlInput.fRightStickAngle = atan2(Controller.lZ, Controller.lRz*-1);
        }
        else
        {
            m_controlInput.bTiltedRightStick = false;
        }
    }
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Update(void)
{
    // 1F�O�̍��̈ʒu���L��
    m_hipPosOld = GetPartsPos(PARTS_HIP);

    // �ԃQ�[�W�̃J�E���g�_�E��
    if (m_nCntStopRedLifeTime > 0)
    {
        m_nCntStopRedLifeTime--;
    }

    // �}�l�L�����[�h�łȂ��Ȃ�
    if (!m_bMannequin)
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
            if (m_nCntStopTime <= 0)
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
        else
        {
            // ���ꂽ���̋���
            DeadMove();
        }

        // �Q�[��UI�̍X�V
        UpdateGameUI();
    }
    else
    {
        // �}�l�L�����[�h�̍X�V
        UpdateMannequin();
    }

    // ������΂���G�t�F�N�g�́A��ʓ��Ő�����΂���Ă���Ȃ�o��
    if (m_damageState == DAMAGE_STATE_BLOWN)
    {
        if (m_nCntStopTime <= 0)
        {
            D3DXVECTOR3 hipPos = GetPartsPos(PARTS_HIP);
            if (hipPos.y > -500.0f)
            {
                CEffect3D::Emit(CEffectData::TYPE_BLOWN, hipPos, m_hipPosOld);
            }
        }
    }
}

//=============================================================================
// �}�l�L�����[�h�̍X�V����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::UpdateMannequin(void)
{
    CManager::MODE mode = CManager::GetMode();

    switch (mode)
    {
    case CManager::MODE_CUSTOM:
    {
        // �J�X�^�}�C�Y��ʂł̑ҋ@
        GetAnimation()->SetAnimation(ANIM_CUSTOM_IDLE);

        // �����ς�����A���������
        if (m_nCntAttackAnimTime > 0)
        {
            // ���������
            m_nCntAttackAnimTime--;
            GetAnimation()->SetAnimation(ANIM_WEAPON_LOOK);
        }

        // �\�����Ȃ�
        if (m_bDisp)
        {
            // ���͏���
            Input();

            // 1P�́A�L�[�{�[�h�ŉE�X�e�B�b�N�̉�]���ł���
            if (m_nIdxControlAndColor == PLAYER_1)
            {
                CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
                if (pInputKeyboard->GetKeyboardPress(DIK_UP) ||
                    pInputKeyboard->GetKeyboardPress(DIK_LEFT) ||
                    pInputKeyboard->GetKeyboardPress(DIK_DOWN) ||
                    pInputKeyboard->GetKeyboardPress(DIK_RIGHT))
                {
                    m_controlInput.bTiltedRightStick = true;

                    // �p�x�����߂�
                    if (pInputKeyboard->GetKeyboardPress(DIK_LEFT))
                    {
                        if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(-45.0f);
                        }
                        else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(-135.0f);
                        }
                        else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                        }
                        else
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(-90.0f);
                        }
                    }
                    else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT))
                    {
                        if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(45.0f);
                        }
                        else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(135.0f);
                        }
                        else if (pInputKeyboard->GetKeyboardPress(DIK_LEFT))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                        }
                        else
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(90.0f);
                        }
                    }
                    else if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                    {
                        if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                        }
                        else
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                        }
                    }
                    else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN))
                    {
                        if (pInputKeyboard->GetKeyboardPress(DIK_UP))
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(0.0f);
                        }
                        else
                        {
                            m_controlInput.fRightStickAngle = D3DXToRadian(180.0f);
                        }
                    }
                }
            }

            // �E�X�e�B�b�N���|��Ă���Ȃ������ς���
            D3DXVECTOR3 rot = GetRot();
            const float ROT_SPEED = 3.0f;
            if (m_controlInput.bTiltedRightStick)
            {
                // �E��]
                if (STICK_RIGHT(m_controlInput.fRightStickAngle))
                {
                    rot.y -= D3DXToRadian(ROT_SPEED);
                }
                else if (STICK_LEFT(m_controlInput.fRightStickAngle))
                {
                    // ����]
                    rot.y += D3DXToRadian(ROT_SPEED);
                }
            }

            // ��]�̐���
            if (rot.y > D3DXToRadian(180.0f) || rot.y < D3DXToRadian(-180.0f))
            {
                rot.y *= -1;
            }

            // ��]�̐ݒ�
            SetRot(rot);

            // �J�X�^�}�C�Y��ʂ�UI�̍X�V
            m_pUI_Custom_Atk->SetLeftToRightGauge(MAX_ATK, m_fAtk);
            m_pUI_Custom_Def->SetLeftToRightGauge(MAX_DEF, m_fDef);
            m_pUI_Custom_Spd->SetLeftToRightGauge(MAX_SPD, m_fSpd);
            m_pUI_Custom_Wei->SetLeftToRightGauge(MAX_WEI, m_fWei);
            if (m_bDispAbility)
            {
                m_pUI_Custom_Ability->SetDisp(true);
                m_pText_Custom_Ex_Head->SetColor(TEXT_EXIST_COLOR);
                m_pText_Custom_Ex_Up->SetColor(TEXT_EXIST_COLOR);
                m_pText_Custom_Ex_Down->SetColor(TEXT_EXIST_COLOR);
                m_pText_Custom_Ex_Wep->SetColor(TEXT_EXIST_COLOR);
                m_pText_Custom_Sp->SetColor(TEXT_EXIST_COLOR);
            }
            else
            {
                m_pUI_Custom_Ability->SetDisp(false);
                m_pText_Custom_Ex_Head->SetColor(TEXT_NOT_EXIST_COLOR);
                m_pText_Custom_Ex_Up->SetColor(TEXT_NOT_EXIST_COLOR);
                m_pText_Custom_Ex_Down->SetColor(TEXT_NOT_EXIST_COLOR);
                m_pText_Custom_Ex_Wep->SetColor(TEXT_NOT_EXIST_COLOR);
                m_pText_Custom_Sp->SetColor(TEXT_NOT_EXIST_COLOR);
            }
        }
        else
        {
            // ��\���Ȃ�A�e�\����������
            m_pUI_Custom_Atk->SetLeftToRightGauge(MAX_ATK, 0);
            m_pUI_Custom_Def->SetLeftToRightGauge(MAX_DEF, 0);
            m_pUI_Custom_Spd->SetLeftToRightGauge(MAX_SPD, 0);
            m_pUI_Custom_Wei->SetLeftToRightGauge(MAX_WEI, 0);
            m_pUI_Custom_Ability->SetDisp(false);
            m_pText_Custom_Ex_Head->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Ex_Up->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Ex_Down->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Ex_Wep->SetColor(TEXT_NOT_EXIST_COLOR);
            m_pText_Custom_Sp->SetColor(TEXT_NOT_EXIST_COLOR);
        }

        // �A�j���[�V����������
        CCharacter::Update();
        break;
    }

    case CManager::MODE_RESULT:
    {
        // ���U���g��ʂł̃A�j���[�V����
        switch (m_rank)
        {
        case RANK_1:
            GetAnimation()->SetAnimation(ANIM_FIRST);
            m_nCntAttackAnimTime++;
            if (m_nCntAttackAnimTime > PLAYER_VICTORY_WAIT_START_FRAME)
            {
                m_nCntAttackAnimTime = PLAYER_VICTORY_WAIT_START_FRAME;
                GetAnimation()->SetAnimation(ANIM_FIRST_WAIT);
            }
            break;
        case RANK_2:
            GetAnimation()->SetAnimation(ANIM_SECOND);
            break;
        case RANK_3:
            GetAnimation()->SetAnimation(ANIM_THIRD);
            break;
        case RANK_4:
            GetAnimation()->SetAnimation(ANIM_FOURTH);
            break;
        }

        // �A�j���[�V����������
        CCharacter::Update();
        break;
    }
    }

}

//=============================================================================
// �v���C���[������UI�̍X�V����
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::UpdateGameUI(void)
{
    // �e�̈ʒu���X�V
    D3DXVECTOR3 hipPos = CCharacter::GetPartsPos(PARTS_HIP);
    m_pEffect3d_Shadow->SetPos(D3DXVECTOR3(hipPos.x, SHADOW_POS_Y, hipPos.z));

    // HP�Q�[�W�A�ԃQ�[�W���X�V
    m_pUI_HP->SetLeftToRightGauge(m_fDef, m_fLife);
    if (m_fLife_red > m_fLife && m_nCntStopRedLifeTime <= 0)
    {
        // �ԃQ�[�W��~���Ԃ��I����Ă���A�X�V����
        const float LIFE_DOWN_SPD = 6.0f;
        m_fLife_red -= LIFE_DOWN_SPD;
    }

    // �ԃQ�[�W�́A���݂̗̑͂������Ȃ�
    if (m_fLife_red < m_fLife)
    {
        m_fLife_red = m_fLife;
    }
    m_pUI_HP_red->SetLeftToRightGauge(m_fDef, m_fLife_red);

    // SP�Q�[�W�ƃp�[�Z���g���l���X�V
    D3DXVECTOR3 spGaugeSize = m_pUI_SP->GetMemorySize();
    spGaugeSize.x *= m_fSpGaugeCurrent / m_fSpGaugeMax;
    spGaugeSize.y *= m_fSpGaugeCurrent / m_fSpGaugeMax;
    m_pUI_SP->SetSize(spGaugeSize);
    int nDispNumber = (int)((m_fSpGaugeCurrent / m_fSpGaugeMax) * 100.0f);
    m_pNumArray_SP->SetDispNumber(nDispNumber);

    // �X�g�b�N��\��/��\��
    for (int nCnt = 0; nCnt < PLAYER_MAX_STOCK; nCnt++)
    {
        if (m_apUI_Stock[nCnt])
        {
            if (nCnt < m_nStock)
            {
                m_apUI_Stock[nCnt]->CUI::SetCol(DEFAULT_COLOR);
            }
            else
            {
                m_apUI_Stock[nCnt]->CUI::SetCol(DEFAULT_COLOR_NONE);
            }
        }
    }

    // ������HP�ƃ{�[���̑����֌W�Ȃ�AHP�Q�[�W��_��
    float fDamage = CGame::GetBall()->GetSpeed() * PLAYER_TAKE_DAMAGE_RATE_FROM_BALL;
    D3DXCOLOR col = m_pUI_HP->GetCol();
    if (m_fLife <= fDamage)
    {
        if (col.a == 1.0f)
        {
            m_pUI_HP->SetCol(D3DXCOLOR(col.r, col.g, col.b, 0.0f));
        }
        else
        {
            m_pUI_HP->SetCol(D3DXCOLOR(col.r, col.g, col.b, 1.0f));
        }
    }
    else
    {
        m_pUI_HP->SetCol(D3DXCOLOR(col.r, col.g, col.b, 1.0f));
    }

    // ��P���̕\���̈ʒu�X�V�i���Ⴊ�݁A�������̓_�E�����N���オ�蒆�Ȃ�\���̈ʒu��������j
    float fSizeY = m_collisionSizeDeffence.y;
    if (m_bSquat ||
        m_damageState == DAMAGE_STATE_BIG || m_damageState == DAMAGE_STATE_STAND_UP)
    {
        fSizeY *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
    }
    m_pUI_Playable->SetPosTo2D(GetPos() + D3DXVECTOR3(0.0f, fSizeY + 50.0f, 0.0f));
}

//=============================================================================
// ���ꂽ���̋���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::DeadMove(void)
{
    // �d�����Ă��Ȃ��Ȃ�
    if (m_nCntStopTime <= 0)
    {
        // �ʒu���擾
        D3DXVECTOR3 pos = DEFAULT_VECTOR;
        pos = GetPos();

        // �ړ��ʐ���
        m_move.x *= PLAYER_CONTROL_MOVE_DEATH;

        // �ړ��ʂƈʒu�����т���
        pos += m_move;

        // �d��
        m_move.y -= PLAYER_TAKE_DAMAGE_BLOWN_GRAVITY;

        // �����Ȃ��Ƃ���Ŏ~�߂Ă���
        if (pos.y < PLAYER_DEATH_STOP_POS_Y)
        {
            pos.y = PLAYER_DEATH_STOP_POS_Y;
        }

        // �ʒu�𔽉f
        SetPos(pos);

        // ������΂��ꃂ�[�V����
        GetAnimation()->SetAnimation(ANIM_BLOWN);

        // �A�j���[�V�����X�V
        CCharacter::Update();
    }
    else
    {
        // �d�����Ԃ��J�E���g
        m_nCntStopTime--;
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
    m_nCntAttackTime = 0;
    m_nCntAttackAnimTime = 0;
    m_attackState = ATTACK_STATE_NONE;
    m_nCntSwingCharge = 0;
    m_bUseAvoidMultipleHits = false;
}

//=============================================================================
// ���X�|�[������
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::Respawn(void)
{
    // ���X�|�[��
    SetPos(m_startPos);
    SetRot(m_startRot);
    m_bDisp = true;
    m_fLife = m_fDef;
    m_fLife_red = m_fDef;

    // �����̃��Z�b�g�֐�
    ResetOnGround();
    ResetAttack();

    // �����o�ϐ��̃��Z�b�g
    m_move = DEFAULT_VECTOR;
    m_nCntLandingTime = 0;
    m_bGroundOld = false;
    m_nCntTurnTime = 0;
    m_bSquat = false;
    m_nCntStopTime = 0;

    m_nCntTakeDamageTime = 0;
    m_bUsedLuckyGuard = false;
    m_damageState = DAMAGE_STATE_NONE;

    m_fSpGaugeCurrent = 0.0f;
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V���A�r�c�I��i�N���b�s���O�}�X�N�Ή��j
//=============================================================================
void CPlayer::Draw(void)
{
    // �\������Ȃ�A�`��
    if (m_bDisp)
    {
        // �e
        if (m_pEffect3d_Shadow)
        {
            m_pEffect3d_Shadow->CBillboard::Draw();
        }

		// �N���b�s���O�}�X�N
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		if (m_pClipingMusk != NULL)
		{
			m_pClipingMusk->Draw();
			//�X�e���V���̐ݒ�
			pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_STENCILREF, m_pClipingMusk->GetReferenceValue() + 1);
			pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		}

        // �L�����N�^�[
        CCharacter::Draw();

        // �N���b�s���O�}�X�N�̌㏈��
		if (m_pClipingMusk != NULL)
		{
			//�X�e���V��������
			pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		}
    }
    else
    {
        // �}�l�L�����[�h�łȂ��Ȃ�
        if (!m_bMannequin)
        {
            // ���ꂽ���̕`��
            CCharacter::DeathDraw();
        }
    }
}

//=============================================================================
// �C���X�^���X����
// Author : �㓡�T�V��
//=============================================================================
CPlayer * CPlayer::CreateInGame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nStock, int nIdxCreate, int nIdxControlAndColor,
    AI_LEVEL AIlevel, bool bUseKeyboard)
{
    // �������m��
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // �e���̏���ݒ�
    pPlayer->CCharacter::SetPos(pos);
    pPlayer->CCharacter::SetRot(rot);

    // �ǂݍ��ގ�ނ̐ݒ�
    pPlayer->m_nIdxControlAndColor = nIdxControlAndColor;

    // ������
    pPlayer->Init(pos, DEFAULT_SCALE);

    // ���т��郁���o�ϐ��̏�����
    pPlayer->m_startPos = pos;
    pPlayer->m_startRot = rot;
    pPlayer->m_nStock = nStock;
    pPlayer->m_nIdxCreate = nIdxCreate;
    pPlayer->m_AIlevel = AIlevel;
    pPlayer->m_bUseKeyboard = bUseKeyboard;
    if (pPlayer->m_AIlevel != AI_LEVEL_NONE)
    {
        pPlayer->m_pAI = CAi::Create(pPlayer);
    }

    // �e�𐶐�
    pPlayer->m_pEffect3d_Shadow = CEffect3D::Create(CEffectData::TYPE_SHADOW, D3DXVECTOR3(pos.x, SHADOW_POS_Y, pos.z));
    pPlayer->m_pEffect3d_Shadow->SetSize(D3DXVECTOR3(pPlayer->m_collisionSizeDeffence.x, pPlayer->m_collisionSizeDeffence.x, 0.0f));
    pPlayer->m_pEffect3d_Shadow->SetDisp(false);

    // UI�𐶐�
    D3DXCOLOR playableCol = DEFAULT_COLOR;
    D3DXCOLOR spGagueCol = DEFAULT_COLOR;
    int nTexTypePlayable = 0;
    int nTexTypePlayer = 0;
    switch (pPlayer->m_nIdxControlAndColor)
    {
    case PLAYER_1:
        playableCol = PLAYER_COLOR_1;
        spGagueCol = D3DXCOLOR(0.768f, 0.470f, 0.470f, 1.0f);
        nTexTypePlayable = 21;
        nTexTypePlayer = 78;
        break;
    case PLAYER_2:
        playableCol = PLAYER_COLOR_2;
        spGagueCol = D3DXCOLOR(0.501f, 0.607f, 0.737f, 1.0f);
        nTexTypePlayable = 22;
        nTexTypePlayer = 79;
        break;
    case PLAYER_3:
        playableCol = PLAYER_COLOR_3;
        spGagueCol = D3DXCOLOR(0.513f, 0.654f, 0.509f, 1.0f);
        nTexTypePlayable = 23;
        nTexTypePlayer = 80;
        break;
    case PLAYER_4:
        playableCol = PLAYER_COLOR_4;
        spGagueCol = D3DXCOLOR(0.760f, 0.623f, 0.478f, 1.0f);
        nTexTypePlayable = 24;
        nTexTypePlayer = 81;
        break;
    }
    switch (pPlayer->m_AIlevel)
    {
    case AI_LEVEL_1:
        nTexTypePlayable = 25;
        break;
    case AI_LEVEL_2:
        nTexTypePlayable = 27;
        break;
    case AI_LEVEL_3:
        nTexTypePlayable = 28;
        break;
    }
    // HP�Q�[�W
    const float UI_SIZE_X = 294.0f; // �����̒l���AUI�̑�̂̉����ɍ��킹��
    const float UI_FRAME_SIZE_X = 220.0f;
    const float UI_FRAME_SIZE_Y = 28.0f;
    const float UI_BAR_SIZE_X = 204.0f;
    const float UI_BAR_SIZE_Y = 20.0f;
    const float SPACE_SIZE = (SCREEN_WIDTH - (UI_SIZE_X * CGame::GetNumAllPlayer())) / (CGame::GetNumAllPlayer() + 1);
    const float FIRST_UI_POS_X = SPACE_SIZE + (UI_SIZE_X / 2);
    const float NEXT_UI_POS_X = UI_SIZE_X + SPACE_SIZE;
    float fDigitPosX = FIRST_UI_POS_X + (NEXT_UI_POS_X * (float)pPlayer->m_nIdxCreate);
    CUI::Create(17, D3DXVECTOR3(fDigitPosX + 50.0f, 80.0f, 0.0f), D3DXVECTOR3(UI_FRAME_SIZE_X, UI_FRAME_SIZE_Y, 0.0f), 0, DEFAULT_COLOR);
    pPlayer->m_pUI_HP_red = CUI::Create(88, D3DXVECTOR3(fDigitPosX + 50.0f, 80.0f, 0.0f), D3DXVECTOR3(UI_BAR_SIZE_X, UI_BAR_SIZE_Y, 0.0f), 0, DEFAULT_COLOR);
    pPlayer->m_pUI_HP_red->SetActionInfo(0, CUI::ACTION_GAUGE, false);
    pPlayer->m_pUI_HP = CUI::Create(18, D3DXVECTOR3(fDigitPosX + 50.0f, 80.0f, 0.0f), D3DXVECTOR3(UI_BAR_SIZE_X, UI_BAR_SIZE_Y, 0.0f), 0, DEFAULT_COLOR);
    pPlayer->m_pUI_HP->SetActionInfo(0, CUI::ACTION_GAUGE, false);

    // �K�E�Q�[�W
    CUI::Create(76, D3DXVECTOR3(fDigitPosX - 80.0f, 60.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 0, DEFAULT_COLOR);
    pPlayer->m_pUI_SP = CUI::Create(77, D3DXVECTOR3(fDigitPosX - 80.0f, 60.0f, 0.0f), D3DXVECTOR3(85.0f, 85.0f, 0.0f), 0, spGagueCol);
    CUI::Create(86, D3DXVECTOR3(fDigitPosX - 57.5f, 60.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), 0, DEFAULT_COLOR);
    pPlayer->m_pNumArray_SP = CNumberArray::Create(12, D3DXVECTOR3(fDigitPosX - 87.5f, 60.0f, 0.0f), 15.0f,
        DEFAULT_COLOR, 0, false);

    // �X�g�b�N
    for (int nCntStock = 0; nCntStock < pPlayer->m_nStock; nCntStock++)
    {
        const float STOCK_SIZE_X = 36.0f;
        float fStockDigitPosX = 47.5f + ((STOCK_SIZE_X / 2) * nCntStock);  // -�Q�[�W�̔����̑傫��+�X�g�b�N�̔����̑傫��+(�X�g�b�N�̑傫��*���Ԗڂ̃X�g�b�N��)
        pPlayer->m_apUI_Stock[nCntStock] = CUI::Create(19, D3DXVECTOR3(fDigitPosX + fStockDigitPosX, 98.5f, 0.0f), D3DXVECTOR3(STOCK_SIZE_X, 24.0f, 0.0f), 0, DEFAULT_COLOR);
    }

    // Player�\��
    CUI::Create(nTexTypePlayer, D3DXVECTOR3(fDigitPosX + 10.0f, 50.0f, 0.0f), D3DXVECTOR3(82.5f, 54.0f, 0.0f), 0, DEFAULT_COLOR);

    // �v���C�A�u���\��
    pPlayer->m_pUI_Playable = CUI::Create(nTexTypePlayable, DEFAULT_VECTOR, D3DXVECTOR3(50.0f, 50.0f, 0.0f), 0, playableCol);

    return pPlayer;
}

//=============================================================================
// �J�X�^�}�C�Y��ʂł̐���
// Author : �㓡�T�V��
//=============================================================================
CPlayer * CPlayer::CreateInCustom(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxControlAndColor, bool bDisp)
{
    // �������m��
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // �e���̏���ݒ�
    pPlayer->CCharacter::SetPos(pos);
    pPlayer->CCharacter::SetRot(rot);

    // �ǂݍ��ގ�ނ̐ݒ�
    pPlayer->m_nIdxControlAndColor = nIdxControlAndColor;

    // UI�𐶐�
    D3DXVECTOR3 startPos = D3DXVECTOR3(205.0f, 293.5f, 0.0f);
    D3DXVECTOR3 clipingPos = DEFAULT_VECTOR;
    const D3DXVECTOR3 gaugeSize = D3DXVECTOR3(210.0f, 22.0f, 0.0f);
    const D3DXVECTOR3 abilitySize = D3DXVECTOR3(297.0f, 155.0f, 0.0f);
    D3DXVECTOR3 abilityPos = D3DXVECTOR3(167.0f, 200.5f, 0.0f);
    switch (pPlayer->m_nIdxControlAndColor)
    {
    case PLAYER_1:
        clipingPos = D3DXVECTOR3(167.0f, 390.0f, 0.0f);
        break;
    case PLAYER_2:
        startPos.x += 315.0f;
        abilityPos.x += 315.0f;
        clipingPos = D3DXVECTOR3(482.0f, 390.0f, 0.0f);
        break;
    case PLAYER_3:
        startPos.x += 315.0f * PLAYER_3;
        abilityPos.x += 315.0f * PLAYER_3;
        clipingPos = D3DXVECTOR3(797.0f, 390.0f, 0.0f);
        break;
    case PLAYER_4:
        startPos.x += 315.0f * PLAYER_4;
        abilityPos.x += 315.0f * PLAYER_4;
        clipingPos = D3DXVECTOR3(1112.0f, 390.0f, 0.0f);
        break;
    }
    pPlayer->m_pUI_Custom_Ability = CUI::Create(73, abilityPos, abilitySize, 0, DEFAULT_COLOR);
    float fDigitPosY = 0.0f;
    const float DIGIT_UI_VALUE = 31.5f;
    pPlayer->m_pUI_Custom_Def = CUI::Create(75, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_DEF_COLOR);
    pPlayer->m_pUI_Custom_Def->SetActionInfo(0, CUI::ACTION_GAUGE, false); // �Q�[�W�ɕς���
    fDigitPosY += DIGIT_UI_VALUE;

    pPlayer->m_pUI_Custom_Atk = CUI::Create(75, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_ATK_COLOR);
    pPlayer->m_pUI_Custom_Atk->SetActionInfo(0, CUI::ACTION_GAUGE, false); // �Q�[�W�ɕς���
    fDigitPosY += DIGIT_UI_VALUE;

    pPlayer->m_pUI_Custom_Spd = CUI::Create(75, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_SPD_COLOR);
    pPlayer->m_pUI_Custom_Spd->SetActionInfo(0, CUI::ACTION_GAUGE, false); // �Q�[�W�ɕς���
    fDigitPosY += DIGIT_UI_VALUE;

    pPlayer->m_pUI_Custom_Wei = CUI::Create(75, startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), gaugeSize, 0, CUSTOM_WEI_COLOR);
    pPlayer->m_pUI_Custom_Wei->SetActionInfo(0, CUI::ACTION_GAUGE, false); // �Q�[�W�ɕς���

    // �e�L�X�g�𐶐�
    startPos += D3DXVECTOR3(-105.0f, -155.0f, 0.0f); // UI�̃X�e�[�^�X�o�[����A�A�r���e�B�̃e�L�X�g�ֈʒu�����킹��
    fDigitPosY = 0.0f;
    const float DIGIT_TEXT_VALUE = 26.0f;
    const int textSize = 20;
    pPlayer->m_pText_Custom_Ex_Head = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "�Ȃ�",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Ex_Up = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "�Ȃ�",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Ex_Down = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "�Ȃ�",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Ex_Wep = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "�Ȃ�",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);
    fDigitPosY += DIGIT_TEXT_VALUE;

    pPlayer->m_pText_Custom_Sp = CText::Create(startPos + D3DXVECTOR3(0.0f, fDigitPosY, 0.0f), textSize, "�Ȃ�",
        CText::ALIGN_LEFT, "Reggae One", TEXT_NOT_EXIST_COLOR);

    // �N���b�s���O�𐶐�
    pPlayer->ApplyMusk(clipingPos, D3DXVECTOR3(298.0f, 625.0f, 0.0f));

    // ������
    pPlayer->Init(pos, DEFAULT_SCALE);

    // ���т��郁���o�ϐ��̏�����
    pPlayer->m_startPos = pos;
    pPlayer->m_startRot = rot;
    pPlayer->m_bDisp = bDisp;

    // �}�l�L�����[�h��
    pPlayer->m_bMannequin = true;

    return pPlayer;
}

//=============================================================================
// ���U���g��ʂł̐���
// Author : �㓡�T�V��
//=============================================================================
CPlayer * CPlayer::CreateInResult(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxControlAndColor, RANK rank)
{
    // �������m��
    CPlayer *pPlayer = NULL;
    pPlayer = new CPlayer;

    // �e���̏���ݒ�
    pPlayer->CCharacter::SetPos(pos);
    pPlayer->CCharacter::SetRot(rot);

    // �ǂݍ��ގ�ނ̐ݒ�
    pPlayer->m_nIdxControlAndColor = nIdxControlAndColor;

    // ������
    pPlayer->Init(pos, DEFAULT_SCALE);

    // ���т��郁���o�ϐ��̏�����
    pPlayer->m_rank = rank;
    if (pPlayer->m_rank != RANK_1)
    {
        // 1�ʈȊO�́A�����`�悵�Ȃ�
        pPlayer->SetDrawWeapon(false);
    }

    // �}�l�L�����[�h��
    pPlayer->m_bMannequin = true;

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

    // 1F�O�̈ړ��ʂ��L��
    m_moveOld = m_move;

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
    if (pos.x - m_collisionSizeDeffence.x < -mapLimit.fWidth)
    {
        pos.x = -mapLimit.fWidth + m_collisionSizeDeffence.x;
    }
    if (pos.x + m_collisionSizeDeffence.x > mapLimit.fWidth)
    {
        pos.x = mapLimit.fWidth - m_collisionSizeDeffence.x;
    }
    if (pos.y + m_collisionSizeDeffence.y > mapLimit.fHeight)
    {
        pos.y = mapLimit.fHeight - m_collisionSizeDeffence.y;
    }
    
    //====================================================
    // �U������
    // �U�����q�b�g���Ă��Ȃ��Ȃ�A�U���̔��蓙���X�V
    if (!m_bUseAvoidMultipleHits)
    {
        AttackUpdate(pos, true); // bFirestCollision��true�́A�������ŏ��̐ڐG�ɂȂ肤�邩��
    }

    // �U���̐���
    AttackGenerator(pos);

    // �W�����v
    Jump();

    // �ړ����[�V�����̊Ǘ�
    MoveMotion();

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
        if (m_damageState == DAMAGE_STATE_NONE || m_damageState == DAMAGE_STATE_STAND_UP && m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
        {
            D3DXVECTOR3 size = m_collisionSizeDeffence;
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
        // ���n�̍����i�����蔻��̕��A�������L����j
        CEffect3D::Emit(CEffectData::TYPE_GROUND_SMOKE, GetPos(), GetPos(), m_collisionSizeDeffence.x);

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
                if (pBall->GetWhoShooting() != m_nIdxCreate && pBall->GetWhoShooting() != BALL_NOT_ANYONE)
                {
                    // �����̓����蔻��̑傫��������
                    D3DXVECTOR3 playerSize = m_collisionSizeDeffence;

                    // ���Ⴊ��ł��邩�A�N���オ�蒆�Ȃ�c�̓����蔻������
                    if (m_bSquat || 
                        m_damageState == DAMAGE_STATE_STAND_UP && m_nCntTakeDamageTime <= PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME)
                    {
                        playerSize.y *= PLAYER_SQUAT_CUT_COLLISION_SIZE;
                    }

                    // �������Ă���Ȃ�
                    if (IsCollisionToRotationRect(playerPos, playerSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&playerPos, &pBall->GetCollisionPos(),
                            &playerSize, &BALL_COLLISION_SIZE))
                    {
                        // �_���[�W���v�Z�i���b�L�[�K�[�h�����\������j
                        float fDamage = pBall->GetSpeed() * PLAYER_TAKE_DAMAGE_RATE_FROM_BALL;
                        TakeDamage(fDamage, pBall->GetWhoShooting(), pBall->GetPos(), pBall->GetPosOld(), true);
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
void CPlayer::TakeDamage(float fDamage, int nWho, D3DXVECTOR3 damagePos, D3DXVECTOR3 damageOldPos, bool bUseLuckyGuard, bool bSetOff)
{
    // �������Ă���Ȃ�
    if (m_bDisp)
    {
        // �ԃQ�[�W�d�����Ԃ�ݒ�
        if (m_nCntStopRedLifeTime <= 0)
        {
            const int STOP_RED_TIME = 60;
            m_nCntStopRedLifeTime = STOP_RED_TIME;
        }

        // �U����Ԃ����Z�b�g
        ResetAttack();

        // �ϐ��錾
        D3DXVECTOR3 playerPos = GetPos();       // �ʒu���擾
        float fKnockbackValue = 0.0f;           // �m�b�N�o�b�N��
        bool bUseLuckyGuardThisFrame = false;   // ���̃t���[�����Ƀ��b�L�[�K�[�h�������������ǂ���

        // ���E�������ǂ���
        if (bSetOff)
        {
            // ���b�L�[�K�[�h���p
            bUseLuckyGuardThisFrame = true;
            fKnockbackValue = PLAYER_KNOCK_BACK_LUCKY_GUARD;        // ��p�̃m�b�N�o�b�N��
            m_nCntStopTime = BALL_SHOOT_STOP_LUCKY_GUARD_FRAME;
        }
        else
        {
            // ���E�����łȂ��Ȃ�A���b�L�[�K�[�h���g���U�����ǂ���
            if (bUseLuckyGuard)
            {
                // ���b�L�[�K�[�h���g���Ă��Ȃ��Ȃ�
                if (!m_bUsedLuckyGuard)
                {
                    // �����̌��ʂŁA���b�L�[�K�[�h
                    int nRandNum = GetRandNum(PLAYER_LUCKY_GUARD_MAX, 1);

                    // �t�@�[�X�g�q�b�g�K�[�h�Ȃ�A�ŏ��̈ꌂ��K���K�[�h
                    if (IS_BITON(m_exFlag, EX_FLAG_FIRST_HIT_GUARD))
                    {
                        nRandNum = 0;
                    }

                    // �ő�h����������̌��ʂ����������A���b�L�[�K�[�h
                    if (nRandNum <= (int)m_fDef)
                    {
                        // ���b�L�[�K�[�h�G�t�F�N�g����
                        CEffect3D::Emit(CEffectData::TYPE_LUCKY_GUARD_FIRST, playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f));
                        CEffect3D::Emit(CEffectData::TYPE_LUCKY_GUARD_SECOND, playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f));
                        CEffect3D::Emit(CEffectData::TYPE_LUCKY_GUARD_SECOND, playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f));

                        bUseLuckyGuardThisFrame = true;
                        m_bUsedLuckyGuard = true;
                        fDamage = 1.0f;                                         // 1�_���[�W�͎󂯂�
                        fKnockbackValue = PLAYER_KNOCK_BACK_LUCKY_GUARD;        // ��p�̃m�b�N�o�b�N��
                        m_nCntStopTime = CGame::GetBall()->Launch(playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f),
                            BALL_SHOOT_STOP_LUCKY_GUARD_FRAME);                 // �{�[����ł��グ��
                        CWave::Create(playerPos, BALL_WAVE_FIRST_SIZE);         // �g����
                        CManager::SoundPlay(CSound::LABEL_SE_OFFSET);           // ���E��
                    }
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
        int nEffectFrame = PLAYER_TAKE_DAMAGE_SMALL_EFFECT_FRAME;   // �U���t���[��
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

            // �傫���U��
            nEffectFrame = PLAYER_TAKE_DAMAGE_BIG_EFFECT_FRAME;
        }

        // �_���[�W���󂯂�
        m_fLife -= fDamage;

        // �_���[�W��\��
        if (fDamage > 0)
        {
            D3DXVECTOR3 dispDamagePos = ConvertScreenPos(playerPos);
            CNumberArray::Create(12, dispDamagePos, NUMBER_SIZE_X_DAMAGE, D3DXCOLOR(0.933f, 0.427f, 0.513f, 1.0f), (int)fDamage, false, true);

            // ���b�L�[�K�[�h�ȏ�̃_���[�W�ŁA�q�b�g�G�t�F�N�g
            if (fDamage > 1.0f)
            {
                // �����_���[�W�̈ʒu�ƁA1F�O�̃_���[�W�̈ʒu�������Ȃ�A�_���[�W�̈ʒu���v���C���[�̈ʒu�ɂ���
                D3DXVECTOR3 hitEffectPos = damagePos;
                if (damagePos == damageOldPos)
                {
                    hitEffectPos = playerPos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y * 0.5f, 0.0f);
                }
                CEffect3D::Emit(CEffectData::TYPE_HIT_BALL, hitEffectPos, damageOldPos);
            }
        }

        // �̗͂��Ȃ��Ȃ�����
        if (m_fLife <= 0.0f)
        {
            // ���S���̐U��
            nEffectFrame = PLAYER_TAKE_DAMAGE_DEATH_EFFECT_FRAME;

            // KO��
            CManager::SoundPlay(CSound::LABEL_SE_KO);

            // ���S�\����
            m_fLife = 0.0f;
            m_fSpGaugeCurrent = 0.0f;
            m_bDisp = false;

            // ���S�p�m�b�N�o�b�N
            fKnockbackValue = PLAYER_KNOCK_BACK_DEATH_X;
            m_move.y = PLAYER_KNOCK_BACK_DEATH_Y;

            // �X�g�b�N�����炷
            m_nStock--;

            // �X�g�b�N�����邩�ǂ���
            if (m_nStock > 0)
            {
                // ���ꂽ�v���C���[���܂��N�����Ȃ��Ȃ�
                if (CGame::GetNumDefeatPlayer() == 0)
                {
                    // �ŉ��ʂ̃v���C���[�ɂ���
                    CGame::SetWorstPlayer(m_nIdxCreate);
                }

                // ���ꂽ�v���C���[�l���𑝂₷
                CGame::SetAddNumDefeatPlayer(m_nIdxControlAndColor);
            }
            else
            {
                // ���񂾃v���C���[�l���𑝂₷
                CGame::SetAddNumDeathPlayer(m_nIdxControlAndColor);
            }

            // �{�[���ɂ��ꂽ�Ȃ�A�{�[�������������~�߂�i�{�[���ł������b�L�[�K�[�h�͔������Ȃ����ߑ�p�j
            m_nCntStopTime = PLAYER_DEATH_STOP_FRAME;
            if (bUseLuckyGuard)
            {
                CGame::GetBall()->SetStopTime(PLAYER_DEATH_STOP_FRAME);
            }

            // �Ō�ɍU�������l�Ƀ|�C���g������
            CPlayer* pPlayer = CGame::GetPlayer(nWho);
            if (pPlayer)
            {
                pPlayer->SetAddPoint();
            }
        }
        else
        {
            // ���b�L�[�K�[�h�ȏ�̃_���[�W��
            if (fDamage > 1.0f)
            {
                // �_���[�W��
                CManager::SoundPlay(CSound::LABEL_SE_DAMAGE);
            }
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

        // �R���g���[���̐U��
        if (GetUseControllerEffect())
        {
            CManager::GetInputJoypad()->StartEffect(m_nIdxControlAndColor, nEffectFrame);
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
                                // �����̔���
                                if (fabsf(m_moveOld.x) <= PLAYER_SET_WALK_SMOKE_VALUE)
                                {
                                    CEffect3D::Emit(CEffectData::TYPE_WALK_SMOKE_LEFT, GetPos(), GetPos());
                                }
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
                                // �����̔���
                                if (fabsf(m_moveOld.x) <= PLAYER_SET_WALK_SMOKE_VALUE)
                                {
                                    CEffect3D::Emit(CEffectData::TYPE_WALK_SMOKE_RIGHT, GetPos(), GetPos());
                                }
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
                    // �����ɂ���āA�����̌�����ς���
                    if (GetRot().y == PLAYER_ROT_LEFT)
                    {
                        // �W�����v�̍���
                        CEffect3D::Emit(CEffectData::TYPE_JUMP_SMOKE_RIGHT, GetPos(), GetPos());
                    }
                    else
                    {
                        // �W�����v�̍���
                        CEffect3D::Emit(CEffectData::TYPE_JUMP_SMOKE_LEFT, GetPos(), GetPos());
                    }

                    // �W�����v��
                    CManager::SoundPlay(CSound::LABEL_SE_JUMP);

                    // �W�����v�̏�����
                    m_move.y = PLAYER_JUMP_FIRST_RATE;

                    // ����܂ł̃J�E���^��ݒ�
                    m_nCntStartGlide = PLAYER_FIRST_JUMP_MOTION_TO_GLIDE;

                    // �n�ʂɂ��Ȃ����Ƃ�
                    m_bGround = false;

                    // �O�̂��ߒ��n���ԂƐU��������Ԃ����Z�b�g
                    m_nCntLandingTime = 0;
                    m_nCntTurnTime = 0;

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
                                // ���i�W�����v�̋�C�̗ւƍ����i�����蔻��̔����̑傫���j
                                CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_AIR_RING, GetPos(), GetPos(), m_collisionSizeDeffence.x * 0.5f);
                                CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_SMOKE, GetPos(), GetPos());

                                // �W�����v��
                                CManager::SoundPlay(CSound::LABEL_SE_JUMP);

                                // 1F����������ς��邱�Ƃ��ł���
                                RotControl();

                                // �W�����v�̏�����
                                m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                                // ����܂ł̃J�E���^��ݒ�
                                m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                                // �O�i�W�����v���g�����t���O��true��
                                m_bUsedThreeJump = true;

                                // �O�̂��ߒ��n���ԂƐU��������Ԃ����Z�b�g
                                m_nCntLandingTime = 0;
                                m_nCntTurnTime = 0;

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
                            // ���i�W�����v�̋�C�̗ւƍ����i�����蔻��̔����̑傫���j
                            CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_AIR_RING, GetPos(), GetPos(), m_collisionSizeDeffence.x * 0.5f);
                            CEffect3D::Emit(CEffectData::TYPE_SECOND_JUMP_SMOKE, GetPos(), GetPos());

                            // �W�����v��
                            CManager::SoundPlay(CSound::LABEL_SE_JUMP);

                            // 1F����������ς��邱�Ƃ��ł���
                            RotControl();

                            // �W�����v�̏�����
                            m_move.y = PLAYER_JUMP_FIRST_RATE * PLAYER_NEXT_JUMP_DOWN_RATE;

                            // ����܂ł̃J�E���^��ݒ�
                            m_nCntStartGlide = PLAYER_NEXT_JUMP_MOTION_TO_GLIDE;

                            // ��i�W�����v���g�����t���O��true��
                            m_bUsedSecondJump = true;

                            // �O�̂��ߒ��n���ԂƐU��������Ԃ����Z�b�g
                            m_nCntLandingTime = 0;
                            m_nCntTurnTime = 0;

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
            // �������Ă��Ȃ���ԂȂ�i�`���[�W���ł��}�~���ł���j
            if (m_damageState == DAMAGE_STATE_NONE)
            {
                // �����͂ŁA�}�~���̐���/���s����
                if (STICK_DOWN(m_controlInput.fLeftStickAngle))
                {
                    // �W�����v�������Ă��Ȃ��Ȃ琬��
                    if (m_nCntPressJump <= 0)
                    {
                        // �n�ʂɂ��Ȃ��Ȃ�
                        if (!m_bGround)
                        {
                            D3DXVECTOR3 headPos = GetPartsPos(PARTS_HEAD);
                            CEffect3D::Create(CEffectData::TYPE_DIVING, headPos);
                            m_bDiving = true;
                        }
                    }
                    else
                    {
                        m_bDiveCheck = false;
                    }
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
                // �L���b�`���ł���Ȃ�
                if (IS_BITOFF(m_exFlag, EX_FLAG_DONT_CATCH))
                {
                    // �L���b�`�̍\��
                    m_attackState = ATTACK_STATE_CATCH_READY;
                    m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                    CatchReady(pos);
                }
            }
            else if (m_controlInput.bTriggerB)
            {
                // �f�U�艹
                CManager::SoundPlay(CSound::LABEL_SE_SWISH);

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
                        // �f�U�艹
                        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

                        // ���E�̓X�}�b�V��(���̎��̂݁A�������󒆂ł��ς����)
                        m_attackState = ATTACK_STATE_SMASH;
                        m_nCntAttackTime = ATTACK_SMASH_WHOLE_FRAME;
                        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
                        RotControl();
                        Smash(pos, true);
                    }
                    else if (STICK_DOWN(m_controlInput.fLeftStickAngle))
                    {
                        // �f�U�艹
                        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

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
                // �L���b�`���ł���Ȃ�
                if (IS_BITOFF(m_exFlag, EX_FLAG_DONT_CATCH))
                {
                    // �L���b�`�̍\��(���̎��̂݁A�������󒆂ł��ς����)
                    m_attackState = ATTACK_STATE_CATCH_READY;
                    m_nCntAttackTime = ATTACK_CATCH_READY_WHOLE_FRAME;
                    RotControl();
                    CatchReady(pos);
                }
            }
            else if (m_controlInput.bTriggerB)
            {
                // �f�U�艹
                CManager::SoundPlay(CSound::LABEL_SE_SWISH);

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
        // �f�U�艹
        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

        // �X�C���O�̃N�[���^�C��������
        m_attackState = ATTACK_STATE_SWING;
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }

    // �ő�`���[�W�ɒB������
    if (m_nCntSwingCharge >= m_nSwingChargeMax)
    {
        // �f�U�艹
        CManager::SoundPlay(CSound::LABEL_SE_SWISH);

        // �X�C���O�̃N�[���^�C��������
        m_attackState = ATTACK_STATE_SWING;
        m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
        m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
    }

    // �`���[�W�G�t�F�N�g
    if (m_nCntSwingCharge >= ATTACK_SWING_CHARGE_EMIT_EFFECT_FRAME)
    {
        D3DXVECTOR3 pos = GetPos();
        D3DXVECTOR3 rot = GetRot();
        if (rot.y == PLAYER_ROT_LEFT)
        {
            CEffect3D::Emit(CEffectData::TYPE_CHARGE_SWING_LEFT, pos, pos);
        }
        else if (rot.y == PLAYER_ROT_RIGHT)
        {
            CEffect3D::Emit(CEffectData::TYPE_CHARGE_SWING_RIGHT, pos, pos);
        }
        D3DXVECTOR3 wepPos = GetPartsPos(PARTS_WEP);
        CEffect3D::Create(CEffectData::TYPE_CHARGE_LIGHT, wepPos);
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
    slidePos.x = (m_collisionSizeAttack.x * 0.5f) + (m_collisionSizeDeffence.x * 0.25f);
    attackSize.x = m_collisionSizeAttack.x + (m_collisionSizeDeffence.x * 0.5f);
    attackSize.y = m_collisionSizeDeffence.y;

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
    if (m_nCntSwingCharge >= m_nSwingChargeMax)
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
    if (m_nCntSwingCharge >= m_nSwingChargeMax)
    {
        BITON(flag, CBall::SHOOT_FLAG_QUICK);
    }
    if (IsAttackBall(attackPos, attackSize, moveAngle, fFinalPower, bFirstCollision, flag))
    {
        // �������Ă����瑦���ɃX�C���O�Ɂi����̓����蔻�莞�̂݁j
        if (bFirstCollision)
        {
            m_attackState = ATTACK_STATE_SWING;
            m_nCntAttackTime = ATTACK_SWING_WHOLE_FRAME;
            m_nCntAttackAnimTime = PLAYER_ATTACK_ANIM_MIN_FRAME;
        }
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
    slidePos.x = (m_collisionSizeAttack.x * 0.5f) + (m_collisionSizeDeffence.x * 0.25f);
    attackSize.x = m_collisionSizeAttack.x + (m_collisionSizeDeffence.x * 0.5f);
    slidePos.y = (m_collisionSizeAttack.y * 0.5f) + (m_collisionSizeDeffence.y * 0.25f);
    attackSize.y = m_collisionSizeAttack.y + (m_collisionSizeDeffence.y * 0.5f);

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
    float fMaxChargePower = (ATTACK_SWING_BASE_POWER + ATTACK_SWING_CHARGE_ATTACK_RATE * ATTACK_SWING_CHARGE_MAX_FRAME_NORMAL)
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
    slidePos.y = -((m_collisionSizeAttack.y * 0.5f) + (m_collisionSizeDeffence.y * 0.25f));
    attackSize.y = m_collisionSizeAttack.y + (m_collisionSizeDeffence.y * 0.5f);
    attackSize.x = m_collisionSizeDeffence.x * ATTACK_SPIKE_SIZE_RATE_X;

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

    // �����̑傫���ɂ���ĕς�����́i�L���b�`�ŕ���͍l�����Ȃ��j
    slidePos.x = m_collisionSizeDeffence.x * ATTACK_CATCH_READY_SLIDE_POS_X_RATE;
    attackSize.x = slidePos.x * 2.0f;
    attackSize.y = m_collisionSizeDeffence.y;

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
    if (IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, true, flag))
    {
        // �R���g���[���̐U��
        if (GetUseControllerEffect())
        {
            CManager::GetInputJoypad()->StartEffect(m_nIdxControlAndColor, ATTACK_CATCH_READY_EFFECT_FRAME);
        }

        // �L���b�`��
        CManager::SoundPlay(CSound::LABEL_SE_CATCH);

        // �������Ă����瑦���ɓ�����
        m_attackState = ATTACK_STATE_THROW;

        // �����̃N�[���^�C������
        m_nCntAttackTime = ATTACK_THROW_WHOLE_FRAME;
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
    slidePos.x = (m_collisionSizeAttack.x * 0.5f) + (m_collisionSizeDeffence.x * 0.25f);
    attackSize.x = m_collisionSizeAttack.x + (m_collisionSizeDeffence.x * 0.5f);
    attackSize.y = m_collisionSizeDeffence.y;

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
    IsAttackBall(attackPos, attackSize, moveAngle, 0.0f, true, flag);

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

    // �z���̑傫���́A�����̖h�䓖���蔻��̕��ςɊ���������������
    float fSize = (m_collisionSizeDeffence.x + m_collisionSizeDeffence.y) * 0.5f * ATTACK_ABSORB_SIZE_RATE;
    attackSize.x = fSize;
    attackSize.y = fSize;

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
    IsAttackBall(attackPos, attackSize, DEFAULT_VECTOR, 0.0f, false, flag); // ��ɍŏ��̐ڐG�ł͂Ȃ��A���E�ɏ���

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
                if (pBall->GetWhoAbsorbing() == m_nIdxCreate || pBall->GetWhoAbsorbing() == BALL_NOT_ANYONE)
                {
                    // �������Ă���Ȃ�
                    if (IsCollisionToRotationRect(attackPos, attackSize,
                        pBall->GetCornerPos(0), pBall->GetCornerPos(1), pBall->GetCornerPos(2), pBall->GetCornerPos(3))
                        || IsCollisionRectangle3D(&attackPos, &pBall->GetCollisionPos(),
                            &attackSize, &BALL_COLLISION_SIZE))
                    {
                        // �o���g�ȊO�Ȃ�
                        if (IS_BITOFF(flag, CBall::SHOOT_FLAG_BUNT))
                        {
                            // �t�@�[�q�b�g�Ȃ�t���OON
                            if (!IsCollisionRectangle3D(&attackPos, &pBall->GetCollisionPos(),
                                &attackSize, &BALL_COLLISION_SIZE))
                            {
                                BITON(flag, CBall::SHOOT_FLAG_FAR_HIT);
                            }
                        }
                        else
                        {
                            // �o���g�͑傫����1.5�{�Ŋ��Z���āA�t�@�[�q�b�g�Ȃ�t���OON�i�A���Ńo���g�������ۂɁA�s���R�Ɍ��������߁j
                            if (!IsCollisionRectangle3D(&attackPos, &pBall->GetCollisionPos(),
                                &(attackSize * 1.5f), &BALL_COLLISION_SIZE))
                            {
                                BITON(flag, CBall::SHOOT_FLAG_FAR_HIT);
                            }
                        }

                        // �{�[�����΂����߂̗\�������i�����Ń{�[���̒��S��n���̂́A�t�@�[�q�b�g�̎��ɂ��̈ʒu�֎����Ă������߁j
                        D3DXVECTOR3 attackCenterPos = attackPos + D3DXVECTOR3(0.0f, attackSize.y / 2, 0.0f);
                        CGame::ReserveShoot(attackCenterPos, moveAngle, fPower, bFirstCollision, flag, m_nIdxCreate);

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
// ����̎c�����c��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::LeaveWepAfterimage(void)
{
    // �U���A�j���[�V�����̍Œ�ۏ؊��ԂȂ�
    if (m_nCntAttackAnimTime > 0)
    {
        // ����̈ʒu���擾
        D3DXVECTOR3 wepPos = CCharacter::GetPartsPos(PARTS_WEP);

        // �v���C���[�̌����ɍ��킹�āA���f���G�t�F�N�g�̌��������킹��
        D3DXVECTOR3 playerRot = GetRot();
        D3DXVECTOR3 wepRot = DEFAULT_VECTOR;
        float fAngle = D3DXToRadian(12.5f);
        if (playerRot.y == PLAYER_ROT_LEFT)
        {
            fAngle = D3DXToRadian(-12.5f);
        }
        wepRot.z = fAngle * m_nCntAttackAnimTime;

        // ���f���G�t�F�N�g�𐶐�
        D3DXCOLOR col = DEFAULT_COLOR;
        switch (m_nIdxControlAndColor)
        {
        case PLAYER_1:
            col = PLAYER_COLOR_1;
            break;
        case PLAYER_2:
            col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case PLAYER_3:
            col = PLAYER_COLOR_3;
            break;
        case PLAYER_4:
            col = PLAYER_COLOR_4;
            break;
        }
        
        // ����ʑΉ�
        if (IS_BITON(m_exFlag, EX_FLAG_TRAIL_GREEN))
        {
            col = PLAYER_COLOR_3;
        }
        else if (IS_BITON(m_exFlag, EX_FLAG_TRAIL_PURPLE))
        {
            col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
        }

        CModelEffect *pCopy = CModelEffect::Create(m_nNumWep, wepPos, wepRot, col, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.025f));
        pCopy->SetAdditive();
    }
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
        LeaveWepAfterimage();
        GetAnimation()->SetAnimation(ANIM_SWING);
        break;
    case ATTACK_STATE_SMASH:
        LeaveWepAfterimage();
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
                // �z���G�t�F�N�g
                CEffect3D::Emit(CEffectData::TYPE_ABSORB, pos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y / 2, 0.0f),
                    pos + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y / 2, 0.0f), m_collisionSizeDeffence.y);

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

//=============================================================================
// �����E���h���Z�b�g����X�e�[�^�X
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::ResetStatusEveryRound(void)
{
    // �K�E�Q�[�W
    m_fSpGaugeCurrent = 0.0f;

    // ���b�L�[�K�[�h�i�t�@�[�X�g�q�b�g�K�[�h�̃L�����̂݃��Z�b�g�j
    if (IS_BITON(m_exFlag, EX_FLAG_FIRST_HIT_GUARD))
    {
        m_bUsedLuckyGuard = false;
    }
}

//=============================================================================
// �R���g���[���̐U�����g�p���邩�ǂ���
// Author : �㓡�T�V��
//=============================================================================
bool CPlayer::GetUseControllerEffect(void)
{
    bool bUseControllerEffect = false;

    // AI�łȂ����A�L�[�{�[�h����łȂ��Ȃ�A�U�����g��
    if (m_AIlevel == AI_LEVEL_NONE && !m_bUseKeyboard)
    {
        bUseControllerEffect = true;
    }

    // ������I�v�V�����ŋ���OFF�ł���悤�ɂ����ɏ���

    return bUseControllerEffect;
}


//=============================================================================
// �N���b�s���O�}�X�N��K�p
// Author : �r�c�I��
//=============================================================================
void CPlayer::ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumTexture)
{
    if (!m_pClipingMusk)
    {
        m_pClipingMusk = CClipingMusk::Create(pos, size, nNumTexture);
    }
}

//=============================================================================
// �e�N�X�`���Ȃ��N���b�s���O�}�X�N��K�p
// Author : �r�c�I��
//=============================================================================
void CPlayer::ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    if (!m_pClipingMusk)
    {
        m_pClipingMusk = CClipingMusk::Create(pos, size);
    }
}