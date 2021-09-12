//===============================================
//
// �V�[����̃I�u�W�F�N�g���� (scene.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "scene.h"
#include "renderer.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
int CScene::m_nNumAll[CScene::OBJTYPE_MAX] = {};
int CScene::m_nNumObjectAll = 0;

CScene *CScene::m_pTop[CScene::OBJTYPE_MAX] = {};
CScene *CScene::m_pCur[CScene::OBJTYPE_MAX] = {};

//========================================
// �V�[����̃I�u�W�F�N�g�̃f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CScene::CScene()
{
    // �I�u�W�F�N�g�̎�ނ�NULL�ɂ���
    m_objType = OBJTYPE_NONE;

    // �擪���Ȃ��Ȃ�A�擪��
    if (m_pTop[m_objType] == NULL)
    {
        m_pTop[m_objType] = this;
    }

    // ���݂ɂ�����ŐV�̃I�u�W�F�N�g���Ȃ��Ȃ�A�ŐV��
    if (m_pCur[m_objType] == NULL)
    {
        m_pCur[m_objType] = this;
    }

    // ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���A�����ɂ���
    m_pCur[m_objType]->m_pNext = this;

    // ���݂̃I�u�W�F�N�g�������̏ꍇ
    if (m_pCur[m_objType] == this)
    {
        // �����̑O�̃I�u�W�F�N�g���ANULL�ɂ���
        m_pPrev = NULL;
    }
    else
    {
        // �����̑O�̃I�u�W�F�N�g���A���݂̃I�u�W�F�N�g�ɂ���
        m_pPrev = m_pCur[m_objType];
    }

    // ���݂̃I�u�W�F�N�g���A�����ɂ���
    m_pCur[m_objType] = this;

    // �����̎��̃I�u�W�F�N�g���ANULL�ɂ���
    m_pNext = NULL;

    // �������I�u�W�F�N�g���J�E���g
    m_nNumAll[m_objType]++;

    // �S�̂̐����C���N�������g
    m_nNumObjectAll++;

    // �g�p����t���O��true��
    m_bUse = true;
}

//========================================
// �V�[����̃I�u�W�F�N�g�̃I�[�o�[���C�h���ꂽ�R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CScene::CScene(OBJTYPE objType)
{
    // �I�u�W�F�N�g�̎�ނ������ƌ��т���
    m_objType = objType;

    // �擪���Ȃ��Ȃ�A�擪��
    if (m_pTop[m_objType] == NULL)
    {
        m_pTop[m_objType] = this;
    }

    // ���݂ɂ�����ŐV�̃I�u�W�F�N�g���Ȃ��Ȃ�A�ŐV��
    if (m_pCur[m_objType] == NULL)
    {
        m_pCur[m_objType] = this;
    }

    // ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���A�����ɂ���
    m_pCur[m_objType]->m_pNext = this;

    // ���݂̃I�u�W�F�N�g�������̏ꍇ
    if (m_pCur[m_objType] == this)
    {
        // �����̑O�̃I�u�W�F�N�g���ANULL�ɂ���
        m_pPrev = NULL;
    }
    else
    {
        // �����̑O�̃I�u�W�F�N�g���A���݂̃I�u�W�F�N�g�ɂ���
        m_pPrev = m_pCur[m_objType];
    }

    // ���݂̃I�u�W�F�N�g���A�����ɂ���
    m_pCur[m_objType] = this;

    // �����̎��̃I�u�W�F�N�g���ANULL�ɂ���
    m_pNext = NULL;

    // �������I�u�W�F�N�g���J�E���g
    m_nNumAll[m_objType]++;

    // �S�̂̐����C���N�������g
    m_nNumObjectAll++;

    // �g�p����t���O��true��
    m_bUse = true;
}

//========================================
// �V�[����̃I�u�W�F�N�g�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CScene::~CScene()
{

}

//==============================================
// �V�[����̃I�u�W�F�N�g��S�čX�V
// Author : �㓡�T�V��
//==============================================
void CScene::UpdateAll(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // �擪�A�ŐV�̂��̂�����Ȃ�
        if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
        {
            // �L���p�̕ϐ�
            CScene*pScene = m_pTop[nCnt];

            do
            {
                // �L���p�̕ϐ�(Update���ɁAUninit����邱�Ƃ��l��)
                CScene*pNextScene = pScene->m_pNext;

                // �X�V����
                pScene->Update();

                // �g�p�t���O��false�Ȃ�
                if (pScene->m_bUse == false)
                {
                    // �������̊J��
                    delete pScene;
                    pScene = NULL;
                }

                // ���̃V�[���ɕς��Ă���
                pScene = pNextScene;

            } while (pScene != NULL);
        }
    }
}

