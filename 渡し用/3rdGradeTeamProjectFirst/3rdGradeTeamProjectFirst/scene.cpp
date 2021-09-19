//===============================================
//
// シーン上のオブジェクト処理 (scene.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "scene.h"
#include "renderer.h"
#include "ui.h"

//========================================
// 静的メンバ変数宣言
//========================================
int CScene::m_nNumAll[CScene::OBJTYPE_MAX] = {};
int CScene::m_nNumObjectAll = 0;

CScene *CScene::m_apTop[CScene::OBJTYPE_MAX] = {};
CScene *CScene::m_apCur[CScene::OBJTYPE_MAX] = {};

//========================================
// シーン上のオブジェクトのオーバーライドされたコンストラクタ
// Author : 後藤慎之助
//========================================
CScene::CScene(OBJTYPE objType)
{
    // オブジェクトの種類を引数と結びつける
    m_objType = objType;

    // 先頭がないなら、先頭に
    if (m_apTop[m_objType] == NULL)
    {
        m_apTop[m_objType] = this;
    }

    // 現在における最新のオブジェクトがないなら、最新に
    if (m_apCur[m_objType] == NULL)
    {
        m_apCur[m_objType] = this;
    }

    // 現在のオブジェクトの次のオブジェクトを、自分にする
    m_apCur[m_objType]->m_pNext = this;

    // 現在のオブジェクトが自分の場合
    if (m_apCur[m_objType] == this)
    {
        // 自分の前のオブジェクトを、NULLにする
        m_pPrev = NULL;
    }
    else
    {
        // 自分の前のオブジェクトを、現在のオブジェクトにする
        m_pPrev = m_apCur[m_objType];
    }

    // 現在のオブジェクトを、自分にする
    m_apCur[m_objType] = this;

    // 自分の次のオブジェクトを、NULLにする
    m_pNext = NULL;

    // 増えたオブジェクトをカウント
    m_nNumAll[m_objType]++;

    // 全体の数をインクリメント
    m_nNumObjectAll++;

    // 使用するフラグをtrueに
    m_bUse = true;

#ifdef _DEBUG
    m_bReloadUI = false;
#endif
}

//========================================
// シーン上のオブジェクトのデストラクタ
// Author : 後藤慎之助
//========================================
CScene::~CScene()
{

}

//==============================================
// シーン上のオブジェクトを全て更新
// Author : 後藤慎之助
//==============================================
void CScene::UpdateAll(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // 先頭、最新のものがあるなら
        if (m_apTop[nCnt] != NULL && m_apCur[nCnt] != NULL)
        {
            // 記憶用の変数
            CScene*pScene = m_apTop[nCnt];

            do
            {
                // 記憶用の変数(Update中に、Uninitされることを考慮)
                CScene*pNextScene = pScene->m_pNext;

                // 更新処理
                pScene->Update();

                // 使用フラグがfalseなら
                if (pScene->m_bUse == false)
                {
                    // メモリの開放
                    delete pScene;
                    pScene = NULL;
                }

                // 次のシーンに変えていく
                pScene = pNextScene;

            } while (pScene != NULL);
        }
    }
}

//==============================================
// 波紋エフェクト以外を描画
// Author : 後藤慎之助
//==============================================
void CScene::DrawExceptWave(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // 波紋エフェクト以外なら
        if (nCnt != OBJTYPE_WAVE && nCnt != OBJTYPE_NONE_DRAW)
        {
            // 先頭、最新のものがあるなら
            if (m_apTop[nCnt] != NULL && m_apCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_apTop[nCnt];

                do
                {
                    // 描画処理
                    pScene->Draw();

                    // 次のシーンに変えていく
                    pScene = pScene->m_pNext;

                } while (pScene != NULL);
            }
        }
    }
}

