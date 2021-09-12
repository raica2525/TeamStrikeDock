//===============================================
//
// UI���� (ui.cpp)
// Author : �㓡�T�V��
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "character.h"
#include "texture.h"
//#include "player.h"

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================

//========================================
// UI�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_nTexType = 0;
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_action = CUI::ACTION_NONE;
    m_fAngle = 0.0f;
    m_bUse = true;
    m_bBlinking = false;
    m_nCntTime = 0;
    m_bFadeOut = false;
}

//========================================
// UI�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CUI::~CUI()
{

}

//========================================
// UI�̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_nTexType);

    return S_OK;
}

//========================================
// UI�̏I������
// Author : �㓡�T�V��
//========================================
void CUI::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// UI�̍X�V����
// Author : �㓡�T�V��
//========================================
void CUI::Update(void)
{
    // �ϐ��錾
    D3DXVECTOR3 pos = GetPosition();                   // �ʒu���擾
    D3DXVECTOR3 size = GetSize();                      // �T�C�Y���擾
    int nPatternAnim = 0;                              // �p�^�[���A�j���̃J�E���^�𗘗p����Ƃ�

                                                       //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    switch (m_action)
    {
    case ACTION_TITLE_LOGO:

        // ���ꗎ����
        pos.y += TITLE_LOGO_FLOW_DOWN_SPEED;

        // ���̒l�Ŏ~�߂Ă���
        if (pos.y >= TITLE_LOGO_TARGET_POS_Y)
        {
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        // �G���^�[�A�܂��̓X�^�[�g�{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // �^�C�g�����S�������ɉ���
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        break;

    case ACTION_TITLE_PRESS_ENTER:

        // �^�C�g���̎��Ԃ��J�E���g
        m_nCntTime++;

        // �^�C�g���̍ő厞��
        if (m_nCntTime > TITLE_MAX_TIME)
        {
            m_nCntTime = TITLE_MAX_TIME;
        }

        // �F��ς���
        if (m_bFadeOut == true)
        {
            m_col.r -= PRESS_START_FADE_RATE;
            m_col.g -= PRESS_START_FADE_RATE;
            m_col.b -= PRESS_START_FADE_RATE;
        }
        else
        {
            m_col.r += PRESS_START_FADE_RATE;
            m_col.g += PRESS_START_FADE_RATE;
            m_col.b += PRESS_START_FADE_RATE;
        }
        if (m_col.r <= PRESS_START_FADE_OUT_MIN)
        {
            m_bFadeOut = false;
        }
        else if (m_col.r >= 1.0f)
        {
            m_bFadeOut = true;
        }

        // �G���^�[�A�܂��̓X�^�[�g�{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // ��莞�Ԍo�߂œ_�ŉ\��
            if (m_nCntTime >= TITLE_NEXT_SCENE)
            {
                // �_�ł̃t���O��true��
                m_bBlinking = true;
            }
            // ����ȊO�́A�J�E���^��J�ډ\�܂ň�C�ɐi�߂�
            else
            {
                m_nCntTime = TITLE_NEXT_SCENE;
            }
        }

        // �_�ł���Ȃ�
        if (m_bBlinking == true)
        {
            // �J�E���^�[��i�߂āA�p�^�[����؂�ւ���
            nPatternAnim = CountAnimation(2, 2);

            // �F��2�p�^�[���Ő؂�ւ���
            if (nPatternAnim == 1)
            {
                m_col.r = PRESS_START_FADE_OUT_MIN;
                m_col.g = PRESS_START_FADE_OUT_MIN;
                m_col.b = PRESS_START_FADE_OUT_MIN;
            }
            else
            {
                m_col.r = 1.0f;
                m_col.g = 1.0f;
                m_col.b = 1.0f;
            }
        }

        break;
    }

    // �F�𔽉f
    SetColor(m_col);

    // �ʒu�𔽉f
    SetPosition(pos);

    // �T�C�Y�𔽉f
    SetSize(size);

    // �Q�[�W�n�ȊO�́A�ʏ�̒��_���W
    if (m_action != ACTION_GAUGE)
    {
        SetVertex();
    }

    // �g�p�t���O��false�Ȃ����
    if (m_bUse == false)
    {
        Uninit();
    }
}

//========================================
// UI�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CUI::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// UI�̍쐬
// Author : �㓡�T�V��
//========================================
CUI *CUI::Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, ACTION action)
{
    CUI *pUI = NULL;

    // ���������m��
    pUI = new CUI;

    // �e�N�X�`���̃^�C�v�͐�Ɍ��т��Ă���
    pUI->m_nTexType = nTexType;

    // ������
    pUI->Init(pos, size);

    // �����o�ϐ������т���
    pUI->m_col = col;
    pUI->m_action = action;

    return pUI;
}

