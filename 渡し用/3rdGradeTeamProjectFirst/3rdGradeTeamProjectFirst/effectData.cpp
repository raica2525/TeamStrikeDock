//===============================================
//
// �G�t�F�N�g�f�[�^�Ǘ� (effectData.cpp)
// Author : �㓡�T�V��
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "effectData.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define EFFECT_DATA_FILENAME ("data/TXT/effect.txt")

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CEffectData::CEffectData()
{
    memset(m_aCreateInfo, 0, sizeof(m_aCreateInfo));
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CEffectData::~CEffectData()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CEffectData::Init(void)
{
    // �t�@�C���|�C���g
    FILE *pFile = NULL;

    // �ϐ��錾
    char cReadText[2048];	               // �����Ƃ��ēǂݎ��p
    char cHeadText[2048];	               // �����̔��ʗp
    char cDie[2048];		               // �g��Ȃ�����
    int nNumType = 0;                      // �^�C�v�̃i���o�[
    int nBool = 0;                         // int����bool�ւ̋��n��

    //======================================================================================
    // ���f���f�[�^�t�@�C�����J��
    pFile = fopen(EFFECT_DATA_FILENAME, "r");

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

                // cHeadText��EFFECT_DATASET�̎�
                if (strcmp(cHeadText, "EFFECT_DATASET") == 0)
                {
                    // cHeadText��END_EFFECT_DATASET�ɂȂ�܂�
                    while (strcmp(cHeadText, "END_EFFECT_DATASET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "TEX_TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nTexType);
                        }
                        else if (strcmp(cHeadText, "ONCE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nNumOnce);
                        }
                        else if (strcmp(cHeadText, "PLANE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].plane);
                        }
                        else if (strcmp(cHeadText, "ON_CIRCLE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bOnCircleEdge = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bOnCircleEdge = true;
                            }
                        }
                        else if (strcmp(cHeadText, "EMIT_INSIDE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bEmitVectorInside = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bEmitVectorInside = true;
                            }
                        }
                        else if (strcmp(cHeadText, "SCATTER_ANGLE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nScatterAngle);
                        }
                        else if (strcmp(cHeadText, "SCATTER_WIDTH") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nScatterWidth);
                        }
                        else if (strcmp(cHeadText, "SPEED_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSpeedMax);
                        }
                        else if (strcmp(cHeadText, "SPEED_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSpeedMin);
                        }
                        else if (strcmp(cHeadText, "SPEED_AA_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nAuxiliaryAxisSpeedMax);
                        }
                        else if (strcmp(cHeadText, "SPEED_AA_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nAuxiliaryAxisSpeedMin);
                        }
                        else if (strcmp(cHeadText, "ACCEL") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fAcceleration);
                        }
                        else if (strcmp(cHeadText, "GRAVITY") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fGravity);
                        }
                        else if (strcmp(cHeadText, "GRAVITY_LIMIT") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fGravityLimit);
                        }
                        else if (strcmp(cHeadText, "ROT_LAY") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nLayRot);
                        }
                        else if (strcmp(cHeadText, "ROT_EQUAL_MOVE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bRotEqualMoveAngle = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bRotEqualMoveAngle = true;
                            }
                        }
                        else if (strcmp(cHeadText, "ROT_SPEED_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nRotSpeedMax);
                        }
                        else if (strcmp(cHeadText, "ROT_SPEED_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nRotSpeedMin);
                        }
                        else if (strcmp(cHeadText, "COL") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie, 
                                &m_aCreateInfo[nNumType].col.r,
                                &m_aCreateInfo[nNumType].col.g,
                                &m_aCreateInfo[nNumType].col.b,
                                &m_aCreateInfo[nNumType].col.a);
                        }
                        else if (strcmp(cHeadText, "COL_CHANGE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie,
                                &m_aCreateInfo[nNumType].colChangeRate.r,
                                &m_aCreateInfo[nNumType].colChangeRate.g,
                                &m_aCreateInfo[nNumType].colChangeRate.b,
                                &m_aCreateInfo[nNumType].colChangeRate.a);
                        }
                        else if (strcmp(cHeadText, "FADE_OUT_RATE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fFadeOutRate);
                        }
                        else if (strcmp(cHeadText, "FADE_OUT_LOCK_TIME") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nCntFadeOutLock);
                        }
                        else if (strcmp(cHeadText, "SIZE_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSizeMax);
                        }
                        else if (strcmp(cHeadText, "SIZE_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSizeMin);
                        }
                        else if (strcmp(cHeadText, "SIZE_CHANGE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
                                &m_aCreateInfo[nNumType].sizeChangeRate.x,
                                &m_aCreateInfo[nNumType].sizeChangeRate.y,
                                &m_aCreateInfo[nNumType].sizeChangeRate.z);
                        }
                        else if (strcmp(cHeadText, "SIZE_CHANGE_LOCK_TIME") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nCntSizeChangeLock);
                        }
                        else if (strcmp(cHeadText, "SIZE_CHANGE_START_OR_FINISH") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bSizeChangeStartOrFinish = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bSizeChangeStartOrFinish = true;
                            }
                        }
                        else if (strcmp(cHeadText, "EMITRADIUS") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fEmitRadius);
                        }
                        else if (strcmp(cHeadText, "ADD_BLEND") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bUseAdditiveSynthesis = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bUseAdditiveSynthesis = true;
                            }
                        }
                        else if (strcmp(cHeadText, "USE_ZBUFFER") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bUseZBuffer = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bUseZBuffer = true;
                            }
                        }
                        else if (strcmp(cHeadText, "ALPHA_TEST_BORDER") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nAlphaTestBorder);
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

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CEffectData::Uninit(void)
{

}

//=============================================================================
// �G�t�F�N�g�������擾����
// Author : �㓡�T�V��
//=============================================================================
CEffectData::CreateInfo * CEffectData::GetCreateInfo(const int nNum)
{
    if (nNum < MAX_EFFECT_TYPE && nNum >= 0)
    {
        if (&m_aCreateInfo[nNum] != NULL)
        {
            return &m_aCreateInfo[nNum];
        }
    }

    return NULL;
}
