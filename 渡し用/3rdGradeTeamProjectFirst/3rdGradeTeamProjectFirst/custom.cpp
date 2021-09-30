//=============================================================================
//
// �J�X�^�}�C�Y��ʏ��� [custom.cpp]
// Author : �㓡�T�V��
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "custom.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "debug.h"
#include "camera.h"
#include "modelData.h"
#include "text.h"
#include "player.h"
#include "game.h"
#include "effect2d.h"

//========================================
// �}�N����`
//========================================
#define CURSOR_FIRST_POS_P1 D3DXVECTOR3(167.0f, 390.0f, 0.0f)        // �J�[�\���̏����ʒuPlayer1
#define CURSOR_FIRST_POS_P2 D3DXVECTOR3(482.0f, 390.0f, 0.0f)        // �J�[�\���̏����ʒuPlayer2
#define CURSOR_FIRST_POS_P3 D3DXVECTOR3(797.0f, 390.0f, 0.0f)        // �J�[�\���̏����ʒuPlayer3
#define CURSOR_FIRST_POS_P4 D3DXVECTOR3(1112.0f, 390.0f, 0.0f)       // �J�[�\���̏����ʒuPlayer4
#define CURSOR_ADJUST_COLLISON_POS D3DXVECTOR3(-20.0f, -20.0f, 0.0f) // �J�[�\���̓����蔻��̈ʒu����
#define CURSOR_COLLISION_SIZE D3DXVECTOR3(5.0f, 5.0f, 0.0f)          // �J�[�\���̓����蔻��̑傫��
#define CURSOR_VISUAL_SIZE_SIDE 59.0f                                // �J�[�\���̈�ӂ̑傫��
#define CURSOR_VISUAL_SIZE D3DXVECTOR3(CURSOR_VISUAL_SIZE_SIDE, CURSOR_VISUAL_SIZE_SIDE, 0.0f)  // �J�[�\���̌����ڂ̑傫��
#define CURSOR_PRESS_VISUAL_SIZE D3DXVECTOR3(45.0f, 45.0f, 0.0f)     // �v���X�J�[�\���̌����ڂ̑傫��

#define CURSOR_CLICK_ACTION_INFO_IDX 3                               // �J�[�\���̃N���b�N�A�N�V������񂪓������C���f�b�N�X

//=============================================================================
// �J�X�^�}�C�Y��ʂ̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCustom::CCustom()
{
    memset(m_anMemoryPartsHead, NOT_EXIST, sizeof(m_anMemoryPartsHead));
    memset(m_anMemoryPartsUp, NOT_EXIST, sizeof(m_anMemoryPartsUp));
    memset(m_anMemoryPartsDown, NOT_EXIST, sizeof(m_anMemoryPartsDown));
    memset(m_anMemoryPartsWep, NOT_EXIST, sizeof(m_anMemoryPartsWep));
    memset(m_aEntryInfo, 0, sizeof(m_aEntryInfo));

    m_bClickReadyToFight = false;
    m_bDispReadyToFight = false;
}

