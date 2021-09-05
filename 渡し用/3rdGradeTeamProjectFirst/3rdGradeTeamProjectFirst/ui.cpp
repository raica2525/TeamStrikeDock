//=============================================================================
//
// UI処理 (ui.cpp)
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//=======================================
// インクルードファイル
//=======================================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "texture.h"

//=======================================
// 静的メンバ変数宣言
//=======================================

//=========================================================
// UIのコンストラクタ
// Author : 後藤慎之助
//=========================================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_nTexType = 0;
    memset(m_aActionInfo, 0, sizeof(m_aActionInfo));
    memset(m_aActionInfo->afParam, 0, sizeof(m_aActionInfo->afParam));
    memset(m_aActionInfo->afMemoryParam, 0, sizeof(m_aActionInfo->afMemoryParam));
    m_bUse = true;

    m_fRotAngle = 0.0f;
    m_col = DEFAULT_COLOR;
    m_bUseAdditiveSynthesis = false;

    m_memoryPos = DEFAULT_VECTOR;                
    m_memorySize = DEFAULT_VECTOR;
    m_fMemoryRotAngle = 0.0f;          
    m_memoryCol = DEFAULT_COLOR;

    m_bOneRoundAnim = false;
    m_nAnimPattern = 0;
    m_nAnimSpeed = 0;
    m_bRepeat = false;
}

//=========================================================
// UIのデストラクタ
// Author : 後藤慎之助
//=========================================================
CUI::~CUI()
{

}

//=========================================================
// UIの初期化処理
// Author : 後藤慎之助
//=========================================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_nTexType);

    return S_OK;
}

//=========================================================
// UIの終了処理
// Author : 後藤慎之助
//=========================================================
void CUI::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//=========================================================
// UIの更新処理
// Author : 後藤慎之助
//=========================================================
void CUI::Update(void)
{
    // アニメーションを使うなら
    if (m_nAnimPattern > 1)
    {
        // 一周していないならテクスチャアニメーションを更新
        if (!m_bOneRoundAnim)
        {
            // リピートするなら、一周のフラグと結びつけない
            if (m_bRepeat)
            {
                CScene2D::SetAnimation(m_nAnimSpeed, m_nAnimPattern);
            }
            else
            {
                m_bOneRoundAnim = CScene2D::SetAnimation(m_nAnimSpeed, m_nAnimPattern);
            }
        }
    }

    // アクション
    for (int nCnt = 0; nCnt < MAX_ACTION; nCnt++)
    {
        PlayAction(nCnt);
    }
    
    // 各頂点を更新（角度も反映）（ゲージ系は、別で頂点情報を調整済み）
    CScene2D::SetColor(m_col);
    if (m_aActionInfo[0].action != ACTION_GAUGE && m_aActionInfo[1].action != ACTION_GAUGE &&
        m_aActionInfo[2].action != ACTION_GAUGE && m_aActionInfo[3].action != ACTION_GAUGE)
    {
        CScene2D::SetRotVertex(m_fRotAngle);
    }

    // 透明、大きさがないなら使用フラグをfalseに
    D3DXVECTOR3 size = CScene2D::GetSize();
    if (m_col.a < 0.0f || size.x < 0.0f || size.y < 0.0f)
    {
        m_bUse = false;
    }
    
    // 使用フラグがないなら終了処理（上記の使用フラグをfalseに以外にも、セッターからfalseにできるためこの手法）
    if (!m_bUse)
    {
        Uninit();
    }
}

//=========================================================
// UIの描画処理
// Author : 後藤慎之助
//=========================================================
void CUI::Draw(void)
{
    // 加算合成
    if (m_bUseAdditiveSynthesis)
    {
        CScene2D::SetAdditiveSynthesis();
    }

    // 描画処理
    CScene2D::Draw();
}

