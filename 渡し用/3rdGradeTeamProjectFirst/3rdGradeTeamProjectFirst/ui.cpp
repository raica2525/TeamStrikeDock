//=============================================================================
//
// UI (ui.cpp)
// Author : ã¡TV
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//=======================================
// CN[ht@C
//=======================================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "texture.h"
#include "effect2d.h"

//=======================================
// ÃIoÏé¾
//=======================================
CUI *CUI::m_apAccessUI[] = {};

//=========================================================
// UIÌRXgN^
// Author : ã¡TV
//=========================================================
CUI::CUI(CScene::OBJTYPE objtype) :CScene2D(objtype)
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
    m_nAnimParagraph = 0;
    m_nAnimPattern = 0;
    m_nAnimSpeed = 0;
    m_bRepeat = false;

    m_collisionPos = DEFAULT_VECTOR;
    m_collisionSize = DEFAULT_VECTOR;
    m_nAccessNum = NOT_EXIST;
    m_bDisp = true;
}

//=========================================================
// UIÌfXgN^
// Author : ã¡TV
//=========================================================
CUI::~CUI()
{

}

//=========================================================
// UIÌú»
// Author : ã¡TV
//=========================================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ú»
    CScene2D::Init(pos, size);

    // eNX`ðèÄ
    BindTexture(m_nTexType);

    return S_OK;
}

//=========================================================
// UIÌI¹
// Author : ã¡TV
//=========================================================
void CUI::Uninit(void)
{
    // ANZX ðÁÄ¢éUIÍA»êðjü·é
    if (m_nAccessNum > NOT_EXIST && m_nAccessNum < MAX_ACCESS_NUM)
    {
        m_apAccessUI[m_nAccessNum] = NULL;
    }

    // I¹
    CScene2D::Uninit();
}

//=========================================================
// UIÌXV
// Author : ã¡TV
//=========================================================
void CUI::Update(void)
{
    // Aj[Vðg¤Èç
    if (m_nAnimPattern > 1)
    {
        // êüµÄ¢È¢ÈçeNX`Aj[VðXV
        if (!m_bOneRoundAnim)
        {
            // s[g·éÈçAêüÌtOÆÑÂ¯È¢
            if (m_bRepeat)
            {
                if (m_nAnimParagraph > 2)
                {
                    CScene2D::SetAllParagraphAnimation(m_nAnimParagraph, m_nAnimSpeed, m_nAnimPattern);
                }
                else
                {
                    CScene2D::SetAnimation(m_nAnimSpeed, m_nAnimPattern);
                }
            }
            else
            {
                if (m_nAnimParagraph > 2)
                {
                    m_bOneRoundAnim = CScene2D::SetAllParagraphAnimation(m_nAnimParagraph, m_nAnimSpeed, m_nAnimPattern);
                }
                else
                {
                    m_bOneRoundAnim = CScene2D::SetAnimation(m_nAnimSpeed, m_nAnimPattern);
                }
            }
        }
    }

    // ANV
    for (int nCnt = 0; nCnt < MAX_ACTION; nCnt++)
    {
        PlayAction(nCnt);
    }
    
    // e¸_ðXVipxà½fjiQ[WnÍAÊÅ¸_îñð²®ÏÝj
    CScene2D::SetColor(m_col);
    if (m_aActionInfo[0].action != ACTION_GAUGE && m_aActionInfo[1].action != ACTION_GAUGE &&
        m_aActionInfo[2].action != ACTION_GAUGE && m_aActionInfo[3].action != ACTION_GAUGE)
    {
        CScene2D::SetRotVertex(m_fRotAngle);
    }

    // §¾Aå«³ªÈ¢ÈçgptOðfalseÉ
    D3DXVECTOR3 size = CScene2D::GetSize();
    if (m_col.a < 0.0f || size.x < 0.0f || size.y < 0.0f)
    {
        m_bUse = false;
    }
    
    // gptOªÈ¢ÈçI¹iãLÌgptOðfalseÉÈOÉàAZb^[©çfalseÉÅ«é½ß±Ìè@j
    if (!m_bUse)
    {
        Uninit();
    }
}

//=========================================================
// UIÌ`æ
// Author : ã¡TV
//=========================================================
void CUI::Draw(void)
{
    // \¦·éÈç
    if (m_bDisp)
    {
        // ÁZ¬
        if (m_bUseAdditiveSynthesis)
        {
            CScene2D::SetAdditiveSynthesis();
        }

        // `æ
        CScene2D::Draw();
    }
}

