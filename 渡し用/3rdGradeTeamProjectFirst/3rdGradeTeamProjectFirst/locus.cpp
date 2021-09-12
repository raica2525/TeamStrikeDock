//=============================================================================
//
// 軌跡の処理 [locus.cpp]
// Author : 伊藤陽梧
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================	
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "locus.h"
#include "texture.h"
#include <D3dx9math.h>

//=============================================================================
// マクロ定義
//=============================================================================
#define LUCOS_SUBALPHA (0.025f) // 透明度を下げる値
#define LOCUS_TEXTURE_NUM (57)  // 軌跡のテクスチャのナンバー

//=============================================================================
// コンストラクタ
// Author : 伊藤陽梧
//=============================================================================
CLocus::CLocus(CScene::OBJTYPE objtype) : CScene(objtype)
{
    m_pTexture = NULL;
    m_pBuffMat = NULL;
    memset(m_mtxWorld, 0, sizeof(m_mtxWorld));
    m_col = DEFAULT_COLOR;
    m_TypeLucus = TYPE_NONE;
    m_ShrinkSpeed = 0.0f;
}

//=============================================================================
// デストラクタ
// Author : 伊藤陽梧
//=============================================================================
CLocus::~CLocus()
{
}

//=============================================================================
// 生成処理
// pos1, pos2		: 現在のフレームの座標
// posOld1, posOld2	: 1フレーム前の座標
// TypeLucus		: 軌跡の種類
// ShrinkSpeed		: 軌跡の縮まる速度
// Author : 伊藤陽梧
//=============================================================================
CLocus * CLocus::Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2, D3DXCOLOR col, TYPE_LOCUS TypeLucus, float ShrinkSpeed)
{
    // メモリ確保
    CLocus *pLocus = new CLocus(CScene::OBJTYPE_EFFECT3D);

    // NULL確認
    if (pLocus != NULL)
    {
        // 初期化処理
        pLocus->Init(DEFAULT_VECTOR, DEFAULT_VECTOR);

        // 座標設定
        pLocus->SetPos(pos1, pos2, posOld1, posOld2);

        // 座標設定
        pLocus->m_col = col;

        // 軌跡の種類
        pLocus->m_TypeLucus = TypeLucus;

        // 軌跡の縮める速さ
        pLocus->m_ShrinkSpeed = ShrinkSpeed;
    }

    return pLocus;
}

//=============================================================================
// 初期化処理
// Author : 伊藤陽梧
//=============================================================================
HRESULT CLocus::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // レンダラー取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    BindTexture(LOCUS_TEXTURE_NUM);

    // 頂点バッファの生成
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	//頂点データ用に確保するバッファサイズ(バイト単位)
        D3DUSAGE_WRITEONLY,													//頂点バッファの使用法
        FVF_VERTEX_3D,														//使用する頂点フォーマット
        D3DPOOL_MANAGED,													//リソースのバッファを保持するメモリクラスを指定
        &m_pBuffMat,														//頂点バッファへのポインタ
        NULL)))																//NULLに設定
    {
        return E_FAIL;
    }

    // 頂点情報を設定
    VERTEX_3D *pVtx = NULL;

    // 頂点バッファのロック
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点数分回す
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        // 場所の設定
        pVtx[nCount].pos = DEFAULT_VECTOR;

        // 法線の設定
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        // カラーの設定
        pVtx[nCount].col = m_col;
    }

    // テクスチャ座標の設定
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    // 頂点バッファのアンロック
    m_pBuffMat->Unlock();

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 伊藤陽梧
//=============================================================================
void CLocus::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pBuffMat != NULL)
    {
        m_pBuffMat->Release();
        m_pBuffMat = NULL;
    }

    // 破棄
    Release();
}

//=============================================================================
// 更新処理
// Author : 伊藤陽梧
//=============================================================================
void CLocus::Update(void)
{
    // 頂点情報を設定
    VERTEX_3D *pVtx = NULL;

    // 頂点バッファのロック
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // 透明度を下げる
    m_col.a -= LUCOS_SUBALPHA;

    // 頂点バッファのアンロック
    m_pBuffMat->Unlock();

    // 設定した種類によって処理を変える
    switch (m_TypeLucus)
    {
    case TYPE_SHRINK:
        Shrink();
    default:
        break;
    }

    // 色の設定
    SetColor(m_col);

    // α値が0未満になったら終了
    if (m_col.a < 0.0f)
    {
        Uninit();
        return;
    }
}

