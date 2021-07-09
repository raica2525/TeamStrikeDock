//===============================================
//
// 描画処理 (renderer.cpp)
// Author : 後藤慎之助、池田悠希
//
//===============================================

//========================
// インクルードファイル
//========================
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "fade.h"
#include "manager.h"
#include "game.h"
#include "pause.h"
#include "player.h"
#include "ball.h"

//========================================
// 静的メンバ変数宣言
//========================================
#ifdef _DEBUG
bool CRenderer::m_bDispFont = true;
#endif

//========================================
// レンダリングのデフォルトコンストラクタ
// Author : 後藤慎之助
//========================================
CRenderer::CRenderer()
{
    m_pD3D = NULL;			// Direct3Dオブジェクト
    m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
    m_pScreenTexture = NULL;
#ifdef _DEBUG
    m_pFont = NULL;        // フォントへのポインタ
#endif
}

//========================================
// レンダリングのデストラクタ
// Author : 後藤慎之助
//========================================
CRenderer::~CRenderer()
{

}

//========================================
// レンダリングの初期化処理
// Author : 後藤慎之助
//========================================
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
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// ステンシルバッファに24bit、デプスバッファに8bitを使う
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
        &d3dpp, &m_pD3DDevice)))
    {
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

    // スクリーン全体のテクスチャを生成
    D3DXCreateTexture(
        m_pD3DDevice,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_pScreenTexture);

    // レンダーステートの設定
    m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);          // カリングをする
    m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                  // Zバッファを使用
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);         // αブレンドを行う
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // αソースカラーの指定
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// αデスティネーションカラー
    m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                 // カメラを使用する
    m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00444444);            // アンビエントの設定

                                                                        // サンプラーステートの設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);   // テクスチャU値の繰り返し設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);   // テクスチャV値の繰り返し設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);    // テクスチャ拡大時の補間設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);    // テクスチャ縮小時の補間設定

                                                                            // テクスチャステージステートの設定
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // アルファブレンディング処理
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // 最初のアルファ引数(初期値)
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // 2番目のアルファ引数(初期値)

                                                                            // マテリアルの設定
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    m_pD3DDevice->SetMaterial(&material);

#ifdef _DEBUG
    // デバッグ情報表示用フォントの生成
    D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

    return S_OK;
}

//========================================
// レンダリングの終了処理
// Author : 後藤慎之助
//========================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
    // デバッグ情報表示用フォントの破棄
    if (m_pFont != NULL)
    {
        m_pFont->Release();
        m_pFont = NULL;
    }
#endif

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

    // スクリーン全体のテクスチャを破棄
    if (m_pScreenTexture != NULL)
    {
        m_pScreenTexture->Release();
        m_pScreenTexture = NULL;
    }
}

//========================================
// レンダリングの更新処理
// Author : 後藤慎之助
//========================================
void CRenderer::Update(void)
{
    // 全てのオブジェクトを更新
    CScene::UpdateAll();
}

//========================================
// レンダリングの描画処理
// Author : 池田悠希、後藤慎之助
//========================================
void CRenderer::Draw(void)
{
    // 現在の描画先を保存
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;
    LPDIRECT3DSURFACE9 pZBuffer = NULL;
    m_pD3DDevice->GetRenderTarget(0, &pBackBuffer);
    m_pD3DDevice->GetDepthStencilSurface(&pZBuffer);

    // 描画先をテクスチャに設定
    LPDIRECT3DSURFACE9 pSurf = NULL;
    m_pScreenTexture->GetSurfaceLevel(0, &pSurf);
    m_pD3DDevice->SetRenderTarget(0, pSurf);

    // 先に波紋エフェクトをかけたいやつを描画してから、普段の描画をする
    for (int nCount = 0; nCount < 2; nCount++)
    {
        // バックバッファ＆Ｚバッファ＆ステンシルバッファのクリア
        m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(249, 102, 71, 255), 1.0f, 0);

        // Direct3Dによる描画の開始
        if (SUCCEEDED(m_pD3DDevice->BeginScene()))
        {
            // ポリゴンの描画処理
            if (nCount == 0)
            {
                // 波以外を描画
                CScene::DrawExceptWave();
            }
            else if (nCount == 1)
            {
                // 波を先に描画してから、普段の描画
                CScene::DrawWave();
                CScene::DrawExceptWave();

                // ゲーム中かつ、ポーズ中なら
                if (CManager::GetMode() == CManager::MODE_GAME)
                {
                    if (CGame::GetState() == CGame::STATE_PAUSE_MENU)
                    {
                        // ポーズを描画
                        CPause::Draw();
                    }
                }

                // フェードを描画
                CFade::Draw();

#ifdef _DEBUG
                if (m_bDispFont)
                {
                    // FPS表示
                    DrawDebugData();
                }
#endif
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

//========================================
// デバッグデータ描画処理
// Author : 後藤慎之助
//========================================
#ifdef _DEBUG
void CRenderer::DrawDebugData(void)
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    char str[256];

    // モードによって、表示するものを変える
    CManager::MODE mode = CManager::GetMode();   
    switch (mode)
    {
    case CManager::MODE_TITLE:
        wsprintf(str, "FPS:%d\nOBJ:%d", GetFPS(), CScene::GetNumObjAll());
        m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
        break;
    case CManager::MODE_MANUAL:
        wsprintf(str, "FPS:%d\nOBJ:%d", GetFPS(), CScene::GetNumObjAll());
        m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
        break;
    case CManager::MODE_GAME:
        GameDebugData();
        break;
    case CManager::MODE_RESULT:
        wsprintf(str, "FPS:%d\nOBJ:%d", GetFPS(), CScene::GetNumObjAll());
        m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
        break;
    }
}

//========================================
// ゲーム内のデバッグデータ描画処理
// Author : 後藤慎之助
//========================================
void CRenderer::GameDebugData(void)
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    char str[256];

    // プレイヤー1の能力を取得
    CPlayer* pPlayer = CGame::GetPlayer(PLAYER_1);
    CBall* pBall = CGame::GetBall();

    // 描画内容を決める
    wsprintf(str, "FPS:%d\nOBJ:%d\n\nATK:%d\nDEF:%d\nSPD:%d\nWEI:%d\n\nLIFE:%d\nSP_GAUGE:%d\nPOINT:%d\nSTOCK:%d\n\nCHARGE:%d\n\nBALL_SPD:%d\nBALL_WHO_SHOOT:%d\nBALL_WHO_ABSORB:%d\nBALL_STOP:%d\n\nALL_PLAYERS:%d\nDEFEATED_PLAYERS:%d\nPRE_WORST_PLAYER:%d\nDEATH_PLAYERS:%d", 
        GetFPS(), CScene::GetNumObjAll(), (int)pPlayer->GetAtk(), (int)pPlayer->GetDef(), (int)pPlayer->GetSpd(), (int)pPlayer->GetWei(), (int)pPlayer->GetLife(), (int)pPlayer->GetSpGaugeCurrent(), (int)pPlayer->GetPoint(), (int)pPlayer->GetStock(),
        (int)pPlayer->GetSwingCharge(),
        (int)pBall->GetSpeed(), pBall->GetWhoShooting(), pBall->GetWhoAbsorbing(), pBall->GetStopTime(),
        CGame::GetNumAllPlayer(), CGame::GetNumDefeatPlayer(), CGame::GetWhoWorstPlayer(), CGame::GetNumDeathPlayer());

    // テキスト描画
    m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif
