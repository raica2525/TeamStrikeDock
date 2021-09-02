//=============================================================================
//
// ���f���f�[�^�̏��� [modelData.cpp]
// Author : �㓡�T�V��
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "modelData.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MODEL_DATA_FILENAME ("data/TXT/modelData.txt")	    // ���f���f�[�^��ǂݍ��ރt�@�C���̖��O
#define MODEL_POS_DEF_FILENAME ("data/TXT/modelPosDef.txt")	// ���f���̏����ʒu�f�[�^��ǂݍ��ރt�@�C���̖��O
#define PARTS_LIST_FILENAME ("data/TXT/partsList.txt")	    // �p�[�c���X�g��ǂݍ��ރt�@�C���̖��O
#define PARTS_RATE_FILENAME ("data/TXT/partsRate.txt")	    // �p�[�c���[�g��ǂݍ��ރt�@�C���̖��O

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CModelData::CModelData()
{
    memset(m_aModelData, 0, sizeof(m_aModelData));
    memset(m_aModelData->apTexMat, 0, sizeof(m_aModelData->apTexMat));

    memset(m_aModelPosDef, 0, sizeof(m_aModelPosDef));
    memset(m_aModelPosDef->nIndex, 0, sizeof(m_aModelPosDef->nIndex));
    memset(m_aModelPosDef->nParent, 0, sizeof(m_aModelPosDef->nParent));
    memset(m_aModelPosDef->pos, 0, sizeof(m_aModelPosDef->pos));

    // �p�[�c���X�g�́A���ʔԍ���-1�ŏ������i0�Ԗڂ͓��ł���A�J�X�^�}�C�Y��ʂő��݂��Ȃ��p�[�c���Q�Ƃ��Ȃ����߁j
    memset(m_aPartsList, 0, sizeof(m_aPartsList));
    memset(m_aPartsList->afParam, 0, sizeof(m_aPartsList->afParam));
    for (int nCnt = 0; nCnt < MAX_PARTS_LIST; nCnt++)
    {
        m_aPartsList[nCnt].part = NOT_EXIST;
    }

    memset(m_aPartsRate, 0, sizeof(m_aPartsRate));
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CModelData::~CModelData()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CModelData::Init(void)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    char cReadText[2048];	               // �����Ƃ��ēǂݎ��p
    char cHeadText[2048];	               // �����̔��ʗp
    char cDie[2048];		               // �g��Ȃ�����
    int nNumType = 0;                      // �^�C�v�̃i���o�[
    char cLoadModelName[256];              // �ǂݍ��ݎ��̃��f����

    // ���f����ǂݍ��ނ��߂̃f�o�C�X�擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //======================================================================================
    // ���f���f�[�^�t�@�C�����J��
    pFile = fopen(MODEL_DATA_FILENAME, "r");

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

                // cHeadText��MODEL_DATASET�̎�
                if (strcmp(cHeadText, "MODEL_DATASET") == 0)
                {
                    // cHeadText��END_MODEL_DATASET�ɂȂ�܂�
                    while (strcmp(cHeadText, "END_MODEL_DATASET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "NAME") == 0)
                        {
                            sscanf(cReadText, "%s %s %s", &cDie, &cDie, &cLoadModelName);

                            // ���f���f�[�^�̓ǂݍ���
                            D3DXLoadMeshFromX(LPCSTR(cLoadModelName), D3DXMESH_SYSTEMMEM, pDevice, NULL, 
                                &m_aModelData[nNumType].pBuffMat, NULL, &m_aModelData[nNumType].nNumMat, &m_aModelData[nNumType].pMesh);

                            // �}�e���A���̃|�C���^���擾
                            D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_aModelData[nNumType].pBuffMat->GetBufferPointer();

                            // �}�e���A���̃e�N�X�`�������т���
                            for (int nCntMat = 0; nCntMat < (int)m_aModelData[nNumType].nNumMat; nCntMat++)
                            {
                                D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_aModelData[nNumType].apTexMat[nCntMat]);
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
    //======================================================================================

    // ���f���̏����ʒu�f�[�^�ǂݍ���
    LoadModelPosDef();

    // �p�[�c���X�g�ǂݍ���
    LoadPartsList();

    // �p�[�c���[�g�ǂݍ���
    LoadPartsRate();

    return S_OK;
}

//=============================================================================
// ���f���̏����ʒu�f�[�^�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
void CModelData::LoadModelPosDef(void)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    char cReadText[2048];	               // �����Ƃ��ēǂݎ��p
    char cHeadText[2048];	               // �����̔��ʗp
    char cDie[2048];		               // �g��Ȃ�����
    int nNumType = 0;                      // �^�C�v�̃i���o�[
    int nModelPosPartsSet = 0;             // ���f���p�[�c�Z�b�g

    // ���f���̏����ʒu�f�[�^�t�@�C�����J��
    pFile = fopen(MODEL_POS_DEF_FILENAME, "r");

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

                // cHeadText��CHARACTERSET�̎�
                if (strcmp(cHeadText, "CHARACTERSET") == 0)
                {
                    // cHeadText��END_CHARACTERSET�ɂȂ�܂�
                    while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);

                            // ���f���p�[�c�Z�b�g�̃��Z�b�g
                            nModelPosPartsSet = 0;
                        }
                        else if (strcmp(cHeadText, "PARTSSET") == 0)
                        {
                            // cHeadText��END_PARTSSET�ɂȂ�܂�
                            while (strcmp(cHeadText, "END_PARTSSET") != 0)
                            {
                                fgets(cReadText, sizeof(cReadText), pFile);
                                sscanf(cReadText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "INDEX") == 0)
                                {
                                    sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aModelPosDef[nNumType].nIndex[nModelPosPartsSet]);
                                }
                                else if (strcmp(cHeadText, "PARENT") == 0)
                                {
                                    sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aModelPosDef[nNumType].nParent[nModelPosPartsSet]);
                                }
                                else if (strcmp(cHeadText, "POS") == 0)
                                {
                                    sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
                                        &m_aModelPosDef[nNumType].pos[nModelPosPartsSet].x, 
                                        &m_aModelPosDef[nNumType].pos[nModelPosPartsSet].y,
                                        &m_aModelPosDef[nNumType].pos[nModelPosPartsSet].z);

                                    // ���f���p�[�c�Z�b�g�̉��Z
                                    nModelPosPartsSet++;

                                    // �O�̂��ߔz��̍ő吔�ɍs���Ȃ��悤�ɂ���
                                    if (nModelPosPartsSet > MAX_MODEL_PARTS_SET - 1)
                                    {
                                        nModelPosPartsSet = MAX_MODEL_PARTS_SET - 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // �t�@�C�������
        fclose(pFile);
    }

    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        int n = m_aModelPosDef[0].nIndex[nCnt];
        int p = m_aModelPosDef[0].nParent[nCnt];
        D3DXVECTOR3 pos = m_aModelPosDef[0].pos[nCnt];
    }
}

