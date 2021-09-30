//===============================================
//
// �|�[�Y���� (pause.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "pause.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "ui.h"
#include "camera.h"
#include "texture.h"

//=======================================
// �}�N����`
//=======================================

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[CPause::TYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 CPause::m_pVtxBuff = NULL;

//========================================
// �|�[�Y�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CPause::CPause()
{
    m_pVtxBuff = NULL;

    m_nWho = PLAYER_1;
    m_state = STATE_NONE;
    m_typePauseSelect = TYPE_CONTINUE;
    m_colSelect = DEFAULT_COLOR;
    m_bAlphaDown = true;
    m_nSelectCoolTime = 0;
}

//========================================
// �|�[�Y�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CPause::~CPause()
{

}

//========================================
// �|�[�Y�̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CPause::Init(void)
{
    // �摜�����т���
    m_apTexture[TYPE_BG] = CManager::GetTexture()->GetInfo(16)->pTexture;
    m_apTexture[TYPE_CONTINUE] = CManager::GetTexture()->GetInfo(13)->pTexture;
    m_apTexture[TYPE_RETRY] = CManager::GetTexture()->GetInfo(14)->pTexture;
    m_apTexture[TYPE_QUIT] = CManager::GetTexture()->GetInfo(15)->pTexture;

    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���_�o�b�t�@����
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*TYPE_MAX,
        D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

    // ���_���W�̐ݒ�i�E���Őݒ肷��j
    // �w�i
    pVtx[0].pos = PAUSE_BG_POS + D3DXVECTOR3(-(SCREEN_WIDTH / 2), -(SCREEN_HEIGHT / 2), 0.0f);
    pVtx[1].pos = PAUSE_BG_POS + D3DXVECTOR3(+(SCREEN_WIDTH / 2), -(SCREEN_HEIGHT / 2), 0.0f);
    pVtx[2].pos = PAUSE_BG_POS + D3DXVECTOR3(-(SCREEN_WIDTH / 2), +(SCREEN_HEIGHT / 2), 0.0f);
    pVtx[3].pos = PAUSE_BG_POS + D3DXVECTOR3(+(SCREEN_WIDTH / 2), +(SCREEN_HEIGHT / 2), 0.0f);

    // �R���e�B�j���[
    pVtx[4].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[5].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[6].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[7].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);

    // ���g���C
    pVtx[8].pos = PAUSE_RETRY_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[9].pos = PAUSE_RETRY_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[10].pos = PAUSE_RETRY_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[11].pos = PAUSE_RETRY_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);

    // ��߂�
    pVtx[12].pos = PAUSE_QUIT_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[13].pos = PAUSE_QUIT_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[14].pos = PAUSE_QUIT_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[15].pos = PAUSE_QUIT_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);

    for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
    {
        // �F(�S�ē����ɂ���)
        pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

        // rhw�̐ݒ�i�l��1.0�Őݒ肷��j
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        // �e�N�X�`�����W�̐ݒ�
        pVtx[0].tex[0] = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex[0] = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex[0] = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex[0] = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return S_OK;
}

//========================================
// �|�[�Y�̏I������
// Author : �㓡�T�V��
//========================================
void CPause::Uninit(void)
{
    // ���_�o�b�t�@�̔j��
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
}

