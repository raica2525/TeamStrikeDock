//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
// scene.cpp
// Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E


//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "scene.h"
#include "renderer.h"
#include "scene2d.h"
#include "debug.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�ÓI�����o�ϐ��錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CScene* CScene::m_pTop[PRIORITY_MAX] = {};
CScene* CScene::m_pCur[PRIORITY_MAX] = {};

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�R���X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CScene::CScene(int nPriority)
{
	m_bDestroy = false;
	m_nPriority = nPriority;
	if (m_pTop[nPriority] != NULL)
	{
		m_pCur[nPriority]->m_pNext = this;
	}
	else
	{
		m_pTop[nPriority] = this;
	}
	this->m_pPrev = m_pCur[nPriority];
	this->m_pNext = NULL;
	m_pCur[nPriority] = this;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�f�X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CScene::~CScene()
{

}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�X�V����
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CScene::UpdateAll(void)
{
	CScene* pNext = NULL;
	CScene* pNextBuff = NULL;
	int SceneCount = 0;
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		if (m_pTop[nCount] != NULL)
		{
			pNext = m_pTop[nCount];
			while (pNext != NULL&&!pNext->m_bDestroy)
			{
				pNextBuff = pNext->m_pNext;
				pNext->Update();
				pNext = pNextBuff;
				SceneCount++;
			}
		}
	}

	//���S�t���O�����Ă����E��
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		if (m_pTop[nCount] != NULL)
		{
			pNext = m_pTop[nCount];
			while (pNext != NULL)
			{
				pNextBuff = pNext->m_pNext;
				if (pNext->m_bDestroy == true)
				{
					pNext->Death();
				}
				pNext = pNextBuff;
			}
		}
	}
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�`�揈��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E 
void CScene::DrawAll(void)
{
	CScene* pNext = NULL;
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		if (m_pTop[nCount] != NULL)
		{
			pNext = m_pTop[nCount];
			while (pNext != NULL)
			{
				pNext->Draw();
				pNext = pNext->m_pNext;
			}
		}
	}
}

void CScene::DrawExceptEffect(void)
{
	CScene* pNext = NULL;
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		if (m_pTop[nCount] != NULL)
		{
			pNext = m_pTop[nCount];
			while (pNext != NULL)
			{
				if (pNext->GetObjType() != OBJTYPE_EFFECT)
				{
					pNext->Draw();
				}
				pNext = pNext->m_pNext;
			}
		}
	}
}

void CScene::DrawEffect(void)
{
	CScene* pNext = NULL;
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		if (m_pTop[nCount] != NULL)
		{
			pNext = m_pTop[nCount];
			while (pNext != NULL)
			{
				if (pNext->GetObjType() == OBJTYPE_EFFECT)
				{
					pNext->Draw();
				}
				pNext = pNext->m_pNext;
			}
		}
	}
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�S�J������
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void  CScene::ReleaseAll(void)
{
	CScene* pNext = NULL;
	CScene* pNextBuff = NULL;
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		if (m_pTop[nCount] != NULL)
		{
			pNext = m_pTop[nCount];
			while (pNext != NULL)
			{
				pNextBuff = pNext->m_pNext;
				pNext->Uninit();
				delete pNext;
				pNext = pNextBuff;
			}
		}
	}
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�J������ 
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CScene::Release(void)
{
	//���S�t���O�𗧂Ă�
	m_bDestroy = true;
}

void CScene::Death(void)
{
	if (this == m_pTop[m_nPriority])
	{
		m_pTop[m_nPriority] = m_pNext;
	}
	if (this == m_pCur[m_nPriority])
	{
		m_pCur[m_nPriority] = m_pPrev;
	}
	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;
	}
	if (m_pPrev != NULL)
	{
		m_pPrev->m_pNext = m_pNext;
	}
	delete this;
}

void CScene::SetObjType(OBJTYPE type)
{
	m_objType = type;
}

CScene** CScene::GetTop(void)
{
	return m_pTop;
}
CScene* CScene::GetNext(void)
{
	return m_pNext;
}
CScene* CScene::GetPrev(void)
{
	return m_pPrev;
}

CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}
