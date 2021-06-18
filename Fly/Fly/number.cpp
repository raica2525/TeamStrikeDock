//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
// number.cpp
// Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・



//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#include "number.h"
#include "renderer.h"
#include "manager.h"

LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//コンストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CNumber::CNumber()
{
	m_nValue = 0;
	m_pos = D3DXVECTOR3(0, 0, 0);			// ポリゴンの位置
	m_size = D3DXVECTOR3(0, 0, 0);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//デストラクタ
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CNumber::~CNumber()
{

}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//初期化処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CNumber::Init(void)
{
	VERTEX_2D*pVtx;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,//バッファサイズ
		D3DUSAGE_WRITEONLY,//固定
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//固定
		&m_pVtxBuff,//変数名によって変化
		NULL);

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x / 2, -m_size.y / 2, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x / 2, m_size.y / 2, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x / 2, m_size.y / 2, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_nValue*0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_nValue +1)*0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nValue*0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_nValue +1)*0.1f, 1.0f);

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
	m_pVtxBuff->Unlock();


	return S_OK;
}


//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//更新処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CNumber::Update(void)
{
	VERTEX_2D*pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].tex = D3DXVECTOR2(m_nValue*0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_nValue + 1)*0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nValue*0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_nValue + 1)*0.1f, 1.0f);
	m_pVtxBuff->Unlock();
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//描画処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,//プリティブの種類
		0,
		NUM_POLYGON//プリティブの数
	);
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//終了処理
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CNumber::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//テクスチャ読み込み
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
HRESULT CNumber::Load(void)
{
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), "Resources\\number000.png", &m_pTexture);
	return S_OK;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//テクスチャ破棄
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CNumber::Unload(void)
{
	if (m_pTexture != NULL) {
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クリエイト関数
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
CNumber* CNumber::Create(int nValue, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CNumber* pNumber = NULL;
	pNumber = new CNumber;
	pNumber->SetValue(nValue);
	pNumber->SetPos(pos);
	pNumber->SetSize(size);
	pNumber->Init();
	
	return pNumber;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//数値設定
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CNumber::SetValue(int nValue)
{
	m_nValue = nValue;
}

void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CNumber::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}