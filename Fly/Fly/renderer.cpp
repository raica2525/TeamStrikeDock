#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "number.h"
#include "manager.h"
#include "debug.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_pTexture = NULL;
}

CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

																// デバイスの生成
																// ディスプレイアダプタを表すためのデバイスを作成
																// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice))) {
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	D3DXCreateTexture(
		m_pD3DDevice,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture);

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//透明度を使うか
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//αデスティ

																		// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //ドット絵のときはD3DTEXF_POINT
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	srand((unsigned int)time(0));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	// ポリゴンの更新処理
	CScene::UpdateAll();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pZBuffer = NULL;

	//現在の描画先を保存
	m_pD3DDevice->GetRenderTarget(0, &pBackBuffer);
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffer);

	//描画先をテクスチャに設定
	LPDIRECT3DSURFACE9 pSurf = NULL;
	m_pTexture->GetSurfaceLevel(0, &pSurf);
	m_pD3DDevice->SetRenderTarget(0, pSurf);

	for (int nCount = 0; nCount < 2; nCount++)
	{
		// バックバッファ＆Ｚバッファのクリア
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		// Direct3Dによる描画の開始
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{
			// ポリゴンの描画処理
			if (nCount == 0)
			{
				CScene::DrawExceptEffect();
			}
			else if (nCount == 1)
			{
				CScene::DrawEffect();
				CScene::DrawExceptEffect();
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			}

			// Direct3Dによる描画の終了
			m_pD3DDevice->EndScene();

		}
		if (nCount == 0)
		{
			//描画先をバックバッファに変更
			m_pD3DDevice->SetRenderTarget(0, pBackBuffer);
			m_pD3DDevice->SetDepthStencilSurface(pZBuffer);
		}
	}
	pBackBuffer->Release();
	pBackBuffer = NULL;

	pZBuffer->Release();
	pZBuffer = NULL;

	pSurf->Release();
	pSurf = NULL;

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

