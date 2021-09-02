//===============================================
//
// 生成の管理処理 (manager.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "manager.h"
#include "scene2d.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "mode.h"
#include "title.h"
#include "game.h"
#include "ui.h"
#include <typeinfo.h>
#include "fade.h"
#include "light.h"
#include "camera.h"
#include "result.h"
#include "manual.h"
#include "texture.h"
#include "modelData.h"
#include "effectData.h"
#include "debug.h"
#include "custom.h"
#include "text.h"

//========================================
// マクロ定義
//========================================

//========================================
// 静的メンバ変数宣言
//========================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CMouse *CManager::m_pMouse = NULL;
CSound *CManager::m_pSound = NULL;
CMode *CManager::m_pMode = NULL;
CFade *CManager::m_pFade = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CTexture *CManager::m_pTexture = NULL;
CModelData *CManager::m_pModelData = NULL;
CEffectData *CManager::m_pEffectData = NULL;

int CManager::m_nScore = 0;

//========================================
// 生成の管理のデフォルトコンストラクタ
// Author : 後藤慎之助
//========================================
CManager::CManager()
{

}

//========================================
// 生成の管理のデストラクタ
// Author : 後藤慎之助
//========================================
CManager::~CManager()
{

}

//========================================
// 生成の管理の初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
    // レンダラーの生成(一回しか生成しないものは、Createを用意していない)
    m_pRenderer = new CRenderer;
    if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
    {
        return E_FAIL;
    }

    // キーボードの生成(newとInitは連続させて書く)
    m_pInputKeyboard = new CInputKeyboard;
    if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
    {
        return E_FAIL;
    }

    // コントローラの生成
    m_pInputJoypad = new CInputJoypad;
    if (FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
    {
        return E_FAIL;
    }

    // マウスの生成
    m_pMouse = new CMouse;
    if (FAILED(m_pMouse->Init(hInstance, hWnd)))
    {
        return E_FAIL;
    }

    // サウンドの生成
    m_pSound = new CSound;
    if (FAILED(m_pSound->Init(hWnd)))
    {
        return E_FAIL;
    }

    // ライトの生成
    m_pLight = new CLight;
    if (FAILED(m_pLight->Init()))
    {
        return E_FAIL;
    }

    // カメラの生成
    m_pCamera = new CCamera;
    if (FAILED(m_pCamera->Init()))
    {
        return E_FAIL;
    }

    // テクスチャの生成
    m_pTexture = new CTexture;
    if (FAILED(m_pTexture->Init()))
    {
        return E_FAIL;
    }

    // モデルデータの生成
    m_pModelData = new CModelData;
    if (FAILED(m_pModelData->Init()))
    {
        return E_FAIL;
    }

    // エフェクトデータ管理の生成
    m_pEffectData = new CEffectData;
    if (FAILED(m_pEffectData->Init()))
    {
        return E_FAIL;
    }

    // フォントの読み込み
    CText::Load();

    // フェードの生成
    m_pFade = CFade::Create(MODE_DEBUG_MENU);    // 最初はタイトル

    return S_OK;
}

//========================================
// 生成の管理の終了処理
// Author : 後藤慎之助
//========================================
void CManager::Uninit(void)
{
    // 全てのオブジェクトを破棄
    CScene::ReleaseAll();

    // フォントの破棄
    CText::Unload();

    // モード破棄
    if (m_pMode != NULL)
    {
        // モードの終了処理
        m_pMode->Uninit();

        // モードのメモリの開放
        delete m_pMode;
        m_pMode = NULL;
    }

    // フェード破棄
    if (m_pFade != NULL)
    {
        // フェードの終了処理
        m_pFade->Uninit();

        // フェードのメモリの開放
        delete m_pFade;
        m_pFade = NULL;
    }

    // エフェクトデータ管理破棄
    if (m_pEffectData != NULL)
    {
        // モデルデータの終了処理
        m_pEffectData->Uninit();

        // モデルデータのメモリの開放
        delete m_pEffectData;
        m_pEffectData = NULL;
    }

    // モデルデータ破棄
    if (m_pModelData != NULL)
    {
        // モデルデータの終了処理
        m_pModelData->Uninit();

        // モデルデータのメモリの開放
        delete m_pModelData;
        m_pModelData = NULL;
    }

    // テクスチャ破棄
    if (m_pTexture != NULL)
    {
        // テクスチャの終了処理
        m_pTexture->Uninit();

        // テクスチャのメモリの開放
        delete m_pTexture;
        m_pTexture = NULL;
    }

    // サウンド破棄
    if (m_pSound != NULL)
    {
        // サウンド終了処理
        m_pSound->Uninit();

        // サウンドのメモリの開放
        delete m_pSound;
        m_pSound = NULL;
    }

    // マウスの破棄
    if (m_pMouse != NULL)
    {
        // マウス終了処理
        m_pMouse->Uninit();

        // マウスのメモリの開放
        delete m_pMouse;
        m_pMouse = NULL;
    }

    // コントローラ破棄
    if (m_pInputJoypad != NULL)
    {
        // コントローラ終了処理
        m_pInputJoypad->Uninit();

        // コントローラのメモリの開放
        delete m_pInputJoypad;
        m_pInputJoypad = NULL;
    }

    // キーボード破棄（Initで生成した逆順で破棄する）
    if (m_pInputKeyboard != NULL)
    {
        // キーボード終了処理
        m_pInputKeyboard->Uninit();

        // キーボードのメモリの開放
        delete m_pInputKeyboard;
        m_pInputKeyboard = NULL;
    }

    // レンダラー破棄
    if (m_pRenderer != NULL)
    {
        // レンダラー終了処理
        m_pRenderer->Uninit();

        // レンダラーのメモリの開放
        delete m_pRenderer;
        m_pRenderer = NULL;
    }
}

