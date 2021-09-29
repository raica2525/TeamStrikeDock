//===============================================
//
// エフェクトデータ管理 (effectData.cpp)
// Author : 後藤慎之助
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "effectData.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EFFECT_DATA_FILENAME ("data/TXT/effect.txt")

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CEffectData::CEffectData()
{
    memset(m_aCreateInfo, 0, sizeof(m_aCreateInfo));
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CEffectData::~CEffectData()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CEffectData::Init(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[2048];	               // 文字として読み取り用
    char cHeadText[2048];	               // 文字の判別用
    char cDie[2048];		               // 使わない文字
    int nNumType = 0;                      // タイプのナンバー
    int nBool = 0;                         // intからboolへの橋渡し

    //======================================================================================
    // モデルデータファイルを開く
    pFile = fopen(EFFECT_DATA_FILENAME, "r");

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

                // cHeadTextがEFFECT_DATASETの時
                if (strcmp(cHeadText, "EFFECT_DATASET") == 0)
                {
                    // cHeadTextがEND_EFFECT_DATASETになるまで
                    while (strcmp(cHeadText, "END_EFFECT_DATASET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "TEX_TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nTexType);
                        }
                        else if (strcmp(cHeadText, "ONCE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nNumOnce);
                        }
                        else if (strcmp(cHeadText, "PLANE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].plane);
                        }
                        else if (strcmp(cHeadText, "ON_CIRCLE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bOnCircleEdge = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bOnCircleEdge = true;
                            }
                        }
                        else if (strcmp(cHeadText, "EMIT_INSIDE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bEmitVectorInside = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bEmitVectorInside = true;
                            }
                        }
                        else if (strcmp(cHeadText, "SCATTER_ANGLE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nScatterAngle);
                        }
                        else if (strcmp(cHeadText, "SCATTER_WIDTH") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nScatterWidth);
                        }
                        else if (strcmp(cHeadText, "SPEED_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSpeedMax);
                        }
                        else if (strcmp(cHeadText, "SPEED_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSpeedMin);
                        }
                        else if (strcmp(cHeadText, "SPEED_AA_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nAuxiliaryAxisSpeedMax);
                        }
                        else if (strcmp(cHeadText, "SPEED_AA_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nAuxiliaryAxisSpeedMin);
                        }
                        else if (strcmp(cHeadText, "ACCEL") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fAcceleration);
                        }
                        else if (strcmp(cHeadText, "GRAVITY") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fGravity);
                        }
                        else if (strcmp(cHeadText, "GRAVITY_LIMIT") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fGravityLimit);
                        }
                        else if (strcmp(cHeadText, "ROT_LAY") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nLayRot);
                        }
                        else if (strcmp(cHeadText, "ROT_EQUAL_MOVE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bRotEqualMoveAngle = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bRotEqualMoveAngle = true;
                            }
                        }
                        else if (strcmp(cHeadText, "ROT_SPEED_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nRotSpeedMax);
                        }
                        else if (strcmp(cHeadText, "ROT_SPEED_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nRotSpeedMin);
                        }
                        else if (strcmp(cHeadText, "COL") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie, 
                                &m_aCreateInfo[nNumType].col.r,
                                &m_aCreateInfo[nNumType].col.g,
                                &m_aCreateInfo[nNumType].col.b,
                                &m_aCreateInfo[nNumType].col.a);
                        }
                        else if (strcmp(cHeadText, "COL_CHANGE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie,
                                &m_aCreateInfo[nNumType].colChangeRate.r,
                                &m_aCreateInfo[nNumType].colChangeRate.g,
                                &m_aCreateInfo[nNumType].colChangeRate.b,
                                &m_aCreateInfo[nNumType].colChangeRate.a);
                        }
                        else if (strcmp(cHeadText, "FADE_OUT_RATE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fFadeOutRate);
                        }
                        else if (strcmp(cHeadText, "FADE_OUT_LOCK_TIME") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nCntFadeOutLock);
                        }
                        else if (strcmp(cHeadText, "SIZE_MAX") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSizeMax);
                        }
                        else if (strcmp(cHeadText, "SIZE_MIN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nSizeMin);
                        }
                        else if (strcmp(cHeadText, "SIZE_CHANGE") == 0)
                        {
                            sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
                                &m_aCreateInfo[nNumType].sizeChangeRate.x,
                                &m_aCreateInfo[nNumType].sizeChangeRate.y,
                                &m_aCreateInfo[nNumType].sizeChangeRate.z);
                        }
                        else if (strcmp(cHeadText, "SIZE_CHANGE_LOCK_TIME") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nCntSizeChangeLock);
                        }
                        else if (strcmp(cHeadText, "SIZE_CHANGE_START_OR_FINISH") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bSizeChangeStartOrFinish = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bSizeChangeStartOrFinish = true;
                            }
                        }
                        else if (strcmp(cHeadText, "EMITRADIUS") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aCreateInfo[nNumType].fEmitRadius);
                        }
                        else if (strcmp(cHeadText, "ADD_BLEND") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bUseAdditiveSynthesis = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bUseAdditiveSynthesis = true;
                            }
                        }
                        else if (strcmp(cHeadText, "USE_ZBUFFER") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aCreateInfo[nNumType].bUseZBuffer = false;
                            }
                            else
                            {
                                m_aCreateInfo[nNumType].bUseZBuffer = true;
                            }
                        }
                        else if (strcmp(cHeadText, "ALPHA_TEST_BORDER") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aCreateInfo[nNumType].nAlphaTestBorder);
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

        return E_FAIL;
    }
    //======================================================================================

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CEffectData::Uninit(void)
{

}

//=============================================================================
// エフェクト生成情報取得処理
// Author : 後藤慎之助
//=============================================================================
CEffectData::CreateInfo * CEffectData::GetCreateInfo(const int nNum)
{
    if (nNum < MAX_EFFECT_TYPE && nNum >= 0)
    {
        if (&m_aCreateInfo[nNum] != NULL)
        {
            return &m_aCreateInfo[nNum];
        }
    }

    return NULL;
}
