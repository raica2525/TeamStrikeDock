//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	manager.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�O���錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CRenderer;
class CInput;
class CCamera;
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�N���X��`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CManager
{
public:
	CManager();		//�R���X�g���N�^
	~CManager();	//�f�X�g���N�^

	HRESULT Init(HWND hWnd, bool bWindow, HINSTANCE hInstance);	//�������֐�
	void Uninit(void);						//�I���֐�
	void Update(void);						//�X�V�֐�
	void Draw(void);						//�`��֐�
	static CRenderer* GetRenderer(void);	//�Q�b�^�[
	static CInput* GetInput(void);
	static CCamera* GetCamera(int nIndex);
	static void LoadFile(void);
	static void UnloadFiles(void);
	static bool GetUseShadow(void);
private:
	static CRenderer* m_pRenderer;			//�����_���̃|�C���^
	static CInput* m_pInput;				//�C���v�b�g�̃|�C���^
	static CCamera* m_pCamera;
	static bool m_bImShowAnotherWindow;
	static bool m_bUseShadow;
};

#endif

