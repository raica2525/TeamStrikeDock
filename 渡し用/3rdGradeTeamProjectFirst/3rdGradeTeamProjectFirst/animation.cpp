//===============================================
//
// �A�j���[�V�������� (animation.cpp)
// Author : �㓡�T�V��
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "animation.h"
#include "game.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CAnimation::CAnimation()
{
    memset(m_aKeyInfo, 0, sizeof(m_aKeyInfo));
    m_nAnimation = 0;

    for (int nCount = 0; nCount < CHARCTER_PARTS_MAX; nCount++)
    {
        m_aPos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aPosDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aRot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aRotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }

    m_nAnimationOld = 0;
    m_pCharacter = NULL;

    m_nFrame = 0;
    m_nKey = 0;
    m_nPartsNum = 0;
    m_bAdjustment = false;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CAnimation::~CAnimation()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CAnimation::Init(CCharacter *pCharacter, int nPartsNum)
{
    memset(m_aKeyInfo, 0, sizeof(m_aKeyInfo));
    m_nAnimation = 0;
    m_pCharacter = pCharacter;
    m_nFrame = 0;
    m_nKey = 0;
    m_nPartsNum = nPartsNum;

    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_aPos[nCount] = m_pCharacter->GetDefaultPos(nCount);
        m_aRot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aPosDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aRotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CAnimation::Uninit(void)
{
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CAnimation::Update(void)
{
    //���[�V�������ς�����Ƃ�
    if (m_nAnimationOld != m_nAnimation)
    {
        m_nFrame = 0;
        m_nKey = 0;
    }
    // ���f���̃p�[�c���X�V
    for (int nCntParts = 0; nCntParts < m_nPartsNum; nCntParts++)
    {
        // 0�t���[���ڂȂ�p�[�c�̃t���[�����Ƃ̈ړ��ʂ��v�Z
        if (m_nFrame == 0)
        {
            m_aPosDest[nCntParts].x = ((m_aKeyInfo[m_nAnimation].aParts[nCntParts].aPos[m_nKey].x - (m_aPos[nCntParts].x - m_pCharacter->GetDefaultPos(nCntParts).x)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]));
            m_aPosDest[nCntParts].y = ((m_aKeyInfo[m_nAnimation].aParts[nCntParts].aPos[m_nKey].y - (m_aPos[nCntParts].y - m_pCharacter->GetDefaultPos(nCntParts).y)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]));
            m_aPosDest[nCntParts].z = ((m_aKeyInfo[m_nAnimation].aParts[nCntParts].aPos[m_nKey].z - (m_aPos[nCntParts].z - m_pCharacter->GetDefaultPos(nCntParts).z)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]));

            m_aRotDest[nCntParts].x = (m_aKeyInfo[m_nAnimation].aParts[nCntParts].aRot[m_nKey].x - m_aRot[nCntParts].x) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]);
            m_aRotDest[nCntParts].y = (m_aKeyInfo[m_nAnimation].aParts[nCntParts].aRot[m_nKey].y - m_aRot[nCntParts].y) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]);
            m_aRotDest[nCntParts].z = (m_aKeyInfo[m_nAnimation].aParts[nCntParts].aRot[m_nKey].z - m_aRot[nCntParts].z) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]);
        }

        // �l�����Z
        m_aPos[nCntParts] += m_aPosDest[nCntParts];
        m_aRot[nCntParts] += m_aRotDest[nCntParts];

        // x y z�����180���ȏ��]������p�x���C��(���ɏC������Ă����ꍇ�͏������Ȃ�)
        if (m_aRot[0].x >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_aRot[0].x = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }
        if (m_aRot[0].x <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_aRot[0].x = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].y >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].y = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].y <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].y = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].z >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].z = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].z <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].z = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        // ��]�ƍ��W�̐ݒ�
        m_pCharacter->CCharacter::SetPartPos(nCntParts, m_aPos[nCntParts]);
        m_pCharacter->CCharacter::SetPartRot(nCntParts, m_aRot[nCntParts]);
    }
    // ����̃A�j���[�V���������񔻒�p�ɕێ�
    m_nAnimationOld = m_nAnimation;

    // ���[�v����Ƃ�
    if (m_nFrame >= m_aKeyInfo[m_nAnimation].anFrame[m_nKey] && m_aKeyInfo[m_nAnimation].nLoop == 1)
    {
        // �L�[�t���[���̉��Z�ƕϐ��̏�����
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // �L�[���̔���
        if (m_nKey >= m_aKeyInfo[m_nAnimation].nKeyNum)
        {
            m_nKey = 0;
        }
    }
    // ���[�v���Ȃ��Ƃ�
    else if (m_nFrame >= m_aKeyInfo[m_nAnimation].anFrame[m_nKey] && m_aKeyInfo[m_nAnimation].nLoop == 0)
    {
        // �L�[�t���[���̉��Z�ƕϐ��̏�����
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // �L�[���̔���
        if (m_nKey >= m_aKeyInfo[m_nAnimation].nKeyNum)
        {
            // �p�[�c�̈ړ��ʂ̃��Z�b�g
            for (int nCntMotion = 0; nCntMotion < m_nPartsNum; nCntMotion++)
            {
                m_aPosDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                m_aRotDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }

            // �A�j���[�V������ҋ@���[�V�����ɐݒ�
            m_nAnimation = 0;
            m_nKey = 0;
        }
    }
    else
    {
        // �t���[���𐔉��Z
        m_nFrame++;
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CAnimation::Draw(void)
{
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CAnimation * CAnimation::Create(CCharacter *pCharacter, int nPartsNum, char* cFilePass)
{
    // �C���X�^���X�̐���
    CAnimation *pAnimation;
    pAnimation = new CAnimation;
    // ������
    pAnimation->Init(pCharacter, nPartsNum);
    // �A�j���[�V�����̓ǂݍ���
    pAnimation->LoadAnimation(cFilePass);

    return pAnimation;
}

//=============================================================================
// �A�j���[�V�����ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
void CAnimation::LoadAnimation(char* pass)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    int  nCntLoad = 0;
    int	 nCntKey = 0;
    int	 nCntMotion = 0;

    char cReedText[2048];	// �����ǂݎ��p
    char cHeadText[2048];	//
    char cDie[2048];		//

    // �t�@�C���J
    pFile = fopen(pass, "r");

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            // END_SCRIPT���ǂݍ��܂��܂ŌJ��Ԃ�
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "MOTIONSET") == 0)
                {
                    // END_MOTIONSET���ǂݍ��܂��܂ŌJ��Ԃ�
                    while (strcmp(cHeadText, "END_MOTIONSET") != 0)
                    {
                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        // ���[�v���邩�ǂ����̓ǂݍ���
                        if (strcmp(cHeadText, "LOOP") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_aKeyInfo[nCntMotion].nLoop);
                        }
                        // �L�[���̓ǂݍ���
                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_aKeyInfo[nCntMotion].nKeyNum);
                        }
                        // �L�[�Z�b�g�̓ǂݍ���
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {
                            // END_KEYSET���ǂݍ��܂��܂ŌJ��Ԃ�
                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                // �t���[�����̓ǂݍ���
                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_aKeyInfo[nCntMotion].anFrame[nCntKey]);
                                }
                                // �L�[�f�[�^�̓ǂݍ���
                                if (strcmp(cHeadText, "KEY") == 0)
                                {
                                    // END_KEY���ǂݍ��܂��܂ŌJ��Ԃ�
                                    while (strcmp(cHeadText, "END_KEY") != 0)
                                    {
                                        fgets(cReedText, sizeof(cReedText), pFile);
                                        sscanf(cReedText, "%s", &cHeadText);

                                        // ���W�̓ǂݍ���
                                        if (strcmp(cHeadText, "POS") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aPos[nCntKey].x,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aPos[nCntKey].y,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aPos[nCntKey].z);
                                        }
                                        // ��]�̓ǂݍ���
                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aRot[nCntKey].x,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aRot[nCntKey].y,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aRot[nCntKey].z);
                                        }
                                    }
                                    // �ϐ��̃C���N�������g
                                    nCntLoad++;
                                }
                            }
                            nCntLoad = 0;
                            // �ϐ��̃C���N�������g
                            nCntKey++;
                        }
                    }
                    nCntKey = 0;
                    // �ϐ��̃C���N�������g
                    nCntMotion++;
                }
            }
        }
        // �t�@�C����
        fclose(pFile);
    }
    // �J���Ȃ�
    else
    {
        printf("�J����܂���ł���\n");
    }
}