//=========================================================
// UIの作成
// Author : 後藤慎之助
//=========================================================
CUI *CUI::Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, int nRotAngle, D3DXCOLOR col, bool bUseAddiveSynthesis, int nAlphaTestBorder, bool bUseZBuffer)
{
    CUI *pUI = NULL;

    // メモリを確保
    pUI = new CUI;

    // テクスチャのタイプは先に結びつけておく
    pUI->m_nTexType = nTexType;

    // 初期化（3Dモデルの裏に行くなら、ZBufferを考慮する）
    float fZBufferValue = 0.0f;
    if (bUseZBuffer)
    {
        fZBufferValue = 1.0f;
    }
    pos.z = fZBufferValue;
    pUI->Init(pos, size);

    // 親元に設定を反映
    pUI->CScene2D::SetAlphaTestBorder(nAlphaTestBorder);

    // メンバ変数を結びつける
    if (nRotAngle != 0)
    {
        pUI->m_fRotAngle = D3DXToRadian((float)nRotAngle);
    }
    pUI->m_col = col;
    pUI->m_bUseAdditiveSynthesis = bUseAddiveSynthesis;

    // 記憶用も結びつける
    pUI->m_memoryPos = pos;
    pUI->m_memorySize = size;
    pUI->m_fMemoryRotAngle = pUI->m_fRotAngle;
    pUI->m_memoryCol = col;

    // テクスチャ情報から、アニメーションの有無等を取得
    CTexture *pTexture = CManager::GetTexture();
    pUI->m_nAnimPattern = pTexture->GetInfo(pUI->m_nTexType)->nPattern;
    pUI->m_nAnimSpeed = pTexture->GetInfo(pUI->m_nTexType)->nSpeed;
    pUI->m_bRepeat = pTexture->GetInfo(pUI->m_nTexType)->bRepeat;

    return pUI;
}

