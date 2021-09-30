//====================================================================
//
// ���͏��� (input.cpp)
// Author : �㓡�T�V���A�r�c�I��i�R���g���[���̐U���Ή��j
//
//====================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "input.h"

//*****************************************************************************
// Joypad�p�ݒ�l
//*****************************************************************************
#define DEADZONE        2500            // �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX       10000           // �L���͈͂̍ő�l
#define RANGE_MIN       -10000          // �L���͈͂̍ŏ��l

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
CInputJoypad::CONTROLLER CInputJoypad::m_aController[MAX_PLAYER] = {};	//�r�c�ǉ�
int CInputJoypad::m_nCntController = 0;
CMouse::MSTATE CMouse::m_MState = {};

//========================================
// ���͂̃f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CInput::CInput()
{
    m_pDevice = NULL;
}

//========================================
// ���͂̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CInput::~CInput()
{

}

//========================================
// ���͂̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
    // ���łɍ�������ǂ�������
    if (m_pInput == NULL)
    {
        // DirectInput�I�u�W�F�N�g�̍쐬
        if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
        {
            return E_FAIL;
        }
    }
    else
    {
        return E_FAIL;
    }

    return S_OK;
}

//========================================
// ���͂̏I������
// Author : �㓡�T�V��
//========================================
void CInput::Uninit(void)
{
    // ���̓f�o�C�X(�L�[�{�[�h)�̊J��
    if (m_pDevice != NULL)
    {
        // �L�[�{�[�h�̃A�N�Z�X�����J��(���͐���I��)
        m_pDevice->Unacquire();
        m_pDevice->Release();
        m_pDevice = NULL;
    }

    // Directinput�I�u�W�F�N�g�̊J��
    if (m_pInput != NULL)
    {
        m_pInput->Release();
        m_pInput = NULL;
    }
}

//========================================
// �L�[�{�[�h���͂̃f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CInputKeyboard::CInputKeyboard()
{
    // memset�ł��������ł���
    memset(m_aKeyStateOld, 0, sizeof(m_aKeyStateOld));
    memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
    memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//========================================
// �L�[�{�[�h���͂̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CInputKeyboard::~CInputKeyboard()
{

}

//========================================
// �L�[�{�[�h���͂̏���������
// Author : �㓡�T�V��
//========================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
    CInput::Init(hInstance, hWnd);

    // ���̓f�o�C�X(�L�[�{�[�h)�̍쐬
    if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
    {
        return E_FAIL;
    }

    // �f�[�^�t�H�[�}�b�g���쐬
    if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
    {
        return E_FAIL;
    }

    // �������[�h��ݒ�(�t�H�A�O���E���h&��r�����[�h)
    if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
    {
        return E_FAIL;
    }

    // �L�[�{�[�h�̃A�N�Z�X�����l��
    m_pDevice->Acquire();

    return S_OK;
}

//========================================
// �L�[�{�[�h���͂̏I������
// Author : �㓡�T�V��
//========================================
void CInputKeyboard::Uninit(void)
{
    CInput::Uninit();
}

//========================================
// �L�[�{�[�h���͂̍X�V����
// Author : �㓡�T�V��
//========================================
void CInputKeyboard::Update(void)
{
    // �ϐ��錾
    BYTE aKeyState[NUM_KEY_MAX]; // �L�[�{�[�h�̌��݂̓��͏��
    int nCntKey;

    // �f�o�C�X����f�[�^���擾�i�g���K�[�j
    if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            // �g���K�[
            m_aKeyStateTrigger[nCntKey] = (m_aKeyStateOld[nCntKey] & aKeyState[nCntKey]) ^ aKeyState[nCntKey];

            // �����[�X
            m_aKeyStateRelease[nCntKey] = (m_aKeyStateOld[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

            // 1F�O�̃L�[�v���X����ۑ�
            m_aKeyStateOld[nCntKey] = aKeyState[nCntKey];
        }
    }
    else
    {
        // �L�[�{�[�h�ւ̃A�N�Z�X�����擾
        m_pDevice->Acquire();
    }
}

//========================================
// �L�[�{�[�h���͂̃v���X����
// Author : �㓡�T�V��
//========================================
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
    return(m_aKeyStateOld[nKey] & 0x80) ? true : false;
}

//========================================
// �L�[�{�[�h���͂̃g���K�[����
// Author : �㓡�T�V��
//========================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
    return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================
