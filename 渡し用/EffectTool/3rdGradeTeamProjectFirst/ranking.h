//====================================================================
//
// ランキング処理 (ranking.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CNumber;

//================================================
// マクロ定義
//================================================
#define RANKING_FIRST_POS D3DXVECTOR3(SCREEN_WIDTH / 2 + 30.0f, 625.0f, 0.0f)   // ランキングの最初の位置
#define MAX_RANKING_NUMBER 48                                                   // ランキングの数字の最大数(自分のスコアと、8桁*5位まで)

#define RANKING_SCORE_MAX (6)   //5位まで(内部で6つの値を計算するため6)

//================================================
// クラス宣言
//================================================

// ランキングのクラス
class CRanking : public CScene
{
public:
    CRanking();
    ~CRanking();
    static CRanking *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // 数字情報開放
    void Update(void);
    void Draw(void);

    void SetNumber(int nPlayScore, int nStartIndex);    // スコアをナンバーに渡して表示する関数
    void ReadScore(void);                               // 外部ファイルからスコアを読み込む
    bool RankIn(int nScore);                            // ランクインしたかどうか

    void FadeInNumber(int nCnt);                        // フェードインする数字
    int SwapScoreNumberIndex(int nIndex);               // スコアの数字のインデックスを入れ替える

private:
    static CNumber *m_apNumber[MAX_RANKING_NUMBER];  // 数字情報
    int m_nCntTime;                                  // 時間を数える
    int m_aRankingScore[RANKING_SCORE_MAX];		     // 比較するスコア
    int m_aRank[RANKING_SCORE_MAX];                  // ランク付けするため
    float m_aAlpha[MAX_RANKING_NUMBER];              // 各数字のα値
};

#endif