//=========================================================
// UIÌì¬
// Author : ã¡TV
//=========================================================
CUI *CUI::Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, int nRotAngle, D3DXCOLOR col,
    bool bFrontText, bool bUseAddiveSynthesis, int nAlphaTestBorder, bool bUseZBuffer,  D3DXVECTOR3 collisionPos, D3DXVECTOR3 collisionSize)
{
    // ÌmÛ
    CUI *pUI = NULL;

    // ^Cv²ÆÉAeLXgÌèO©ãëÌUI©Ç¤©ðßé
    if (bFrontText)
    {
        pUI = new CUI(CScene::OBJTYPE_UI_FRONT_TEXT);
    }
    else
    {
        pUI = new CUI(CScene::OBJTYPE_UI_BACK_TEXT);
    }

    // eNX`Ì^CvÍæÉÑÂ¯Ä¨­
    pUI->m_nTexType = nTexType;

    // ú»i3DfÌ És­ÈçAZBufferðl¶·éj
    float fZBufferValue = 0.0f;
    if (bUseZBuffer)
    {
        fZBufferValue = 1.0f;
    }
    pos.z = fZBufferValue;
    pUI->Init(pos, size);

    // e³ÉÝèð½f
    pUI->CScene2D::SetAlphaTestBorder(nAlphaTestBorder);

    // øðoÏÉÑÂ¯é
    if (nRotAngle != 0)
    {
        pUI->m_fRotAngle = D3DXToRadian((float)nRotAngle);
    }
    pUI->m_col = col;
    pUI->m_bUseAdditiveSynthesis = bUseAddiveSynthesis;
    pUI->m_collisionPos = collisionPos;
    pUI->m_collisionSize = collisionSize;

    // L¯pàÑÂ¯é
    pUI->m_memoryPos = pos;
    pUI->m_memorySize = size;
    pUI->m_fMemoryRotAngle = pUI->m_fRotAngle;
    pUI->m_memoryCol = col;

    // eNX`îñ©çAAj[VÌL³ðæ¾
    CTexture *pTexture = CManager::GetTexture();
    pUI->m_nAnimParagraph = pTexture->GetInfo(pUI->m_nTexType)->nParagraph;
    pUI->m_nAnimPattern = pTexture->GetInfo(pUI->m_nTexType)->nPattern;
    pUI->m_nAnimSpeed = pTexture->GetInfo(pUI->m_nTexType)->nSpeed;
    pUI->m_bRepeat = pTexture->GetInfo(pUI->m_nTexType)->bRepeat;

#ifdef _DEBUG
    if (collisionSize != DEFAULT_VECTOR && CManager::GetRenderer()->GetDispFont())
    {
        CUI *pDebugCollision = Create(52, collisionPos, collisionSize, 0, DEFAULT_COLOR, true);
        pDebugCollision->SetReloadUI();
    }
#endif

    return pUI;
}

