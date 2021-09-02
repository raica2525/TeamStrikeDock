//=============================================================================
//
// モデルデータの処理 [modelData.cpp]
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルード
//=============================================================================
#include "modelData.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MODEL_DATA_FILENAME ("data/TXT/modelData.txt")	    // モデルデータを読み込むファイルの名前
#define MODEL_POS_DEF_FILENAME ("data/TXT/modelPosDef.txt")	// モデルの初期位置データを読み込むファイルの名前
#define PARTS_LIST_FILENAME ("data/TXT/partsList.txt")	    // パーツリストを読み込むファイルの名前
#define PARTS_RATE_FILENAME ("data/TXT/partsRate.txt")	    // パーツレートを読み込むファイルの名前

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CModelData::CModelData()
{
    memset(m_aModelData, 0, sizeof(m_aModelData));
    memset(m_aModelData->apTexMat, 0, sizeof(m_aModelData->apTexMat));

    memset(m_aModelPosDef, 0, sizeof(m_aModelPosDef));
    memset(m_aModelPosDef->nIndex, 0, sizeof(m_aModelPosDef->nIndex));
    memset(m_aModelPosDef->nParent, 0, sizeof(m_aModelPosDef->nParent));
    memset(m_aModelPosDef->pos, 0, sizeof(m_aModelPosDef->pos));

    // パーツリストの、部位番号は-1で初期化（0番目は頭であり、カスタマイズ画面で存在しないパーツを参照しないため）
    memset(m_aPartsList, 0, sizeof(m_aPartsList));
    memset(m_aPartsList->afParam, 0, sizeof(m_aPartsList->afParam));
    for (int nCnt = 0; nCnt < MAX_PARTS_LIST; nCnt++)
    {
        m_aPartsList[nCnt].part = NOT_EXIST;
    }

    memset(m_aPartsRate, 0, sizeof(m_aPartsRate));
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CModelData::~CModelData()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CModelData::Init(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[2048];	               // 文字として読み取り用
    char cHeadText[2048];	               // 文字の判別用
    char cDie[2048];		               // 使わない文字
    int nNumType = 0;                      // タイプのナンバー
    char cLoadModelName[256];              // 読み込み時のモデル名

    // モデルを読み込むためのデバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //======================================================================================
    // モデルデータファイルを開く
    pFile = fopen(MODEL_DATA_FILENAME, "r");

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

                // cHeadTextがMODEL_DATASETの時
                if (strcmp(cHeadText, "MODEL_DATASET") == 0)
                {
                    // cHeadTextがEND_MODEL_DATASETになるまで
                    while (strcmp(cHeadText, "END_MODEL_DATASET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "NAME") == 0)
                        {
                            sscanf(cReadText, "%s %s %s", &cDie, &cDie, &cLoadModelName);

                            // モデルデータの読み込み
                            D3DXLoadMeshFromX(LPCSTR(cLoadModelName), D3DXMESH_SYSTEMMEM, pDevice, NULL, 
                                &m_aModelData[nNumType].pBuffMat, NULL, &m_aModelData[nNumType].nNumMat, &m_aModelData[nNumType].pMesh);

                            // マテリアルのポインタを取得
                            D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_aModelData[nNumType].pBuffMat->GetBufferPointer();

                            // マテリアルのテクスチャを結びつける
                            for (int nCntMat = 0; nCntMat < (int)m_aModelData[nNumType].nNumMat; nCntMat++)
                            {
                                D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_aModelData[nNumType].apTexMat[nCntMat]);
                            }
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

    // モデルの初期位置データ読み込み
    LoadModelPosDef();

    // パーツリスト読み込み
    LoadPartsList();

    // パーツレート読み込み
    LoadPartsRate();

    return S_OK;
}

//=============================================================================
// モデルの初期位置データ読み込み処理
// Author : 後藤慎之助
//=============================================================================
void CModelData::LoadModelPosDef(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[2048];	               // 文字として読み取り用
    char cHeadText[2048];	               // 文字の判別用
    char cDie[2048];		               // 使わない文字
    int nNumType = 0;                      // タイプのナンバー
    int nModelPosPartsSet = 0;             // モデルパーツセット

    // モデルの初期位置データファイルを開く
    pFile = fopen(MODEL_POS_DEF_FILENAME, "r");

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

                // cHeadTextがCHARACTERSETの時
                if (strcmp(cHeadText, "CHARACTERSET") == 0)
                {
                    // cHeadTextがEND_CHARACTERSETになるまで
                    while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);

                            // モデルパーツセットのリセット
                            nModelPosPartsSet = 0;
                        }
                        else if (strcmp(cHeadText, "PARTSSET") == 0)
                        {
                            // cHeadTextがEND_PARTSSETになるまで
                            while (strcmp(cHeadText, "END_PARTSSET") != 0)
                            {
                                fgets(cReadText, sizeof(cReadText), pFile);
                                sscanf(cReadText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "INDEX") == 0)
                                {
                                    sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aModelPosDef[nNumType].nIndex[nModelPosPartsSet]);
                                }
                                else if (strcmp(cHeadText, "PARENT") == 0)
                                {
                                    sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aModelPosDef[nNumType].nParent[nModelPosPartsSet]);
                                }
                                else if (strcmp(cHeadText, "POS") == 0)
                                {
                                    sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
                                        &m_aModelPosDef[nNumType].pos[nModelPosPartsSet].x, 
                                        &m_aModelPosDef[nNumType].pos[nModelPosPartsSet].y,
                                        &m_aModelPosDef[nNumType].pos[nModelPosPartsSet].z);

                                    // モデルパーツセットの加算
                                    nModelPosPartsSet++;

                                    // 念のため配列の最大数に行かないようにする
                                    if (nModelPosPartsSet > MAX_MODEL_PARTS_SET - 1)
                                    {
                                        nModelPosPartsSet = MAX_MODEL_PARTS_SET - 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // ファイルを閉じる
        fclose(pFile);
    }

    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        int n = m_aModelPosDef[0].nIndex[nCnt];
        int p = m_aModelPosDef[0].nParent[nCnt];
        D3DXVECTOR3 pos = m_aModelPosDef[0].pos[nCnt];
    }
}

//=============================================================================
// パーツリスト読み込み処理
// Author : 後藤慎之助
//=============================================================================
void CModelData::LoadPartsList(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[2048];	               // 文字として読み取り用
    char cHeadText[2048];	               // 文字の判別用
    char cDie[2048];		               // 使わない文字
    int nNumType = 0;                      // タイプのナンバー

    // パーツリストファイルを開く
    pFile = fopen(PARTS_LIST_FILENAME, "r");

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

                // cHeadTextがPARTS_PARAMの時
                if (strcmp(cHeadText, "PARTS_PARAM") == 0)
                {
                    // cHeadTextがEND_PARTS_PARAMになるまで
                    while (strcmp(cHeadText, "END_PARTS_PARAM") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "PART") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aPartsList[nNumType].part);
                        }
                        else if (strcmp(cHeadText, "NAME") == 0)
                        {
                            // sscanfでは、fscanfと違って日本語を上手く受け取れなかったので、必殺技や特殊能力と同様の形式で受け取ることにした
                            //sscanf(cReadText, "%s %s %s", &cDie, &cDie, &m_aPartsList[nNumType].cName);
                            CPlayer::CustomPartsName(m_aPartsList[nNumType].cName, nNumType);
                        }
                        else if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fAtk);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fDef);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fSpd);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fWei);
                        }
                        else if (strcmp(cHeadText, "EX0") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aPartsList[nNumType].nEx0);
                        }
                        else if (strcmp(cHeadText, "EX1") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aPartsList[nNumType].nEx1);
                        }
                        else if (strcmp(cHeadText, "WIDTH") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fWidth);
                        }
                        else if (strcmp(cHeadText, "HEIGHT") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].fHeight);
                        }
                        else if (strcmp(cHeadText, "PARAM0") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[0]);
                        }
                        else if (strcmp(cHeadText, "PARAM1") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[1]);
                        }
                        else if (strcmp(cHeadText, "PARAM2") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[2]);
                        }
                        else if (strcmp(cHeadText, "PARAM3") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[3]);
                        }
                        else if (strcmp(cHeadText, "PARAM4") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[4]);
                        }
                        else if (strcmp(cHeadText, "PARAM5") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[5]);
                        }
                        else if (strcmp(cHeadText, "PARAM6") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[6]);
                        }
                        else if (strcmp(cHeadText, "PARAM7") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsList[nNumType].afParam[7]);
                        }
                    }
                }
            }
        }
        // ファイルを閉じる
        fclose(pFile);
    }
}

