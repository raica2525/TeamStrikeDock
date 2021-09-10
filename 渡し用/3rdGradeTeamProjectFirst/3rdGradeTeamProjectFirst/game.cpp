//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : �㓡�T�V��
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "game.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "input.h"
#include "scene.h"
#include "library.h"
#include "debug.h"
#include "pause.h"
#include "scene2d.h"
#include "camera.h"
#include "ball.h"
#include "bg.h"
#include "wave.h"
#include "effect2d.h"
#include "effect3d.h"
#include "number_array.h"

//========================================
// �}�N����`
//========================================

// �Q�[����Ԃ̊Ǘ��t���[��
#define FINISH_WAIT_FRAME 240   // �������ɁA�҂t���[����
#define CREATE_POS_Y_RATE 0.8f  // �{�[���̔����ʒuY�̊���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CGame::m_bStopObjUpdate = false;

CPlayer *CGame::m_apPlayer[] = {};
CBall *CGame::m_pBall = NULL;
CPause *CGame::m_pPause = NULL;
CEffect2D *CGame::m_pEffect2d_Nega = NULL;
CEffect2D *CGame::m_pEffect2d_Posi = NULL;
CNumberArray *CGame::m_pNumArray_BallSpd = NULL;

CGame::TYPE CGame::m_type = TYPE_TRAINING;
int CGame::m_nNumAllPlayer = 0;
int CGame::m_nNumStock = 0;
bool CGame::m_bUseKeyboard = false;
CPlayer::AI_LEVEL CGame::m_aAILevel[] = {};
CGame::STATE CGame::m_state = STATE_ROUND_START;
CGame::MAP_LIMIT CGame::m_mapLimit = {};
int CGame::m_nNumDefeatPlayer = 0;
int CGame::m_nWhoWorstPlayer = PLAYER_1;
int CGame::m_nNumDeathPlayer = 0;
CGame::RESERVE_SHOOT CGame::m_aReserveShoot[] = {};

//=============================================================================
// �Q�[���̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CGame::CGame()
{
    m_bStopObjUpdate = false;

    memset(m_apPlayer, 0, sizeof(m_apPlayer));
    m_pBall = NULL;
    m_pPause = NULL;
    m_pEffect2d_Nega = NULL;
    m_pEffect2d_Posi = NULL;
    m_pNumArray_BallSpd = NULL;

    // �ÓI�����o�ϐ����������i�J�ڎ��ɖ���K�v�Ȃ��̂����j
    //m_type = TYPE_TRAINING;
    //m_nNumAllPlayer = 0;
    //m_nNumStock = 0;
    //m_bUseKeyboard = false;
    //memset(m_aAILevel, 0, sizeof(m_aAILevel));
    m_state = STATE_ROUND_START;
    m_nNumDefeatPlayer = 0;
    m_nWhoWorstPlayer = PLAYER_1;
    m_nNumDeathPlayer = 0;
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        m_aReserveShoot[nCnt].attackCenterPos = DEFAULT_VECTOR;
        m_aReserveShoot[nCnt].moveAngle = DEFAULT_VECTOR;
        m_aReserveShoot[nCnt].fPower = 0.0f;
        m_aReserveShoot[nCnt].bFirstCollision = true;
        m_aReserveShoot[nCnt].flag = CBall::SHOOT_FLAG_NONE;
        m_aReserveShoot[nCnt].bReserved = false;
    }

    // ���Ń}�b�v���������Ă���
    m_mapLimit.fHeight = GAME_LIMIT_HEIGHT;
    m_mapLimit.fWidth = GAME_LIMIT_WIDTH;

    m_nCntGameTime = 0;
}

