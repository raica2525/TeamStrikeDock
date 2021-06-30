//===============================================
//
// ビルボード処理 (billboard.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// 静的メンバ変数宣言
//========================================
bool CBillboard::m_bAdditiveSynthesis = false;
bool CBillboard::m_bUseZBuffer = true;

//=============================================================================
// オーバーライドされたコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CBillboard::CBillboard(CScene::OBJTYPE objtype) :CScene(objtype)
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_scale = DEFAULT_SCALE;
    m_col = DEFAULT_COLOR;
    m_fTexX1 = 0.0f;
    m_fTexX2 = 1.0f;
    m_fTexY1 = 0.0f;
    m_fTexY2 = 1.0f;
    m_nAlphaTestBorder = DEFAULT_ALPHA_TEST_BORDER;
}

//=============================================================================
// デフォルトコンストラクタ
// Author : 後藤慎之助
//=============================================================================
CBillboard::CBillboard()
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    m_rot = DEFAULT_VECTOR;
    m_scale = DEFAULT_SCALE;
    m_col = DEFAULT_COLOR;
    m_fTexX1 = 0.0f;
    m_fTexX2 = 1.0f;
    m_fTexY1 = 0.0f;
    m_fTexY2 = 1.0f;
    m_nAlphaTestBorder = DEFAULT_ALPHA_TEST_BORDER;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CBillboard::~CBillboard()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // デバイスを取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 位置、スケール、大きさ、色を初期化
    m_pos = pos;
    m_scale = DEFAULT_SCALE;
    m_size = size;
    m_col = DEFAULT_COLOR;

    // 頂点バッファを生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    // 頂点バッファをロック
    VERTEX_3D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 各頂点の位置を初期化
    pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2, -m_size.y / 2, 0.0f);

    // 各頂点のテクスチャ座標を初期化
    pVtx[0].tex = D3DXVECTOR2(m_fTexX1, m_fTexY1);
    pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY1);
    pVtx[2].tex = D3DXVECTOR2(m_fTexX1, m_fTexY2);
    pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);

    // 各頂点の色と法線ベクトルを初期化
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = DEFAULT_COLOR;
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    }

    // 頂点バッファをアンロック
    m_pVtxBuff->Unlock();

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CBillboard::Uninit(void)
{
    // 頂点バッファを開放
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // シーンの開放
    Release();
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CBillboard::Update(void)
{
    // 頂点バッファをロック
    VERTEX_3D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 各頂点の位置を更新
    pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2, -m_size.y / 2, 0.0f);

    // 各頂点のテクスチャ座標を更新
    pVtx[0].tex = D3DXVECTOR2(m_fTexX1, m_fTexY1);
    pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY1);
    pVtx[2].tex = D3DXVECTOR2(m_fTexX1, m_fTexY2);
    pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);

    // 各頂点の色と法線ベクトルを更新
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = m_col;
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    }

    // 頂点バッファをアンロック
    m_pVtxBuff->Unlock();
}

//=============================================================================
// 回転を考慮した更新処理
// Author : 後藤慎之助
//=============================================================================
void CBillboard::Update(float fAngle)
{
    // 変数宣言
    // 各頂点
    D3DXVECTOR3 vertex1;
    D3DXVECTOR3 vertex2;
    D3DXVECTOR3 vertex3;
    D3DXVECTOR3 vertex4;

    //==========================================================================================================
    //画像を、画像の中心を軸に回転させる
    //左上の頂点
    vertex1.x = -(m_size.x / 2)*cosf(fAngle)
        - ((m_size.y / 2))*sinf(fAngle);
    vertex1.y = -(m_size.x / 2)*sinf(fAngle)
        + ((m_size.y / 2))*cosf(fAngle);
    vertex1.z = 0.0f;

    //右上の頂点
    vertex2.x = (m_size.x / 2)*cosf(fAngle)
        - ((m_size.y / 2))*sinf(fAngle);
    vertex2.y = (m_size.x / 2)*sinf(fAngle)
        + ((m_size.y / 2))*cosf(fAngle);
    vertex2.z = 0.0f;

    //左下の頂点
    vertex3.x = -(m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex3.y = -(m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);
    vertex3.z = 0.0f;

    //右下の頂点
    vertex4.x = (m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex4.y = (m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);
    vertex4.z = 0.0f;
    //==========================================================================================================

    VERTEX_3D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // 頂点座標の設定
    pVtx[0].pos = vertex1;
    pVtx[1].pos = vertex2;
    pVtx[2].pos = vertex3;
    pVtx[3].pos = vertex4;

    // 各頂点のテクスチャ座標を更新
    pVtx[0].tex = D3DXVECTOR2(m_fTexX1, m_fTexY1);
    pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY1);
    pVtx[2].tex = D3DXVECTOR2(m_fTexX1, m_fTexY2);
    pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);

    // 各頂点の色と法線ベクトルを更新
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = m_col;
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    }

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
// Author : 後藤慎之助
//=============================================================================
void CBillboard::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;

    // Zバッファを使用するフラグがfalseなら
    if (m_bUseZBuffer == false)
    {
        // Zバッファを無効化
        pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    }

    // ライトを無効に
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaTestBorder);

    //アルファテストの比較方法の設定
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    // 加算合成
    if (m_bAdditiveSynthesis == true)
    {
        // レンダーステート(加算合成にする)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    // ワールドマトリクスの初期化
    D3DXMatrixIdentity(&m_mtxWorld);

    // 回転を決めてないなら、カメラに対して常に正面
    if (m_rot == DEFAULT_VECTOR)
    {
        D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
        pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
        D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);

        mtxRot._41 = 0.0f;
        mtxRot._42 = 0.0f;
        mtxRot._43 = 0.0f;

        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

        D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
    }
    else
    {
        // 向きを反映
        D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

        // 位置を反映
        D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
    }

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

    pDevice->SetFVF(FVF_VERTEX_3D);

    pDevice->SetTexture(0, m_pTexture);

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // ライトを有効化
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    // テクスチャをNULLに
    pDevice->SetTexture(0, NULL);

    // 加算合成を戻す
    if (m_bAdditiveSynthesis == true)
    {
        // レンダーステート(加算合成を戻す)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // 加算合成のフラグをfalseに戻す
        m_bAdditiveSynthesis = false;
    }

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    // Zバッファを使用するフラグがfalseなら
    if (m_bUseZBuffer == false)
    {
        // Zバッファを有効化
        pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

        // Zバッファを無効化するフラグをtrueに戻す
        m_bUseZBuffer = true;
    }
}

//=============================================================================
// テクスチャをバインドする処理
// Author : 後藤慎之助
//=============================================================================
void CBillboard::BindTexture(const int nNumTexture)
{
    CTexture *pTexture = CManager::GetTexture();
    m_pTexture = pTexture->GetInfo(nNumTexture)->pTexture;
}

//=============================================================================
// テクスチャ座標を更新する処理
// Author : 後藤慎之助
//=============================================================================
bool CBillboard::SetUpdateTexLeftToRight(int nMaxPattern)
{
    // 一周したかどうか
    bool bOneRound = false;

    // テクスチャ座標を更新する割合
    float fUpdateRate = 0.0f;

    // パターンによって、更新量を変える
    fUpdateRate = 1.0f / (float)nMaxPattern;

    // それぞれのテクスチャ座標Xに加える
    m_fTexX1 += fUpdateRate;
    m_fTexX2 += fUpdateRate;

    // テクスチャ座標の右側が1.0fを超えたら
    if (m_fTexX2 >= 1.0f)
    {
        // 一周のフラグをtrueに
        bOneRound = true;
    }

    return bOneRound;
}
