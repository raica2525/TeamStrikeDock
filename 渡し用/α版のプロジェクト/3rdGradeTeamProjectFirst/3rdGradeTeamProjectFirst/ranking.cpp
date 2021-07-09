//===============================================
//
// �����L���O�̏��� (ranking.cpp)
// Author : �㓡�T�V��
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "ranking.h"
#include "number.h"
#include "score.h"
#include "manager.h"
#include <stdio.h>

//========================================
// �}�N����`
//========================================
#define MAX_RANKING 5                                                       // ���ʂ̍ő吔
#define YOUR_SCORE_SIZE_X 200.0f
#define YOUR_SCORE_SIZE D3DXVECTOR3(YOUR_SCORE_SIZE_X / MAX_SCORE, 40.0f, 0.0f)         // ���Ȃ��̃X�R�A�̐����̑傫��
#define RANKING_SCORE_SIZE_X 350.0f
#define RANKING_SCORE_SIZE  D3DXVECTOR3(RANKING_SCORE_SIZE_X / MAX_SCORE, 70.0f, 0.0f)  // �����L���O�̃X�R�A�̐����̑傫��

#define DOWN_POS_Y 80.0f                                    // ���ɂ��炷�l
#define SLIDE D3DXVECTOR3(0.0f, -420.0f, 0.0f)              // �����̃X�R�A���炸�炷�l

#define RANKIN_COLOR D3DXCOLOR(1.0f , 0.0f, 0.0f, 1.0f)     // �����N�C�������Ƃ��̐F

// �e���ʂ́A�t�F�[�h�C��
#define FADE_IN_5 60
#define FADE_IN_4 150
#define FADE_IN_3 255
#define FADE_IN_2 375
#define FADE_IN_1 495
#define STOP 1000               // �J�E���^�̃X�g�b�v
#define FADE_IN_START_NUMBER 8  // �����̃X�R�A�̎��̐�������t�F�[�h�C���J�n
#define FADE_IN_RATE 0.05f      // �t�F�[�h�C������

// �e�����́A�t�F�[�h�C�����̃f�B���C
#define DELAY_ALPHA 0.5f

//========================================
// �ÓI�����o�ϐ��錾
//========================================
CNumber *CRanking::m_apNumber[MAX_RANKING_NUMBER] = {};

//========================================
// �����L���O�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CRanking::CRanking() :CScene(OBJTYPE::OBJTYPE_UI)
{
    m_nCntTime = 0;

    // �z��̏�����
    memset(m_aRankingScore, 0, sizeof(m_aRankingScore));
    memset(m_aRank, 0, sizeof(m_aRank));
    memset(m_aAlpha, 0, sizeof(m_aAlpha));
}

//========================================
// �����L���O�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CRanking::~CRanking()
{

}

//========================================
// �����L���O�̐���
// Author : �㓡�T�V��
//========================================
CRanking* CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CRanking *pRanking = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pRanking = new CRanking;

    // ������
    pRanking->Init(pos, size);

    return pRanking;
}

