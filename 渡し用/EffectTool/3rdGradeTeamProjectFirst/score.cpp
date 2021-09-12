//===============================================
//
// �X�R�A�̏��� (score.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "score.h"
#include "number.h"

//========================================
// �}�N����`
//========================================
#define SCORE_NUMBER_SIZE_X 20.0f                                 // �X�R�A�̐����̉��̑傫��
#define NUMBER_SIZE D3DXVECTOR3(SCORE_NUMBER_SIZE_X, 40.0f, 0.0f) // �X�R�A�̐����̑傫��

//========================================
// �ÓI�����o�ϐ��錾
//========================================
CNumber *CScore::m_apNumber[MAX_NUMBER] = {};

//========================================
// �X�R�A�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CScore::CScore() :CScene(OBJTYPE::OBJTYPE_UI)
{
    m_nScore = 0;
}

//========================================
// �X�R�A�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CScore::~CScore()
{

}

//========================================
// �X�R�A�̐���
// Author : �㓡�T�V��
//========================================
CScore* CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CScore *pScore = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pScore = new CScore;

    // ������
    pScore->Init(pos, size);

    return pScore;
}

//========================================
// �X�R�A�̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ����̃X�R�A
    // �傫���ʂ���A���ꂼ��Create
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 1) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 3) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 5) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[3] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 7) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[4] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 7) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[5] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 5) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[6] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 3) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[7] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 1) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// �X�R�A�̏I������
// Author : �㓡�T�V��
//========================================
void CScore::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
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
// �X�R�A�N���X�������Ă���A�����̊J������
// Author : �㓡�T�V��
//========================================
void CScore::ReleaseNumber(int nCntNumber)
{
    // �������̊J��
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// �X�R�A�̍X�V����
// Author : �㓡�T�V��
//========================================
void CScore::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
    {
        // �ϐ��錾
        bool bUse = true;   // �g�p��

                            // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            bUse = m_apNumber[nCntNumber]->Update();

            // ���g�p�ɂȂ�����
            if (bUse == false)
            {
                // �������̃������̊J��
                ReleaseNumber(nCntNumber);
            }
        }
    }
}

//========================================
// �X�R�A�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CScore::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// �X�R�A�̕\������
// Author : �㓡�T�V��
//========================================
void CScore::SetScore(const int nDisplayScore, D3DXVECTOR3 pos, D3DXCOLOR col)
{
    // �ϐ��錾
    int nIndex;		                      // �w���@(�X�R�A�\���p)
    int nRadix = 10;	                  // ��@(�X�R�A�\���p)
    float fSpace = -SCORE_NUMBER_SIZE_X;  // �����̊Ԃ̊Ԋu(�኱���ւ��炵�āA�|�����G�̐^�񒆕ӂ�Ɍ�����)
    bool bCheckStart = false;             // �������n�܂�܂ł̃`�F�b�N(������E�ւƐ��������邽��)(��:00000500�Ȃǂ��Ȃ���)

                                          // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);
        int nAnswer = nDisplayScore % nScore / nScore2;

        // 0�ȊO�̐����������
        if (nAnswer > 0)
        {
            // �����J�n�̃`�F�b�N��true
            bCheckStart = true;
        }

        // �������J�n���Ă���Ȃ�
        if (bCheckStart == true)
        {
            // �X�R�A�������Ă���A�����̔z����Q��
            for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
            {
                // ��Ȃ�
                if (m_apNumber[nCntNumber] == NULL)
                {
                    // �z��ɓ����
                    m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(pos.x + fSpace, pos.y, 0.0f), NUMBER_SIZE, col, true);

                    // �ԍ���ݒ�
                    m_apNumber[nCntNumber]->SetNumber(nAnswer);

                    // �Ԋu�����Z
                    fSpace += SCORE_NUMBER_SIZE_X;

                    // �󂢂Ă��郁�����Ԓn���擾������for�𔲂���
                    break;
                }
            }
        }
    }
}

//========================================
// �X�R�A�̉��Z����
// Author : �㓡�T�V��
//========================================
void CScore::AddScore(const int nValue)
{
    // �X�R�A���Z
    m_nScore += nValue;

    // �ϐ��錾
    int nIndex;		        // �w���@(�X�R�A�\���p)
    int nRadix = 10;	    // ��@(�X�R�A�\���p)

                            // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);

        // 
        int nAnswer = m_nScore % nScore / nScore2;

        // �ԍ���ݒ�
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// �X�R�A�̐ݒ菈��(��ɁA�R���e�B�j���[���Ɏg��)
// Author : �㓡�T�V��
//========================================
void CScore::SetScore(const int nScore)
{
    // �X�R�A�����т���
    m_nScore = nScore;

    // �ϐ��錾
    int nIndex;		        // �w���@(�X�R�A�\���p)
    int nRadix = 10;	    // ��@(�X�R�A�\���p)

                            // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);

        // 
        int nAnswer = m_nScore % nScore / nScore2;

        // �ԍ���ݒ�
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// �_���[�W�̕\������
// Author : �㓡�T�V��
//========================================
void CScore::SetDamage(const int nDisplayDamage, D3DXVECTOR3 pos, D3DXCOLOR col)
{
    // �ϐ��錾
    int nIndex;		                      // �w���@(�X�R�A�\���p)
    int nRadix = 10;	                  // ��@(�X�R�A�\���p)
    float fSpace = -SCORE_NUMBER_SIZE_X;  // �����̊Ԃ̊Ԋu(�኱���ւ��炵�āA�|�����G�̐^�񒆕ӂ�Ɍ�����)
    bool bCheckStart = false;             // �������n�܂�܂ł̃`�F�b�N(������E�ւƐ��������邽��)(��:00000500�Ȃǂ��Ȃ���)

                                          // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);
        int nAnswer = nDisplayDamage % nScore / nScore2;

        // 0�ȏ�̐����������
        if (nAnswer > 0)
        {
            // �����J�n�̃`�F�b�N��true
            bCheckStart = true;
        }

        // 0�_���[�W�̎�
        if (nDisplayDamage == 0)
        {
            // �����J�n�̃`�F�b�N��true
            bCheckStart = true;
        }

        // �������J�n���Ă���Ȃ�
        if (bCheckStart == true)
        {
            // �X�R�A�������Ă���A�����̔z����Q��
            for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
            {
                // ��Ȃ�
                if (m_apNumber[nCntNumber] == NULL)
                {
                    // �z��ɓ����
                    m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(pos.x + fSpace, pos.y, 0.0f), NUMBER_SIZE, col, true);

                    // �ԍ���ݒ�
                    m_apNumber[nCntNumber]->SetNumber(nAnswer);

                    // �Ԋu�����Z
                    fSpace += SCORE_NUMBER_SIZE_X;

                    // �󂢂Ă��郁�����Ԓn���擾������for�𔲂���
                    break;
                }
            }
        }

        // 0�_���[�W�̎�
        if (nDisplayDamage == 0)
        {
            // for�𔲂���
            break;
        }
    }
}