//=========================================================
// UIの設置
// Author : 後藤慎之助
//=========================================================
void CUI::Place(SET set)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[1024];	// 文字として読み取り用
    char cHeadText[1024];	// 文字の判別用
    char cDie[1024];		// 使わない文字 
    int nBool = 0;          // intからboolへの橋渡し

    // ファイルを開く
    switch (set)
    {
    case SET_TITLE:
        pFile = fopen("data/TXT/ui_title.txt", "r");
        break;
    case SET_MANUAL:
        pFile = fopen("data/TXT/ui_manual.txt", "r");
        break;
    case SET_CUSTOM:
        pFile = fopen("data/TXT/ui_custom.txt", "r");
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
                    // 生成時に結びつけるもの
                    int nTexType = 0;                                   // 画像番号
                    D3DXVECTOR3 pos = DEFAULT_VECTOR;                   // 位置
                    D3DXVECTOR3 size = DEFAULT_VECTOR;                  // 大きさ
                    int nRot = 0;                                       // 角度
                    D3DXCOLOR col = DEFAULT_COLOR;                      // 色
                    bool bAddBrend = false;                             // 加算合成
                    bool bUseZBuffer = false;                           // 3Dモデルの後ろに出すかどうか
                    int nAlphaTestBorder = DEFAULT_ALPHATEST_BORDER_2D; // アルファテストのボーダー
                    int nIndexAction = 0;                               // アクションのインデックス    
                    ActionInfo aActionInfo[MAX_ACTION] = {};            // アクションの情報
                    memset(aActionInfo, 0, sizeof(aActionInfo));
                    memset(aActionInfo->afParam, 0, sizeof(aActionInfo->afParam));

                    // cHeadTextがEND_UISETになるまで
                    while (strcmp(cHeadText, "END_UISET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nTexType);

                            // 不正な値は全てデフォルトを呼び出す
                            if (nTexType < 0 || nTexType >= MAX_TEXTURE)
                            {
                                nTexType = 0;
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
                        else if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nRot);
                        }
                        else if (strcmp(cHeadText, "COL") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie, &col.r, &col.g, &col.b, &col.a);
                        }
                        else if (strcmp(cHeadText, "ADD_BLEND") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                bAddBrend = false;
                            }
                            else
                            {
                                bAddBrend = true;
                            }
                        }
                        else if (strcmp(cHeadText, "USE_ZBUFFER") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                bUseZBuffer = false;
                            }
                            else
                            {
                                bUseZBuffer = true;
                            }
                        }
                        else if (strcmp(cHeadText, "ALPHA_TEST_BORDER") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nAlphaTestBorder);
                        }
                        else if (strcmp(cHeadText, "ACTION0") == 0)
                        {
                            nIndexAction = 0;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // 不正な値は全てデフォルトを呼び出す
                            if (aActionInfo[nIndexAction].action < ACTION_NONE || aActionInfo[nIndexAction].action >= ACTION_MAX)
                            {
                                aActionInfo[nIndexAction].action = ACTION_NONE;
                            }
                        }
                        else if (strcmp(cHeadText, "ACTION1") == 0)
                        {
                            nIndexAction = 1;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // 不正な値は全てデフォルトを呼び出す
                            if (aActionInfo[nIndexAction].action < ACTION_NONE || aActionInfo[nIndexAction].action >= ACTION_MAX)
                            {
                                aActionInfo[nIndexAction].action = ACTION_NONE;
                            }
                        }
                        else if (strcmp(cHeadText, "ACTION2") == 0)
                        {
                            nIndexAction = 2;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // 不正な値は全てデフォルトを呼び出す
                            if (aActionInfo[nIndexAction].action < ACTION_NONE || aActionInfo[nIndexAction].action >= ACTION_MAX)
                            {
                                aActionInfo[nIndexAction].action = ACTION_NONE;
                            }
                        }
                        else if (strcmp(cHeadText, "ACTION3") == 0)
                        {
                            nIndexAction = 3;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // 不正な値は全てデフォルトを呼び出す
                            if (aActionInfo[nIndexAction].action < ACTION_NONE || aActionInfo[nIndexAction].action >= ACTION_MAX)
                            {
                                aActionInfo[nIndexAction].action = ACTION_NONE;
                            }
                        }
                        else if (strcmp(cHeadText, "LOCK") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                aActionInfo[nIndexAction].bLock = false;
                            }
                            else
                            {
                                aActionInfo[nIndexAction].bLock = true;
                            }
                        }
                        else if (strcmp(cHeadText, "PARAM0") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[0]);
                        }
                        else if (strcmp(cHeadText, "PARAM1") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[1]);
                        }
                        else if (strcmp(cHeadText, "PARAM2") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[2]);
                        }
                        else if (strcmp(cHeadText, "PARAM3") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[3]);
                        }
                        else if (strcmp(cHeadText, "PARAM4") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[4]);
                        }
                        else if (strcmp(cHeadText, "PARAM5") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[5]);
                        }
                        else if (strcmp(cHeadText, "PARAM6") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[6]);
                        }
                        else if (strcmp(cHeadText, "PARAM7") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &aActionInfo[nIndexAction].afParam[7]);
                        }
                    }

                    // 生成（アクションの情報も結びつける）
                    CUI* pUI = Create(nTexType, pos, size, nRot, col, bAddBrend, nAlphaTestBorder, bUseZBuffer);
                    for (int nCnt = 0; nCnt < MAX_ACTION; nCnt++)
                    {
                        pUI->SetActionInfo(nCnt, aActionInfo[nCnt].action, aActionInfo[nCnt].bLock,
                            aActionInfo[nCnt].afParam[0], aActionInfo[nCnt].afParam[1], aActionInfo[nCnt].afParam[2], aActionInfo[nCnt].afParam[3],
                            aActionInfo[nCnt].afParam[4], aActionInfo[nCnt].afParam[5], aActionInfo[nCnt].afParam[6], aActionInfo[nCnt].afParam[7]);
                    }