//==============================================
// �V�[����̃I�u�W�F�N�g��S�ĕ`��
// Author : �㓡�T�V��
//==============================================
void CScene::DrawAll(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // �X�e���V���G�t�F�N�g�ȊO�Ȃ�
        if (nCnt != OBJTYPE_EFFECT3D_STENCIL)
        {
            // �擪�A�ŐV�̂��̂�����Ȃ�
            if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
            {
                // �L���p�̕ϐ�
                CScene*pScene = m_pTop[nCnt];

                do
                {
                    // �`�揈��
                    pScene->Draw();

                    // ���̃V�[���ɕς��Ă���
                    pScene = pScene->m_pNext;

                } while (pScene != NULL);
            }
        }
    }
}

//==============================================
// �N�C�b�N�\�[�g
// Author : �㓡�T�V��
//==============================================
void CScene::QuickSort(CScene * apScene[], int left, int right)
{
    // �ϐ���`
    int Left;
    int Right;
    CScene* pivot;
    int nPivotAnswer = (left + right) / 2;

    // �����l�͈�������
    Left = left;
    Right = right;

    // �s�{�b�g����Ȃ�A�J�艺����
    if (nPivotAnswer % 2 != 0)
    {
        nPivotAnswer = nPivotAnswer - (1 / 2);
    }

    // �s�{�b�g�͈̔͂�ݒ�(����)
    if (nPivotAnswer < 0)
    {
        nPivotAnswer = 0;
    }

    // �s�{�b�g�͈̔͂�ݒ�(���)
    if (nPivotAnswer > m_nNumObjectAll - 1)
    {
        nPivotAnswer = m_nNumObjectAll - 1;
    }

    // ��͐^�񒆂ɐݒ�
    pivot = apScene[nPivotAnswer];

    // �\�[�e�B���O
    while (1)
    {
        // ���菬�����l�������猩���Ă���
        while (apScene[Left]->m_objType < pivot->m_objType)
        {
            Left++;

            // �ő�l�̐���
            if (Left > m_nNumObjectAll - 1)
            {
                Left = m_nNumObjectAll - 1;

                break;
            }
        }

        // ����傫���l���E���猩���Ă���
        while (pivot->m_objType < apScene[Right]->m_objType)
        {
            Right--;

            // �ŏ��l�̐���
            if (Right < 0)
            {
                Right = 0;

                break;
            }
        }

        // ���������l�̏������t�ɂȂ�����I�� 
        if (Left >= Right)
        {
            break;
        }

        // ��������
        if (apScene[Left] != NULL && apScene[Right] != NULL)
        {
            // �Ⴂ�ԍ����L��
            CScene *pScene = apScene[Left];

            // �Ⴂ�ԍ��Ɏ��̔ԍ�����
            apScene[Left] = apScene[Right];

            // ���̔ԍ��ɋL���������̂���
            apScene[Right] = pScene;
        }

        // ���̒l�Ɉړ�
        Left++;
        Right--;

        // �ő�l�̐���
        if (Left > m_nNumObjectAll - 1)
        {
            break;
        }

        // �ŏ��l�̐���
        if (Right < 0)
        {
            break;
        }
    }

    //���̃f�[�^�Q��ΏۂƂ��ăN�C�b�N�\�[�g���ċA
    if (left < Left - 1)
    {
        QuickSort(apScene, left, Left - 1);
    }

    //�E�̃f�[�^�Q��ΏۂƂ��ăN�C�b�N�\�[�g���ċA
    if (Right + 1 < right)
    {
        QuickSort(apScene, Right + 1, right);
    }
}

//==============================================
// �V�[����̃I�u�W�F�N�g��S�ĊJ��
// Author : �㓡�T�V��
//==============================================
void CScene::ReleaseAll(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // �擪�A�ŐV�̂��̂�����Ȃ�
        if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
        {
            // �L���p�̕ϐ�
            CScene*pScene = m_pTop[nCnt];

            do
            {
                // �L���p�̕ϐ�
                CScene*pNextScene = pScene->m_pNext;

                // �I������
                pScene->Uninit();

                // �g�p�t���O��false�Ȃ�
                if (pScene->m_bUse == false)
                {
                    // �������̊J��
                    delete pScene;
                    pScene = NULL;
                }

                // ���̃V�[���ɕς��Ă���
                pScene = pNextScene;

            } while (pScene != NULL);
        }
    }
}

