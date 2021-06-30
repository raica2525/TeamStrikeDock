//===============================================
//
// スコアの処理 (score.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "score.h"
#include "number.h"

//========================================
// マクロ定義
//========================================
#define SCORE_NUMBER_SIZE_X 20.0f                                 // スコアの数字の横の大きさ
#define NUMBER_SIZE D3DXVECTOR3(SCORE_NUMBER_SIZE_X, 40.0f, 0.0f) // スコアの数字の大きさ

//========================================
// 静的メンバ変数宣言
//========================================
CNumber *CScore::m_apNumber[MAX_NUMBER] = {};

//========================================
// スコアのコンストラクタ
// Author : 後藤慎之助
//========================================
CScore::CScore() :CScene(OBJTYPE::OBJTYPE_UI)
{
    m_nScore = 0;
}

//========================================
// スコアのデストラクタ
// Author : 後藤慎之助
//========================================
CScore::~CScore()
{

}

//========================================
// スコアの生成
// Author : 後藤慎之助
//========================================
CScore* CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CScore *pScore = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pScore = new CScore;

    // 初期化
    pScore->Init(pos, size);

    return pScore;
}

//========================================
// スコアの初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 左上のスコア
    // 大きい位から、それぞれCreate
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 1) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 3) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 5) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[3] = CNumber::Create(D3DXVECTOR3(pos.x - size.x * (MAX_SCORE - 7) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[4] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 7) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[5] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 5) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[6] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 3) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[7] = CNumber::Create(D3DXVECTOR3(pos.x + size.x * (MAX_SCORE - 1) / MAX_SCORE / 2, pos.y, 0.0f),
        D3DXVECTOR3(size.x / 8, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// スコアの終了処理
// Author : 後藤慎之助
//========================================
void CScore::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            // 数字情報の終了処理
            m_apNumber[nCntNumber]->Uninit();

            // 数字情報のメモリの開放
            ReleaseNumber(nCntNumber);
        }
    }

    // オブジェクトの破棄
    Release();
}

//========================================
// スコアクラスが持っている、数字の開放処理
// Author : 後藤慎之助
//========================================
void CScore::ReleaseNumber(int nCntNumber)
{
    // メモリの開放
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// スコアの更新処理
// Author : 後藤慎之助
//========================================
void CScore::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
    {
        // 変数宣言
        bool bUse = true;   // 使用中

                            // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            bUse = m_apNumber[nCntNumber]->Update();

            // 未使用になったら
            if (bUse == false)
            {
                // 数字情報のメモリの開放
                ReleaseNumber(nCntNumber);
            }
        }
    }
}

//========================================
// スコアの描画処理
// Author : 後藤慎之助
//========================================
void CScore::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// スコアの表示処理
// Author : 後藤慎之助
//========================================
void CScore::SetScore(const int nDisplayScore, D3DXVECTOR3 pos, D3DXCOLOR col)
{
    // 変数宣言
    int nIndex;		                      // 指数　(スコア表示用)
    int nRadix = 10;	                  // 基数　(スコア表示用)
    float fSpace = -SCORE_NUMBER_SIZE_X;  // 数字の間の間隔(若干左へずらして、倒した敵の真ん中辺りに見せる)
    bool bCheckStart = false;             // 数字が始まるまでのチェック(左から右へと数字を見るため)(例:00000500などをなくす)

                                          // 各桁について計算
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);
        int nAnswer = nDisplayScore % nScore / nScore2;

        // 0以外の数字が入ると
        if (nAnswer > 0)
        {
            // 数字開始のチェックをtrue
            bCheckStart = true;
        }

        // 数字が開始しているなら
        if (bCheckStart == true)
        {
            // スコアが持っている、数字の配列を参照
            for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
            {
                // 空なら
                if (m_apNumber[nCntNumber] == NULL)
                {
                    // 配列に入れる
                    m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(pos.x + fSpace, pos.y, 0.0f), NUMBER_SIZE, col, true);

                    // 番号を設定
                    m_apNumber[nCntNumber]->SetNumber(nAnswer);

                    // 間隔を加算
                    fSpace += SCORE_NUMBER_SIZE_X;

                    // 空いているメモリ番地を取得したらforを抜ける
                    break;
                }
            }
        }
    }
}

//========================================
// スコアの加算処理
// Author : 後藤慎之助
//========================================
void CScore::AddScore(const int nValue)
{
    // スコア加算
    m_nScore += nValue;

    // 変数宣言
    int nIndex;		        // 指数　(スコア表示用)
    int nRadix = 10;	    // 基数　(スコア表示用)

                            // 各桁について計算
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);

        // 
        int nAnswer = m_nScore % nScore / nScore2;

        // 番号を設定
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// スコアの設定処理(主に、コンティニュー時に使う)
// Author : 後藤慎之助
//========================================
void CScore::SetScore(const int nScore)
{
    // スコアを結びつける
    m_nScore = nScore;

    // 変数宣言
    int nIndex;		        // 指数　(スコア表示用)
    int nRadix = 10;	    // 基数　(スコア表示用)

                            // 各桁について計算
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);

        // 
        int nAnswer = m_nScore % nScore / nScore2;

        // 番号を設定
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// ダメージの表示処理
// Author : 後藤慎之助
//========================================
void CScore::SetDamage(const int nDisplayDamage, D3DXVECTOR3 pos, D3DXCOLOR col)
{
    // 変数宣言
    int nIndex;		                      // 指数　(スコア表示用)
    int nRadix = 10;	                  // 基数　(スコア表示用)
    float fSpace = -SCORE_NUMBER_SIZE_X;  // 数字の間の間隔(若干左へずらして、倒した敵の真ん中辺りに見せる)
    bool bCheckStart = false;             // 数字が始まるまでのチェック(左から右へと数字を見るため)(例:00000500などをなくす)

                                          // 各桁について計算
    for (nIndex = 0; nIndex < MAX_SCORE; nIndex++)
    {
        int nScore = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex);
        int nScore2 = (int)powf((float)nRadix, MAX_SCORE - (float)nIndex - 1);
        int nAnswer = nDisplayDamage % nScore / nScore2;

        // 0以上の数字が入ると
        if (nAnswer > 0)
        {
            // 数字開始のチェックをtrue
            bCheckStart = true;
        }

        // 0ダメージの時
        if (nDisplayDamage == 0)
        {
            // 数字開始のチェックをtrue
            bCheckStart = true;
        }

        // 数字が開始しているなら
        if (bCheckStart == true)
        {
            // スコアが持っている、数字の配列を参照
            for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
            {
                // 空なら
                if (m_apNumber[nCntNumber] == NULL)
                {
                    // 配列に入れる
                    m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(pos.x + fSpace, pos.y, 0.0f), NUMBER_SIZE, col, true);

                    // 番号を設定
                    m_apNumber[nCntNumber]->SetNumber(nAnswer);

                    // 間隔を加算
                    fSpace += SCORE_NUMBER_SIZE_X;

                    // 空いているメモリ番地を取得したらforを抜ける
                    break;
                }
            }
        }

        // 0ダメージの時
        if (nDisplayDamage == 0)
        {
            // forを抜ける
            break;
        }
    }
}