//=============================================================================
// �Q�[���̃f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CGame::Init(void)
{
    // ��`
    const float SPLIT_RATE_UNDER_3 = 0.5f;
    const float SPLIT_RATE_ABOVE_2 = 0.333f;
    if (m_type == TYPE_TRAINING)
    {
        m_nNumAllPlayer = 1; // �g���[�j���O��1�l�Œ�
    }

    // �X�e�[�W�̃��f���𐶐�
    CBg::Create(34, DEFAULT_VECTOR);    // �X�e�[�W1��34
    CBg::Create(83, DEFAULT_VECTOR);    // �X�e�[�W1�̐���83

    // UI�𐶐�
    CUI::Place(CUI::SET_GAME);

    // �|�[�Y�̐���
    m_pPause = CPause::Create();

    // ��(8�̂ł��d����������A���炩�̕��׌y�����K�v�i���f���̃|���S�����팸�i2�����x�j���A�`�����j)
    //for (int nCnt = 0; nCnt < 16; nCnt++)
    //{
    //    // �|���S�������ߐڒ��_���������邱�ƂłȂ�Ƃ��팸�i32�̑ς��j
    //    float fPosX = 200.0f * nCnt;
    //    CPlayer::Create(D3DXVECTOR3(-1000.0f + fPosX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), CPlayer::PLAYABLE_001);
    //}
    // �v���C���[�̐���
    D3DXVECTOR3 player1Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player2Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player3Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    D3DXVECTOR3 player4Pos = D3DXVECTOR3(m_mapLimit.fWidth, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f);
    float fSplitXRate = 0.0f;
    switch (m_nNumAllPlayer)
    {
    case 1:
        fSplitXRate = SPLIT_RATE_UNDER_3;
        player1Pos.x *= -fSplitXRate;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_001, m_aAILevel[PLAYER_1], m_bUseKeyboard);
        break;
    case 2:
        fSplitXRate = SPLIT_RATE_UNDER_3;
        player1Pos.x *= -fSplitXRate;
        player2Pos.x *= fSplitXRate;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_001, m_aAILevel[PLAYER_1], m_bUseKeyboard);
        m_apPlayer[1] = CPlayer::CreateInGame(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_002, m_aAILevel[PLAYER_2]);
        break;
    case 3:
        fSplitXRate = SPLIT_RATE_ABOVE_2;
        player1Pos.x *= -fSplitXRate * 2;
        player2Pos.x *= -fSplitXRate;
        player3Pos.x *= fSplitXRate;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_001, m_aAILevel[PLAYER_1], m_bUseKeyboard);
        m_apPlayer[1] = CPlayer::CreateInGame(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_002, m_aAILevel[PLAYER_2]);
        m_apPlayer[2] = CPlayer::CreateInGame(player3Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_003, m_aAILevel[PLAYER_3]);
        break;
    case 4:
        fSplitXRate = SPLIT_RATE_ABOVE_2;
        player1Pos.x *= -fSplitXRate * 2;
        player2Pos.x *= -fSplitXRate;
        player3Pos.x *= fSplitXRate;
        player4Pos.x *= fSplitXRate * 2;
        m_apPlayer[0] = CPlayer::CreateInGame(player1Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_001, m_aAILevel[PLAYER_1], m_bUseKeyboard);
        m_apPlayer[1] = CPlayer::CreateInGame(player2Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_002, m_aAILevel[PLAYER_2]);
        m_apPlayer[2] = CPlayer::CreateInGame(player3Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_003, m_aAILevel[PLAYER_3]);
        m_apPlayer[3] = CPlayer::CreateInGame(player4Pos, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), m_nNumStock, CPlayer::PLAYABLE_004, m_aAILevel[PLAYER_4]);
        break;
    }

    // �J�����̃��b�N�I���ꏊ��ς���
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_GAME);

    // �{�[���𐶐�
    m_pBall = CBall::Create(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f), true);

    // ���]�����p�̃G�t�F�N�g�𐶐�
    m_pEffect2d_Nega = CEffect2D::Create(3, DEFAULT_VECTOR);
    m_pEffect2d_Nega->SetUseUpdate(false);
    m_pEffect2d_Posi = CEffect2D::Create(3, DEFAULT_VECTOR);
    m_pEffect2d_Posi->SetUseUpdate(false);

    // �{�[���X�s�[�h�\���𐶐�
    m_pNumArray_BallSpd = CNumberArray::Create(12, D3DXVECTOR3(640.0f, 665.0f, 0.0f), NUMBER_SIZE_X_BALL_SPD, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), (int)BALL_FIRST_SPEED, false);

    // �X�e�[�W�̐ݒ�
    //SetStage(STAGE_1);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CGame::Uninit(void)
{
    // �S�Ẳ����~
    CManager::SoundStopAll();

    // �|�[�Y��j��
    if (m_pPause != NULL)
    {
        m_pPause->Uninit();
        delete m_pPause;
        m_pPause = NULL;
    }
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CGame::Update(void)
{
    // �Q�[����Ԃ̊Ǘ�
    ManageState();

    // ���]�����̏I���`�F�b�N
    if (m_pEffect2d_Nega && m_pEffect2d_Posi)
    {
        // �|�W�̂ق������̑傫���ɒB������A�ǂ�����T�C�Y��߂�
        if (m_pEffect2d_Posi->GetSize().x >= SCREEN_WIDTH * 2)
        {
            m_pEffect2d_Nega->SetSize(DEFAULT_VECTOR);
            m_pEffect2d_Posi->SetSize(DEFAULT_VECTOR);
            m_pEffect2d_Nega->SetUseUpdate(false);
            m_pEffect2d_Posi->SetUseUpdate(false);
            m_pEffect2d_Nega->SetRotVertex(0.0f);     // �X�V���~�߂����ɁA���_�͂����Œ���
            m_pEffect2d_Posi->SetRotVertex(0.0f);     // �X�V���~�߂����ɁA���_�͂����Œ���
        }
    }
}

//=============================================================================
// �Q�[����Ԃ̊Ǘ�
// Author : �㓡�T�V��
//=============================================================================
void CGame::ManageState(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    switch (m_state)
    {
    case STATE_ROUND_START:

        // ���E���h�J�n
        RoundStart();

        break;

    case STATE_BUTTLE:

        // �o�g����
        InButtle();

        break;

    case STATE_FINISH:

        // ���s����
        JudgmentFinish();

        break;

    case STATE_PAUSE_MENU:

        // �|�[�Y�̍X�V
        m_pPause->Update();

        break;
    }
}

//=============================================================================
// ���E���h�J�n����
// Author : �㓡�T�V��
//=============================================================================
void CGame::RoundStart(void)
{
    // ���X�|�[������
    for (int nCntPlayer = 0; nCntPlayer < m_nNumAllPlayer; nCntPlayer++)
    {
        // �������Ă��Ȃ����A�X�g�b�N��1�ȏ゠��Ȃ�
        if (!m_apPlayer[nCntPlayer]->GetDisp() && m_apPlayer[nCntPlayer]->GetStock() > 0)
        {
            m_apPlayer[nCntPlayer]->Respawn();
        }

        // �S��SP�Q�[�W�̓��Z�b�g
        m_apPlayer[nCntPlayer]->SetSpGaugeCurrent(0.0f);
    }

    // �{�[���X�s�[�h�\�������Z�b�g
    m_pNumArray_BallSpd->SetDispNumber((int)BALL_FIRST_SPEED);

    // �J�E���^�����Z
    m_nCntGameTime++;

    // ���t���[���ŁA�o�g������
    if (m_nCntGameTime >= BALL_DISPING_TIME)
    {
        // �J�E���^���Z�b�g
        m_nCntGameTime = 0;

        //// �~�b�V�����X�^�[�g�\��
        //CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_START);

        // �o�g������
        m_state = STATE_BUTTLE;
    }
}

//=============================================================================
// �o�g����
// Author : �㓡�T�V��
//=============================================================================
void CGame::InButtle(void)
{
    // �{�[���̗\��󋵔��f�i�����t�B�j�b�V���ɂȂ���1F�ł��A���̃��E���h�ɗ\��������z���Ȃ��悤�ɁA�\��𔽉f�����Ă����j
    JudgmentShoot();

    switch (m_type)
    {
    case TYPE_ARENA:
        // �A���[�i���[�h��1�l�c������
        if (m_nNumDefeatPlayer >= m_nNumAllPlayer - m_nNumDeathPlayer - 1)
        {
            // �t�B�j�b�V����
            m_state = STATE_FINISH;

            // �{�[���͏����Ă���
            m_pBall->SetDispOff();
        }
        break;
    }

    // ��`
    const int NO_PAUSE_PLAYER = -1; // �N���|�[�Y�������Ă��Ȃ�

    // �ϐ��錾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();  // �L�[�{�[�h
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();        // �R���g���[��
    int nNumPausePlayer = NO_PAUSE_PLAYER;  // �|�[�Y���������v���C���[

    // �X�^�[�g�{�^�����������l�����т���
    if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        nNumPausePlayer = PLAYER_1;
    }
    else if (pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_START))
    {
        nNumPausePlayer = PLAYER_2;
    }
    else if (pInputJoypad->GetJoypadTrigger(PLAYER_3, CInputJoypad::BUTTON_START))
    {
        nNumPausePlayer = PLAYER_3;
    }
    else if (pInputJoypad->GetJoypadTrigger(PLAYER_4, CInputJoypad::BUTTON_START))
    {
        nNumPausePlayer = PLAYER_4;
    }

    // �|�[�Y����Ȃ�
    if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || nNumPausePlayer != NO_PAUSE_PLAYER)
    {
        // �L�[�{�[�h����Ń|�[�Y����ۂ́A1P�̃R���g���[�����g��
        if (nNumPausePlayer == NO_PAUSE_PLAYER)
        {
            nNumPausePlayer = PLAYER_1;
        }

        //// �|�[�Y��
        //pSound->Play(CSound::LABEL_SE_SYSTEM_PAUSE);

        // �|�[�Y��Ԃɂ���
        m_state = STATE_PAUSE_MENU;
        m_pPause->SetPauseMenuSelect(nNumPausePlayer);
        m_bStopObjUpdate = true;

        // �J�������~�߂�
        CManager::GetCamera()->SetState(CCamera::STATE_NONE);
    }
}

