//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "game.h"
#include "debug.h"
#include "effect2d.h"

//=============================================================================
// タイトルのコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CTitle::CTitle()
{
    m_nCntTime = 0;
}

//=============================================================================
// タイトルのデストラクタ
// Author : 後藤慎之助
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CTitle::Init(void)
{
    // マウスカーソルの表示(念のため)
    ShowCursor(TRUE);

    // UIを生成
    CUI::Place(CUI::SET_TITLE);

    //// BGMを再生
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_TITLE);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CTitle::Uninit(void)
{
    //// BGMを停止(仮)
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_TITLE);
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CTitle::Update(void)
{
    // 炎エフェクトを上下に発生
    EmitFire(FIRE_POS_UP);
    EmitFire(FIRE_POS_DOWN);

    // タイトルの時間をカウント
    m_nCntTime++;

    // タイトルの最大時間
    if (m_nCntTime > TITLE_MAX_TIME)
    {
        m_nCntTime = TITLE_MAX_TIME;
    }

    // 一定時間経過で遷移可能に
    if (m_nCntTime >= TITLE_SHOW_TIME)
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
            // 仮にゲームモードに移行
            CFade::SetFade(CManager::MODE_GAME);
        }
    }
}

//=============================================================================
// 画面の上か下に炎を出す処理
// Author : 後藤慎之助
//=============================================================================
void CTitle::EmitFire(FIRE_POS firePos)
{
    // 炎エフェクト生成
    const float ANGLE_ADJUST = 90.0f;
    CEffect2D::Create(CEffectData::TYPE_TITLE_FIRE_RED, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXToRadian(247.5f));
    CEffect2D::Create(CEffectData::TYPE_TITLE_FIRE_RED, D3DXVECTOR3(SCREEN_WIDTH, -50.0f, 0.0f), D3DXToRadian(247.5f));
    CEffect2D::Create(CEffectData::TYPE_TITLE_FIRE_BLUE, D3DXVECTOR3(0.0f, SCREEN_HEIGHT + 50.0f, 0.0f), D3DXToRadian(67.5f));
}
