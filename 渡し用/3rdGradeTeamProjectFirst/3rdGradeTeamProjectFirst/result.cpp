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
#include "ranking.h"
#include "debug.h"

//========================================
// マクロ定義
//========================================

// 各順位に、カウンタを利用して敵を配置していく
#define ENEMY_5 5
#define ENEMY_4 95
#define ENEMY_3 215
#define ENEMY_2 335
#define ENEMY_1 455
#define STOP 1000       // カウンタのストップ

#define SET_POS_X 1400  // 配置のX座標

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
    // マウスカーソルの表示
    ShowCursor(TRUE);

    // UIを生成
    CUI::Place(CUI::SET_RESULT);

    // ランキングを生成
    CRanking::Create(RANKING_FIRST_POS, DEFAULT_VECTOR);

    //// BGMを再生
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_RESULT);

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

    // 決定キーでタイトルへ
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        // フェードしていないなら
        if (CFade::GetFade() == CFade::FADE_NONE)
        {
            //// 決定音再生
            //CSound *pSound = CManager::GetSound();
            //pSound->Play(CSound::LABEL_SE_SYSTEM_MODE_CHANGE);

            // タイトルモードに移行
            CFade::SetFade(CManager::MODE_TITLE);
        }
    }

#ifdef _DEBUG
    CDebug::ResultCommand();
#endif
}