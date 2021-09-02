//=============================================================================
//
// �e�N�X�`���̃f�[�^�̏��� [texture.cpp]
// Author : �㓡�T�V��
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTURE_FILENAME ("data/TXT/texture.txt")	// �e�N�X�`����ǂݍ��ރt�@�C���̖��O

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CTexture::CTexture()
{
    // �O���t�@�C�����Ŗ��ݒ�̂��̂��������߁A�����I�ɏ�����
    for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
    {
        m_aInfo[nCount].pTexture = NULL;
        m_aInfo[nCount].nPattern = 0;
        m_aInfo[nCount].nSpeed = 0;
        m_aInfo[nCount].bRepeat = false;
    }
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CTexture::~CTexture()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CTexture::Init(void)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    char cReadText[2048];	// �����Ƃ��ēǂݎ��p
    char cHeadText[2048];	// �����̔��ʗp
    char cDie[2048];		// �g��Ȃ�����
    int nNumType = 0;       // �^�C�v�̃i���o�[
    char cLoadName[256];    // �ǂݍ��ݎ��̖��O
    int nBool = 0;          // int����bool�ւ̋��n��

    // �t�@�C�����J��
    pFile = fopen(TEXTURE_FILENAME, "r");

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

                // cHeadText��TEXTURESET�̎�
                if (strcmp(cHeadText, "TEXTURESET") == 0)
                {
                    // cHeadText��END_TEXTURESET�ɂȂ�܂�
                    while (strcmp(cHeadText, "END_TEXTURESET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "NAME") == 0)
                        {
                            sscanf(cReadText, "%s %s %s", &cDie, &cDie, &cLoadName);

                            //�e�N�X�`���̓ǂݍ���
                            LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
                            D3DXCreateTextureFromFile(pDevice, cLoadName, &m_aInfo[nNumType].pTexture);
                        }
                        else if (strcmp(cHeadText, "PATTERN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aInfo[nNumType].nPattern);
                        }
                        else if (strcmp(cHeadText, "SPEED") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aInfo[nNumType].nSpeed);
                        }
                        else if (strcmp(cHeadText, "REPEAT") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aInfo[nNumType].bRepeat = false;
                            }
                            else
                            {
                                m_aInfo[nNumType].bRepeat = true;
                            }
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

        return E_FAIL;
    }

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CTexture::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
    {
        if (m_aInfo[nCount].pTexture != NULL)
        {
            m_aInfo[nCount].pTexture->Release();
            m_aInfo[nCount].pTexture = NULL;
        }
    }
}

//=============================================================================
// �e�N�X�`�����̎󂯎��̏���
// Author : �㓡�T�V��
//=============================================================================
CTexture::Info* CTexture::GetInfo(const int nNum)
{
    if (nNum < MAX_TEXTURE && nNum >= 0)
    {
        if (&m_aInfo[nNum] != NULL)
        {
            return &m_aInfo[nNum];
        }
    }

    return NULL;
}