//=============================================================================
// パーツレート読み込み処理
// Author : 後藤慎之助
//=============================================================================
void CModelData::LoadPartsRate(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[256];	               // 文字として読み取り用
    char cHeadText[256];	               // 文字の判別用
    char cDie[256];		                   // 使わない文字

    // パーツレートファイルを開く
    pFile = fopen(PARTS_RATE_FILENAME, "r");

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

                if (strcmp(cHeadText, "PARTS_RATE_HEAD") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_HEAD") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[0].fWeiRate);
                        }
                    }
                }
                else if (strcmp(cHeadText, "PARTS_RATE_UP") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_UP") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[1].fWeiRate);
                        }
                    }
                }
                else if (strcmp(cHeadText, "PARTS_RATE_DOWN") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_DOWN") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[2].fWeiRate);
                        }
                    }
                }
                else if (strcmp(cHeadText, "PARTS_RATE_WEA") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTS_RATE_WEA") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "ATK") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fAtkRate);
                        }
                        else if (strcmp(cHeadText, "DEF") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fDefRate);
                        }
                        else if (strcmp(cHeadText, "SPD") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fSpdRate);
                        }
                        else if (strcmp(cHeadText, "WEI") == 0)
                        {
                            sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_aPartsRate[3].fWeiRate);
                        }
                    }
                }
            }
        }
        // ファイルを閉じる
        fclose(pFile);
    }
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CModelData::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_MODEL_DATA; nCount++)
    {
        // マテリアルのテクスチャの破棄
        for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
        {
            if (m_aModelData[nCount].apTexMat[nCntMat] != NULL)
            {
                m_aModelData[nCount].apTexMat[nCntMat]->Release();
                m_aModelData[nCount].apTexMat[nCntMat] = NULL;
            }
        }

        // メッシュの破棄
        if (m_aModelData[nCount].pMesh != NULL)
        {
            m_aModelData[nCount].pMesh->Release();
            m_aModelData[nCount].pMesh = NULL;
        }

        // マテリアルの破棄
        if (m_aModelData[nCount].pBuffMat != NULL)
        {
            m_aModelData[nCount].pBuffMat->Release();
            m_aModelData[nCount].pBuffMat = NULL;
        }
    }
}

