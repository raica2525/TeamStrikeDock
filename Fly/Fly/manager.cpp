#include "manager.h"
#include <stdio.h>
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "input.h"
#include "inputkeyboard.h"
#include "debug.h"
#include "camera.h"
#include "light.h"
#include "scene3d.h"
#include "polygon3d.h"
#include "model.h"
#include "player.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "effect.h"
#include "glyph.h"
#include "text.h"
CRenderer* CManager::m_pRenderer = NULL;
CInput* CManager::m_pInput = NULL;
CCamera* CManager::m_pCamera = NULL;
bool CManager::m_bImShowAnotherWindow = false;
bool CManager::m_bUseShadow = true;
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�R���X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CManager::CManager()
{
	m_pRenderer = NULL;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�f�X�g���N�^
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
CManager::~CManager()
{

}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�������֐�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
HRESULT CManager::Init(HWND hWnd, bool bWindow, HINSTANCE hInstance)
{
	HRESULT hResult = NULL;
	m_pRenderer = new CRenderer;				//�����_���𐶐�
	hResult = m_pRenderer->Init(hWnd, true);	//�����_����������

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontConfig config;
	config.MergeMode = true;
	io.DisplaySize.x = SCREEN_WIDTH;
	io.DisplaySize.y = SCREEN_HEIGHT;
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("msgothic.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(m_pRenderer->GetDevice());
	ImGui::StyleColorsDark();
	ImGui::SetNextWindowSize(ImVec2(320, 100));

	m_pCamera = new CCamera;					//�J�����I�u�W�F�N�g�̐���
	m_pCamera->Init();
	
	CLight* pLight = NULL;
	pLight = new CLight;
	pLight->Init();

	m_pInput = new CInputKeyboard;				//�L�[�{�[�h����
	m_pInput->Init(hInstance, hWnd);

	LoadFile();									//�t�@�C���ǂݍ���
	CDebug::Init();								//�f�o�b�O���̏����������i����FPS�̂݁j
	CText::Create(ZERO_VEC, 100, "�e�X�g�Ă���test����������123", CText::ALIGN_LEFT, "Nikkyou Sans", D3DCOLOR_RGBA(255, 255, 255, 255));
	CPlayer::Create(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	CPolygon3d::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1000.0f, 1000.0f, 0));
	CPolygon3d::Create(D3DXVECTOR3(0, 1000, 0), D3DXVECTOR3(0, 0, 180), D3DXVECTOR3(1000.0f, 1000.0f, 0));
	CPolygon3d::Create(D3DXVECTOR3(0, 500, -500), D3DXVECTOR3(90, 0, 0), D3DXVECTOR3(1000.0f, 1000.0f, 0));
	CPolygon3d::Create(D3DXVECTOR3(0, 500, 500), D3DXVECTOR3(-90, 0, 0), D3DXVECTOR3(1000.0f, 1000.0f, 0));
	CPolygon3d::Create(D3DXVECTOR3(500, 500, 0), D3DXVECTOR3(0, 0, 90), D3DXVECTOR3(1000.0f, 1000.0f, 0));
	CPolygon3d::Create(D3DXVECTOR3(-500, 500, 0), D3DXVECTOR3(0, 0, -90), D3DXVECTOR3(1000.0f, 1000.0f, 0));
	
	return hResult;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�I���֐�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CManager::Uninit(void)
{
	//�V�[���̏I��
	CScene::ReleaseAll();

	//�f�o�b�O���̏I��
	CDebug::Uninit();

	//���͂̏I��
	m_pInput->Uninit();		
	delete m_pInput;		
	m_pInput = NULL;
	
	//�J�����̏I��
	m_pCamera->Uninit();	
	delete m_pCamera;		
	m_pCamera = NULL;	

	//�����_���̏I��
	m_pRenderer->Uninit();	
	delete m_pRenderer;
	m_pRenderer = NULL; 

	//�ǂݍ��񂾃t�@�C���̃A�����[�h
	UnloadFiles();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�X�V�֐�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CManager::Update(void)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//���͂̍X�V
	m_pInput->Update();

	//�����_���̍X�V
	m_pRenderer->Update();	

	m_pCamera->Update();


	ImGui::Begin("Debug",&m_bImShowAnotherWindow);
	ImGui::Text("FPS:%d", GetFPS());
	if (ImGui::Button("ToggleShadow"))
	{
		m_bUseShadow = !m_bUseShadow;
	}
	if (ImGui::Button("Effect"))
	{
		CEffect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(1, 1, 0),false);
	}
	if (ImGui::Button("Effect(Nega)"))
	{
		CEffect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(1, 1, 0), true);
	}
	if (ImGui::Button("mozi"))
	{
		CGlyph::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), "��");
	}
	ImGui::End();
	ImGui::EndFrame();
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�`��֐�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CManager::Draw(void)
{
	m_pCamera->SetCamera();
	//�����_���̕`��
	m_pRenderer->Draw();	
	
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�Q�b�^�[�֐�
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}

CInput* CManager::GetInput(void)
{
	return m_pInput;
}

CCamera* CManager::GetCamera(int nIndex)
{
	//�z�񉻕K�v
	return m_pCamera;
}

bool CManager::GetUseShadow(void)
{
	return m_bUseShadow;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�t�@�C���̃��[�h
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CManager::LoadFile(void)
{
	CPlayer::Load();
	CEffect::Load();
	CText::Load();
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�t�@�C���̃A�����[�h
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CManager::UnloadFiles(void)
{
	CPlayer::Unload();
	CEffect::Unload();
	CText::Unload();
}