// �L�[�{�[�h���͂̃����[�X����
// Author : �㓡�T�V��
//========================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
    return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================================
// �W���C�p�b�h�N���X�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================================
CInputJoypad::CInputJoypad()
{
    //�e�����o�ϐ��̏�����
    memset(m_aController, 0, sizeof(m_aController));
    m_nCntController = 0;
    memset(m_aEffectInput, 0, sizeof(m_aEffectInput));
    memset(m_anFrameEffect, 0, sizeof(m_anFrameEffect));
    ZeroMemory(m_adwNumFFBAxis, sizeof(m_adwNumFFBAxis));
}

//========================================================
// �W���C�p�b�h�N���X�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================================
CInputJoypad::~CInputJoypad()
{

}

//=============================================================================
// [GetJoystickCallback] �W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
// Author : �㓡�T�V��
//=============================================================================
HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
    //�ϐ��錾
    HRESULT hr;

    //�L���X�g
    CInputJoypad *pJoypad = (CInputJoypad*)pvRef;

    //�R���g���[���[��Create
    hr = pJoypad->CInputJoypad::CreateDevice(lpddi);

    return DIENUM_CONTINUE; // ���̃f�o�C�X���
}

//=============================================================================
// [EnumObjectCallBack] �W���C�X�e�B�b�N�̃v���p�e�B��ݒ�i�R�[���o�b�N�֐��j
// Author : �r�c�I��
//=============================================================================
HRESULT CInputJoypad::EnumObjectCallBack(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{

    // ���̒l�͈̔͂�ݒ�
    // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
    // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
    // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�

    if (lpddoi->dwType & DIDFT_AXIS)
    {
        DIPROPRANGE diprg;
        ZeroMemory(&diprg, sizeof(diprg));
        diprg.diph.dwSize = sizeof(diprg);
        diprg.diph.dwHeaderSize = sizeof(diprg.diph);
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = lpddoi->dwType;
        diprg.lMin = RANGE_MIN;
        diprg.lMax = RANGE_MAX;
        for (int nCntCntoroller = 0; nCntCntoroller < m_nCntController; nCntCntoroller++)
        {
            m_aController[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
        }

        DWORD* pdwNumForceFeedbackAxis = (DWORD*)pvRef;
        if ((lpddoi->dwFlags & DIDOI_FFACTUATOR) != 0)
        {
            (*pdwNumForceFeedbackAxis)++;
        }

    }
    return DIENUM_CONTINUE; // ���̃f�o�C�X���
}

//========================================================
// �W���C�p�b�h�N���X�̏���������
// Author : �㓡�T�V���A�r�c�I��
//========================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
    //�ϐ��錾
    HRESULT	hr = NULL;

    CInput::Init(hInstance, hWnd);

    // �R���g���[���[��T��
    hr = m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, this, DIEDFL_ATTACHEDONLY); //���łɐڑ�����Ă��ăt�H�[�X�t�B�[�h�o�b�N�Ή����Ă���R���g���[���[��� //�r�c�ύX 

    for (int nCntCntroller = 0; nCntCntroller < m_nCntController; nCntCntroller++)
    {
        if (FAILED(hr))
        {
            m_aController[nCntCntroller].State.rgdwPOV[nCntCntroller] = BUTTON_NEUTRAL;
        }
        if (!m_aController[nCntCntroller].pDIJoypad)
        {
            return false;
        }

        for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
        {//�{�^���̏�����
            m_aController[nCntCntroller].Trigger.rgbButtons[nCntButton] = '\0';
        }

        //�\���L�[�@�̏�����
        for (int nCnt = 0; nCnt < 3; nCnt++)
        {
            m_aController[nCntCntroller].State.rgdwPOV[nCnt] = BUTTON_NEUTRAL;
        }

        // �R���g���[���[�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
        hr = m_aController[nCntCntroller].pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
        if (FAILED(hr))
        {
            return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s
        }

        // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
        hr = m_aController[nCntCntroller].pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
        if (FAILED(hr))
        {
            return false; // ���[�h�̐ݒ�Ɏ��s
        }

        //�r�c�ύX
        m_aController[nCntCntroller].pDIJoypad->EnumObjects((LPDIENUMDEVICEOBJECTSCALLBACK)EnumObjectCallBack, &m_adwNumFFBAxis[nCntCntroller], DIDFT_AXIS);

        //�r�c�ǉ�
        //�U���p
        //�G�t�F�N�g�̏ڍׂ�ݒ�
        DWORD rgdwAxes[2] = { DIJOFS_X , DIJOFS_Y };
        LONG  rglDirection[2] = { 1 , 1 };
        DICONSTANTFORCE cf;
        DIEFFECT        eff;

        ZeroMemory(&eff, sizeof(eff));
        eff.dwSize = sizeof(DIEFFECT);
        eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
        eff.dwDuration = INFINITE;
        eff.dwSamplePeriod = 0;
        eff.dwGain = DI_FFNOMINALMAX;
        eff.dwTriggerButton = DIEB_NOTRIGGER;
        eff.dwTriggerRepeatInterval = 0;
        eff.cAxes = m_adwNumFFBAxis[nCntCntroller];
        eff.rgdwAxes = rgdwAxes;
        eff.rglDirection = rglDirection;
        eff.lpEnvelope = 0;
        eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
        eff.lpvTypeSpecificParams = &cf;
        eff.dwStartDelay = 0;

        m_aController[nCntCntroller].pDIJoypad->CreateEffect(GUID_ConstantForce, &eff, &m_aEffectInput[nCntCntroller], NULL); //�G�t�F�N�g�𐶐�
        //�����܂�
		
        // �e�����ƂɁA�����̃]�[���l��ݒ肷��B
        // �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
        // �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
        DIPROPDWORD	dipdw;
        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
        dipdw.diph.dwHow = DIPH_BYOFFSET;
        dipdw.dwData = DEADZONE;
        //X���̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_X;
        m_aController[nCntCntroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Y���̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_Y;
        m_aController[nCntCntroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Z���̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_Z;
        m_aController[nCntCntroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Z��]�̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_RZ;
        m_aController[nCntCntroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

        //�R���g���[���[���͐���J�n
        m_aController[nCntCntroller].pDIJoypad->Acquire();
    }

    return S_OK;
}

//========================================================
// �W���C�p�b�h�N���X��Release����
// Author : �㓡�T�V���A�r�c�I��
//========================================================
void CInputJoypad::Release(void)
{
    for (int nCntDevice = 0; nCntDevice < MAX_PLAYER; nCntDevice++)
    {
        //���̓f�o�C�X�̊J��
        if (m_aController[nCntDevice].pDIJoypad != NULL)
        {
            //�R���g���[���̃A�N�Z�X�����J��
            m_aController[nCntDevice].pDIJoypad->Unacquire();
            m_aController[nCntDevice].pDIJoypad->Release();
            m_aController[nCntDevice].pDIJoypad = NULL;
        }
        if (m_aEffectInput[nCntDevice] != NULL)	//�r�c�ǉ�
        {
            m_aEffectInput[nCntDevice]->Release();
            m_aEffectInput[nCntDevice] = NULL;
        }
    }
    m_nCntController = 0;
    CInput::Uninit();
}

//========================================================
// �W���C�p�b�h�N���X�̍X�V����
// Author : �㓡�T�V���A�r�c�I��
//========================================================
void CInputJoypad::Update(void)
{
    DIJOYSTATE2 ControllerState;
    HRESULT hr;
    for (int nCntCntoroller = 0; nCntCntoroller < MAX_PLAYER; nCntCntoroller++)
    {
        //�\���L�[�̓��͂��A�����Ȃ��悤�ɂ��Ă���
        m_aController[nCntCntoroller].State.rgdwPOV[0] = BUTTON_NEUTRAL;

        if (m_aController[nCntCntoroller].pDIJoypad != NULL)
        {

            hr = m_aController[nCntCntoroller].pDIJoypad->Poll();

            //�f�o�C�X����f�[�^���擾
            if (SUCCEEDED(hr = m_aController[nCntCntoroller].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
            {
                m_aController[nCntCntoroller].State.lY = ControllerState.lY;
                m_aController[nCntCntoroller].State.lX = ControllerState.lX;

                m_aController[nCntCntoroller].State.lZ = ControllerState.lZ;		    //�X�e�B�b�N�̉��iZ���j
                m_aController[nCntCntoroller].State.lRz = ControllerState.lRz;		//�X�e�B�b�N�̏c�iZ��]�j

                                                                                    //�\���L�[�̏��
                for (int nCnt = 0; nCnt < 3; nCnt++)
                {
                    //�\���L�[�v���X����ۑ�
                    m_aController[nCntCntoroller].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

                }

                //�{�^���̏��
                for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
                {
                    //�L�[�g���K�[����ۑ�
                    m_aController[nCntCntoroller].Trigger.rgbButtons[nCntButton] = (m_aController[nCntCntoroller].State.rgbButtons[nCntButton] & ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

                    //�L�[�����[�X����ۑ�
                    m_aController[nCntCntoroller].Release.rgbButtons[nCntButton] = (m_aController[nCntCntoroller].State.rgbButtons[nCntButton] | ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

                    //�L�[�v���X����ۑ�
                    m_aController[nCntCntoroller].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];

                }
            }
            else
            {
                //�R���g���[���[�̃A�N�Z�X�����擾
                m_aController[nCntCntoroller].pDIJoypad->Acquire();
            }
            //�r�c�ǉ�
            if (m_anFrameEffect[nCntCntoroller] >= 0)	//�U�����~���邽�߂̏���
            {
                if (--m_anFrameEffect[nCntCntoroller] <= 0)
                {
                    StopEffect(nCntCntoroller);
                }
            }
            //�����܂�
        }
    }
}

//========================================================
// �W���C�p�b�h�N���X��Press����
// Author : �㓡�T�V��
//========================================================
bool CInputJoypad::GetJoypadPress(int nController, int nButton)
{
    return(m_aController[nController].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//========================================================
// �W���C�p�b�h�N���X��Trigger����
// Author : �㓡�T�V��
//========================================================
bool CInputJoypad::GetJoypadTrigger(int nController, int nButton)
{
    return(m_aController[nController].Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//========================================================
// �W���C�p�b�h�N���X��Release����
// Author : �㓡�T�V��
//========================================================
bool CInputJoypad::GetJoypadRelease(int nController, int nButton)
{
    return(m_aController[nController].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �W���C�p�b�h�N���X�̃R���g���[���̏�Ԃ��擾
// Author : �㓡�T�V��
//=============================================================================
DIJOYSTATE2 CInputJoypad::GetController(int nController)
{
    return m_aController[nController].State;
}

//=============================================================================
// �R���g���[���[�̃f�o�C�X��Create
// Author : �㓡�T�V��
//=============================================================================
HRESULT CInputJoypad::CreateDevice(LPDIDEVICEINSTANCE lpddi)
{
    HRESULT hr = m_pInput->CreateDevice(lpddi->guidInstance, &m_aController[m_nCntController].pDIJoypad, NULL);

    // �������R���g���[���[���J�E���g
    m_nCntController++;

    return hr;
}

//=============================================================================
// �R���g���[���[�̐U�����J�n(nFrame���̒l����͂����StopEffect���Ă΂��܂Łj
// Author : �r�c�I��
//=============================================================================
HRESULT CInputJoypad::StartEffect(int nController, int nFrame)
{
    HRESULT hr = E_FAIL;
    if (m_aEffectInput[nController] != NULL)
    {
        hr = m_aEffectInput[nController]->Start(1, DIES_SOLO);
        m_anFrameEffect[nController] = nFrame;
    }
    return hr;
}

//=============================================================================
// �R���g���[���[�̐U�����I��
// Author : �r�c�I��
//=============================================================================
HRESULT CInputJoypad::StopEffect(int nController)
{
    HRESULT hr = E_FAIL;
    if (m_aEffectInput[nController] != NULL)
    {
        hr = m_aEffectInput[nController]->Stop();
    }
    return hr;
}


//========================================================
// �}�E�X�N���X�̃R���X�g���N�^
// Author : �㓡�T�V��
//========================================================
CMouse::CMouse()
{

}

//========================================================
// �}�E�X�N���X�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================================
CMouse::~CMouse()
{

}

//========================================================
// �}�E�X�̏�����
// Author : �㓡�T�V��
//========================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
    {
        return E_FAIL;
    }

    m_pDevice->SetDataFormat(&c_dfDIMouse2); //ϳ��p���ް��E̫�ϯĐݒ�
    m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    DIPROPDWORD diprop;
    diprop.diph.dwSize = sizeof(diprop);
    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
    diprop.diph.dwObj = 0;
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.dwData = DIPROPAXISMODE_REL; // ��Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j�i���Βl��DIPROPAXISMODE_REL�j
    m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

    SetRect(&m_MState.moveRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	// �}�E�X�J�[�\���̓����͈�
    m_MState.x = m_MState.moveRect.left;	// �}�E�X�J�[�\���̂w���W��������
    m_MState.y = m_MState.moveRect.top;	// �}�E�X�J�[�\���̂x���W��������
    m_MState.moveAdd = 1;	// �}�E�X�J�[�\���̈ړ��ʂ�ݒ�

    m_pDevice->Acquire();
    return S_OK;
}

//========================================================
// �}�E�X�̏I������
// Author : �㓡�T�V��
//========================================================
void CMouse::Uninit(void)
{
    if (m_pDevice != NULL)
    {
        m_pDevice->Unacquire();
        m_pDevice->Release();
        m_pDevice = NULL;
    }

    if (m_pInput != NULL)
    {
        m_pInput->Release();
        m_pInput = NULL;
    }
}


//========================================================
// �}�E�X�̍X�V����
// Author : �㓡�T�V��
//========================================================
void CMouse::Update(void)
{
    m_dIMouseState.rgbButtons[0] = 0;

    m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_dIMouseState);

    m_pDevice->Acquire();

    m_MState.x += (m_dIMouseState.lX * m_MState.moveAdd);
    m_MState.y += (m_dIMouseState.lY * m_MState.moveAdd);
    if (m_MState.x < m_MState.moveRect.left) m_MState.x = m_MState.moveRect.left;
    if (m_MState.x > m_MState.moveRect.right - m_MState.imgWidth) m_MState.x = m_MState.moveRect.right - m_MState.imgWidth;
    if (m_MState.y < m_MState.moveRect.top) m_MState.y = m_MState.moveRect.top;
    if (m_MState.y > m_MState.moveRect.bottom - m_MState.imgHeight) m_MState.y = m_MState.moveRect.bottom - m_MState.imgHeight;
    (m_dIMouseState.rgbButtons[0] & 0x80) ? m_MState.lButton = true : m_MState.lButton = false;
    (m_dIMouseState.rgbButtons[1] & 0x80) ? m_MState.rButton = true : m_MState.rButton = false;
    (m_dIMouseState.rgbButtons[2] & 0x80) ? m_MState.cButton = true : m_MState.cButton = false;
}

//=============================================================================
// �}�E�X�̍��g���K�[
// Author : �㓡�T�V��
//=============================================================================
bool CMouse::GetMouseTriggerLeft(void)
{
    // �O�Ɏ擾�������̃}�E�X�̏��
    static bool prevState[sizeof(m_dIMouseState.rgbButtons) / sizeof(m_dIMouseState.rgbButtons[0])];

    // ���̃}�E�X�̏��
    bool current = m_MState.lButton;

    // �O�̏�Ԃ�false�ŁA���̏�Ԃ�true�Ȃ�΁A�N���b�N�����u�ԂƔ��肷��
    bool ret = current && !prevState[0];

    // ���̏�Ԃ�ۑ�����
    prevState[0] = current;

    // ���茋�ʂ�Ԃ�
    return ret;
}

//=============================================================================
// �}�E�X�̉E�g���K�[
// Author : �㓡�T�V��
//=============================================================================
bool CMouse::GetMouseTriggerRight(void)
{
    // �O�Ɏ擾�������̃}�E�X�̏��
    static bool prevState[sizeof(m_dIMouseState.rgbButtons) / sizeof(m_dIMouseState.rgbButtons[0])];

    // ���̃}�E�X�̏��
    bool current = m_MState.rButton;

    // �O�̏�Ԃ�false�ŁA���̏�Ԃ�true�Ȃ�΁A�N���b�N�����u�ԂƔ��肷��
    bool ret = current && !prevState[0];

    // ���̏�Ԃ�ۑ�����
    prevState[0] = current;

    // ���茋�ʂ�Ԃ�
    return ret;
}

//=============================================================================
// �}�E�X�̍��v���X
// Author : �㓡�T�V��
//=============================================================================
bool CMouse::GetMousePressLeft(void)
{
    // ���̃}�E�X�̏��
    bool current = m_MState.lButton;

    return current;
}

//=============================================================================
// �}�E�X�̉E�v���X
// Author : �㓡�T�V��
//=============================================================================
bool CMouse::GetMousePressRight(void)
{
    // ���̃}�E�X�̏��
    bool current = m_MState.rButton;

    return current;
}

//=============================================================================
// �}�E�X�̏�Ԃ��擾
// Author : �㓡�T�V��
//=============================================================================
CMouse::MSTATE *CMouse::GetMouse(void)
{
    return &m_MState;
}