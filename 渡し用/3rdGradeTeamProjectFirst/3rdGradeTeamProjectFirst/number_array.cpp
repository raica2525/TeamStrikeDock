//===============================================
//
// �����z��̏��� (number_array.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "number_array.h"
#include "number.h"

//========================================
// �����z��̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CNumberArray::CNumberArray() :CScene(OBJTYPE::OBJTYPE_UI_FRONT_TEXT)
{
    m_nTexType = 0;
    memset(m_apNumber, 0, sizeof(m_apNumber));

    m_createPos = DEFAULT_VECTOR;
    m_createColor = DEFAULT_COLOR;
    m_bDispUselessDigits = true;
    m_fSizeX = 0.0f;
}

//========================================
// �����z��̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CNumberArray::~CNumberArray()
{

}

//========================================
// �����z��̐���
// Author : �㓡�T�V��
//========================================
CNumberArray* CNumberArray::Create(int nTexType, D3DXVECTOR3 pos, float fSizeX, D3DXCOLOR col, int nDispNumber, bool bDispUselessDigits, bool bMove)
{
    CNumberArray *pNumberArray = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pNumberArray = new CNumberArray;

    // ���������т���
    pNumberArray->m_nTexType = nTexType;
    pNumberArray->m_createPos = pos;
    pNumberArray->m_fSizeX = fSizeX;
    pNumberArray->m_createColor = col;
    pNumberArray->m_bDispUselessDigits = bDispUselessDigits;

    // ������
    pNumberArray->Init(pos, DEFAULT_VECTOR);

    // ����������
    pNumberArray->SetDispNumber(nDispNumber, bMove);

    return pNumberArray;
}

//========================================
// �����z��̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CNumberArray::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{

    return S_OK;
}

//========================================
// �����z��̏I������
// Author : �㓡�T�V��
//========================================
void CNumberArray::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER_ARRAY; nCntNumber++)
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
// �����z��N���X�������Ă���A�����̊J������
// Author : �㓡�T�V��
//========================================
void CNumberArray::ReleaseNumber(int nCntNumber)
{
    // �������̊J��
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// �����z��̍X�V����
// Author : �㓡�T�V��
//========================================
void CNumberArray::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER_ARRAY; nCntNumber++)
    {
        // �ϐ��錾
        bool bUse = true;

        // ���g������Ȃ�
        if (m_apNumber[nCntNumber])
        {
            bUse = m_apNumber[nCntNumber]->Update();

            // ���g�p�ɂȂ�����
            if (!bUse)
            {
                // �������̃������̊J��
                ReleaseNumber(nCntNumber);
            }
        }
    }
}