//=============================================================================
// モデルデータ情報の受け取りの処理
// Author : 後藤慎之助
//=============================================================================
CModelData::ModelData* CModelData::GetModelData(const int nNum)
{
    if (nNum < MAX_MODEL_DATA && nNum >= 0)
    {
        if (&m_aModelData[nNum] != NULL)
        {
            return &m_aModelData[nNum];
        }
    }

    return NULL;
}

//=============================================================================
// モデル初期位置データ情報の受け取りの処理
// Author : 後藤慎之助
//=============================================================================
CModelData::ModelPosDef * CModelData::GetPosDef(const int nNum)
{
    if (nNum < MAX_MODEL_POS_DEF && nNum >= 0)
    {
        if (&m_aModelPosDef[nNum] != NULL)
        {
            return &m_aModelPosDef[nNum];
        }
    }

    return NULL;
}

//=============================================================================
// パーツリスト情報の受け取りの処理
// Author : 後藤慎之助
//=============================================================================
CModelData::PartsParam* CModelData::GetPartsList(const int nNum)
{
    if (nNum < MAX_PARTS_LIST && nNum >= 0)
    {
        if (&m_aPartsList[nNum] != NULL)
        {
            return &m_aPartsList[nNum];
        }
    }

    return NULL;
}

//=============================================================================
// パーツレート情報の受け取りの処理
// Author : 後藤慎之助
//=============================================================================
CModelData::PartsRate* CModelData::GetPartsRate(const int nNum)
{
    if (nNum < MAX_PARTS && nNum >= 0)
    {
        if (&m_aPartsRate[nNum] != NULL)
        {
            return &m_aPartsRate[nNum];
        }
    }

    return NULL;
}