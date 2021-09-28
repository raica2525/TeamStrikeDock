//=============================================================================
//
// ���j���[���� [menu.cpp]
// Author : �㓡�T�V��
//
//=============================================================================
#include "menu.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "debug.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// ���j���[�̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CMenu::CMenu()
{
    m_nSelectCoolTime = 0;
}

//=============================================================================
// ���j���[�̃f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CMenu::~CMenu()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CMenu::Init(void)
{
    // UI�𐶐�
    CUI::Place(CUI::SET_MENU);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CMenu::Uninit(void)
{
    // �m�ۂ���UI�̃������́ACScene��ReleaseAll�ŊJ������Ă���

    //// BGM���~
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_TITLE);
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CMenu::Update(void)
{
    ////�L�[�{�[�h�̊m�ۂ������������擾
    //CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    //// �R���g���[�����擾
    //CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    //DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    //// �I���̃N�[���^�C���𐔂���
    //if (m_nSelectCoolTime > 0)
    //{
    //    m_nSelectCoolTime--;
    //}

    //// �t�F�[�h���Ă��Ȃ��Ȃ�
    //if (CFade::GetFade() == CFade::FADE_NONE)
    //{
    //    switch (m_menu)
    //    {
    //    case MENU_001:

    //        // �e�N�X�`�����W���X�V
    //        m_pMenuUI->SetTexturePlace(0, 4);

    //        // �ړ��L�[����������(�E)
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputKeyboard->GetKeyboardTrigger(DIK_D) || pInputKeyboard->GetKeyboardTrigger(DIK_RIGHT) ||
    //            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START) ||
    //            Controller.lX > 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// �I��
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_002;

    //            // �I���̃N�[���^�C����ݒ�
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        break;

    //    case MENU_002:

    //        // �e�N�X�`�����W���X�V
    //        m_pMenuUI->SetTexturePlace(1, 4);

    //        // �ړ��L�[����������(��)
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_A) || pInputKeyboard->GetKeyboardTrigger(DIK_LEFT) ||
    //            Controller.lX < 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// �I��
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_001;

    //            // �I���̃N�[���^�C����ݒ�
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }
    //        // �ړ��L�[����������(�E)
    //        else if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputKeyboard->GetKeyboardTrigger(DIK_D) || pInputKeyboard->GetKeyboardTrigger(DIK_RIGHT) ||
    //            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START) ||
    //            Controller.lX > 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// �I��
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_003;

    //            // �I���̃N�[���^�C����ݒ�
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        break;

    //    case MENU_003:

    //        // �e�N�X�`�����W���X�V
    //        m_pMenuUI->SetTexturePlace(2, 4);

    //        // �ړ��L�[����������(��)
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_A) || pInputKeyboard->GetKeyboardTrigger(DIK_LEFT) ||
    //            Controller.lX < 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// �I��
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_002;

    //            // �I���̃N�[���^�C����ݒ�
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        // ����L�[����������
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START) ||
    //            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B))
    //        {
    //            //// �Q�[�����[�h�Ɉڍs
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_MODE_CHANGE);
    //            //CFade::SetFade(CManager::MODE_GAME);
    //        }

    //        break;
    //    }
    //}
}