#ifdef _DEBUG
                    pUI->SetReloadUI();
#endif
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

//=========================================================
// UIの動き情報を設定
// Author : 後藤慎之助
//=========================================================
void CUI::SetActionInfo(int nNum, int action, bool bLock, float fParam0, float fParam1, float fParam2, float fParam3, float fParam4, float fParam5, float fParam6, float fParam7)
{
    m_aActionInfo[nNum].action = action;  
    m_aActionInfo[nNum].nCntTime = 0;
    m_aActionInfo[nNum].bLock = bLock;
    m_aActionInfo[nNum].afParam[0] = fParam0;
    m_aActionInfo[nNum].afParam[1] = fParam1;
    m_aActionInfo[nNum].afParam[2] = fParam2;
    m_aActionInfo[nNum].afParam[3] = fParam3;
    m_aActionInfo[nNum].afParam[4] = fParam4;
    m_aActionInfo[nNum].afParam[5] = fParam5;
    m_aActionInfo[nNum].afParam[6] = fParam6;
    m_aActionInfo[nNum].afParam[7] = fParam7;

    // テクスチャブレンドアクションなら、ここで用いるテクスチャをバインド
    if (m_aActionInfo[nNum].action == ACTION_TEX_BREND)
    {
        // ブレンド方法
        CScene2D::BREND brend = CScene2D::BREND_NORMAL;
        switch ((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_HOW_TO])
        {
        case CScene2D::BREND_NORMAL:
            brend = CScene2D::BREND_NORMAL;
            break;
        case CScene2D::BREND_SEAL:
            brend = CScene2D::BREND_SEAL;
            break;
        }

        // ブレンド用のテクスチャをバインド
        m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_IDX]
            = (float)BindTexture((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_TEX_NUMBER], brend);
    }

    // 記憶用変数に結びつける
    m_aActionInfo[nNum].bMemoryLock = m_aActionInfo[nNum].bLock;
    for (int nCnt = 0; nCnt < MAX_ACTION_PARAM; nCnt++)
    {
        m_aActionInfo[nNum].afMemoryParam[nCnt] = m_aActionInfo[nNum].afParam[nCnt];
    }
}

//=========================================================
// UIのアクションのロック状態を設定
// Author : 後藤慎之助
//=========================================================
void CUI::SetActionLock(int nNum, bool bLock)
{
    if (nNum >= 0 && nNum < MAX_ACTION)
    {
        m_aActionInfo[nNum].bLock = bLock;
    }
}

//=========================================================
// UIの一部のアクション状況をリセット
// Author : 後藤慎之助
//=========================================================
void CUI::SetActionReset(int nNum)
{
    // アクションによって、リセットするものを変える
    switch (m_aActionInfo[nNum].action)
    {
    case ACTION_SIZE:
        CScene2D::SetSize(m_memorySize);
        break;
    case ACTION_POS:
        CScene2D::SetPosition(m_memoryPos);
        break;
    case ACTION_ALPHA:
        m_col.a = m_memoryCol.a;
        break;
    case ACTION_COLOR:
        m_col = m_memoryCol;
        break;
    case ACTION_ROT:
        m_fRotAngle = m_fMemoryRotAngle;
        break;
    }

    // 構造体の内容をリセット
    m_aActionInfo[nNum].nCntTime = 0;
    m_aActionInfo[nNum].bLock = m_aActionInfo[nNum].bMemoryLock;
    for (int nCntParam = 0; nCntParam < MAX_ACTION_PARAM; nCntParam++)
    {
        m_aActionInfo[nNum].afParam[nCntParam] = m_aActionInfo[nNum].afMemoryParam[nCntParam];
    }
}

