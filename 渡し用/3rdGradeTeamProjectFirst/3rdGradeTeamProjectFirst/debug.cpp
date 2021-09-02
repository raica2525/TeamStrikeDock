//===============================================
//
// �f�o�b�O����̏��� (debug.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "debug.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "game.h"
#include "modelData.h"
#include "player.h"
#include "effectData.h"
#include "custom.h"
#include "wave.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CDebug::CDebug() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nLife = 2;
    m_type = TYPE_PERMANENT;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CDebug::~CDebug()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CDebug::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CScene3D::Init(pos, size);

    SetScale(size);

    BindModelData(1);   // �����蔻�������1��

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Update(void)
{
    CScene3D::Update();

    // 1F���������郂�f���Ȃ�A���̓s�x����(�����ď������ׂ������A����ȏ�Q�[�����d���Ȃ�Ȃ��悤�ɍ��)
    if (m_type == TYPE_MOMENT)
    {
        m_nLife--;
    }

    if (m_nLife <= 0)
    {
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Draw(void)
{
    CScene3D::Draw();
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
void CDebug::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
#ifdef _DEBUG
    if (!CManager::GetRenderer()->GetDispFont())
    {
        return;
    }
#endif

    CDebug *pDebug = NULL;
    pDebug = new CDebug;
    pDebug->m_type = type;
    pDebug->Init(pos, size);
}

//=============================================================================
// UI�̃����[�h
// Author : �㓡�T�V��
//=============================================================================
#ifdef _DEBUG
void CDebug::ReloadUI(CUI::SET set)
{
    // �����[�h����UI���폜
    CScene::ReleaseReloadUI();

    // UI�̍Ĕz�u
    CUI::Place(set);
}

//=============================================================================
// ���ʂ̃f�o�b�O�R�}���h
// Author : �㓡�T�V��
//=============================================================================
void CDebug::CommonCommand(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F4�L�[��UI���̃����[�h
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // �ǂ�UI���������[�h���邩�A���݂̃��[�h�Ō��߂�
        CManager::MODE mode = CManager::GetMode();
        switch (mode)
        {
        case CManager::MODE_TITLE:
            ReloadUI(CUI::SET_TITLE);
            break;
        case CManager::MODE_CUSTOM:
            ReloadUI(CUI::SET_CUSTOM);
            break;
        case CManager::MODE_MANUAL:
            ReloadUI(CUI::SET_MANUAL);
            break;
        case CManager::MODE_RESULT:
            ReloadUI(CUI::SET_RESULT);
            break;
        case CManager::MODE_GAME:
            {
                // UI�̃����[�h
                ReloadUI(CUI::SET_GAME);

                // �v���C���[�̃p�[�c�ݒ�������[�h
                CModelData* pModelData = CManager::GetModelData();
                pModelData->LoadPartsList();
                pModelData->LoadPartsRate();
                for (int nCnt = 0; nCnt < CGame::GetNumAllPlayer(); nCnt++)
                {
                    CPlayer* pPlayer = CGame::GetPlayer(nCnt);
                    pPlayer->LoadCustom();
                }
                break;
            }
        }
        // �G�t�F�N�g�f�[�^�̃����[�h
        CEffectData* pEffectData = CManager::GetEffectData();
        pEffectData->Init();
    }

    // F8�L�[�̓f�o�b�O�\�L��ONOFF
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F8))
    {
        CRenderer::SwitchDispFont();
    }

    // F7�L�[�̓f�o�b�O���j���[�ɑJ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F7))
    {
        CFade::SetFade(CManager::MODE_DEBUG_MENU);
    }
}
#endif

//=============================================================================
// �f�o�b�O���j���[�̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CDebugMenu::CDebugMenu()
{
    m_pUI_Cursor = NULL;
    m_nSelectCoolTime = 0;
    m_select = SELECT_TRAINING;
}

//=============================================================================
// �f�o�b�O���j���[�̃f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CDebugMenu::~CDebugMenu()
{
}

//=============================================================================
// �f�o�b�O���j���[�̏���������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CDebugMenu::Init(void)
{
    m_pUI_Cursor = CUI::Create(19, DEFAULT_VECTOR, D3DXVECTOR3(20.0f, 20.0f, 0.0f), 0, DEFAULT_COLOR);

    return S_OK;
}

//=============================================================================
// �f�o�b�O���j���[�̏I������
// Author : �㓡�T�V��
//=============================================================================
void CDebugMenu::Uninit(void)
{
}

//=============================================================================
// �f�o�b�O���j���[�̍X�V����
// Author : �㓡�T�V��
//=============================================================================
void CDebugMenu::Update(void)
{
    // �I���̃N�[���^�C���𐔂���
    if (m_nSelectCoolTime > 0)
    {
        m_nSelectCoolTime--;
    }

    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // �t�F�[�h���Ă��Ȃ��Ȃ�A�I���\
    if (CFade::GetFade() == CFade::FADE_NONE)
    {
        // A�������ꂽ(����)
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_A))
        {
            // �I�񂾃��[�h�Ɉڍs
            switch (m_select)
            {
            case SELECT_TRAINING:
                CFade::SetFade(CManager::MODE_GAME);
                if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
                {
                    CGame::SetNextGame(CGame::TYPE_TRAINING, 1, 1, true);
                }
                else
                {
                    CGame::SetNextGame(CGame::TYPE_TRAINING, 1, 1);
                }
                CGame::SetAILevel(PLAYER_1, CPlayer::AI_LEVEL_NONE);
                break;
            case SELECT_CUSTOMIZE:
                CFade::SetFade(CManager::MODE_CUSTOM);
                break;
            case SELECT_TITLE:
                CFade::SetFade(CManager::MODE_TITLE);
                break;
            case SELECT_ARENA_1:
                CFade::SetFade(CManager::MODE_GAME);
                if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3, true);
                }
                else
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3);
                }
                CGame::SetAILevel(PLAYER_1, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_2, CPlayer::AI_LEVEL_NONE);
                break;
            case SELECT_ARENA_2:
                CFade::SetFade(CManager::MODE_GAME);
                if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3, true);
                }
                else
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3);
                }
                CGame::SetAILevel(PLAYER_1, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_2, CPlayer::AI_LEVEL_1);
                break;
            case SELECT_ARENA_3:
                CFade::SetFade(CManager::MODE_GAME);
                if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3, true);
                }
                else
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3);
                }
                CGame::SetAILevel(PLAYER_1, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_2, CPlayer::AI_LEVEL_2);
                break;
            case SELECT_ARENA_4:
                CFade::SetFade(CManager::MODE_GAME);
                if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3, true);
                }
                else
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 2, 3);
                }
                CGame::SetAILevel(PLAYER_1, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_2, CPlayer::AI_LEVEL_3);
                break;
            case SELECT_ARENA_5:
                CFade::SetFade(CManager::MODE_GAME);
                if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 4, 3, true);
                }
                else
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 4, 3);
                }
                CGame::SetAILevel(PLAYER_1, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_2, CPlayer::AI_LEVEL_1);
                CGame::SetAILevel(PLAYER_3, CPlayer::AI_LEVEL_2);
                CGame::SetAILevel(PLAYER_4, CPlayer::AI_LEVEL_3);
                break;
            case SELECT_ARENA_6:
                CFade::SetFade(CManager::MODE_GAME);
                if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 4, 3, true);
                }
                else
                {
                    CGame::SetNextGame(CGame::TYPE_ARENA, 4, 3);
                }
                CGame::SetAILevel(PLAYER_1, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_2, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_3, CPlayer::AI_LEVEL_NONE);
                CGame::SetAILevel(PLAYER_4, CPlayer::AI_LEVEL_NONE);
                break;
            }
        }
        else
        {
            // ��ړ�
            if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || Controller.lY < 0 && m_nSelectCoolTime <= 0)
            {
                // ��̑I������
                m_select--;

                // ��ԏ�̑I�����Ȃ�A��ԉ���
                if (m_select < SELECT_TRAINING)
                {
                    m_select = SELECT_MAX - 1;
                }

                // �I���̃N�[���^�C����ݒ�
                m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
            }

            // ���ړ�
            if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || Controller.lY > 0 && m_nSelectCoolTime <= 0)
            {
                // ���̑I������
                m_select++;

                // ��ԉ��̑I�����Ȃ�A��ԏ��
                if (m_select >= SELECT_MAX)
                {
                    m_select = SELECT_TRAINING;
                }

                // �I���̃N�[���^�C����ݒ�
                m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
            }
        }
    }

    // �J�[�\���̈ʒu��ς���
    D3DXVECTOR3 pos = D3DXVECTOR3(190.0f, 209.5f, 0.0f);
    float fDigitPosY = 18.2f * m_select;
    pos.y += fDigitPosY;
    if (m_pUI_Cursor)
    {
        m_pUI_Cursor->SetPosition(pos);
    }
}