//=============================================================================
// ���s����
// Author : �㓡�T�V��
//=============================================================================
void CGame::JudgmentFinish(void)
{
    // �J�E���^�����Z
    m_nCntGameTime++;

    // ���t���[���ŁA���s����
    if (m_nCntGameTime >= FINISH_WAIT_FRAME)
    {
        // �J�E���^�����Z�b�g
        m_nCntGameTime = 0;

        // ���񂾃v���C���[���S�̂̃v���C���[-1�ɒB������
        if (m_nNumDeathPlayer >= m_nNumAllPlayer - 1)
        {
            // ���Ƀf�o�b�O���j���[�Ɉڍs
            CFade::SetFade(CManager::MODE_DEBUG_MENU);
        }
        else
        {
            // ���ꂽ�v���C���[�l�������Z�b�g
            m_nNumDefeatPlayer = 0;

            // �{�[�������Z�b�g���A�ł��������l�ɋz��������
            m_pBall->Reset(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * CREATE_POS_Y_RATE, 0.0f));
            m_pBall->SetAbsorb(m_apPlayer[m_nWhoWorstPlayer]);

            // ���ɂ�����x�X�e�[�W�J�n��
            m_state = STATE_ROUND_START;
        }
    }
}

//========================================
// �V���[�g�̗\��
//========================================
void CGame::ReserveShoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, int nWho)
{
    // �\������X�V
    m_aReserveShoot[nWho].attackCenterPos = attackCenterPos;
    m_aReserveShoot[nWho].moveAngle = moveAngle;
    m_aReserveShoot[nWho].fPower = fPower;
    m_aReserveShoot[nWho].bFirstCollision = bFirstCollision;
    m_aReserveShoot[nWho].flag = flag;
    m_aReserveShoot[nWho].bReserved = true;
}