//=========================================================
// UIÌÝu
// Author : ã¡TV
//=========================================================
void CUI::Place(SET set)
{
    // t@C|Cg
    FILE *pFile = NULL;

    // Ïé¾
    char cReadText[1024];	// ¶ÆµÄÇÝæèp
    char cHeadText[1024];	// ¶Ì»Êp
    char cDie[1024];		// gíÈ¢¶ 
    int nBool = 0;          // int©çboolÖÌ´nµ

    // t@CðJ­
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
    case SET_MENU:
        pFile = fopen("data/TXT/ui_menu.txt", "r");
        break;
    }

    // J¯½ç
    if (pFile != NULL)
    {
        // SCRIPTÌ¶ª©Â©éÜÅ
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            // eLXg©çcReadTextª¶ðó¯æé
            fgets(cReadText, sizeof(cReadText), pFile);

            // cReedTextðcHeadTextÉi[
            sscanf(cReadText, "%s", &cHeadText);
        }

        // cHeadTextªSCRIPTÌ
        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            // cHeadTextªEND_SCRIPTÉÈéÜÅ
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {
                fgets(cReadText, sizeof(cReadText), pFile);
                sscanf(cReadText, "%s", &cHeadText);

                // cHeadTextªUISETÌ
                if (strcmp(cHeadText, "UISET") == 0)
                {
                    // ¶¬ÉÑÂ¯éàÌ
                    int nTexType = 0;                                   // æÔ
                    int nAccessNum = NOT_EXIST;                         // ANZXio[
                    D3DXVECTOR3 pos = DEFAULT_VECTOR;                   // Êu
                    D3DXVECTOR3 size = DEFAULT_VECTOR;                  // å«³
                    D3DXVECTOR3 collisionPos = DEFAULT_VECTOR;          // ½è»èÌÊu
                    D3DXVECTOR3 collisionSize = DEFAULT_VECTOR;         // ½è»èÌå«³
                    int nRot = 0;                                       // px
                    D3DXCOLOR col = DEFAULT_COLOR;                      // F
                    bool bAddBrend = false;                             // ÁZ¬
                    bool bUseZBuffer = false;                           // 3DfÌãëÉo·©Ç¤©
                    bool bFrontText = false;                            // eLXgæèàèO©Ç¤©
                    int nAlphaTestBorder = DEFAULT_ALPHATEST_BORDER_2D; // At@eXgÌ{[_[
                    bool bShaveTex = false;                             // [Ì1sNZíé©Ç¤©
                    bool bDisp = true;                                  // \¦·é©Ç¤©
                    int nIndexAction = 0;                               // ANVÌCfbNX    
                    ActionInfo aActionInfo[MAX_ACTION] = {};            // ANVÌîñ
                    memset(aActionInfo, 0, sizeof(aActionInfo));
                    memset(aActionInfo->afParam, 0, sizeof(aActionInfo->afParam));

                    // cHeadTextªEND_UISETÉÈéÜÅ
                    while (strcmp(cHeadText, "END_UISET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nTexType);

                            // s³ÈlÍSÄftHgðÄÑo·
                            if (nTexType < 0 || nTexType >= MAX_TEXTURE)
                            {
                                nTexType = 0;
                            }
                        }
                        else if (strcmp(cHeadText, "ACCESS_NUM") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nAccessNum);

                            // s³ÈlÍSÄftHgðÄÑo·
                            if (nAccessNum < 0 || nAccessNum >= MAX_ACCESS_NUM)
                            {
                                nAccessNum = NOT_EXIST;
                            }
                        }
                        else if (strcmp(cHeadText, "NO_DRAW") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 1)
                            {
                                bDisp = false;
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
                        else if (strcmp(cHeadText, "COLLISION_POS") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f", &cDie, &cDie, &collisionPos.x, &collisionPos.y);
                        }
                        else if (strcmp(cHeadText, "COLLISION_SIZE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f", &cDie, &cDie, &collisionSize.x, &collisionSize.y);
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
                        else if (strcmp(cHeadText, "FRONT_TEXT") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                bFrontText = false;
                            }
                            else
                            {
                                bFrontText = true;
                            }
                        }
                        else if (strcmp(cHeadText, "ALPHA_TEST_BORDER") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nAlphaTestBorder);
                        }
                        else if (strcmp(cHeadText, "SHAVE_TEX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                bShaveTex = false;
                            }
                            else
                            {
                                bShaveTex = true;
                            }
                        }
                        else if (strcmp(cHeadText, "ACTION0") == 0)
                        {
                            nIndexAction = 0;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // s³ÈlÍSÄftHgðÄÑo·
                            if (aActionInfo[nIndexAction].action < ACTION_NONE || aActionInfo[nIndexAction].action >= ACTION_MAX)
                            {
                                aActionInfo[nIndexAction].action = ACTION_NONE;
                            }
                        }
                        else if (strcmp(cHeadText, "ACTION1") == 0)
                        {
                            nIndexAction = 1;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // s³ÈlÍSÄftHgðÄÑo·
                            if (aActionInfo[nIndexAction].action < ACTION_NONE || aActionInfo[nIndexAction].action >= ACTION_MAX)
                            {
                                aActionInfo[nIndexAction].action = ACTION_NONE;
                            }
                        }
                        else if (strcmp(cHeadText, "ACTION2") == 0)
                        {
                            nIndexAction = 2;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // s³ÈlÍSÄftHgðÄÑo·
                            if (aActionInfo[nIndexAction].action < ACTION_NONE || aActionInfo[nIndexAction].action >= ACTION_MAX)
                            {
                                aActionInfo[nIndexAction].action = ACTION_NONE;
                            }
                        }
                        else if (strcmp(cHeadText, "ACTION3") == 0)
                        {
                            nIndexAction = 3;
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &aActionInfo[nIndexAction].action);

                            // s³ÈlÍSÄftHgðÄÑo·
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

                    // ¶¬iANVÌîñàÑÂ¯éj
                    CUI* pUI = Create(nTexType, pos, size, nRot, col, bFrontText, bAddBrend, nAlphaTestBorder, bUseZBuffer, collisionPos, collisionSize);
                    for (int nCnt = 0; nCnt < MAX_ACTION; nCnt++)
                    {
                        pUI->SetActionInfo(nCnt, aActionInfo[nCnt].action, aActionInfo[nCnt].bLock,
                            aActionInfo[nCnt].afParam[0], aActionInfo[nCnt].afParam[1], aActionInfo[nCnt].afParam[2], aActionInfo[nCnt].afParam[3],
                            aActionInfo[nCnt].afParam[4], aActionInfo[nCnt].afParam[5], aActionInfo[nCnt].afParam[6], aActionInfo[nCnt].afParam[7]);
                    }

                    // ANZX ðæ¾·é
                    if (nAccessNum > NOT_EXIST && nAccessNum < MAX_ACCESS_NUM)
                    {
                        pUI->SetAccessUI(nAccessNum);
                    }

                    // [Ì1sNZðíé©Ç¤©
                    if (bShaveTex)
                    {
                        pUI->SetShaveTex();
                    }

                    // \¦·é©Ç¤©ðÝè
                    pUI->SetDisp(bDisp);
#ifdef _DEBUG
                    pUI->SetReloadUI();
#endif
                }
            }
        }
        // t@CðÂ¶é
        fclose(pFile);
    }
    // J¯È©Á½ç
    else
    {
        printf("J¯êÜ¹ñÅµ½\n");
    }
}