//=============================================================================
// �J�X�^�}�C�Y��ʂ̃f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCustom::~CCustom()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CCustom::Init(void)
{
    // �����Ă���p�[�c�������т���
    BindHaveParts();

    // UI���O���t�@�C�����琶��
    CUI::Place(CUI::SET_CUSTOM);

    // �v���C���[(�}�l�L��)�̐���
    m_aEntryInfo[PLAYER_1].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(-950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_1, false);
    m_aEntryInfo[PLAYER_2].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(-316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_2, false);
    m_aEntryInfo[PLAYER_3].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_3, false);
    m_aEntryInfo[PLAYER_4].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_4, false);

    // �J�[�\������
    m_aEntryInfo[PLAYER_1].pUI_Cursor = CUI::Create(53, CURSOR_FIRST_POS_P1, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);
    m_aEntryInfo[PLAYER_2].pUI_Cursor = CUI::Create(54, CURSOR_FIRST_POS_P2, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);
    m_aEntryInfo[PLAYER_3].pUI_Cursor = CUI::Create(55, CURSOR_FIRST_POS_P3, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);
    m_aEntryInfo[PLAYER_4].pUI_Cursor = CUI::Create(56, CURSOR_FIRST_POS_P4, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);

    // �p�[�c�I�����̏�����
    float fTextPosX = 167.0f;
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        int nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_HEAD);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsHead[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectHead = nCntEachParts;
            }
        }

        nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_UP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsUp[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectUp = nCntEachParts;
            }
        }

        nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_DOWN);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsDown[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectDown = nCntEachParts;
            }
        }

        nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_WEP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsWep[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectWep = nCntEachParts;
            }
        }

        // �e�L�X�g��ݒ�
        D3DCOLOR color = TEXT_NOT_EXIST_COLOR;
        m_aEntryInfo[nCnt].pText_Head = CText::Create(D3DXVECTOR3(fTextPosX, 427.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsHead[m_aEntryInfo[nCnt].nNumSelectHead]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Up = CText::Create(D3DXVECTOR3(fTextPosX, 490.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsUp[m_aEntryInfo[nCnt].nNumSelectUp]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Down = CText::Create(D3DXVECTOR3(fTextPosX, 553.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsDown[m_aEntryInfo[nCnt].nNumSelectDown]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Wep = CText::Create(D3DXVECTOR3(fTextPosX, 616.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsWep[m_aEntryInfo[nCnt].nNumSelectWep]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);

        // �e�L�X�g�̉��ʒu�����Z
        fTextPosX += 315.0f;

        // 1F�O�̑I��������������
        m_aEntryInfo[nCnt].nNumSelectUIOld = NOT_EXIST;

        // �eUI�̃A�N�Z�X�������т���
        m_aEntryInfo[nCnt].pUI_Bg_Select = CUI::GetAccessUI(100 + (nCnt * 3));              // �G���g���[�ς�100
        m_aEntryInfo[nCnt].pUI_Bg_Ready = CUI::GetAccessUI(101 + (nCnt * 3));               // ��������101
        m_aEntryInfo[nCnt].pUI_Bg_Wait = CUI::GetAccessUI(102 + (nCnt * 3));                // ���G���g���[102
        m_aEntryInfo[nCnt].pUI_Bg_Select_Out_Frame = CUI::GetAccessUI(112 + (nCnt * 2));    // �J�X�^���O�g112
        m_aEntryInfo[nCnt].pUI_Bg_Select_In_Frame = CUI::GetAccessUI(113 + (nCnt * 2));     // �J�X�^�����g113
        m_aEntryInfo[nCnt].pUI_Bg_Select_Status = CUI::GetAccessUI(120 + nCnt);             // �J�X�^�����g120

        // �J�[�\���������Ă���
        m_aEntryInfo[nCnt].pUI_Cursor->SetDisp(false);
    }

    // �J�����̃��b�N�I���ꏊ��ς���
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    // �X�g�b�N���\�����X�V
    CUI *pStock = CUI::GetAccessUI(SELECT_STOCK);
    if (pStock)
    {
        pStock->SetTexturePlace(CGame::GetStock() - 1, PLAYER_MAX_STOCK);
    }

    // BGM���Đ�
    CManager::SoundPlay(CSound::LABEL_BGM_MENU);

    return S_OK;
}

//=============================================================================
// �����Ă���p�[�c�������т���
// Author : �㓡�T�V��
//=============================================================================
void CCustom::BindHaveParts(void)
{
    // �p�[�c���X�g����擾
    for (int nCntParts = 0; nCntParts < MAX_PARTS_LIST; nCntParts++)
    {
        // �ǂ��̕��ʂ��擾
        int nNumPart = CManager::GetModelData()->CModelData::GetPartsList(nCntParts)->part;

        // ���݂��Ȃ��Ȃ�A����
        if (nNumPart == NOT_EXIST)
        {
            continue;
        }

        // �󂫂Ƀp�[�c�ԍ������т��A���т�����for�𔲂���
        switch (nNumPart)
        {
        case CPlayer::CUSTOM_PARTS_HEAD:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsHead[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsHead[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_UP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsUp[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsUp[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_DOWN:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsDown[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsDown[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_WEP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsWep[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsWep[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        }
    }
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CCustom::Uninit(void)
{
    // BGM���~
    CManager::SoundStop(CSound::LABEL_BGM_MENU);
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CCustom::Update(void)
{
    // ReadyToFight�������ꂽ���ǂ����`�F�b�N����t���O����x������
    m_bClickReadyToFight = false;

    // �J�[�\���ړ�
    MoveCursor();

    // ���������ł��Ă��邩�̔��f
    JudgmentReadyToFight();
}

//=============================================================================
// �L�[�{�[�h����
// Author : �㓡�T�V��
//=============================================================================
void CCustom::MoveCursorByKeyboard(float& fStickAngle, bool& bTiltedStick, float& fTiltedStickValue)
{
    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
    // ���X�e�B�b�N���X���Ă��邩�ǂ���
    if (pInputKeyboard->GetKeyboardPress(DIK_W) ||
        pInputKeyboard->GetKeyboardPress(DIK_A) ||
        pInputKeyboard->GetKeyboardPress(DIK_S) ||
        pInputKeyboard->GetKeyboardPress(DIK_D))
    {
        bTiltedStick = true;
        fTiltedStickValue = 9000.0f;    // �R���g���[�����̍ő呬�x���A�኱�x�����Ă���

        // �p�x�����߂�
        if (pInputKeyboard->GetKeyboardPress(DIK_A))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                fStickAngle = D3DXToRadian(-45.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                fStickAngle = D3DXToRadian(-135.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(-90.0f);
            }
        }
        else if (pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                fStickAngle = D3DXToRadian(45.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                fStickAngle = D3DXToRadian(135.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(90.0f);
            }
        }
        else if (pInputKeyboard->GetKeyboardPress(DIK_W))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
        }
        else if (pInputKeyboard->GetKeyboardPress(DIK_S))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(180.0f);
            }
        }
    }
}

//=============================================================================
// �J�[�\���ړ�
// Author : �㓡�T�V��
//=============================================================================
void CCustom::MoveCursor(void)
{
    for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
    {
        // �R���g���[�����擾
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
        DIJOYSTATE2 Controller = pInputJoypad->GetController(nCntPlayer);
        float fStickAngle = 0.0f;
        bool bTiltedStick = false;
        float fTiltedStickValue = 0.0f;

        // ���X�e�B�b�N���X���Ă��邩�ǂ���
        if (Controller.lY != 0 || Controller.lX != 0)
        {
            bTiltedStick = true;

            // �p�x�����߂�
            fStickAngle = atan2(Controller.lX, Controller.lY*-1);

            // �傫�������߂�
            fTiltedStickValue = sqrtf(
                powf(float(Controller.lX), 2) +
                powf((float(Controller.lY)*-1), 2));

            // �ő�X�����傫���Ȃ�A�����i�����`�̑Ίp���́A�e�ӂ����������߁j
            if (fTiltedStickValue > STICK_MAX_TILT)
            {
                fTiltedStickValue = STICK_MAX_TILT;
            }
        }
        else
        {
            // �v���C���[1�Ȃ�L�[�{�[�h����\
            if (nCntPlayer == PLAYER_1)
            {
                MoveCursorByKeyboard(fStickAngle, bTiltedStick, fTiltedStickValue);
            }
        }

        // �J�[�\�����g���Ȃ�
        if (m_aEntryInfo[nCntPlayer].bUseCursor)
        {
            // �J�[�\��������Ȃ�
            if (m_aEntryInfo[nCntPlayer].pUI_Cursor)
            {
                // �ʒu���擾
                D3DXVECTOR3 cursorPos = m_aEntryInfo[nCntPlayer].pUI_Cursor->GetPosition();

                // �ړ�
                if (bTiltedStick)
                {
                    const float ADJUST_RATE = 0.0008f;   // �X�e�B�b�N�̌X���̒l���A�ʒu�ɑ�����悤����
                    cursorPos.x += sinf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                    cursorPos.y += -cosf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                }

                // �J�[�\���ړ��̐���
                RimitMoveCursor(cursorPos);

                // �ʒu��ݒ�
                m_aEntryInfo[nCntPlayer].pUI_Cursor->SetPosition(cursorPos);

                // �v���X�t�@�C�A
                PressFire(nCntPlayer);

                // �I�����Ƃ̓����蔻��i�J�[�\���̓����蔻��̈ʒu�͍���ɔ������j
                CollisionSelect(nCntPlayer, cursorPos + CURSOR_ADJUST_COLLISON_POS);
            }
        }
        else
        {
            // �J�[�\�����g���Ă��Ȃ��Ȃ�A�X�e�B�b�N���X������G���g���[
            if (bTiltedStick)
            {
                m_aEntryInfo[nCntPlayer].bUseCursor = true;

                // �G���g���[��Ԃ̃`�F���W
                ChangeEntryStatus(nCntPlayer, ENTRY_STATUS_PLAYER);

                // �����������Ȃ�A��������������
                if (m_aEntryInfo[nCntPlayer].bReady)
                {
                    ToggleReady(nCntPlayer);
                }
            }
        }
    }
}

//=============================================================================
// �J�[�\���ړ��̐���
// Author : �㓡�T�V��
//=============================================================================
void CCustom::RimitMoveCursor(D3DXVECTOR3& cursorPos)
{
    // ��
    if (cursorPos.x - (CURSOR_VISUAL_SIZE_SIDE / 2) < 0.0f)
    {
        cursorPos.x = (CURSOR_VISUAL_SIZE_SIDE / 2);
    }
    else if (cursorPos.x + (CURSOR_VISUAL_SIZE_SIDE / 2) > SCREEN_WIDTH)
    {
        cursorPos.x = SCREEN_WIDTH - (CURSOR_VISUAL_SIZE_SIDE / 2);
    }

    // �c
    if (cursorPos.y + (CURSOR_VISUAL_SIZE_SIDE / 2) > SCREEN_HEIGHT)
    {
        cursorPos.y = SCREEN_HEIGHT - (CURSOR_VISUAL_SIZE_SIDE / 2);
    }
    else if (cursorPos.y - (CURSOR_VISUAL_SIZE_SIDE / 2) < 0.0f)
    {
        cursorPos.y = (CURSOR_VISUAL_SIZE_SIDE / 2);
    }
}

//=============================================================================
// �v���X��������Ɖ����o��
// Author : �㓡�T�V��
//=============================================================================
void CCustom::PressFire(int nNumWho)
{
    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

    // A�{�^�����v���X�ŉ����ꂽ��
    bool bPressA = false;
    if (pInputJoypad->GetJoypadPress(nNumWho, CInputJoypad::BUTTON_A) ||
        pInputKeyboard->GetKeyboardPress(DIK_RETURN) && nNumWho == PLAYER_1)
    {
        bPressA = true;
    }

    // �J�[�\���̃T�C�Y���A�����Ă���Ԃ͏�������������
    if (bPressA)
    {
        m_aEntryInfo[nNumWho].pUI_Cursor->SetSize(CURSOR_PRESS_VISUAL_SIZE);
    }
    else
    {
        m_aEntryInfo[nNumWho].pUI_Cursor->SetSize(CURSOR_VISUAL_SIZE);
    }
}

//=============================================================================
// �I�����Ƃ̓����蔻��
// Author : �㓡�T�V��
//=============================================================================
void CCustom::CollisionSelect(int nNumWho, D3DXVECTOR3 cursorPos)
{
    // ���񓖂����Ă���I����
    int nNumSelectUICurrent = NOT_EXIST;
    for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
    {
        // �I���ł���UI���擾
        CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);

        // ���g������Ȃ�
        if (pSelectUI)
        {
            // �N��UI���ƁA��ނ��擾
            int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
            int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);

            // ����̐l��UI�Ȃ�
            if (nParamWho >= PLAYER_1 && nParamWho <= PLAYER_4)
            {
                // ���̐l�̑ҋ@���́A�`�F���W�{�^���ȊO�����Ȃ�
                if (m_aEntryInfo[nParamWho].status == ENTRY_STATUS_WAITING && nParamType != CLICK_TYPE_CHANGE)
                {
                    continue;
                }

                // ���̐l�̏����������́A���������{�^���Əڍ׃{�^���ȊO�����Ȃ�
                if (m_aEntryInfo[nParamWho].bReady && nParamType != CLICK_TYPE_READY && nParamType != CLICK_TYPE_DETAIL)
                {
                    continue;
                }

                // �J�[�\�����\������Ă��Ă��邩�A�G���g���[��Ԃ��v���C���[�Ȃ�
                if (m_aEntryInfo[nParamWho].bUseCursor && m_aEntryInfo[nParamWho].status == ENTRY_STATUS_PLAYER)
                {
                    // �{�l�����I������ς����Ȃ�
                    if (nNumWho != nParamWho)
                    {
                        continue;
                    }
                }

                // �ڍ׃{�^���́AReadyToFight������Ƃ������Ȃ�
                if(m_bDispReadyToFight && nParamType == CLICK_TYPE_DETAIL)
                {
                    continue;
                }
            }
            else
            {
                // ReadyToFight �́A�\������Ă��Ȃ��Ɖ����Ȃ�
                if (nParamType == CLICK_TYPE_READY_TO_FIGHT && !m_bDispReadyToFight)
                {
                    continue;
                }

                // �X�g�b�N �́A�\������Ă��Ȃ��Ɖ����Ȃ�
                if (nParamType == CLICK_TYPE_STOCK && !m_bDispReadyToFight)
                {
                    continue;
                }
            }

            // �������Ă���Ȃ�
            if (IsCollisionRectangle2D(&cursorPos, &pSelectUI->GetCollisionPos(),
                &CURSOR_COLLISION_SIZE, &pSelectUI->GetCollisionSize()))
            {
                // SE
                if (m_aEntryInfo[nNumWho].nNumSelectUIOld == NOT_EXIST)
                {
                    CManager::SoundPlay(CSound::LABEL_SE_CURSOLON);
                }

                // �g��k��������
                pSelectUI->SetActionLock(0, false);

                // �N���b�N����
                ClickSelect(nNumWho, pSelectUI, cursorPos);

                // ���񓖂����Ă���I����
                nNumSelectUICurrent = nCntSelect;

                // ��ނɂ���ẮA�X���C�h�C������
                if (nParamType == CLICK_TYPE_RETURN)
                {
                    pSelectUI->SetActionLock(1, false);
                    pSelectUI->SetActionLock(2, true);
                }

                // for�𔲂���
                break;
            }
        }
    }

    // 1F�O�̑I����������I���ł��Ă��Ȃ��Ȃ�A�T�C�Y�����Z�b�g���g�k���b�N
    if (nNumSelectUICurrent != m_aEntryInfo[nNumWho].nNumSelectUIOld)
    {
        CUI *pResetUI = CUI::GetAccessUI(m_aEntryInfo[nNumWho].nNumSelectUIOld);
        if (pResetUI)
        {
            pResetUI->SetActionReset(0);
            pResetUI->SetActionLock(0, true);

            // ��ނɂ���ẮA�X���C�h�C������
            int nParamType = (int)pResetUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
            if (nParamType == CLICK_TYPE_RETURN)
            {
                pResetUI->SetActionLock(1, true);
                pResetUI->SetActionLock(2, false);
            }
        }
    }

    // 1F�O�̑I�������L��
    m_aEntryInfo[nNumWho].nNumSelectUIOld = nNumSelectUICurrent;
}

//=============================================================================
// �I�����̃N���b�N
// Author : �㓡�T�V��
//=============================================================================
void CCustom::ClickSelect(int nNumWho, CUI* pSelectUI, D3DXVECTOR3 cursorPos)
{
    // �I��UI�����݂��Ȃ��Ȃ�֐��𔲂���
    if (!pSelectUI)
    {
        return;
    }

    // �t�F�[�h���Ă��Ȃ��Ȃ�A�I���\
    if (CFade::GetFade() == CFade::FADE_NONE)
    {
        //�L�[�{�[�h�̊m�ۂ������������擾
        CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

        // �R���g���[�����擾
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

        // A�{�^�����g���K�[�ŉ����ꂽ��
        bool bTriggerA = false;
        bool bTriggerReturn = false;
        if (pInputJoypad->GetJoypadTrigger(nNumWho, CInputJoypad::BUTTON_A) ||
            pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) && nNumWho == PLAYER_1)
        {
            bTriggerA = true;

            // P1���L�[�{�[�h���Q�[���ł��g�����ǂ���
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) && nNumWho == PLAYER_1)
            {
                bTriggerReturn = true;
            }
        }

        if (bTriggerA)
        {
            // �N���b�N���̃L���L��
            CEffect2D::Emit(CEffectData::TYPE_KIRA, cursorPos, cursorPos);

            // ��ނɂ���Ĕ�����ς���
            switch ((int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE))
            {
            case CLICK_TYPE_PARTS:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);
                    // �N���ǂ��̃p�[�c�̍��E�ǂ����I�񂾂�
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    int nParamWhere = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHERE);
                    bool bRight = false;
                    if ((int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_RIGHT) == 1)
                    {
                        bRight = true;
                    }
                    SelectParts(nParamWho, nParamWhere, bRight);
                }
                break;

            case CLICK_TYPE_CHANGE:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_SELECT);
                    // ���̏�Ԃɉ����āA���̃G���g���[��ԂɃ`�F���W
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    ENTRY_STATUS nextEntryStatus = ENTRY_STATUS_WAITING;
                    switch (m_aEntryInfo[nParamWho].status)
                    {
                    case ENTRY_STATUS_WAITING:
                        if (m_aEntryInfo[nParamWho].bUseCursor)
                        {
                            nextEntryStatus = ENTRY_STATUS_PLAYER;
                        }
                        else
                        {
                            nextEntryStatus = ENTRY_STATUS_CP_LEVEL_1;
                        }
                        break;
                    case ENTRY_STATUS_PLAYER:
                        nextEntryStatus = ENTRY_STATUS_CP_LEVEL_1;
                        break;
                    case ENTRY_STATUS_CP_LEVEL_1:
                        nextEntryStatus = ENTRY_STATUS_CP_LEVEL_2;
                        break;
                    case ENTRY_STATUS_CP_LEVEL_2:
                        nextEntryStatus = ENTRY_STATUS_CP_LEVEL_3;
                        break;
                    case ENTRY_STATUS_CP_LEVEL_3:
                        // �����J�[�\�����\������Ă��Ă��A���̒N���ɂ���ĉ����ꂽ�Ȃ�ACP��킢��
                        // �������Ă���Œ��ɁA���̒N���ɂ���ď������̂�h��
                        if (m_aEntryInfo[nParamWho].bUseCursor && nNumWho != nParamWho)
                        {
                            nextEntryStatus = ENTRY_STATUS_CP_LEVEL_1;
                        }
                        else
                        {
                            nextEntryStatus = ENTRY_STATUS_WAITING;
                        }
                        break;
                    }
                    ChangeEntryStatus(nParamWho, nextEntryStatus);
                }
                break;

            case CLICK_TYPE_READY:
                {
                    // 1P���������ꍇ�A�L�[�{�[�h���������悤�ɓo�^
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    ToggleReady(nParamWho);
                    if (nNumWho == PLAYER_1)
                    {
                        CGame::SetUseKeyboard(bTriggerReturn);
                    }
                }
                break;

            case CLICK_TYPE_DETAIL:
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    m_aEntryInfo[nParamWho].pPlayer->SetDispAbility(!m_aEntryInfo[nParamWho].pPlayer->GetDispAbility());
                    if (m_aEntryInfo[nParamWho].pPlayer->GetDispAbility())
                    {
                        CManager::SoundPlay(CSound::LABEL_SE_INFO);
                    }
                    else
                    {
                        CManager::SoundPlay(CSound::LABEL_SE_CANCEL);
                    }
                }
                break;

            case CLICK_TYPE_READY_TO_FIGHT:
                m_bClickReadyToFight = true;
                break;

            case CLICK_TYPE_RETURN:
                // ������x��ʂɕ\�����ꂽ��
                if (pSelectUI->GetPosition().x > 40.0f)
                {
                    CManager::SoundPlay(CSound::LABEL_SE_CANCEL);
                    CFade::SetFade(CManager::MODE_TITLE);   // ���Ƀ^�C�g��
                }
                break;

            case CLICK_TYPE_STOCK:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_SELECT);
                    // �X�g�b�N���擾���A���Z���A���f
                    int nStock = CGame::GetStock() + 1;
                    if (nStock > PLAYER_MAX_STOCK)
                    {
                        nStock = 1;
                    }
                    pSelectUI->SetTexturePlace(nStock - 1, PLAYER_MAX_STOCK);
                    CGame::SetStock(nStock);
                }
                break;

            case CLICK_TYPE_OSUSUME:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_SELECT);
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    DoOsusume(nParamWho);
                }
                break;
            }
        }
    }
}

//=============================================================================
// �G���g���[��Ԃ̃`�F���W
// Author : �㓡�T�V��
//=============================================================================
void CCustom::ChangeEntryStatus(int nNumWho, ENTRY_STATUS nextEntryStatus)
{
    // ���̃G���g���[��Ԃɍ��킹�āA�\������ς���
    switch (nextEntryStatus)
    {
    case ENTRY_STATUS_WAITING:
        {
            // ���̑S�����J�[�\�����g���Ă��Ȃ��Ȃ�
            bool bAllOtherCursorNone = true;
            for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
            {
                if (nNumWho == nCntPlayer)
                {
                    continue;
                }

                if (m_aEntryInfo[nCntPlayer].bUseCursor)
                {
                    bAllOtherCursorNone = false;
                    break;
                }
            }

            // �N��1�l�ł��J�[�\�����g���Ă���Ȃ�
            if (!bAllOtherCursorNone)
            {
                // �J�[�\���𕕈�
                m_aEntryInfo[nNumWho].bUseCursor = false;
                m_aEntryInfo[nNumWho].nNumSelectUIOld = NOT_EXIST;
                m_aEntryInfo[nNumWho].bReady = false;
                m_aEntryInfo[nNumWho].pUI_Cursor->SetFirstPos();
                m_aEntryInfo[nNumWho].pUI_Cursor->SetDisp(false);
            }

            // ��\��
            m_aEntryInfo[nNumWho].pPlayer->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_In_Frame->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Status->SetDisp(false);
            m_aEntryInfo[nNumWho].pText_Head->SetColor(TEXT_NOT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Up->SetColor(TEXT_NOT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Down->SetColor(TEXT_NOT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Wep->SetColor(TEXT_NOT_EXIST_COLOR);
            for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
            {
                CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
                if (pSelectUI)
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    if (nParamWho == nNumWho)
                    {
                        int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                        if (nParamType != CLICK_TYPE_CHANGE)
                        {
                            // �I�����������Ȃ��悤��
                            pSelectUI->SetDisp(false);
                        }
                        else
                        {
                            // �`�F���W�̓v���C���[�\���ɂ��A�T�C�Y���Z�b�g����̊g�k���b�N
                            pSelectUI->SetTexturePlace(0, 4);
                            pSelectUI->SetActionReset(0);
                            pSelectUI->SetActionLock(0, true);
                        }
                    }
                }
            }
            // �\��
            m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetDisp(true);
        }
        break;

    case ENTRY_STATUS_PLAYER:
    case ENTRY_STATUS_CP_LEVEL_1:
        // �����A�ҋ@������̑J�ڂȂ�
        if (m_aEntryInfo[nNumWho].status == ENTRY_STATUS_WAITING)
        {
            // ��\��
            m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(false);
            // �\��
            m_aEntryInfo[nNumWho].pPlayer->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_In_Frame->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Status->SetDisp(true);
            m_aEntryInfo[nNumWho].pText_Head->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Up->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Down->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Wep->SetColor(TEXT_EXIST_COLOR);
            // ���������
            m_aEntryInfo[nNumWho].pPlayer->SetCustomWeaponLook();
        }

        if (nextEntryStatus == ENTRY_STATUS_PLAYER)
        {
            // �v���C���[�ւ̑J�ڂȂ�A�J�[�\����\��
            if (!m_aEntryInfo[nNumWho].pUI_Cursor->GetDisp())
            {
                m_aEntryInfo[nNumWho].pUI_Cursor->SetDisp(true);
                CManager::SoundPlay(CSound::LABEL_SE_ENTRY);
            }
        }

        // �I�����͑J�ڌ��Ɋւ�炸�A�ς��\��������
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                if (nParamWho == nNumWho)
                {
                    int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                    if (nParamType != CLICK_TYPE_CHANGE)
                    {
                        // �����A�ҋ@������̑J�ڂȂ�
                        if (m_aEntryInfo[nNumWho].status == ENTRY_STATUS_WAITING)
                        {
                            // �I������������悤��
                            pSelectUI->SetDisp(true);
                        }
                    }
                    else
                    {
                        // �`�F���W�\���ؑ�
                        if (nextEntryStatus == ENTRY_STATUS_PLAYER)
                        {
                            pSelectUI->SetTexturePlace(0, 4);
                        }
                        else if (nextEntryStatus == ENTRY_STATUS_CP_LEVEL_1)
                        {
                            pSelectUI->SetTexturePlace(1, 4);
                        }
                    }
                }
            }
        }
        break;

    case ENTRY_STATUS_CP_LEVEL_2:
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                if (nParamType == CLICK_TYPE_CHANGE)
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    if (nParamWho == nNumWho)
                    {
                        // �`�F���W�\���ؑ�
                        pSelectUI->SetTexturePlace(2, 4);
                    }
                }
            }
        }
        break;

    case ENTRY_STATUS_CP_LEVEL_3:
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                if (nParamType == CLICK_TYPE_CHANGE)
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    if (nParamWho == nNumWho)
                    {
                        // �`�F���W�\���ؑ�
                        pSelectUI->SetTexturePlace(3, 4);
                    }
                }
            }
        }
        break;
    }

    // ���݂̏�Ԃ��A���̃G���g���[��ԂɍX�V
    m_aEntryInfo[nNumWho].status = nextEntryStatus;
}

