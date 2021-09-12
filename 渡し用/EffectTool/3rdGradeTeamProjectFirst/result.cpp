//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : �㓡�T�V��
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "game.h"
#include "ranking.h"
#include "debug.h"

//========================================
// �}�N����`
//========================================

// �e���ʂɁA�J�E���^�𗘗p���ēG��z�u���Ă���
#define ENEMY_5 5
#define ENEMY_4 95
#define ENEMY_3 215
#define ENEMY_2 335
#define ENEMY_1 455
#define STOP 1000       // �J�E���^�̃X�g�b�v

#define SET_POS_X 1400  // �z�u��X���W

//=============================================================================
// ���U���g�̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// ���U���g�̃f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CResult::Init(void)
{
    // �}�E�X�J�[�\���̕\��
    ShowCursor(TRUE);

    // UI�𐶐�
    CUI::Place(CUI::SET_RESULT);

    // �����L���O�𐶐�
    CRanking::Create(RANKING_FIRST_POS, DEFAULT_VECTOR);

    //// BGM���Đ�
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_RESULT);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CResult::Uninit(void)
{
    //// BGM���~
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_RESULT);
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CResult::Update(void)
{
    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // ����L�[�Ń^�C�g����
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        // �t�F�[�h���Ă��Ȃ��Ȃ�
        if (CFade::GetFade() == CFade::FADE_NONE)
        {
            //// ���艹�Đ�
            //CSound *pSound = CManager::GetSound();
            //pSound->Play(CSound::LABEL_SE_SYSTEM_MODE_CHANGE);

            // �^�C�g�����[�h�Ɉڍs
            CFade::SetFade(CManager::MODE_TITLE);
        }
    }

#ifdef _DEBUG
    CDebug::ResultCommand();
#endif
}