//=========================================================
// UIÌANZX ðæ¾
// Author : ã¡TV
//=========================================================
CUI * CUI::GetAccessUI(int nNum)
{
    if (nNum > NOT_EXIST && nNum < MAX_ACCESS_NUM)
    {
        return m_apAccessUI[nNum];
    }

    return NULL;
}

//=========================================================
// UIÌANZX ðÝè
// Author : ã¡TV
//=========================================================
void CUI::SetAccessUI(int nNum)
{
    if (nNum > NOT_EXIST && nNum < MAX_ACCESS_NUM)
    {
        m_nAccessNum = nNum;
        m_apAccessUI[nNum] = this;
    }
}

//=========================================================
// UIÌ®«îñðÝè
// Author : ã¡TV
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

    // eNX`uhANVÈçA±±Åp¢éeNX`ðoCh
    if (m_aActionInfo[nNum].action == ACTION_TEX_BREND)
    {
        // uhû@
        CScene2D::BREND brend = CScene2D::BREND_NORMAL;
        switch ((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_HOW_TO])
        {
        case CScene2D::BREND_NORMAL:
            brend = CScene2D::BREND_NORMAL;
            break;
        case CScene2D::BREND_SEAL:
            brend = CScene2D::BREND_SEAL;
            break;
        case CScene2D::BREND_IGNORE_INFO:
            brend = CScene2D::BREND_APPLY_INFO;
            CScene2D::SetBrend(CScene2D::BREND_IGNORE_INFO);
            break;
        }

        // uhpÌeNX`ðoCh
        m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_IDX]
            = (float)BindTexture((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_TEX_NUMBER], brend);
    }
    else if (m_aActionInfo[nNum].action == ACTION_ROT)
    {
        // ñ]ANVÈçAlðWAÉ·é
        m_aActionInfo[nNum].afParam[PARAM_ROT_CHANGE_RATE] = D3DXToRadian(m_aActionInfo[nNum].afParam[PARAM_ROT_CHANGE_RATE]);
        m_aActionInfo[nNum].afParam[PARAM_ROT_VALUE] = D3DXToRadian(m_aActionInfo[nNum].afParam[PARAM_ROT_VALUE]);
    }
    else if (m_aActionInfo[nNum].action == ACTION_TEX_PLACE)
    {
        // `æ·éeNX`Ìêðwè
        CScene2D::SetTexturePlace((int)m_aActionInfo[nNum].afParam[PARAM_TEX_PLACE_PLACE], (int)m_aActionInfo[nNum].afParam[PARAM_TEX_PLACE_PATTERN]);
    }

    // L¯pÏÉÑÂ¯é
    m_aActionInfo[nNum].bMemoryLock = m_aActionInfo[nNum].bLock;
    for (int nCnt = 0; nCnt < MAX_ACTION_PARAM; nCnt++)
    {
        m_aActionInfo[nNum].afMemoryParam[nCnt] = m_aActionInfo[nNum].afParam[nCnt];
    }
}

//=========================================================
// UIÌANVÌbNóÔðÝè
// Author : ã¡TV
//=========================================================
void CUI::SetActionLock(int nNum, bool bLock)
{
    // ÍÍàÈç
    if (nNum >= 0 && nNum < MAX_ACTION)
    {
        m_aActionInfo[nNum].bLock = bLock;
    }
}

//=========================================================
// UIÌêÌANVóµðZbg
// Author : ã¡TV
//=========================================================
void CUI::SetActionReset(int nNum)
{
    // ÍÍàÈç
    if (nNum >= 0 && nNum < MAX_ACTION)
    {
        // \¢ÌÌàeðZbg
        m_aActionInfo[nNum].nCntTime = 0;
        m_aActionInfo[nNum].bLock = m_aActionInfo[nNum].bMemoryLock;
        for (int nCntParam = 0; nCntParam < MAX_ACTION_PARAM; nCntParam++)
        {
            m_aActionInfo[nNum].afParam[nCntParam] = m_aActionInfo[nNum].afMemoryParam[nCntParam];
        }

        // ANVÉæÁÄAZbg·éàÌðÏ¦é
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
        case ACTION_TEX_BREND:
            CScene2D::ResetCountAnim((int)m_aActionInfo[nNum].afMemoryParam[PARAM_TEX_BREND_IDX]);
            PlayActionTexBrend(nNum);   // eNX`ÀWXV
            break;
        case ACTION_LOOP_ANIM:
            CScene2D::ResetCountAnim();
            PlayActionLoopAnim(nNum);   // eNX`ÀWXV
            break;
        case ACTION_TEX_PLACE:
            CScene2D::SetTexturePlace((int)m_aActionInfo[nNum].afMemoryParam[PARAM_TEX_PLACE_PLACE], (int)m_aActionInfo[nNum].afMemoryParam[PARAM_TEX_PLACE_PATTERN]);
            break;
        }
    }
}

//=========================================================
// UIÌSÄÌANVóµðZbg
// Author : ã¡TV
//=========================================================
void CUI::SetAllActionReset(void)
{
    for (int nCnt = 0; nCnt < MAX_ACTION; nCnt++)
    {
        SetActionReset(nCnt);
    }
}

//=========================================================
// SÄÌANVðÀs·éàÆÌ
// Author : ã¡TV
//=========================================================
void CUI::PlayAction(int nNum)
{
    // bN³êÄ¢È¢Èç
    if (!m_aActionInfo[nNum].bLock)
    {
        // ANVðÀs·é
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
        case ACTION_LOOP_ANIM:
            PlayActionLoopAnim(nNum);
            break;
        case ACTION_EMIT_EFFECT:
            PlayActionEmitEffect(nNum);
            break;
        }
    }
}

//=========================================================
// TCYANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionSize(int nNum)
{
    // å«³ðæ¾
    D3DXVECTOR3 size = CScene2D::GetSize();

    // §Àðl¶
    bool bUpdateX = false;   // XðXV·é©Ç¤©
    bool bUpdateY = false;   // YðXV·é©Ç¤©
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_SIZE_RIMIT])
    {
    case RIMIT_NONE:
        bUpdateX = true;
        bUpdateY = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_SIZE_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdateX = true;
            bUpdateY = true;
        }
        break;

    case RIMIT_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_SIZE_FRAME])
        {
            bUpdateX = true;
            bUpdateY = true;
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X],
            size.x, m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_X], bUpdateX);
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_Y],
            size.y, m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_Y], bUpdateY);
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_SIZE_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdateX = true;
            bUpdateY = true;
        }
        else
        {
            // JE^ZbgµAÏ»Êð½]³¹é
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X] *= -1;
            m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_Y] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X],
            m_memorySize.x, size.x, m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_X], bUpdateX);
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_Y],
            m_memorySize.y, size.y, m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_Y], bUpdateY);
        break;

    case RIMIT_EQUAL_VALUE_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_SIZE_FRAME])
        {
            // äÏ`©Ç¤©
            if ((int)m_aActionInfo[nNum].afParam[PARAM_SIZE_EQUAL_RATIO] == 0)
            {
                size.x = m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_X];
                size.y = m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_Y];
            }
            else if ((int)m_aActionInfo[nNum].afParam[PARAM_SIZE_EQUAL_RATIO] == 1)
            {
                if (size.x != 0.0f)
                {
                    size.y = (size.y / size.x) * m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_X];
                }
                size.x = m_aActionInfo[nNum].afParam[PARAM_SIZE_VALUE_X];
            }
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;
    }

    // äÏ`©Ç¤©
    if ((int)m_aActionInfo[nNum].afParam[PARAM_SIZE_EQUAL_RATIO] == 0)
    {
        // XXV
        if (bUpdateX)
        {
            size.x += m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X];
        }

        // YXV
        if (bUpdateY)
        {
            size.y += m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_Y];
        }
    }
    else if ((int)m_aActionInfo[nNum].afParam[PARAM_SIZE_EQUAL_RATIO] == 1)
    {
        // äÏ`ÍxîÅÁZµÄ¢é½ßAyðæÉvZµÈ¢Æs[gÅ¸êé
        if (bUpdateX)
        {
            if (size.x != 0.0f)
            {
                size.y += (size.y / size.x) * m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X];
            }
            size.x += m_aActionInfo[nNum].afParam[PARAM_SIZE_CHANGE_RATE_X];
        }
    }

    // å«³ðÝè
    CScene2D::SetSize(size);
}