//=========================================================
// UIの全てのアクション状況をリセット
// Author : 後藤慎之助
//=========================================================
void CUI::SetAllActionReset(void)
{
    // 記憶しておいたものを結びつける
    for (int nCntInfo = 0; nCntInfo < MAX_ACTION; nCntInfo++)
    {
        // アクションによって、リセットするものを変える
        switch (m_aActionInfo[nCntInfo].action)
        {
        case ACTION_SIZE:
            CScene2D::SetSize(m_memorySize);
            break;
        case ACTION_POS:
            CScene2D::SetPosition(m_memoryPos);
            break;
        case ACTION_ALPHA:
            m_col.a = m_memoryCol.a;
            break;
        case ACTION_COLOR:
            m_col = m_memoryCol;
            break;
        case ACTION_ROT:
            m_fRotAngle = m_fMemoryRotAngle;
            break;
        }

        // 構造体の内容をリセット
        m_aActionInfo[nCntInfo].nCntTime = 0;
        m_aActionInfo[nCntInfo].bLock = m_aActionInfo[nCntInfo].bMemoryLock;
        for (int nCntParam = 0; nCntParam < MAX_ACTION_PARAM; nCntParam++)
        {
            m_aActionInfo[nCntInfo].afParam[nCntParam] = m_aActionInfo[nCntInfo].afMemoryParam[nCntParam];
        }
    }
}

//=========================================================
// 全てのアクションを実行するもとの処理
// Author : 後藤慎之助
//=========================================================
void CUI::PlayAction(int nNum)
{
    // ロックされていないなら
    if (!m_aActionInfo[nNum].bLock)
    {
        // アクションを実行する
        switch (m_aActionInfo[nNum].action)
        {
        case ACTION_SIZE:
            PlayActionSize(nNum);
            break;
        case ACTION_POS:
            PlayActionPos(nNum);
            break;
        case ACTION_ALPHA:
            PlayActionAlpha(nNum);
            break;
        case ACTION_COLOR:
            PlayActionColor(nNum);
            break;
        case ACTION_ROT:
            PlayActionRot(nNum);
            break;
        case ACTION_TEX_BREND:
            PlayActionTexBrend(nNum);
            break;
        }
    }
}

//=========================================================
// サイズアクション
// Author : 後藤慎之助
//=========================================================
void CUI::PlayActionSize(int nNum)
{
    // 大きさを取得
    D3DXVECTOR3 size = CScene2D::GetSize();

    // 制限を考慮
    bool bUpdate = false;   // 更新するかどうか
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_SIZE_RIMIT])
    {
    case RIMIT_NONE:
        bUpdate = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_SIZE_FRAME])
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
            bUpdate = true;
        }
        break;

    case RIMIT_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_SIZE_FRAME])
        {
            bUpdate = true;
        }
        else
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        // 拡大中なら
        if (m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] > 0)
        {
            // 指定値より小さいなら拡大
            if (size.x < m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE])
            {
                bUpdate = true;
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] < 0)
        {
            if (size.x > m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE])
            {
                bUpdate = true;
            }
        }
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_SIZE_FRAME])
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
            bUpdate = true;
        }
        else
        {
            // カウンタリセットし、変化量を反転させる
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        // 拡大中なら
        if (m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] > 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memorySize.x < m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE])
            {
                // 「指定値」より小さいなら拡大
                if (size.x < m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE])
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] *= -1;
                }
            }
            else
            {
                // 「記憶した位置」より小さいなら拡大
                if (size.x < m_memorySize.x)
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] *= -1;
                }
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] < 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memorySize.x < m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE])
            {
                // 「記憶した位置」より大きいなら縮小
                if (size.x > m_memorySize.x)
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] *= -1;
                }
            }
            else
            {
                // 「指定値」より大きいなら縮小
                if (size.x > m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE])
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] *= -1;
                }
            }
        }
        break;

    case RIMIT_EQUAL_VALUE:
        size.y = (size.x / size.y) * m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE];
        size.x = m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE];
        break;
    }

    // 更新
    if (bUpdate)
    {
        // 大きさの変化量を加算（x基準で加算しているため、yを先に計算しないとリピートでずれる）
        if (size.y != 0.0f)
        {
            size.y += (size.x / size.y) * m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X];
        }
        size.x += m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X];
    }

    // 大きさを設定
    CScene2D::SetSize(size);
}

