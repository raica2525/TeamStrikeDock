//===============================================
//
// シーン上の2Dポリゴン処理 (scene2d.cpp)
// Author : 後藤慎之助、池田悠希（テクスチャブレンディング対応）
//
//===============================================

//========================
// インクルードファイル
//========================
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// 静的メンバ変数宣言
//========================================
bool CScene2D::m_bAdditiveSynthesis = false;
bool CScene2D::m_bNega = false;

//===========================================
// テンプレート関数                       
//===========================================

//===========================================
// 割り算
//
// 引数   :data0,data1はデータ
// 戻り値 :計算結果
//===========================================
template<class T> T Divide(const T data0, const T data1)
{
    // 変数宣言
    T answer;   // 答え

                // 割る
    answer = data0 / data1;

    return answer;
}

//============================================================
// シーン上の2Dポリゴンのオーバーライドされたコンストラクタ
// Author : 後藤慎之助
//============================================================
CScene2D::CScene2D(OBJTYPE objType) :CScene(objType)
{
    ZeroMemory(m_apTexture, sizeof(m_apTexture));
    m_pVtxBuff = NULL;
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    memset(m_anCounterAnim, 0, sizeof(m_anCounterAnim));
    memset(m_anPatternAnim, 0, sizeof(m_anPatternAnim));

    m_nNumTexture = 0;
    memset(m_aBrend, BREND_NORMAL, sizeof(m_aBrend));

    m_nAlphaTestBorder = DEFAULT_ALPHATEST_BORDER_2D;
}

//============================================================
// シーン上の2Dポリゴンのデストラクタ
// Author : 後藤慎之助
//============================================================
CScene2D::~CScene2D()
{

}

//============================================================
// シーン上の2Dポリゴンの初期化処理
// Author : 後藤慎之助
//============================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 位置を結びつける
    m_pos = pos;

    // 大きさを結びつける
    m_size = size;

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

    // 頂点座標の設定（右回りで設定する）
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // rhwの設定（値は1.0で設定する）
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // 頂点カラーを設定（0.0f～1.0fの値で設定する）
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //テクスチャ座標の設定
    for (int nCount = 0; nCount < MAX_BREND_TEXTURE; nCount++)
    {
        pVtx[0].tex[nCount] = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex[nCount] = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex[nCount] = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex[nCount] = D3DXVECTOR2(1.0f, 1.0f);
    }

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();

    return S_OK;
}

//============================================================
// シーン上の2Dポリゴンの終了処理
// Author : 後藤慎之助
//============================================================
void CScene2D::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // オブジェクトの破棄
    Release();
}

//============================================================
// シーン上の2Dポリゴンの更新処理
// Author : 後藤慎之助
//============================================================
void CScene2D::Update(void)
{

}

//=============================================================
// シーン上の2Dポリゴンの描画処理
// Author : 池田悠希（テクスチャブレンディング対応）、後藤慎之助
//=============================================================
void CScene2D::Draw(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    // アルファテスト（やや強引）
    if (m_nAlphaTestBorder != DEFAULT_ALPHATEST_BORDER_2D)
    {
        //アルファテストを有効化
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        //アルファテスト基準値の設定
        pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaTestBorder);
        //アルファテストの比較方法の設定
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    }

    // 加算合成
    if (m_bAdditiveSynthesis)
    {
        // レンダーステート(加算合成にする)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    // 反転合成
    if (m_bNega)
    {
        // 反転合成で考慮できないα値を、アルファテストで制御する
        // アルファテストを有効化
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        // アルファテスト基準値の設定
        pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaTestBorder);
        // アルファテストの比較方法の設定
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

        // 反転合成
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
    }

    // ブレンド方法設定　//池田追加
    for (int nCount = 0; nCount < m_nNumTexture; nCount++)
    {
		if (m_aBrend[nCount] == BREND_IGNORE_INFO)
		{
			pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		}
		else
		{
			switch (nCount)
			{
			case 0:
                pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
                pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_MODULATE);
				pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // アルファブレンディング処理
				break;
            default:
				switch (m_aBrend[nCount])
				{
				case BREND_NORMAL:
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_CURRENT);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					break;

				case BREND_SEAL:
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_ADD);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
					break;
				case BREND_APPLY_INFO:
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_MODULATE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                    break;
				}
				break;
			}
		}
    }

    // テクスチャの設定 //池田変更
    for (int nCount = 0; nCount < m_nNumTexture; nCount++)
    {
        pDevice->SetTexture(nCount, m_apTexture[nCount]);
    }

    // ポリゴンの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // 反転合成を戻す
    if (m_bNega)
    {
        // アルファテストを無効化
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

        // 反転合成を戻す
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        m_bNega = false;
    }

    // 加算合成を戻す
    if (m_bAdditiveSynthesis)
    {
        // レンダーステート(加算合成を戻す)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // 加算合成のフラグをfalseに戻す
        m_bAdditiveSynthesis = false;
    }

    // アルファテストを戻す
    if (m_nAlphaTestBorder != DEFAULT_ALPHATEST_BORDER_2D)
    {
        //アルファテストを無効化
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }

    // テクスチャの設定の解除　//池田変更
    for (int nCount = 0; nCount < MAX_BREND_TEXTURE; nCount++)
    {
        pDevice->SetTexture(nCount, NULL);
    }

    // ブレンド方法を戻す
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

