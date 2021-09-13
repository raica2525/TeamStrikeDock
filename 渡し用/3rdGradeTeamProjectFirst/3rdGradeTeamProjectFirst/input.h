//====================================================================
//
// ���͏��� (input.h)
// Author : �㓡�T�V���A�r�c�I��i�R���g���[���̐U���Ή��j
//
//====================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �}�N����`
//================================================
#define NUM_KEY_MAX (256)//�L�[�̍ő吔

#define BUTTON_UP     (0)	    // �\���L�[��
#define BUTTON_DOWN   (18000)	// �\���L�[��
#define BUTTON_LEFT   (27000)	// �\���L�[��
#define BUTTON_RIGHT  (9000)	// �\���L�[�E

#define BUTTON_UP_RIGHT     (4500)	// �\���L�[��E
#define BUTTON_RIGHT_DOWN  (13500)	// �\���L�[�E��
#define BUTTON_DOWN_LEFT   (22500)	// �\���L�[����
#define BUTTON_LEFT_UP   (31500)	// �\���L�[����

#define BUTTON_NEUTRAL -1       // �������͂��Ă��Ȃ��Ƃ�
#define CROSSZONE (4500)        // �\���L�[�̋��e�͈�

// �X�e�B�b�N�̊e�͈͂𔻕�
#define STICK_UP(angle) angle < D3DXToRadian(45.0f) && angle > D3DXToRadian(-45.0f)
#define STICK_RIGHT(angle) angle >= D3DXToRadian(45.0f) && angle <= D3DXToRadian(135.0f)
#define STICK_DOWN(angle) angle > D3DXToRadian(135.0f) && angle <= D3DXToRadian(180.0f) || angle < D3DXToRadian(-135.0f) && angle > D3DXToRadian(-180.0f)
#define STICK_LEFT(angle) angle <= D3DXToRadian(-45.0f) && angle >= D3DXToRadian(-135.0f)

// �X�e�B�b�N�j���[�g����
#define STICK_NEUTRAL -1000

// �X�e�B�b�N�̍ő�X��
#define STICK_MAX_TILT 10000.0f

//================================================
// �N���X�錾
//================================================

// ���͂̃N���X
class CInput
{
public:
    CInput();
    virtual ~CInput();  // �e�N���X�̃f�X�g���N�^��virtual�����邱�ƂŁA���������[�N��}����
    virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
    virtual void Uninit(void);
    virtual void Update(void) = 0;

protected:
    LPDIRECTINPUTDEVICE8 m_pDevice;	// ���̓f�o�C�X�ւ̃|�C���^
    static LPDIRECTINPUT8 m_pInput;	// DirectInput�ւ̃|�C���^
};

// �L�[�{�[�h���͂̃N���X
class CInputKeyboard : public CInput
{
public:
    CInputKeyboard();
    ~CInputKeyboard();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd);
    void Uninit(void);
    void Update(void);
    bool GetKeyboardPress(int nKey);
    bool GetKeyboardTrigger(int nKey);
    bool GetKeyboardRelease(int nKey);

private:
    BYTE m_aKeyStateOld[NUM_KEY_MAX];       // 1F�O�̃L�[�{�[�h�̓��͏�񃏁[�N
    BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[���
    BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X���
};

// �W���C�p�b�h�̃N���X
class CInputJoypad :public CInput
{
public:

    // �{�^���̎��
    typedef enum
    {
        BUTTON_X = 0,
        BUTTON_Y,
        BUTTON_A,
        BUTTON_B,
        BUTTON_L1,
        BUTTON_R1,
        BUTTON_L2,
        BUTTON_R2,
        BUTTON_L3,
        BUTTON_R3,
        BUTTON_SELECT,
        BUTTON_START,
        BUTTON_GUIDE,
        MAX_BUTTON
    }BUTTON;

    // �R���g���[���̏��
    typedef struct
    {
        LPDIRECTINPUTDEVICE8 pDIJoypad = NULL; // �R���g���[���[�f�o�C�X
        DIJOYSTATE2 State;       // �R���g���[���[�̃v���X���
        DIJOYSTATE2 Trigger;     // �R���g���[���[�̃g���K�[���
        DIJOYSTATE2 Release;     // �R���g���[���[�̃v���X���
    }CONTROLLER;

    CInputJoypad();
    ~CInputJoypad();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd);
    void Update(void);
    void Release(void);
    bool GetJoypadPress(int nController, int nButton);
    bool GetJoypadTrigger(int nController, int nButton);
    bool GetJoypadRelease(int nController, int nButton);
    DIJOYSTATE2 GetController(int nController);
    static LPDIRECTINPUT8 GetInput(void) { return m_pInput; }
    HRESULT CreateDevice(LPDIDEVICEINSTANCE lpddi);
    HRESULT StartEffect(int nController, int nFrame);	//�r�c�ǉ�
    HRESULT StopEffect(int nController);				//�r�c�ǉ�
    static HRESULT CALLBACK EnumObjectCallBack(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
private:
    static CONTROLLER m_aController[MAX_PLAYER];    // �R���g���[���[�̏��	//�r�c�C��
    static int m_nCntController;					// �R���g���[���[�̐ڑ���
    LPDIRECTINPUTEFFECT m_aEffectInput[MAX_PLAYER];	// �G�t�F�N�g�̃|�C���^//�r�c
    int m_anFrameEffect[MAX_PLAYER];				// �U������c��t���[��//�r�c
    DWORD m_adwNumFFBAxis[MAX_PLAYER];				// �U���̎��̐�//�r�c
};

// �}�E�X�̃N���X
class CMouse : public CInput
{
public:
    typedef struct _MSTATE {
        RECT    moveRect;     // ��ʏ�œ�����͈�
        int     x;            // X���W
        int     y;            // Y���W
        bool    lButton;      // ���{�^��
        bool    rButton;      // �E�{�^��
        bool    cButton;      // �^�񒆃{�^��
        int     moveAdd;      // �ړ���
        RECT    imgRect;      // �}�E�X�p�摜��`
        int     imgWidth;     // �}�E�X�摜��
        int     imgHeight;    // �}�E�X�摜����
    } MSTATE;

    CMouse();
    ~CMouse();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd);
    void Uninit(void);
    void Update(void);

    static MSTATE *GetMouse(void);
    bool GetMouseTriggerLeft(void);
    bool GetMouseTriggerRight(void);
    bool GetMousePressLeft(void);
    bool GetMousePressRight(void);

private:
    DIMOUSESTATE2 m_dIMouseState;
    static CMouse::MSTATE m_MState;
};

#endif