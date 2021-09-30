//===============================================
//
// �L�����N�^�[���� (character.cpp)
// Author : �㓡�T�V��
//
//===============================================
#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "character.h"
#include "animation.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animation.h"
#include "scene3d.h"
#include "library.h"
#include "game.h"
#include "player.h"
#include "modelData.h"
#include "camera.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//=============================================================================
// �I�[�o�[���C�h���ꂽ�R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCharacter::CCharacter(OBJTYPE objtype) :CScene(objtype)
{
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_aPosDefault, 0, sizeof(m_aPosDefault));
    memset(m_anIndexParent, 0, sizeof(m_anIndexParent));
    memset(m_aPartsType, 0, sizeof(m_aPartsType));

    m_pAnimation = NULL;
    m_cAnimFilePass = NULL;

    // �A�j���[�V�����̓f�t�H���g�Ŏg�����Ƃ�ݒ�
    m_bUseAnimation = true;

    m_bDrawWeapon = true;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCharacter::~CCharacter()
{
}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CCharacter::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �ʒu�����т���
    m_pos = pos;
    m_posOld = pos;

    // ���f�����p�[�c���������m�ۂ��A������
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        // �������Ă��Ȃ��Ȃ琶��
        if (!m_apModel[nCount])
        {
            m_apModel[nCount] = new CModel;
            m_apModel[nCount]->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
            m_apModel[nCount]->SetScale(size);
            m_apModel[nCount]->SetPos(m_aPosDefault[nCount]);
        }

        // �J�X�^�}�C�Y�p�ŁA�������Ă��Ă��ǂݍ���
        m_apModel[nCount]->BindModelData(m_aPartsType[nCount]);

        // �A�j���[�V�����𐶐����Ă���Ȃ�
        if (m_pAnimation)
        {
            m_pAnimation->SetAnimPosReset(nCount);
        }
    }

    // �A�j���[�V�����̃��[�h�i�������Ă��Ȃ��Ȃ�j
    if (!m_pAnimation)
    {
        m_pAnimation = CAnimation::Create(this, m_nPartsNum, m_cAnimFilePass);
    }

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CCharacter::Uninit(void)
{
    // ���f�����p�[�c���A�I�����A�������J��
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        // ���g������Ȃ�
        if (m_apModel[nCount] != NULL)
        {
            // �I������
            m_apModel[nCount]->Uninit();

            // �������J��
            delete m_apModel[nCount];
            m_apModel[nCount] = NULL;
        }
    }

    // �A�j���[�V���������I�����A�������J��
    if (m_pAnimation != NULL)
    {
        // �I������
        m_pAnimation->Uninit();

        // �������J��
        delete m_pAnimation;
        m_pAnimation = NULL;
    }

    Release();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CCharacter::Update(void)
{
    // �A�j���[�V�������g���t���O��true�Ȃ�A�j���[�V����������
    if (m_bUseAnimation)
    {
        m_pAnimation->Update();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CCharacter::Draw(void)
{
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        if (nCount == 0)
        {
            // �S�Ă̐e�́A�L�����N�^�[�̈ʒu���̂��̂�n��
            m_apModel[nCount]->Draw(m_pos, m_rot);
        }
        else if (nCount == CPlayer::PARTS_WEP)
        {
            // �����`�悷��Ȃ�
            if (m_bDrawWeapon)
            {
                m_apModel[nCount]->Draw(m_apModel[m_anIndexParent[nCount]]);
            }
        }
        else
        {
            m_apModel[nCount]->Draw(m_apModel[m_anIndexParent[nCount]]);
        }
    }
}

//=============================================================================
// ���ꂽ���̕`��
// Author : �㓡�T�V��
//=============================================================================
void CCharacter::DeathDraw(void)
{
    // �����̈ꌂ���ȊO�Ȃ�
    if (CManager::GetCamera()->CCamera::GetState() != CCamera::STATE_FINISH_EACH)
    {
        // Z�o�b�t�@�𖳌���
        LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
        pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    }

    // �e�p�[�c���A�������邽�߂�true
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        if (nCount == 0)
        {
            m_apModel[nCount]->Draw(m_pos, m_rot, true);
        }
        else
        {
            m_apModel[nCount]->Draw(m_apModel[m_anIndexParent[nCount]], true);
        }
    }

    // �����̈ꌂ���ȊO�Ȃ�
    if (CManager::GetCamera()->CCamera::GetState() != CCamera::STATE_FINISH_EACH)
    {
        // Z�o�b�t�@��L����
        LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
        pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    }
}

//=============================================================================
// ���f���f�[�^�i���W,�e�q�֌W�j�ǂݍ��ݏ����i�e�L�X�g�t�@�C�����璼�ړǂݍ��ގ��Ɏg���֐��j
// Author : �㓡�T�V��
//=============================================================================
void CCharacter::LoadModelData(char* cFilePass)
{
    // ���O���L��
    m_cAnimFilePass = cFilePass;

    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // �t�@�C���J
    pFile = fopen(cFilePass, "r");

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "CHARACTERSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "CHARACTERSET") == 0)
        {
            while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "PARTSSET") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTSSET") != 0)
                    {

                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "INDEX") == 0)
                        {
                            int nNum;
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nNum);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_anIndexParent[nCntLoad]);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos.x, &pos.y, &pos.z);

                            m_aPosDefault[nCntLoad] = pos;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot.x, &rot.y, &rot.z);
                        }
                    }
                    nCntLoad++;
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
// ���f���f�[�^�i���W,�e�q�֌W�j�ǂݍ��ݏ����i���f���̏����ʒu�f�[�^����ǂݍ��ނƂ��Ɏg���֐��j
// Author : �㓡�T�V��
//=============================================================================
void CCharacter::LoadModelData(int nModelPosDefUp, int nModelPosDefDown)
{
    // ���f���̏����ʒu�f�[�^���󂯎��
    CModelData *pModelData = CManager::GetModelData();

    // �㔼�g�Ɖ����g�𕪂���
    CModelData::ModelPosDef *pModelPosDefUp = pModelData->GetPosDef(nModelPosDefUp);
    CModelData::ModelPosDef *pModelPosDefDown = pModelData->GetPosDef(nModelPosDefDown);

    // �㔼�g
    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        m_anIndexParent[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->nParent[nCnt];
        m_aPosDefault[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->pos[nCnt];
    }

    // �����g
    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        m_anIndexParent[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->nParent[nCnt];
        m_aPosDefault[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->pos[nCnt];
    }
}
