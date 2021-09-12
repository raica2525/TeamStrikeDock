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
//#include "block.h"
#include "input.h"
#include "scene.h"
#include "library.h"
#include "debug.h"
//#include "score.h"
//#include "item.h"
#include "pause.h"
//#include "enemy.h"
#include "scene2d.h"
//#include "monster.h"
#include "player.h"
#include "camera.h"
#include "ball.h"
#include "bg.h"

//========================================
// �}�N����`
//========================================

// �Q�[����Ԃ̊Ǘ��t���[��
#define START_MISSION 30    // �~�b�V�����̊J�n

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CPlayer *CGame::m_apPlayer[] = {};
CBall *CGame::m_pBall = NULL;
CScore *CGame::m_pScore = NULL;
CPause *CGame::m_pPause = NULL;

CGame::STATE CGame::m_state = STATE_BUTTLE;
CGame::MAP_LIMIT CGame::m_mapLimit = {};
int CGame::m_nNumCurrentPlayer = 0;

//=============================================================================
// �Q�[���̃R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CGame::CGame()
{
    memset(m_apPlayer, 0, sizeof(m_apPlayer));
    m_pBall = NULL;
    m_pScore = NULL;
    m_pPause = NULL;

    // �ÓI�����o�ϐ����������i�J�ڎ��ɖ���K�v�Ȃ��̂����j
    m_state = STATE_BUTTLE; // ���Ƀo�g��������ɂ��Ă���
    m_nNumCurrentPlayer = 0;

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
    // UI�𐶐�
    CUI::Place(CUI::SET_GAME);

    //// �X�R�A�̐���
    //m_pScore = CScore::Create(SCORE_POS, SCORE_SIZE);

    // �|�[�Y�̐���
    m_pPause = CPause::Create();

    // ��(8�̂ł��d����������A���炩�̕��׌y�����K�v�i���f���̃|���S�����팸�i2�����x�j���A�`�����j)
    //for (int nCnt = 0; nCnt < 16; nCnt++)
    //{
    //    // �|���S�������ߐڒ��_���������邱�ƂłȂ�Ƃ��팸�i32�̑ς��j
    //    float fPosX = 200.0f * nCnt;
    //    CPlayer::Create(D3DXVECTOR3(-1000.0f + fPosX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), CPlayer::PLAYABLE_001);
    //}
    m_apPlayer[0] = CPlayer::Create(PLAYER_START_POS1, D3DXVECTOR3(0.0f, PLAYER_ROT_RIGHT, 0.0f), CPlayer::PLAYABLE_001);
    m_apPlayer[1] = CPlayer::Create(PLAYER_START_POS2, D3DXVECTOR3(0.0f, PLAYER_ROT_LEFT, 0.0f), CPlayer::PLAYABLE_002/*, CPlayer::AI_LEVEL_1*/);

    // �J�����̃��b�N�I���ꏊ��ς���
    CManager::GetCamera()->CCamera::ResetGameCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT);

    // �{�[���𐶐�
    m_pBall = CBall::Create(D3DXVECTOR3(0.0f, m_mapLimit.fHeight * BALL_START_BUTTLE_CREATE_POS_Y_RATE, 0.0f), true);
    m_pBall->SetAbsorb(m_apPlayer[0]);  // ���ɋz�������Ă���

    // �X�e�[�W�̃��f���𐶐�
    //CBg::Create();

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
    // �m�ۂ����X�R�A�ƃv���C���[�Ɗe����UI�̃������́ACScene��ReleaseAll�ŊJ������Ă���

    // �S�Ẳ����~
    CSound *pSound = CManager::GetSound();
    pSound->StopAll();

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

#ifdef _DEBUG
    CDebug::GameCommand();
#endif
}

