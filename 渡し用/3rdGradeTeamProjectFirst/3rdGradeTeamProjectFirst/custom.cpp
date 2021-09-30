//=============================================================================
//
// カスタマイズ画面処理 [custom.cpp]
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "custom.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "debug.h"
#include "camera.h"
#include "modelData.h"
#include "text.h"
#include "player.h"
#include "game.h"
#include "effect2d.h"

//========================================
// マクロ定義
//========================================
#define CURSOR_FIRST_POS_P1 D3DXVECTOR3(167.0f, 390.0f, 0.0f)        // カーソルの初期位置Player1
#define CURSOR_FIRST_POS_P2 D3DXVECTOR3(482.0f, 390.0f, 0.0f)        // カーソルの初期位置Player2
#define CURSOR_FIRST_POS_P3 D3DXVECTOR3(797.0f, 390.0f, 0.0f)        // カーソルの初期位置Player3
#define CURSOR_FIRST_POS_P4 D3DXVECTOR3(1112.0f, 390.0f, 0.0f)       // カーソルの初期位置Player4
#define CURSOR_ADJUST_COLLISON_POS D3DXVECTOR3(-20.0f, -20.0f, 0.0f) // カーソルの当たり判定の位置調整
#define CURSOR_COLLISION_SIZE D3DXVECTOR3(5.0f, 5.0f, 0.0f)          // カーソルの当たり判定の大きさ
#define CURSOR_VISUAL_SIZE_SIDE 59.0f                                // カーソルの一辺の大きさ
#define CURSOR_VISUAL_SIZE D3DXVECTOR3(CURSOR_VISUAL_SIZE_SIDE, CURSOR_VISUAL_SIZE_SIDE, 0.0f)  // カーソルの見た目の大きさ
#define CURSOR_PRESS_VISUAL_SIZE D3DXVECTOR3(45.0f, 45.0f, 0.0f)     // プレスカーソルの見た目の大きさ

#define CURSOR_CLICK_ACTION_INFO_IDX 3                               // カーソルのクリックアクション情報が入ったインデックス

//=============================================================================
// カスタマイズ画面のコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CCustom::CCustom()
{
    memset(m_anMemoryPartsHead, NOT_EXIST, sizeof(m_anMemoryPartsHead));
    memset(m_anMemoryPartsUp, NOT_EXIST, sizeof(m_anMemoryPartsUp));
    memset(m_anMemoryPartsDown, NOT_EXIST, sizeof(m_anMemoryPartsDown));
    memset(m_anMemoryPartsWep, NOT_EXIST, sizeof(m_anMemoryPartsWep));
    memset(m_aEntryInfo, 0, sizeof(m_aEntryInfo));

    m_bClickReadyToFight = false;
    m_bDispReadyToFight = false;
}

