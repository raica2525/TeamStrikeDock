//===============================================
//
// キャラクター処理 (character.cpp)
// Author : 後藤慎之助
//
//===============================================
#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "character.h"
#include "animation.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animation.h"
#include "scene3d.h"
#include "library.h"
#include "game.h"
#include "player.h"
#include "modelData.h"

//========================================
// 静的メンバ変数宣言
//========================================

//=============================================================================
// オーバーライドされたコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CCharacter::CCharacter(OBJTYPE objtype) :CScene(objtype)
{
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_aPosDefault, 0, sizeof(m_aPosDefault));
    memset(m_anIndexParent, 0, sizeof(m_anIndexParent));
    memset(m_aPartsType, 0, sizeof(m_aPartsType));

    m_pAnimation = NULL;
    m_cAnimFilePass = NULL;

    // アニメーションはデフォルトで使うことを設定
    m_bUseAnimation = true;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CCharacter::~CCharacter()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CCharacter::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 位置を結びつける
    m_pos = pos;
    m_posOld = pos;

    // モデルをパーツ分メモリ確保し、初期化
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        // 生成していないなら生成
        if (!m_apModel[nCount])
        {
            m_apModel[nCount] = new CModel;
            m_apModel[nCount]->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
            m_apModel[nCount]->SetScale(size);
            m_apModel[nCount]->SetPos(m_aPosDefault[nCount]);
        }

        // カスタマイズ用で、生成していても読み込む
        m_apModel[nCount]->BindModelData(m_aPartsType[nCount]);

        // アニメーションを生成してあるなら
        if (m_pAnimation)
        {
            m_pAnimation->SetAnimPosReset(nCount);
        }
    }

    // アニメーションのロード（生成していないなら）
    if (!m_pAnimation)
    {
        m_pAnimation = CAnimation::Create(this, m_nPartsNum, m_cAnimFilePass);
    }

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CCharacter::Uninit(void)
{
    // モデルをパーツ分、終了し、メモリ開放
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        // 中身があるなら
        if (m_apModel[nCount] != NULL)
        {
            // 終了処理
            m_apModel[nCount]->Uninit();

            // メモリ開放
            delete m_apModel[nCount];
            m_apModel[nCount] = NULL;
        }
    }

    // アニメーション情報を終了し、メモリ開放
    if (m_pAnimation != NULL)
    {
        // 終了処理
        m_pAnimation->Uninit();

        // メモリ開放
        delete m_pAnimation;
        m_pAnimation = NULL;
    }

    Release();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CCharacter::Update(void)
{
    // アニメーションを使うフラグがtrueならアニメーションさせる
    if (m_bUseAnimation)
    {
        m_pAnimation->Update();
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CCharacter::Draw(void)
{
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        if (nCount == 0)
        {
            // 全ての親は、キャラクターの位置そのものを渡す
            m_apModel[nCount]->Draw(m_pos, m_rot);
        }
        else
        {
            m_apModel[nCount]->Draw(m_apModel[m_anIndexParent[nCount]]);
        }
    }
}

//=============================================================================
// やられた時の描画
// Author : 後藤慎之助
//=============================================================================
void CCharacter::DeathDraw(void)
{
    // Zバッファを無効化
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

    // 各パーツを、白くするためのtrue
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        if (nCount == 0)
        {
            m_apModel[nCount]->Draw(m_pos, m_rot, true);
        }
        else
        {
            m_apModel[nCount]->Draw(m_apModel[m_anIndexParent[nCount]], true);
        }
    }

    // Zバッファを有効化
    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

//=============================================================================
// モデルデータ（座標,親子関係）読み込み処理（テキストファイルから直接読み込む時に使う関数）
// Author : 後藤慎之助
//=============================================================================
void CCharacter::LoadModelData(char* cFilePass)
{
    // 名前を記憶
    m_cAnimFilePass = cFilePass;

    // ファイルポイント
    FILE *pFile;

    // 変数宣言
    int  nCntLoad = 0;
    char cReedText[128];	// 文字として読み取り用
    char cHeadText[256];	//
    char cDie[128];
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // ファイル開
    pFile = fopen(cFilePass, "r");

    // 開けた
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "CHARACTERSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "CHARACTERSET") == 0)
        {
            while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "PARTSSET") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTSSET") != 0)
                    {

                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "INDEX") == 0)
                        {
                            int nNum;
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nNum);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_anIndexParent[nCntLoad]);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos.x, &pos.y, &pos.z);

                            m_aPosDefault[nCntLoad] = pos;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot.x, &rot.y, &rot.z);
                        }
                    }
                    nCntLoad++;
                }
            }
        }
        // ファイル閉
        fclose(pFile);
    }

    // 開けない
    else
    {
        printf("開けれませんでした\n");
    }
}

//=============================================================================
// モデルデータ（座標,親子関係）読み込み処理（モデルの初期位置データから読み込むときに使う関数）
// Author : 後藤慎之助
//=============================================================================
void CCharacter::LoadModelData(int nModelPosDefUp, int nModelPosDefDown)
{
    // モデルの初期位置データを受け取る
    CModelData *pModelData = CManager::GetModelData();

    // 上半身と下半身を分ける
    CModelData::ModelPosDef *pModelPosDefUp = pModelData->GetPosDef(nModelPosDefUp);
    CModelData::ModelPosDef *pModelPosDefDown = pModelData->GetPosDef(nModelPosDefDown);

    // 上半身
    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        m_anIndexParent[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->nParent[nCnt];
        m_aPosDefault[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->pos[nCnt];
    }

    // 下半身
    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        m_anIndexParent[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->nParent[nCnt];
        m_aPosDefault[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->pos[nCnt];
    }
}