//=========================================================
// ÊuANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionPos(int nNum)
{
    // Êuðæ¾
    D3DXVECTOR3 pos = CScene2D::GetPosition();

    // §Àðl¶
    bool bUpdateX = false;   // XXV·é©Ç¤©
    bool bUpdateY = false;   // YXV·é©Ç¤©
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_POS_RIMIT])
    {
    case RIMIT_NONE:
        bUpdateX = true;
        bUpdateY = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_POS_FRAME])
        {
            // JE^ÁZ
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
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        // X
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X],
            pos.x, m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X], bUpdateX);
        // Y
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y],
            pos.y, m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y], bUpdateY);
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_POS_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdateX = true;
            bUpdateY = true;
        }
        else
        {
            // JE^ZbgµAÏ»Êð½]³¹é
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= -1;
            m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        // X
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X],
            m_memoryPos.x, pos.x, m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X], bUpdateX);
        // Y
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y],
            m_memoryPos.y, pos.y, m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y], bUpdateY);
        break;

    case RIMIT_EQUAL_VALUE_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_POS_FRAME])
        {
            pos.x = m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_X];
            pos.y = m_aActionInfo[nNum].afParam[PARAM_POS_VALUE_Y];
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;
    }

    // XXV
    if (bUpdateX)
    {
        pos.x += m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X];
        m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_X] *= m_aActionInfo[nNum].afParam[PARAM_POS_ACCEL];
    }

    // YXV
    if (bUpdateY)
    {
        pos.y += m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y];
        m_aActionInfo[nNum].afParam[PARAM_POS_CHANGE_RATE_Y] *= m_aActionInfo[nNum].afParam[PARAM_POS_ACCEL];
    }

    // ÊuðÝè
    CScene2D::SetPosition(pos);
}

//=========================================================
// §¾xANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionAlpha(int nNum)
{
    // §Àðl¶
    bool bUpdate = false;   // XV·é©Ç¤©
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_RIMIT])
    {
    case RIMIT_NONE:
        bUpdate = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_FRAME])
        {
            // JE^ÁZ
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
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE],
            m_col.a, m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE], bUpdate);
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdate = true;
        }
        else
        {
            // JE^ZbgµAÏ»Êð½]³¹é
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE],
            m_memoryCol.a, m_col.a, m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE], bUpdate);
        break;

    case RIMIT_EQUAL_VALUE_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_ALPHA_FRAME])
        {
            m_col.a = m_aActionInfo[nNum].afParam[PARAM_ALPHA_VALUE];
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;
    }

    // XV
    if (bUpdate)
    {
        m_col.a += m_aActionInfo[nNum].afParam[PARAM_ALPHA_CHANGE_RATE];
    }
}

//=========================================================
// FÏ¦ANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionColor(int nNum)
{
    // §Àðl¶
    bool bUpdateR = false;   // RXV·é©Ç¤©
    bool bUpdateG = false;   // GXV·é©Ç¤©
    bool bUpdateB = false;   // BXV·é©Ç¤©
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_COLOR_RIMIT])
    {
    case RIMIT_NONE:
        bUpdateR = true;
        bUpdateG = true;
        bUpdateB = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_COLOR_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdateR = true;
            bUpdateG = true;
            bUpdateB = true;
        }
        break;

    case RIMIT_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_COLOR_FRAME])
        {
            bUpdateR = true;
            bUpdateG = true;
            bUpdateB = true;
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        // R
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_R],
            m_col.r, m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_R], bUpdateR);
        // G
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_G],
            m_col.g, m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_G], bUpdateG);
        // B
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_B],
            m_col.b, m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_B], bUpdateB);
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_COLOR_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdateR = true;
            bUpdateG = true;
            bUpdateB = true;
        }
        else
        {
            // JE^ZbgµAÏ»Êð½]³¹é
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_R] *= -1;
            m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_G] *= -1;
            m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_B] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        // R
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_R],
            m_memoryCol.r, m_col.r, m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_R], bUpdateR);
        // G
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_G],
            m_memoryCol.g, m_col.g, m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_G], bUpdateG);
        // B
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_B],
            m_memoryCol.b, m_col.b, m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_B], bUpdateB);
        break;

    case RIMIT_EQUAL_VALUE_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_COLOR_FRAME])
        {
            m_col.r = m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_R];
            m_col.g = m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_G];
            m_col.b = m_aActionInfo[nNum].afParam[PARAM_COLOR_VALUE_B];
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;
    }

    // RXV
    if (bUpdateR)
    {
        m_col.r += m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_R];
    }

    // GXV
    if (bUpdateG)
    {
        m_col.g += m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_G];
    }

    // BXV
    if (bUpdateB)
    {
        m_col.b += m_aActionInfo[nNum].afParam[PARAM_COLOR_CHANGE_RATE_B];
    }
}