//=============================================================================
// 描画処理
// Author : 伊藤陽梧
//=============================================================================
void CLocus::Draw(void)
{
    // レンダラー取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ライト無効
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    // アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    // アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

    // アルファテストの比較方法の設定
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    // マトリックスの初期化
    D3DXMatrixIdentity(&m_mtxWorld);

    // ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    // 頂点バッファをデバイスのデータストリームに設定
    pDevice->SetStreamSource(0, m_pBuffMat, 0, sizeof(VERTEX_3D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_3D);

    // セットテクスチャ
    pDevice->SetTexture(0, m_pTexture);

    // ポリゴン描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // テクスチャの初期化
    pDevice->SetTexture(0, NULL);

    // ライト有効
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    // アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 軌跡を縮める処理
// Author : 伊藤陽梧
//=============================================================================
void CLocus::Shrink()
{
    // 頂点情報を設定
    VERTEX_3D *pVtx = NULL;

    // 頂点バッファのロック
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // 左右の頂点を近づけるための角度を計算する
    float fAngle1 = atan2f(pVtx[2].pos.y - pVtx[0].pos.y, pVtx[2].pos.x - pVtx[0].pos.x);
    float fAngle2 = atan2f(pVtx[0].pos.y - pVtx[2].pos.y, pVtx[0].pos.x - pVtx[2].pos.x);
    float fAngle3 = atan2f(pVtx[3].pos.y - pVtx[1].pos.y, pVtx[3].pos.x - pVtx[1].pos.x);
    float fAngle4 = atan2f(pVtx[1].pos.y - pVtx[3].pos.y, pVtx[1].pos.x - pVtx[3].pos.x);

    // 縮める計算をして加算
    pVtx[0].pos.x += cosf(fAngle1) * m_ShrinkSpeed;
    pVtx[0].pos.y += sinf(fAngle1) * m_ShrinkSpeed;

    pVtx[1].pos.x += cosf(fAngle3) * m_ShrinkSpeed;
    pVtx[1].pos.y += sinf(fAngle3) * m_ShrinkSpeed;

    pVtx[2].pos.x += cosf(fAngle2) * m_ShrinkSpeed;
    pVtx[2].pos.y += sinf(fAngle2) * m_ShrinkSpeed;

    pVtx[3].pos.x += cosf(fAngle4) * m_ShrinkSpeed;
    pVtx[3].pos.y += sinf(fAngle4) * m_ShrinkSpeed;

    //頂点バッファのアンロック
    m_pBuffMat->Unlock();
}

//=============================================================================
// 座標設定
// pos1, pos2		: 現在の座標
// posOld1, posOld2	: 1フレーム前の座標
// Author : 伊藤陽梧
//=============================================================================
void CLocus::SetPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2)
{
    // 頂点情報を設定
    VERTEX_3D *pVtx = NULL;

    // 頂点バッファのロック
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // 場所の設定
    pVtx[0].pos = posOld1;
    pVtx[1].pos = pos1;
    pVtx[2].pos = posOld2;
    pVtx[3].pos = pos2;

    //頂点バッファのアンロック
    m_pBuffMat->Unlock();
}

//=============================================================================
// 色の設定
// col	: 色の情報
// Author : 伊藤陽梧
//=============================================================================
void CLocus::SetColor(D3DXCOLOR col)
{
    // 頂点情報を設定
    VERTEX_3D *pVtx = NULL;

    // 頂点バッファのロック
    m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

    // 各頂点の色を更新
    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = col;
    }

    //頂点バッファのアンロック
    m_pBuffMat->Unlock();
}

//=============================================================================
// テクスチャ受け渡し
// Author : 伊藤陽梧
//=============================================================================
void CLocus::BindTexture(int nNumTexture)
{
    CTexture *pTexture = CManager::GetTexture();
    m_pTexture = pTexture->GetInfo(nNumTexture)->pTexture;
}