//========================================
// �N���V���[�g�����Ă����̔���
//========================================
void CGame::JudgmentShoot(void)
{
    // ��`
    const int NO_PLAYER = -1; // �N���łĂȂ�����
    
    // �ϐ��錾
    int nNumShootingPlayer = NO_PLAYER;  // �V���[�g��łv���C���[
    int nNumAbsorbingPlayer = NO_PLAYER; // �z������v���C���[
    bool abSetOff[MAX_PLAYER];           // ���E����v���C���[
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        abSetOff[nCnt] = false;
    }

    // �\��󋵂��`�F�b�N
    int nNumReservingPlayer = 0;        // �\�񒆂̃v���C���[�l��
    int nNumFirstCollisionPlayer = 0;   // �ŏ��̐ڐG������v���C���[
    for (int nCnt = 0; nCnt < m_nNumAllPlayer; nCnt++)
    {
        if (m_aReserveShoot[nCnt].bReserved)
        {
            // �\�񒆂Ȃ�A�\��𔽉f
            m_aReserveShoot[nCnt].bReserved = false;
            nNumReservingPlayer++;

            // �ŏ��̐ڐG�ł͂Ȃ��A���łɍd�����Ȃ�قڊm���ɑłĂ�i�z�������ɂ͕�����j
            if (!m_aReserveShoot[nCnt].bFirstCollision)
            {
                if (IS_BITOFF(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_ABSORB))
                {
                    nNumShootingPlayer = nCnt;
                }
                else
                {
                    nNumAbsorbingPlayer = nCnt;
                }
            }
            else
            {
                // �ŏ��̐ڐG�Ȃ�A���̃v���C���[�Ƒ��E�̋���
                abSetOff[nCnt] = true;
                nNumFirstCollisionPlayer = nCnt;
            }
        }
    }

    // ���E�󋵂��`�F�b�N
    if (nNumReservingPlayer >= 2)
    {
        bool bUseWave = false;
        D3DXVECTOR3 setOffPos = m_pBall->GetPos();
        for (int nCnt = 0; nCnt < m_nNumAllPlayer; nCnt++)
        {
            if (abSetOff[nCnt])
            {
                // ���E�̃m�b�N�o�b�N
                m_apPlayer[nCnt]->TakeDamage(0.0f, nCnt, setOffPos, setOffPos, false, true);

                // �g�Ɖ�����
                if (!bUseWave)
                {
                    bUseWave = true;
                    CWave::Create(setOffPos, BALL_WAVE_FIRST_SIZE);
                    CManager::SoundPlay(CSound::LABEL_SE_OFFSET);
                }
            }
        }

        // ���E�ɑł��������l�����Ȃ��Ȃ�
        if (nNumShootingPlayer == NO_PLAYER && nNumAbsorbingPlayer == NO_PLAYER)
        {
            // �{�[����ł��グ��
            m_pBall->Launch(setOffPos);
        }
    }

    // �z���҂�����Ȃ�
    if (nNumAbsorbingPlayer != NO_PLAYER)
    {
        // �ł̂������
        nNumShootingPlayer = NO_PLAYER;
        nNumReservingPlayer = 1;
        nNumFirstCollisionPlayer = nNumAbsorbingPlayer;
    }

    // �N�����V���[�g��łȂ�A�V���[�g
    if (nNumShootingPlayer != NO_PLAYER)
    {
        m_pBall->Shoot(m_aReserveShoot[nNumShootingPlayer].attackCenterPos,
            m_aReserveShoot[nNumShootingPlayer].moveAngle,
            m_aReserveShoot[nNumShootingPlayer].fPower,
            m_aReserveShoot[nNumShootingPlayer].bFirstCollision,
            m_aReserveShoot[nNumShootingPlayer].flag,
            m_apPlayer[nNumShootingPlayer]);
    }
    else
    {
        // 1�l�݂̗̂\��Ȃ�A�ŏ��̐ڐG
        if (nNumReservingPlayer == 1)
        {
            // �ŏ��̐ڐG�́A�d�����Ԃ����т���
            m_apPlayer[nNumFirstCollisionPlayer]->SetStopTime(m_pBall->Shoot(m_aReserveShoot[nNumFirstCollisionPlayer].attackCenterPos,
                m_aReserveShoot[nNumFirstCollisionPlayer].moveAngle,
                m_aReserveShoot[nNumFirstCollisionPlayer].fPower,
                m_aReserveShoot[nNumFirstCollisionPlayer].bFirstCollision,
                m_aReserveShoot[nNumFirstCollisionPlayer].flag,
                m_apPlayer[nNumFirstCollisionPlayer]));

            // �K�E�Q�[�W�㏸�i�o���g�Ƌz���͏オ�炸�A�L���b�`�͒ǉ��ōX�ɏオ��j
            if (IS_BITOFF(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_BUNT) &&
                IS_BITOFF(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_ABSORB))
            {
                if (IS_BITON(m_aReserveShoot[nNumFirstCollisionPlayer].flag, CBall::SHOOT_FLAG_THROW))
                {
                    m_apPlayer[nNumFirstCollisionPlayer]->GainSpGauge(true);
                }
                else
                {
                    // �ʏ�U���̃G�t�F�N�g
                    switch (nNumFirstCollisionPlayer)
                    {
                    case PLAYER_1:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_1P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_1P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    case PLAYER_2:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_2P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_2P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    case PLAYER_3:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_3P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_3P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    case PLAYER_4:
                        if (m_pBall->GetSpeed() < BALL_SHOOT_BIG_HIT_SPEED)
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_4P, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        else
                        {
                            CEffect3D::Emit(CEffectData::TYPE_SHOOT_CIRCLE_4P_FAST, m_pBall->GetPos(), m_pBall->GetPos());
                        }
                        break;
                    }

                    m_apPlayer[nNumFirstCollisionPlayer]->GainSpGauge();
                }
            }
        }
    }
}