//========================================
// �����z��̕`�揈��
// Author : �㓡�T�V��
//========================================
void CNumberArray::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER_ARRAY; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// �\�����鐔����ݒ�
// Author : �㓡�T�V��
//========================================
void CNumberArray::SetDispNumber(int nDispNumber, bool bMove)
{
    // �ϐ��錾
    int nIndex;		                      // �w���@(�����z��\���p)
    int nRadix = 10;	                  // ��@(�����z��\���p)
    float fSpace = 0.0f;                  // �����̊Ԃ̊Ԋu
    bool bCheckStart = false;             // �������n�܂�܂ł̃`�F�b�N(������E�ւƐ��������邽��)(��:00000500�Ȃǂ��Ȃ���)
    bool bDispZeroOnce = false;           // ��x��0�̔z�u���I��������ǂ���
    int nNumDigit = 0;                    // ����
    bool bGetDigit = false;               // �������󂯎�������ǂ���

    // ��������ɂ���āA�Ԋu�̃X�^�[�g��ς���
    if (MAX_NUMBER_ARRAY % 2 == 0)
    {
        fSpace = -(m_fSizeX * ((float)MAX_NUMBER_ARRAY * 0.5f)) + (m_fSizeX * 0.5f);
    }
    else
    {
        fSpace = -(m_fSizeX * ((float)MAX_NUMBER_ARRAY * 0.5f - 0.5f));
    }

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_NUMBER_ARRAY; nIndex++)
    {
        int nNumberArray = (int)powf((float)nRadix, MAX_NUMBER_ARRAY - (float)nIndex);
        int nNumberArray2 = (int)powf((float)nRadix, MAX_NUMBER_ARRAY - (float)nIndex - 1);
        int nAnswer = nDispNumber % nNumberArray / nNumberArray2;

        // ��������ĂȂ��Ȃ�A�z��ɓ����
        if (!m_apNumber[nIndex])
        {
            m_apNumber[nIndex] = CNumber::Create(m_nTexType, D3DXVECTOR3(m_createPos.x + fSpace, m_createPos.y, 0.0f),
                D3DXVECTOR3(m_fSizeX, m_fSizeX * 2.0f, 0.0f), m_createColor, bMove, m_createPos);
        }
        m_apNumber[nIndex]->SetAlpha(0.0f);   // ��x�����Ȃ��悤��
        fSpace += m_fSizeX;        // �����̑傫�������炷

        // ���ʂȌ���\�����邩�ǂ���
        if (!m_bDispUselessDigits)
        {
            // 0�ȏ�̐����������
            if (nAnswer > 0)
            {
                // �����J�n�̃`�F�b�N��true
                bCheckStart = true;
            }

            // 0�̎�
            if (nDispNumber == 0)
            {
                // �����J�n�̃`�F�b�N��true
                bCheckStart = true;
            }
        }
        else
        {
            bCheckStart = true;
        }

        // �������J�n���Ă���Ȃ�
        if (bCheckStart)
        {
            // 0�̎�
            if (nDispNumber == 0)
            {
                // ���ʂȌ����\������Ȃ�
                if (m_bDispUselessDigits)
                {
                    m_apNumber[nIndex]->SetAlpha(1.0f);
                    m_apNumber[nIndex]->SetNumber(nAnswer);
                }
                else
                {
                    // ���ʂȌ���\�����Ȃ��Ȃ�A��x����0��u��
                    if (!bDispZeroOnce)
                    {
                        bDispZeroOnce = true;
                        m_apNumber[nIndex]->SetAlpha(1.0f);
                        m_apNumber[nIndex]->SetNumber(nAnswer);

                        // �ʒu�𔽉f
                        m_apNumber[nIndex]->SetPos(D3DXVECTOR3(m_createPos.x, m_createPos.y, 0.0f));
                    }
                }
            }
            else
            {
                // ���ʂȌ���\�����Ă��Ȃ��Ȃ�A�ʒu����
                if (!m_bDispUselessDigits)
                {
                    // �������󂯎���Ă��Ȃ��Ȃ�A�󂯎��
                    if (!bGetDigit)
                    {
                        bGetDigit = true;
                        nNumDigit = MAX_NUMBER_ARRAY - nIndex;

                        // ��������ɂ���āA�Ԋu�̃X�^�[�g��ς���
                        if (nNumDigit % 2 == 0)
                        {
                            // ����
                            fSpace = -(m_fSizeX * ((float)nNumDigit * 0.5f)) + (m_fSizeX * 0.5f);
                        }
                        else
                        {
                            // �
                            fSpace = -(m_fSizeX * ((float)nNumDigit * 0.5f - 0.5f));
                        }
                    }

                    // ������̏��𒲐�
                    m_apNumber[nIndex]->SetDigitInfo(nNumDigit, MAX_NUMBER_ARRAY - nIndex);

                    // �ʒu�𔽉f
                    m_apNumber[nIndex]->SetPos(D3DXVECTOR3(m_createPos.x + fSpace, m_createPos.y, 0.0f));
                }

                // �ԍ���ݒ�
                m_apNumber[nIndex]->SetAlpha(1.0f);
                m_apNumber[nIndex]->SetNumber(nAnswer); // �����Œ��_���̍X�V���s���Ă���̂ŁA������O�Ɉʒu�Ȃǒ�������
            }
        }
    }
}