//=============================================================
// シーン上の2Dポリゴンのテクスチャを割り当て
// Author : 後藤慎之助、池田悠希
//=============================================================
int CScene2D::BindTexture(const int nNumTexture, const BREND brend)
{
    // 戻り値で、現在使っているテクスチャ数を返す用
    int nCurrentNumTex = m_nNumTexture;

    // テクスチャを持ってきて、ブレンドの方法を決めつつ現在使っているテクスチャ数を加算
    CTexture *pTexture = CManager::GetTexture();
    m_apTexture[m_nNumTexture] = pTexture->GetInfo(nNumTexture)->pTexture;
    m_aBrend[m_nNumTexture++] = brend;

    return nCurrentNumTex;
}

//=============================================================
// シーン上の2Dポリゴンの頂点座標を設定
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetVertex(void)
{
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // 頂点座標の設定
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), +(m_size.y / 2), 0.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// シーン上の2Dポリゴンの回転する頂点座標を設定
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetRotVertex(float fAngle)
{
    // 変数宣言
    // 各頂点
    D3DXVECTOR3 vertex1 = DEFAULT_VECTOR;
    D3DXVECTOR3 vertex2= DEFAULT_VECTOR;
    D3DXVECTOR3 vertex3= DEFAULT_VECTOR;
    D3DXVECTOR3 vertex4= DEFAULT_VECTOR;

    //==========================================================================================================
    // 画像を、画像の中心を軸に回転させる
    // 左上の頂点
    vertex1.x = -(m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex1.y = -(m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);

    // 右上の頂点
    vertex2.x = (m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex2.y = (m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);

    // 左下の頂点
    vertex3.x = -(m_size.x / 2)*cosf(fAngle)
        - (m_size.y / 2)*sinf(fAngle);
    vertex3.y = -(m_size.x / 2)*sinf(fAngle)
        + (m_size.y / 2)*cosf(fAngle);

    // 右下の頂点
    vertex4.x = (m_size.x / 2)*cosf(fAngle)
        - (m_size.y / 2)*sinf(fAngle);
    vertex4.y = (m_size.x / 2)*sinf(fAngle)
        + (m_size.y / 2)*cosf(fAngle);
    //==========================================================================================================

    // 頂点情報へのポインタ
    VERTEX_2D *pVtx = NULL;

    // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点座標の設定
    pVtx[0].pos = m_pos + vertex1;
    pVtx[1].pos = m_pos + vertex2;
    pVtx[2].pos = m_pos + vertex3;
    pVtx[3].pos = m_pos + vertex4;

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// シーン上の2Dポリゴンの見かけ上の頂点座標を設定
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual)
{
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // 頂点座標の設定
    pVtx[0].pos = posVisual + D3DXVECTOR3(-(sizeVisual.x / 2), -(sizeVisual.y / 2), 0.0f);
    pVtx[1].pos = posVisual + D3DXVECTOR3(+(sizeVisual.x / 2), -(sizeVisual.y / 2), 0.0f);
    pVtx[2].pos = posVisual + D3DXVECTOR3(-(sizeVisual.x / 2), +(sizeVisual.y / 2), 0.0f);
    pVtx[3].pos = posVisual + D3DXVECTOR3(+(sizeVisual.x / 2), +(sizeVisual.y / 2), 0.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// シーン上の2Dポリゴンのアニメーションを設定
// Author : 後藤慎之助
//=============================================================
bool CScene2D::SetAnimation(int nSpeed, int nPattern, int nTex)
{
    // 変数宣言
    bool bOneRound = false;   // アニメーションが一周したかどうか

    // アニメーション
    m_anCounterAnim[nTex]++;	//カウンタ加算
    if (m_anCounterAnim[nTex] == nSpeed)//速さ
    {
        // オーバーフロー防止
        m_anCounterAnim[nTex] = 0;  // カウンタを0に戻す

        // アニメーションが一周したら
        if ((m_anPatternAnim[nTex] + 1) % nPattern == 0)
        {
            // 一周のフラグをtrueに
            bOneRound = true;
        }

        // アニメーションを切り替える
        m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // 枚数
    }

    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // テクスチャ座標の設定
                                                // 変数宣言
    float fEqualDivision = 0.0f;   // テクスチャを等分する

                                   // 何等分するか計算
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // テクスチャの座標を反映
    pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 1.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// シーン上の2Dポリゴンの逆向きのアニメーションを設定
// Author : 後藤慎之助
//=============================================================
bool CScene2D::SetReverseAnimation(int nSpeed, int nPattern, int nTex)
{
    // 変数宣言
    bool bOneRound = false;   // アニメーションが一周したかどうか

    // アニメーション
    m_anCounterAnim[nTex]++;	//カウンタ加算
    if (m_anCounterAnim[nTex] == nSpeed)//速さ
    {
        // オーバーフロー防止
        m_anCounterAnim[nTex] = 0;  // カウンタを0に戻す

        // アニメーションが一周したら
        if ((m_anPatternAnim[nTex] + 1) % nPattern == 0)
        {
            // 一周のフラグをtrueに
            bOneRound = true;
        }

        // アニメーションを切り替える
        m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // 枚数
    }

    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // テクスチャ座標の設定
                                                // 変数宣言
    float fEqualDivision = 0.0f;   // テクスチャを等分する

                                   // 何等分するか計算
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // テクスチャの座標を反映
    pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 1.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// シーン上の2Dポリゴンのアニメーションを設定
// Author : 後藤慎之助
//=============================================================
bool CScene2D::SetFlowingAnimation(int nSpeed, int nPattern, bool bRightToLeft, DIRECT direct, int nTex)
{
    // 変数宣言
    bool bOneRound = false;   // アニメーションが一周したかどうか

    // アニメーション
    m_anCounterAnim[nTex]++;	//カウンタ加算
    if (m_anCounterAnim[nTex] == nSpeed)//速さ
    {
        // オーバーフロー防止
        m_anCounterAnim[nTex] = 0;  // カウンタを0に戻す

        // アニメーションが一周したら
        if ((m_anPatternAnim[nTex] + 1) % nPattern == 0)
        {
            // 一周のフラグをtrueに
            bOneRound = true;
        }

        if (bRightToLeft)
        {
            // アニメーションを切り替える
            m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // 枚数
        }
        else
        {
            // アニメーションを切り替える
            m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] - 1) % nPattern;  // 枚数
        }
    }

    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

    // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

    // テクスチャ座標の設定
    // 変数宣言
    float fEqualDivision = 0.0f;   // テクスチャを等分する

    // 何等分するか計算
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // テクスチャの座標を反映
    switch (direct)
    {
    case CScene2D::DIRECT_VERTICAL:
        // 縦
        pVtx[0].tex[nTex] = D3DXVECTOR2(0.0f, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[1].tex[nTex] = D3DXVECTOR2(1.0f, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[2].tex[nTex] = D3DXVECTOR2(0.0f, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(1.0f, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        break;

    case CScene2D::DIRECT_HORIZON:
        // 横
        pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 0.0f);
        pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, 0.0f);
        pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, 1.0f);
        break;

    case CScene2D::DIRECT_RIGHT_UP:
        // 右肩上がり
        pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * (fEqualDivision * (-1)));
        pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * (fEqualDivision * (-1)));
        pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * (fEqualDivision * (-1)) + 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * (fEqualDivision * (-1)) + 1.0f);

        break;

    case CScene2D::DIRECT_RIGHT_DOWN:
        // 右肩下がり
        pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        break;
    }

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// テクスチャの描画範囲の設定
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetTextureRange(int nRange, int nPattern, int nTex)
{
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // テクスチャ座標の設定
                                                // 変数宣言
    float fEqualDivision = 0.0f;   // テクスチャを等分する

                                   // 何等分するか計算
    fEqualDivision = Divide((float)nRange, (float)nPattern);

    // テクスチャの座標を反映
    pVtx[0].tex[nTex] = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(fEqualDivision, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fEqualDivision, 1.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// 段落のあるアニメーションの設定
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetParagraphAnimation(int nParagraph, int nMaxParagraph, int nSpeed, int nPattern, int nTex)
{
    // アニメーション
    m_anCounterAnim[nTex]++;	//カウンタ加算
    if (m_anCounterAnim[nTex] == nSpeed)//速さ
    {
        // オーバーフロー防止
        m_anCounterAnim[nTex] = 0;  // カウンタを0に戻す

        // アニメーションを切り替える
        m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // 枚数
    }

    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // テクスチャ座標の設定
                                                // 変数宣言
    float fEqualDivisionX = 0.0f;   // テクスチャを等分する（縦）
    float fEqualDivisionY = 0.0f;   // テクスチャを等分する（横）

                                    // 何等分するか計算
    fEqualDivisionX = Divide(1.0f, (float)nPattern);
    fEqualDivisionY = Divide(1.0f, (float)nMaxParagraph);

    // テクスチャの座標を反映
    pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX, fEqualDivisionY * (nParagraph - 1));
    pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX + fEqualDivisionX, fEqualDivisionY * (nParagraph - 1));
    pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX, fEqualDivisionY * nParagraph);
    pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX + fEqualDivisionX, fEqualDivisionY * nParagraph);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// テクスチャの描画場所を決める
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetTexturePlace(int nPlace, int nPattern, int nTex)
{
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

    // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

    // テクスチャ座標の設定
    // 変数宣言
    float fEqualDivision = 0.0f;   // テクスチャを等分する

    // 何等分するか計算
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // テクスチャの座標を反映
    pVtx[0].tex[nTex] = D3DXVECTOR2((fEqualDivision * nPlace), 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(fEqualDivision + (fEqualDivision * nPlace), 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2((fEqualDivision * nPlace), 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fEqualDivision + (fEqualDivision * nPlace), 1.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// 段落があるテクスチャの描画場所を決める
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetParagraphTexturePlace(int nPlace, int nParagraph, int nMaxParagraph, int nPattern, int nTex)
{
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

                                                // テクスチャ座標の設定
                                                // 変数宣言
    float fEqualDivisionX = 0.0f;   // テクスチャを等分する（縦）
    float fEqualDivisionY = 0.0f;   // テクスチャを等分する（横）

                                    // 何等分するか計算
    fEqualDivisionX = Divide(1.0f, (float)nPattern);
    fEqualDivisionY = Divide(1.0f, (float)nMaxParagraph);

    // テクスチャの座標を反映
    pVtx[0].tex[nTex] = D3DXVECTOR2((fEqualDivisionX * nPlace), fEqualDivisionY * (nParagraph - 1));
    pVtx[1].tex[nTex] = D3DXVECTOR2(fEqualDivisionX + (fEqualDivisionX * nPlace), fEqualDivisionY * (nParagraph - 1));
    pVtx[2].tex[nTex] = D3DXVECTOR2((fEqualDivisionX * nPlace), fEqualDivisionY * nParagraph);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fEqualDivisionX + (fEqualDivisionX * nPlace), fEqualDivisionY * nParagraph);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// アニメーションのカウンタを利用するときに使う
// Author : 後藤慎之助
//=============================================================
int CScene2D::CountAnimation(int nSpeed, int nPattern)
{
    // アニメーション
    m_anCounterAnim[0]++;	//カウンタ加算
    if (m_anCounterAnim[0] == nSpeed)//速さ
    {
        // オーバーフロー防止
        m_anCounterAnim[0] = 0;  // カウンタを0に戻す

                             // アニメーションを切り替える
        m_anPatternAnim[0] = (m_anPatternAnim[0] + 1) % nPattern;  // 枚数
    }

    // アニメーションのパターンのカウンタを返す
    return m_anPatternAnim[0];
}

//=============================================================
// シーン上の2Dポリゴンの色を変える
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点カラーを設定（0.0f～1.0fの値で設定する）
    pVtx[0].col = col;
    pVtx[1].col = col;
    pVtx[2].col = col;
    pVtx[3].col = col;

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// 横長ゲージのサイズを設定
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetLeftToRightGauge(float fMax, float fNow, int nTex)
{
    // 重み
    float fWeight = fNow / fMax;

    VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点座標の設定(左に揃えてから、右に伸ばすイメージ)
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2) + (m_size.x * fWeight), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2) + (m_size.x * fWeight), +(m_size.y / 2), 0.0f);

    // テクスチャ座標を更新
    pVtx[0].tex[nTex] = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(fWeight, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fWeight, 1.0f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//=============================================================
// 端の1ピクセルが反対側に行く現象を解決する関数
// Author : 後藤慎之助
//=============================================================
void CScene2D::SetShaveTex(void)
{
    // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    VERTEX_2D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 1ピクセルぐらい削る
    pVtx[0].tex[0] = D3DXVECTOR2(0.01f, 0.01f);
    pVtx[1].tex[0] = D3DXVECTOR2(0.99f, 0.01f);
    pVtx[2].tex[0] = D3DXVECTOR2(0.01f, 0.99f);
    pVtx[3].tex[0] = D3DXVECTOR2(0.99f, 0.99f);

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}