//=============================================================================
// �p�[�c���X�g�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
void CModelData::LoadPartsList(void)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    char cReadText[2048];	               // �����Ƃ��ēǂݎ��p
    char cHeadText[2048];	               // �����̔��ʗp
    char cDie[2048];		               // �g��Ȃ�����
    int nNumType = 0;                      // �^�C�v�̃i���o�[

    // �p�[�c���X�g�t�@�C�����J��
    pFile = fopen(PARTS_LIST_FILENAME, "r");

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

                // cHeadText��PARTS_PARAM�̎�
                if (strcmp(cHeadText, "PARTS_PARAM") == 0)
                {
                    // cHeadText��END_PARTS_PARAM�ɂȂ�܂�
                    while (strcmp(cHeadText, "END_PARTS_PARAM") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "PART") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aPartsList[nNumType].part);
                        }
                        else if (strcmp(cHeadText, "NAME") == 0)
                        {
                            // sscanf�ł́Afscanf�ƈ���ē��{�����肭�󂯎��Ȃ������̂ŁA�K�E�Z�����\�͂Ɠ��l�̌`���Ŏ󂯎�邱�Ƃɂ���
                            //sscanf(cReadText, "%s %s %s", &cDie, &cDie, &m_aPartsList[nNumType].cName);
                            CPlayer::CustomPartsName(m_aPartsList[nNumType].cName, nNumType);
                        }
                        else if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fAtk);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fDef);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fSpd);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fWei);
                        }
                        else if (strcmp(cHeadText, "EX0") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aPartsList[nNumType].nEx0);
                        }
                        else if (strcmp(cHeadText, "EX1") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aPartsList[nNumType].nEx1);
                        }
                        else if (strcmp(cHeadText, "WIDTH") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fWidth);
                        }
                        else if (strcmp(cHeadText, "HEIGHT") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fHeight);
                        }
                        else if (strcmp(cHeadText, "PARAM0") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[0]);
                        }
                        else if (strcmp(cHeadText, "PARAM1") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[1]);
                        }
                        else if (strcmp(cHeadText, "PARAM2") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[2]);
                        }
                        else if (strcmp(cHeadText, "PARAM3") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[3]);
                        }
                        else if (strcmp(cHeadText, "PARAM4") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[4]);
                        }
                        else if (strcmp(cHeadText, "PARAM5") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[5]);
                        }
                        else if (strcmp(cHeadText, "PARAM6") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[6]);
                        }
                        else if (strcmp(cHeadText, "PARAM7") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[7]);
                        }
                    }
                }
            }
        }
        // �t�@�C�������
        fclose(pFile);
    }
}

