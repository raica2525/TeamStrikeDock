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
#include "debug.h"
#include "camera.h"
#include "effect2d.h"
#include "effectData.h"

//========================================
// �}�N����`
//========================================

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
    // UI�𐶐�
    CUI::Place(CUI::SET_RESULT);

    // �v���C���[����
    for (int nCntPlayer = 0; nCntPlayer < CGame::GetNumAllPlayer(); nCntPlayer++)
    {
        switch (nCntPlayer)
        {
        case CPlayer::RANK_1:
            CPlayer::CreateInResult(D3DXVECTOR3(250.0f, 200.0f, -1200.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_1);
            break;
        case CPlayer::RANK_2:
            CPlayer::CreateInResult(D3DXVECTOR3(-640.0f, 100.0f, -600.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_2);
            break;
        case CPlayer::RANK_3:
            CPlayer::CreateInResult(D3DXVECTOR3(-325.0f, 25.0f, -310.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_3);
            break;
        case CPlayer::RANK_4:
            CPlayer::CreateInResult(D3DXVECTOR3(125.0f, -40.0f, -100.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_4);
            break;
        }
    }

    // �J�����̃��b�N�I���ꏊ��ς���
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    // BGM���Đ�
    CManager::SoundPlay(CSound::LABEL_BGM_RESULT);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CResult::Uninit(void)
{
    // BGM���~
    CManager::SoundStop(CSound::LABEL_BGM_RESULT);
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CResult::Update(void)
{
    // �ϐ��錾
    const float ANGLE_ADJUST = 90.0f;   // ���v�ł���0��0�x�̎��ɍ��킹��i2D�|���S���̒��_��Z�Ŏw�肵�Ă��邩��j
    const float POS_Y_ADJUST = 50.0f;
    const int RAND_X = 1380;
    const float BASE_ANGLE = 67.5f;
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    float fAngle = 0.0f;
    CEffectData::TYPE effectType = CEffectData::TYPE_KAMI;

    // �ʒu�����߂�
    pos.y = -POS_Y_ADJUST;
    pos.x = float(rand() % RAND_X);
    fAngle = D3DXToRadian(180.0f) + BASE_ANGLE - ANGLE_ADJUST;

    // �����ᐶ��
    CEffect2D *pKami = CEffect2D::Create(effectType, pos, fAngle);
    float fRed = (float)GetRandNum(100, 0) / 100.0f;
    float fGreen = (float)GetRandNum(100, 0) / 100.0f;
    float fBlue = (float)GetRandNum(100, 0) / 100.0f;
    pKami->CEffect2D::SetCol(D3DXCOLOR(fRed, fGreen, fBlue, 1.0f));

    // �G���^�[�A�܂��͉����{�^������������
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_A)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_X)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_Y)
        || pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_START)
        || pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_A)
        || pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_B)
        || pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_X)
        || pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_Y)
        || pInputJoypad->GetJoypadTrigger(PLAYER_3, CInputJoypad::BUTTON_START)
        || pInputJoypad->GetJoypadTrigger(PLAYER_3, CInputJoypad::BUTTON_A)
        || pInputJoypad->GetJoypadTrigger(PLAYER_3, CInputJoypad::BUTTON_B)
        || pInputJoypad->GetJoypadTrigger(PLAYER_3, CInputJoypad::BUTTON_X)
        || pInputJoypad->GetJoypadTrigger(PLAYER_3, CInputJoypad::BUTTON_Y)
        || pInputJoypad->GetJoypadTrigger(PLAYER_4, CInputJoypad::BUTTON_START)
        || pInputJoypad->GetJoypadTrigger(PLAYER_4, CInputJoypad::BUTTON_A)
        || pInputJoypad->GetJoypadTrigger(PLAYER_4, CInputJoypad::BUTTON_B)
        || pInputJoypad->GetJoypadTrigger(PLAYER_4, CInputJoypad::BUTTON_X)
        || pInputJoypad->GetJoypadTrigger(PLAYER_4, CInputJoypad::BUTTON_Y))
    {
        // ���Ƀ^�C�g����ʂɈڍs
        CManager::SoundPlay(CSound::LABEL_SE_OK);
        CFade::SetFade(CManager::MODE_TITLE);
    }
}