//========================================
// ��ԋ߂��v���C���[�ւ̊p�x�����߂�
//========================================
float CGame::GetAngleToClosestPlayer(int playable, D3DXVECTOR3 myPos)
{
    // �ϐ��錾
    float fAngle = 0.0f;                        // �Ԃ��p�x
    float fFirstDistance = 99999.9f;            // ����
    D3DXVECTOR3 targetPos = DEFAULT_VECTOR;     // �Ώۂ̈ʒu

    // �Ώۂ̈ʒu���A�����̈ʒu�̐^��ֈ�x���߂Ă���
    targetPos = D3DXVECTOR3(myPos.x, fFirstDistance, 0.0f);

    // ��������ԋ߂��v���C���[�����߂�i�����ȊO�Łj
    for (int nCntPlayer = 0; nCntPlayer < m_nNumAllPlayer; nCntPlayer++)
    {
        // �����ȊO��
        if (playable == nCntPlayer)
        {
            continue;
        }

        // �������Ă���Ȃ�
        if (m_apPlayer[nCntPlayer]->GetDisp())
        {
            // ���̃v���C���[�̈ʒu
            D3DXVECTOR3 otherPlayerPos = m_apPlayer[nCntPlayer]->GetPos();

            // ���������߂�
            float fSecondDistance = sqrtf(
                powf((myPos.x - otherPlayerPos.x), 2) +
                powf((myPos.y - otherPlayerPos.y), 2));

            // �����̔�r�ƁA�Ώۂ̈ʒu���X�V
            if (fFirstDistance > fSecondDistance)
            {
                fFirstDistance = fSecondDistance;
                targetPos = otherPlayerPos;
            }
        }
    }

    // �p�x�����߂�
    fAngle = atan2((targetPos.x - myPos.x), (targetPos.y - myPos.y));

    return fAngle;
}

