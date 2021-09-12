//===============================================
//
// UI処理 (ui.cpp)
// Author : 後藤慎之助
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "character.h"
#include "texture.h"
//#include "player.h"

//=======================================
// 静的メンバ変数宣言
//=======================================

//========================================
// UIのコンストラクタ
// Author : 後藤慎之助
//========================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_nTexType = 0;
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_action = CUI::ACTION_NONE;
    m_fAngle = 0.0f;
    m_bUse = true;
    m_bBlinking = false;
    m_nCntTime = 0;
    m_bFadeOut = false;
}

//========================================
// UIのデストラクタ
// Author : 後藤慎之助
//========================================
CUI::~CUI()
{

}

//========================================
// UIの初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_nTexType);

    return S_OK;
}

//========================================
// UIの終了処理
// Author : 後藤慎之助
//========================================
void CUI::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// UIの更新処理
// Author : 後藤慎之助
//========================================
void CUI::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得
    int nPatternAnim = 0;                              // パターンアニメのカウンタを利用するとき

                                                       //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    switch (m_action)
    {
    case ACTION_TITLE_LOGO:

        // 流れ落ちる
        pos.y += TITLE_LOGO_FLOW_DOWN_SPEED;

        // 一定の値で止めておく
        if (pos.y >= TITLE_LOGO_TARGET_POS_Y)
        {
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // タイトルロゴをすぐに下す
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        break;

    case ACTION_TITLE_PRESS_ENTER:

        // タイトルの時間をカウント
        m_nCntTime++;

        // タイトルの最大時間
        if (m_nCntTime > TITLE_MAX_TIME)
        {
            m_nCntTime = TITLE_MAX_TIME;
        }

        // 色を変える
        if (m_bFadeOut == true)
        {
            m_col.r -= PRESS_START_FADE_RATE;
            m_col.g -= PRESS_START_FADE_RATE;
            m_col.b -= PRESS_START_FADE_RATE;
        }
        else
        {
            m_col.r += PRESS_START_FADE_RATE;
            m_col.g += PRESS_START_FADE_RATE;
            m_col.b += PRESS_START_FADE_RATE;
        }
        if (m_col.r <= PRESS_START_FADE_OUT_MIN)
        {
            m_bFadeOut = false;
        }
        else if (m_col.r >= 1.0f)
        {
            m_bFadeOut = true;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // 一定時間経過で点滅可能に
            if (m_nCntTime >= TITLE_NEXT_SCENE)
            {
                // 点滅のフラグをtrueに
                m_bBlinking = true;
            }
            // それ以外は、カウンタを遷移可能まで一気に進める
            else
            {
                m_nCntTime = TITLE_NEXT_SCENE;
            }
        }

        // 点滅するなら
        if (m_bBlinking == true)
        {
            // カウンターを進めて、パターンを切り替える
            nPatternAnim = CountAnimation(2, 2);

            // 色を2パターンで切り替える
            if (nPatternAnim == 1)
            {
                m_col.r = PRESS_START_FADE_OUT_MIN;
                m_col.g = PRESS_START_FADE_OUT_MIN;
                m_col.b = PRESS_START_FADE_OUT_MIN;
            }
            else
            {
                m_col.r = 1.0f;
                m_col.g = 1.0f;
                m_col.b = 1.0f;
            }
        }

        break;
    }

    // 色を反映
    SetColor(m_col);

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // ゲージ系以外は、通常の頂点座標
    if (m_action != ACTION_GAUGE)
    {
        SetVertex();
    }

    // 使用フラグがfalseなら消す
    if (m_bUse == false)
    {
        Uninit();
    }
}

//========================================
// UIの描画処理
// Author : 後藤慎之助
//========================================
void CUI::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// UIの作成
// Author : 後藤慎之助
//========================================
CUI *CUI::Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, ACTION action)
{
    CUI *pUI = NULL;

    // メモリを確保
    pUI = new CUI;

    // テクスチャのタイプは先に結びつけておく
    pUI->m_nTexType = nTexType;

    // 初期化
    pUI->Init(pos, size);

    // メンバ変数を結びつける
    pUI->m_col = col;
    pUI->m_action = action;

    return pUI;
}