//=============================================================================
// カスタマイズ画面のデストラクタ
// Author : 後藤慎之助
//=============================================================================
CCustom::~CCustom()
{

}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CCustom::Init(void)
{
    // 持っているパーツ情報を結びつける
    BindHaveParts();

    // UIを外部ファイルから生成
    CUI::Place(CUI::SET_CUSTOM);

    // プレイヤー(マネキン)の生成
    m_aEntryInfo[PLAYER_1].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(-950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_1, false);
    m_aEntryInfo[PLAYER_2].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(-316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_2, false);
    m_aEntryInfo[PLAYER_3].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_3, false);
    m_aEntryInfo[PLAYER_4].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, PLAYER_4, false);

    // カーソル生成
    m_aEntryInfo[PLAYER_1].pUI_Cursor = CUI::Create(53, CURSOR_FIRST_POS_P1, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);
    m_aEntryInfo[PLAYER_2].pUI_Cursor = CUI::Create(54, CURSOR_FIRST_POS_P2, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);
    m_aEntryInfo[PLAYER_3].pUI_Cursor = CUI::Create(55, CURSOR_FIRST_POS_P3, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);
    m_aEntryInfo[PLAYER_4].pUI_Cursor = CUI::Create(56, CURSOR_FIRST_POS_P4, CURSOR_VISUAL_SIZE, 0, DEFAULT_COLOR, true);

    // パーツ選択情報の初期化
    float fTextPosX = 167.0f;
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        int nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_HEAD);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsHead[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectHead = nCntEachParts;
            }
        }

        nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_UP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsUp[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectUp = nCntEachParts;
            }
        }

        nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_DOWN);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsDown[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectDown = nCntEachParts;
            }
        }

        nPartNum = m_aEntryInfo[nCnt].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_WEP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anMemoryPartsWep[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectWep = nCntEachParts;
            }
        }

        // テキストを設定
        D3DCOLOR color = TEXT_NOT_EXIST_COLOR;
        m_aEntryInfo[nCnt].pText_Head = CText::Create(D3DXVECTOR3(fTextPosX, 427.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsHead[m_aEntryInfo[nCnt].nNumSelectHead]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Up = CText::Create(D3DXVECTOR3(fTextPosX, 490.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsUp[m_aEntryInfo[nCnt].nNumSelectUp]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Down = CText::Create(D3DXVECTOR3(fTextPosX, 553.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsDown[m_aEntryInfo[nCnt].nNumSelectDown]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Wep = CText::Create(D3DXVECTOR3(fTextPosX, 616.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anMemoryPartsWep[m_aEntryInfo[nCnt].nNumSelectWep]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);

        // テキストの横位置を加算
        fTextPosX += 315.0f;

        // 1F前の選択肢情報を初期化
        m_aEntryInfo[nCnt].nNumSelectUIOld = NOT_EXIST;

        // 各UIのアクセス権を結びつける
        m_aEntryInfo[nCnt].pUI_Bg_Select = CUI::GetAccessUI(100 + (nCnt * 3));              // エントリー済み100
        m_aEntryInfo[nCnt].pUI_Bg_Ready = CUI::GetAccessUI(101 + (nCnt * 3));               // 準備完了101
        m_aEntryInfo[nCnt].pUI_Bg_Wait = CUI::GetAccessUI(102 + (nCnt * 3));                // 未エントリー102
        m_aEntryInfo[nCnt].pUI_Bg_Select_Out_Frame = CUI::GetAccessUI(112 + (nCnt * 2));    // カスタム外枠112
        m_aEntryInfo[nCnt].pUI_Bg_Select_In_Frame = CUI::GetAccessUI(113 + (nCnt * 2));     // カスタム内枠113
        m_aEntryInfo[nCnt].pUI_Bg_Select_Status = CUI::GetAccessUI(120 + nCnt);             // カスタム内枠120

        // カーソルを消しておく
        m_aEntryInfo[nCnt].pUI_Cursor->SetDisp(false);
    }

    // カメラのロックオン場所を変える
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    // ストック数表示を更新
    CUI *pStock = CUI::GetAccessUI(SELECT_STOCK);
    if (pStock)
    {
        pStock->SetTexturePlace(CGame::GetStock() - 1, PLAYER_MAX_STOCK);
    }

    // BGMを再生
    CManager::SoundPlay(CSound::LABEL_BGM_MENU);

    return S_OK;
}

//=============================================================================
// 持っているパーツ情報を結びつける
// Author : 後藤慎之助
//=============================================================================
void CCustom::BindHaveParts(void)
{
    // パーツリストから取得
    for (int nCntParts = 0; nCntParts < MAX_PARTS_LIST; nCntParts++)
    {
        // どこの部位か取得
        int nNumPart = CManager::GetModelData()->CModelData::GetPartsList(nCntParts)->part;

        // 存在しないなら、次へ
        if (nNumPart == NOT_EXIST)
        {
            continue;
        }

        // 空きにパーツ番号を結びつけ、結びつけたらforを抜ける
        switch (nNumPart)
        {
        case CPlayer::CUSTOM_PARTS_HEAD:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsHead[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsHead[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_UP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsUp[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsUp[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_DOWN:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsDown[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsDown[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_WEP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anMemoryPartsWep[nCntEachParts] == NOT_EXIST)
                {
                    m_anMemoryPartsWep[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        }
    }
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CCustom::Uninit(void)
{
    // BGMを停止
    CManager::SoundStop(CSound::LABEL_BGM_MENU);
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CCustom::Update(void)
{
    // ReadyToFightが押されたかどうかチェックするフラグを一度初期化
    m_bClickReadyToFight = false;

    // カーソル移動
    MoveCursor();

    // 準備完了できているかの判断
    JudgmentReadyToFight();
}

//=============================================================================
// キーボード操作
// Author : 後藤慎之助
//=============================================================================
void CCustom::MoveCursorByKeyboard(float& fStickAngle, bool& bTiltedStick, float& fTiltedStickValue)
{
    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
    // 左スティックが傾いているかどうか
    if (pInputKeyboard->GetKeyboardPress(DIK_W) ||
        pInputKeyboard->GetKeyboardPress(DIK_A) ||
        pInputKeyboard->GetKeyboardPress(DIK_S) ||
        pInputKeyboard->GetKeyboardPress(DIK_D))
    {
        bTiltedStick = true;
        fTiltedStickValue = 9000.0f;    // コントローラ時の最大速度より、若干遅くしている

        // 角度を求める
        if (pInputKeyboard->GetKeyboardPress(DIK_A))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                fStickAngle = D3DXToRadian(-45.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                fStickAngle = D3DXToRadian(-135.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(-90.0f);
            }
        }
        else if (pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                fStickAngle = D3DXToRadian(45.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                fStickAngle = D3DXToRadian(135.0f);
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(90.0f);
            }
        }
        else if (pInputKeyboard->GetKeyboardPress(DIK_W))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
        }
        else if (pInputKeyboard->GetKeyboardPress(DIK_S))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                fStickAngle = D3DXToRadian(0.0f);
            }
            else
            {
                fStickAngle = D3DXToRadian(180.0f);
            }
        }
    }
}

//=============================================================================
// カーソル移動
// Author : 後藤慎之助
//=============================================================================
void CCustom::MoveCursor(void)
{
    for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
    {
        // コントローラを取得
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
        DIJOYSTATE2 Controller = pInputJoypad->GetController(nCntPlayer);
        float fStickAngle = 0.0f;
        bool bTiltedStick = false;
        float fTiltedStickValue = 0.0f;

        // 左スティックが傾いているかどうか
        if (Controller.lY != 0 || Controller.lX != 0)
        {
            bTiltedStick = true;

            // 角度を求める
            fStickAngle = atan2(Controller.lX, Controller.lY*-1);

            // 大きさを求める
            fTiltedStickValue = sqrtf(
                powf(float(Controller.lX), 2) +
                powf((float(Controller.lY)*-1), 2));

            // 最大傾きより大きいなら、制限（正方形の対角線は、各辺よりも長いため）
            if (fTiltedStickValue > STICK_MAX_TILT)
            {
                fTiltedStickValue = STICK_MAX_TILT;
            }
        }
        else
        {
            // プレイヤー1ならキーボード操作可能
            if (nCntPlayer == PLAYER_1)
            {
                MoveCursorByKeyboard(fStickAngle, bTiltedStick, fTiltedStickValue);
            }
        }

        // カーソルを使うなら
        if (m_aEntryInfo[nCntPlayer].bUseCursor)
        {
            // カーソルがあるなら
            if (m_aEntryInfo[nCntPlayer].pUI_Cursor)
            {
                // 位置を取得
                D3DXVECTOR3 cursorPos = m_aEntryInfo[nCntPlayer].pUI_Cursor->GetPosition();

                // 移動
                if (bTiltedStick)
                {
                    const float ADJUST_RATE = 0.0008f;   // スティックの傾きの値を、位置に足せるよう調整
                    cursorPos.x += sinf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                    cursorPos.y += -cosf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                }

                // カーソル移動の制限
                RimitMoveCursor(cursorPos);

                // 位置を設定
                m_aEntryInfo[nCntPlayer].pUI_Cursor->SetPosition(cursorPos);

                // プレスファイア
                PressFire(nCntPlayer);

                // 選択肢との当たり判定（カーソルの当たり判定の位置は左上に微調整）
                CollisionSelect(nCntPlayer, cursorPos + CURSOR_ADJUST_COLLISON_POS);
            }
        }
        else
        {
            // カーソルを使っていないなら、スティックが傾き次第エントリー
            if (bTiltedStick)
            {
                m_aEntryInfo[nCntPlayer].bUseCursor = true;

                // エントリー状態のチェンジ
                ChangeEntryStatus(nCntPlayer, ENTRY_STATUS_PLAYER);

                // 準備完了中なら、準備完了を解く
                if (m_aEntryInfo[nCntPlayer].bReady)
                {
                    ToggleReady(nCntPlayer);
                }
            }
        }
    }
}

//=============================================================================
// カーソル移動の制限
// Author : 後藤慎之助
//=============================================================================
void CCustom::RimitMoveCursor(D3DXVECTOR3& cursorPos)
{
    // 横
    if (cursorPos.x - (CURSOR_VISUAL_SIZE_SIDE / 2) < 0.0f)
    {
        cursorPos.x = (CURSOR_VISUAL_SIZE_SIDE / 2);
    }
    else if (cursorPos.x + (CURSOR_VISUAL_SIZE_SIDE / 2) > SCREEN_WIDTH)
    {
        cursorPos.x = SCREEN_WIDTH - (CURSOR_VISUAL_SIZE_SIDE / 2);
    }

    // 縦
    if (cursorPos.y + (CURSOR_VISUAL_SIZE_SIDE / 2) > SCREEN_HEIGHT)
    {
        cursorPos.y = SCREEN_HEIGHT - (CURSOR_VISUAL_SIZE_SIDE / 2);
    }
    else if (cursorPos.y - (CURSOR_VISUAL_SIZE_SIDE / 2) < 0.0f)
    {
        cursorPos.y = (CURSOR_VISUAL_SIZE_SIDE / 2);
    }
}

//=============================================================================
// プレスし続けると炎が出る
// Author : 後藤慎之助
//=============================================================================
void CCustom::PressFire(int nNumWho)
{
    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

    // Aボタンがプレスで押されたら
    bool bPressA = false;
    if (pInputJoypad->GetJoypadPress(nNumWho, CInputJoypad::BUTTON_A) ||
        pInputKeyboard->GetKeyboardPress(DIK_RETURN) && nNumWho == PLAYER_1)
    {
        bPressA = true;
    }

    // カーソルのサイズを、押している間は少し小さくする
    if (bPressA)
    {
        m_aEntryInfo[nNumWho].pUI_Cursor->SetSize(CURSOR_PRESS_VISUAL_SIZE);
    }
    else
    {
        m_aEntryInfo[nNumWho].pUI_Cursor->SetSize(CURSOR_VISUAL_SIZE);
    }
}

//=============================================================================
// 選択肢との当たり判定
// Author : 後藤慎之助
//=============================================================================
void CCustom::CollisionSelect(int nNumWho, D3DXVECTOR3 cursorPos)
{
    // 今回当たっている選択肢
    int nNumSelectUICurrent = NOT_EXIST;
    for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
    {
        // 選択できるUIを取得
        CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);

        // 中身があるなら
        if (pSelectUI)
        {
            // 誰のUIかと、種類を取得
            int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
            int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);

            // 特定の人のUIなら
            if (nParamWho >= PLAYER_1 && nParamWho <= PLAYER_4)
            {
                // その人の待機中は、チェンジボタン以外押せない
                if (m_aEntryInfo[nParamWho].status == ENTRY_STATUS_WAITING && nParamType != CLICK_TYPE_CHANGE)
                {
                    continue;
                }

                // その人の準備完了中は、準備完了ボタンと詳細ボタン以外押せない
                if (m_aEntryInfo[nParamWho].bReady && nParamType != CLICK_TYPE_READY && nParamType != CLICK_TYPE_DETAIL)
                {
                    continue;
                }

                // カーソルが表示されていているかつ、エントリー状態がプレイヤーなら
                if (m_aEntryInfo[nParamWho].bUseCursor && m_aEntryInfo[nParamWho].status == ENTRY_STATUS_PLAYER)
                {
                    // 本人しか選択肢を変えられない
                    if (nNumWho != nParamWho)
                    {
                        continue;
                    }
                }

                // 詳細ボタンは、ReadyToFightがあるとき押せない
                if(m_bDispReadyToFight && nParamType == CLICK_TYPE_DETAIL)
                {
                    continue;
                }
            }
            else
            {
                // ReadyToFight は、表示されていないと押せない
                if (nParamType == CLICK_TYPE_READY_TO_FIGHT && !m_bDispReadyToFight)
                {
                    continue;
                }

                // ストック は、表示されていないと押せない
                if (nParamType == CLICK_TYPE_STOCK && !m_bDispReadyToFight)
                {
                    continue;
                }
            }

            // 当たっているなら
            if (IsCollisionRectangle2D(&cursorPos, &pSelectUI->GetCollisionPos(),
                &CURSOR_COLLISION_SIZE, &pSelectUI->GetCollisionSize()))
            {
                // SE
                if (m_aEntryInfo[nNumWho].nNumSelectUIOld == NOT_EXIST)
                {
                    CManager::SoundPlay(CSound::LABEL_SE_CURSOLON);
                }

                // 拡大縮小を解除
                pSelectUI->SetActionLock(0, false);

                // クリック処理
                ClickSelect(nNumWho, pSelectUI, cursorPos);

                // 今回当たっている選択肢
                nNumSelectUICurrent = nCntSelect;

                // 種類によっては、スライドイン処理
                if (nParamType == CLICK_TYPE_RETURN)
                {
                    pSelectUI->SetActionLock(1, false);
                    pSelectUI->SetActionLock(2, true);
                }

                // forを抜ける
                break;
            }
        }
    }

    // 1F前の選択肢を今回選択できていないなら、サイズをリセットし拡縮ロック
    if (nNumSelectUICurrent != m_aEntryInfo[nNumWho].nNumSelectUIOld)
    {
        CUI *pResetUI = CUI::GetAccessUI(m_aEntryInfo[nNumWho].nNumSelectUIOld);
        if (pResetUI)
        {
            pResetUI->SetActionReset(0);
            pResetUI->SetActionLock(0, true);

            // 種類によっては、スライドイン処理
            int nParamType = (int)pResetUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
            if (nParamType == CLICK_TYPE_RETURN)
            {
                pResetUI->SetActionLock(1, true);
                pResetUI->SetActionLock(2, false);
            }
        }
    }

    // 1F前の選択肢を記憶
    m_aEntryInfo[nNumWho].nNumSelectUIOld = nNumSelectUICurrent;
}

//=============================================================================
// 選択肢のクリック
// Author : 後藤慎之助
//=============================================================================
void CCustom::ClickSelect(int nNumWho, CUI* pSelectUI, D3DXVECTOR3 cursorPos)
{
    // 選んだUIが存在しないなら関数を抜ける
    if (!pSelectUI)
    {
        return;
    }

    // フェードしていないなら、選択可能
    if (CFade::GetFade() == CFade::FADE_NONE)
    {
        //キーボードの確保したメモリを取得
        CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

        // コントローラを取得
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

        // Aボタンがトリガーで押されたら
        bool bTriggerA = false;
        bool bTriggerReturn = false;
        if (pInputJoypad->GetJoypadTrigger(nNumWho, CInputJoypad::BUTTON_A) ||
            pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) && nNumWho == PLAYER_1)
        {
            bTriggerA = true;

            // P1がキーボードをゲームでも使うかどうか
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) && nNumWho == PLAYER_1)
            {
                bTriggerReturn = true;
            }
        }

        if (bTriggerA)
        {
            // クリック時のキラキラ
            CEffect2D::Emit(CEffectData::TYPE_KIRA, cursorPos, cursorPos);

            // 種類によって反応を変える
            switch ((int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE))
            {
            case CLICK_TYPE_PARTS:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);
                    // 誰がどこのパーツの左右どちらを選んだか
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    int nParamWhere = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHERE);
                    bool bRight = false;
                    if ((int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_RIGHT) == 1)
                    {
                        bRight = true;
                    }
                    SelectParts(nParamWho, nParamWhere, bRight);
                }
                break;

            case CLICK_TYPE_CHANGE:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_SELECT);
                    // 今の状態に応じて、次のエントリー状態にチェンジ
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    ENTRY_STATUS nextEntryStatus = ENTRY_STATUS_WAITING;
                    switch (m_aEntryInfo[nParamWho].status)
                    {
                    case ENTRY_STATUS_WAITING:
                        if (m_aEntryInfo[nParamWho].bUseCursor)
                        {
                            nextEntryStatus = ENTRY_STATUS_PLAYER;
                        }
                        else
                        {
                            nextEntryStatus = ENTRY_STATUS_CP_LEVEL_1;
                        }
                        break;
                    case ENTRY_STATUS_PLAYER:
                        nextEntryStatus = ENTRY_STATUS_CP_LEVEL_1;
                        break;
                    case ENTRY_STATUS_CP_LEVEL_1:
                        nextEntryStatus = ENTRY_STATUS_CP_LEVEL_2;
                        break;
                    case ENTRY_STATUS_CP_LEVEL_2:
                        nextEntryStatus = ENTRY_STATUS_CP_LEVEL_3;
                        break;
                    case ENTRY_STATUS_CP_LEVEL_3:
                        // もしカーソルが表示されていてかつ、他の誰かによって押されたなら、CPよわいへ
                        // 動かしている最中に、他の誰かによって消されるのを防ぐ
                        if (m_aEntryInfo[nParamWho].bUseCursor && nNumWho != nParamWho)
                        {
                            nextEntryStatus = ENTRY_STATUS_CP_LEVEL_1;
                        }
                        else
                        {
                            nextEntryStatus = ENTRY_STATUS_WAITING;
                        }
                        break;
                    }
                    ChangeEntryStatus(nParamWho, nextEntryStatus);
                }
                break;

            case CLICK_TYPE_READY:
                {
                    // 1Pが押した場合、キーボード操作をするように登録
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    ToggleReady(nParamWho);
                    if (nNumWho == PLAYER_1)
                    {
                        CGame::SetUseKeyboard(bTriggerReturn);
                    }
                }
                break;

            case CLICK_TYPE_DETAIL:
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    m_aEntryInfo[nParamWho].pPlayer->SetDispAbility(!m_aEntryInfo[nParamWho].pPlayer->GetDispAbility());
                    if (m_aEntryInfo[nParamWho].pPlayer->GetDispAbility())
                    {
                        CManager::SoundPlay(CSound::LABEL_SE_INFO);
                    }
                    else
                    {
                        CManager::SoundPlay(CSound::LABEL_SE_CANCEL);
                    }
                }
                break;

            case CLICK_TYPE_READY_TO_FIGHT:
                m_bClickReadyToFight = true;
                break;

            case CLICK_TYPE_RETURN:
                // ある程度画面に表示されたら
                if (pSelectUI->GetPosition().x > 40.0f)
                {
                    CManager::SoundPlay(CSound::LABEL_SE_CANCEL);
                    CFade::SetFade(CManager::MODE_TITLE);   // 仮にタイトル
                }
                break;

            case CLICK_TYPE_STOCK:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_SELECT);
                    // ストックを取得し、加算し、反映
                    int nStock = CGame::GetStock() + 1;
                    if (nStock > PLAYER_MAX_STOCK)
                    {
                        nStock = 1;
                    }
                    pSelectUI->SetTexturePlace(nStock - 1, PLAYER_MAX_STOCK);
                    CGame::SetStock(nStock);
                }
                break;

            case CLICK_TYPE_OSUSUME:
                {
                    CManager::SoundPlay(CSound::LABEL_SE_SELECT);
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    DoOsusume(nParamWho);
                }
                break;
            }
        }
    }
}

