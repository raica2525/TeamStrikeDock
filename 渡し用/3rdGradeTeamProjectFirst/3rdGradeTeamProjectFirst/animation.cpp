//===============================================
//
// アニメーション処理 (animation.cpp)
// Author : 後藤慎之助
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "animation.h"
#include "game.h"

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CAnimation::CAnimation()
{
    memset(m_aKeyInfo, 0, sizeof(m_aKeyInfo));
    m_nAnimation = 0;

    for (int nCount = 0; nCount < CHARCTER_PARTS_MAX; nCount++)
    {
        m_aPos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aPosDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aRot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aRotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }

    m_nAnimationOld = 0;
    m_pCharacter = NULL;

    m_nFrame = 0;
    m_nKey = 0;
    m_nPartsNum = 0;
    m_bAdjustment = false;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CAnimation::~CAnimation()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CAnimation::Init(CCharacter *pCharacter, int nPartsNum)
{
    memset(m_aKeyInfo, 0, sizeof(m_aKeyInfo));
    m_nAnimation = 0;
    m_pCharacter = pCharacter;
    m_nFrame = 0;
    m_nKey = 0;
    m_nPartsNum = nPartsNum;

    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_aPos[nCount] = m_pCharacter->GetDefaultPos(nCount);
        m_aRot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aPosDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_aRotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CAnimation::Uninit(void)
{
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CAnimation::Update(void)
{
    //モーションが変わったとき
    if (m_nAnimationOld != m_nAnimation)
    {
        m_nFrame = 0;
        m_nKey = 0;
    }
    // モデルのパーツ数更新
    for (int nCntParts = 0; nCntParts < m_nPartsNum; nCntParts++)
    {
        // 0フレーム目ならパーツのフレームごとの移動量を計算
        if (m_nFrame == 0)
        {
            m_aPosDest[nCntParts].x = ((m_aKeyInfo[m_nAnimation].aParts[nCntParts].aPos[m_nKey].x - (m_aPos[nCntParts].x - m_pCharacter->GetDefaultPos(nCntParts).x)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]));
            m_aPosDest[nCntParts].y = ((m_aKeyInfo[m_nAnimation].aParts[nCntParts].aPos[m_nKey].y - (m_aPos[nCntParts].y - m_pCharacter->GetDefaultPos(nCntParts).y)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]));
            m_aPosDest[nCntParts].z = ((m_aKeyInfo[m_nAnimation].aParts[nCntParts].aPos[m_nKey].z - (m_aPos[nCntParts].z - m_pCharacter->GetDefaultPos(nCntParts).z)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]));

            m_aRotDest[nCntParts].x = (m_aKeyInfo[m_nAnimation].aParts[nCntParts].aRot[m_nKey].x - m_aRot[nCntParts].x) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]);
            m_aRotDest[nCntParts].y = (m_aKeyInfo[m_nAnimation].aParts[nCntParts].aRot[m_nKey].y - m_aRot[nCntParts].y) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]);
            m_aRotDest[nCntParts].z = (m_aKeyInfo[m_nAnimation].aParts[nCntParts].aRot[m_nKey].z - m_aRot[nCntParts].z) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey]);
        }

        // 値を加算
        m_aPos[nCntParts] += m_aPosDest[nCntParts];
        m_aRot[nCntParts] += m_aRotDest[nCntParts];

        // x y z軸上で180°以上回転したら角度を修正(既に修正されていた場合は処理しない)
        if (m_aRot[0].x >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_aRot[0].x = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }
        if (m_aRot[0].x <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_aRot[0].x = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].y >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].y = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].y <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].y = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].z >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].z = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }

        if (m_aRot[0].z <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_aRot[nCntParts].z = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        // 回転と座標の設定
        m_pCharacter->CCharacter::SetPartPos(nCntParts, m_aPos[nCntParts]);
        m_pCharacter->CCharacter::SetPartRot(nCntParts, m_aRot[nCntParts]);
    }
    // 今回のアニメーションを次回判定用に保持
    m_nAnimationOld = m_nAnimation;

    // ループするとき
    if (m_nFrame >= m_aKeyInfo[m_nAnimation].anFrame[m_nKey] && m_aKeyInfo[m_nAnimation].nLoop == 1)
    {
        // キーフレームの加算と変数の初期化
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // キー数の判定
        if (m_nKey >= m_aKeyInfo[m_nAnimation].nKeyNum)
        {
            m_nKey = 0;
        }
    }
    // ループしないとき
    else if (m_nFrame >= m_aKeyInfo[m_nAnimation].anFrame[m_nKey] && m_aKeyInfo[m_nAnimation].nLoop == 0)
    {
        // キーフレームの加算と変数の初期化
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // キー数の判定
        if (m_nKey >= m_aKeyInfo[m_nAnimation].nKeyNum)
        {
            // パーツの移動量のリセット
            for (int nCntMotion = 0; nCntMotion < m_nPartsNum; nCntMotion++)
            {
                m_aPosDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                m_aRotDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }

            // アニメーションを待機モーションに設定
            m_nAnimation = 0;
            m_nKey = 0;
        }
    }
    else
    {
        // フレームを数加算
        m_nFrame++;
    }
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CAnimation::Draw(void)
{
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CAnimation * CAnimation::Create(CCharacter *pCharacter, int nPartsNum, char* cFilePass)
{
    // インスタンスの生成
    CAnimation *pAnimation;
    pAnimation = new CAnimation;
    // 初期化
    pAnimation->Init(pCharacter, nPartsNum);
    // アニメーションの読み込み
    pAnimation->LoadAnimation(cFilePass);

    return pAnimation;
}

//=============================================================================
// アニメーション読み込み処理
// Author : 後藤慎之助
//=============================================================================
void CAnimation::LoadAnimation(char* pass)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    int  nCntLoad = 0;
    int	 nCntKey = 0;
    int	 nCntMotion = 0;

    char cReedText[2048];	// 文字読み取り用
    char cHeadText[2048];	//
    char cDie[2048];		//

    // ファイル開
    pFile = fopen(pass, "r");

    // 開けた
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            // END_SCRIPTが読み込まれるまで繰り返す
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "MOTIONSET") == 0)
                {
                    // END_MOTIONSETが読み込まれるまで繰り返す
                    while (strcmp(cHeadText, "END_MOTIONSET") != 0)
                    {
                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        // ループするかどうかの読み込み
                        if (strcmp(cHeadText, "LOOP") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_aKeyInfo[nCntMotion].nLoop);
                        }
                        // キー数の読み込み
                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_aKeyInfo[nCntMotion].nKeyNum);
                        }
                        // キーセットの読み込み
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {
                            // END_KEYSETが読み込まれるまで繰り返す
                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                // フレーム数の読み込み
                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_aKeyInfo[nCntMotion].anFrame[nCntKey]);
                                }
                                // キーデータの読み込み
                                if (strcmp(cHeadText, "KEY") == 0)
                                {
                                    // END_KEYが読み込まれるまで繰り返す
                                    while (strcmp(cHeadText, "END_KEY") != 0)
                                    {
                                        fgets(cReedText, sizeof(cReedText), pFile);
                                        sscanf(cReedText, "%s", &cHeadText);

                                        // 座標の読み込み
                                        if (strcmp(cHeadText, "POS") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aPos[nCntKey].x,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aPos[nCntKey].y,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aPos[nCntKey].z);
                                        }
                                        // 回転の読み込み
                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aRot[nCntKey].x,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aRot[nCntKey].y,
                                                &m_aKeyInfo[nCntMotion].aParts[nCntLoad].aRot[nCntKey].z);
                                        }
                                    }
                                    // 変数のインクリメント
                                    nCntLoad++;
                                }
                            }
                            nCntLoad = 0;
                            // 変数のインクリメント
                            nCntKey++;
                        }
                    }
                    nCntKey = 0;
                    // 変数のインクリメント
                    nCntMotion++;
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
// アニメーションの位置をリセット
// Author : 後藤慎之助
//=============================================================================
void CAnimation::SetAnimPosReset(int nNum)
{
    // 大元の親パーツは、いきなり位置を変えるとガタつくため、更新処理で取得させる
    if (nNum != 0)
    {
        // 現在の位置に初期位置を代入
        m_aPos[nNum].x = m_pCharacter->GetDefaultPos(nNum).x;
        m_aPos[nNum].y = m_pCharacter->GetDefaultPos(nNum).y;
        m_aPos[nNum].z = m_pCharacter->GetDefaultPos(nNum).z;

        // 残りキーフレーム数を考慮した、目的値を代入
        m_aPosDest[nNum].x = ((m_aKeyInfo[m_nAnimation].aParts[nNum].aPos[m_nKey].x - (m_aPos[nNum].x - m_pCharacter->GetDefaultPos(nNum).x)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey] - m_nFrame + 1));
        m_aPosDest[nNum].y = ((m_aKeyInfo[m_nAnimation].aParts[nNum].aPos[m_nKey].y - (m_aPos[nNum].y - m_pCharacter->GetDefaultPos(nNum).y)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey] - m_nFrame + 1));
        m_aPosDest[nNum].z = ((m_aKeyInfo[m_nAnimation].aParts[nNum].aPos[m_nKey].z - (m_aPos[nNum].z - m_pCharacter->GetDefaultPos(nNum).z)) / float(m_aKeyInfo[m_nAnimation].anFrame[m_nKey] - m_nFrame + 1));
    }
}