//========================================
// UIの設置
// Author : 後藤慎之助
//========================================
void CUI::Place(SET set)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[256];	             // 文字として読み取り用
    char cHeadText[256];	             // 文字の判別用
    char cDie[256];		                 // 使わない文字
    int nNumType = 0;                    // タイプのナンバー
    D3DXVECTOR3 pos = DEFAULT_VECTOR;    // 位置
    D3DXVECTOR3 size = DEFAULT_VECTOR;   // 大きさ
    D3DXCOLOR col = DEFAULT_COLOR;       // 色
    int nAction = 0;                     // 動き

    // ファイルを開く
    switch (set)
    {
    case SET_TITLE:
        pFile = fopen("data/TXT/ui_title.txt", "r");
        break;
    case SET_MANUAL:
        pFile = fopen("data/TXT/ui_manual.txt", "r");
        break;
    case SET_GAME:
        pFile = fopen("data/TXT/ui_game.txt", "r");
        break;
    case SET_RESULT:
        pFile = fopen("data/TXT/ui_result.txt", "r");
        break;
    }

    // 開けたら
    if (pFile != NULL)
    {
        // SCRIPTの文字が見つかるまで
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            // テキストからcReadText分文字を受け取る
            fgets(cReadText, sizeof(cReadText), pFile);

            // cReedTextをcHeadTextに格納
            sscanf(cReadText, "%s", &cHeadText);
        }

        // cHeadTextがSCRIPTの時
        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            // cHeadTextがEND_SCRIPTになるまで
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {
                fgets(cReadText, sizeof(cReadText), pFile);
                sscanf(cReadText, "%s", &cHeadText);

                // cHeadTextがUISETの時
                if (strcmp(cHeadText, "UISET") == 0)
                {
                    // cHeadTextがEND_UISETになるまで
                    while (strcmp(cHeadText, "END_UISET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);

                            // 不正な値は全てデフォルトを呼び出す
                            if (nNumType < 0 || nNumType >= MAX_TEXTURE)
                            {
                                nNumType = 0;
                            }
                        }
                        else if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f", &cDie, &cDie, &pos.x, &pos.y);
                        }
                        else if (strcmp(cHeadText, "SIZE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f", &cDie, &cDie, &size.x, &size.y);
                        }
                        else if (strcmp(cHeadText, "COL") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie, &col.r, &col.g, &col.b, &col.a);
                        }
                        else if (strcmp(cHeadText, "ACTION") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nAction);

                            // 不正な値は全てデフォルトを呼び出す
                            if (nAction < ACTION_NONE || nAction >= ACTION_MAX)
                            {
                                nAction = ACTION_NONE;
                            }

                            // 生成
                            Create(nNumType, pos, size, col, (ACTION)nAction);
                        }
                    }
                }
            }
        }
        // ファイルを閉じる
        fclose(pFile);
    }
    // 開けなかったら
    else
    {
        printf("開けれませんでした\n");
    }
}

//========================================
// 拡大するUI
// Author : 後藤慎之助
//========================================
D3DXVECTOR3 CUI::Extend(D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 returnSize = size;  // 返すサイズ
    int nCnt = 0;                   // カウンタ

                                    // 横サイズ拡大
    returnSize.x += UI_EXTEND_MISSION_EXTEND_RATE_X;

    // 縦サイズ拡大
    returnSize.y += UI_EXTEND_MISSION_EXTEND_RATE_Y;

    // 横サイズの制限
    if (returnSize.x >= UI_EXTEND_MISSION_MAX_SIZE_X)
    {
        returnSize.x = UI_EXTEND_MISSION_MAX_SIZE_X;
    }

    // 縦サイズの制限
    if (returnSize.y >= UI_EXTEND_MISSION_MAX_SIZE_Y)
    {
        returnSize.y = UI_EXTEND_MISSION_MAX_SIZE_Y;
    }

    // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
    nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

    // フェードアウトを始めるカウントを超えたら、α値を下げていく
    if (nCnt >= UI_EXTEND_MISSION_FADE_OUT_COUNT)
    {
        m_col.a -= UI_EXTEND_MISSION_FADE_OUT_RATE;
    }

    // 透明度が0を下回ったら、消す
    if (m_col.a <= 0.0f)
    {
        m_bUse = false;
    }

    return returnSize;
}