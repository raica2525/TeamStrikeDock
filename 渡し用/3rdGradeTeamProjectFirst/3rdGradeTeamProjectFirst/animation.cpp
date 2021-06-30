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
    memset(m_keyInfo, 0, sizeof(m_keyInfo));
    m_nAnimation = 0;
    m_type = TYPE_NONE;

    for (int nCount = 0; nCount < CHARCTER_PARTS_MAX; nCount++)
    {
        m_pos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_posDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_rot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_rotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
HRESULT CAnimation::Init(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type)
{
    memset(m_keyInfo, 0, sizeof(m_keyInfo));
    m_nAnimation = 0;
    m_pCharacter = pCharacter;
    m_nFrame = 0;
    m_nKey = 0;
    m_type = type;
    m_nPartsNum = nPartsNum;

    for (int nCount = 0; nCount < m_nPartsNum; nCount++)
    {
        m_pos[nCount] = m_pCharacter->GetDefaultPos(nCount);
        m_rot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_posDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_rotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
            m_posDest[nCntParts].x = ((m_keyInfo[m_type][m_nAnimation].parts[nCntParts].pos[m_nKey].x - (m_pos[nCntParts].x - m_pCharacter->GetDefaultPos(nCntParts).x)) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]));
            m_posDest[nCntParts].y = ((m_keyInfo[m_type][m_nAnimation].parts[nCntParts].pos[m_nKey].y - (m_pos[nCntParts].y - m_pCharacter->GetDefaultPos(nCntParts).y)) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]));
            m_posDest[nCntParts].z = ((m_keyInfo[m_type][m_nAnimation].parts[nCntParts].pos[m_nKey].z - (m_pos[nCntParts].z - m_pCharacter->GetDefaultPos(nCntParts).z)) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]));

            m_rotDest[nCntParts].x = (m_keyInfo[m_type][m_nAnimation].parts[nCntParts].rot[m_nKey].x - m_rot[nCntParts].x) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]);
            m_rotDest[nCntParts].y = (m_keyInfo[m_type][m_nAnimation].parts[nCntParts].rot[m_nKey].y - m_rot[nCntParts].y) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]);
            m_rotDest[nCntParts].z = (m_keyInfo[m_type][m_nAnimation].parts[nCntParts].rot[m_nKey].z - m_rot[nCntParts].z) / float(m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey]);
        }

        // 値を加算
        m_pos[nCntParts] += m_posDest[nCntParts];
        m_rot[nCntParts] += m_rotDest[nCntParts];

        // x y z軸上で180°以上回転したら角度を修正(既に修正されていた場合は処理しない)
        if (m_rot[0].x >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_rot[0].x = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }
        if (m_rot[0].x <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_rot[0].x = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].y >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].y = D3DXToRadian(-180.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].y <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].z = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].z >= D3DXToRadian(180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].z = D3DXToRadian(1 - 80.0f);
            m_bAdjustment = true;
        }

        if (m_rot[0].z <= D3DXToRadian(-180.0f) && !m_bAdjustment)
        {
            m_rot[nCntParts].z = D3DXToRadian(180.0f);
            m_bAdjustment = true;
        }

        // 回転と座標の設定
        m_pCharacter->SetPartPos(nCntParts, m_pos[nCntParts]);
        m_pCharacter->SetPartRot(nCntParts, m_rot[nCntParts]);
    }
    // 今回のアニメーションを次回判定用に保持
    m_nAnimationOld = m_nAnimation;

    // ループするとき
    if (m_nFrame >= m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey] && m_keyInfo[m_type][m_nAnimation].nLoop == 1)
    {
        // キーフレームの加算と変数の初期化
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // キー数の判定
        if (m_nKey >= m_keyInfo[m_type][m_nAnimation].nKeyNum)
        {
            m_nKey = 0;
        }
    }
    // ループしないとき
    else if (m_nFrame >= m_keyInfo[m_type][m_nAnimation].nFrame[m_nKey] && m_keyInfo[m_type][m_nAnimation].nLoop == 0)
    {
        // キーフレームの加算と変数の初期化
        m_nFrame = 0;
        m_nKey++;
        m_bAdjustment = false;

        // キー数の判定
        if (m_nKey >= m_keyInfo[m_type][m_nAnimation].nKeyNum)
        {
            // パーツの移動量のリセット
            for (int nCntMotion = 0; nCntMotion < m_nPartsNum; nCntMotion++)
            {
                m_posDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                m_rotDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
CAnimation * CAnimation::Create(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type, char* cFilePass)
{
    // インスタンスの生成
    CAnimation *pAnimation;
    pAnimation = new CAnimation;
    // 初期化
    pAnimation->Init(pCharacter, nPartsNum, type);
    // アニメーションの読み込み
    pAnimation->LoadAnimation(type, cFilePass);

    return pAnimation;
}

//=============================================================================
// アニメーション読み込み処理
// Author : 後藤慎之助
//=============================================================================
void CAnimation::LoadAnimation(ANIM_TYPE type, char* pass)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    int  nCntLoad = 0;
    int	 nCntKey = 0;
    int	 nCntMotion = 0;

    char cReedText[1024];	// 文字読み取り用
    char cHeadText[1024];	//
    char cDie[1024];		//

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[type][nCntMotion].nLoop);
                        }
                        // キー数の読み込み
                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[type][nCntMotion].nKeyNum);
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
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[type][nCntMotion].nFrame[nCntKey]);
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
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].pos[nCntKey].x,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].pos[nCntKey].y,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].pos[nCntKey].z);
                                        }
                                        // 回転の読み込み
                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].rot[nCntKey].x,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].rot[nCntKey].y,
                                                &m_keyInfo[type][nCntMotion].parts[nCntLoad].rot[nCntKey].z);
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
