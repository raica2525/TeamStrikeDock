//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	scene.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

#define PRIORITY_MAX	(3)

//�I�u�W�F�N�g�N���X

class CScene
{
public:
	
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_EFFECT
	}OBJTYPE;
	CScene(int nPriority = 1);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;		//�������z�֐��Q
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void UpdateAll(void);		//�S�V�[�������Q
	static void DrawAll(void);
	static void DrawExceptEffect(void);
	static void DrawEffect(void);
	static void ReleaseAll(void);
	void SetObjType(OBJTYPE type);
	static CScene** GetTop(void);
	CScene* GetNext(void);
	CScene* GetPrev(void);
	OBJTYPE GetObjType(void);
protected:
	void Release(void);					//�V�[���̊J��

private:
	void Death(void);
	static CScene* m_pTop[PRIORITY_MAX];
	static CScene* m_pCur[PRIORITY_MAX];
	CScene* m_pPrev;
	CScene* m_pNext;
	int m_nPriority;
	bool m_bDestroy;
	OBJTYPE m_objType;
};

#endif
