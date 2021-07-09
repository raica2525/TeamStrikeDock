//===============================================
//
// デバッグ周りの処理 (debug.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "debug.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "game.h"
#include "modelData.h"
#include "player.h"
#include "effectData.h"

#include "wave.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CDebug::CDebug() :CScene3D(CScene::OBJTYPE_MODEL_EFFECT)
{
    m_nLife = 2;
    m_type = TYPE_PERMANENT;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CDebug::~CDebug()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CDebug::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CScene3D::Init(pos, size);

    SetScale(size);

    BindModelData(1);   // 当たり判定可視化は1番

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CDebug::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CDebug::Update(void)
{
    CScene3D::Update();

    // 1Fずつ生成するモデルなら、その都度消す(あえて少し負荷をかけ、それ以上ゲームが重くならないように作る)
    if (m_type == TYPE_MOMENT)
    {
        m_nLife--;
    }

    if (m_nLife <= 0)
    {
        Uninit();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CDebug::Draw(void)
{
    CScene3D::Draw();
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CDebug * CDebug::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
    CDebug *pDebug = NULL;
    pDebug = new CDebug;
    pDebug->m_type = type;
    pDebug->Init(pos, size);

    return pDebug;
}

//=============================================================================
// UIのリロード
// Author : 後藤慎之助
//=============================================================================
#ifdef _DEBUG
void CDebug::ReloadUI(CUI::SET set)
{
    // UIを削除
    CScene::ReleaseUI();

    // UIの再配置
    CUI::Place(set);
}

//=============================================================================
// 共通のデバッグコマンド
// Author : 後藤慎之助
//=============================================================================
void CDebug::CommonCommand(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F8キーが押されたら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F8))
    {
        CRenderer::SwitchDispFont();
    }
}

//=============================================================================
// タイトル中のデバッグコマンド
// Author : 後藤慎之助
//=============================================================================
void CDebug::TitleCommand(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F4キーが押されたら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UIのリロード
        ReloadUI(CUI::SET_TITLE);
    }

    // 共通のデバッグコマンド
    CommonCommand();
}

//=============================================================================
// 遊び方中のデバッグコマンド
// Author : 後藤慎之助
//=============================================================================
void CDebug::ManualCommand(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F4キーが押されたら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UIのリロード
        ReloadUI(CUI::SET_MANUAL);
    }

    // 共通のデバッグコマンド
    CommonCommand();
}

//=============================================================================
// ゲーム中のデバッグコマンド
// Author : 後藤慎之助
//=============================================================================
void CDebug::GameCommand(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    //// エンターキーが押されたら
    //if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
    //{
    //    // フェードしていないなら
    //    if (CFade::GetFade() == CFade::FADE_NONE)
    //    {
    //        // タイトルに移行
    //        CFade::SetFade(CManager::MODE_TITLE);
    //    }
    //}
    //else if (pInputKeyboard->GetKeyboardTrigger(DIK_O))
    //{
    //    CWave::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 0.0f));
    //}

    // F4キーが押されたら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UIのリロード
        ReloadUI(CUI::SET_GAME);

        // プレイヤーのパーツ設定をリロード
        CModelData* pModelData = CManager::GetModelData();
        pModelData->LoadPartsList();
        pModelData->LoadPartsRate();
        for (int nCnt = 0; nCnt < CGame::GetNumAllPlayer(); nCnt++)
        {
            CPlayer* pPlayer = CGame::GetPlayer(nCnt);
            pPlayer->LoadCustom();
        }

        // エフェクトデータのリロード
        CEffectData* pEffectData = CManager::GetEffectData();
        pEffectData->Init();
    }

    // 共通のデバッグコマンド
    CommonCommand();
}

//=============================================================================
// リザルト中のデバッグコマンド
// Author : 後藤慎之助
//=============================================================================
void CDebug::ResultCommand(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // F4キーが押されたら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_F4))
    {
        // UIのリロード
        ReloadUI(CUI::SET_RESULT);
    }

    // 共通のデバッグコマンド
    CommonCommand();
}
#endif