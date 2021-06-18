//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	effect.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#define TEX_DIV_RATE 1.2f
#define SIZE_SCALE_RATE 1.1f
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//静的メンバ変数宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[2] = {};

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CEffect::CEffect()
{
	m_pos = ZERO_VEC;
	m_bNega = false;
	m_size = ZERO_VEC;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CEffect::~CEffect()
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CEffect::Init(void)
{
	VERTEX_2D*pVtx = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	for (int nCount = 0; nCount < 2; nCount++)
	{
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_2D)*NUM_VERTEX,//バッファサイズ
			D3DUSAGE_WRITEONLY,//固定
			FVF_VERTEX_2D,//頂点フォーマット
			D3DPOOL_MANAGED,//固定
			&m_apVtxBuff[nCount],//変数名によって変化
			NULL);
	}
	//円形のステンシル生成用ポリゴン
	m_apVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)-m_size.y / 2, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)-m_size.y / 2, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)m_size.y / 2, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)m_size.y / 2, 0.0f);
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255)
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	m_apVtxBuff[0]->Unlock();

	pVtx = NULL;

	//テクスチャ
	m_apVtxBuff[1]->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定

	pVtx[0].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)-m_size.y / 2, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)-m_size.y / 2, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)m_size.y / 2, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)m_size.y / 2, 0.0f);
	//テクスチャ座標の設定
	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].tex = D3DXVECTOR2(pVtx[nCount].pos.x / SCREEN_WIDTH, pVtx[nCount].pos.y / SCREEN_HEIGHT);
	}

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255)
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	m_apVtxBuff[1]->Unlock();

	return S_OK;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//終了処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CEffect::Uninit(void)
{
	// 頂点バッファの破棄
	for (int nCount = 0; nCount < 2; nCount++)
	{
		if (m_apVtxBuff[nCount] != NULL)
		{
			m_apVtxBuff[nCount]->Release();
			m_apVtxBuff[nCount] = NULL;
		}
	}
	Release();
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//更新処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CEffect::Update(void)
{
	VERTEX_2D*pVtx = NULL;
	m_size.x*= SIZE_SCALE_RATE;
	m_size.y*= SIZE_SCALE_RATE;
	if (m_bNega)
	{
		//テクスチャ
		for (int nCount = 0; nCount < 2; nCount++)
		{
			m_apVtxBuff[nCount]->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)-m_size.y / 2, 0.0f);
			pVtx[1].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)-m_size.y / 2, 0.0f);
			pVtx[2].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)m_size.y / 2, 0.0f);
			pVtx[3].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)m_size.y / 2, 0.0f);

			if (nCount == 1)
			{
				for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
				{
					pVtx[nCount].tex = D3DXVECTOR2(pVtx[nCount].pos.x / SCREEN_WIDTH, pVtx[nCount].pos.y / SCREEN_HEIGHT);
				}
			}

			m_apVtxBuff[nCount]->Unlock();
		}
	}
	else
	{
		//テクスチャ
		for (int nCount = 0; nCount < 2; nCount++)
		{
			pVtx = NULL;
			m_apVtxBuff[nCount]->Lock(0, 0, (void**)&pVtx, 0);
			if (nCount == 1)
			{
				m_size.x /= TEX_DIV_RATE;
				m_size.y /= TEX_DIV_RATE;

				pVtx[0].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)-m_size.y / 2, 0.0f);
				pVtx[1].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)-m_size.y / 2, 0.0f);
				pVtx[2].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)m_size.y / 2, 0.0f);
				pVtx[3].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)m_size.y / 2, 0.0f);

				m_size.x *= TEX_DIV_RATE;
				m_size.y *= TEX_DIV_RATE;

				for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
				{
					pVtx[nCount].tex = D3DXVECTOR2(pVtx[nCount].pos.x / SCREEN_WIDTH, pVtx[nCount].pos.y / SCREEN_HEIGHT);
				}
			}

			pVtx[0].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)-m_size.y / 2, 0.0f);
			pVtx[1].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)-m_size.y / 2, 0.0f);
			pVtx[2].pos = m_pos + D3DXVECTOR3((float)-m_size.x / 2, (float)m_size.y / 2, 0.0f);
			pVtx[3].pos = m_pos + D3DXVECTOR3((float)m_size.x / 2, (float)m_size.y / 2, 0.0f);

			m_apVtxBuff[nCount]->Unlock();
		}
	}
	if (m_size.x > SCREEN_WIDTH * 2)
	{
		Uninit();
	}
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//描画処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CEffect::Draw(void)
{
	CreateStencil(0);

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);				//両方bad
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);				//ステンシルだけok
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);				//両方ok
	if (m_bNega)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 128);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
	}
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_apVtxBuff[1], 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetRenderer()->GetScreenTex());

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetTexture(0, NULL);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クリエイト関数
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size , bool bNega)
{
	CEffect* pEffect = new CEffect;
	pEffect->m_pos = pos;
	pEffect->m_size = size;
	pEffect->m_bNega = bNega;
	pEffect->SetObjType(OBJTYPE_EFFECT);
	pEffect->Init();
	return pEffect;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//テクスチャ読み込み
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CEffect::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "Data\\Texture\\Donuts.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "Data\\Texture\\Circle.png", &m_apTexture[1]);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//テクスチャ破棄
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CEffect::Unload(void)
{
	for (int nCount = 0; nCount < 2; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//ステンシル生成
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CEffect::CreateStencil(int nVtxIndex)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// 不透明にする値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_apVtxBuff[nVtxIndex], 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	if (m_bNega)
	{
		pDevice->SetTexture(0, m_apTexture[1]);
	}
	else
	{
		pDevice->SetTexture(0, m_apTexture[0]);
	}
	

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetTexture(0, NULL);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
