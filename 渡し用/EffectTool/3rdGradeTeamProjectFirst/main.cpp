//=============================================================================
//
// ���C������ [main.cpp]
// Author : �㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "imgui/imgui_impl_win32.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			    // �E�C���h�E�̃N���X��
#define WINDOW_NAME		"LEATHAL"	                // �E�C���h�E�̃L���v�V������
//#undef _DEBUG

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
#ifdef _DEBUG
int						g_nCountFPS;
#endif

//=============================================================================
// ���C���֐�
// Author : �㓡�T�V��
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

    // �E�B���h�E�N���X�̓o�^
    RegisterClassEx(&wcex);

    // �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    // �E�B���h�E�̍쐬
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

    // �����̏�����
    srand((unsigned int)time(NULL));

    //===============================================
    // �}�l�[�W���[�����i�����_���[��V�[���̑���j
    CManager *pManager = NULL;

    // �}�l�[�W���[����
    pManager = new CManager;

    // ����������
    if (FAILED(pManager->Init(hInstance, hWnd, TRUE))) //FALSE�ɂ���ƁA�S��ʕ\��
    {
        return -1;
    }
    //================================================

    // ����\��ݒ�
    timeBeginPeriod(1);

    // �t���[���J�E���g������
    dwCurrentTime =
        dwFrameCount = 0;
    dwExecLastTime =
        dwFPSLastTime = timeGetTime();

    // �E�C���h�E�̕\��
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // ���b�Z�[�W���[�v
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
                break;
            }
            else
            {
                // ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾
            if ((dwCurrentTime - dwFPSLastTime) >= 500)
            {// 0.5�b���ƂɎ��s
#ifdef _DEBUG
             // FPS���Z�o
                g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
                dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
                dwFrameCount = 0;
            }

            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {// 1/60�b�o��
                dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

                pManager->Update();

                pManager->Draw();

                dwFrameCount++;
            }
        }
    }

    // �I������
    if (pManager != NULL)
    {
        pManager->Uninit();

        delete pManager;
        pManager = NULL;
    }

    // �E�B���h�E�N���X�̓o�^������
    UnregisterClass(CLASS_NAME, wcex.hInstance);

    // ����\��߂�
    timeEndPeriod(1);

    return (int)msg.wParam;
}

// imgui�g���Ƃ��ɕK�v
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// �E�C���h�E�v���V�[�W��
// Author : �㓡�T�V��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// imgui�̃E�B���h�E�v���V�[�W��
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
        case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
            DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
            break;
        }
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=========================================
// FPS�擾
// Author : �㓡�T�V��
//=========================================
#ifdef _DEBUG
int GetFPS(void)
{
    return g_nCountFPS;
}
#endif