//==============================================
// 波紋エフェクトとUI以外を描画
// Author : 後藤慎之助
//==============================================
void CScene::DrawExceptWaveAndUI(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // 波紋エフェクトかUI以外なら
        if (nCnt != OBJTYPE_WAVE && nCnt != OBJTYPE_UI_BACK_TEXT && nCnt != OBJTYPE_UI_FRONT_TEXT && nCnt != OBJTYPE_NONE_DRAW)
        {
            // 先頭、最新のものがあるなら
            if (m_apTop[nCnt] != NULL && m_apCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_apTop[nCnt];

                do
                {
                    // 描画処理
                    pScene->Draw();

                    // 次のシーンに変えていく
                    pScene = pScene->m_pNext;

                } while (pScene != NULL);
            }
        }
    }
}

//==============================================
// 波紋エフェクトを描画
// Author : 後藤慎之助
//==============================================
void CScene::DrawWave(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // 波紋エフェクトなら
        if (nCnt == OBJTYPE_WAVE)
        {
            // 先頭、最新のものがあるなら
            if (m_apTop[nCnt] != NULL && m_apCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_apTop[nCnt];

                do
                {
                    // 描画処理
                    pScene->Draw();

                    // 次のシーンに変えていく
                    pScene = pScene->m_pNext;

                } while (pScene != NULL);
            }
        }
    }
}

//==============================================
// プレイヤーを描画
// Author : 後藤慎之助
//==============================================
void CScene::DrawPlayer(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // プレイヤーなら
        if (nCnt == OBJTYPE_PLAYER)
        {
            // 先頭、最新のものがあるなら
            if (m_apTop[nCnt] != NULL && m_apCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_apTop[nCnt];

                do
                {
                    // 描画処理
                    pScene->Draw();

                    // 次のシーンに変えていく
                    pScene = pScene->m_pNext;

                } while (pScene != NULL);
            }
        }
    }
}

//==============================================
// クイックソート
// Author : 後藤慎之助
//==============================================
void CScene::QuickSort(CScene * apScene[], int left, int right)
{
    // 変数定義
    int Left;
    int Right;
    CScene* pivot;
    int nPivotAnswer = (left + right) / 2;

    // 初期値は引数から
    Left = left;
    Right = right;

    // ピボットが奇数なら、繰り下げる
    if (nPivotAnswer % 2 != 0)
    {
        nPivotAnswer = nPivotAnswer - (1 / 2);
    }

    // ピボットの範囲を設定(下限)
    if (nPivotAnswer < 0)
    {
        nPivotAnswer = 0;
    }

    // ピボットの範囲を設定(上限)
    if (nPivotAnswer > m_nNumObjectAll - 1)
    {
        nPivotAnswer = m_nNumObjectAll - 1;
    }

    // 基準は真ん中に設定
    pivot = apScene[nPivotAnswer];

    // ソーティング
    while (1)
    {
        // 基準より小さい値を左から見つけていく
        while (apScene[Left]->m_objType < pivot->m_objType)
        {
            Left++;

            // 最大値の制限
            if (Left > m_nNumObjectAll - 1)
            {
                Left = m_nNumObjectAll - 1;

                break;
            }
        }

        // 基準より大きい値を右から見つけていく
        while (pivot->m_objType < apScene[Right]->m_objType)
        {
            Right--;

            // 最小値の制限
            if (Right < 0)
            {
                Right = 0;

                break;
            }
        }

        // 見つかった値の順序が逆になったら終了 
        if (Left >= Right)
        {
            break;
        }

        // 交換処理
        if (apScene[Left] != NULL && apScene[Right] != NULL)
        {
            // 若い番号を記憶
            CScene *pScene = apScene[Left];

            // 若い番号に次の番号を代入
            apScene[Left] = apScene[Right];

            // 次の番号に記憶したものを代入
            apScene[Right] = pScene;
        }

        // 次の値に移動
        Left++;
        Right--;

        // 最大値の制限
        if (Left > m_nNumObjectAll - 1)
        {
            break;
        }

        // 最小値の制限
        if (Right < 0)
        {
            break;
        }
    }

    //左のデータ群を対象としてクイックソートを再帰
    if (left < Left - 1)
    {
        QuickSort(apScene, left, Left - 1);
    }

    //右のデータ群を対象としてクイックソートを再帰
    if (Right + 1 < right)
    {
        QuickSort(apScene, Right + 1, right);
    }
}

