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
    memset(m_keyInfo, 0, sizeof(m_keyInfo));
    m_nAnimation = 0;
    m_type = TYPE_NONE;

    for (int nCount = 0; nCount < CHARCTER_PARTS_MAX; nCount++)
    {
        m_pos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_posDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_rot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_rotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
HRESULT CAnimation::Init(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type)
{
    memset(m_keyInfo, 0, sizeof(m_keyInfo));
    m_nAnimation = 0;
    m_pCharacter = pCharacter;
    m_nFrame = 0;
    m_nKey = 0;
    m_type = type;
    m_nPartsNum = nPartsNum;

    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_pos[nCount] = m_pCharacter->GetDefaultPos(nCount);
        m_rot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_posDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_rotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
            m_posDest[nCntParts].x = ((m_keyInfo[m_type][m_nAnimation].parts[nCntParts].pos[m_nKey].x - (m_pos[nCntParts].x - m_pCharacter->GetDefaultPos(nCntParts).x)) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]));
            m_posDest[nCntParts].y = ((m_keyInfo[m_type][m_nAnimation].parts[nCntParts].pos[m_nKey].y - (m_pos[nCntParts].y - m_pCharacter->GetDefaultPos(nCntParts).y)) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]));
            m_posDest[nCntParts].z = ((m_keyInfo[m_type][m_nAnimation].parts[nCntParts].pos[m_nKey].z - (m_pos[nCntParts].z - m_pCharacter->GetDefaultPos(nCntParts).z)) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]));

            m_rotDest[nCntParts].x = (m_keyInfo[m_type][m_nAnimation].parts[nCntParts].rot[m_nKey].x - m_rot[nCntParts].x) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]);
            m_rotDest[nCntParts].y = (m_keyInfo[m_type][m_nAnimation].parts[nCntParts].rot[m_nKey].y - m_rot[nCntParts].y) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]);
            m_rotDest[nCntParts].z = (m_keyInfo[m_type][m_nAnimation].parts[nCntParts].rot[m_nKey].z - m_rot[nCntParts].z) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]);
        }
        // �l�����Z
        m_pos[nCntParts] += m_posDest[nCntParts];
        m_rot[nCntParts] += m_rotDest[nCntParts];

        // x y z�����180���ȏ��]������p�x���C��(���ɏC������Ă����ꍇ�͏������Ȃ�)
        if (m_rot[0].x >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_rot[0].x = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }
        if (m_rot[0].x <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_rot[0].x = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].y >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].y = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].y <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].y = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].z >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].z = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].z <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].z = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

		//// �v�Z������]�̒l���Ԃ����ł���
		//if (m_rot[nCntParts].x > D3DX_PI && !m_bAdjustment)
		//{
		//	m_rot[nCntParts].x = -D3DX_PI;
		//	m_bAdjustment = true;
		//}
		//if (m_rot[nCntParts].x < -D3DX_PI && !m_bAdjustment)
		//{
		//	m_rot[nCntParts].x = D3DX_PI;
		//	m_bAdjustment = true;
		//}
		//
		//if (m_rot[nCntParts].y > D3DX_PI && !m_bAdjustment)
		//{
		//	m_rot[nCntParts].y = -D3DX_PI;
		//	m_bAdjustment = true;
		//}
		//if (m_rot[nCntParts].y < -D3DX_PI && !m_bAdjustment)
		//{
		//	m_rot[nCntParts].y = D3DX_PI;
		//	m_bAdjustment = true;
		//}
		//
		//if (m_rot[nCntParts].z > D3DX_PI && !m_bAdjustment)
		//{
		//	m_rot[nCntParts].z = -D3DX_PI;
		//	m_bAdjustment = true;
		//}
		//if (m_rot[nCntParts].z < -D3DX_PI && !m_bAdjustment)
		//{
		//	m_rot[nCntParts].z = D3DX_PI;
		//	m_bAdjustment = true;
		//}

        // ��]�ƍ��W�̐ݒ�
        m_pCharacter->SetPartPos(nCntParts, m_pos[nCntParts]);
        m_pCharacter->SetPartRot(nCntParts, m_rot[nCntParts]);
    }
    // ����̃A�j���[�V���������񔻒�p�ɕێ�
    m_nAnimationOld = m_nAnimation;

    // ���[�v����Ƃ�
    if (m_nFrame >= m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey] && m_keyInfo[m_type][m_nAnimation].nLoop == 1)
    {
        // �L�[�t���[���̉��Z�ƕϐ��̏�����
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // �L�[���̔���
        if (m_nKey >= m_keyInfo[m_type][m_nAnimation].nKeyNum)
        {
            m_nKey = 0;
        }
    }
    // ���[�v���Ȃ��Ƃ�
    else if (m_nFrame >= m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey] && m_keyInfo[m_type][m_nAnimation].nLoop == 0)
    {
        // �L�[�t���[���̉��Z�ƕϐ��̏�����
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // �L�[���̔���
        if (m_nKey >= m_keyInfo[m_type][m_nAnimation].nKeyNum)
        {
            // �p�[�c�̈ړ��ʂ̃��Z�b�g
            for (int nCntMotion = 0; nCntMotion < m_nPartsNum; nCntMotion++)
            {
                m_posDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                m_rotDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
CAnimation * CAnimation::Create(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type, char* cFilePass)
{
    // �C���X�^���X�̐���
    CAnimation *pAnimation;
    pAnimation = new CAnimation;
    // ������
    pAnimation->Init(pCharacter, nPartsNum, type);
    // �A�j���[�V�����̓ǂݍ���
    pAnimation->LoadAnimation(type, cFilePass);

    return pAnimation;
}

//=============================================================================
// �A�j���[�V�����ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
void CAnimation::LoadAnimation(ANIM_TYPE type, char* pass)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    int  nCntLoad = 0;
    int	 nCntKey = 0;
    int	 nCntMotion = 0;

    char cReedText[1024];	// �����ǂݎ��p
    char cHeadText[1024];	//
    char cDie[1024];		//

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[type][nCntMotion].nLoop);
                        }
                        // �L�[���̓ǂݍ���
                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[type][nCntMotion].nKeyNum);
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
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[type][nCntMotion].nFrame[nCntKey]);
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
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].pos[nCntKey].x,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].pos[nCntKey].y,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].pos[nCntKey].z);
                                        }
                                        // ��]�̓ǂݍ���
                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].rot[nCntKey].x,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].rot[nCntKey].y,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].rot[nCntKey].z);
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
