//===============================================
//
// 数字配列の処理 (number_array.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "number_array.h"
#include "number.h"

//========================================
// 数字配列のコンストラクタ
// Author : 後藤慎之助
//========================================
CNumberArray::CNumberArray() :CScene(OBJTYPE::OBJTYPE_UI_FRONT_TEXT)
{
    m_nTexType = 0;
    memset(m_apNumber, 0, sizeof(m_apNumber));

    m_createPos = DEFAULT_VECTOR;
    m_createColor = DEFAULT_COLOR;
    m_bDispUselessDigits = true;
    m_fSizeX = 0.0f;
}

//========================================
// 数字配列のデストラクタ
// Author : 後藤慎之助
//========================================
CNumberArray::~CNumberArray()
{

}

//========================================
// 数字配列の生成
// Author : 後藤慎之助
//========================================
CNumberArray* CNumberArray::Create(int nTexType, D3DXVECTOR3 pos, float fSizeX, D3DXCOLOR col, int nDispNumber, bool bDispUselessDigits, bool bMove)
{
    CNumberArray *pNumberArray = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pNumberArray = new CNumberArray;

    // 引数を結びつける
    pNumberArray->m_nTexType = nTexType;
    pNumberArray->m_createPos = pos;
    pNumberArray->m_fSizeX = fSizeX;
    pNumberArray->m_createColor = col;
    pNumberArray->m_bDispUselessDigits = bDispUselessDigits;

    // 初期化
    pNumberArray->Init(pos, DEFAULT_VECTOR);

    // 数字を入れる
    pNumberArray->SetDispNumber(nDispNumber, bMove);

    return pNumberArray;
}

//========================================
// 数字配列の初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CNumberArray::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{

    return S_OK;
}

//========================================
// 数字配列の終了処理
// Author : 後藤慎之助
//========================================
void CNumberArray::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER_ARRAY; nCntNumber++)
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
// 数字配列クラスが持っている、数字の開放処理
// Author : 後藤慎之助
//========================================
void CNumberArray::ReleaseNumber(int nCntNumber)
{
    // メモリの開放
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// 数字配列の更新処理
// Author : 後藤慎之助
//========================================
void CNumberArray::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER_ARRAY; nCntNumber++)
    {
        // 変数宣言
        bool bUse = true;

        // 中身があるなら
        if (m_apNumber[nCntNumber])
        {
            bUse = m_apNumber[nCntNumber]->Update();

            // 未使用になったら
            if (!bUse)
            {
                // 数字情報のメモリの開放
                ReleaseNumber(nCntNumber);
            }
        }
    }
}

//========================================
// 数字配列の描画処理
// Author : 後藤慎之助
//========================================
void CNumberArray::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_NUMBER_ARRAY; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// 表示する数字を設定
// Author : 後藤慎之助
//========================================
void CNumberArray::SetDispNumber(int nDispNumber, bool bMove)
{
    // 変数宣言
    int nIndex;		                      // 指数　(数字配列表示用)
    int nRadix = 10;	                  // 基数　(数字配列表示用)
    float fSpace = 0.0f;                  // 数字の間の間隔
    bool bCheckStart = false;             // 数字が始まるまでのチェック(左から右へと数字を見るため)(例:00000500などをなくす)
    bool bDispZero = false;               // 0の配置が終わったかどうか
    int nNumDigit = 0;                    // 桁数
    bool bGetDigit = false;               // 桁数を受け取ったかどうか

    // 奇数か偶数によって、間隔のスタートを変える
    if (MAX_NUMBER_ARRAY % 2 == 0)
    {
        fSpace = -(m_fSizeX * ((float)MAX_NUMBER_ARRAY * 0.5f)) + (m_fSizeX * 0.5f);
    }
    else
    {
        fSpace = -(m_fSizeX * ((float)MAX_NUMBER_ARRAY * 0.5f - 0.5f));
    }

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_NUMBER_ARRAY; nIndex++)
    {
        int nNumberArray = (int)powf((float)nRadix, MAX_NUMBER_ARRAY - (float)nIndex);
        int nNumberArray2 = (int)powf((float)nRadix, MAX_NUMBER_ARRAY - (float)nIndex - 1);
        int nAnswer = nDispNumber % nNumberArray / nNumberArray2;

        // 生成されてないなら、配列に入れる
        if (!m_apNumber[nIndex])
        {
            m_apNumber[nIndex] = CNumber::Create(m_nTexType, D3DXVECTOR3(m_createPos.x + fSpace, m_createPos.y, 0.0f),
                D3DXVECTOR3(m_fSizeX, m_fSizeX * 2.0f, 0.0f), m_createColor, bMove, m_createPos);
        }
        m_apNumber[nIndex]->SetAlpha(0.0f);   // 一度見えないように
        fSpace += m_fSizeX;        // 数字の大きさ分ずらす

        // 無駄な桁を表示するかどうか
        if (!m_bDispUselessDigits)
        {
            // 0以上の数字が入ると
            if (nAnswer > 0)
            {
                // 数字開始のチェックをtrue
                bCheckStart = true;
            }

            // 0の時
            if (nDispNumber == 0)
            {
                // 数字開始のチェックをtrue
                bCheckStart = true;
            }
        }
        else
        {
            bCheckStart = true;
        }

        // 数字が開始しているなら
        if (bCheckStart)
        {
            // 0の時
            if (nDispNumber == 0)
            {
                // 無駄な桁も表示するなら
                if (m_bDispUselessDigits)
                {
                    m_apNumber[nIndex]->SetAlpha(1.0f);
                    m_apNumber[nIndex]->SetNumber(nAnswer);
                }
                else
                {
                    // 無駄な桁を表示しないなら
                    if (!bDispZero)
                    {
                        bDispZero = true;
                        m_apNumber[nIndex]->SetAlpha(1.0f);
                        m_apNumber[nIndex]->SetNumber(nAnswer);
                    }
                }
            }
            else
            {
                // 無駄な桁を表示していないなら、位置調整
                if (!m_bDispUselessDigits)
                {
                    // 桁数を受け取っていないなら、受け取る
                    if (!bGetDigit)
                    {
                        bGetDigit = true;
                        nNumDigit = MAX_NUMBER_ARRAY - nIndex;

                        // 奇数か偶数によって、間隔のスタートを変える
                        if (nNumDigit % 2 == 0)
                        {
                            // 偶数
                            fSpace = -(m_fSizeX * ((float)nNumDigit * 0.5f)) + (m_fSizeX * 0.5f);
                        }
                        else
                        {
                            // 奇数
                            fSpace = -(m_fSizeX * ((float)nNumDigit * 0.5f - 0.5f));
                        }
                    }

                    // 桁周りの情報を調整
                    m_apNumber[nIndex]->SetDigitInfo(nNumDigit, MAX_NUMBER_ARRAY - nIndex);

                    // 位置を反映
                    m_apNumber[nIndex]->SetPos(D3DXVECTOR3(m_createPos.x + fSpace, m_createPos.y, 0.0f));
                }

                // 番号を設定
                m_apNumber[nIndex]->SetAlpha(1.0f);
                m_apNumber[nIndex]->SetNumber(nAnswer); // ここで頂点情報の更新も行っているので、これより前に位置など調整する
            }
        }
    }
}