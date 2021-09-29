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
    m_bEmitWingEffect = true;
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
    // 炎エフェクトを上下に発生(6回分でちょうどよくなった)
    for (int nCnt = 0; nCnt < 6; nCnt++)
    {
        EmitFire(FIRE_POS_UP);
        EmitFire(FIRE_POS_DOWN);
    }

    // タイトルの時間をカウント
    m_nCntTime++;

    // 一定時間経過で遷移可能に
    if (m_nCntTime >= TITLE_SHOW_TIME)
    {
        // カウンタストップ
        m_nCntTime = TITLE_SHOW_TIME;

        // 翼エフェクトを放出
        if (m_bEmitWingEffect)
        {
            m_bEmitWingEffect = false;
            CEffect2D::Emit(CEffectData::TYPE_RED_WING, D3DXVECTOR3(640.0f, 160.0f,0.0f), D3DXVECTOR3(640.0f, 360.0f, 0.0f));
            CEffect2D::Emit(CEffectData::TYPE_BLUE_WING, D3DXVECTOR3(640.0f, 160.0f, 0.0f), D3DXVECTOR3(640.0f, 360.0f, 0.0f));
        }

        // プレスボタンのUIを出現させる
        CUI *pPressButton = CUI::GetAccessUI(0);
        if (pPressButton)
        {
            pPressButton->SetActionLock(1, false);
        }

        // フェードしていないなら、遷移可能
        if (CFade::GetFade() == CFade::FADE_NONE)
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
                // 仮にカスタマイズ画面に移行
                CFade::SetFade(CManager::MODE_CUSTOM);

                // プレスボタンを点滅させる
                if (pPressButton)
                {
                    // 色変えをリセットしてロックする
                    pPressButton->SetActionReset(0);
                    pPressButton->SetActionLock(0, true);

                    // 点滅をアンロック
                    pPressButton->SetActionLock(2, false);
                }
            }
        }
    }
}

//=============================================================================
// 画面の上か下に炎を出す処理
// Author : 後藤慎之助
//=============================================================================
void CTitle::EmitFire(FIRE_POS firePos)
{
    // 変数宣言
    const float ANGLE_ADJUST = 90.0f;   // 時計でいう0が0度の時に合わせる（2Dポリゴンの頂点をZで指定しているから）
    const float POS_Y_ADJUST = 50.0f;
    const int RAND_X = 1380;
    const float BASE_ANGLE = 67.5f;
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    float fAngle = 0.0f;
    CEffectData::TYPE effectType = CEffectData::TYPE_TITLE_FIRE_RED;

    // 位置と向きと色を決める
    switch (firePos)
    {
    case FIRE_POS_UP:
        pos.y = -POS_Y_ADJUST;
        pos.x = float(rand() % RAND_X);
        fAngle = D3DXToRadian(180.0f) + BASE_ANGLE - ANGLE_ADJUST;
        effectType = CEffectData::TYPE_TITLE_FIRE_BLUE;
        break;
    case FIRE_POS_DOWN:
        pos.y = SCREEN_HEIGHT + POS_Y_ADJUST;
        pos.x = float(rand() % RAND_X) - float(RAND_X - SCREEN_WIDTH);
        fAngle = BASE_ANGLE - ANGLE_ADJUST;
        effectType = CEffectData::TYPE_TITLE_FIRE_RED;
        break;
    }

    // 炎生成
    CEffect2D::Create(effectType, pos, fAngle);
}