//==============================================
// シーン上のオブジェクトを全て開放
// Author : 後藤慎之助
//==============================================
void CScene::ReleaseAll(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // 先頭、最新のものがあるなら
        if (m_apTop[nCnt] != NULL && m_apCur[nCnt] != NULL)
        {
            // 記憶用の変数
            CScene*pScene = m_apTop[nCnt];

            do
            {
                // 記憶用の変数
                CScene*pNextScene = pScene->m_pNext;

                // 終了処理
                pScene->Uninit();

                // 使用フラグがfalseなら
                if (pScene->m_bUse == false)
                {
                    // メモリの開放
                    delete pScene;
                    pScene = NULL;
                }

                // 次のシーンに変えていく
                pScene = pNextScene;

            } while (pScene != NULL);
        }
    }
}

//==============================================
// リロードするUIを開放
// Author : 後藤慎之助
//==============================================
#ifdef _DEBUG
void CScene::ReleaseReloadUI(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        if (nCnt == OBJTYPE_UI_BACK_TEXT || nCnt == OBJTYPE_UI_FRONT_TEXT)
        {
            // 先頭、最新のものがあるなら
            if (m_apTop[nCnt] != NULL && m_apCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_apTop[nCnt];

                do
                {
                    // 記憶用の変数
                    CScene*pNextScene = pScene->m_pNext;

                    // リロードするUIなら
                    if (pScene->m_bReloadUI)
                    {
                        // 終了処理
                        pScene->Uninit();
                    }

                    // 使用フラグがfalseなら
                    if (!pScene->m_bUse)
                    {
                        // メモリの開放
                        delete pScene;
                        pScene = NULL;
                    }

                    // 次のシーンに変えていく
                    pScene = pNextScene;

                } while (pScene != NULL);
            }
        }
    }
}
#endif

//==============================================
// シーン上のオブジェクトを全て開放
// Author : 後藤慎之助
//==============================================
void CScene::Release(void)
{
    // 中身があるなら、
    if (this != NULL)
    {
        // 先頭なら
        if (this == m_apTop[m_objType])
        {
            // 次の中身があるなら
            if (m_pNext != NULL)
            {
                // 次のものの前の情報を、NULLに
                m_pNext->m_pPrev = NULL;

                // それを先頭に
                m_apTop[m_objType] = m_pNext;
            }
            else
            {
                // 先頭の次がないなら、先頭と現在を示すポインタをNULLに
                m_apTop[m_objType] = NULL;
                m_apCur[m_objType] = NULL;
            }
        }
        // 先頭でないなら
        else
        {
            // 次の中身があるなら
            if (m_pNext != NULL)
            {
                // 前のものの次の情報を、自身の次の情報に
                m_pPrev->m_pNext = m_pNext;

                // 次のものの前の情報を、自身の前の情報に
                m_pNext->m_pPrev = m_pPrev;
            }
            else
            {
                // 前のものの次の情報を、なくす
                m_pPrev->m_pNext = NULL;

                // 現在の最新のものを、前の情報に
                m_apCur[m_objType] = m_pPrev;
            }
        }

        //// メモリの開放(リスト構造はm_pNextでシーンを切り替えていく際に支障が出るので、ここでは行わない)
        //delete this;
        ////this = NULL;

        // 使用フラグをfalseに
        m_bUse = false;

        // 種類ごとの総数を減らす
        m_nNumAll[m_objType]--;

        // 全体の数をデクリメント
        m_nNumObjectAll--;
    }
}