//========================================
// UI�̐ݒu
// Author : �㓡�T�V��
//========================================
void CUI::Place(SET set)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    char cReadText[256];	             // �����Ƃ��ēǂݎ��p
    char cHeadText[256];	             // �����̔��ʗp
    char cDie[256];		                 // �g��Ȃ�����
    int nNumType = 0;                    // �^�C�v�̃i���o�[
    D3DXVECTOR3 pos = DEFAULT_VECTOR;    // �ʒu
    D3DXVECTOR3 size = DEFAULT_VECTOR;   // �傫��
    D3DXCOLOR col = DEFAULT_COLOR;       // �F
    int nAction = 0;                     // ����

    // �t�@�C�����J��
    switch (set)
    {
    case SET_TITLE:
        pFile = fopen("data/TXT/ui_title.txt", "r");
        break;
    case SET_MANUAL:
        pFile = fopen("data/TXT/ui_manual.txt", "r");
        break;
    case SET_GAME:
        pFile = fopen("data/TXT/ui_game.txt", "r");
        break;
    case SET_RESULT:
        pFile = fopen("data/TXT/ui_result.txt", "r");
        break;
    }

    // �J������
    if (pFile != NULL)
    {
        // SCRIPT�̕�����������܂�
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            // �e�L�X�g����cReadText���������󂯎��
            fgets(cReadText, sizeof(cReadText), pFile);

            // cReedText��cHeadText�Ɋi�[
            sscanf(cReadText, "%s", &cHeadText);
        }

        // cHeadText��SCRIPT�̎�
        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            // cHeadText��END_SCRIPT�ɂȂ�܂�
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {
                fgets(cReadText, sizeof(cReadText), pFile);
                sscanf(cReadText, "%s", &cHeadText);

                // cHeadText��UISET�̎�
                if (strcmp(cHeadText, "UISET") == 0)
                {
                    // cHeadText��END_UISET�ɂȂ�܂�
                    while (strcmp(cHeadText, "END_UISET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);

                            // �s���Ȓl�͑S�ăf�t�H���g���Ăяo��
                            if (nNumType < 0 || nNumType >= MAX_TEXTURE)
                            {
                                nNumType = 0;
                            }
                        }
                        else if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f", &cDie, &cDie, &pos.x, &pos.y);
                        }
                        else if (strcmp(cHeadText, "SIZE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f", &cDie, &cDie, &size.x, &size.y);
                        }
                        else if (strcmp(cHeadText, "COL") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie, &col.r, &col.g, &col.b, &col.a);
                        }
                        else if (strcmp(cHeadText, "ACTION") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nAction);

                            // �s���Ȓl�͑S�ăf�t�H���g���Ăяo��
                            if (nAction < ACTION_NONE || nAction >= ACTION_MAX)
                            {
                                nAction = ACTION_NONE;
                            }

                            // ����
                            Create(nNumType, pos, size, col, (ACTION)nAction);
                        }
                    }
                }
            }
        }
        // �t�@�C�������
        fclose(pFile);
    }
    // �J���Ȃ�������
    else
    {
        printf("�J����܂���ł���\n");
    }
}

//========================================
// �g�傷��UI
// Author : �㓡�T�V��
//========================================
D3DXVECTOR3 CUI::Extend(D3DXVECTOR3 size)
{
    // �ϐ��錾
    D3DXVECTOR3 returnSize = size;  // �Ԃ��T�C�Y
    int nCnt = 0;                   // �J�E���^

                                    // ���T�C�Y�g��
    returnSize.x += UI_EXTEND_MISSION_EXTEND_RATE_X;

    // �c�T�C�Y�g��
    returnSize.y += UI_EXTEND_MISSION_EXTEND_RATE_Y;

    // ���T�C�Y�̐���
    if (returnSize.x >= UI_EXTEND_MISSION_MAX_SIZE_X)
    {
        returnSize.x = UI_EXTEND_MISSION_MAX_SIZE_X;
    }

    // �c�T�C�Y�̐���
    if (returnSize.y >= UI_EXTEND_MISSION_MAX_SIZE_Y)
    {
        returnSize.y = UI_EXTEND_MISSION_MAX_SIZE_Y;
    }

    // �A�j���[�V�����̃J�E���^�𗘗p���āAUI�̃t�F�[�h�A�E�g�ɂȂ���
    nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

    // �t�F�[�h�A�E�g���n�߂�J�E���g�𒴂�����A���l�������Ă���
    if (nCnt >= UI_EXTEND_MISSION_FADE_OUT_COUNT)
    {
        m_col.a -= UI_EXTEND_MISSION_FADE_OUT_RATE;
    }

    // �����x��0�����������A����
    if (m_col.a <= 0.0f)
    {
        m_bUse = false;
    }

    return returnSize;
}