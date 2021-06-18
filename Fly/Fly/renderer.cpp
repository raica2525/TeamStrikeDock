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
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

																// �f�o�C�X�̐���
																// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
																// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice))) {
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
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

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�����x���g����
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//���f�X�e�B

																		// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //�h�b�g�G�̂Ƃ���D3DTEXF_POINT
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	srand((unsigned int)time(0));
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void)
{

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
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
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	// �|���S���̍X�V����
	CScene::UpdateAll();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pZBuffer = NULL;

	//���݂̕`����ۑ�
	m_pD3DDevice->GetRenderTarget(0, &pBackBuffer);
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffer);

	//�`�����e�N�X�`���ɐݒ�
	LPDIRECT3DSURFACE9 pSurf = NULL;
	m_pTexture->GetSurfaceLevel(0, &pSurf);
	m_pD3DDevice->SetRenderTarget(0, pSurf);

	for (int nCount = 0; nCount < 2; nCount++)
	{
		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		// Direct3D�ɂ��`��̊J�n
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{
			// �|���S���̕`�揈��
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

			// Direct3D�ɂ��`��̏I��
			m_pD3DDevice->EndScene();

		}
		if (nCount == 0)
		{
			//�`�����o�b�N�o�b�t�@�ɕύX
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

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

