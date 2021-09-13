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
#include "ui.h"
#include "fade.h"
#include "debug.h"
#include "player.h"
#include "camera.h"
#include "modelData.h"
#include "text.h"

//========================================
// �}�N����`
//========================================

//=============================================================================
// �J�X�^�}�C�Y��ʂ̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCustom::CCustom()
{
    memset(m_apPlayer, 0, sizeof(m_apPlayer));
    memset(m_anPartsHead, NOT_EXIST, sizeof(m_anPartsHead));
    memset(m_anPartsUp, NOT_EXIST, sizeof(m_anPartsUp));
    memset(m_anPartsDown, NOT_EXIST, sizeof(m_anPartsDown));
    memset(m_anPartsWep, NOT_EXIST, sizeof(m_anPartsWep));
    memset(m_aEntryInfo, 0, sizeof(m_aEntryInfo));
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
    m_apPlayer[PLAYER_1] = CPlayer::CreateInCustom(D3DXVECTOR3(-950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_001, true);
    m_apPlayer[PLAYER_2] = CPlayer::CreateInCustom(D3DXVECTOR3(-316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_002, false);
    m_apPlayer[PLAYER_3] = CPlayer::CreateInCustom(D3DXVECTOR3(316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_003, false);
    m_apPlayer[PLAYER_4] = CPlayer::CreateInCustom(D3DXVECTOR3(950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_004, false);

    // �J�[�\������
    const D3DXVECTOR3 cursorFirstPos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    m_aEntryInfo[PLAYER_1].pUI_Cursor = CUI::Create(53, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR);
    m_aEntryInfo[PLAYER_2].pUI_Cursor = CUI::Create(54, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);
    m_aEntryInfo[PLAYER_3].pUI_Cursor = CUI::Create(55, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);
    m_aEntryInfo[PLAYER_4].pUI_Cursor = CUI::Create(56, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);

    // �p�[�c�I�����̏�����
    float fTextPosX = 167.0f;
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        int nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_HEAD);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsHead[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectHead = nCntEachParts;
            }
        }

        nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_UP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsUp[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectUp = nCntEachParts;
            }
        }

        nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_DOWN);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsDown[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectDown = nCntEachParts;
            }
        }

        nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_WEP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsWep[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectWep = nCntEachParts;
            }
        }

        // �e�L�X�g��ݒ�
        D3DCOLOR color = TEXT_EXIST_COLOR;
        if (nCnt != PLAYER_1)
        {
            color = TEXT_NOT_EXIST_COLOR;
        }
        m_aEntryInfo[nCnt].pText_Head = CText::Create(D3DXVECTOR3(fTextPosX, 427.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsHead[m_aEntryInfo[nCnt].nNumSelectHead]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Up = CText::Create(D3DXVECTOR3(fTextPosX, 490.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsUp[m_aEntryInfo[nCnt].nNumSelectUp]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Down = CText::Create(D3DXVECTOR3(fTextPosX, 553.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsDown[m_aEntryInfo[nCnt].nNumSelectDown]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Wep = CText::Create(D3DXVECTOR3(fTextPosX, 616.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsWep[m_aEntryInfo[nCnt].nNumSelectWep]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);

        // �e�L�X�g�̉��ʒu�����Z
        fTextPosX += 315.0f;
    }

    // �J�����̃��b�N�I���ꏊ��ς���
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    // 1P�̃J�[�\���͕K���g��
    m_aEntryInfo[PLAYER_1].bUseCursor = true;

    //// BGM���Đ�
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_RESULT);

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
                if (m_anPartsHead[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsHead[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_UP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anPartsUp[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsUp[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_DOWN:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anPartsDown[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsDown[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_WEP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anPartsWep[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsWep[nCntEachParts] = nCntParts;
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
    //// BGM���~
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_RESULT);
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CCustom::Update(void)
{
    // �J�[�\���ړ�
    MoveCursor();

    //// �t�F�[�h���Ă��Ȃ��Ȃ�A�I���\
    //if (CFade::GetFade() == CFade::FADE_NONE)
    //{
    //    // �E�܂��͍��������ꂽ��
    //    if (pInputKeyboard->GetKeyboardTrigger(DIK_D) || pInputKeyboard->GetKeyboardTrigger(DIK_A) ||
    //        bTiltedStickP1 && STICK_RIGHT(fStickAngleP1)
    //        || bTiltedStickP1 && STICK_LEFT(fStickAngleP1))
    //    {
    //        bool bRight = false;
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_D) || bTiltedStickP1 && STICK_RIGHT(fStickAngleP1) && m_nSelectCoolTime <= 0)
    //        {
    //            bRight = true;
    //        }

    //        // �p�[�c��؂�ւ�
    //        switch (m_select)
    //        {
    //        case SELECT_1P_HEAD:
    //            if (bRight)
    //            {
    //                m_nNumSelectHead++;

    //                // �E�[
    //                if (m_anPartsHead[m_nNumSelectHead] == NOT_EXIST)
    //                {
    //                    m_nNumSelectHead = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectHead--;

    //                // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
    //                if (m_nNumSelectHead < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsHead[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectHead = nCnt - 1;

    //                            if (m_nNumSelectHead < 0)
    //                            {
    //                                m_nNumSelectHead = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_HEAD, m_anPartsHead[m_nNumSelectHead]);
    //            m_pText_Head->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsHead[m_nNumSelectHead]))->cName);
    //            break;

    //        case SELECT_1P_UP:
    //            if (bRight)
    //            {
    //                m_nNumSelectUp++;

    //                // �E�[
    //                if (m_anPartsUp[m_nNumSelectUp] == NOT_EXIST)
    //                {
    //                    m_nNumSelectUp = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectUp--;

    //                // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
    //                if (m_nNumSelectUp < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsUp[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectUp = nCnt - 1;

    //                            if (m_nNumSelectUp < 0)
    //                            {
    //                                m_nNumSelectUp = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_UP, m_anPartsUp[m_nNumSelectUp]);
    //            m_pText_Up->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsUp[m_nNumSelectUp]))->cName);
    //            break;

    //        case SELECT_1P_DOWN:
    //            if (bRight)
    //            {
    //                m_nNumSelectDown++;

    //                // �E�[
    //                if (m_anPartsDown[m_nNumSelectDown] == NOT_EXIST)
    //                {
    //                    m_nNumSelectDown = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectDown--;

    //                // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
    //                if (m_nNumSelectDown < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsDown[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectDown = nCnt - 1;

    //                            if (m_nNumSelectDown < 0)
    //                            {
    //                                m_nNumSelectDown = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_DOWN, m_anPartsDown[m_nNumSelectDown]);
    //            m_pText_Down->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsDown[m_nNumSelectDown]))->cName);
    //            break;

    //        case SELECT_1P_WEP:
    //            if (bRight)
    //            {
    //                m_nNumSelectWep++;

    //                // �E�[
    //                if (m_anPartsWep[m_nNumSelectWep] == NOT_EXIST)
    //                {
    //                    m_nNumSelectWep = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectWep--;

    //                // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
    //                if (m_nNumSelectWep < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsWep[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectWep = nCnt - 1;

    //                            if (m_nNumSelectWep < 0)
    //                            {
    //                                m_nNumSelectWep = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_WEP, m_anPartsWep[m_nNumSelectWep]);
    //            m_pText_Wep->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsWep[m_nNumSelectWep]))->cName);
    //            break;
    //        }

    //        // �I���̃N�[���^�C����ݒ�
    //        m_nSelectCoolTime = MENU_SELECT_COOL_TIME;

    //        // �v���C���[�������[�h
    //        m_apPlayer[0]->LoadCustom();
    //    }
    //    else
    //    {
    //        // ��ړ�
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || bTiltedStickP1 && STICK_UP(fStickAngleP1) && m_nSelectCoolTime <= 0)
    //        {
    //            // ��̑I������
    //            m_select--;

    //            // ��ԏ�̑I�����Ȃ�A��ԉ���
    //            if (m_select < SELECT_1P_HEAD)
    //            {
    //                m_select = SELECT_MAX - 1;
    //            }

    //            // �I���̃N�[���^�C����ݒ�
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        // ���ړ�
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || bTiltedStickP1 && m_nSelectCoolTime <= 0)
    //        {
    //            // STICK_DOWN�̒�`���A�܂��� �Ƒg�ݍ��킹����������
    //            bool bDown = true;
    //            if (bTiltedStickP1)
    //            {
    //                if (STICK_DOWN(fStickAngleP1))
    //                {
    //                    bDown = true;
    //                }
    //                else
    //                {
    //                    bDown = false;
    //                }
    //            }

    //            // �_�E������t���O��true�Ȃ�
    //            if (bDown)
    //            {
    //                // ���̑I������
    //                m_select++;

    //                // ��ԉ��̑I�����Ȃ�A��ԏ��
    //                if (m_select >= SELECT_MAX)
    //                {
    //                    m_select = SELECT_1P_HEAD;
    //                }

    //                // �I���̃N�[���^�C����ݒ�
    //                m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //            }
    //        }
    //    }
    //}

    //// ����L�[�Ń^�C�g����
    //if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    //{
    //    // �^�C�g�����[�h�Ɉڍs
    //    CFade::SetFade(CManager::MODE_TITLE);
    //}

    //// �J�[�\���̈ʒu��ς���
    //D3DXVECTOR3 pos = D3DXVECTOR3(167.0f, 443.0f, 0.0f);
    //float fDigitPosY = 63.0f * m_select;
    //pos.y += fDigitPosY;
    //if (m_pUI_Cursor)
    //{
    //    m_pUI_Cursor->SetPosition(pos);
    //}
}

//=============================================================================
// �J�[�\���ړ�
// Author : �㓡�T�V��
//=============================================================================
void CCustom::MoveCursor(void)
{
    for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
    {
        // �J�[�\�����g���Ȃ�
        if (m_aEntryInfo[nCntPlayer].bUseCursor)
        {
            // �J�[�\��������Ȃ�
            if (m_aEntryInfo[nCntPlayer].pUI_Cursor)
            {
                ////�L�[�{�[�h�̊m�ۂ������������擾
                //CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

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

                // �ʒu���擾
                D3DXVECTOR3 pos = m_aEntryInfo[nCntPlayer].pUI_Cursor->GetPosition();

                // �ړ�
                if (bTiltedStick)
                {
                    const float ADJUST_RATE = 0.0008f;   // �X�e�B�b�N�̌X���̒l���A�ʒu�ɑ�����悤����
                    pos.x += sinf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                    pos.y += -cosf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                }

                // �ʒu��ݒ�
                m_aEntryInfo[nCntPlayer].pUI_Cursor->SetPosition(pos);
            }
        }
    }
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
        if (m_apPlayer[nNumSaveWho])
        {
            anNumParts[nCntWhere] = m_apPlayer[nNumSaveWho]->GetCustomPartsNum(nCntWhere);
        }
    }

    // �t�@�C���|�C���^
    FILE *pFile = NULL;

    // �J�X�^�}�C�Y�f�[�^�̃t�@�C�����J��
    switch (nNumSaveWho)
    {
    case CPlayer::PLAYABLE_001:
        pFile = fopen("data/TXT/custom1.txt", "w");
        break;
    case CPlayer::PLAYABLE_002:
        pFile = fopen("data/TXT/custom2.txt", "w");
        break;
    case CPlayer::PLAYABLE_003:
        pFile = fopen("data/TXT/custom3.txt", "w");
        break;
    case CPlayer::PLAYABLE_004:
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