//=========================================================
// 位置アクション
// Author : 後藤慎之助
//=========================================================
void CUI::PlayActionPos(int nNum)
{
    // 位置を取得
    D3DXVECTOR3 pos = CScene2D::GetPosition();

    // 制限を考慮
    bool bUpdateX = false;   // X更新するかどうか
    bool bUpdateY = false;   // Y更新するかどうか
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_POS_RIMIT])
    {
    case RIMIT_NONE:
        bUpdateX = true;
        bUpdateY = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_POS_FRAME])
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
            bUpdateX = true;
            bUpdateY = true;
        }
        break;

    case RIMIT_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_POS_FRAME])
        {
            bUpdateX = true;
            bUpdateY = true;
        }
        else
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        // X正方向に移動中なら
        if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] > 0)
        {
            // 指定値より小さいなら移動
            if (pos.x < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X])
            {
                bUpdateX = true;
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] < 0)
        {
            if (pos.x > m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X])
            {
                bUpdateX = true;
            }
        }

        // Y正方向に移動中なら
        if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] > 0)
        {
            // 指定値より小さいなら移動
            if (pos.y < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y])
            {
                bUpdateY = true;
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] < 0)
        {
            if (pos.y > m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y])
            {
                bUpdateY = true;
            }
        }
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_POS_FRAME])
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
            bUpdateX = true;
            bUpdateY = true;
        }
        else
        {
            // カウンタリセットし、変化量を反転させる
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= -1;
            m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        // X正方向に移動中なら
        if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] > 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memoryPos.x < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X])
            {
                // 「指定値」より小さいなら移動
                if (pos.x < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X])
                {
                    bUpdateX = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= -1;
                }
            }
            else
            {
                // 「記憶した位置」より小さいなら移動
                if (pos.x < m_memoryPos.x)
                {
                    bUpdateX = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= -1;
                }
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] < 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memoryPos.x < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X])
            {
                // 「記憶した位置」より大きいなら移動
                if (pos.x > m_memoryPos.x)
                {
                    bUpdateX = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= -1;
                }
            }
            else
            {
                // 「指定値」より大きいなら移動
                if (pos.x > m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X])
                {
                    bUpdateX = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= -1;
                }
            }
        }

        // Y正方向に移動中なら
        if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] > 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memoryPos.y < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y])
            {
                // 「指定値」より小さいなら移動
                if (pos.y < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y])
                {
                    bUpdateY = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= -1;
                }
            }
            else
            {
                // 「記憶した位置」より小さいなら移動
                if (pos.y < m_memoryPos.y)
                {
                    bUpdateY = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= -1;
                }
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] < 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memoryPos.y < m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y])
            {
                // 「記憶した位置」より大きいなら移動
                if (pos.y > m_memoryPos.y)
                {
                    bUpdateY = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= -1;
                }
            }
            else
            {
                // 「指定値」より大きいなら移動
                if (pos.y > m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y])
                {
                    bUpdateY = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= -1;
                }
            }
        }
        break;

    case RIMIT_EQUAL_VALUE:
        pos.x = m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X];
        pos.y = m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y];
        break;
    }

    // X更新
    if (bUpdateX)
    {
        pos.x += m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X];
        m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= m_aActionInfo[nNum].afParam[PARAM_POS_ACCEL];
    }

    // Y更新
    if (bUpdateY)
    {
        pos.y += m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y];
        m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= m_aActionInfo[nNum].afParam[PARAM_POS_ACCEL];
    }

    // 位置を設定
    CScene2D::SetPosition(pos);
}