//========================================
// 生成の管理の更新処理
// Author : 後藤慎之助
//========================================
void CManager::Update(void)
{
    // キーボード更新処理(最初に行う)
    if (m_pInputKeyboard != NULL)
    {
        m_pInputKeyboard->Update();
    }

    // コントローラ更新処理
    if (m_pInputJoypad != NULL)
    {
        m_pInputJoypad->Update();
    }

    // マウス更新処理
    if (m_pMouse != NULL)
    {
        m_pMouse->Update();
    }

    // レンダラー更新処理
    if (m_pRenderer != NULL)
    {
        // モードがあるなら
        if (m_pMode != NULL)
        {
            // ゲーム中なら
            if (typeid(*m_pMode) == typeid(CGame))
            {
                // 更新を止めていないなら
                if (!CGame::GetStopObjUpdate())
                {
                    // 全て更新
                    m_pRenderer->Update();
                }
            }
            else
            {
                // ゲーム以外の場面なら、常に全て更新
                m_pRenderer->Update();
            }
        }
    }

    // モードの更新処理
    if (m_pMode != NULL)
    {
        m_pMode->Update();
    }

#ifdef _DEBUG
    CDebug::CommonCommand();
#endif

    // フェードの更新処理
    if (m_pFade != NULL)
    {
        m_pFade->Update();
    }

    // カメラの更新処理
    if (m_pCamera != NULL)
    {
        // カスタマイズ画面またはゲーム中なら
        if (typeid(*m_pMode) == typeid(CCustom) || typeid(*m_pMode) == typeid(CGame))
        {
            m_pCamera->Update();
        }
    }
}

//========================================
// 生成の管理の描画処理
// Author : 後藤慎之助
//========================================
void CManager::Draw(void)
{
    // 描画処理
    if (m_pRenderer != NULL)
    {
        m_pRenderer->Draw();
    }
}

//========================================
// モードの取得
// Author : 後藤慎之助
//========================================
CManager::MODE CManager::GetMode(void)
{
    // 返り値に使うモード
    MODE mode = MODE_DEBUG_MENU;

    // 何のモードが入っているか見て、型名ごとに場合分け
    if (typeid(*m_pMode) == typeid(CTitle))
    {
        mode = MODE_TITLE;
    }
    else if (typeid(*m_pMode) == typeid(CManual))
    {
        mode = MODE_MANUAL;
    }
    else if (typeid(*m_pMode) == typeid(CCustom))
    {
        mode = MODE_CUSTOM;
    }
    else if (typeid(*m_pMode) == typeid(CGame))
    {
        mode = MODE_GAME;
    }
    else if (typeid(*m_pMode) == typeid(CResult))
    {
        mode = MODE_RESULT;
    }

    return mode;
}

//========================================
// モードの設定
// mode	:	設定するモード
// Author : 後藤慎之助
//========================================
void CManager::SetMode(MODE mode)
{
    // モードの中身があるなら
    if (m_pMode != NULL)
    {
        // 終了処理
        m_pMode->Uninit();

        // メモリの破棄
        delete m_pMode;
        m_pMode = NULL;
    }

    // シーン上のオブジェクトを全て終了処理
    CScene::ReleaseAll();

    // 受け取ったモードに画面遷移
    switch (mode)
    {
    case MODE_DEBUG_MENU:
        m_pMode = new CDebugMenu;
        break;

    case MODE_TITLE:
        m_pMode = new CTitle;
        break;

    case MODE_MANUAL:
        m_pMode = new CManual;
        break;

    case MODE_CUSTOM:
        m_pMode = new CCustom;
        break;

    case MODE_GAME:
        m_pMode = new CGame;
        break;

    case MODE_RESULT:
        m_pMode = new CResult;
        break;
    }

    // 確保したモードの初期化
    m_pMode->Init();
}