////=============================================================================
//// �X�e�[�W�̓ǂݍ���
//// Author : �㓡�T�V��
////=============================================================================
//void CGame::SetStage(STAGE stage)
//{
//    // �����������o�ϐ��Ɍ��т���
//    m_stage = stage;
//
//    // �~�b�V�����N���A�̃t���O��false�ɖ߂�
//    m_bMissionClear = false;
//
//    //// �v���C���[�̏ꏊ��ς���
//    //m_pPlayer->SetPos(PLAYER_START_POS_STAGE1);
//
//    // �X�e�[�W�ɂ���āA�}�b�v�̐�����~�b�V����UI��ς���
//    CSound *pSound = CManager::GetSound();
//    // BGM���Đ�
//    pSound->Play(CSound::LABEL_BGM_STAGE_01_NORMAL);
//
//    // �G�̔z�u�ǂݍ���
//    LoadEnemyData(stage);
//
//    // �}�b�v�̓ǂݍ���
//    LoadMapData(stage);
//}

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

    // �����擾
    CSound *pSound = CManager::GetSound();

    // �J�����擾
    CCamera*pCamera = CManager::GetCamera();

    switch (m_state)
    {
    case STATE_ROUND_START:

        // �J�E���^�����Z
        m_nCntGameTime++;

        // �J�����Ǐ]
        pCamera->CCamera::SetState(CCamera::STATE_HORIZON_MOVE);

        // ���t���[���ŁA�Q�[������
        if (m_nCntGameTime == START_MISSION)
        {
            // �J�E���^���Z�b�g
            m_nCntGameTime = 0;

            //// �~�b�V�����X�^�[�g�\��
            //CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_START);

            // �o�g����
            m_state = STATE_BUTTLE;
        }

        break;

    case STATE_BUTTLE:

        // �J�����Ǐ]
        pCamera->CCamera::SetState(CCamera::STATE_HORIZON_MOVE);

        //// �o�g����
        //Buttle();

        break;

    //case STATE_FINISH:

    //    // ���s����
    //    Judgment();

    //    break;

    case STATE_PAUSE:

        // �|�[�Y�̍X�V
        m_pPause->Update();

        break;
    }
}

//=============================================================================
// �o�g����
// Author : �㓡�T�V��
//=============================================================================
void CGame::Buttle(void)
{

}

//=============================================================================
// ���s����
// Author : �㓡�T�V��
//=============================================================================
void CGame::Judgment(void)
{
    // �J�E���^�����Z
    m_nCntGameTime++;

    // �ŏ���1F�Ŕ��f���āAUI���o��
    if (m_nCntGameTime == 1)
    {
        // ���擾
        CSound *pSound = CManager::GetSound();

        //// �X�e�[�W��BGM������
        //switch (m_stage)
        //{
        //case STAGE_1:
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_01_NORMAL);
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_01_BOSS);
        //    break;
        //case STAGE_2:
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_02_NORMAL);
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_02_BOSS);
        //    break;
        //case STAGE_FINAL:
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_03_NORMAL);
        //    pSound->Stop(CSound::LABEL_BGM_STAGE_03_BOSS);
        //    break;
        //}

        //// �~�b�V�����N���A�����Ȃ�
        //if (m_bMissionClear == true)
        //{
        //    // �N���A��
        //    pSound->Play(CSound::LABEL_SE_CLEAR);

        //    // �~�b�V�����N���A��UI
        //    CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_CLEAR);
        //}
        //// �~�b�V�������s�����Ȃ�
        //else
        //{
        //    // ���s��
        //    pSound->Play(CSound::LABEL_SE_FAIL);

        //    // �~�b�V�������s��UI
        //    CUI::Create(UI_EXTEND_MISSION_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_MISSION_FAILD);
        //}
    }

    // ���̉�ʂɍs���܂ł̃J�E���^
    if (m_nCntGameTime == COUNT_WAIT_FINISH)
    {
        // �J�E���^�����Z�b�g
        m_nCntGameTime = 0;

        //// ��Ԃ��A�t�F�[�h����
        //m_state = STATE_IN_FADE;

        //// �~�b�V�����Ɏ��s�����A�܂��͍ŏI�X�e�[�W���I�����Ȃ�
        //if (m_bMissionClear == false || m_stage == STAGE_FINAL)
        //{
        //    // �X�R�A���}�l�[�W���ɓn��
        //    CManager::SetScore(m_pScore->GetScore());

        //    // ���U���g�Ɉڍs
        //    CFade::SetFade(CManager::MODE_RESULT);
        //}
        //// ���̃X�e�[�W��
        //else
        //{
        //    // �z���C�g�t�F�[�h
        //    switch (m_stage)
        //    {
        //    case STAGE_1:
        //        CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE1);
        //        break;

        //    case STAGE_2:
        //        CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, WHITE_FADE_START_COLOR, CWhiteFade::TYPE_STAGE2);
        //        break;
        //    }
        //}
    }
}

