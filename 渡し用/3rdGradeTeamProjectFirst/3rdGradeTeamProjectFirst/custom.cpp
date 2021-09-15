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

//========================================
// マクロ定義
//========================================
#define CURSOR_ADJUST_COLLISON_POS D3DXVECTOR3(-20.0f, -20.0f, 0.0f) // カーソルの当たり判定の位置調整
#define CURSOR_COLLISION_SIZE D3DXVECTOR3(5.0f, 5.0f, 0.0f)          // カーソルの当たり判定の大きさ

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
    m_aEntryInfo[PLAYER_1].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(-950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_001, false);
    m_aEntryInfo[PLAYER_2].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(-316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_002, false);
    m_aEntryInfo[PLAYER_3].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_003, false);
    m_aEntryInfo[PLAYER_4].pPlayer = CPlayer::CreateInCustom(D3DXVECTOR3(950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_004, false);

    // カーソル生成
    const D3DXVECTOR3 cursorFirstPos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    m_aEntryInfo[PLAYER_1].pUI_Cursor = CUI::Create(53, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);
    m_aEntryInfo[PLAYER_2].pUI_Cursor = CUI::Create(54, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);
    m_aEntryInfo[PLAYER_3].pUI_Cursor = CUI::Create(55, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);
    m_aEntryInfo[PLAYER_4].pUI_Cursor = CUI::Create(56, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);

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
        m_aEntryInfo[nCnt].pUI_Bg_Reday = CUI::GetAccessUI(101 + (nCnt * 3));               // 準備完了101
        m_aEntryInfo[nCnt].pUI_Bg_Wait = CUI::GetAccessUI(102 + (nCnt * 3));                // 未エントリー102
        m_aEntryInfo[nCnt].pUI_Bg_Select_Out_Frame = CUI::GetAccessUI(112 + (nCnt * 3));    // カスタム外枠112
        m_aEntryInfo[nCnt].pUI_Bg_Select_In_Frame = CUI::GetAccessUI(113 + (nCnt * 3));     // カスタム外枠113
    }

    // カメラのロックオン場所を変える
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    //// BGMを再生
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::LABEL_BGM_RESULT);

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
    //// BGMを停止
    //CSound *pSound = CManager::GetSound();
    //pSound->Stop(CSound::LABEL_BGM_RESULT);
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CCustom::Update(void)
{
    // カーソル移動
    MoveCursor();
}

//=============================================================================
// カーソル移動
// Author : 後藤慎之助
//=============================================================================
void CCustom::MoveCursor(void)
{
    for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
    {
        ////キーボードの確保したメモリを取得
        //CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

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

                // 位置を設定
                m_aEntryInfo[nCntPlayer].pUI_Cursor->SetPosition(cursorPos);

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
            }
        }
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
            // 当たっているなら
            if (IsCollisionRectangle2D(&cursorPos, &pSelectUI->GetCollisionPos(),
                &CURSOR_COLLISION_SIZE, &pSelectUI->GetCollisionSize()))
            {
                // 拡大縮小を解除
                pSelectUI->SetActionLock(0, false);

                // クリック処理
                ClickSelect(nNumWho, pSelectUI);

                // 今回当たっている選択肢
                nNumSelectUICurrent = nCntSelect;

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
        }
    }

    // 1F前の選択肢を記憶
    m_aEntryInfo[nNumWho].nNumSelectUIOld = nNumSelectUICurrent;
}

//=============================================================================
// 選択肢のクリック
// Author : 後藤慎之助
//=============================================================================
void CCustom::ClickSelect(int nNumWho, CUI* pSelectUI)
{
    // 選んだUIが存在しないなら関数を抜ける
    if (!pSelectUI)
    {
        return;
    }

    // フェードしていないなら、選択可能
    if (CFade::GetFade() == CFade::FADE_NONE)
    {
        // コントローラを取得
        CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

        // Aボタンが押されたら
        if (pInputJoypad->GetJoypadTrigger(nNumWho, CInputJoypad::BUTTON_A))
        {
            // 種類によって反応を変える
            switch ((int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_TYPE))
            {
            case CLICK_TYPE_PARTS:
                {
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
        // 非表示
        m_aEntryInfo[nNumWho].pPlayer->SetDisp(false);
        m_aEntryInfo[nNumWho].pUI_Bg_Select->SetAlpha(0.0f);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_In_Frame->SetAlpha(0.0f);
        m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetAlpha(0.0f);
        m_aEntryInfo[nNumWho].pUI_Cursor->SetAlpha(0.0f);
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
                    pSelectUI->SetAlpha(0.0f);
                }
            }
        }
        // 表示
        m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetAlpha(1.0f);
        break;

    case ENTRY_STATUS_PLAYER:
    case ENTRY_STATUS_CP_LEVEL_1:
        // もし、待機中からの遷移なら
        if (m_aEntryInfo[nNumWho].status == ENTRY_STATUS_WAITING)
        {
            // 非表示
            m_aEntryInfo[nNumWho].pUI_Bg_Wait->SetAlpha(0.0f);
            // 表示
            m_aEntryInfo[nNumWho].pPlayer->SetDisp(true);
            m_aEntryInfo[nNumWho].pUI_Bg_Select->SetAlpha(1.0f);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_In_Frame->SetAlpha(1.0f);
            m_aEntryInfo[nNumWho].pUI_Bg_Select_Out_Frame->SetAlpha(1.0f);
            m_aEntryInfo[nNumWho].pUI_Cursor->SetAlpha(1.0f);
            m_aEntryInfo[nNumWho].pText_Head->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Up->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Down->SetColor(TEXT_EXIST_COLOR);
            m_aEntryInfo[nNumWho].pText_Wep->SetColor(TEXT_EXIST_COLOR);
            for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
            {
                CUI *pSelectUI = CUI::GetAccessUI(nCntSelect);
                if (pSelectUI)
                {
                    int nParamWho = (int)pSelectUI->GetActionParam(CURSOR_CLICK_ACTION_INFO_IDX, PARAM_CLICK_WHO);
                    if (nParamWho == nNumWho)
                    {
                        pSelectUI->SetAlpha(1.0f);
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
    case CPlayer::PLAYABLE_001:
        pFile = fopen("data/TXT/custom1.txt", "w");
        break;
    case CPlayer::PLAYABLE_002:
        pFile = fopen("data/TXT/custom2.txt", "w");
        break;
    case CPlayer::PLAYABLE_003:
        pFile = fopen("data/TXT/custom3.txt", "w");
        break;
    case CPlayer::PLAYABLE_004:
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