//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "imgui/imgui_impl_win32.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			    // ウインドウのクラス名
#define WINDOW_NAME		"LEATHAL"	                // ウインドウのキャプション名
//#undef _DEBUG

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
#ifdef _DEBUG
int						g_nCountFPS;
#endif

//=============================================================================
// メイン関数
// Author : 後藤慎之助
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0,
        0,
        hInstance,
        NULL,
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        CLASS_NAME,
        NULL
    };
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    HWND hWnd;
    MSG msg;
    DWORD dwCurrentTime;
    DWORD dwFrameCount;
    DWORD dwExecLastTime;
    DWORD dwFPSLastTime;

    // ウィンドウクラスの登録
    RegisterClassEx(&wcex);

    // 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウの作成
    hWnd = CreateWindow(CLASS_NAME,
        WINDOW_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (rect.right - rect.left),
        (rect.bottom - rect.top),
        NULL,
        NULL,
        hInstance,
        NULL);

    // 乱数の初期化
    srand((unsigned int)time(NULL));

    //===============================================
    // マネージャー処理（レンダラーやシーンの代わり）
    CManager *pManager = NULL;

    // マネージャー生成
    pManager = new CManager;

    // 初期化処理
    if (FAILED(pManager->Init(hInstance, hWnd, TRUE))) //FALSEにすると、全画面表示
    {
        return -1;
    }
    //================================================

    // 分解能を設定
    timeBeginPeriod(1);

    // フレームカウント初期化
    dwCurrentTime =
        dwFrameCount = 0;
    dwExecLastTime =
        dwFPSLastTime = timeGetTime();

    // ウインドウの表示
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // メッセージループ
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {// PostQuitMessage()が呼ばれたらループ終了
                break;
            }
            else
            {
                // メッセージの翻訳とディスパッチ
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            dwCurrentTime = timeGetTime();	// 現在の時間を取得
            if ((dwCurrentTime - dwFPSLastTime) >= 500)
            {// 0.5秒ごとに実行
#ifdef _DEBUG
             // FPSを算出
                g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
                dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
                dwFrameCount = 0;
            }

            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {// 1/60秒経過
                dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

                pManager->Update();

                pManager->Draw();

                dwFrameCount++;
            }
        }
    }

    // 終了処理
    if (pManager != NULL)
    {
        pManager->Uninit();

        delete pManager;
        pManager = NULL;
    }

    // ウィンドウクラスの登録を解除
    UnregisterClass(CLASS_NAME, wcex.hInstance);

    // 分解能を戻す
    timeEndPeriod(1);

    return (int)msg.wParam;
}

// imgui使うときに必要
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// ウインドウプロシージャ
// Author : 後藤慎之助
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// imguiのウィンドウプロシージャ
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_CREATE:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:				// [ESC]キーが押された
            DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
            break;
        }
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=========================================
// FPS取得
// Author : 後藤慎之助
//=========================================
#ifdef _DEBUG
int GetFPS(void)
{
    return g_nCountFPS;
}
#endif
