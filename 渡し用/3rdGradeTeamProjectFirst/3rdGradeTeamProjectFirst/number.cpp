//===============================================
//
// 数字表示処理(オブジェクトを継承していない)  (number.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"

//=======================================
// マクロ定義
//=======================================
#define MOVE_RATE 2.5f          // 上へ行く移動量
#define FADE_RATE 0.1f          // 消えていく量
#define EXTEND_RATE 5.0f        // 拡大量（X軸基準）
#define MOVE_STOP_TIME 60       // 動きが止まるカウンタ
#define FADE_OUT_START_TIME 120 // フェードアウト開始カウンタ

//============================================================
// 数字表示のデフォルトコンストラクタ
// Author : 後藤慎之助
//============================================================
CNumber::CNumber()
{
    m_nTexType = 0;
    m_pVtxBuff = NULL;
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;

    m_bMove = false;
    m_nCntTime = 0;
    m_nNumDigit = 0;
    m_nThisDigit = 0;
    m_centerPos = DEFAULT_VECTOR;
}

//============================================================
// 数字表示のデストラクタ
// Author : 後藤慎之助
//============================================================
CNumber::~CNumber()
{

}

//============================================================
// 数字表示の初期化処理
// Author : 後藤慎之助
//============================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 位置を結びつける
    m_pos = pos;

    // 大きさを結びつける
    m_size = size;

    // 頂点バッファ生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_2D *pVtx = NULL;	//頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

                                                // 頂点座標の設定（右回りで設定する）
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), +(m_size.y / 2), 0.0f);

    // rhwの設定（値は1.0で設定する）
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // 頂点カラーを設定（0.0f～1.0fの値で設定する）
    pVtx[0].col = m_col;
    pVtx[1].col = m_col;
    pVtx[2].col = m_col;
    pVtx[3].col = m_col;

    // テクスチャ座標の設定
    // ここでは、0に合わせている
    pVtx[0].tex[0] = D3DXVECTOR2(0.0f + (0.1f * 0), 0.0f);
    pVtx[1].tex[0] = D3DXVECTOR2(0.1f + (0.1f * 0), 0.0f);
    pVtx[2].tex[0] = D3DXVECTOR2(0.0f + (0.1f * 0), 1.0f);
    pVtx[3].tex[0] = D3DXVECTOR2(0.1f + (0.1f * 0), 1.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();

    return S_OK;
}

//============================================================
// 数字表示の終了処理
// Author : 後藤慎之助
//============================================================
void CNumber::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // オブジェクトの派生ではない
    //// オブジェクトの破棄
    //Release();
}

//============================================================
// 数字表示の更新処理
// Author : 後藤慎之助
//============================================================
bool CNumber::Update(void)
{
    // 変数宣言
    bool bUse = true;

    // 動くなら
    if (m_bMove)
    {
        // 動いている時間をカウント
        m_nCntTime++;

        // カウンタによって、行う処理を変える
        if (m_nCntTime < MOVE_STOP_TIME)
        {
            // 徐々に上に
            m_pos.y -= MOVE_RATE;
        }
        else if (m_nCntTime > FADE_OUT_START_TIME)
        {
            // だんだん透明に
            m_col.a -= FADE_RATE;

            // サイズ拡大
            if (m_size.y != 0.0f)
            {
                m_size.y += (m_size.y / m_size.x) * EXTEND_RATE;
            }
            m_size.x += EXTEND_RATE;

            // サイズ拡大による位置調整
            float fSpace = 0.0f;
            if (m_nNumDigit % 2 == 0)
            {
                fSpace = -(m_size.x * ((float)m_nNumDigit * 0.5f)) + (m_size.x * 0.5f);
            }
            else
            {
                fSpace = -(m_size.x * ((float)m_nNumDigit * 0.5f - 0.5f));
            }
            fSpace += m_size.x * (m_nNumDigit - m_nThisDigit);  // 左から右に向かって生成してあるため、数える際は（桁数-現在の桁)
            m_pos.x = m_centerPos.x + fSpace;
        }
    }

    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // 頂点座標の設定
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), +(m_size.y / 2), 0.0f);

    // 頂点カラーを設定（0.0f～1.0fの値で設定する）
    pVtx[0].col = m_col;
    pVtx[1].col = m_col;
    pVtx[2].col = m_col;
    pVtx[3].col = m_col;

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();

    // 完全に透明になったら
    if (m_col.a < 0.0f)
    {
        // 数字終了処理
        Uninit();

        // 未使用に
        bUse = false;
    }

    return bUse;
}

//=============================================================
// 数字表示の描画処理
// Author : 後藤慎之助
//=============================================================
void CNumber::Draw(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    // テクスチャの設定（数字は12番）
    pDevice->SetTexture(0, CManager::GetTexture()->GetInfo(m_nTexType)->pTexture);

    // ポリゴンの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================
// 数字表示の生成
// Author : 後藤慎之助
//=============================================================
CNumber *CNumber::Create(int nTexType, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool bMove, D3DXVECTOR3 centerPos)
{
    CNumber *pNumber = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pNumber = new CNumber;

    // 初期化
    pNumber->m_col = col;
    pNumber->Init(pos, size);

    // メンバ変数を結びつける
    pNumber->m_nTexType = nTexType;
    pNumber->m_bMove = bMove;
    pNumber->m_centerPos = centerPos;

    return pNumber;
}

//=============================================================
// 数字表示の番号設定
// Author : 後藤慎之助
//=============================================================
void CNumber::SetNumber(int nNumber)
{
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // テクスチャの座標を反映
    pVtx[0].tex[0] = D3DXVECTOR2(0.0f + (0.1f * nNumber), 0.0f);
    pVtx[1].tex[0] = D3DXVECTOR2(0.1f + (0.1f * nNumber), 0.0f);
    pVtx[2].tex[0] = D3DXVECTOR2(0.0f + (0.1f * nNumber), 1.0f);
    pVtx[3].tex[0] = D3DXVECTOR2(0.1f + (0.1f * nNumber), 1.0f);

    // 頂点カラーを設定（0.0f～1.0fの値で設定する）
    pVtx[0].col = m_col;
    pVtx[1].col = m_col;
    pVtx[2].col = m_col;
    pVtx[3].col = m_col;

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}
