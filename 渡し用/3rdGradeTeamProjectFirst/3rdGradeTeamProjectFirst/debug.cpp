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
CDebug * CDebug::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
    CDebug *pDebug = NULL;
    pDebug = new CDebug;
    pDebug->m_type = type;
    pDebug->Init(pos, size);

    return pDebug;
}

//=============================================================================
// UI�̃����[�h
// Author : �㓡�T�V��
//=============================================================================
#ifdef _DEBUG
void CDebug::ReloadUI(CUI::SET set)
{
    // UI���폜
    CScene::ReleaseUI();

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

    // F8�L�[�������ꂽ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F8))
    {
        CRenderer::SwitchDispFont();
    }
}

//=============================================================================
// �^�C�g�����̃f�o�b�O�R�}���h
// Author : �㓡�T�V��
//=============================================================================
void CDebug::TitleCommand(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F4�L�[�������ꂽ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UI�̃����[�h
        ReloadUI(CUI::SET_TITLE);
    }

    // ���ʂ̃f�o�b�O�R�}���h
    CommonCommand();
}

//=============================================================================
// �V�ѕ����̃f�o�b�O�R�}���h
// Author : �㓡�T�V��
//=============================================================================
void CDebug::ManualCommand(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F4�L�[�������ꂽ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UI�̃����[�h
        ReloadUI(CUI::SET_MANUAL);
    }

    // ���ʂ̃f�o�b�O�R�}���h
    CommonCommand();
}

//=============================================================================
// �Q�[�����̃f�o�b�O�R�}���h
// Author : �㓡�T�V��
//=============================================================================
void CDebug::GameCommand(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �G���^�[�L�[�������ꂽ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
    {
        // �t�F�[�h���Ă��Ȃ��Ȃ�
        if (CFade::GetFade() == CFade::FADE_NONE)
        {
            // �^�C�g���Ɉڍs
            CFade::SetFade(CManager::MODE_TITLE);
        }
    }

    // F4�L�[�������ꂽ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UI�̃����[�h
        ReloadUI(CUI::SET_GAME);

        // �v���C���[1�̃p�[�c�ݒ�������[�h
        CModelData* pModelData = CManager::GetModelData();
        pModelData->LoadPartsList();
        pModelData->LoadPartsRate();
        CPlayer* pPlayer = CGame::GetPlayer(PLAYER_1);
        pPlayer->LoadCustom();

        // �G�t�F�N�g�f�[�^�̃����[�h
        CEffectData* pEffectData = CManager::GetEffectData();
        pEffectData->Init();
    }

    // ���ʂ̃f�o�b�O�R�}���h
    CommonCommand();
}

//=============================================================================
// ���U���g���̃f�o�b�O�R�}���h
// Author : �㓡�T�V��
//=============================================================================
void CDebug::ResultCommand(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F4�L�[�������ꂽ��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UI�̃����[�h
        ReloadUI(CUI::SET_RESULT);
    }

    // ���ʂ̃f�o�b�O�R�}���h
    CommonCommand();
}
#endif