//==============================================
// �V�[����̓G�A�e�A���A�ǁA�u���b�N�A�A�C�e����S�ĊJ��
// Author : �㓡�T�V��
//==============================================
void CScene::ReleaseStage(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        if (nCnt == OBJTYPE_BG || nCnt == OBJTYPE_EFFECT3D_Z_NONE || nCnt == OBJTYPE_BLOCK ||
            nCnt == OBJTYPE_MODEL_EFFECT || nCnt == OBJTYPE_ITEM ||
            nCnt == OBJTYPE_ENEMY)
        {
            // �擪�A�ŐV�̂��̂�����Ȃ�
            if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
            {
                // �L���p�̕ϐ�
                CScene*pScene = m_pTop[nCnt];

                do
                {
                    // �L���p�̕ϐ�
                    CScene*pNextScene = pScene->m_pNext;

                    // �I������
                    pScene->Uninit();

                    // �g�p�t���O��false�Ȃ�
                    if (pScene->m_bUse == false)
                    {
                        // �������̊J��
                        delete pScene;
                        pScene = NULL;
                    }

                    // ���̃V�[���ɕς��Ă���
                    pScene = pNextScene;

                } while (pScene != NULL);
            }
        }
    }
}

//==============================================
// �V�[�����UI��S�ĊJ��
// Author : �㓡�T�V��
//==============================================
void CScene::ReleaseUI(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        if (nCnt == OBJTYPE_UI)
        {
            // �擪�A�ŐV�̂��̂�����Ȃ�
            if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
            {
                // �L���p�̕ϐ�
                CScene*pScene = m_pTop[nCnt];

                do
                {
                    // �L���p�̕ϐ�
                    CScene*pNextScene = pScene->m_pNext;

                    // �I������
                    pScene->Uninit();

                    // �g�p�t���O��false�Ȃ�
                    if (pScene->m_bUse == false)
                    {
                        // �������̊J��
                        delete pScene;
                        pScene = NULL;
                    }

                    // ���̃V�[���ɕς��Ă���
                    pScene = pNextScene;

                } while (pScene != NULL);
            }
        }
    }
}

//==============================================
// �V�[����̃I�u�W�F�N�g��S�ĊJ��
// Author : �㓡�T�V��
//==============================================
void CScene::Release(void)
{
    // ���g������Ȃ�A
    if (this != NULL)
    {
        // �擪�Ȃ�
        if (this == m_pTop[m_objType])
        {
            // ���̒��g������Ȃ�
            if (m_pNext != NULL)
            {
                // ���̂��̂̑O�̏����ANULL��
                m_pNext->m_pPrev = NULL;

                // �����擪��
                m_pTop[m_objType] = m_pNext;
            }
            else
            {
                // �擪�̎����Ȃ��Ȃ�A�擪�ƌ��݂������|�C���^��NULL��
                m_pTop[m_objType] = NULL;
                m_pCur[m_objType] = NULL;
            }
        }
        // �擪�łȂ��Ȃ�
        else
        {
            // ���̒��g������Ȃ�
            if (m_pNext != NULL)
            {
                // �O�̂��̂̎��̏����A���g�̎��̏���
                m_pPrev->m_pNext = m_pNext;

                // ���̂��̂̑O�̏����A���g�̑O�̏���
                m_pNext->m_pPrev = m_pPrev;
            }
            else
            {
                // �O�̂��̂̎��̏����A�Ȃ���
                m_pPrev->m_pNext = NULL;

                // ���݂̍ŐV�̂��̂��A�O�̏���
                m_pCur[m_objType] = m_pPrev;
            }
        }

        //// �������̊J��(���X�g�\����m_pNext�ŃV�[����؂�ւ��Ă����ۂɎx�Ⴊ�o��̂ŁA�����ł͍s��Ȃ�)
        //delete this;
        ////this = NULL;

        // �g�p�t���O��false��
        m_bUse = false;

        // ��ނ��Ƃ̑��������炷
        m_nNumAll[m_objType]--;

        // �S�̂̐����f�N�������g
        m_nNumObjectAll--;
    }
}