//=============================================================================
// �p�[�c���[�g�ǂݍ��ݏ���
// Author : �㓡�T�V��
//=============================================================================
void CModelData::LoadPartsRate(void)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    char cReadText[256];	               // �����Ƃ��ēǂݎ��p
    char cHeadText[256];	               // �����̔��ʗp
    char cDie[256];		                   // �g��Ȃ�����

    // �p�[�c���[�g�t�@�C�����J��
    pFile = fopen(PARTS_RATE_FILENAME, "r");

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

                if (strcmp(cHeadText, "PARTS_RATE_HEAD") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_HEAD") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fWeiRate);
                        }
                    }
                }
                else if (strcmp(cHeadText, "PARTS_RATE_UP") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_UP") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fWeiRate);
                        }
                    }
                }
                else if (strcmp(cHeadText, "PARTS_RATE_DOWN") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_DOWN") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fWeiRate);
                        }
                    }
                }
                else if (strcmp(cHeadText, "PARTS_RATE_WEA") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_WEA") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fWeiRate);
                        }
                    }
                }
            }
        }
        // �t�@�C�������
        fclose(pFile);
    }
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CModelData::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_MODEL_DATA; nCount++)
    {
        // �}�e���A���̃e�N�X�`���̔j��
        for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
        {
            if (m_aModelData[nCount].apTexMat[nCntMat] != NULL)
            {
                m_aModelData[nCount].apTexMat[nCntMat]->Release();
                m_aModelData[nCount].apTexMat[nCntMat] = NULL;
            }
        }

        // ���b�V���̔j��
        if (m_aModelData[nCount].pMesh != NULL)
        {
            m_aModelData[nCount].pMesh->Release();
            m_aModelData[nCount].pMesh = NULL;
        }

        // �}�e���A���̔j��
        if (m_aModelData[nCount].pBuffMat != NULL)
        {
            m_aModelData[nCount].pBuffMat->Release();
            m_aModelData[nCount].pBuffMat = NULL;
        }
    }
}

//=============================================================================
// ���f���f�[�^���̎󂯎��̏���
// Author : �㓡�T�V��
//=============================================================================
CModelData::ModelData* CModelData::GetModelData(const int nNum)
{
    if (nNum < MAX_MODEL_DATA && nNum >= 0)
    {
        if (&m_aModelData[nNum] != NULL)
        {
            return &m_aModelData[nNum];
        }
    }

    return NULL;
}

//=============================================================================
// ���f�������ʒu�f�[�^���̎󂯎��̏���
// Author : �㓡�T�V��
//=============================================================================
CModelData::ModelPosDef * CModelData::GetPosDef(const int nNum)
{
    if (nNum < MAX_MODEL_POS_DEF && nNum >= 0)
    {
        if (&m_aModelPosDef[nNum] != NULL)
        {
            return &m_aModelPosDef[nNum];
        }
    }

    return NULL;
}

//=============================================================================
// �p�[�c���X�g���̎󂯎��̏���
// Author : �㓡�T�V��
//=============================================================================
CModelData::PartsParam* CModelData::GetPartsList(const int nNum)
{
    if (nNum < MAX_PARTS_LIST && nNum >= 0)
    {
        if (&m_aPartsList[nNum] != NULL)
        {
            return &m_aPartsList[nNum];
        }
    }

    return NULL;
}

//=============================================================================
// �p�[�c���[�g���̎󂯎��̏���
// Author : �㓡�T�V��
//=============================================================================
CModelData::PartsRate* CModelData::GetPartsRate(const int nNum)
{
    if (nNum < MAX_PARTS && nNum >= 0)
    {
        if (&m_aPartsRate[nNum] != NULL)
        {
            return &m_aPartsRate[nNum];
        }
    }

    return NULL;
}