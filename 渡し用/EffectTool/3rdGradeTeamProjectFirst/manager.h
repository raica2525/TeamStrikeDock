//====================================================================
//
// �����̊Ǘ����� (manager.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CMouse;
class CSound;
class CMode;
class CFade;
class CCamera;
class CLight;
class CTexture;
class CModelData;
class CEffectData;
class CEffectTool;

//================================================
// �N���X�錾
//================================================

// �}�l�[�W���[�N���X
class CManager
{
public:

    // ���[�h�̎��
    typedef enum
    {
        MODE_NONE = 0,
        MODE_TITLE,
        MODE_MANUAL,
        MODE_GAME,
        MODE_RESULT,
        MODE_MAX
    }MODE;

    CManager();
    ~CManager();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CRenderer *GetRenderer(void) { return m_pRenderer; }
    static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
    static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
    static CMouse* GetMouse(void) { return m_pMouse; }
    static CSound *GetSound(void) { return m_pSound; }
    static CCamera *GetCamera(void) { return m_pCamera; }
    static CTexture *GetTexture(void) { return m_pTexture; }
    static CModelData *GetModelData(void) { return m_pModelData; }
    static CEffectData *GetEffectData(void) { return m_pEffectData; }
	static CEffectTool *GetEffectTool(void) { return m_pEffectTool; }
    static MODE GetMode(void);
    static void SetMode(MODE mode);

    static void SetScore(int nScore) { m_nScore = nScore; }
    static int GetScore(void) { return m_nScore; }

private:
    static CRenderer *m_pRenderer;              // �����_���[�̃|�C���^
    static CInputKeyboard *m_pInputKeyboard;    // �L�[�{�[�h�̃|�C���^
    static CInputJoypad *m_pInputJoypad;        // �R���g���[���[�̃|�C���^
    static CMouse* m_pMouse;                    // �}�E�X�̃|�C���^
    static CSound *m_pSound;                    // �T�E���h�̃|�C���^
    static CMode *m_pMode;                      // ���[�h�̃|�C���^
    static CFade *m_pFade;                      // �t�F�[�h�̃|�C���^
    static CCamera *m_pCamera;                  // �J�����̃|�C���^
    static CLight *m_pLight;                    // ���C�g�̃|�C���^
    static CTexture *m_pTexture;                // �e�N�X�`���̃|�C���^
    static CModelData *m_pModelData;            // ���f���f�[�^�̃|�C���^
    static CEffectData *m_pEffectData;          // �G�t�F�N�g�f�[�^�Ǘ��ւ̃|�C���^
	static CEffectTool *m_pEffectTool;          // �G�t�F�N�g�c�[���̃|�C���^

    static int m_nScore;                        // �X�R�A
};

#endif