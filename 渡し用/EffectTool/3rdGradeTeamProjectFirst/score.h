//====================================================================
//
// スコア処理 (score.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

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
#define MAX_SCORE 8                                                         // スコアの桁数
#define MAX_NUMBER 16                                                       // 一画面における、数字の最大数

#define SCORE_POS (D3DXVECTOR3(530.0f - (300.0 / 2), 528.5f, 0.0f))         // スコアの位置
#define SCORE_SIZE D3DXVECTOR3(150.0f, 30.0f, 0.0f)                         // スコアの大きさ

//================================================
// クラス宣言
//================================================

// スコアのクラス
class CScore : public CScene
{
public:
    CScore();
    ~CScore();
    static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // 数字情報開放
    void Update(void);
    void Draw(void);
    void SetScore(const int nDisplayScore, D3DXVECTOR3 pos, D3DXCOLOR col);  // 数だけ出すとき
    void AddScore(const int nValue);                                         // スコア加算するとき
    int GetScore(void) { return m_nScore; }                                  // スコアを取得
    void SetScore(const int nScore);                                         // スコアを設定
    void SetDamage(const int nDisplayDamage, D3DXVECTOR3 pos, D3DXCOLOR col);// ダメージを設定

private:
    static CNumber *m_apNumber[MAX_NUMBER];  // 数字情報
    int m_nScore;                            // スコア
};

#endif