//========================================
// �|�[�Y�̍X�V����
// Author : �㓡�T�V��
//========================================
void CPause::Update(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(m_nWho);

    // �J�����擾
    CCamera*pCamera = CManager::GetCamera();

    // �I�����Ă�����̂́A�F��ς���
    if (m_bAlphaDown == true)
    {
        m_colSelect.r -= PAUSE_MENU_FADE_RATE;
        m_colSelect.g -= PAUSE_MENU_FADE_RATE;
        m_colSelect.b -= PAUSE_MENU_FADE_RATE;
    }
    else
    {
        m_colSelect.r += PAUSE_MENU_FADE_RATE;
        m_colSelect.g += PAUSE_MENU_FADE_RATE;
        m_colSelect.b += PAUSE_MENU_FADE_RATE;
    }
    if (m_colSelect.r <= PAUSE_MENU_FADE_OUT_MIN)
    {
        m_bAlphaDown = false;
    }
    else if (m_colSelect.r >= 1.0f)
    {
        m_bAlphaDown = true;
    }

    // �I���̃N�[���^�C���𐔂���
    if (m_nSelectCoolTime > 0)
    {
        m_nSelectCoolTime--;
    }

    // �ϐ��錾
    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    VERTEX_2D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
    D3DXCOLOR inactive = PAUSE_MENU_INACTIVE_COLOR; // ��A�N�e�B�u�F

    switch (m_state)
    {
    case STATE_PAUSE:

        // �X�^�[�g�{�^���������ꂽ��
        if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_START))
        {
            // SE
            CManager::SoundPlay(CSound::LABEL_SE_CANCEL);

            // �|�[�Y�����ɂ���
            CGame::SetQuitPause();
            pCamera->SetState(CCamera::STATE_BUTTLE);

            // �I�������R���e�B�j���[�֖߂�
            m_typePauseSelect = TYPE_CONTINUE;

            // ��Ԃ����Z�b�g
            m_state = STATE_NONE;
        }
        else
        {
            // �|�[�Y���j���[��I������
            SelectPauseMenu();
        }

        // �|�[�Y�̔w�i��������悤��
        pVtx[0].col = DEFAULT_COLOR;
        pVtx[1].col = DEFAULT_COLOR;
        pVtx[2].col = DEFAULT_COLOR;
        pVtx[3].col = DEFAULT_COLOR;

        // �I���ɂ���āA�F��ς���
        switch (m_typePauseSelect)
        {
        case TYPE_CONTINUE:

            // �R���e�B�j���[
            pVtx[4].col = m_colSelect;
            pVtx[5].col = m_colSelect;
            pVtx[6].col = m_colSelect;
            pVtx[7].col = m_colSelect;

            // ���g���C
            pVtx[8].col = inactive;
            pVtx[9].col = inactive;
            pVtx[10].col = inactive;
            pVtx[11].col = inactive;

            // ��߂�
            pVtx[12].col = inactive;
            pVtx[13].col = inactive;
            pVtx[14].col = inactive;
            pVtx[15].col = inactive;

            break;

        case TYPE_RETRY:

            // �R���e�B�j���[
            pVtx[4].col = inactive;
            pVtx[5].col = inactive;
            pVtx[6].col = inactive;
            pVtx[7].col = inactive;

            // ���g���C
            pVtx[8].col = m_colSelect;
            pVtx[9].col = m_colSelect;
            pVtx[10].col = m_colSelect;
            pVtx[11].col = m_colSelect;

            // ��߂�
            pVtx[12].col = inactive;
            pVtx[13].col = inactive;
            pVtx[14].col = inactive;
            pVtx[15].col = inactive;

            break;

        case TYPE_QUIT:

            // �R���e�B�j���[
            pVtx[4].col = inactive;
            pVtx[5].col = inactive;
            pVtx[6].col = inactive;
            pVtx[7].col = inactive;

            // ���g���C
            pVtx[8].col = inactive;
            pVtx[9].col = inactive;
            pVtx[10].col = inactive;
            pVtx[11].col = inactive;

            // ��߂�
            pVtx[12].col = m_colSelect;
            pVtx[13].col = m_colSelect;
            pVtx[14].col = m_colSelect;
            pVtx[15].col = m_colSelect;

            break;
        }

        break;
    }

    // ��Ԃ��Ȃ��Ȃ�����A�F�����Z�b�g
    if (m_state == STATE_NONE)
    {
        for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
        {
            // �F(�S�ē����ɂ���)
            pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

            pVtx += 4;
        }
    }

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//========================================
// �|�[�Y�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CPause::Draw(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
    {
        // �e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, m_apTexture[nCnt]);

        // �|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_VERTEX * nCnt, NUM_POLYGON);
    }

    // �e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=======================================
// �|�[�Y�̍쐬
// Author : �㓡�T�V��
//=======================================
CPause *CPause::Create(void)
{
    CPause *pPause = NULL;

    // ���������m��
    pPause = new CPause;

    // ������
    pPause->Init();

    return pPause;
}

//=======================================
// �|�[�Y�̃��j���[�I��
// Author : �㓡�T�V��
//=======================================
void CPause::SelectPauseMenu(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(m_nWho);

    // �J�����擾
    CCamera*pCamera = CManager::GetCamera();

    // �t�F�[�h���Ă��Ȃ��Ȃ�A�I���\
    if (CFade::GetFade() == CFade::FADE_NONE)
    {
        switch (m_typePauseSelect)
        {
        case TYPE_CONTINUE:

            // A�������ꂽ(����)
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_A))
            {
                // SE
                CManager::SoundPlay(CSound::LABEL_SE_SELECT);

                // �|�[�Y�����ɂ���
                CGame::SetQuitPause();
                pCamera->SetState(CCamera::STATE_BUTTLE);

                // ��Ԃ����Z�b�g
                m_state = STATE_NONE;
            }
            else
            {
                // ��ړ�
                if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || Controller.lY < 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // ��̑I������
                    m_typePauseSelect = TYPE_QUIT;

                    // �I���̃N�[���^�C����ݒ�
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }

                // ���ړ�
                if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || Controller.lY > 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // ���̑I������
                    m_typePauseSelect = TYPE_RETRY;

                    // �I���̃N�[���^�C����ݒ�
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }
            }

            break;

        case TYPE_RETRY:

            // A�������ꂽ(����)
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_A))
            {
                // SE
                CManager::SoundPlay(CSound::LABEL_SE_SELECT);

                // ���g���C
                CFade::SetFade(CManager::MODE_GAME);

                // ��Ԃ����Z�b�g
                m_state = STATE_NONE;
            }
            else
            {
                // ��ړ�
                if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || Controller.lY < 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // ��̑I������
                    m_typePauseSelect = TYPE_CONTINUE;

                    // �I���̃N�[���^�C����ݒ�
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }

                // ���ړ�
                if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || Controller.lY > 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // ���̑I������
                    m_typePauseSelect = TYPE_QUIT;

                    // �I���̃N�[���^�C����ݒ�
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }
            }

            break;

        case TYPE_QUIT:

            // A�������ꂽ(����)
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_A))
            {
                // SE
                CManager::SoundPlay(CSound::LABEL_SE_SELECT);

                // ��߂�(�^�C�g����)
                CFade::SetFade(CManager::MODE_TITLE);

                // ��Ԃ����Z�b�g
                m_state = STATE_NONE;
            }
            else
            {
                // ��ړ�
                if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || Controller.lY < 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // ��̑I������
                    m_typePauseSelect = TYPE_RETRY;

                    // �I���̃N�[���^�C����ݒ�
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }

                // ���ړ�
                if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || Controller.lY > 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // ���̑I������
                    m_typePauseSelect = TYPE_CONTINUE;

                    // �I���̃N�[���^�C����ݒ�
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }
            }

            break;
        }
    }
}