//=============================================================================
// エントリー状態のチェンジ
// Author : 後藤慎之助
//=============================================================================
void CCustom::ChangeEntryStatus(int nNumWho, ENTRY_STATUS nextEntryStatus)
{
    // 次のエントリー状態に合わせて、表示物を変える
    switch (nextEntryStatus)
    {
    case ENTRY_STATUS_WAITING:
        {
            // 他の全員がカーソルを使っていないなら
            bool bAllOtherCursorNone = true;
            for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
            {
                if (nNumWho == nCntPlayer)
                {
                    continue;
                }

                if (m_aEntryInfo[nCntPlayer].bUseCursor)
                {
                    bAllOtherCursorNone = false;
                    break;
                }
            }

            // 誰か1人でもカーソルを使っているなら
            if (!bAllOtherCursorNone)
            {
                // カーソルを封印
                m_aEntryInfo[nNumWho].bUseCursor = false;
                m_aEntryInfo[nNumWho].nNumSelectUIOld = NOT_EXIST;
                m_aEntryInfo[nNumWho].bReady = false;
                m_aEntryInfo[nNumWho].pUI_Cursor->SetFirstPos();
                m_aEntryInfo[nNumWho].pUI_Cursor->SetDisp(false);
            }

            // 非表示
            m_aEntryInfo[nNumWho].pPlayer->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_In_Frame->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Status->SetDisp(false);
            m_aEntryInfo[nNumWho].pText_Head->SetColor(TEXT_NOT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Up->SetColor(TEXT_NOT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Down->SetColor(TEXT_NOT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Wep->SetColor(TEXT_NOT_EXIST_COLOR);
            for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
            {
                CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
                if (pSelectUI)
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    if (nParamWho == nNumWho)
                    {
                        int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                        if (nParamType != CLICK_TYPE_CHANGE)
                        {
                            // 選択肢を見えないように
                            pSelectUI->SetDisp(false);
                        }
                        else
                        {
                            // チェンジはプレイヤー表示にし、サイズリセットからの拡縮ロック
                            pSelectUI->SetTexturePlace(0, 4);
                            pSelectUI->SetActionReset(0);
                            pSelectUI->SetActionLock(0, true);
                        }
                    }
                }
            }
            // 表示
            m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetDisp(true);
        }
        break;

    case ENTRY_STATUS_PLAYER:
    case ENTRY_STATUS_CP_LEVEL_1:
        // もし、待機中からの遷移なら
        if (m_aEntryInfo[nNumWho].status == ENTRY_STATUS_WAITING)
        {
            // 非表示
            m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetDisp(false);
            m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(false);
            // 表示
            m_aEntryInfo[nNumWho].pPlayer->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_In_Frame->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Status->SetDisp(true);
            m_aEntryInfo[nNumWho].pText_Head->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Up->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Down->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Wep->SetColor(TEXT_EXIST_COLOR);
            // 武器を見る
            m_aEntryInfo[nNumWho].pPlayer->SetCustomWeaponLook();
        }

        if (nextEntryStatus == ENTRY_STATUS_PLAYER)
        {
            // プレイヤーへの遷移なら、カーソルを表示
            if (!m_aEntryInfo[nNumWho].pUI_Cursor->GetDisp())
            {
                m_aEntryInfo[nNumWho].pUI_Cursor->SetDisp(true);
                CManager::SoundPlay(CSound::LABEL_SE_ENTRY);
            }
        }

        // 選択肢は遷移元に関わらず、変わる表示がある
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                if (nParamWho == nNumWho)
                {
                    int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                    if (nParamType != CLICK_TYPE_CHANGE)
                    {
                        // もし、待機中からの遷移なら
                        if (m_aEntryInfo[nNumWho].status == ENTRY_STATUS_WAITING)
                        {
                            // 選択肢を見えるように
                            pSelectUI->SetDisp(true);
                        }
                    }
                    else
                    {
                        // チェンジ表示切替
                        if (nextEntryStatus == ENTRY_STATUS_PLAYER)
                        {
                            pSelectUI->SetTexturePlace(0, 4);
                        }
                        else if (nextEntryStatus == ENTRY_STATUS_CP_LEVEL_1)
                        {
                            pSelectUI->SetTexturePlace(1, 4);
                        }
                    }
                }
            }
        }
        break;

    case ENTRY_STATUS_CP_LEVEL_2:
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                if (nParamType == CLICK_TYPE_CHANGE)
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    if (nParamWho == nNumWho)
                    {
                        // チェンジ表示切替
                        pSelectUI->SetTexturePlace(2, 4);
                    }
                }
            }
        }
        break;

    case ENTRY_STATUS_CP_LEVEL_3:
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                if (nParamType == CLICK_TYPE_CHANGE)
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    if (nParamWho == nNumWho)
                    {
                        // チェンジ表示切替
                        pSelectUI->SetTexturePlace(3, 4);
                    }
                }
            }
        }
        break;
    }

    // 現在の状態を、次のエントリー状態に更新
    m_aEntryInfo[nNumWho].status = nextEntryStatus;
}

