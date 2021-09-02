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

    m_pUI_Cursor = NULL;
    m_nSelectCoolTime = 0;
    m_select = SELECT_1P_HEAD;
    m_nNumSelectHead = 0;
    m_nNumSelectUp = 0;  
    m_nNumSelectDown = 0;
    m_nNumSelectWep = 0; 
    m_pText_Head = NULL;
    m_pText_Up = NULL;
    m_pText_Down = NULL;
    m_pText_Wep = NULL;
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

    // UI�𐶐�
    CUI::Place(CUI::SET_CUSTOM);

    m_pUI_Cursor = CUI::Create(19, DEFAULT_VECTOR, D3DXVECTOR3(35.0f, 35.0f, 0.0f), 0, DEFAULT_COLOR);

    //// BGM���Đ�
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_RESULT);

    // �v���C���[�̐���
    m_apPlayer[0] = CPlayer::CreateInCustom(D3DXVECTOR3(-300.0f, 300.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_001);

    // �p�[�c�I���J�[�\���̏�����
    int nPartNum = m_apPlayer[0]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_HEAD);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anPartsHead[nCntEachParts] == nPartNum)
        {
            m_nNumSelectHead = nCntEachParts;
        }
    }

    nPartNum = m_apPlayer[0]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_UP);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anPartsUp[nCntEachParts] == nPartNum)
        {
            m_nNumSelectUp = nCntEachParts;
        }
    }

    nPartNum = m_apPlayer[0]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_DOWN);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anPartsDown[nCntEachParts] == nPartNum)
        {
            m_nNumSelectDown = nCntEachParts;
        }
    }

    nPartNum = m_apPlayer[0]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_WEP);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anPartsWep[nCntEachParts] == nPartNum)
        {
            m_nNumSelectWep = nCntEachParts;
        }
    }

    // �J�����̃��b�N�I���ꏊ��ς���
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    // �e�L�X�g��ݒ�
    m_pText_Head = CText::Create(D3DXVECTOR3(200.0f, 250.0f, 0.0f), 30, CManager::GetModelData()->CModelData::GetPartsList((m_anPartsHead[m_nNumSelectHead]))->cName,
        CText::ALIGN_CENTER, "Reggae One", D3DCOLOR_RGBA(255, 255, 255, 255));
    m_pText_Up = CText::Create(D3DXVECTOR3(200.0f, 300.0f, 0.0f), 30, CManager::GetModelData()->CModelData::GetPartsList((m_anPartsUp[m_nNumSelectUp]))->cName,
        CText::ALIGN_CENTER, "Reggae One", D3DCOLOR_RGBA(255, 255, 255, 255));
    m_pText_Down = CText::Create(D3DXVECTOR3(200.0f, 350.0f, 0.0f), 30, CManager::GetModelData()->CModelData::GetPartsList((m_anPartsDown[m_nNumSelectDown]))->cName,
        CText::ALIGN_CENTER, "Reggae One", D3DCOLOR_RGBA(255, 255, 255, 255));
    m_pText_Wep = CText::Create(D3DXVECTOR3(200.0f, 400.0f, 0.0f), 30, CManager::GetModelData()->CModelData::GetPartsList((m_anPartsWep[m_nNumSelectWep]))->cName,
        CText::ALIGN_CENTER, "Reggae One", D3DCOLOR_RGBA(255, 255, 255, 255));

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
    // �I���̃N�[���^�C���𐔂���
    if (m_nSelectCoolTime > 0)
    {
        m_nSelectCoolTime--;
    }

    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 ControllerP1 = pInputJoypad->GetController(PLAYER_1);
    float fStickAngleP1 = 0.0f;
    bool bTiltedStickP1 = false;

    // ���X�e�B�b�N���X���Ă��邩�ǂ���
    if (ControllerP1.lY != 0 || ControllerP1.lX != 0)
    {
        bTiltedStickP1 = true;

        // �p�x�����߂�
        fStickAngleP1 = atan2(ControllerP1.lX, ControllerP1.lY*-1);
    }
    else
    {
        m_nSelectCoolTime = 0;
    }

    // �t�F�[�h���Ă��Ȃ��Ȃ�A�I���\
    if (CFade::GetFade() == CFade::FADE_NONE)
    {
        // �E�܂��͍��������ꂽ��
        if (pInputKeyboard->GetKeyboardTrigger(DIK_D) || pInputKeyboard->GetKeyboardTrigger(DIK_A) ||
            bTiltedStickP1 && STICK_RIGHT(fStickAngleP1) && m_nSelectCoolTime <= 0 
            || bTiltedStickP1 && STICK_LEFT(fStickAngleP1) && m_nSelectCoolTime <= 0)
        {
            bool bRight = false;
            if (pInputKeyboard->GetKeyboardTrigger(DIK_D) || bTiltedStickP1 && STICK_RIGHT(fStickAngleP1) && m_nSelectCoolTime <= 0)
            {
                bRight = true;
            }

            // �p�[�c��؂�ւ�
            switch (m_select)
            {
            case SELECT_1P_HEAD:
                if (bRight)
                {
                    m_nNumSelectHead++;

                    // �E�[
                    if (m_anPartsHead[m_nNumSelectHead] == NOT_EXIST)
                    {
                        m_nNumSelectHead = 0;
                    }
                }
                else
                {
                    m_nNumSelectHead--;

                    // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
                    if (m_nNumSelectHead < 0)
                    {
                        for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
                        {
                            if (m_anPartsHead[nCnt] == NOT_EXIST)
                            {
                                m_nNumSelectHead = nCnt - 1;

                                if (m_nNumSelectHead < 0)
                                {
                                    m_nNumSelectHead = 0;
                                }

                                break;
                            }
                        }
                    }
                }
                SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_HEAD, m_anPartsHead[m_nNumSelectHead]);
                m_pText_Head->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsHead[m_nNumSelectHead]))->cName);
                break;

            case SELECT_1P_UP:
                if (bRight)
                {
                    m_nNumSelectUp++;

                    // �E�[
                    if (m_anPartsUp[m_nNumSelectUp] == NOT_EXIST)
                    {
                        m_nNumSelectUp = 0;
                    }
                }
                else
                {
                    m_nNumSelectUp--;

                    // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
                    if (m_nNumSelectUp < 0)
                    {
                        for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
                        {
                            if (m_anPartsUp[nCnt] == NOT_EXIST)
                            {
                                m_nNumSelectUp = nCnt - 1;

                                if (m_nNumSelectUp < 0)
                                {
                                    m_nNumSelectUp = 0;
                                }

                                break;
                            }
                        }
                    }
                }
                SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_UP, m_anPartsUp[m_nNumSelectUp]);
                m_pText_Up->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsUp[m_nNumSelectUp]))->cName);
                break;

            case SELECT_1P_DOWN:
                if (bRight)
                {
                    m_nNumSelectDown++;

                    // �E�[
                    if (m_anPartsDown[m_nNumSelectDown] == NOT_EXIST)
                    {
                        m_nNumSelectDown = 0;
                    }
                }
                else
                {
                    m_nNumSelectDown--;

                    // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
                    if (m_nNumSelectDown < 0)
                    {
                        for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
                        {
                            if (m_anPartsDown[nCnt] == NOT_EXIST)
                            {
                                m_nNumSelectDown = nCnt - 1;

                                if (m_nNumSelectDown < 0)
                                {
                                    m_nNumSelectDown = 0;
                                }

                                break;
                            }
                        }
                    }
                }
                SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_DOWN, m_anPartsDown[m_nNumSelectDown]);
                m_pText_Down->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsDown[m_nNumSelectDown]))->cName);
                break;

            case SELECT_1P_WEP:
                if (bRight)
                {
                    m_nNumSelectWep++;

                    // �E�[
                    if (m_anPartsWep[m_nNumSelectWep] == NOT_EXIST)
                    {
                        m_nNumSelectWep = 0;
                    }
                }
                else
                {
                    m_nNumSelectWep--;

                    // ���[�i���݂����ԉE�̃p�[�c�ɐ؂�ւ���j
                    if (m_nNumSelectWep < 0)
                    {
                        for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
                        {
                            if (m_anPartsWep[nCnt] == NOT_EXIST)
                            {
                                m_nNumSelectWep = nCnt - 1;

                                if (m_nNumSelectWep < 0)
                                {
                                    m_nNumSelectWep = 0;
                                }

                                break;
                            }
                        }
                    }
                }
                SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_WEP, m_anPartsWep[m_nNumSelectWep]);
                m_pText_Wep->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsWep[m_nNumSelectWep]))->cName);
                break;
            }

            // �I���̃N�[���^�C����ݒ�
            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;

            // �v���C���[�������[�h
            m_apPlayer[0]->LoadCustom();
        }
        else
        {
            // ��ړ�
            if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || bTiltedStickP1 && STICK_UP(fStickAngleP1) && m_nSelectCoolTime <= 0)
            {
                // ��̑I������
                m_select--;

                // ��ԏ�̑I�����Ȃ�A��ԉ���
                if (m_select < SELECT_1P_HEAD)
                {
                    m_select = SELECT_MAX - 1;
                }

                // �I���̃N�[���^�C����ݒ�
                m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
            }

            // ���ړ�
            if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || bTiltedStickP1 && m_nSelectCoolTime <= 0)
            {
                // STICK_DOWN�̒�`���A�܂��� �Ƒg�ݍ��킹����������
                bool bDown = true;
                if (bTiltedStickP1)
                {
                    if (STICK_DOWN(fStickAngleP1))
                    {
                        bDown = true;
                    }
                    else
                    {
                        bDown = false;
                    }
                }

                // �_�E������t���O��true�Ȃ�
                if (bDown)
                {
                    // ���̑I������
                    m_select++;

                    // ��ԉ��̑I�����Ȃ�A��ԏ��
                    if (m_select >= SELECT_MAX)
                    {
                        m_select = SELECT_1P_HEAD;
                    }

                    // �I���̃N�[���^�C����ݒ�
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }
            }
        }
    }

    // ����L�[�Ń^�C�g����
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        // �^�C�g�����[�h�Ɉڍs
        CFade::SetFade(CManager::MODE_TITLE);
    }

    // �J�[�\���̈ʒu��ς���
    D3DXVECTOR3 pos = D3DXVECTOR3(150.0f, 200.0f, 0.0f);
    float fDigitPosY = 50.0f * m_select;
    pos.y += fDigitPosY;
    if (m_pUI_Cursor)
    {
        m_pUI_Cursor->SetPosition(pos);
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