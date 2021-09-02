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
#include "effect2d.h"

//=============================================================================
// �^�C�g���̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CTitle::CTitle()
{
    m_nCntTime = 0;
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
    // ���G�t�F�N�g���㉺�ɔ���
    EmitFire(FIRE_POS_UP);
    EmitFire(FIRE_POS_DOWN);

    // �^�C�g���̎��Ԃ��J�E���g
    m_nCntTime++;

    // �^�C�g���̍ő厞��
    if (m_nCntTime > TITLE_MAX_TIME)
    {
        m_nCntTime = TITLE_MAX_TIME;
    }

    // ��莞�Ԍo�߂őJ�ډ\��
    if (m_nCntTime >= TITLE_SHOW_TIME)
    {
        //�L�[�{�[�h�̊m�ۂ������������擾
        CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

        // �R���g���[�����擾
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
        DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

        // �G���^�[�A�܂��͉����{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START)
            || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_A)
            || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B)
            || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_X)
            || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_Y))
        {
            // ���ɃQ�[�����[�h�Ɉڍs
            CFade::SetFade(CManager::MODE_GAME);
        }
    }
}

//=============================================================================
// ��ʂ̏ォ���ɉ����o������
// Author : �㓡�T�V��
//=============================================================================
void CTitle::EmitFire(FIRE_POS firePos)
{
    // ���G�t�F�N�g����
    const float ANGLE_ADJUST = 90.0f;
    CEffect2D::Create(CEffectData::TYPE_TITLE_FIRE_RED, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXToRadian(247.5f));
    CEffect2D::Create(CEffectData::TYPE_TITLE_FIRE_RED, D3DXVECTOR3(SCREEN_WIDTH, -50.0f, 0.0f), D3DXToRadian(247.5f));
    CEffect2D::Create(CEffectData::TYPE_TITLE_FIRE_BLUE, D3DXVECTOR3(0.0f, SCREEN_HEIGHT + 50.0f, 0.0f), D3DXToRadian(67.5f));
}