//=============================================================================
// �����󋵂𔽓]������
// Author : �㓡�T�V��
//=============================================================================
void CCustom::ToggleReady(int nNumWho)
{
    // ���]
    m_aEntryInfo[nNumWho].bReady = !m_aEntryInfo[nNumWho].bReady;

    // SE
    if (m_aEntryInfo[nNumWho].bReady)
    {
        CManager::SoundPlay(CSound::LABEL_SE_OK);
    }
    else
    {
        CManager::SoundPlay(CSound::LABEL_SE_CANCEL);
    }

    // �����󋵂ɉ����āA�\������ς���
    if (m_aEntryInfo[nNumWho].bReady)
    {
        // ��\��
        m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetDisp(false);
        m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(false);
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                if (nParamWho == nNumWho)
                {
                    int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                    if (nParamType == CLICK_TYPE_PARTS)
                    {
                        // �p�[�c�̑I�����͌����Ȃ��悤��
                        pSelectUI->SetDisp(false);
                    }
                    else if (nParamType == CLICK_TYPE_READY)
                    {
                        // �\���ؑ�
                        pSelectUI->SetTexturePlace(1, 2);
                    }
                }
            }
        }
        // �\��
        m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(true);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetActionLock(0, false);
    }
    else
    {
        // ��\��
        m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(false);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetActionReset(0);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetActionLock(0, true);
        // �\��
        m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(true);
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                if (nParamWho == nNumWho)
                {
                    int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                    if (nParamType == CLICK_TYPE_PARTS)
                    {
                        // �p�[�c�̑I������������悤��
                        pSelectUI->SetDisp(true);
                    }
                    else if (nParamType == CLICK_TYPE_READY)
                    {
                        // �\���ؑ�
                        pSelectUI->SetTexturePlace(0, 2);
                    }
                }
            }
        }
    }
}

