//====================================================================
//
// 入力処理 (input.cpp)
// Author : 後藤慎之助
//
//====================================================================

//========================
// インクルードファイル
//========================
#include "input.h"

//*****************************************************************************
// Joypad用設定値
//*****************************************************************************
#define DEADZONE        2500            // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       10000           // 有効範囲の最大値
#define RANGE_MIN       -10000          // 有効範囲の最小値

//========================================
// 静的メンバ変数宣言
//========================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
int CInputJoypad::m_nCntController = 0;
CMouse::MSTATE CMouse::m_MState = {};

//========================================
// 入力のデフォルトコンストラクタ
// Author : 後藤慎之助
//========================================
CInput::CInput()
{
    m_pDevice = NULL;
}

//========================================
// 入力のデストラクタ
// Author : 後藤慎之助
//========================================
CInput::~CInput()
{

}

//========================================
// 入力の初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
    // すでに作ったかどうか判別
    if (m_pInput == NULL)
    {
        // DirectInputオブジェクトの作成
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
// 入力の終了処理
// Author : 後藤慎之助
//========================================
void CInput::Uninit(void)
{
    // 入力デバイス(キーボード)の開放
    if (m_pDevice != NULL)
    {
        // キーボードのアクセス権を開放(入力制御終了)
        m_pDevice->Unacquire();
        m_pDevice->Release();
        m_pDevice = NULL;
    }

    // Directinputオブジェクトの開放
    if (m_pInput != NULL)
    {
        m_pInput->Release();
        m_pInput = NULL;
    }
}

//========================================
// キーボード入力のデフォルトコンストラクタ
// Author : 後藤慎之助
//========================================
CInputKeyboard::CInputKeyboard()
{
    // memsetでも初期化できる
    memset(m_aKeyStateOld, 0, sizeof(m_aKeyStateOld));
    memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
    memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//========================================
// キーボード入力のデストラクタ
// Author : 後藤慎之助
//========================================
CInputKeyboard::~CInputKeyboard()
{

}

//========================================
// キーボード入力の初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
    CInput::Init(hInstance, hWnd);

    // 入力デバイス(キーボード)の作成
    if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
    {
        return E_FAIL;
    }

    // データフォーマットを作成
    if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
    {
        return E_FAIL;
    }

    // 協調モードを設定(フォアグラウンド&非排他モード)
    if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
    {
        return E_FAIL;
    }

    // キーボードのアクセス権を獲得
    m_pDevice->Acquire();

    return S_OK;
}

//========================================
// キーボード入力の終了処理
// Author : 後藤慎之助
//========================================
void CInputKeyboard::Uninit(void)
{
    CInput::Uninit();
}

//========================================
// キーボード入力の更新処理
// Author : 後藤慎之助
//========================================
void CInputKeyboard::Update(void)
{
    // 変数宣言
    BYTE aKeyState[NUM_KEY_MAX]; // キーボードの現在の入力情報
    int nCntKey;

    // デバイスからデータを取得（トリガー）
    if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            // トリガー
            m_aKeyStateTrigger[nCntKey] = (m_aKeyStateOld[nCntKey] & aKeyState[nCntKey]) ^ aKeyState[nCntKey];

            // リリース
            m_aKeyStateRelease[nCntKey] = (m_aKeyStateOld[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

            // 1F前のキープレス情報を保存
            m_aKeyStateOld[nCntKey] = aKeyState[nCntKey];
        }
    }
    else
    {
        // キーボードへのアクセス権を取得
        m_pDevice->Acquire();
    }
}

//========================================
// キーボード入力のプレス処理
// Author : 後藤慎之助
//========================================
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
    return(m_aKeyStateOld[nKey] & 0x80) ? true : false;
}

//========================================
// キーボード入力のトリガー処理
// Author : 後藤慎之助
//========================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
    return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================
// キーボード入力のリリース処理
// Author : 後藤慎之助
//========================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
    return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================================
