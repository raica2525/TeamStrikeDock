//===============================================
//
// �G�t�F�N�g�}�l�[�W���[���� (effectmanager.cpp)
// Author : �ɓ��z��
//
//===============================================

//===============================================
// �C���N���[�h�t�@�C��
//===============================================
#include "effecttool.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "renderer.h"
#include "manager.h"
#include "effect3d.h"
#include "character.h"
#include "effectData.h"
#include <array>

//================================================
// �}�N����`
//================================================
#define SPEED_MAX (2000)				// ���x�̍ő�l
#define ACCELERATION_MAX (1.0f)			// �����x�̍ő�l
#define GRAVITY_MAX (100.0f)			// �d�͂̍ő�l
#define COLCHANGERATE_MAX (1.0f)		// �F�ω������̍ő�l
#define FADEOUTRATE_MAX (1.0f)			// �t�F�[�h�A�E�g�̊����̍ő�l
#define EFFECT_SIZE_MAX (50000)			// �T�C�Y�̍ő�l
#define EFFECT_SIZE_MIN (0)				// �T�C�Y�̍ŏ��l
#define SIZECHANGERATE_MAX (2000.0f)	// �T�C�Y�̊g��/�k�������ő�l
#define SIZECHANGERATE_MIN (-2000.0f)	// �T�C�Y�̊g��/�k�������ŏ��l
#define ALPHATEST_MAX (255)				// �A���t�@�e�X�g�̍ő�l
#define IMGUI_BAR_WIDTH_INIT (215.0f)	// IMGUI�̃o�[�̉����̏����l
#define IMGUI_BAR_WIDTH_MAX (1000.0f)	// IMGUI�̃o�[�̉����̍ő�l
#define CREATE_FLAME_MAX (240.0f)		// ��������t���[���̍ő�l
#define INT_ZERO (0)
#define FLOAT_ZERO (0.0f)

//===============================================
// �G�t�F�N�g�}�l�[�W���[�̃R���X�g���N�^
// Author : �ɓ��z��
//===============================================
CEffectTool::CEffectTool()
{
	m_EffectInfo = InitEffectInfo();				// �G�t�F�N�g�̏��̏�����
	m_EffectInfoCopy = InitEffectInfo();			// �R�s�[�p�G�t�F�N�g�̏��̏�����

	m_nEffectType = 0;								// �G�t�F�N�g�̃^�C�v
	m_fCreateFlame = 0.0f;							// �G�t�F�N�g�𐶐����鎞��
	m_fCountFlame = 0.0f;							// �G�t�F�N�g�𐶐����鎞�Ԃ̃J�E���g
	m_fBarWidth = IMGUI_BAR_WIDTH_INIT;				// IMGUI�̃o�[�̉���
	memset(m_bCreate, false, sizeof(m_bCreate));	// �������邩���Ȃ���
}

//===============================================
// �G�t�F�N�g�c�[���̃f�X�g���N�^
// Author : �ɓ��z��
//===============================================
CEffectTool::~CEffectTool()
{

}

//===============================================
// �G�t�F�N�g�c�[���̏���������
// Author : �ɓ��z��
//===============================================
HRESULT CEffectTool::Init(HWND hWnd)
{
	// �G�t�F�N�g�̃f�[�^����l���Z�b�g
	SetEffectInfo(m_nEffectType);

	// imgui�̃Z�b�g�A�b�v
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// �t�H���g�̓ǂݍ���
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("Data/fonts/meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// imgui�̃X�^�C��
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	// imgui�̏���������
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CManager::GetRenderer()->GetDevice());
	return S_OK;
}

//===============================================
// �G�t�F�N�g�c�[���̏I������
// Author : �ɓ��z��
//===============================================
void CEffectTool::Uninit(void)
{		
	m_EffectInfo = InitEffectInfo();				// �G�t�F�N�g�̏��̏�����
	m_EffectInfoCopy = InitEffectInfo();			// �R�s�[�p�G�t�F�N�g�̏��̏�����

	m_nEffectType = 0;								// �G�t�F�N�g�̃^�C�v
	m_fCreateFlame = 0.0f;							// �G�t�F�N�g�𐶐����鎞��
	m_fCountFlame = 0.0f;							// �G�t�F�N�g�𐶐����鎞�Ԃ̃J�E���g
	m_fBarWidth = 0.0f;								// IMGUI�̃o�[�̉���
	memset(m_bCreate, false, sizeof(m_bCreate));	// �������邩���Ȃ���

	// imgui�̃V���b�g�_�E��
	ImGui_ImplDX9_Shutdown();
}