//=============================================================================
// ���������ł��Ă��邩�̔��f
// Author : �㓡�T�V��
//=============================================================================
void CCustom::JudgmentReadyToFight(void)
{
    // ���݂̃G���g���[�l���A�eUI
    int nNumCurrentEntryPlayer = 0;
    CUI *pReadyToFightBg = CUI::GetAccessUI(124);
    CUI *pReadyToFightText = CUI::GetAccessUI(SELECT_READY_TO_FIGHT);
    CUI *pStock = CUI::GetAccessUI(SELECT_STOCK);

    // �ҋ@���ȊO�̐l�������Z
    for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
    {
        if (m_aEntryInfo[nCntPlayer].status != ENTRY_STATUS_WAITING)
        {
            nNumCurrentEntryPlayer++;

            // �����������Ă��Ȃ��Ȃ�A�֐��𔲂���
            if (!m_aEntryInfo[nCntPlayer].bReady)
            {
                m_bDispReadyToFight = false;
                if (pReadyToFightBg)
                {
                    pReadyToFightBg->SetActionReset(1);
                    pReadyToFightBg->SetActionLock(1, true);
                    pReadyToFightBg->SetActionReset(2);
                    pReadyToFightBg->SetActionLock(2, true);
                }
                if (pReadyToFightText)
                {
                    pReadyToFightText->SetActionReset(1);
                    pReadyToFightText->SetActionLock(1, true);
                    pReadyToFightText->SetActionReset(2);
                    pReadyToFightText->SetActionLock(2, true);
                }
                if (pStock)
                {
                    pStock->SetActionReset(1);
                    pStock->SetActionLock(1, true);
                }
                return;
            }
        }
    }

    // �G���g���[�l����2�l�ȏ�i�������������Ă���j�Ȃ�AReadyToFight�ƃX�g�b�N��\��
    if (nNumCurrentEntryPlayer >= 2)
    {
        if (!m_bDispReadyToFight)
        {
            CManager::SoundPlay(CSound::LABEL_SE_READY);
        }
        m_bDispReadyToFight = true;
        if (pReadyToFightBg)
        {
            pReadyToFightBg->SetActionLock(1, false);
            pReadyToFightBg->SetActionLock(2, false);
        }
        if (pReadyToFightText)
        {
            pReadyToFightText->SetActionLock(1, false);
            pReadyToFightText->SetActionLock(2, false);
        }
        if (pStock)
        {
            pStock->SetActionLock(1, false);
        }
    }

    // ReadyToFight�������ꂽ�Ȃ�A�Q�[���֑J��
    if (m_bClickReadyToFight)
    {
        // �G�t�F�N�g
        D3DXVECTOR3 effectPos = D3DXVECTOR3(0.0f, 231.0f, 0.0f);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT1, effectPos, effectPos);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT2, effectPos, effectPos);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT3, effectPos, effectPos);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT4, effectPos, effectPos);

        CManager::SoundPlay(CSound::LABEL_SE_HIT_BIG);
        CFade::SetFade(CManager::MODE_GAME);
        CGame::SetNextGameInCustom(CGame::TYPE_ARENA, nNumCurrentEntryPlayer);

        // �v���C�A�u���AAI���x�������т��Ă���
        int nIndexEntryPlayer = 0;
        for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
        {
            // �G���g���[���Ă���L�����Ȃ�
            if (m_aEntryInfo[nCntPlayer].status != ENTRY_STATUS_WAITING)
            {
                // �󂢂Ă���Ƃ���ɁA�������т���
                CPlayer::AI_LEVEL level = CPlayer::AI_LEVEL_NONE;
                switch (m_aEntryInfo[nCntPlayer].status)
                {
                case ENTRY_STATUS_CP_LEVEL_1:
                    level = CPlayer::AI_LEVEL_1;
                    break;
                case ENTRY_STATUS_CP_LEVEL_2:
                    level = CPlayer::AI_LEVEL_2;
                    break;
                case ENTRY_STATUS_CP_LEVEL_3:
                    level = CPlayer::AI_LEVEL_3;
                    break;
                }
                CGame::SetIdxPlayer(nIndexEntryPlayer, nCntPlayer);
                CGame::SetAILevel(nIndexEntryPlayer, level);
                nIndexEntryPlayer++;
            }
        }
    }
}

