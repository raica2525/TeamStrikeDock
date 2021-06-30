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
#include "shadow.h"
#include "player.h"
#include "modelData.h"

//========================================
// 静的メンバ変数宣言
//========================================

//=============================================================================
// デフォルトコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CCharacter::CCharacter()
{
    m_activeCollisionSize = DEFAULT_VECTOR;
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_tribe = TRIBE_NONE;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_posDefault, 0, sizeof(m_posDefault));
    memset(m_nIndexParent, 0, sizeof(m_nIndexParent));
    memset(m_aPartsType, 0, sizeof(m_aPartsType));

    m_pAnimation = NULL;
    m_pShadow = NULL;
    m_pFrame = NULL;
    m_cAnimFilePass = NULL;

    for (int nCnt = 0; nCnt < CHARCTER_COLLISION_MAX; nCnt++)
    {
        m_aCollision[nCnt].pos = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;

    // アニメーションはデフォルトで使うことを設定
    m_bUseAnimation = true;
}

//=============================================================================
// オーバーライドされたコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CCharacter::CCharacter(OBJTYPE objtype) :CScene(objtype)
{
    m_activeCollisionSize = DEFAULT_VECTOR;
    m_posOld = DEFAULT_VECTOR;

    m_pos = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_tribe = TRIBE_NONE;
    m_nPartsNum = 0;
    memset(m_apModel, 0, sizeof(m_apModel));
    memset(m_posDefault, 0, sizeof(m_posDefault));
    memset(m_nIndexParent, 0, sizeof(m_nIndexParent));
    memset(m_aPartsType, 0, sizeof(m_aPartsType));

    m_pAnimation = NULL;
    m_pShadow = NULL;
    m_pFrame = NULL;
    m_cAnimFilePass = NULL;

    for (int nCnt = 0; nCnt < CHARCTER_COLLISION_MAX; nCnt++)
    {
        m_aCollision[nCnt].pos = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;

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

    // モデルのポインタ配列の初期化
    memset(m_apModel, 0, sizeof(m_apModel));

    // モデルをパーツ分メモリ確保し、初期化
    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_apModel[nCount] = new CModel;
        m_apModel[nCount]->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);
        m_apModel[nCount]->SetScale(size);
        m_apModel[nCount]->BindModelData(m_aPartsType[nCount]);
        m_apModel[nCount]->SetPos(m_posDefault[nCount]);
    }

    // アニメーションのロード
    m_pAnimation = CAnimation::Create(this, m_nPartsNum, (CAnimation::ANIM_TYPE)m_tribe, m_cAnimFilePass);

    //// プレイヤーなら
    //if (m_tribe == TRIBE_PLAYER)
    //{
    //    // 影を生成
    //    m_pShadow = CShadow::Create(D3DXVECTOR3(pos.x, SHADOW_POS_Y, pos.z), DEFAULT_VECTOR, HIT_TOP, SHADOW_COLOR);

    //    // ふちどりを生成
    //    m_pFrame = CShadow::Create(pos, DEFAULT_VECTOR, D3DXVECTOR3(D3DXToRadian(45.0f), 0.0f, 0.0f), SHADOW_COLOR);
    //}

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

    // 影を使用しない
    if (m_pShadow != NULL)
    {
        m_pShadow->Unable();
    }

    // ふちどりを使用しない
    if (m_pFrame != NULL)
    {
        m_pFrame->Unable();
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

    //// 影を更新
    //if (m_pShadow != NULL)
    //{
    //    m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, SHADOW_POS_Y, m_pos.z));

    //    // サイズはここではなく初期化で調整、ロットはカメラの向きを参考に
    //    m_pShadow->SetSize(D3DXVECTOR3(m_activeCollisionSize.x * SHADOW_SIZE_RATE, m_activeCollisionSize.x * SHADOW_SIZE_RATE, 0.0f));
    //    m_pShadow->SetRotAngle(m_rot.y);
    //}

    //// ふちどりを更新
    //if (m_pShadow != NULL)
    //{
    //    m_pFrame->SetPos(m_pos);

    //    // サイズはここではなく初期化で調整、ロットはカメラの向きを参考に
    //    m_pFrame->SetSize(D3DXVECTOR3(m_activeCollisionSize.x * 200.0f, m_activeCollisionSize.x * 200.0f, 0.0f));
    //    m_pFrame->SetRotAngle(m_rot.y);
    //}
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CCharacter::Draw(void)
{
    //// ステンシルバッファで影を描画
    //if (m_pShadow != NULL)
    //{
    //    // ステンシルバッファを使う
    //    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //    // Zテスト有効
    //    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    // Zテストを必ず失敗に
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

    //    // ステンシルテスト有効
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    //    // ステンシルバッファへ反映する参照値
    //    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
    //    // 参照値マスク
    //    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
    //    // ステンシルテストを必ず成功に
    //    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    //    // ステンシルテストのテスト設定
    //    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    //    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
    //    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    //    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    //    {
    //        // 仮
    //        m_apModel[nCount]->SetScale(D3DXVECTOR3(0.9f, 0.9f, 0.9f));

    //        if (nCount == 0)
    //        {
    //            // 仮（重いなら、丸い影でも十分）（もしくは、マルチターゲットレンダリングで負荷軽減）
    //            float fRotX = D3DXToRadian(90.0f);
    //            float fRotY = D3DXToRadian(270.0f);
    //            if(m_rot.y == PLAYER_ROT_LEFT)
    //            {
    //                fRotX = D3DXToRadian(270.0f);
    //                fRotY = D3DXToRadian(90.0f);
    //            }
    //            m_apModel[nCount]->Draw(D3DXVECTOR3(m_pos.x + 50.0f, SHADOW_POS_Y, m_pos.z),
    //                D3DXVECTOR3(fRotX, fRotY, m_rot.z));
    //        }
    //        else
    //        {
    //            m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
    //        }

    //        m_apModel[nCount]->SetScale(DEFAULT_SCALE);
    //    }

    //    // ステンシルテスト無効
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

    //    // ZBUFFER比較設定変更 => (参照値 <= バッファ値)(戻す)
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    //    // 影を描画
    //    m_pShadow->Draw();
    //}

    //// ステンシルバッファでふちどりを描画
    //if (m_pFrame != NULL)
    //{
    //    // ステンシルバッファを使う
    //    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //    // Zテスト有効
    //    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    // Zテストを必ず失敗に
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

    //    // ステンシルテスト有効
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    //    // ステンシルバッファへ反映する参照値
    //    pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
    //    // 参照値マスク
    //    pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
    //    // ステンシルテストを必ず成功に
    //    pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    //    // ステンシルテストのテスト設定
    //    pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    //    pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
    //    pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    //    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    //    {
    //        // 仮
    //        m_apModel[nCount]->SetScale(D3DXVECTOR3(1.02f, 1.02f, 1.02f));

    //        if (nCount == 0)
    //        {
    //            m_apModel[nCount]->Draw(m_pos, m_rot);
    //        }
    //        else
    //        {
    //            m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
    //        }

    //        m_apModel[nCount]->SetScale(DEFAULT_SCALE);
    //    }

    //    // ステンシルテスト無効
    //    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

    //    // ZBUFFER比較設定変更 => (参照値 <= バッファ値)(戻す)
    //    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    //    // ふちどりを描画
    //    m_pFrame->Draw();
    //}

    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        if (nCount == 0)
        {
            m_apModel[nCount]->Draw(m_pos, m_rot);
        }
        else
        {
            m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
        }
    }
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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nIndexParent[nCntLoad]);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos.x, &pos.y, &pos.z);

                            m_posDefault[nCntLoad] = pos;
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
        m_nIndexParent[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->nParent[nCnt];
        m_posDefault[pModelPosDefUp->nIndex[nCnt]] = pModelPosDefUp->pos[nCnt];
    }

    // 下半身
    for (int nCnt = 0; nCnt < MAX_MODEL_PARTS_SET; nCnt++)
    {
        m_nIndexParent[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->nParent[nCnt];
        m_posDefault[pModelPosDefDown->nIndex[nCnt]] = pModelPosDefDown->pos[nCnt];
    }
}
