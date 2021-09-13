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
#include "ui.h"
#include "fade.h"
#include "debug.h"
#include "player.h"
#include "camera.h"
#include "modelData.h"
#include "text.h"

//========================================
// マクロ定義
//========================================

//=============================================================================
// カスタマイズ画面のコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CCustom::CCustom()
{
    memset(m_apPlayer, 0, sizeof(m_apPlayer));
    memset(m_anPartsHead, NOT_EXIST, sizeof(m_anPartsHead));
    memset(m_anPartsUp, NOT_EXIST, sizeof(m_anPartsUp));
    memset(m_anPartsDown, NOT_EXIST, sizeof(m_anPartsDown));
    memset(m_anPartsWep, NOT_EXIST, sizeof(m_anPartsWep));
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
    m_apPlayer[PLAYER_1] = CPlayer::CreateInCustom(D3DXVECTOR3(-950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_001, true);
    m_apPlayer[PLAYER_2] = CPlayer::CreateInCustom(D3DXVECTOR3(-316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_002, false);
    m_apPlayer[PLAYER_3] = CPlayer::CreateInCustom(D3DXVECTOR3(316.6f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_003, false);
    m_apPlayer[PLAYER_4] = CPlayer::CreateInCustom(D3DXVECTOR3(950.0f, 650.0f, 0.0f), DEFAULT_VECTOR, CPlayer::PLAYABLE_004, false);

    // カーソル生成
    const D3DXVECTOR3 cursorFirstPos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    m_aEntryInfo[PLAYER_1].pUI_Cursor = CUI::Create(53, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR);
    m_aEntryInfo[PLAYER_2].pUI_Cursor = CUI::Create(54, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);
    m_aEntryInfo[PLAYER_3].pUI_Cursor = CUI::Create(55, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);
    m_aEntryInfo[PLAYER_4].pUI_Cursor = CUI::Create(56, cursorFirstPos, D3DXVECTOR3(59.0f, 59.0f, 0.0f), 0, DEFAULT_COLOR_NONE_ALPHA);

    // パーツ選択情報の初期化
    float fTextPosX = 167.0f;
    for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
    {
        int nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_HEAD);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsHead[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectHead = nCntEachParts;
            }
        }

        nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_UP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsUp[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectUp = nCntEachParts;
            }
        }

        nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_DOWN);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsDown[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectDown = nCntEachParts;
            }
        }

        nPartNum = m_apPlayer[nCnt]->GetCustomPartsNum(CPlayer::CUSTOM_PARTS_WEP);
        for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
        {
            if (m_anPartsWep[nCntEachParts] == nPartNum)
            {
                m_aEntryInfo[nCnt].nNumSelectWep = nCntEachParts;
            }
        }

        // テキストを設定
        D3DCOLOR color = TEXT_EXIST_COLOR;
        if (nCnt != PLAYER_1)
        {
            color = TEXT_NOT_EXIST_COLOR;
        }
        m_aEntryInfo[nCnt].pText_Head = CText::Create(D3DXVECTOR3(fTextPosX, 427.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsHead[m_aEntryInfo[nCnt].nNumSelectHead]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Up = CText::Create(D3DXVECTOR3(fTextPosX, 490.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsUp[m_aEntryInfo[nCnt].nNumSelectUp]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Down = CText::Create(D3DXVECTOR3(fTextPosX, 553.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsDown[m_aEntryInfo[nCnt].nNumSelectDown]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);
        m_aEntryInfo[nCnt].pText_Wep = CText::Create(D3DXVECTOR3(fTextPosX, 616.0f, 0.0f), 30,
            CManager::GetModelData()->CModelData::GetPartsList((m_anPartsWep[m_aEntryInfo[nCnt].nNumSelectWep]))->cName,
            CText::ALIGN_CENTER, "Reggae One", color);

        // テキストの横位置を加算
        fTextPosX += 315.0f;
    }

    // カメラのロックオン場所を変える
    CManager::GetCamera()->CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_CUSTOM);

    // 1Pのカーソルは必ず使う
    m_aEntryInfo[PLAYER_1].bUseCursor = true;

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
                if (m_anPartsHead[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsHead[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_UP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anPartsUp[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsUp[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_DOWN:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anPartsDown[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsDown[nCntEachParts] = nCntParts;
                    break;
                }
            }
            break;
        case CPlayer::CUSTOM_PARTS_WEP:
            for (int nCntEachParts = 0; nCntEachParts < MAX_EACH_PARTS; nCntEachParts++)
            {
                if (m_anPartsWep[nCntEachParts] == NOT_EXIST)
                {
                    m_anPartsWep[nCntEachParts] = nCntParts;
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

    //// フェードしていないなら、選択可能
    //if (CFade::GetFade() == CFade::FADE_NONE)
    //{
    //    // 右または左が押されたら
    //    if (pInputKeyboard->GetKeyboardTrigger(DIK_D) || pInputKeyboard->GetKeyboardTrigger(DIK_A) ||
    //        bTiltedStickP1 && STICK_RIGHT(fStickAngleP1)
    //        || bTiltedStickP1 && STICK_LEFT(fStickAngleP1))
    //    {
    //        bool bRight = false;
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_D) || bTiltedStickP1 && STICK_RIGHT(fStickAngleP1) && m_nSelectCoolTime <= 0)
    //        {
    //            bRight = true;
    //        }

    //        // パーツを切り替え
    //        switch (m_select)
    //        {
    //        case SELECT_1P_HEAD:
    //            if (bRight)
    //            {
    //                m_nNumSelectHead++;

    //                // 右端
    //                if (m_anPartsHead[m_nNumSelectHead] == NOT_EXIST)
    //                {
    //                    m_nNumSelectHead = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectHead--;

    //                // 左端（存在する一番右のパーツに切り替える）
    //                if (m_nNumSelectHead < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsHead[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectHead = nCnt - 1;

    //                            if (m_nNumSelectHead < 0)
    //                            {
    //                                m_nNumSelectHead = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_HEAD, m_anPartsHead[m_nNumSelectHead]);
    //            m_pText_Head->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsHead[m_nNumSelectHead]))->cName);
    //            break;

    //        case SELECT_1P_UP:
    //            if (bRight)
    //            {
    //                m_nNumSelectUp++;

    //                // 右端
    //                if (m_anPartsUp[m_nNumSelectUp] == NOT_EXIST)
    //                {
    //                    m_nNumSelectUp = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectUp--;

    //                // 左端（存在する一番右のパーツに切り替える）
    //                if (m_nNumSelectUp < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsUp[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectUp = nCnt - 1;

    //                            if (m_nNumSelectUp < 0)
    //                            {
    //                                m_nNumSelectUp = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_UP, m_anPartsUp[m_nNumSelectUp]);
    //            m_pText_Up->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsUp[m_nNumSelectUp]))->cName);
    //            break;

    //        case SELECT_1P_DOWN:
    //            if (bRight)
    //            {
    //                m_nNumSelectDown++;

    //                // 右端
    //                if (m_anPartsDown[m_nNumSelectDown] == NOT_EXIST)
    //                {
    //                    m_nNumSelectDown = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectDown--;

    //                // 左端（存在する一番右のパーツに切り替える）
    //                if (m_nNumSelectDown < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsDown[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectDown = nCnt - 1;

    //                            if (m_nNumSelectDown < 0)
    //                            {
    //                                m_nNumSelectDown = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_DOWN, m_anPartsDown[m_nNumSelectDown]);
    //            m_pText_Down->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsDown[m_nNumSelectDown]))->cName);
    //            break;

    //        case SELECT_1P_WEP:
    //            if (bRight)
    //            {
    //                m_nNumSelectWep++;

    //                // 右端
    //                if (m_anPartsWep[m_nNumSelectWep] == NOT_EXIST)
    //                {
    //                    m_nNumSelectWep = 0;
    //                }
    //            }
    //            else
    //            {
    //                m_nNumSelectWep--;

    //                // 左端（存在する一番右のパーツに切り替える）
    //                if (m_nNumSelectWep < 0)
    //                {
    //                    for (int nCnt = 0; nCnt < MAX_EACH_PARTS; nCnt++)
    //                    {
    //                        if (m_anPartsWep[nCnt] == NOT_EXIST)
    //                        {
    //                            m_nNumSelectWep = nCnt - 1;

    //                            if (m_nNumSelectWep < 0)
    //                            {
    //                                m_nNumSelectWep = 0;
    //                            }

    //                            break;
    //                        }
    //                    }
    //                }
    //            }
    //            SaveCustom(PLAYER_1, CPlayer::CUSTOM_PARTS_WEP, m_anPartsWep[m_nNumSelectWep]);
    //            m_pText_Wep->SetText(CManager::GetModelData()->CModelData::GetPartsList((m_anPartsWep[m_nNumSelectWep]))->cName);
    //            break;
    //        }

    //        // 選択のクールタイムを設定
    //        m_nSelectCoolTime = MENU_SELECT_COOL_TIME;

    //        // プレイヤーをリロード
    //        m_apPlayer[0]->LoadCustom();
    //    }
    //    else
    //    {
    //        // 上移動
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || bTiltedStickP1 && STICK_UP(fStickAngleP1) && m_nSelectCoolTime <= 0)
    //        {
    //            // 上の選択肢に
    //            m_select--;

    //            // 一番上の選択肢なら、一番下へ
    //            if (m_select < SELECT_1P_HEAD)
    //            {
    //                m_select = SELECT_MAX - 1;
    //            }

    //            // 選択のクールタイムを設定
    //            m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //        }

    //        // 下移動
    //        if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || bTiltedStickP1 && m_nSelectCoolTime <= 0)
    //        {
    //            // STICK_DOWNの定義が、または と組み合わせが悪いため
    //            bool bDown = true;
    //            if (bTiltedStickP1)
    //            {
    //                if (STICK_DOWN(fStickAngleP1))
    //                {
    //                    bDown = true;
    //                }
    //                else
    //                {
    //                    bDown = false;
    //                }
    //            }

    //            // ダウンするフラグがtrueなら
    //            if (bDown)
    //            {
    //                // 下の選択肢に
    //                m_select++;

    //                // 一番下の選択肢なら、一番上へ
    //                if (m_select >= SELECT_MAX)
    //                {
    //                    m_select = SELECT_1P_HEAD;
    //                }

    //                // 選択のクールタイムを設定
    //                m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
    //            }
    //        }
    //    }
    //}

    //// 決定キーでタイトルへ
    //if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    //{
    //    // タイトルモードに移行
    //    CFade::SetFade(CManager::MODE_TITLE);
    //}

    //// カーソルの位置を変える
    //D3DXVECTOR3 pos = D3DXVECTOR3(167.0f, 443.0f, 0.0f);
    //float fDigitPosY = 63.0f * m_select;
    //pos.y += fDigitPosY;
    //if (m_pUI_Cursor)
    //{
    //    m_pUI_Cursor->SetPosition(pos);
    //}
}

//=============================================================================
// カーソル移動
// Author : 後藤慎之助
//=============================================================================
void CCustom::MoveCursor(void)
{
    for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
    {
        // カーソルを使うなら
        if (m_aEntryInfo[nCntPlayer].bUseCursor)
        {
            // カーソルがあるなら
            if (m_aEntryInfo[nCntPlayer].pUI_Cursor)
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

                // 位置を取得
                D3DXVECTOR3 pos = m_aEntryInfo[nCntPlayer].pUI_Cursor->GetPosition();

                // 移動
                if (bTiltedStick)
                {
                    const float ADJUST_RATE = 0.0008f;   // スティックの傾きの値を、位置に足せるよう調整
                    pos.x += sinf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                    pos.y += -cosf(fStickAngle)* fTiltedStickValue * ADJUST_RATE;
                }

                // 位置を設定
                m_aEntryInfo[nCntPlayer].pUI_Cursor->SetPosition(pos);
            }
        }
    }
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
        if (m_apPlayer[nNumSaveWho])
        {
            anNumParts[nCntWhere] = m_apPlayer[nNumSaveWho]->GetCustomPartsNum(nCntWhere);
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