//=============================================================================
// �A�j���[�V�����̈ʒu�����Z�b�g
// Author : �㓡�T�V��
//=============================================================================
void CAnimation::SetAnimPosReset(int nNum)
{
    // �匳�̐e�p�[�c�́A�����Ȃ�ʒu��ς���ƃK�^�����߁A�X�V�����Ŏ擾������
    if (nNum != 0)
    {
        // ���݂̈ʒu�ɏ����ʒu����
        m_aPos[nNum].x = m_pCharacter->GetDefaultPos(nNum).x;
        m_aPos[nNum].y = m_pCharacter->GetDefaultPos(nNum).y;
        m_aPos[nNum].z = m_pCharacter->GetDefaultPos(nNum).z;

        // �c��L�[�t���[�������l�������A�ړI�l����
        m_aPosDest[nNum].x = ((m_aKeyInfo[m_nAnimation].aParts[nNum].aPos[m_nKey].x - (m_aPos[nNum].x - m_pCharacter->GetDefaultPos(nNum).x)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey] - m_nFrame + 1));
        m_aPosDest[nNum].y = ((m_aKeyInfo[m_nAnimation].aParts[nNum].aPos[m_nKey].y - (m_aPos[nNum].y - m_pCharacter->GetDefaultPos(nNum).y)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey] - m_nFrame + 1));
        m_aPosDest[nNum].z = ((m_aKeyInfo[m_nAnimation].aParts[nNum].aPos[m_nKey].z - (m_aPos[nNum].z - m_pCharacter->GetDefaultPos(nNum).z)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey] - m_nFrame + 1));
    }
}