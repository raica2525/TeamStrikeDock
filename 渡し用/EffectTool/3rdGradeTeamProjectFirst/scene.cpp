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

//========================================
// 静的メンバ変数宣言
//========================================
int CScene::m_nNumAll[CScene::OBJTYPE_MAX] = {};
int CScene::m_nNumObjectAll = 0;

CScene *CScene::m_pTop[CScene::OBJTYPE_MAX] = {};
CScene *CScene::m_pCur[CScene::OBJTYPE_MAX] = {};

//========================================
// シーン上のオブジェクトのデフォルトコンストラクタ
// Author : 後藤慎之助
//========================================
CScene::CScene()
{
    // オブジェクトの種類をNULLにする
    m_objType = OBJTYPE_NONE;

    // 先頭がないなら、先頭に
    if (m_pTop[m_objType] == NULL)
    {
        m_pTop[m_objType] = this;
    }

    // 現在における最新のオブジェクトがないなら、最新に
    if (m_pCur[m_objType] == NULL)
    {
        m_pCur[m_objType] = this;
    }

    // 現在のオブジェクトの次のオブジェクトを、自分にする
    m_pCur[m_objType]->m_pNext = this;

    // 現在のオブジェクトが自分の場合
    if (m_pCur[m_objType] == this)
    {
        // 自分の前のオブジェクトを、NULLにする
        m_pPrev = NULL;
    }
    else
    {
        // 自分の前のオブジェクトを、現在のオブジェクトにする
        m_pPrev = m_pCur[m_objType];
    }

    // 現在のオブジェクトを、自分にする
    m_pCur[m_objType] = this;

    // 自分の次のオブジェクトを、NULLにする
    m_pNext = NULL;

    // 増えたオブジェクトをカウント
    m_nNumAll[m_objType]++;

    // 全体の数をインクリメント
    m_nNumObjectAll++;

    // 使用するフラグをtrueに
    m_bUse = true;
}

//========================================
// シーン上のオブジェクトのオーバーライドされたコンストラクタ
// Author : 後藤慎之助
//========================================
CScene::CScene(OBJTYPE objType)
{
    // オブジェクトの種類を引数と結びつける
    m_objType = objType;

    // 先頭がないなら、先頭に
    if (m_pTop[m_objType] == NULL)
    {
        m_pTop[m_objType] = this;
    }

    // 現在における最新のオブジェクトがないなら、最新に
    if (m_pCur[m_objType] == NULL)
    {
        m_pCur[m_objType] = this;
    }

    // 現在のオブジェクトの次のオブジェクトを、自分にする
    m_pCur[m_objType]->m_pNext = this;

    // 現在のオブジェクトが自分の場合
    if (m_pCur[m_objType] == this)
    {
        // 自分の前のオブジェクトを、NULLにする
        m_pPrev = NULL;
    }
    else
    {
        // 自分の前のオブジェクトを、現在のオブジェクトにする
        m_pPrev = m_pCur[m_objType];
    }

    // 現在のオブジェクトを、自分にする
    m_pCur[m_objType] = this;

    // 自分の次のオブジェクトを、NULLにする
    m_pNext = NULL;

    // 増えたオブジェクトをカウント
    m_nNumAll[m_objType]++;

    // 全体の数をインクリメント
    m_nNumObjectAll++;

    // 使用するフラグをtrueに
    m_bUse = true;
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
        if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
        {
            // 記憶用の変数
            CScene*pScene = m_pTop[nCnt];

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
// シーン上のオブジェクトを全て描画
// Author : 後藤慎之助
//==============================================
void CScene::DrawAll(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        // ステンシルエフェクト以外なら
        if (nCnt != OBJTYPE_EFFECT3D_STENCIL)
        {
            // 先頭、最新のものがあるなら
            if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_pTop[nCnt];

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
        if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
        {
            // 記憶用の変数
            CScene*pScene = m_pTop[nCnt];

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
// シーン上の敵、弾、床、壁、ブロック、アイテムを全て開放
// Author : 後藤慎之助
//==============================================
void CScene::ReleaseStage(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        if (nCnt == OBJTYPE_BG || nCnt == OBJTYPE_EFFECT3D_Z_NONE || nCnt == OBJTYPE_BLOCK ||
            nCnt == OBJTYPE_MODEL_EFFECT || nCnt == OBJTYPE_ITEM ||
            nCnt == OBJTYPE_ENEMY)
        {
            // 先頭、最新のものがあるなら
            if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_pTop[nCnt];

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
}

//==============================================
// シーン上のUIを全て開放
// Author : 後藤慎之助
//==============================================
void CScene::ReleaseUI(void)
{
    for (int nCnt = 0; nCnt < OBJTYPE_MAX; nCnt++)
    {
        if (nCnt == OBJTYPE_UI)
        {
            // 先頭、最新のものがあるなら
            if (m_pTop[nCnt] != NULL && m_pCur[nCnt] != NULL)
            {
                // 記憶用の変数
                CScene*pScene = m_pTop[nCnt];

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
}

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
        if (this == m_pTop[m_objType])
        {
            // 次の中身があるなら
            if (m_pNext != NULL)
            {
                // 次のものの前の情報を、NULLに
                m_pNext->m_pPrev = NULL;

                // それを先頭に
                m_pTop[m_objType] = m_pNext;
            }
            else
            {
                // 先頭の次がないなら、先頭と現在を示すポインタをNULLに
                m_pTop[m_objType] = NULL;
                m_pCur[m_objType] = NULL;
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
                m_pCur[m_objType] = m_pPrev;
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