//�O���[�o���̈�Ƀt���O�z�u
static bool show_demo_window = true;
static bool show_another_window = true;
//===============================================
// �G�t�F�N�g�}�l�[�W���[�̍X�V����
// Author : �ɓ��z��
//===============================================
void CEffectTool::Update(void)
{
	int nEffectType = m_nEffectType;
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �G�t�F�N�g�̏����f�[�^����󂯎��
	SetEffectInfo(m_nEffectType);

	//if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::PushItemWidth(m_fBarWidth);

	ImGui::Begin(u8"�G�t�F�N�g�c�[��");

	// ���W�Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("pos"))
	{
		ImGui::DragFloat3("pos", m_EffectInfo.pos, 0.1f, -1000.0f, 1000.0f);	// ���W
	}

	// �G�t�F�N�g�Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("effectoption"))
	{
		// ���{����͍�肽���������ǖ�����������
		//ImGui::InputText(u8"�G�t�F�N�g�̖��O", m_Text, sizeof(m_Text));

		// �^�C�v�̐؂�ւ�
		ImGui::SliderInt(u8"�G�t�F�N�g�̃^�C�v", &m_nEffectType, INT_ZERO, CEffectData::TYPE::TYPE_MAX - 1);

		// �e�N�X�`���̑I��
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGui::PushButtonRepeat(true);
		if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { m_EffectInfo.nTexType--; }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { m_EffectInfo.nTexType++; }

		// 0�ȉ��ɂȂ�Ȃ��悤��
		if (m_EffectInfo.nTexType < INT_ZERO)
		{
			m_EffectInfo.nTexType = INT_ZERO;
		}
		ImGui::SameLine();
		ImGui::Text("%d", m_EffectInfo.nTexType);
		ImGui::SameLine();
		ImGui::Text(u8"�e�N�X�`���̎��");

		// ������
		ImGui::InputInt(u8"������", &m_EffectInfo.nNumOnce);

		// ��������1���͏��Ȃ��Ȃ�Ȃ�
		if (m_EffectInfo.nNumOnce < 1)
		{
			m_EffectInfo.nNumOnce = 1;
		}

		ImGui::SliderInt(u8"����", &m_EffectInfo.nPlane, INT_ZERO, 1);									// ���ʂ̐؂�ւ�
		ImGui::Checkbox(u8"�~����ɂ��邩�ǂ���", &m_EffectInfo.bOnCircleEdge);							// �~����ɂ��邩�ǂ����i�ő���o���a�����ƂɁj
		ImGui::Checkbox(u8"���o����������ɂ��邩�ǂ���", &m_EffectInfo.bEmitVectorInside);				// ���o����������ɂ��邩�ǂ���
		ImGui::DragInt(u8"��U�p�x�i0�`314�j", &m_EffectInfo.nScatterAngle, 1, INT_ZERO, EFFECT_PI);	// ��U�p�x�i0�`314�j
		ImGui::DragInt(u8"��U���i0�`314�j", &m_EffectInfo.nScatterWidth, 1, INT_ZERO, EFFECT_PI);		// ��U���i0�`314�j
		ImGui::DragFloat(u8"���a�̑傫��", &m_EffectInfo.fEmitRadius, 1.0f, FLOAT_ZERO, 10000.0f);		// ���a�̑傫��
	}

	// �ړ��Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("move"))
	{
		ImGui::DragInt(u8"�ő呬�x", &m_EffectInfo.nSpeedMax, 1, m_EffectInfo.nSpeedMin, SPEED_MAX);								// �ő呬�x
		ImGui::DragInt(u8"�ŏ����x", &m_EffectInfo.nSpeedMin, 1, -SPEED_MAX, m_EffectInfo.nSpeedMax);								// �ŏ����x
		ImGui::DragInt(u8"�ő�⏕��", &m_EffectInfo.nAuxiliaryAxisSpeedMax, 1, m_EffectInfo.nAuxiliaryAxisSpeedMin, SPEED_MAX);	// �ő�⏕���i2D��3D�ŁA���̊����o�����߂̂��́j
		ImGui::DragInt(u8"�ŏ��⏕��", &m_EffectInfo.nAuxiliaryAxisSpeedMin, 1, -SPEED_MAX, m_EffectInfo.nAuxiliaryAxisSpeedMax);	// �ŏ��⏕���i2D��3D�ŁA���̊����o�����߂̂��́j
		ImGui::DragFloat(u8"�����x(1.0�ő�)", &m_EffectInfo.fAcceleration, 0.001f, FLOAT_ZERO, ACCELERATION_MAX);					// �����x(1.0�ő�)
		ImGui::DragFloat(u8"�d��", &m_EffectInfo.fGravity, 0.01f, FLOAT_ZERO, GRAVITY_MAX);											// �d��
		ImGui::DragFloat(u8"�d�͐���", &m_EffectInfo.fGravityLimit, 0.01f, FLOAT_ZERO, GRAVITY_MAX);								// �d��
	}

	// ��]�Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("rot"))
	{
		ImGui::SliderInt(u8"�|���S�����ǂ̕����ɌŒ肷�邩", &m_EffectInfo.nLayRot, INT_ZERO, 4);					// �|���S�����̂��̂��ǂ̕����ɌŒ肷�邩�i1,2�Ȃ�r���{�[�h�����j
		ImGui::Checkbox(u8"��]���ړ��̌����ɍ��킹�邩", &m_EffectInfo.bRotEqualMoveAngle);						// ��]���ړ��̌����ɍ��킹�邩
		ImGui::DragInt(u8"�ő��]���x", &m_EffectInfo.nRotSpeedMax, 1, m_EffectInfo.nRotSpeedMin, EFFECT_PI*2);	// �ő��]���x
		ImGui::DragInt(u8"�ŏ���]���x", &m_EffectInfo.nRotSpeedMin, 1, INT_ZERO, m_EffectInfo.nRotSpeedMax);		// �ŏ���]���x
	}

	// �F�Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("col"))
	{
		ImGui::ColorEdit4(u8"�F", m_EffectInfo.col, ImGuiColorEditFlags_Float);											// �F
		ImGui::DragFloat4(u8"�F�ω�����", m_EffectInfo.colChangeRate, 0.001f, -COLCHANGERATE_MAX, COLCHANGERATE_MAX);	// �F�ω�����
		ImGui::DragFloat(u8"�t�F�[�h�A�E�g�̊���", &m_EffectInfo.fFadeOutRate, 0.001f, FLOAT_ZERO, FADEOUTRATE_MAX);	// �t�F�[�h�A�E�g�̊���
		ImGui::DragInt(u8"�A���t�@�l�����b�N����J�E���^", &m_EffectInfo.nCntFadeOutLock, 1, INT_ZERO, 1000);			// ���l�̕ω������b�N����J�E���^

	}

	// �T�C�Y�Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("size"))
	{
		ImGui::DragInt(u8"�ő�̑傫��", &m_EffectInfo.nSizeMax, 1, m_EffectInfo.nSizeMin, EFFECT_SIZE_MAX);				// �ő�傫��
		ImGui::DragInt(u8"�ŏ��̑傫��", &m_EffectInfo.nSizeMin, 1, EFFECT_SIZE_MIN, m_EffectInfo.nSizeMax);				// �ŏ��傫��
		ImGui::DragFloat3(u8"�g��/�k������", m_EffectInfo.fSizeChangeRate, 0.01f, SIZECHANGERATE_MIN, SIZECHANGERATE_MAX);	// �g��/�k������
		ImGui::DragInt(u8"�傫�������b�N����J�E���^", &m_EffectInfo.nCntSizeChangeLock, 1, EFFECT_SIZE_MIN, 1000);			// �ŏ��傫��
		ImGui::Checkbox(u8"�傫����ς���̂�", &m_EffectInfo.bSizeChangeStartOrFinish);									// �傫����ς���̂��n�܂邩�A�I���̂�
	}

	// �`��Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("draw"))
	{
		ImGui::Checkbox(u8"���Z�������邩�ǂ���", &m_EffectInfo.bUseAdditiveSynthesis);								// ���Z�������邩�ǂ���
		ImGui::Checkbox(u8"Z�o�b�t�@���g����", &m_EffectInfo.bUseZBuffer);											// Z�o�b�t�@���g����
		ImGui::DragInt(u8"�A���t�@�e�X�g�̃{�[�_�[", &m_EffectInfo.nAlphaTestBorder, 1, INT_ZERO, ALPHATEST_MAX);	// ���e�X�g�̃{�[�_�[
	}

	// �G�t�F�N�g�̐����Ɋւ���ݒ�
	if (ImGui::CollapsingHeader("create"))
	{
		ImGui::DragFloat(u8"��������t���[��", &m_fCreateFlame, 1.0f, FLOAT_ZERO, CREATE_FLAME_MAX);	// ��������t���[��

		// �G�t�F�N�g�𐶐����邩�ǂ���(for����ł��Ȃ���������Ȃɂ��������@�͂Ȃ����񂩂˂�)
		ImGui::Checkbox(u8"�G�t�F�N�g1", &m_bCreate[0]);
		ImGui::Checkbox(u8"�G�t�F�N�g2", &m_bCreate[1]);
		ImGui::Checkbox(u8"�G�t�F�N�g3", &m_bCreate[2]);
		ImGui::Checkbox(u8"�G�t�F�N�g4", &m_bCreate[3]);
		ImGui::Checkbox(u8"�G�t�F�N�g5", &m_bCreate[4]);
		ImGui::Checkbox(u8"�G�t�F�N�g6", &m_bCreate[5]);
		ImGui::Checkbox(u8"�G�t�F�N�g7", &m_bCreate[6]);
		ImGui::Checkbox(u8"�G�t�F�N�g8", &m_bCreate[7]);
		ImGui::Checkbox(u8"�G�t�F�N�g9", &m_bCreate[8]);
		ImGui::Checkbox(u8"�G�t�F�N�g10", &m_bCreate[9]);
		ImGui::Checkbox(u8"�G�t�F�N�g11", &m_bCreate[10]);
		ImGui::Checkbox(u8"�G�t�F�N�g12", &m_bCreate[11]);
		ImGui::Checkbox(u8"�G�t�F�N�g13", &m_bCreate[12]);
		ImGui::Checkbox(u8"�G�t�F�N�g14", &m_bCreate[13]);

		if (ImGui::Button("upload"))
		{
			CManager::GetEffectData()->UploadEffectInfo(m_nEffectType);
		}
	}

	// Imgui�̊e��ݒ�
	if (ImGui::CollapsingHeader("Option"))
	{
		ImGui::DragFloat(u8"����", &m_fBarWidth, 1.0f, FLOAT_ZERO, IMGUI_BAR_WIDTH_MAX);	// ����
	}

	// �J�E���g���ő�l�𒴂�����
	if (m_fCountFlame > CREATE_FLAME_MAX)
	{
		m_fCountFlame = INT_ZERO;
	}
	else
	{
		// �J�E���g�̉��Z
		m_fCountFlame++;
	}

	// �ݒ肵���t���[���ƃJ�E���g����v������
	if (m_fCreateFlame == m_fCountFlame)
	{
		// �G�t�F�N�g�̐��������擾
		CEffectData *pEffectData = CManager::GetEffectData();

		for (int nCount = 0; nCount < CEffectData::TYPE::TYPE_MAX; nCount++)
		{
			if (m_bCreate[nCount])
			{
				// �G�t�F�N�g����
				CEffect3D::Emit(nCount, D3DXVECTOR3(m_EffectInfo.pos[0], m_EffectInfo.pos[1], m_EffectInfo.pos[2]),
					D3DXVECTOR3(m_EffectInfo.pos[0], m_EffectInfo.pos[1], m_EffectInfo.pos[2]), pEffectData->GetCreateInfo(nCount)->fEmitRadius);
			}
		}
		// �J�E���g�̏�����
		m_fCountFlame = INT_ZERO;
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	// �G�t�F�N�g�̃^�C�v���ύX����Ă���
	if (nEffectType != m_nEffectType)
	{
		// �G�t�F�N�g�̏����f�[�^����󂯎��
		SetEffectInfo(m_nEffectType);
	}

	// �X�V�����l���f�[�^�ɃZ�b�g����
	CManager::GetEffectData()->SetCreateInfo(m_nEffectType);

	ImGui::EndFrame(); //�X�V�����̏I����
}

//===============================================
// �G�t�F�N�g�c�[���̕`�揈��
// Author : �ɓ��z��
//===============================================
void CEffectTool::Draw(void)
{
	// imgui�̕`�揈��
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//===============================================
// �G�t�F�N�g�c�[���̏�����C�ɏ��������鏈��
// Author : �ɓ��z��
//===============================================
CEffectTool::EffectInfo CEffectTool::InitEffectInfo(void)
{
	EffectInfo Effectinfo;
	memset(Effectinfo.pos, NULL, sizeof(Effectinfo.pos));							// ���W

	Effectinfo.nTexType = 0;														// �g���e�N�X�`���̎��
	Effectinfo.nNumOnce = 0;														// ����������
	Effectinfo.nPlane = 0;															// ����

	Effectinfo.bOnCircleEdge = false;												// �~����ɂ��邩�ǂ����i�ő���o���a�����ƂɁj
	Effectinfo.bEmitVectorInside = false;											// ���o����������ɂ��邩�ǂ���
	Effectinfo.nScatterAngle = 0;													// ��U�p�x�i0�`314�j
	Effectinfo.nScatterWidth = 0;													// ��U���i0�`314�j
	Effectinfo.fEmitRadius = 0.0f;													// ���a�̑傫��

	Effectinfo.nSpeedMax = 0;														// �ő呬�x
	Effectinfo.nSpeedMin = 0;														// �ŏ����x
	Effectinfo.nAuxiliaryAxisSpeedMax = 0;											// �ő�⏕���i2D��3D�ŁA���̊����o�����߂̂��́j
	Effectinfo.nAuxiliaryAxisSpeedMin = 0;											// �ŏ��⏕���i2D��3D�ŁA���̊����o�����߂̂��́j

	Effectinfo.fAcceleration = 0.0f;												// �����x(1.0�ő�)
	Effectinfo.fGravity = 0.0f;														// �d��
	Effectinfo.fGravityLimit = 0.0f;												// �d�͐���

	Effectinfo.nLayRot = 0;														// �|���S�����̂��̂����ɂ��邩�ǂ����itrue�Ȃ�r���{�[�h�����j
	Effectinfo.nRotSpeedMax = 0;													// �ő��]���x
	Effectinfo.nRotSpeedMin = 0;													// �ŏ���]���x

	memset(Effectinfo.col, NULL, sizeof(Effectinfo.col));							// �F
	memset(Effectinfo.colChangeRate, NULL, sizeof(Effectinfo.colChangeRate));		// �F�ω�����
	Effectinfo.fFadeOutRate = 0.0f;													// �t�F�[�h�A�E�g�̊���
	Effectinfo.nCntFadeOutLock = 0;													// ���l�̕ω������b�N����J�E���^

	Effectinfo.nSizeMax = 0;														// �ő�傫��
	Effectinfo.nSizeMin = 0;														// �ŏ��傫��
	memset(Effectinfo.fSizeChangeRate, NULL, sizeof(Effectinfo.fSizeChangeRate));	// �g��/�k������	
	Effectinfo.nCntSizeChangeLock = 0;												// �傫�������b�N����J�E���^
	Effectinfo.bSizeChangeStartOrFinish = false;									// �傫����ς���̂��n�܂邩�A�I���̂�

	Effectinfo.bUseAdditiveSynthesis = false;										// ���Z�������邩�ǂ���
	Effectinfo.bUseZBuffer = false;													// Z�o�b�t�@���g����
	Effectinfo.nAlphaTestBorder = 0;												// ���e�X�g�̃{�[�_�[

	return Effectinfo;
}

//===============================================
// �G�t�F�N�g�c�[���̃G�t�F�N�g���Z�b�g����
// Author : �ɓ��z��
//===============================================
void CEffectTool::SetEffectInfo(int nEffectType)
{
	// �G�t�F�N�g�̐��������擾
	CEffectData *pEffectData = CManager::GetEffectData();
	CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nEffectType);

	m_EffectInfo.nTexType = pCreateInfo->nTexType;								// �g���e�N�X�`���̎��
	m_EffectInfo.nNumOnce = pCreateInfo->nNumOnce;								// ����������
	m_EffectInfo.nPlane = pCreateInfo->plane;									// ����

	m_EffectInfo.bOnCircleEdge = pCreateInfo->bOnCircleEdge;					// �~����ɂ��邩�ǂ���
	m_EffectInfo.bEmitVectorInside = pCreateInfo->bEmitVectorInside;			// ���o����������ɂ��邩�ǂ���
	m_EffectInfo.nScatterAngle = pCreateInfo->nScatterAngle;					// ��U�p�x�i0�`314�j
	m_EffectInfo.nScatterWidth = pCreateInfo->nScatterWidth;					// ��U���i0�`314�j
	m_EffectInfo.fEmitRadius = pCreateInfo->fEmitRadius;						// ���a�̑傫��

	m_EffectInfo.nSpeedMax = pCreateInfo->nSpeedMax;							// �ő呬�x
	m_EffectInfo.nSpeedMin = pCreateInfo->nSpeedMin;							// �ŏ����x
	m_EffectInfo.nAuxiliaryAxisSpeedMax = pCreateInfo->nAuxiliaryAxisSpeedMax;	// �ő�⏕���i2D��3D�ŁA���̊����o�����߂̂��́j
	m_EffectInfo.nAuxiliaryAxisSpeedMin = pCreateInfo->nAuxiliaryAxisSpeedMin;	// �ŏ��⏕���i2D��3D�ŁA���̊����o�����߂̂��́j

	m_EffectInfo.fAcceleration = pCreateInfo->fAcceleration;					// �����x(1.0�ő�)
	m_EffectInfo.fGravity = pCreateInfo->fGravity;								// �d��
	m_EffectInfo.fGravityLimit = pCreateInfo->fGravityLimit;					// �d�͐���

	m_EffectInfo.nLayRot = pCreateInfo->nLayRot;								// �|���S�����̂��̂����ɂ��邩�ǂ����itrue�Ȃ�r���{�[�h�����j
	m_EffectInfo.bRotEqualMoveAngle = pCreateInfo->bRotEqualMoveAngle;			// ��]���ړ��̌����ɍ��킹�邩
	m_EffectInfo.nRotSpeedMax = pCreateInfo->nRotSpeedMax;						// �ő��]���x
	m_EffectInfo.nRotSpeedMin = pCreateInfo->nRotSpeedMin;						// �ŏ���]���x
																					
	m_EffectInfo.col[0] = pCreateInfo->col.r;									// �F
	m_EffectInfo.col[1] = pCreateInfo->col.g;										
	m_EffectInfo.col[2] = pCreateInfo->col.b;										
	m_EffectInfo.col[3] = pCreateInfo->col.a;										
	m_EffectInfo.colChangeRate[0] = pCreateInfo->colChangeRate.r;				// �F�ω�����
	m_EffectInfo.colChangeRate[1] = pCreateInfo->colChangeRate.g;					
	m_EffectInfo.colChangeRate[2] = pCreateInfo->colChangeRate.b;
	m_EffectInfo.colChangeRate[3] = pCreateInfo->colChangeRate.a;					
	m_EffectInfo.fFadeOutRate = pCreateInfo->fFadeOutRate;						// �t�F�[�h�A�E�g�̊���
	m_EffectInfo.nCntFadeOutLock = pCreateInfo->nCntFadeOutLock;				// ���l�̕ω������b�N����J�E���^

	m_EffectInfo.nSizeMax = pCreateInfo->nSizeMax;								// �ő�傫��
	m_EffectInfo.nSizeMin = pCreateInfo->nSizeMin;								// �ŏ��傫��
	m_EffectInfo.fSizeChangeRate[0] = pCreateInfo->sizeChangeRate.x;			// �g��/�k������
	m_EffectInfo.fSizeChangeRate[1] = pCreateInfo->sizeChangeRate.y;				
	m_EffectInfo.fSizeChangeRate[2] = pCreateInfo->sizeChangeRate.z;				
	m_EffectInfo.nCntSizeChangeLock = pCreateInfo->nCntSizeChangeLock;				// �傫�������b�N����J�E���^
	m_EffectInfo.bSizeChangeStartOrFinish = pCreateInfo->bSizeChangeStartOrFinish;	// �傫����ς���̂��n�܂邩�A�I���̂�

	m_EffectInfo.bUseAdditiveSynthesis = pCreateInfo->bUseAdditiveSynthesis;	// ���Z�������邩�ǂ���
	m_EffectInfo.bUseZBuffer = pCreateInfo->bUseZBuffer;						// Z�o�b�t�@���g����
	m_EffectInfo.nAlphaTestBorder = pCreateInfo->nAlphaTestBorder;				// ���e�X�g�̃{�[�_�[
}

//===============================================
// �G�t�F�N�g�c�[���̃G�t�F�N�g���擾����
// Author : �ɓ��z��
//===============================================
CEffectTool::EffectInfo * CEffectTool::GetEffectInfo(int nEffectType)
{
	return &m_EffectInfo;
}
