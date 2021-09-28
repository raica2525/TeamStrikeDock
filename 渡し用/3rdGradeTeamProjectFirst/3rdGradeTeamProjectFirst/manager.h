//====================================================================
//
// �����̊Ǘ����� (manager.h)
// Author : �㓡�T�V���A�r�c�I��i�R���g���[���̍Đڑ��Ή��j
//
//====================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "sound.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CMouse;
class CMode;
class CFade;
class CCamera;
class CLight;
class CTexture;
class CModelData;
class CEffectData;

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
        MODE_DEBUG_MENU = 0,// �f�o�b�O���j���[
        MODE_TITLE,         // �^�C�g��
        MODE_MANUAL,        // �V�ѕ�
        MODE_CUSTOM,        // �J�X�^�}�C�Y���
        MODE_GAME,          // �Q�[��
        MODE_RESULT,        // ���U���g
        MODE_MENU,          // ���j���[
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
    static CCamera *GetCamera(void) { return m_pCamera; }
    static CTexture *GetTexture(void) { return m_pTexture; }
    static CModelData *GetModelData(void) { return m_pModelData; }
    static CEffectData *GetEffectData(void) { return m_pEffectData; }
    static MODE GetMode(void);
    static void SetMode(MODE mode);

    static void SetScore(int nScore) { m_nScore = nScore; }
    static int GetScore(void) { return m_nScore; }

    /*========================================================
    // ������
    //======================================================*/
    static void SoundPlay(CSound::LABEL label) { m_pSound->Play(label); }
    static void SoundStop(CSound::LABEL label) { m_pSound->Stop(label); }
    static void SoundStopAll(void) { m_pSound->StopAll(); }

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

    static int m_nScore;                        // �X�R�A
};

#endif