//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : �㓡�T�V��
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "game.h"
#include "debug.h"
//#include "effect.h"

//=============================================================================
// �^�C�g���̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CTitle::CTitle()
{
    m_nCntTime = 0;
    m_bNextScene = false;
}

//=============================================================================
// �^�C�g���̃f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CTitle::Init(void)
{
    // �}�E�X�J�[�\���̕\��(�O�̂���)
    ShowCursor(TRUE);

    // UI�𐶐�
    CUI::Place(CUI::SET_TITLE);

    //// BGM���Đ�
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_TITLE);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CTitle::Uninit(void)
{
    //// BGM���~(��)
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_TITLE);
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CTitle::Update(void)
{
    // �^�C�g���̎��Ԃ��J�E���g
    m_nCntTime++;

    // �^�C�g���̍ő厞��
    if (m_nCntTime > TITLE_MAX_TIME)
    {
        m_nCntTime = TITLE_MAX_TIME;
    }

    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // �J�ڂ̃t���O��true�Ȃ�
    if (m_bNextScene == true)
    {
        // �G���^�[�A�܂��̓X�^�[�g�{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // �t�F�[�h���Ă��Ȃ��Ȃ�
            if (CFade::GetFade() == CFade::FADE_NONE)
            {
                //// ���艹�Đ�
                //CSound *pSound = CManager::GetSound();
                //pSound->Play(CSound::LABEL_SE_SYSTEM_MODE_CHANGE);

                //// �V�ѕ����[�h�Ɉڍs
                //CFade::SetFade(CManager::MODE_MANUAL);

                // ���ɃQ�[�����[�h�Ɉڍs
                CFade::SetFade(CManager::MODE_GAME);
            }
        }
    }
    // �J�ڂ̃t���O��false�Ȃ�
    else
    {
        // ��莞�Ԍo�߂őJ�ډ\��
        if (m_nCntTime >= TITLE_NEXT_SCENE)
        {
            m_bNextScene = true;
        }
        // ��莞�Ԍo�߂��Ă��Ȃ��Ă�
        else
        {
            // �G���^�[�A�܂��̓X�^�[�g�{�^������������
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
            {
                // �J�ډ\��
                m_bNextScene = true;
            }
        }
    }

#ifdef _DEBUG
    CDebug::TitleCommand();
#endif
}
