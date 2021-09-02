//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 後藤慎之助
//
//=============================================================================

//========================
// インクルードファイル
//========================
#include "fade.h"
#include "renderer.h"
#include "manager.h"

//===========================================================
// マクロ定義
//===========================================================
#define FADE_RATE 0.05f;    // フェードの割合

//===========================================================
// 静的メンバ変数宣言
//===========================================================
D3DXCOLOR CFade::m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);     // 黒
CFade::FADE	CFade::m_fade = FADE_NONE;
CManager::MODE	CFade::m_modeNext = CManager::MODE_DEBUG_MENU;
LPDIRECT3DVERTEXBUFFER9 CFade::m_pVtxBuff = NULL;

//===========================================================
// コンストラクタ
// Author : 後藤慎之助
//===========================================================
CFade::CFade()
{

}

//===========================================================
// デストラクタ
// Author : 後藤慎之助
//===========================================================
CFade::~CFade()
{

}

//===========================================================
// 初期化処理
// Author : 後藤慎之助
//===========================================================
HRESULT CFade::Init(CManager::MODE mode)
{
    // 更新処理のSetModeに入るため、フェードアウトから始める(起動時からフェードをする)
    m_fade = FADE_OUT;

    // 引数のモードをメンバ変数に結びつける
    m_modeNext = mode;

    // レンダラーを取得
    CRenderer *pRenderer = CManager::GetRenderer();

    // 頂点バッファの生成
    pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY
        , FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

    // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    VERTEX_2D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 各頂点の位置を設定
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    // 各頂点で共通のrhw、色を設定
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].rhw = 1.0f;
        pVtx[nCount].col = m_col;
    }

    // 頂点データをアンロック
    m_pVtxBuff->Unlock();

    return S_OK;
}

//==============================================================
// 終了処理
// Author : 後藤慎之助
//==============================================================
void CFade::Uninit(void)
{
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
}

//==============================================================
// 更新処理
// Author : 後藤慎之助
//==============================================================
void CFade::Update(void)
{
    switch (m_fade)
    {
    case FADE_OUT:

        // フェードのポリゴンをだんだん見せる
        m_col.a += FADE_RATE;

        // 完全にフェードのポリゴンが表示されたら
        if (m_col.a >= 1.0f)
        {
            // 透明度を固定
            m_col.a = 1.0f;

            // 次のモードにする
            CManager::SetMode(m_modeNext);

            // フェードインする
            m_fade = FADE_IN;
        }

        break;

    case FADE_IN:

        // 次のモードの画面をだんだん見せる
        m_col.a -= FADE_RATE;

        // 完全に次の画面が表示されたら
        if (m_col.a <= 0.0f)
        {
            // 透明度を固定
            m_col.a = 0.0f;

            // フェードを使用していない状態にする
            m_fade = FADE_NONE;
        }

        break;
    }

    // 頂点バッファの更新
    VERTEX_2D* pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 透明度を各頂点に反映
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = m_col;
    }

    // 頂点データをアンロック
    m_pVtxBuff->Unlock();
}

//=============================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================
void CFade::Draw(void)
{
    // レンダラーを取得
    CRenderer *pRenderer = CManager::GetRenderer();

    // 頂点バッファをデータストリームに設定
    pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

    // ポリゴンの描画
    pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================
// フェードアウトを始める処理
// Author : 後藤慎之助
//=============================================================
void CFade::SetFade(CManager::MODE mode)
{
    // フェードしていないなら
    if (m_fade == FADE_NONE)
    {
        // フェードアウトに
        m_fade = FADE_OUT;

        // フェードのポリゴンの色を透明に
        m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

        // 受け取ったモードを次のモードに設定
        m_modeNext = mode;
    }
}

//=============================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================
CFade * CFade::Create(CManager::MODE mode)
{
    // メモリを確保
    CFade *pFade = NULL;
    pFade = new CFade;

    // 初期化
    pFade->Init(mode);

    return pFade;
}