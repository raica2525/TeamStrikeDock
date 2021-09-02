//=============================================================================
//
// テクスチャのデータの処理 [texture.cpp]
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルード
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_FILENAME ("data/TXT/texture.txt")	// テクスチャを読み込むファイルの名前

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CTexture::CTexture()
{
    // 外部ファイル内で未設定のものが多いため、明示的に初期化
    for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
    {
        m_aInfo[nCount].pTexture = NULL;
        m_aInfo[nCount].nPattern = 0;
        m_aInfo[nCount].nSpeed = 0;
        m_aInfo[nCount].bRepeat = false;
    }
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CTexture::~CTexture()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CTexture::Init(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    char cReadText[2048];	// 文字として読み取り用
    char cHeadText[2048];	// 文字の判別用
    char cDie[2048];		// 使わない文字
    int nNumType = 0;       // タイプのナンバー
    char cLoadName[256];    // 読み込み時の名前
    int nBool = 0;          // intからboolへの橋渡し

    // ファイルを開く
    pFile = fopen(TEXTURE_FILENAME, "r");

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

                // cHeadTextがTEXTURESETの時
                if (strcmp(cHeadText, "TEXTURESET") == 0)
                {
                    // cHeadTextがEND_TEXTURESETになるまで
                    while (strcmp(cHeadText, "END_TEXTURESET") != 0)
                    {
                        fgets(cReadText, sizeof(cReadText), pFile);
                        sscanf(cReadText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "TYPE") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
                        }
                        else if (strcmp(cHeadText, "NAME") == 0)
                        {
                            sscanf(cReadText, "%s %s %s", &cDie, &cDie, &cLoadName);

                            //テクスチャの読み込み
                            LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
                            D3DXCreateTextureFromFile(pDevice, cLoadName, &m_aInfo[nNumType].pTexture);
                        }
                        else if (strcmp(cHeadText, "PATTERN") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aInfo[nNumType].nPattern);
                        }
                        else if (strcmp(cHeadText, "SPEED") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aInfo[nNumType].nSpeed);
                        }
                        else if (strcmp(cHeadText, "REPEAT") == 0)
                        {
                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);

                            if (nBool == 0)
                            {
                                m_aInfo[nNumType].bRepeat = false;
                            }
                            else
                            {
                                m_aInfo[nNumType].bRepeat = true;
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

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CTexture::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
    {
        if (m_aInfo[nCount].pTexture != NULL)
        {
            m_aInfo[nCount].pTexture->Release();
            m_aInfo[nCount].pTexture = NULL;
        }
    }
}

//=============================================================================
// テクスチャ情報の受け取りの処理
// Author : 後藤慎之助
//=============================================================================
CTexture::Info* CTexture::GetInfo(const int nNum)
{
    if (nNum < MAX_TEXTURE && nNum >= 0)
    {
        if (&m_aInfo[nNum] != NULL)
        {
            return &m_aInfo[nNum];
        }
    }

    return NULL;
}