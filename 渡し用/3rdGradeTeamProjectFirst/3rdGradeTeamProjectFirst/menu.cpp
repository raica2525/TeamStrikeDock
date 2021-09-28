//=============================================================================
//
// メニュー処理 [menu.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "menu.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "debug.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// メニューのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CMenu::CMenu()
{
    m_nSelectCoolTime = 0;
}

//=============================================================================
// メニューのデストラクタ
// Author : 後藤慎之助
//=============================================================================
CMenu::~CMenu()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CMenu::Init(void)
{
    // UIを生成
    CUI::Place(CUI::SET_MENU);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CMenu::Uninit(void)
{
    // 確保したUIのメモリは、CSceneのReleaseAllで開放されている

    //// BGMを停止
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_TITLE);
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CMenu::Update(void)
{
    ////キーボードの確保したメモリを取得
    //CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    //// コントローラを取得
    //CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    //DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    //// 選択のクールタイムを数える
    //if (m_nSelectCoolTime > 0)
    //{
    //    m_nSelectCoolTime--;
    //}

    //// フェードしていないなら
    //if (CFade::GetFade() == CFade::FADE_NONE)
    //{
    //    switch (m_menu)
    //    {
    //    case MENU_001:

    //        // テクスチャ座標を更新
    //        m_pMenuUI->SetTexturePlace(0, 4);

    //        // 移動キーを押したら(右)
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputKeyboard->GetKeyboardTrigger(DIK_D) || pInputKeyboard->GetKeyboardTrigger(DIK_RIGHT) ||
    //            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START) ||
    //            Controller.lX > 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// 選択
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_002;

    //            // 選択のクールタイムを設定
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        break;

    //    case MENU_002:

    //        // テクスチャ座標を更新
    //        m_pMenuUI->SetTexturePlace(1, 4);

    //        // 移動キーを押したら(左)
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_A) || pInputKeyboard->GetKeyboardTrigger(DIK_LEFT) ||
    //            Controller.lX < 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// 選択
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_001;

    //            // 選択のクールタイムを設定
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }
    //        // 移動キーを押したら(右)
    //        else if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputKeyboard->GetKeyboardTrigger(DIK_D) || pInputKeyboard->GetKeyboardTrigger(DIK_RIGHT) ||
    //            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START) ||
    //            Controller.lX > 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// 選択
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_003;

    //            // 選択のクールタイムを設定
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        break;

    //    case MENU_003:

    //        // テクスチャ座標を更新
    //        m_pMenuUI->SetTexturePlace(2, 4);

    //        // 移動キーを押したら(左)
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_A) || pInputKeyboard->GetKeyboardTrigger(DIK_LEFT) ||
    //            Controller.lX < 0 && m_nSelectCoolTime <= 0)
    //        {
    //            //// 選択
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_SELECT);
    //            m_menu = MENU_002;

    //            // 選択のクールタイムを設定
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        // 決定キーを押したら
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START) ||
    //            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B))
    //        {
    //            //// ゲームモードに移行
    //            //pSound->Play(CSound::LABEL_SE_SYSTEM_MODE_CHANGE);
    //            //CFade::SetFade(CManager::MODE_GAME);
    //        }

    //        break;
    //    }
    //}
}
