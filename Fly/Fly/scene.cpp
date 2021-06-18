//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
// scene.cpp
// Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・


//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "scene.h"
#include "renderer.h"
#include "scene2d.h"
#include "debug.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//静的メンバ変数宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CScene* CScene::m_pTop[PRIORITY_MAX] = {};
CScene* CScene::m_pCur[PRIORITY_MAX] = {};

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
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

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CScene::~CScene()
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//更新処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
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

	//死亡フラグ立ってるやつを殺す
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

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//描画処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・ 
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

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//全開放処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
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

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//開放処理 
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CScene::Release(void)
{
	//死亡フラグを立てる
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