////=============================================================================
//// �}�b�v�̓ǂݍ���
//// Author : �㓡�T�V��
////=============================================================================
//void CGame::LoadMapData(STAGE stage)
//{
//    //// �t�@�C���|�C���g
//    //FILE *pFile = NULL;
//
//    //// �ϐ��錾
//    //int  nCntLoad = 0;
//    //char cReedText[128];	// �����Ƃ��ēǂݎ��p
//    //char cHeadText[256];	//
//    //char cDie[128];
//
//    //// �f�o�b�O�p�̕ϐ�
//    //int nType[256];
//    //int nWidth = 0; // ��
//    //int nDepth = 0; // ���s
//    //D3DXVECTOR3 pos[256];
//    //D3DXVECTOR3 rot[256];
//
//    //// �X�e�[�W�ɂ���āA�t�@�C�����J��
//    //switch (stage)
//    //{
//    //case STAGE_1:
//    //    pFile = fopen("data/TXT/stage1.txt", "r");
//    //    break;
//    //case STAGE_2:
//    //    pFile = fopen("data/TXT/stage2.txt", "r");
//    //    break;
//    //case STAGE_FINAL:
//    //    pFile = fopen("data/TXT/stage3.txt", "r");
//    //    break;
//    //}
//
//    //// �J����
//    //if (pFile != NULL)
//    //{
//    //    while (strcmp(cHeadText, "MODELSET") != 0)
//    //    {
//    //        fgets(cReedText, sizeof(cReedText), pFile);
//    //        sscanf(cReedText, "%s", &cHeadText);
//
//    //        if (strcmp(cHeadText, "MAP_SIZ") == 0)
//    //        {
//    //            sscanf(cReedText, "%s %s %d %d", &cDie, &cDie, &nWidth, &nDepth);
//    //        }
//    //    }
//
//    //    if (strcmp(cHeadText, "MODELSET") == 0)
//    //    {
//    //        while (strcmp(cHeadText, "END_SCRIPT") != 0)
//    //        {
//    //            fgets(cReedText, sizeof(cReedText), pFile);
//    //            sscanf(cReedText, "%s", &cHeadText);
//
//    //            if (strcmp(cHeadText, "\n") == 0)
//    //            {
//    //            }
//    //            else if (strcmp(cHeadText, "END_MODELSET") != 0)
//    //            {
//    //                if (strcmp(cHeadText, "TYPE") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
//    //                }
//
//    //                if (strcmp(cHeadText, "POS") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
//    //                }
//
//    //                if (strcmp(cHeadText, "ROT") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);
//
//    //                    // �u���b�N�𐶐�
//    //                    CBlock::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                        D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), nType[nCntLoad]);
//
//    //                    nCntLoad++;
//    //                }
//    //            }
//    //        }
//
//    //    }
//
//    //    // �t�B�[���h����
//    //    CField::Create(stage, nWidth, nDepth);
//
//    //    // �t�@�C����
//    //    fclose(pFile);
//    //}
//
//    //// �J���Ȃ�
//    //else
//    //{
//    //    printf("�J����܂���ł���\n");
//    //}
//}