//========================================
// ��ԋ߂��v���C���[�̈ʒu�����߂�
//========================================
D3DXVECTOR3 CGame::GetPosToClosestPlayer(int playable, D3DXVECTOR3 myPos)
{
    // �ϐ��錾
    float fFirstDistance = 99999.9f;            // ����
    D3DXVECTOR3 targetPos = DEFAULT_VECTOR;     // �Ώۂ̈ʒu

    // �Ώۂ̈ʒu���A�����̈ʒu�̐^��ֈ�x���߂Ă���
    targetPos = D3DXVECTOR3(myPos.x, fFirstDistance, 0.0f);

    // ��������ԋ߂��v���C���[�����߂�i�����ȊO�Łj
    for (int nCntPlayer = 0; nCntPlayer < m_nNumAllPlayer; nCntPlayer++)
    {
        // �����ȊO��
        if (playable == nCntPlayer)
        {
            continue;
        }

        // �������Ă���Ȃ�
        if (m_apPlayer[nCntPlayer]->GetDisp())
        {
            // ���̃v���C���[�̈ʒu
            D3DXVECTOR3 otherPlayerPos = m_apPlayer[nCntPlayer]->GetPos();

            // ���������߂�
            float fSecondDistance = sqrtf(
                powf((myPos.x - otherPlayerPos.x), 2) +
                powf((myPos.y - otherPlayerPos.y), 2));

            // �����̔�r�ƁA�Ώۂ̈ʒu���X�V
            if (fFirstDistance > fSecondDistance)
            {
                fFirstDistance = fSecondDistance;
                targetPos = otherPlayerPos;
            }
        }
    }

    return targetPos;
}