//========================================
// �����L���O�̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �����̃X�R�A
    // �傫���ʂ���A���ꂼ��Create
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - YOUR_SCORE_SIZE_X * (MAX_SCORE - 1) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[0] = 1.0f;

    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - YOUR_SCORE_SIZE_X * (MAX_SCORE - 3) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[1] = 1.0f;

    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(pos.x - YOUR_SCORE_SIZE_X * (MAX_SCORE - 5) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[2] = 1.0f;

    m_apNumber[3] = CNumber::Create(D3DXVECTOR3(pos.x - YOUR_SCORE_SIZE_X * (MAX_SCORE - 7) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[3] = 1.0f;

    m_apNumber[4] = CNumber::Create(D3DXVECTOR3(pos.x + YOUR_SCORE_SIZE_X * (MAX_SCORE - 7) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[4] = 1.0f;

    m_apNumber[5] = CNumber::Create(D3DXVECTOR3(pos.x + YOUR_SCORE_SIZE_X * (MAX_SCORE - 5) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[5] = 1.0f;

    m_apNumber[6] = CNumber::Create(D3DXVECTOR3(pos.x + YOUR_SCORE_SIZE_X * (MAX_SCORE - 3) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[6] = 1.0f;

    m_apNumber[7] = CNumber::Create(D3DXVECTOR3(pos.x + YOUR_SCORE_SIZE_X * (MAX_SCORE - 1) / MAX_SCORE / 2, pos.y, 0.0f),
        YOUR_SCORE_SIZE, DEFAULT_COLOR, false);
    m_aAlpha[7] = 1.0f;

    // �����̃X�R�A��\��
    SetNumber(CManager::GetScore(), 0);

    // �����L���O�擾
    ReadScore();

    // �ϐ��錾
    int nIndex = 8;             // m_apNumber�̃C���f�b�N�X
    float fPosDownY = 0.0f;     // �ǂꂾ�������L���O�����ɂ��炷��
    bool bOnlyOnce = false;     // �F��ς���͈̂��̂�(�����_���łǂ�����F���ς��̂����)
    for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
    {
        // �ϐ��錾
        D3DXCOLOR color = DEFAULT_COLOR;    // �����̐F
        bool bRankIn = false;               // �����N�C���������ǂ���

                                            // �����N�C���������ǂ������A�e���ʂ̃X�R�A�𑗂邱�ƂŎ擾
        bRankIn = RankIn(m_aRankingScore[nCnt]);

        // �����N�C�������Ȃ�A�F��ς���
        if (bRankIn == true && bOnlyOnce == false)
        {
            // �����N�C���̐F
            color = RANKIN_COLOR;

            // ���݂̂̃t���O��true��
            bOnlyOnce = true;
        }

        // �傫���ʂ���A���ꂼ��Create
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x - RANKING_SCORE_SIZE_X * (MAX_SCORE - 1) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x - RANKING_SCORE_SIZE_X * (MAX_SCORE - 3) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x - RANKING_SCORE_SIZE_X * (MAX_SCORE - 5) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x - RANKING_SCORE_SIZE_X * (MAX_SCORE - 7) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x + RANKING_SCORE_SIZE_X * (MAX_SCORE - 7) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x + RANKING_SCORE_SIZE_X * (MAX_SCORE - 5) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x + RANKING_SCORE_SIZE_X * (MAX_SCORE - 3) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;
        m_apNumber[nIndex] = CNumber::Create(D3DXVECTOR3(pos.x + RANKING_SCORE_SIZE_X * (MAX_SCORE - 1) / MAX_SCORE / 2,
            pos.y + fPosDownY, 0.0f) + SLIDE,
            RANKING_SCORE_SIZE, color, false);
        nIndex++;

        // ���ɂ��炷�l�����Z
        fPosDownY += DOWN_POS_Y;

        // �ԍ���ݒ�
        SetNumber(m_aRankingScore[nCnt], nIndex - MAX_SCORE);
    }

    return S_OK;
}

//========================================
// �����L���O�̏I������
// Author : �㓡�T�V��
//========================================
void CRanking::Uninit(void)
{
    // �����L���O�̐����̐�����
    for (int nCntNumber = 0; nCntNumber < MAX_RANKING_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            // �������̏I������
            m_apNumber[nCntNumber]->Uninit();

            // �������̃������̊J��
            ReleaseNumber(nCntNumber);
        }
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//========================================
// �����L���O�N���X�������Ă���A�����̊J������
// Author : �㓡�T�V��
//========================================
void CRanking::ReleaseNumber(int nCntNumber)
{
    // �������̊J��
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// �����L���O�̍X�V����
// Author : �㓡�T�V��
//========================================
void CRanking::Update(void)
{

}

//========================================
// �����L���O�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CRanking::Draw(void)
{
    // �����L���O�̐����̐�����
    for (int nCntNumber = 0; nCntNumber < MAX_RANKING_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// �����ݒ菈��
// Author : �㓡�T�V��
//========================================
void CRanking::SetNumber(int nPlayScore, int nStartIndex)
{
    // �ϐ��錾
    int nIndex;		        // �w���@(�X�R�A�\���p)
    int nRadix = 10;	    // ��@(�X�R�A�\���p)

                            // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);

        // 
        int nAnswer = nPlayScore % nScore / nScore2;

        // �ԍ���ݒ�
        m_apNumber[nStartIndex]->SetNumber(nAnswer);

        // �󂯎�����C���f�b�N�X��i�߂�
        nStartIndex++;
    }
}

//==============================================================
// �e�����L���O�̃X�R�A�̓ǂݍ���
// Author : �㓡�T�V��
//==============================================================
void CRanking::ReadScore(void)
{
    FILE *pFile = NULL; //�t�@�C���|�C���^

                        // �����L���O���t�@�C������擾
    pFile = fopen("data/TXT/score.txt", "r");
    if (pFile != NULL)
    {//�t�@�C���I�[�v������
        for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
        {
            fscanf(pFile, "%d", &m_aRankingScore[nCntScore]);
        }
        fclose(pFile);
    }

    // �v���C�X�R�A���L��
    m_aRankingScore[RANKING_SCORE_MAX - 1] = CManager::GetScore();

    //�����N�\�[�g
    for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
    {
        m_aRank[nCntScore] = 0;
        for (int nData = 0; nData < RANKING_SCORE_MAX; nData++)
        {
            if (m_aRankingScore[nCntScore] < m_aRankingScore[nData])
            {
                m_aRank[nCntScore]++;
            }
        }
    }

    //�X�R�A���͏���
    pFile = fopen("data/TXT/score.txt", "w");
    if (pFile != NULL)
    {//�t�@�C���I�[�v������
        for (int nCntRank = 0; nCntRank < RANKING_SCORE_MAX; nCntRank++)
        {
            for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
            {
                if (m_aRank[nCntScore] == nCntRank)
                {
                    fprintf(pFile, "%d\n", m_aRankingScore[nCntScore]);
                }
            }
        }
        fclose(pFile);
    }

    //�X�R�A�o�͏���
    pFile = fopen("data/TXT/score.txt", "r");
    if (pFile != NULL)
    {//�t�@�C���I�[�v������
        for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
        { //��ʃX�R�A�̏o�͏���
            fscanf(pFile, "%d", &m_aRankingScore[nCntScore]);
        }
        fclose(pFile);
    }
}

//==============================================================
// �����N�C���������ǂ���
// Author : �㓡�T�V��
//==============================================================
bool CRanking::RankIn(int nScore)
{
    // �ϐ��錾
    bool bRankIn = false;

    // ��r
    if (CManager::GetScore() == nScore)
    {
        bRankIn = true;
    }

    return bRankIn;
}

//==============================================================
// �����̃t�F�[�h�C��
// Author : �㓡�T�V��
//==============================================================
void CRanking::FadeInNumber(int nCnt)
{
    // �t�F�[�h�C�����鐔���Ȃ�
    if (nCnt >= FADE_IN_START_NUMBER)
    {
        // 1�ʂ̍ō�������A5�ʂ̍Œጅ�܂ł����ւ���
        int nSwapNumber = SwapScoreNumberIndex(nCnt);

        // �����x��1�����Ȃ�
        if (m_aAlpha[nSwapNumber] < 1.0f)
        {
            // �ϐ��錾
            int nBorder = 100;    // �����x���グ��A���E��(�ŏ��́A����100)

                                  // ���Ԃ��i�ނɂ�A�{�[�_�[�������Ă����C���[�W
                                  // 5��
            if (m_nCntTime >= FADE_IN_5)
            {
                nBorder = 40;
            }
            // 4��
            if (m_nCntTime >= FADE_IN_4)
            {
                nBorder = 32;
            }
            // 3��
            if (m_nCntTime >= FADE_IN_3)
            {
                nBorder = 24;
            }
            // 2��
            if (m_nCntTime >= FADE_IN_2)
            {
                nBorder = 16;
            }
            // 1��
            if (m_nCntTime >= FADE_IN_1)
            {
                nBorder = 8;
            }

            // �����x���グ��{�[�_�[�ȏ�Ȃ�
            if (nSwapNumber >= nBorder)
            {
                // �ŉ��ʂ̍ŏ��̌��Ȃ�
                if (nSwapNumber == MAX_RANKING_NUMBER - 1)
                {
                    // �t�F�[�h�C��
                    m_aAlpha[nSwapNumber] += FADE_IN_RATE;
                }
                // ����ȊO�Ȃ�
                else
                {
                    // (�E���琔����)�ЂƂO�̐����̃��l���A���ȏ�Ȃ�
                    if (m_aAlpha[nSwapNumber + 1] >= DELAY_ALPHA)
                    {
                        // �t�F�[�h�C��
                        m_aAlpha[nSwapNumber] += FADE_IN_RATE;
                    }
                }
            }
        }
    }
}

//==============================================================
// �X�R�A�̐����̃C���f�b�N�X�����ւ���
// Author : �㓡�T�V��
//==============================================================
int CRanking::SwapScoreNumberIndex(int nIndex)
{
    // �ϐ��錾
    int nReturnNum = 0; // �Ԃ��l

    for (int nCntNumber = 0; nCntNumber < MAX_RANKING_NUMBER; nCntNumber++)
    {
        // �����Z�̎�
        int nFormula = nIndex + nCntNumber;

        // �������l���A�����L���O�̐����̃C���f�b�N�X�̒[�ƒ[�𑫂����l�Ȃ�i��: 8+47 = 55�A9+46 = 55�A10+45 = 55�j
        if (nFormula == (FADE_IN_START_NUMBER + MAX_RANKING_NUMBER - 1))
        {
            // �Ԃ��l�́A�����Ă��������ɂȂ�
            nReturnNum = nCntNumber;

            // for�𔲂���
            break;
        }
    }

    return nReturnNum;
}