// ジョイパッドクラスのコンストラクタ
// Author : 後藤慎之助
//========================================================
CInputJoypad::CInputJoypad()
{
    //各メンバ変数の初期化
    memset(m_Controller, 0, sizeof(m_Controller));
}

//========================================================
// ジョイパッドクラスのデストラクタ
// Author : 後藤慎之助
//========================================================
CInputJoypad::~CInputJoypad()
{

}

//=============================================================================
// [GetJoystickCallback] ジョイスティック・デバイスを列挙（コールバック関数）
// Author : 後藤慎之助
//=============================================================================
HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
    //変数宣言
    HRESULT hr;

    //キャスト
    CInputJoypad *pJoypad = (CInputJoypad*)pvRef;

    //コントローラーをCreate
    hr = pJoypad->CInputJoypad::CreateDevice(lpddi);

    return DIENUM_CONTINUE; // 次のデバイスを列挙
}

//========================================================
// ジョイパッドクラスの初期化処理
// Author : 後藤慎之助
//========================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
    //変数宣言
    HRESULT	hr = NULL;

    CInput::Init(hInstance, hWnd);

    // コントローラーを探す
    hr = m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, this, DIEDFL_ATTACHEDONLY);

    for (int nCntCntoroller = 0; nCntCntoroller < m_nCntController; nCntCntoroller++)
    {
        if (FAILED(hr))
        {
            m_Controller[nCntCntoroller].State.rgdwPOV[nCntCntoroller] = BUTTON_NEUTRAL;
        }
        if (!m_Controller[nCntCntoroller].pDIJoypad)
        {
            return false;
        }

        for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
        {//ボタンの初期化
            m_Controller[nCntCntoroller].Trigger.rgbButtons[nCntButton] = '\0';
        }

        //十字キー　の初期化
        for (int nCnt = 0; nCnt < 3; nCnt++)
        {
            m_Controller[nCntCntoroller].State.rgdwPOV[nCnt] = BUTTON_NEUTRAL;
        }

        // コントローラー用のデータ・フォーマットを設定
        hr = m_Controller[nCntCntoroller].pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
        if (FAILED(hr))
        {
            return false; // データフォーマットの設定に失敗
        }

        // モードを設定（フォアグラウンド＆非排他モード）
        hr = m_Controller[nCntCntoroller].pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
        if (FAILED(hr))
        {
            return false; // モードの設定に失敗
        }

        // 軸の値の範囲を設定
        // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
        // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
        // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)
        DIPROPRANGE diprg;
        ZeroMemory(&diprg, sizeof(diprg));
        diprg.diph.dwSize = sizeof(diprg);
        diprg.diph.dwHeaderSize = sizeof(diprg.diph);
        diprg.diph.dwHow = DIPH_BYOFFSET;
        diprg.lMin = RANGE_MIN;
        diprg.lMax = RANGE_MAX;
        // X軸の範囲を設定
        diprg.diph.dwObj = DIJOFS_X;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
        // Y軸の範囲を設定
        diprg.diph.dwObj = DIJOFS_Y;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
        // Z軸の範囲を設定
        diprg.diph.dwObj = DIJOFS_Z;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
        // Z回転の範囲を設定
        diprg.diph.dwObj = DIJOFS_RZ;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

        // 各軸ごとに、無効のゾーン値を設定する。
        // 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
        // 指定する値は、10000に対する相対値(2000なら20パーセント)。
        DIPROPDWORD	dipdw;
        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
        dipdw.diph.dwHow = DIPH_BYOFFSET;
        dipdw.dwData = DEADZONE;
        //X軸の無効ゾーンを設定
        dipdw.diph.dwObj = DIJOFS_X;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Y軸の無効ゾーンを設定
        dipdw.diph.dwObj = DIJOFS_Y;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Z軸の無効ゾーンを設定
        dipdw.diph.dwObj = DIJOFS_Z;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Z回転の無効ゾーンを設定
        dipdw.diph.dwObj = DIJOFS_RZ;
        m_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

        //コントローラー入力制御開始
        m_Controller[nCntCntoroller].pDIJoypad->Acquire();
    }
    return S_OK;
}

