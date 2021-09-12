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
#include "shadow.h"
#include "player.h"
#include "modelData.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCharacter::CCharacter()
{
    m_activeCollisionSize = DEFAULT_VECTOR;
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_tribe = TRIBE_NONE;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_posDefault, 0, sizeof(m_posDefault));
    memset(m_nIndexParent, 0, sizeof(m_nIndexParent));
    memset(m_aPartsType, 0, sizeof(m_aPartsType));

    m_pAnimation = NULL;
    m_pShadow = NULL;
    m_pFrame = NULL;
    m_cAnimFilePass = NULL;

    for (int nCnt = 0; nCnt < CHARCTER_COLLISION_MAX; nCnt++)
    {
        m_aCollision[nCnt].pos = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;

    // �A�j���[�V�����̓f�t�H���g�Ŏg�����Ƃ�ݒ�
    m_bUseAnimation = true;
}

//=============================================================================
// �I�[�o�[���C�h���ꂽ�R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCharacter::CCharacter(OBJTYPE objtype) :CScene(objtype)
{
    m_activeCollisionSize = DEFAULT_VECTOR;
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_tribe = TRIBE_NONE;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_posDefault, 0, sizeof(m_posDefault));
    memset(m_nIndexParent, 0, sizeof(m_nIndexParent));
    memset(m_aPartsType, 0, sizeof(m_aPartsType));

    m_pAnimation = NULL;
    m_pShadow = NULL;
    m_pFrame = NULL;
    m_cAnimFilePass = NULL;

    for (int nCnt = 0; nCnt < CHARCTER_COLLISION_MAX; nCnt++)
    {
        m_aCollision[nCnt].pos = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;

    // �A�j���[�V�����̓f�t�H���g�Ŏg�����Ƃ�ݒ�
    m_bUseAnimation = true;
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

    // ���f���̃|�C���^�z��̏�����
    memset(m_apModel, 0, sizeof(m_apModel));

    // ���f�����p�[�c���������m�ۂ��A������
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_apModel[nCount] = new CModel;
        m_apModel[nCount]->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
        m_apModel[nCount]->SetScale(size);
        m_apModel[nCount]->BindModelData(m_aPartsType[nCount]);
        m_apModel[nCount]->SetPos(m_posDefault[nCount]);
    }

    // �A�j���[�V�����̃��[�h
    m_pAnimation = CAnimation::Create(this, m_nPartsNum, (CAnimation::ANIM_TYPE)m_tribe, m_cAnimFilePass);

    //// �v���C���[�Ȃ�
    //if (m_tribe == TRIBE_PLAYER)
    //{
    //    // �e�𐶐�
    //    m_pShadow = CShadow::Create(D3DXVECTOR3(pos.x, SHADOW_POS_Y, pos.z), DEFAULT_VECTOR, HIT_TOP, SHADOW_COLOR);

    //    // �ӂ��ǂ�𐶐�
    //    m_pFrame = CShadow::Create(pos, DEFAULT_VECTOR, D3DXVECTOR3(D3DXToRadian(45.0f), 0.0f, 0.0f), SHADOW_COLOR);
    //}

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

    // �e���g�p���Ȃ�
    if (m_pShadow != NULL)
    {
        m_pShadow->Unable();
    }

    // �ӂ��ǂ���g�p���Ȃ�
    if (m_pFrame != NULL)
    {
        m_pFrame->Unable();
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

    //// �e���X�V
    //if (m_pShadow != NULL)
    //{
    //    m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, SHADOW_POS_Y, m_pos.z));

    //    // �T�C�Y�͂����ł͂Ȃ��������Œ����A���b�g�̓J�����̌������Q�l��
    //    m_pShadow->SetSize(D3DXVECTOR3(m_activeCollisionSize.x * SHADOW_SIZE_RATE, m_activeCollisionSize.x * SHADOW_SIZE_RATE, 0.0f));
    //    m_pShadow->SetRotAngle(m_rot.y);
    //}

    //// �ӂ��ǂ���X�V
    //if (m_pShadow != NULL)
    //{
    //    m_pFrame->SetPos(m_pos);

    //    // �T�C�Y�͂����ł͂Ȃ��������Œ����A���b�g�̓J�����̌������Q�l��
    //    m_pFrame->SetSize(D3DXVECTOR3(m_activeCollisionSize.x * 200.0f, m_activeCollisionSize.x * 200.0f, 0.0f));
    //    m_pFrame->SetRotAngle(m_rot.y);
    //}
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CCharacter::Draw(void)
{
    //// �X�e���V���o�b�t�@�ŉe��`��
    //if (m_pShadow != NULL)
    //{
    //    // �X�e���V���o�b�t�@���g��
    //    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //    // Z�e�X�g�L��
    //    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    // Z�e�X�g��K�����s��
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

    //    // �X�e���V���e�X�g�L��
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    //    // �X�e���V���o�b�t�@�֔��f����Q�ƒl
    //    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
    //    // �Q�ƒl�}�X�N
    //    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
    //    // �X�e���V���e�X�g��K��������
    //    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    //    // �X�e���V���e�X�g�̃e�X�g�ݒ�
    //    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    //    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
    //    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    //    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    //    {
    //        // ��
    //        m_apModel[nCount]->SetScale(D3DXVECTOR3(0.9f, 0.9f, 0.9f));

    //        if (nCount == 0)
    //        {
    //            // ���i�d���Ȃ�A�ۂ��e�ł��\���j�i�������́A�}���`�^�[�Q�b�g�����_�����O�ŕ��׌y���j
    //            float fRotX = D3DXToRadian(90.0f);
    //            float fRotY = D3DXToRadian(270.0f);
    //            if(m_rot.y == PLAYER_ROT_LEFT)
    //            {
    //                fRotX = D3DXToRadian(270.0f);
    //                fRotY = D3DXToRadian(90.0f);
    //            }
    //            m_apModel[nCount]->Draw(D3DXVECTOR3(m_pos.x + 50.0f, SHADOW_POS_Y, m_pos.z),
    //                D3DXVECTOR3(fRotX, fRotY, m_rot.z));
    //        }
    //        else
    //        {
    //            m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
    //        }

    //        m_apModel[nCount]->SetScale(DEFAULT_SCALE);
    //    }

    //    // �X�e���V���e�X�g����
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

    //    // ZBUFFER��r�ݒ�ύX => (�Q�ƒl <= �o�b�t�@�l)(�߂�)
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    //    // �e��`��
    //    m_pShadow->Draw();
    //}

    //// �X�e���V���o�b�t�@�łӂ��ǂ��`��
    //if (m_pFrame != NULL)
    //{
    //    // �X�e���V���o�b�t�@���g��
    //    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //    // Z�e�X�g�L��
    //    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    // Z�e�X�g��K�����s��
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

    //    // �X�e���V���e�X�g�L��
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    //    // �X�e���V���o�b�t�@�֔��f����Q�ƒl
    //    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
    //    // �Q�ƒl�}�X�N
    //    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
    //    // �X�e���V���e�X�g��K��������
    //    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    //    // �X�e���V���e�X�g�̃e�X�g�ݒ�
    //    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    //    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
    //    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    //    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    //    {
    //        // ��
    //        m_apModel[nCount]->SetScale(D3DXVECTOR3(1.02f, 1.02f, 1.02f));

    //        if (nCount == 0)
    //        {
    //            m_apModel[nCount]->Draw(m_pos, m_rot);
    //        }
    //        else
    //        {
    //            m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
    //        }

    //        m_apModel[nCount]->SetScale(DEFAULT_SCALE);
    //    }

    //    // �X�e���V���e�X�g����
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

    //    // ZBUFFER��r�ݒ�ύX => (�Q�ƒl <= �o�b�t�@�l)(�߂�)
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    //    // �ӂ��ǂ��`��
    //    m_pFrame->Draw();
    //}

    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        if (nCount == 0)
        {
            m_apModel[nCount]->Draw(m_pos, m_rot);
        }
        else
        {
            m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
        }
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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nIndexParent[nCntLoad]);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos.x, &pos.y, &pos.z);

                            m_posDefault[nCntLoad] = pos;
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
        m_nIndexParent[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->nParent[nCnt];
        m_posDefault[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->pos[nCnt];
    }

    // �����g
    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        m_nIndexParent[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->nParent[nCnt];
        m_posDefault[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->pos[nCnt];
    }
}