//=============================================================================
// �J�X�^�}�C�Y����p�[�c��I��
// Author : �㓡�T�V��
//=============================================================================
void CCustom::SelectParts(int nNumWho, int nNumWhere, bool bRight)
{
    // �I���̃C���f�b�N�X
    int nSelectIndex = 0;
    switch (nNumWhere)
    {
    case CPlayer::CUSTOM_PARTS_HEAD:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectHead;
        break;
    case CPlayer::CUSTOM_PARTS_UP:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectUp;
        break;
    case CPlayer::CUSTOM_PARTS_DOWN:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectDown;
        break;
    case CPlayer::CUSTOM_PARTS_WEP:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectWep;
        break;
    }

    // �E������
    if (bRight)
    {
        nSelectIndex++;

        // ���̑I���ŉE�[�̃p�[�c�����邩�ǂ����m���߂�
        int nRightParts = 0;
        switch (nNumWhere)
        {
        case CPlayer::CUSTOM_PARTS_HEAD:
            nRightParts = m_anMemoryPartsHead[nSelectIndex];
            break;
        case CPlayer::CUSTOM_PARTS_UP:
            nRightParts = m_anMemoryPartsUp[nSelectIndex];
            break;
        case CPlayer::CUSTOM_PARTS_DOWN:
            nRightParts = m_anMemoryPartsDown[nSelectIndex];
            break;
        case CPlayer::CUSTOM_PARTS_WEP:
            nRightParts = m_anMemoryPartsWep[nSelectIndex];
            break;
        }

        // �E�[
        if (nRightParts == NOT_EXIST)
        {
            nSelectIndex = 0;
        }
    }
    else
    {
        nSelectIndex--;

        // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
        if (nSelectIndex < 0)
        {
            for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
            {
                // �E�[�̃p�[�c�𕔈ʂ��ƂɒT��
                int nRightParts = 0;
                switch (nNumWhere)
                {
                case CPlayer::CUSTOM_PARTS_HEAD:
                    nRightParts = m_anMemoryPartsHead[nCnt];
                    break;
                case CPlayer::CUSTOM_PARTS_UP:
                    nRightParts = m_anMemoryPartsUp[nCnt];
                    break;
                case CPlayer::CUSTOM_PARTS_DOWN:
                    nRightParts = m_anMemoryPartsDown[nCnt];
                    break;
                case CPlayer::CUSTOM_PARTS_WEP:
                    nRightParts = m_anMemoryPartsWep[nCnt];
                    break;
                }

                // ���̃p�[�c�����݂��Ă��Ȃ��Ȃ�A��O���E�[
                if (nRightParts == NOT_EXIST)
                {
                    nSelectIndex = nCnt - 1;

                    if (nSelectIndex < 0)
                    {
                        nSelectIndex = 0;
                    }

                    // ��������Afor�𔲂���
                    break;
                }
            }
        }
    }

    // �ۑ�����p�[�c�ԍ����擾�A�e�L�X�g�ƑI���̃C���f�b�N�X���X�V
    int nNumSaveParts = 0;
    switch (nNumWhere)
    {
    case CPlayer::CUSTOM_PARTS_HEAD:
        nNumSaveParts = m_anMemoryPartsHead[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Head->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectHead = nSelectIndex;
        break;
    case CPlayer::CUSTOM_PARTS_UP:
        nNumSaveParts = m_anMemoryPartsUp[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Up->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectUp = nSelectIndex;
        break;
    case CPlayer::CUSTOM_PARTS_DOWN:
        nNumSaveParts = m_anMemoryPartsDown[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Down->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectDown = nSelectIndex;
        break;
    case CPlayer::CUSTOM_PARTS_WEP:
        nNumSaveParts = m_anMemoryPartsWep[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Wep->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectWep = nSelectIndex;
        // ���������
        m_aEntryInfo[nNumWho].pPlayer->SetCustomWeaponLook();
        break;
    }

    // �p�[�c��ۑ�
    SaveCustom(nNumWho, nNumWhere, nNumSaveParts);

    // �v���C���[�������[�h
    m_aEntryInfo[nNumWho].pPlayer->LoadCustom();
}

//=============================================================================
// �J�X�^�}�C�Y�p�[�c�Z�[�u����
// Author : �㓡�T�V��
//=============================================================================
void CCustom::SaveCustom(int nNumSaveWho, int nNumSaveWhere, int nNumSaveParts)
{
    // �������ރp�[�c�̐�������
    int anNumParts[MAX_PARTS] = {};
    for (int nCntWhere = 0; nCntWhere < MAX_PARTS; nCntWhere++)
    {
        // �Z�[�u����p�[�c�̉ӏ��́A�Z�[�u����p�[�c�̔ԍ�����
        if (nCntWhere == nNumSaveWhere)
        {
            anNumParts[nCntWhere] = nNumSaveParts;
            continue;
        }

        // ����Z�[�u����p�[�c�̉ӏ��ȊO�̂��̂́A���̏������̂܂܏������ނ��߁A�擾
        if (m_aEntryInfo[nNumSaveWho].pPlayer)
        {
            anNumParts[nCntWhere] = m_aEntryInfo[nNumSaveWho].pPlayer->GetCustomPartsNum(nCntWhere);
        }
    }

    // �t�@�C���|�C���^
    FILE *pFile = NULL;

    // �J�X�^�}�C�Y�f�[�^�̃t�@�C�����J��
    switch (nNumSaveWho)
    {
    case PLAYER_1:
        pFile = fopen("data/TXT/custom1.txt", "w");
        break;
    case PLAYER_2:
        pFile = fopen("data/TXT/custom2.txt", "w");
        break;
    case PLAYER_3:
        pFile = fopen("data/TXT/custom3.txt", "w");
        break;
    case PLAYER_4:
        pFile = fopen("data/TXT/custom4.txt", "w");
        break;
    }

    // �t�@�C�����J������
    if (pFile != NULL)
    {
        for (int nCntWhere = 0; nCntWhere < MAX_PARTS; nCntWhere++)
        {
            fprintf(pFile, "%d\n", anNumParts[nCntWhere]);
        }

        // �t�@�C�������
        fclose(pFile);
    }
}

//=============================================================================
// �I�X�X�������iSaveCustom��Init�����킳���������j
// Author : �㓡�T�V��
//=============================================================================
void CCustom::DoOsusume(int nNumWho)
{
    // �������ރp�[�c�̐�������
    int anNumParts[MAX_PARTS] = {};
    int nRand = 0;

	//�A���œ������̂��o�Ȃ��悤�ɂ��鏈��
	do
	{
		nRand = GetRandNum(7, 1);
	} while (nRand == m_aEntryInfo[nNumWho].nOsusumeOld);
	m_aEntryInfo[nNumWho].nOsusumeOld = nRand;

    // �������ߑ����͐����X�V
    switch (nRand)
    {
    case 1:
        anNumParts[0] = 0;
        anNumParts[1] = 1;
        anNumParts[2] = 2;
        anNumParts[3] = 3;
        break;
    case 2:
        anNumParts[0] = 4;
        anNumParts[1] = 5;
        anNumParts[2] = 6;
        anNumParts[3] = 7;
        break;
    case 3:
        anNumParts[0] = 8;
        anNumParts[1] = 9;
        anNumParts[2] = 10;
        anNumParts[3] = 11;
        break;
	case 4:
		anNumParts[0] = 12;
		anNumParts[1] = 13;
		anNumParts[2] = 14;
		anNumParts[3] = 26;
		break;
    case 5:
        anNumParts[0] = 15;
        anNumParts[1] = 16;
        anNumParts[2] = 17;
        anNumParts[3] = 18;
        break;
	case 6:
		anNumParts[0] = 19;
		anNumParts[1] = 20;
		anNumParts[2] = 21;
		anNumParts[3] = 22;
		break;
	case 7:
		anNumParts[0] = 23;
		anNumParts[1] = 24;
		anNumParts[2] = 25;
		anNumParts[3] = 22;
		break;
    }

    // �J�X�^�}�C�Y�f�[�^�̃t�@�C�����J��
    FILE *pFile = NULL;
    switch (nNumWho)
    {
    case PLAYER_1:
        pFile = fopen("data/TXT/custom1.txt", "w");
        break;
    case PLAYER_2:
        pFile = fopen("data/TXT/custom2.txt", "w");
        break;
    case PLAYER_3:
        pFile = fopen("data/TXT/custom3.txt", "w");
        break;
    case PLAYER_4:
        pFile = fopen("data/TXT/custom4.txt", "w");
        break;
    }

    // �t�@�C�����J������A�����o�^
    if (pFile != NULL)
    {
        for (int nCntWhere = 0; nCntWhere < MAX_PARTS; nCntWhere++)
        {
            fprintf(pFile, "%d\n", anNumParts[nCntWhere]);
        }

        // �t�@�C�������
        fclose(pFile);
    }

    // �v���C���[�������[�h
    m_aEntryInfo[nNumWho].pPlayer->LoadCustom();

    // �I���J�[�\���ƃe�L�X�g�����킹��
    int nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_HEAD);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsHead[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectHead = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Head->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }

    nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_UP);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsUp[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectUp = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Up->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }

    nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_DOWN);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsDown[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectDown = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Down->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }

    nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_WEP);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsWep[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectWep = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Wep->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }
}