//=============================================================================
// 準備状況を反転させる
// Author : 後藤慎之助
//=============================================================================
void CCustom::ToggleReady(int nNumWho)
{
    // 反転
    m_aEntryInfo[nNumWho].bReady = !m_aEntryInfo[nNumWho].bReady;

    // SE
    if (m_aEntryInfo[nNumWho].bReady)
    {
        CManager::SoundPlay(CSound::LABEL_SE_OK);
    }
    else
    {
        CManager::SoundPlay(CSound::LABEL_SE_CANCEL);
    }

    // 準備状況に応じて、表示物を変える
    if (m_aEntryInfo[nNumWho].bReady)
    {
        // 非表示
        m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetDisp(false);
        m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(false);
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                if (nParamWho == nNumWho)
                {
                    int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                    if (nParamType == CLICK_TYPE_PARTS)
                    {
                        // パーツの選択肢は見えないように
                        pSelectUI->SetDisp(false);
                    }
                    else if (nParamType == CLICK_TYPE_READY)
                    {
                        // 表示切替
                        pSelectUI->SetTexturePlace(1, 2);
                    }
                }
            }
        }
        // 表示
        m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(true);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetActionLock(0, false);
    }
    else
    {
        // 非表示
        m_aEntryInfo[nNumWho].pUI_Bg_Ready->SetDisp(false);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetActionReset(0);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetActionLock(0, true);
        // 表示
        m_aEntryInfo[nNumWho].pUI_Bg_Select->SetDisp(true);
        for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
        {
            CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
            if (pSelectUI)
            {
                int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                if (nParamWho == nNumWho)
                {
                    int nParamType = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE);
                    if (nParamType == CLICK_TYPE_PARTS)
                    {
                        // パーツの選択肢を見えるように
                        pSelectUI->SetDisp(true);
                    }
                    else if (nParamType == CLICK_TYPE_READY)
                    {
                        // 表示切替
                        pSelectUI->SetTexturePlace(0, 2);
                    }
                }
            }
        }
    }
}