//========================================================
// ジョイパッドクラスのRelease処理
// Author : 後藤慎之助
//========================================================
void CInputJoypad::Release(void)
{
    for (int nCntDevise = 0; nCntDevise < MAX_PLAYER; nCntDevise++)
    {
        //入力デバイスの開放
        if (m_Controller[nCntDevise].pDIJoypad != NULL)
        {
            //コントローラのアクセス権を開放
            m_Controller[nCntDevise].pDIJoypad->Unacquire();
            m_Controller[nCntDevise].pDIJoypad->Release();
            m_Controller[nCntDevise].pDIJoypad = NULL;
        }
    }

    CInput::Uninit();
}

//========================================================
// ジョイパッドクラスの更新処理
// Author : 後藤慎之助
//========================================================
void CInputJoypad::Update(void)
{
    DIJOYSTATE2 ControllerState;
    HRESULT hr;

    for (int nCntCntoroller = 0; nCntCntoroller < MAX_PLAYER; nCntCntoroller++)
    {
        //十字キーの入力を、何もないようにしておく
        m_Controller[nCntCntoroller].State.rgdwPOV[0] = BUTTON_NEUTRAL;

        if (m_Controller[nCntCntoroller].pDIJoypad != NULL)
        {

            hr = m_Controller[nCntCntoroller].pDIJoypad->Poll();

            //デバイスからデータを取得
            if (SUCCEEDED(hr = m_Controller[nCntCntoroller].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
            {
                m_Controller[nCntCntoroller].State.lY = ControllerState.lY;
                m_Controller[nCntCntoroller].State.lX = ControllerState.lX;

                m_Controller[nCntCntoroller].State.lZ = ControllerState.lZ;		    //スティックの横（Z軸）
                m_Controller[nCntCntoroller].State.lRz = ControllerState.lRz;		//スティックの縦（Z回転）

                                                                                    //十字キーの情報
                for (int nCnt = 0; nCnt < 3; nCnt++)
                {
                    //十字キープレス情報を保存
                    m_Controller[nCntCntoroller].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

                }

                //ボタンの情報
                for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
                {
                    //キートリガー情報を保存
                    m_Controller[nCntCntoroller].Trigger.rgbButtons[nCntButton] = (m_Controller[nCntCntoroller].State.rgbButtons[nCntButton] & ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

                    //キーリリース情報を保存
                    m_Controller[nCntCntoroller].Release.rgbButtons[nCntButton] = (m_Controller[nCntCntoroller].State.rgbButtons[nCntButton] | ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

                    //キープレス情報を保存
                    m_Controller[nCntCntoroller].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];

                }
            }
            else
            {
                //コントローラーのアクセス権を取得
                m_Controller[nCntCntoroller].pDIJoypad->Acquire();
            }
        }
    }
}

//========================================================
// ジョイパッドクラスのPress処理
// Author : 後藤慎之助
//========================================================
bool CInputJoypad::GetJoypadPress(int nController, int nButton)
{
    return(m_Controller[nController].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//========================================================
// ジョイパッドクラスのTrigger処理
// Author : 後藤慎之助
//========================================================
bool CInputJoypad::GetJoypadTrigger(int nController, int nButton)
{
    return(m_Controller[nController].Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//========================================================
// ジョイパッドクラスのRelease処理
// Author : 後藤慎之助
//========================================================
bool CInputJoypad::GetJoypadRelease(int nController, int nButton)
{
    return(m_Controller[nController].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// ジョイパッドクラスのコントローラの状態を取得
// Author : 後藤慎之助
//=============================================================================
DIJOYSTATE2 CInputJoypad::GetController(int nContllre)
{
    return m_Controller[nContllre].State;
}

//=============================================================================
// コントローラーのデバイスをCreate
// Author : 後藤慎之助
//=============================================================================
HRESULT CInputJoypad::CreateDevice(LPDIDEVICEINSTANCE lpddi)
{
    HRESULT hr = m_pInput->CreateDevice(lpddi->guidInstance, &m_Controller[m_nCntController].pDIJoypad, NULL);

    // 増えたコントローラーをカウント
    m_nCntController++;

    return hr;
}

//========================================================
// マウスクラスのコンストラクタ
// Author : 後藤慎之助
//========================================================
CMouse::CMouse()
{

}

//========================================================
// マウスクラスのデストラクタ
// Author : 後藤慎之助
//========================================================
CMouse::~CMouse()
{

}

//========================================================
// マウスの初期化
// Author : 後藤慎之助
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

    m_pDevice->SetDataFormat(&c_dfDIMouse2); //ﾏｳｽ用のﾃﾞｰﾀ・ﾌｫｰﾏｯﾄ設定
    m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    DIPROPDWORD diprop;
    diprop.diph.dwSize = sizeof(diprop);
    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
    diprop.diph.dwObj = 0;
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.dwData = DIPROPAXISMODE_REL; // 絶対値モードで設定（絶対値はDIPROPAXISMODE_ABS）（相対値はDIPROPAXISMODE_REL）
    m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

    SetRect(&m_MState.moveRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	// マウスカーソルの動く範囲
    m_MState.x = m_MState.moveRect.left;	// マウスカーソルのＸ座標を初期化
    m_MState.y = m_MState.moveRect.top;	// マウスカーソルのＹ座標を初期化
    m_MState.moveAdd = 1;	// マウスカーソルの移動量を設定

    m_pDevice->Acquire();
    return S_OK;
}

//========================================================
// マウスの終了処理
// Author : 後藤慎之助
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
// マウスの更新処理
// Author : 後藤慎之助
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
// マウスの左トリガー
// Author : 後藤慎之助
//=============================================================================
bool CMouse::GetMouseTriggerLeft(void)
{
    // 前に取得した時のマウスの状態
    static bool prevState[sizeof(m_dIMouseState.rgbButtons) / sizeof(m_dIMouseState.rgbButtons[0])];

    // 今のマウスの状態
    bool current = m_MState.lButton;

    // 前の状態がfalseで、今の状態がtrueならば、クリックした瞬間と判定する
    bool ret = current && !prevState[0];

    // 今の状態を保存する
    prevState[0] = current;

    // 判定結果を返す
    return ret;
}

//=============================================================================
// マウスの右トリガー
// Author : 後藤慎之助
//=============================================================================
bool CMouse::GetMouseTriggerRight(void)
{
    // 前に取得した時のマウスの状態
    static bool prevState[sizeof(m_dIMouseState.rgbButtons) / sizeof(m_dIMouseState.rgbButtons[0])];

    // 今のマウスの状態
    bool current = m_MState.rButton;

    // 前の状態がfalseで、今の状態がtrueならば、クリックした瞬間と判定する
    bool ret = current && !prevState[0];

    // 今の状態を保存する
    prevState[0] = current;

    // 判定結果を返す
    return ret;
}

//=============================================================================
// マウスの左プレス
// Author : 後藤慎之助
//=============================================================================
bool CMouse::GetMousePressLeft(void)
{
    // 今のマウスの状態
    bool current = m_MState.lButton;

    return current;
}

//=============================================================================
// マウスの右プレス
// Author : 後藤慎之助
//=============================================================================
bool CMouse::GetMousePressRight(void)
{
    // 今のマウスの状態
    bool current = m_MState.rButton;

    return current;
}

//=============================================================================
// マウスの状態を取得
// Author : 後藤慎之助
//=============================================================================
CMouse::MSTATE *CMouse::GetMouse(void)
{
    return &m_MState;
}