//=========================================================
// 透明度アクション
// Author : 後藤慎之助
//=========================================================
void CUI::PlayActionAlpha(int nNum)
{
    // 制限を考慮
    bool bUpdate = false;   // 更新するかどうか
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_RIMIT])
    {
    case RIMIT_NONE:
        bUpdate = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_FRAME])
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
            bUpdate = true;
        }
        break;

    case RIMIT_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_FRAME])
        {
            bUpdate = true;
        }
        else
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        // 加算中なら
        if (m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] > 0)
        {
            // 指定値より小さいなら加算
            if (m_col.a < m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE])
            {
                bUpdate = true;
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] < 0)
        {
            if (m_col.a > m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE])
            {
                bUpdate = true;
            }
        }
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_FRAME])
        {
            // カウンタ加算
            m_aActionInfo[nNum].nCntTime++;
            bUpdate = true;
        }
        else
        {
            // カウンタリセットし、変化量を反転させる
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        // 加算中なら
        if (m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] > 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memoryCol.a < m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE])
            {
                // 「指定値」より小さいなら加算
                if (m_col.a < m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE])
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] *= -1;
                }
            }
            else
            {
                // 「記憶した位置」より小さいなら加算
                if (m_col.a < m_memoryCol.a)
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] *= -1;
                }
            }
        }
        else if (m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] < 0)
        {
            // 記憶した値が指定値より小さいなら
            if (m_memoryCol.a < m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE])
            {
                // 「記憶した位置」より大きいなら減算
                if (m_col.a > m_memoryCol.a)
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] *= -1;
                }
            }
            else
            {
                // 「指定値」より大きいなら減算
                if (m_col.a > m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE])
                {
                    bUpdate = true;
                }
                else
                {
                    // 変化量を反転
                    m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] *= -1;
                }
            }
        }
        break;

    case RIMIT_EQUAL_VALUE:
        m_col.a = m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE];
        break;
    }

    // 更新
    if (bUpdate)
    {
        m_col.a += m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE];
    }
}

//=========================================================
// 色変えアクション
// Author : 後藤慎之助
//=========================================================
void CUI::PlayActionColor(int nNum)
{
}

//=========================================================
// 回転アクション
// Author : 後藤慎之助
//=========================================================
void CUI::PlayActionRot(int nNum)
{
}

//=========================================================
// テクスチャブレンドアクション
// Author : 後藤慎之助
//=========================================================
void CUI::PlayActionTexBrend(int nNum)
{
    // インターバル時間をカウントダウン
    if (m_aActionInfo[nNum].nCntTime > 0)
    {
        m_aActionInfo[nNum].nCntTime--;
    }
    else
    {
        // 変数宣言
        bool bRightToLeft = false;
        CScene2D::DIRECT direct = CScene2D::DIRECT_VERTICAL;

        // 右から左か
        if ((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_RIGHT_TO_LEFT] == 0)
        {
            bRightToLeft = false;
        }
        else if ((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_RIGHT_TO_LEFT] == 1)
        {
            bRightToLeft = true;
        }

        // 向き
        switch ((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_DIRECT])
        {
        case CScene2D::DIRECT_VERTICAL:
            direct = CScene2D::DIRECT_VERTICAL;
            break;
        case CScene2D::DIRECT_HORIZON:
            direct = CScene2D::DIRECT_HORIZON;
            break;
        case CScene2D::DIRECT_RIGHT_UP:
            direct = CScene2D::DIRECT_RIGHT_UP;
            break;
        case CScene2D::DIRECT_RIGHT_DOWN:
            direct = CScene2D::DIRECT_RIGHT_DOWN;
            break;
        }

        // アニメーションさせる
        if (SetFlowingAnimation(1, (int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_ONE_ROUND_FRAME],
            bRightToLeft, direct, (int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_IDX]))
        {
            // 一周したら、インターバル
            m_aActionInfo[nNum].nCntTime = (int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_INTERVAL_FRAME];
        }
    }
}