//=============================================================================
// 準備完了できているかの判断
// Author : 後藤慎之助
//=============================================================================
void CCustom::JudgmentReadyToFight(void)
{
    // 現在のエントリー人数、各UI
    int nNumCurrentEntryPlayer = 0;
    CUI *pReadyToFightBg = CUI::GetAccessUI(124);
    CUI *pReadyToFightText = CUI::GetAccessUI(SELECT_READY_TO_FIGHT);
    CUI *pStock = CUI::GetAccessUI(SELECT_STOCK);

    // 待機中以外の人数を加算
    for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
    {
        if (m_aEntryInfo[nCntPlayer].status != ENTRY_STATUS_WAITING)
        {
            nNumCurrentEntryPlayer++;

            // 準備完了していないなら、関数を抜ける
            if (!m_aEntryInfo[nCntPlayer].bReady)
            {
                m_bDispReadyToFight = false;
                if (pReadyToFightBg)
                {
                    pReadyToFightBg->SetActionReset(1);
                    pReadyToFightBg->SetActionLock(1, true);
                    pReadyToFightBg->SetActionReset(2);
                    pReadyToFightBg->SetActionLock(2, true);
                }
                if (pReadyToFightText)
                {
                    pReadyToFightText->SetActionReset(1);
                    pReadyToFightText->SetActionLock(1, true);
                    pReadyToFightText->SetActionReset(2);
                    pReadyToFightText->SetActionLock(2, true);
                }
                if (pStock)
                {
                    pStock->SetActionReset(1);
                    pStock->SetActionLock(1, true);
                }
                return;
            }
        }
    }

    // エントリー人数が2人以上（かつ準備完了している）なら、ReadyToFightとストックを表示
    if (nNumCurrentEntryPlayer >= 2)
    {
        if (!m_bDispReadyToFight)
        {
            CManager::SoundPlay(CSound::LABEL_SE_READY);
        }
        m_bDispReadyToFight = true;
        if (pReadyToFightBg)
        {
            pReadyToFightBg->SetActionLock(1, false);
            pReadyToFightBg->SetActionLock(2, false);
        }
        if (pReadyToFightText)
        {
            pReadyToFightText->SetActionLock(1, false);
            pReadyToFightText->SetActionLock(2, false);
        }
        if (pStock)
        {
            pStock->SetActionLock(1, false);
        }
    }

    // ReadyToFightが押されたなら、ゲームへ遷移
    if (m_bClickReadyToFight)
    {
        // エフェクト
        D3DXVECTOR3 effectPos = D3DXVECTOR3(0.0f, 231.0f, 0.0f);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT1, effectPos, effectPos);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT2, effectPos, effectPos);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT3, effectPos, effectPos);
        CEffect2D::Emit(CEffectData::TYPE_READY_TO_FIGHT4, effectPos, effectPos);

        CManager::SoundPlay(CSound::LABEL_SE_HIT_BIG);
        CFade::SetFade(CManager::MODE_GAME);
        CGame::SetNextGameInCustom(CGame::TYPE_ARENA, nNumCurrentEntryPlayer);

        // プレイアブル、AIレベルを結びつけていく
        int nIndexEntryPlayer = 0;
        for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
        {
            // エントリーしているキャラなら
            if (m_aEntryInfo[nCntPlayer].status != ENTRY_STATUS_WAITING)
            {
                // 空いているところに、情報を結びつける
                CPlayer::AI_LEVEL level = CPlayer::AI_LEVEL_NONE;
                switch (m_aEntryInfo[nCntPlayer].status)
                {
                case ENTRY_STATUS_CP_LEVEL_1:
                    level = CPlayer::AI_LEVEL_1;
                    break;
                case ENTRY_STATUS_CP_LEVEL_2:
                    level = CPlayer::AI_LEVEL_2;
                    break;
                case ENTRY_STATUS_CP_LEVEL_3:
                    level = CPlayer::AI_LEVEL_3;
                    break;
                }
                CGame::SetIdxPlayer(nIndexEntryPlayer, nCntPlayer);
                CGame::SetAILevel(nIndexEntryPlayer, level);
                nIndexEntryPlayer++;
            }
        }
    }
}