//=========================================================
// ñ]ANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionRot(int nNum)
{
    // Ïé¾
    const float ANGLE_ADJUST = D3DXToRadian(90.0f);   // vÅ¢¤0ª0xÌÉí¹éi2D|SÌ¸_ðZÅwèµÄ¢é©çj

    // §Àðl¶
    bool bUpdate = false;   // XV·é©Ç¤©
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_ROT_RIMIT])
    {
    case RIMIT_NONE:
        bUpdate = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_ROT_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdate = true;
        }
        break;

    case RIMIT_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_ROT_FRAME])
        {
            bUpdate = true;
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_TO_VALUE:
        RimitToValue(m_aActionInfo[nNum].afParam[PARAM_ROT_CHANGE_RATE],
            m_fRotAngle, m_aActionInfo[nNum].afParam[PARAM_ROT_VALUE] - ANGLE_ADJUST, bUpdate);
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_ROT_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bUpdate = true;
        }
        else
        {
            // JE^ZbgµAÏ»Êð½]³¹é
            m_aActionInfo[nNum].nCntTime = 0;
            m_aActionInfo[nNum].afParam[PARAM_ROT_CHANGE_RATE] *= -1;
        }
        break;

    case RIMIT_REPEAT_VALUE:
        RimitRepeatValue(m_aActionInfo[nNum].afParam[PARAM_ROT_CHANGE_RATE],
            m_fMemoryRotAngle, m_fRotAngle, m_aActionInfo[nNum].afParam[PARAM_ROT_VALUE] - ANGLE_ADJUST, bUpdate);
        break;

    case RIMIT_EQUAL_VALUE_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_ROT_FRAME])
        {
            m_fRotAngle = m_aActionInfo[nNum].afParam[PARAM_ROT_VALUE];
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;
    }

    // XV
    if (bUpdate)
    {
        m_fRotAngle += m_aActionInfo[nNum].afParam[PARAM_ROT_CHANGE_RATE];
    }

    // pxÌ²®
    if (m_fRotAngle > D3DXToRadian(180.0f) || m_fRotAngle < D3DXToRadian(-180.0f))
    {
        m_fRotAngle *= -1;
    }
}

//=========================================================
// eNX`uhANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionTexBrend(int nNum)
{
    // eNX`îñ©çAAj[VÌL³ðæ¾
    CTexture::Info *pTextureInfo = CManager::GetTexture()->GetInfo((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_TEX_NUMBER]);
    int nPattern = pTextureInfo->nPattern;
    int nAnimSpeed = pTextureInfo->nSpeed;

    // Aj[Vðg¤Èç
    if (nPattern > 1)
    {
        // textureÌûÅÝè³êÄ¢éàÌðg¤
        CScene2D::SetAnimation(nAnimSpeed, nPattern, (int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_IDX]);
    }
    else
    {
        // C^[oÔðJEg_E
        if (m_aActionInfo[nNum].nCntTime > 0)
        {
            m_aActionInfo[nNum].nCntTime--;
        }
        else
        {
            // Ïé¾
            bool bRightToLeft = false;
            CScene2D::DIRECT direct = CScene2D::DIRECT_VERTICAL;

            // E©ç¶©
            if ((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_RIGHT_TO_LEFT] == 0)
            {
                bRightToLeft = false;
            }
            else if ((int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_RIGHT_TO_LEFT] == 1)
            {
                bRightToLeft = true;
            }

            // ü«
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

            // Aj[V³¹é
            if (SetFlowingAnimation(1, (int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_ONE_ROUND_FRAME],
                bRightToLeft, direct, (int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_IDX]))
            {
                // êüµ½çAC^[o
                m_aActionInfo[nNum].nCntTime = (int)m_aActionInfo[nNum].afParam[PARAM_TEX_BREND_INTERVAL_FRAME];
            }
        }
    }
}

//=========================================================
// [vAj[VANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionLoopAnim(int nNum)
{
    // Ïé¾
    bool bRightToLeft = false;
    CScene2D::DIRECT direct = CScene2D::DIRECT_VERTICAL;

    // E©ç¶©
    if ((int)m_aActionInfo[nNum].afParam[PARAM_LOOP_ANIM_RIGHT_TO_LEFT] == 0)
    {
        bRightToLeft = false;
    }
    else if ((int)m_aActionInfo[nNum].afParam[PARAM_LOOP_ANIM_RIGHT_TO_LEFT] == 1)
    {
        bRightToLeft = true;
    }

    // ü«
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_LOOP_ANIM_DIRECT])
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

    // Aj[V³¹é
    SetFlowingAnimation(1, (int)m_aActionInfo[nNum].afParam[PARAM_LOOP_ANIM_ONE_ROUND_FRAME], bRightToLeft, direct);
}

