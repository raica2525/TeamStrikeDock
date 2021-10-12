//===============================================
//
// エフェクトデータ管理 (effectData.cpp)
// Author : v
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "effectData.h"
#include "effecttool.h"
#include "manager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EFFECT_DATA_FILENAME ("data/TXT/effect.txt")
#define EFFECT_DATA_OUTPUT_FILENAME ("data/TXT/effectinfo.txt")

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
// Author : 伊藤陽梧
//=============================================================================
CEffectData::~CEffectData()
{
}

//=============================================================================
// 初期化処理
// Author : 伊藤陽梧
//=============================================================================
HRESULT CEffectData::Init(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[256];	               // 文字として読み取り用
    char cHeadText[256];	               // 文字の判別用
    char cDie[256];		                   // 使わない文字
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
// Author : 伊藤陽梧
//=============================================================================
void CEffectData::Uninit(void)
{

}

//=============================================================================
// エフェクトの情報出力処理
// Author : 伊藤陽梧
// nNum : 出力するエフェクトの番号
//=============================================================================
HRESULT CEffectData::UploadEffectInfo(const int nNum)
{
	// ファイルポイント
	FILE *pFile = NULL;

	// 変数宣言
	int nNumType = 0;                      // タイプのナンバー
	int nBool = 0;                         // intからboolへの橋渡し

	// ファイルを開く
	pFile = fopen(EFFECT_DATA_OUTPUT_FILENAME, "w");

	// 開けたら
	if (pFile != NULL)
	{
		fprintf(pFile, "SCRIPT		# この行は絶対消さないこと！\n");
		fprintf(pFile, "\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# エフェクトの情報（F4でリロード可能）\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "\n");

		fprintf(pFile, "#-------------------------------------------------------------\n");
		fprintf(pFile, "# [%d] （エフェクト名）\n", nNum);
		fprintf(pFile, "#-------------------------------------------------------------\n");

		fprintf(pFile, "EFFECT_DATASET\n");

		fprintf(pFile, "	TYPE = %d\n", nNum);
		fprintf(pFile, "	TEX_TYPE = %d\n", m_aCreateInfo[nNum].nTexType);
		fprintf(pFile, "	ONCE = %d\n", m_aCreateInfo[nNum].nNumOnce);
		fprintf(pFile, "	PLANE = %d		# 0がXYで1がXZ\n", m_aCreateInfo[nNum].plane);
		fprintf(pFile, "	ON_CIRCLE = %d		# bool\n", m_aCreateInfo[nNum].bOnCircleEdge);
		fprintf(pFile, "	EMIT_INSIDE = %d		# bool\n", m_aCreateInfo[nNum].bEmitVectorInside);
		fprintf(pFile, "	SCATTER_ANGLE = %d	# 0～314\n", m_aCreateInfo[nNum].nScatterAngle);
		fprintf(pFile, "	SCATTER_WIDTH = %d	# 0～314\n", m_aCreateInfo[nNum].nScatterWidth);

		fprintf(pFile, "	SPEED_MAX = %d\n", m_aCreateInfo[nNum].nSpeedMax);
		fprintf(pFile, "	SPEED_MIN = %d\n", m_aCreateInfo[nNum].nSpeedMin);
		fprintf(pFile, "	SPEED_AA_MAX = %d\n", m_aCreateInfo[nNum].nAuxiliaryAxisSpeedMax);
		fprintf(pFile, "	SPEED_AA_MIN = %d\n", m_aCreateInfo[nNum].nAuxiliaryAxisSpeedMin);
		fprintf(pFile, "	ACCEL = %.3f	# 1.0倍が基本\n", m_aCreateInfo[nNum].fAcceleration);
		fprintf(pFile, "	GRAVITY = %.3f\n", m_aCreateInfo[nNum].fGravity);
		fprintf(pFile, "	GRAVITYLIMIT = %.3f\n", m_aCreateInfo[nNum].fGravityLimit);


		fprintf(pFile, "	ROT_LAY = %d		# bool\n", m_aCreateInfo[nNum].nLayRot);
		fprintf(pFile, "	ROT_EQUAL_MOVE = %d	# bool\n", m_aCreateInfo[nNum].bRotEqualMoveAngle);
		fprintf(pFile, "	ROT_SPEED_MAX = %d\n", m_aCreateInfo[nNum].nRotSpeedMax);
		fprintf(pFile, "	ROT_SPEED_MIN = %d\n", m_aCreateInfo[nNum].nRotSpeedMin);
		fprintf(pFile, "	COL = %.3f %.3f %.3f %.3f\n", m_aCreateInfo[nNum].col.r,
														  m_aCreateInfo[nNum].col.g,
														  m_aCreateInfo[nNum].col.b,
														  m_aCreateInfo[nNum].col.a);
		fprintf(pFile, "	COL_CHANGE = %.3f %.3f %.3f %.3f\n", m_aCreateInfo[nNum].colChangeRate.r,
																 m_aCreateInfo[nNum].colChangeRate.g,
																 m_aCreateInfo[nNum].colChangeRate.b,
																 m_aCreateInfo[nNum].colChangeRate.a);
		fprintf(pFile, "	FADE_OUT_RATE = %.3f	# アルファ値減衰(0ならサイズを減衰させる)\n", m_aCreateInfo[nNum].fFadeOutRate);
		fprintf(pFile, "	FADE_OUT_LOCK_COUNT = %d\n", m_aCreateInfo[nNum].nCntFadeOutLock);

		fprintf(pFile, "	SIZE_MAX = %d\n", m_aCreateInfo[nNum].nSizeMax);
		fprintf(pFile, "	SIZE_MIN = %d\n", m_aCreateInfo[nNum].nSizeMin);
		fprintf(pFile, "	SIZE_CHANGE = %.1f %.1f %.1f\n", m_aCreateInfo[nNum].sizeChangeRate.x,
															 m_aCreateInfo[nNum].sizeChangeRate.y,
															 m_aCreateInfo[nNum].sizeChangeRate.z);
		fprintf(pFile, "	EMITRADIUS = %.0f\n", m_aCreateInfo[nNum].fEmitRadius);

		fprintf(pFile, "	ADD_BLEND = %d		# bool\n", m_aCreateInfo[nNum].bUseAdditiveSynthesis);
		fprintf(pFile, "	USE_ZBUFFER = %d		# bool\n", m_aCreateInfo[nNum].bUseZBuffer);
		fprintf(pFile, "	ALPHA_TEST_BORDER = %d\n", m_aCreateInfo[nNum].nAlphaTestBorder);

		fprintf(pFile, "END_EFFECT_DATASET\n");

		fprintf(pFile, "\n\n\n");
		fprintf(pFile, "END_SCRIPT		# この行は絶対消さないこと！\n");
		// ファイルを閉じる
		fclose(pFile);
	}
	// 開けなかったら
	else
	{
		printf("開けれませんでした\n");

		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// エフェクト生成情報取得処理
// Author : 伊藤陽梧
//=============================================================================
CEffectData::CreateInfo * CEffectData::GetCreateInfo(const int nNum)
{
    if (nNum < TYPE_MAX && nNum >= 0)
    {
        if (&m_aCreateInfo[nNum] != NULL)
        {
            return &m_aCreateInfo[nNum];
        }
    }

    return NULL;
}

//=============================================================================
// エフェクト生成情報セット
// Author : 伊藤陽梧
//=============================================================================
void CEffectData::SetCreateInfo(const int nNum)
{
	CEffectTool *pEffectTool = CManager::GetEffectTool();
	CEffectTool::EffectInfo *pEffectInfo = pEffectTool->GetEffectInfo(nNum);

	m_aCreateInfo[nNum].nTexType = pEffectInfo->nTexType;								// 使うテクスチャの種類
	m_aCreateInfo[nNum].nNumOnce = pEffectInfo->nNumOnce;								// 同時発生数
	m_aCreateInfo[nNum].plane = pEffectInfo->nPlane;									// 平面

	m_aCreateInfo[nNum].bOnCircleEdge = pEffectInfo->bOnCircleEdge;						// 円の縁から放出するかどうか
	m_aCreateInfo[nNum].bEmitVectorInside = pEffectInfo->bEmitVectorInside;				// 放出方向を内側にするかどうか
	m_aCreateInfo[nNum].nScatterAngle = pEffectInfo->nScatterAngle;						// 飛散角度（0～314）
	m_aCreateInfo[nNum].nScatterWidth = pEffectInfo->nScatterWidth;						// 飛散幅（0～314）
	m_aCreateInfo[nNum].fEmitRadius = pEffectInfo->fEmitRadius;							// 半径の大きさ

	m_aCreateInfo[nNum].nSpeedMax = pEffectInfo->nSpeedMax;								// 最大速度
	m_aCreateInfo[nNum].nSpeedMin = pEffectInfo->nSpeedMin;								// 最小速度
	m_aCreateInfo[nNum].nAuxiliaryAxisSpeedMax = pEffectInfo->nAuxiliaryAxisSpeedMax;	// 補助軸の最大速度（2D→3Dで、立体感を出すためのもの）
	m_aCreateInfo[nNum].nAuxiliaryAxisSpeedMin = pEffectInfo->nAuxiliaryAxisSpeedMin;	// 補助軸の最小速度（2D→3Dで、立体感を出すためのもの）

	m_aCreateInfo[nNum].fAcceleration = pEffectInfo->fAcceleration;						// 加速度（1.0基準）
	m_aCreateInfo[nNum].fGravity = pEffectInfo->fGravity;								// 重力
	m_aCreateInfo[nNum].fGravityLimit = pEffectInfo->fGravityLimit;						// 重力制限

	m_aCreateInfo[nNum].nLayRot = pEffectInfo->nLayRot;									// ポリゴンそのものを横にするかどうか（trueならビルボード解除）
	m_aCreateInfo[nNum].bRotEqualMoveAngle = pEffectInfo->bRotEqualMoveAngle;			// 回転を移動の向きに合わせるか
	m_aCreateInfo[nNum].nRotSpeedMax = pEffectInfo->nRotSpeedMax;						// 最大回転速度
	m_aCreateInfo[nNum].nRotSpeedMin = pEffectInfo->nRotSpeedMin;						// 最小回転速度

	m_aCreateInfo[nNum].col =															// 色
		D3DXCOLOR(pEffectInfo->col[0], 
			pEffectInfo->col[1], 
			pEffectInfo->col[2], 
			pEffectInfo->col[3]);														
	m_aCreateInfo[nNum].colChangeRate =													// 色変化割合
		D3DXCOLOR(pEffectInfo->colChangeRate[0], 
			pEffectInfo->colChangeRate[1], 
			pEffectInfo->colChangeRate[2], 
			pEffectInfo->colChangeRate[3]);												
	m_aCreateInfo[nNum].fFadeOutRate = pEffectInfo->fFadeOutRate;						// α値を下げていく割合（色変化割合でも可能だが、明示的に用意した）
	m_aCreateInfo[nNum].nCntFadeOutLock = pEffectInfo->nCntFadeOutLock;					// α値の変化をロックするカウンタ

	m_aCreateInfo[nNum].nSizeMax = pEffectInfo->nSizeMax;								// 最大大きさ
	m_aCreateInfo[nNum].nSizeMin = pEffectInfo->nSizeMin;								// 最小大きさ
	m_aCreateInfo[nNum].sizeChangeRate =												// 拡大/縮小割合
		D3DXVECTOR3(pEffectInfo->fSizeChangeRate[0], 
			pEffectInfo->fSizeChangeRate[1],  
			pEffectInfo->fSizeChangeRate[2]);											
	m_aCreateInfo[nNum].nCntSizeChangeLock = pEffectInfo->nCntSizeChangeLock;				// 大きさをロックするカウンタ
	m_aCreateInfo[nNum].bSizeChangeStartOrFinish = pEffectInfo->bSizeChangeStartOrFinish;	// 大きさを変えるのが始まるか、終わるのか

	m_aCreateInfo[nNum].bUseAdditiveSynthesis = pEffectInfo->bUseAdditiveSynthesis;		// 加算合成にするかどうか
	m_aCreateInfo[nNum].bUseZBuffer = pEffectInfo->bUseZBuffer;							// Zバッファをつかうかどうか
	m_aCreateInfo[nNum].nAlphaTestBorder = pEffectInfo->nAlphaTestBorder;				// αテストのボーダー
}																																												