//=============================================================================
// カスタマイズするパーツを選択
// Author : 後藤慎之助
//=============================================================================
void CCustom::SelectParts(int nNumWho, int nNumWhere, bool bRight)
{
    // 選択のインデックス
    int nSelectIndex = 0;
    switch (nNumWhere)
    {
    case CPlayer::CUSTOM_PARTS_HEAD:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectHead;
        break;
    case CPlayer::CUSTOM_PARTS_UP:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectUp;
        break;
    case CPlayer::CUSTOM_PARTS_DOWN:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectDown;
        break;
    case CPlayer::CUSTOM_PARTS_WEP:
        nSelectIndex = m_aEntryInfo[nNumWho].nNumSelectWep;
        break;
    }

    // 右か左か
    if (bRight)
    {
        nSelectIndex++;

        // この選択で右端のパーツがあるかどうか確かめる
        int nRightParts = 0;
        switch (nNumWhere)
        {
        case CPlayer::CUSTOM_PARTS_HEAD:
            nRightParts = m_anMemoryPartsHead[nSelectIndex];
            break;
        case CPlayer::CUSTOM_PARTS_UP:
            nRightParts = m_anMemoryPartsUp[nSelectIndex];
            break;
        case CPlayer::CUSTOM_PARTS_DOWN:
            nRightParts = m_anMemoryPartsDown[nSelectIndex];
            break;
        case CPlayer::CUSTOM_PARTS_WEP:
            nRightParts = m_anMemoryPartsWep[nSelectIndex];
            break;
        }

        // 右端
        if (nRightParts == NOT_EXIST)
        {
            nSelectIndex = 0;
        }
    }
    else
    {
        nSelectIndex--;

        // 左端（存在する一番右のパーツに切り替える）
        if (nSelectIndex < 0)
        {
            for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
            {
                // 右端のパーツを部位ごとに探す
                int nRightParts = 0;
                switch (nNumWhere)
                {
                case CPlayer::CUSTOM_PARTS_HEAD:
                    nRightParts = m_anMemoryPartsHead[nCnt];
                    break;
                case CPlayer::CUSTOM_PARTS_UP:
                    nRightParts = m_anMemoryPartsUp[nCnt];
                    break;
                case CPlayer::CUSTOM_PARTS_DOWN:
                    nRightParts = m_anMemoryPartsDown[nCnt];
                    break;
                case CPlayer::CUSTOM_PARTS_WEP:
                    nRightParts = m_anMemoryPartsWep[nCnt];
                    break;
                }

                // このパーツが存在していないなら、一つ前が右端
                if (nRightParts == NOT_EXIST)
                {
                    nSelectIndex = nCnt - 1;

                    if (nSelectIndex < 0)
                    {
                        nSelectIndex = 0;
                    }

                    // 見つけたら、forを抜ける
                    break;
                }
            }
        }
    }

    // 保存するパーツ番号を取得、テキストと選択のインデックスを更新
    int nNumSaveParts = 0;
    switch (nNumWhere)
    {
    case CPlayer::CUSTOM_PARTS_HEAD:
        nNumSaveParts = m_anMemoryPartsHead[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Head->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectHead = nSelectIndex;
        break;
    case CPlayer::CUSTOM_PARTS_UP:
        nNumSaveParts = m_anMemoryPartsUp[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Up->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectUp = nSelectIndex;
        break;
    case CPlayer::CUSTOM_PARTS_DOWN:
        nNumSaveParts = m_anMemoryPartsDown[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Down->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectDown = nSelectIndex;
        break;
    case CPlayer::CUSTOM_PARTS_WEP:
        nNumSaveParts = m_anMemoryPartsWep[nSelectIndex];
        m_aEntryInfo[nNumWho].pText_Wep->SetText(CManager::GetModelData()->CModelData::GetPartsList(nNumSaveParts)->cName);
        m_aEntryInfo[nNumWho].nNumSelectWep = nSelectIndex;
        // 武器を見る
        m_aEntryInfo[nNumWho].pPlayer->SetCustomWeaponLook();
        break;
    }

    // パーツを保存
    SaveCustom(nNumWho, nNumWhere, nNumSaveParts);

    // プレイヤーをリロード
    m_aEntryInfo[nNumWho].pPlayer->LoadCustom();
}

//=============================================================================
// カスタマイズパーツセーブ処理
// Author : 後藤慎之助
//=============================================================================
void CCustom::SaveCustom(int nNumSaveWho, int nNumSaveWhere, int nNumSaveParts)
{
    // 書き込むパーツの数字たち
    int anNumParts[MAX_PARTS] = {};
    for (int nCntWhere = 0; nCntWhere < MAX_PARTS; nCntWhere++)
    {
        // セーブするパーツの箇所は、セーブするパーツの番号を代入
        if (nCntWhere == nNumSaveWhere)
        {
            anNumParts[nCntWhere] = nNumSaveParts;
            continue;
        }

        // 今回セーブするパーツの箇所以外のものは、元の情報をそのまま書き込むため、取得
        if (m_aEntryInfo[nNumSaveWho].pPlayer)
        {
            anNumParts[nCntWhere] = m_aEntryInfo[nNumSaveWho].pPlayer->GetCustomPartsNum(nCntWhere);
        }
    }

    // ファイルポインタ
    FILE *pFile = NULL;

    // カスタマイズデータのファイルを開く
    switch (nNumSaveWho)
    {
    case PLAYER_1:
        pFile = fopen("data/TXT/custom1.txt", "w");
        break;
    case PLAYER_2:
        pFile = fopen("data/TXT/custom2.txt", "w");
        break;
    case PLAYER_3:
        pFile = fopen("data/TXT/custom3.txt", "w");
        break;
    case PLAYER_4:
        pFile = fopen("data/TXT/custom4.txt", "w");
        break;
    }

    // ファイルを開けたら
    if (pFile != NULL)
    {
        for (int nCntWhere = 0; nCntWhere < MAX_PARTS; nCntWhere++)
        {
            fprintf(pFile, "%d\n", anNumParts[nCntWhere]);
        }

        // ファイルを閉じる
        fclose(pFile);
    }
}

//=============================================================================
// オススメ処理（SaveCustomとInitが合わさった感じ）
// Author : 後藤慎之助
//=============================================================================
void CCustom::DoOsusume(int nNumWho)
{
    // 書き込むパーツの数字たち
    int anNumParts[MAX_PARTS] = {};
    int nRand = 0;

	//連続で同じものが出ないようにする処理
	do
	{
		nRand = GetRandNum(7, 1);
	} while (nRand == m_aEntryInfo[nNumWho].nOsusumeOld);
	m_aEntryInfo[nNumWho].nOsusumeOld = nRand;

    // おすすめ装備は随時更新
    switch (nRand)
    {
    case 1:
        anNumParts[0] = 0;
        anNumParts[1] = 1;
        anNumParts[2] = 2;
        anNumParts[3] = 3;
        break;
    case 2:
        anNumParts[0] = 4;
        anNumParts[1] = 5;
        anNumParts[2] = 6;
        anNumParts[3] = 7;
        break;
    case 3:
        anNumParts[0] = 8;
        anNumParts[1] = 9;
        anNumParts[2] = 10;
        anNumParts[3] = 11;
        break;
	case 4:
		anNumParts[0] = 12;
		anNumParts[1] = 13;
		anNumParts[2] = 14;
		anNumParts[3] = 26;
		break;
    case 5:
        anNumParts[0] = 15;
        anNumParts[1] = 16;
        anNumParts[2] = 17;
        anNumParts[3] = 18;
        break;
	case 6:
		anNumParts[0] = 19;
		anNumParts[1] = 20;
		anNumParts[2] = 21;
		anNumParts[3] = 22;
		break;
	case 7:
		anNumParts[0] = 23;
		anNumParts[1] = 24;
		anNumParts[2] = 25;
		anNumParts[3] = 22;
		break;
    }

    // カスタマイズデータのファイルを開く
    FILE *pFile = NULL;
    switch (nNumWho)
    {
    case PLAYER_1:
        pFile = fopen("data/TXT/custom1.txt", "w");
        break;
    case PLAYER_2:
        pFile = fopen("data/TXT/custom2.txt", "w");
        break;
    case PLAYER_3:
        pFile = fopen("data/TXT/custom3.txt", "w");
        break;
    case PLAYER_4:
        pFile = fopen("data/TXT/custom4.txt", "w");
        break;
    }

    // ファイルを開けたら、装備登録
    if (pFile != NULL)
    {
        for (int nCntWhere = 0; nCntWhere < MAX_PARTS; nCntWhere++)
        {
            fprintf(pFile, "%d\n", anNumParts[nCntWhere]);
        }

        // ファイルを閉じる
        fclose(pFile);
    }

    // プレイヤーをリロード
    m_aEntryInfo[nNumWho].pPlayer->LoadCustom();

    // 選択カーソルとテキストを合わせる
    int nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_HEAD);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsHead[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectHead = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Head->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }

    nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_UP);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsUp[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectUp = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Up->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }

    nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_DOWN);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsDown[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectDown = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Down->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }

    nPartNum = m_aEntryInfo[nNumWho].pPlayer->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_WEP);
    for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
    {
        if (m_anMemoryPartsWep[nCntEachParts] == nPartNum)
        {
            m_aEntryInfo[nNumWho].nNumSelectWep = nCntEachParts;
            m_aEntryInfo[nNumWho].pText_Wep->SetText(CManager::GetModelData()->CModelData::GetPartsList(nPartNum)->cName);
        }
    }
}