//=========================================================
// GtFNg­¶ANV
// Author : ã¡TV
//=========================================================
void CUI::PlayActionEmitEffect(int nNum)
{
    // §Àðl¶ilÉÍÎµÄ¢È¢j
    bool bEmit = false;   // ­¶³¹é©Ç¤©
    switch ((int)m_aActionInfo[nNum].afParam[PARAM_EMIT_EFFECT_RIMIT])
    {
    case RIMIT_NONE:
        bEmit = true;
        break;

    case RIMIT_TO_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_EMIT_EFFECT_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
            bEmit = true;
        }
        break;

    case RIMIT_FROM_FRAME:
        if (m_aActionInfo[nNum].nCntTime >= (int)m_aActionInfo[nNum].afParam[PARAM_EMIT_EFFECT_FRAME])
        {
            bEmit = true;
        }
        else
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        break;

    case RIMIT_REPEAT_FRAME:
        if (m_aActionInfo[nNum].nCntTime < (int)m_aActionInfo[nNum].afParam[PARAM_EMIT_EFFECT_FRAME])
        {
            // JE^ÁZ
            m_aActionInfo[nNum].nCntTime++;
        }
        else
        {
            // GtFNgð­¶³¹AJE^Zbg
            bEmit = true;
            m_aActionInfo[nNum].nCntTime = 0;
        }
        break;
    }

    // GtFNg­¶
    if (bEmit)
    {
        // ­¶ÊuÍASÈOÉÏ¦é±ÆàÅ«é
        D3DXVECTOR3 emitPos = CScene2D::GetPosition() +
            D3DXVECTOR3(m_aActionInfo[nNum].afParam[PARAM_EMIT_EFFECT_ADJUST_EMIT_POS_X], 
                m_aActionInfo[nNum].afParam[PARAM_EMIT_EFFECT_ADJUST_EMIT_POS_Y], 0.0f);
        CEffect2D::Emit((int)m_aActionInfo[nNum].afParam[PARAM_EMIT_EFFECT_TYPE], emitPos, emitPos);
    }
}

//=========================================================
// ZlÜÅÌ¤Ê
// Author : ã¡TV
//=========================================================
void CUI::RimitToValue(const float fChangeRate, float& fCurrentValue, const float fDestValue, bool& bUpdate)
{
    // ÁZÈç
    if (fChangeRate > 0)
    {
        // ¡ñÌt[Åwèµ½lðI[o[µÈ¢ÈçAXV
        float fThisFrameValue = fCurrentValue + fChangeRate;
        if (fThisFrameValue < fDestValue)
        {
            bUpdate = true;
        }
        else
        {
            // wèµ½lÅ~ßÄ¨­
            fCurrentValue = fDestValue;
        }
    }
    else if (fChangeRate < 0)
    {
        // ¡ñÌt[Åwèµ½lðI[o[µÈ¢ÈçAXV
        float fThisFrameValue = fCurrentValue + fChangeRate;
        if (fThisFrameValue > fDestValue)
        {
            bUpdate = true;
        }
        else
        {
            // wèµ½lÅ~ßÄ¨­
            fCurrentValue = fDestValue;
        }
    }
}

//=========================================================
// ls[gÌ¤Ê
// Author : ã¡TV
//=========================================================
void CUI::RimitRepeatValue(float& fChangeRate, const float fMemoryValue, const float fCurrentValue, const float fDestValue, bool& bUpdate)
{
    // ÁZÈç
    if (fChangeRate > 0)
    {
        // L¯µ½lªÚIlæè¬³¢Èç
        if (fMemoryValue < fDestValue)
        {
            // »ÝÌlªÚIlæè¬³¢ÈçXV
            if (fCurrentValue < fDestValue)
            {
                bUpdate = true;
            }
            else
            {
                // Ï»Êð½]
                fChangeRate *= -1;
            }
        }
        else
        {
            // »ÝÌlªL¯µ½læè¬³¢ÈçXV
            if (fCurrentValue < fMemoryValue)
            {
                bUpdate = true;
            }
            else
            {
                // Ï»Êð½]
                fChangeRate *= -1;
            }
        }
    }
    else if (fChangeRate < 0)
    {
        // L¯µ½lªÚIlæè¬³¢Èç
        if (fMemoryValue < fDestValue)
        {
            // »ÝÌlªL¯µ½læèå«¢ÈçXV
            if (fCurrentValue > fMemoryValue)
            {
                bUpdate = true;
            }
            else
            {
                // Ï»Êð½]
                fChangeRate *= -1;
            }
        }
        else
        {
            // »ÝÌlªÚIlæèå«¢ÈçÚ®
            if (fCurrentValue > fDestValue)
            {
                bUpdate = true;
            }
            else
            {
                // Ï»Êð½]
                fChangeRate *= -1;
            }
        }
    }
}
