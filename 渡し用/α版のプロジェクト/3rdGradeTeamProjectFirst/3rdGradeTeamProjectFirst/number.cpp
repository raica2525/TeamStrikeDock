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
#define MOVE_RATE 1.0f      // 上へ行く移動量
#define FADE_RATE 0.025f    // 消えていく量

//============================================================
// 数字表示のデフォルトコンストラクタ
// Author : 後藤慎之助
//============================================================
CNumber::CNumber()
{
    m_pVtxBuff = NULL;
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_bMove = false;
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
    pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * 0), 0.0f);
    pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * 0), 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * 0), 1.0f);
    pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * 0), 1.0f);

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
    bool bUse = true;   // 使用中に

                        // 動くなら
    if (m_bMove == true)
    {
        // 徐々に上に
        m_pos.y -= MOVE_RATE;

        // だんだん透明に
        m_col.a -= FADE_RATE;
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
    pDevice->SetTexture(0, CManager::GetTexture()->GetInfo(12)->pTexture);

    // ポリゴンの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================
// 数字表示の生成
// Author : 後藤慎之助
//=============================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool bMove)
{
    CNumber *pNumber = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pNumber = new CNumber;

    // 初期化
    pNumber->m_col = col;
    pNumber->Init(pos, size);

    // メンバ変数を結びつける
    pNumber->m_bMove = bMove;

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
    pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 0.0f);
    pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 1.0f);
    pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 1.0f);

    // 頂点カラーを設定（0.0f～1.0fの値で設定する）
    pVtx[0].col = m_col;
    pVtx[1].col = m_col;
    pVtx[2].col = m_col;
    pVtx[3].col = m_col;

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}