////=============================================================================
//// �G�l�~�[�̔z�u�ǂݍ���
//// Author : �㓡�T�V��
////=============================================================================
//void CGame::LoadEnemyData(STAGE stage)
//{
//    //// �t�@�C���|�C���g
//    //FILE *pFile = NULL;
//
//    //// �ϐ��錾
//    //int  nCntLoad = 0;
//    //char cReedText[128];	// �����Ƃ��ēǂݎ��p
//    //char cHeadText[256];	//
//    //char cDie[128];
//
//    //// �f�o�b�O�p�̕ϐ�
//    //int nType[256];
//    //D3DXVECTOR3 pos[256];
//    //D3DXVECTOR3 rot[256];
//
//    //// �X�e�[�W�ɂ���āA�t�@�C�����J��
//    //switch (stage)
//    //{
//    //case STAGE_1:
//    //    pFile = fopen("data/TXT/enemystage1.txt", "r");
//    //    break;
//    //case STAGE_2:
//    //    pFile = fopen("data/TXT/enemystage2.txt", "r");
//    //    break;
//    //case STAGE_FINAL:
//    //    pFile = fopen("data/TXT/enemystage3.txt", "r");
//    //    break;
//    //}
//
//    //// �J����
//    //if (pFile != NULL)
//    //{
//    //    while (strcmp(cHeadText, "ENEMYSET") != 0)
//    //    {
//    //        fgets(cReedText, sizeof(cReedText), pFile);
//    //        sscanf(cReedText, "%s", &cHeadText);
//    //    }
//
//    //    if (strcmp(cHeadText, "ENEMYSET") == 0)
//    //    {
//    //        while (strcmp(cHeadText, "END_SCRIPT") != 0)
//    //        {
//    //            fgets(cReedText, sizeof(cReedText), pFile);
//    //            sscanf(cReedText, "%s", &cHeadText);
//
//    //            if (strcmp(cHeadText, "\n") == 0)
//    //            {
//    //            }
//    //            else if (strcmp(cHeadText, "END_ENEMYSET") != 0)
//    //            {
//    //                if (strcmp(cHeadText, "TYPE") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
//    //                }
//
//    //                if (strcmp(cHeadText, "POS") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
//    //                }
//
//    //                if (strcmp(cHeadText, "ROT") == 0)
//    //                {
//    //                    sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);
//
//    //                    switch (nType[nCntLoad])
//    //                    {
//    //                    case ENEMYSET_WHITE_ANT:
//    //                        // ���A���𐶐�
//    //                        CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_WHITE);
//    //                        break;
//    //                    case ENEMYSET_BLACK_ANT:
//    //                        // ���单�A���𐶐�
//    //                        CAnt::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CAnt::TYPE_BLACK);
//    //                        break;
//    //                    case ENEMYSET_YELLOW_SPIDER:
//    //                        // ���O���𐶐�
//    //                        CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_YELLOW);
//    //                        break;
//    //                    case ENEMYSET_BLACK_SPIDER:
//    //                        // ���单�O���𐶐�
//    //                        CSpider::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)), false, CSpider::TYPE_BLACK);
//    //                        break;
//    //                    case ENEMYSET_UFO:
//    //                        // UFO�𐶐�
//    //                        CUfo::Create(pos[nCntLoad], D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
//    //                            D3DXToRadian(rot[nCntLoad].y), D3DXToRadian(rot[nCntLoad].z)));
//    //                        break;
//    //                    }
//
//    //                    nCntLoad++;
//    //                }
//    //            }
//    //        }
//
//    //    }
//    //    // �t�@�C����
//    //    fclose(pFile);
//    //}
//
//    //// �J���Ȃ�
//    //else
//    //{
//    //    printf("�J����܂���ł���\n");
//    //}
//}

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
    targetPos = D3DXVECTOR3(myPos.x, 5000.0f, 0.0f);

    // ��������ԋ߂��v���C���[�����߂�i�����ȊO�Łj
    for (int nCntPlayer = 0; nCntPlayer < m_nNumCurrentPlayer; nCntPlayer++)
    {
        // �����ȊO��
        if (playable == nCntPlayer)
        {
            continue;
        }

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

    // �p�x�����߂�
    fAngle = atan2((targetPos.x - myPos.x), (targetPos.y - myPos.y));

    return fAngle;
}
//
////========================================
//// ��ԋ߂��G�̈ʒu�����߂�
////========================================
//D3DXVECTOR3 CGame::GetPosToClosestEnemy(D3DXVECTOR3 myPos)
//{
//    // �ϐ��錾
//    float fFirstDistance = BONDS_MAX_DISTANCE_TO_CLOSEST_ENEMY;  // ����
//    D3DXVECTOR3 targetPos = DEFAULT_VECTOR;                      // �Ώۂ̈ʒu
//
//                                                                 // �Ώۂ̈ʒu���A�����̈ʒu�̐^��ֈ�x���߂Ă���
//    targetPos = D3DXVECTOR3(myPos.x, 0.0f, 0.0f);
//
//    // ��������ԋ߂��G�����߂�
//    CScene *pScene = NULL;
//    pScene = CScene::GetTopScene(CScene::OBJTYPE_ENEMY);
//    for (int nCntScene = 0; nCntScene < CScene::GetNumAll(CScene::OBJTYPE_ENEMY); nCntScene++)
//    {
//        // ���g������Ȃ�
//        if (pScene != NULL)
//        {
//            // ���̃V�[�����L��
//            CScene*pNextScene = pScene->GetNextScene();
//
//            // �G�l�~�[�ɃL���X�g
//            CEnemy *pEnemy = (CEnemy*)pScene;
//
//            // �ʒu�����߂�
//            D3DXVECTOR3 enemyPos = DEFAULT_VECTOR;
//            enemyPos = pEnemy->GetPosition();
//
//            // ���������߂�
//            float fSecondDistance = sqrtf(
//                powf((myPos.x - enemyPos.x), 2) +
//                powf((myPos.y - enemyPos.y), 2));
//
//            // �����̔�r�ƁA�Ώۂ̈ʒu���X�V
//            if (fFirstDistance > fSecondDistance)
//            {
//                fFirstDistance = fSecondDistance;
//                targetPos = enemyPos;
//            }
//
//            // ���̃V�[���ɂ���
//            pScene = pNextScene;
//        }
//        else
//        {
//            // ���g���Ȃ��Ȃ�A�����ŏ������I����
//            break;
//        }
//    }
//
//    return targetPos;
//}
