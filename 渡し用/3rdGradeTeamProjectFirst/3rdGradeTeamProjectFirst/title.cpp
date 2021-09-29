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
    m_bEmitWingEffect = true;
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
    // ���G�t�F�N�g���㉺�ɔ���(6�񕪂ł��傤�ǂ悭�Ȃ���)
    for (int nCnt = 0; nCnt < 6; nCnt++)
    {
        EmitFire(FIRE_POS_UP);
        EmitFire(FIRE_POS_DOWN);
    }

    // �^�C�g���̎��Ԃ��J�E���g
    m_nCntTime++;

    // ��莞�Ԍo�߂őJ�ډ\��
    if (m_nCntTime >= TITLE_SHOW_TIME)
    {
        // �J�E���^�X�g�b�v
        m_nCntTime = TITLE_SHOW_TIME;

        // ���G�t�F�N�g����o
        if (m_bEmitWingEffect)
        {
            m_bEmitWingEffect = false;
            CEffect2D::Emit(CEffectData::TYPE_RED_WING, D3DXVECTOR3(640.0f, 160.0f,0.0f), D3DXVECTOR3(640.0f, 360.0f, 0.0f));
            CEffect2D::Emit(CEffectData::TYPE_BLUE_WING, D3DXVECTOR3(640.0f, 160.0f, 0.0f), D3DXVECTOR3(640.0f, 360.0f, 0.0f));
        }

        // �v���X�{�^����UI���o��������
        CUI *pPressButton = CUI::GetAccessUI(0);
        if (pPressButton)
        {
            pPressButton->SetActionLock(1, false);
        }

        // �t�F�[�h���Ă��Ȃ��Ȃ�A�J�ډ\
        if (CFade::GetFade() == CFade::FADE_NONE)
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
                // ���ɃJ�X�^�}�C�Y��ʂɈڍs
                CFade::SetFade(CManager::MODE_CUSTOM);

                // �v���X�{�^����_�ł�����
                if (pPressButton)
                {
                    // �F�ς������Z�b�g���ă��b�N����
                    pPressButton->SetActionReset(0);
                    pPressButton->SetActionLock(0, true);

                    // �_�ł��A�����b�N
                    pPressButton->SetActionLock(2, false);
                }
            }
        }
    }
}

//=============================================================================
// ��ʂ̏ォ���ɉ����o������
// Author : �㓡�T�V��
//=============================================================================
void CTitle::EmitFire(FIRE_POS firePos)
{
    // �ϐ��錾
    const float ANGLE_ADJUST = 90.0f;   // ���v�ł���0��0�x�̎��ɍ��킹��i2D�|���S���̒��_��Z�Ŏw�肵�Ă��邩��j
    const float POS_Y_ADJUST = 50.0f;
    const int RAND_X = 1380;
    const float BASE_ANGLE = 67.5f;
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    float fAngle = 0.0f;
    CEffectData::TYPE effectType = CEffectData::TYPE_TITLE_FIRE_RED;

    // �ʒu�ƌ����ƐF�����߂�
    switch (firePos)
    {
    case FIRE_POS_UP:
        pos.y = -POS_Y_ADJUST;
        pos.x = float(rand() % RAND_X);
        fAngle = D3DXToRadian(180.0f) + BASE_ANGLE - ANGLE_ADJUST;
        effectType = CEffectData::TYPE_TITLE_FIRE_BLUE;
        break;
    case FIRE_POS_DOWN:
        pos.y = SCREEN_HEIGHT + POS_Y_ADJUST;
        pos.x = float(rand() % RAND_X) - float(RAND_X - SCREEN_WIDTH);
        fAngle = BASE_ANGLE - ANGLE_ADJUST;
        effectType = CEffectData::TYPE_TITLE_FIRE_RED;
        break;
    }

    // ������
    CEffect2D::Create(effectType, pos, fAngle);
}
