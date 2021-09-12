//===============================================
//
// �`�揈�� (renderer.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
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
#include "effecttool.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
#ifdef _DEBUG
bool CRenderer::m_bDispFont = true;
#endif

//========================================
// �����_�����O�̃f�t�H���g�R���X�g���N�^
// Author : �㓡�T�V��
//========================================
CRenderer::CRenderer()
{
    m_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
    m_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
    m_pFont = NULL;        // �t�H���g�ւ̃|�C���^
#endif
}

//========================================
// �����_�����O�̃f�X�g���N�^
// Author : �㓡�T�V��
//========================================
CRenderer::~CRenderer()
{

}

//========================================
// �����_�����O�̏���������
// Author : �㓡�T�V��
//========================================
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
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �X�e���V���o�b�t�@��24bit�A�f�v�X�o�b�t�@��8bit���g��
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
        &d3dpp, &m_pD3DDevice)))
    {
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

    // �����_�[�X�e�[�g�̐ݒ�
    m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);          // �J�����O������
    m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                  // Z�o�b�t�@���g�p
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);         // ���u�����h���s��
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // ���\�[�X�J���[�̎w��
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// ���f�X�e�B�l�[�V�����J���[
    m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                 // �J�������g�p����
    m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00444444);            // �A���r�G���g�̐ݒ�

                                                                        // �T���v���[�X�e�[�g�̐ݒ�
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);   // �e�N�X�`��U�l�̌J��Ԃ��ݒ�
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);   // �e�N�X�`��V�l�̌J��Ԃ��ݒ�
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);    // �e�N�X�`���g�厞�̕�Ԑݒ�
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);    // �e�N�X�`���k�����̕�Ԑݒ�

                                                                            // �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // �A���t�@�u�����f�B���O����
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // �ŏ��̃A���t�@����(�����l)
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // 2�Ԗڂ̃A���t�@����(�����l)

                                                                            // �}�e���A���̐ݒ�
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    m_pD3DDevice->SetMaterial(&material);

#ifdef _DEBUG
    // �f�o�b�O���\���p�t�H���g�̐���
    D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

    return S_OK;
}

//========================================
// �����_�����O�̏I������
// Author : �㓡�T�V��
//========================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
    // �f�o�b�O���\���p�t�H���g�̔j��
    if (m_pFont != NULL)
    {
        m_pFont->Release();
        m_pFont = NULL;
    }
#endif

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
}

//========================================
// �����_�����O�̍X�V����
// Author : �㓡�T�V��
//========================================
void CRenderer::Update(void)
{
    // �S�ẴI�u�W�F�N�g���X�V
    CScene::UpdateAll();
}

//========================================
// �����_�����O�̕`�揈��
// Author : �㓡�T�V��
//========================================
void CRenderer::Draw(void)
{
    // �o�b�N�o�b�t�@���y�o�b�t�@���X�e���V���o�b�t�@�̃N���A
    m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);

    // Direct3D�ɂ��`��̊J�n
    if (SUCCEEDED(m_pD3DDevice->BeginScene()))
    {
        // �S�ẴI�u�W�F�N�g��`��
        CScene::DrawAll();

        // �Q�[�������A�|�[�Y���Ȃ�
        if (CManager::GetMode() == CManager::MODE_GAME)
        {
            if (CGame::GetState() == CGame::STATE_PAUSE)
            {
                // �|�[�Y��`��
                CPause::Draw();
            }
        }

        // �t�F�[�h��`��
        CFade::Draw();

		CEffectTool *pEffectTool = CManager::GetEffectTool();
		// �G�t�F�N�g�c�[���̕`�揈��
		if (pEffectTool != NULL)
		{
			pEffectTool->Draw();
		}

#ifdef _DEBUG
        if (m_bDispFont)
        {
            // FPS�\��
            DrawDebugData();
        }
#endif
        // Direct3D�ɂ��`��̏I��
        m_pD3DDevice->EndScene();
    }

    // �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
    m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// �f�o�b�O�f�[�^�`�揈��
// Author : �㓡�T�V��
//========================================
#ifdef _DEBUG
void CRenderer::DrawDebugData(void)
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    char str[256];

    // ���[�h�ɂ���āA�\��������̂�ς���
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
// �Q�[�����̃f�o�b�O�f�[�^�`�揈��
// Author : �㓡�T�V��
//========================================
void CRenderer::GameDebugData(void)
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    char str[256];

    // �v���C���[1�̔\�͂��擾
    CPlayer* pPlayer = CGame::GetPlayer(PLAYER_1);
    CBall* pBall = CGame::GetBall();

    // �`����e�����߂�
    wsprintf(str, "FPS:%d\nOBJ:%d\n\nATK:%d\nDEF:%d\nSPD:%d\nWEI:%d\n\nLIFE:%d\nSP_GAUGE:%d\nPOINT:%d\n\nCHARGE:%d\n\nBALL_SPD:%d\nBALL_WHO_SHOOT:%d\nBALL_WHO_ABSORB:%d\nBALL_STOP:%d", 
        GetFPS(), CScene::GetNumObjAll(), (int)pPlayer->GetAtk(), (int)pPlayer->GetDef(), (int)pPlayer->GetSpd(), (int)pPlayer->GetWei(), (int)pPlayer->GetLife(), (int)pPlayer->GetSpGaugeCurrent(), (int)pPlayer->GetPoint(),
        (int)pPlayer->GetSwingCharge(),
        (int)pBall->GetSpeed(), pBall->GetWhoShooting(), pBall->GetWhoAbsorbing(), pBall->GetStopTime());

    // �e�L�X�g�`��
    m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif
