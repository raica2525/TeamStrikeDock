//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "game.h"
#include "debug.h"
#include "camera.h"

//========================================
// マクロ定義
//========================================

//=============================================================================
// リザルトのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// リザルトのデストラクタ
// Author : 後藤慎之助
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CResult::Init(void)
{
    // UIを生成
    CUI::Place(CUI::SET_RESULT);

    //// BGMを再生
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_RESULT);

    // プレイヤー生成
    for (int nCntPlayer = 0; nCntPlayer < CGame::GetNumAllPlayer(); nCntPlayer++)
    {
        switch (nCntPlayer)
        {
        case CPlayer::RANK_1:
            CPlayer::CreateInResult(D3DXVECTOR3(250.0f, 200.0f, -1200.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_1);
            break;
        case CPlayer::RANK_2:
            CPlayer::CreateInResult(D3DXVECTOR3(-640.0f, 100.0f, -600.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_2);
            break;
        case CPlayer::RANK_3:
            CPlayer::CreateInResult(D3DXVECTOR3(-325.0f, 25.0f, -310.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_3);
            break;
        case CPlayer::RANK_4:
            CPlayer::CreateInResult(D3DXVECTOR3(125.0f, -40.0f, -100.0f), DEFAULT_VECTOR, CGame::GetPlayerRank(nCntPlayer), CPlayer::RANK_4);
            break;
        }
    }

    // カメラのロックオン場所を変える
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CResult::Uninit(void)
{
    //// BGMを停止
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_RESULT);
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CResult::Update(void)
{
    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // エンター、または何かボタンを押したら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_A)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_X)
        || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_Y))
    {
        // 仮にタイトル画面に移行
        CFade::SetFade(CManager::MODE_TITLE);
    }
}