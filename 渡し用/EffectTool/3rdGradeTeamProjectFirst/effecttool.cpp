//===============================================
//
// エフェクトマネージャー処理 (effectmanager.cpp)
// Author : 伊藤陽梧
//
//===============================================

//===============================================
// インクルードファイル
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
// マクロ定義
//================================================
#define SPEED_MAX (2000)				// 速度の最大値
#define ACCELERATION_MAX (1.0f)			// 加速度の最大値
#define GRAVITY_MAX (100.0f)			// 重力の最大値
#define COLCHANGERATE_MAX (1.0f)		// 色変化割合の最大値
#define FADEOUTRATE_MAX (1.0f)			// フェードアウトの割合の最大値
#define EFFECT_SIZE_MAX (50000)			// サイズの最大値
#define EFFECT_SIZE_MIN (0)				// サイズの最小値
#define SIZECHANGERATE_MAX (2000.0f)	// サイズの拡大/縮小割合最大値
#define SIZECHANGERATE_MIN (-2000.0f)	// サイズの拡大/縮小割合最小値
#define ALPHATEST_MAX (255)				// アルファテストの最大値
#define IMGUI_BAR_WIDTH_INIT (215.0f)	// IMGUIのバーの横幅の初期値
#define IMGUI_BAR_WIDTH_MAX (1000.0f)	// IMGUIのバーの横幅の最大値
#define CREATE_FLAME_MAX (240.0f)		// 生成するフレームの最大値
#define INT_ZERO (0)
#define FLOAT_ZERO (0.0f)

//===============================================
// エフェクトマネージャーのコンストラクタ
// Author : 伊藤陽梧
//===============================================
CEffectTool::CEffectTool()
{
	m_EffectInfo = InitEffectInfo();				// エフェクトの情報の初期化
	m_EffectInfoCopy = InitEffectInfo();			// コピー用エフェクトの情報の初期化

	m_nEffectType = 0;								// エフェクトのタイプ
	m_fCreateFlame = 0.0f;							// エフェクトを生成する時間
	m_fCountFlame = 0.0f;							// エフェクトを生成する時間のカウント
	m_fBarWidth = IMGUI_BAR_WIDTH_INIT;				// IMGUIのバーの横幅
	memset(m_bCreate, false, sizeof(m_bCreate));	// 生成するかしないか
}

//===============================================
// エフェクトツールのデストラクタ
// Author : 伊藤陽梧
//===============================================
CEffectTool::~CEffectTool()
{

}

//===============================================
// エフェクトツールの初期化処理
// Author : 伊藤陽梧
//===============================================
HRESULT CEffectTool::Init(HWND hWnd)
{
	// エフェクトのデータから値をセット
	SetEffectInfo(m_nEffectType);

	// imguiのセットアップ
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// フォントの読み込み
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("Data/fonts/meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// imguiのスタイル
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	// imguiの初期化処理
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CManager::GetRenderer()->GetDevice());
	return S_OK;
}

//===============================================
// エフェクトツールの終了処理
// Author : 伊藤陽梧
//===============================================
void CEffectTool::Uninit(void)
{		
	m_EffectInfo = InitEffectInfo();				// エフェクトの情報の初期化
	m_EffectInfoCopy = InitEffectInfo();			// コピー用エフェクトの情報の初期化

	m_nEffectType = 0;								// エフェクトのタイプ
	m_fCreateFlame = 0.0f;							// エフェクトを生成する時間
	m_fCountFlame = 0.0f;							// エフェクトを生成する時間のカウント
	m_fBarWidth = 0.0f;								// IMGUIのバーの横幅
	memset(m_bCreate, false, sizeof(m_bCreate));	// 生成するかしないか

	// imguiのシャットダウン
	ImGui_ImplDX9_Shutdown();
}

//グローバル領域にフラグ配置
static bool show_demo_window = true;
static bool show_another_window = true;
//===============================================
// エフェクトマネージャーの更新処理
// Author : 伊藤陽梧
//===============================================
void CEffectTool::Update(void)
{
	int nEffectType = m_nEffectType;
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// エフェクトの情報をデータから受け取る
	SetEffectInfo(m_nEffectType);

	//if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::PushItemWidth(m_fBarWidth);

	ImGui::Begin(u8"エフェクトツール");

	// 座標に関する設定
	if (ImGui::CollapsingHeader("pos"))
	{
		ImGui::DragFloat3("pos", m_EffectInfo.pos, 0.1f, -1000.0f, 1000.0f);	// 座標
	}

	// エフェクトに関する設定
	if (ImGui::CollapsingHeader("effectoption"))
	{
		// 日本語入力作りたかったけど無理だったわ
		//ImGui::InputText(u8"エフェクトの名前", m_Text, sizeof(m_Text));

		// タイプの切り替え
		ImGui::SliderInt(u8"エフェクトのタイプ", &m_nEffectType, INT_ZERO, CEffectData::TYPE::TYPE_MAX - 1);

		// テクスチャの選択
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGui::PushButtonRepeat(true);
		if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { m_EffectInfo.nTexType--; }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { m_EffectInfo.nTexType++; }

		// 0以下にならないように
		if (m_EffectInfo.nTexType < INT_ZERO)
		{
			m_EffectInfo.nTexType = INT_ZERO;
		}
		ImGui::SameLine();
		ImGui::Text("%d", m_EffectInfo.nTexType);
		ImGui::SameLine();
		ImGui::Text(u8"テクスチャの種類");

		// 発生数
		ImGui::InputInt(u8"発生数", &m_EffectInfo.nNumOnce);

		// 発生数は1よりは少なくならない
		if (m_EffectInfo.nNumOnce < 1)
		{
			m_EffectInfo.nNumOnce = 1;
		}

		ImGui::SliderInt(u8"平面", &m_EffectInfo.nPlane, INT_ZERO, 1);									// 平面の切り替え
		ImGui::Checkbox(u8"円周上にするかどうか", &m_EffectInfo.bOnCircleEdge);							// 円周上にするかどうか（最大放出半径をもとに）
		ImGui::Checkbox(u8"放出方向を内側にするかどうか", &m_EffectInfo.bEmitVectorInside);				// 放出方向を内側にするかどうか
		ImGui::DragInt(u8"飛散角度（0～314）", &m_EffectInfo.nScatterAngle, 1, INT_ZERO, EFFECT_PI);	// 飛散角度（0～314）
		ImGui::DragInt(u8"飛散幅（0～314）", &m_EffectInfo.nScatterWidth, 1, INT_ZERO, EFFECT_PI);		// 飛散幅（0～314）
		ImGui::DragFloat(u8"半径の大きさ", &m_EffectInfo.fEmitRadius, 1.0f, FLOAT_ZERO, 10000.0f);		// 半径の大きさ
	}

	// 移動に関する設定
	if (ImGui::CollapsingHeader("move"))
	{
		ImGui::DragInt(u8"最大速度", &m_EffectInfo.nSpeedMax, 1, m_EffectInfo.nSpeedMin, SPEED_MAX);								// 最大速度
		ImGui::DragInt(u8"最小速度", &m_EffectInfo.nSpeedMin, 1, -SPEED_MAX, m_EffectInfo.nSpeedMax);								// 最小速度
		ImGui::DragInt(u8"最大補助軸", &m_EffectInfo.nAuxiliaryAxisSpeedMax, 1, m_EffectInfo.nAuxiliaryAxisSpeedMin, SPEED_MAX);	// 最大補助軸（2D→3Dで、立体感を出すためのもの）
		ImGui::DragInt(u8"最小補助軸", &m_EffectInfo.nAuxiliaryAxisSpeedMin, 1, -SPEED_MAX, m_EffectInfo.nAuxiliaryAxisSpeedMax);	// 最小補助軸（2D→3Dで、立体感を出すためのもの）
		ImGui::DragFloat(u8"加速度(1.0最大)", &m_EffectInfo.fAcceleration, 0.001f, FLOAT_ZERO, ACCELERATION_MAX);					// 加速度(1.0最大)
		ImGui::DragFloat(u8"重力", &m_EffectInfo.fGravity, 0.01f, FLOAT_ZERO, GRAVITY_MAX);											// 重力
		ImGui::DragFloat(u8"重力制限", &m_EffectInfo.fGravityLimit, 0.01f, FLOAT_ZERO, GRAVITY_MAX);								// 重力
	}

	// 回転に関する設定
	if (ImGui::CollapsingHeader("rot"))
	{
		ImGui::SliderInt(u8"ポリゴンを横にするか", &m_EffectInfo.nLayRot, INT_ZERO, 2);								// ポリゴンそのものを横にするかどうか（1,2ならビルボード解除）
		ImGui::Checkbox(u8"回転を移動の向きに合わせるか", &m_EffectInfo.bRotEqualMoveAngle);						// 回転を移動の向きに合わせるか
		ImGui::DragInt(u8"最大回転速度", &m_EffectInfo.nRotSpeedMax, 1, m_EffectInfo.nRotSpeedMin, EFFECT_PI*2);	// 最大回転速度
		ImGui::DragInt(u8"最小回転速度", &m_EffectInfo.nRotSpeedMin, 1, INT_ZERO, m_EffectInfo.nRotSpeedMax);		// 最小回転速度
	}

	// 色に関する設定
	if (ImGui::CollapsingHeader("col"))
	{
		ImGui::ColorEdit4(u8"色", m_EffectInfo.col, ImGuiColorEditFlags_Float);											// 色
		ImGui::DragFloat4(u8"色変化割合", m_EffectInfo.colChangeRate, 0.001f, -COLCHANGERATE_MAX, COLCHANGERATE_MAX);	// 色変化割合
		ImGui::DragFloat(u8"フェードアウトの割合", &m_EffectInfo.fFadeOutRate, 0.001f, FLOAT_ZERO, FADEOUTRATE_MAX);	// フェードアウトの割合
		ImGui::DragInt(u8"アルファ値をロックするカウンタ", &m_EffectInfo.nCntFadeOutLock, 1, INT_ZERO, 1000);			// α値の変化をロックするカウンタ

	}

	// サイズに関する設定
	if (ImGui::CollapsingHeader("size"))
	{
		ImGui::DragInt(u8"最大の大きさ", &m_EffectInfo.nSizeMax, 1, m_EffectInfo.nSizeMin, EFFECT_SIZE_MAX);				// 最大大きさ
		ImGui::DragInt(u8"最小の大きさ", &m_EffectInfo.nSizeMin, 1, EFFECT_SIZE_MIN, m_EffectInfo.nSizeMax);				// 最小大きさ
		ImGui::DragFloat3(u8"拡大/縮小割合", m_EffectInfo.fSizeChangeRate, 0.01f, SIZECHANGERATE_MIN, SIZECHANGERATE_MAX);	// 拡大/縮小割合
		ImGui::DragInt(u8"大きさをロックするカウンタ", &m_EffectInfo.nCntSizeChangeLock, 1, EFFECT_SIZE_MIN, 1000);			// 最小大きさ
		ImGui::Checkbox(u8"大きさを変えるのか", &m_EffectInfo.bSizeChangeStartOrFinish);								// 大きさを変えるのが始まるか、終わるのか
	}

	// 描画に関する設定
	if (ImGui::CollapsingHeader("draw"))
	{
		ImGui::Checkbox(u8"加算合成するかどうか", &m_EffectInfo.bUseAdditiveSynthesis);								// 加算合成するかどうか
		ImGui::Checkbox(u8"Zバッファを使うか", &m_EffectInfo.bUseZBuffer);											// Zバッファを使うか
		ImGui::DragInt(u8"アルファテストのボーダー", &m_EffectInfo.nAlphaTestBorder, 1, INT_ZERO, ALPHATEST_MAX);	// αテストのボーダー
	}

	// エフェクトの生成に関する設定
	if (ImGui::CollapsingHeader("create"))
	{
		ImGui::DragFloat(u8"生成するフレーム", &m_fCreateFlame, 1.0f, FLOAT_ZERO, CREATE_FLAME_MAX);	// 生成するフレーム

		// エフェクトを生成するかどうか(forじゃできなかったからなにかいい方法はないもんかねぇ)
		ImGui::Checkbox(u8"エフェクト1", &m_bCreate[0]);
		ImGui::Checkbox(u8"エフェクト2", &m_bCreate[1]);
		ImGui::Checkbox(u8"エフェクト3", &m_bCreate[2]);
		ImGui::Checkbox(u8"エフェクト4", &m_bCreate[3]);
		ImGui::Checkbox(u8"エフェクト5", &m_bCreate[4]);
		ImGui::Checkbox(u8"エフェクト6", &m_bCreate[5]);
		ImGui::Checkbox(u8"エフェクト7", &m_bCreate[6]);
		ImGui::Checkbox(u8"エフェクト8", &m_bCreate[7]);
		ImGui::Checkbox(u8"エフェクト9", &m_bCreate[8]);
		ImGui::Checkbox(u8"エフェクト10", &m_bCreate[9]);
		ImGui::Checkbox(u8"エフェクト11", &m_bCreate[10]);
		ImGui::Checkbox(u8"エフェクト12", &m_bCreate[11]);
		ImGui::Checkbox(u8"エフェクト13", &m_bCreate[12]);
		ImGui::Checkbox(u8"エフェクト14", &m_bCreate[13]);

		if (ImGui::Button("upload"))
		{
			CManager::GetEffectData()->UploadEffectInfo(m_nEffectType);
		}
	}

	// Imguiの各種設定
	if (ImGui::CollapsingHeader("Option"))
	{
		ImGui::DragFloat(u8"横幅", &m_fBarWidth, 1.0f, FLOAT_ZERO, IMGUI_BAR_WIDTH_MAX);	// 横幅
	}

	// カウントが最大値を超えたら
	if (m_fCountFlame > CREATE_FLAME_MAX)
	{
		m_fCountFlame = INT_ZERO;
	}
	else
	{
		// カウントの加算
		m_fCountFlame++;
	}

	// 設定したフレームとカウントが一致したら
	if (m_fCreateFlame == m_fCountFlame)
	{
		// エフェクトの生成情報を取得
		CEffectData *pEffectData = CManager::GetEffectData();

		for (int nCount = 0; nCount < CEffectData::TYPE::TYPE_MAX; nCount++)
		{
			if (m_bCreate[nCount])
			{
				// エフェクト発生
				CEffect3D::Emit(nCount, D3DXVECTOR3(m_EffectInfo.pos[0], m_EffectInfo.pos[1], m_EffectInfo.pos[2]),
					D3DXVECTOR3(m_EffectInfo.pos[0], m_EffectInfo.pos[1], m_EffectInfo.pos[2]), pEffectData->GetCreateInfo(nCount)->fEmitRadius);
			}
		}
		// カウントの初期化
		m_fCountFlame = INT_ZERO;
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	// エフェクトのタイプが変更されてたら
	if (nEffectType != m_nEffectType)
	{
		// エフェクトの情報をデータから受け取る
		SetEffectInfo(m_nEffectType);
	}

	// 更新した値をデータにセットする
	CManager::GetEffectData()->SetCreateInfo(m_nEffectType);

	ImGui::EndFrame(); //更新処理の終わりに
}

//===============================================
// エフェクトツールの描画処理
// Author : 伊藤陽梧
//===============================================
void CEffectTool::Draw(void)
{
	// imguiの描画処理
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//===============================================
// エフェクトツールの情報を一気に初期化する処理
// Author : 伊藤陽梧
//===============================================
CEffectTool::EffectInfo CEffectTool::InitEffectInfo(void)
{
	EffectInfo Effectinfo;
	memset(Effectinfo.pos, NULL, sizeof(Effectinfo.pos));							// 座標

	Effectinfo.nTexType = 0;														// 使うテクスチャの種類
	Effectinfo.nNumOnce = 0;														// 同時発生数
	Effectinfo.nPlane = 0;															// 平面

	Effectinfo.bOnCircleEdge = false;												// 円周上にするかどうか（最大放出半径をもとに）
	Effectinfo.bEmitVectorInside = false;											// 放出方向を内側にするかどうか
	Effectinfo.nScatterAngle = 0;													// 飛散角度（0～314）
	Effectinfo.nScatterWidth = 0;													// 飛散幅（0～314）
	Effectinfo.fEmitRadius = 0.0f;													// 半径の大きさ

	Effectinfo.nSpeedMax = 0;														// 最大速度
	Effectinfo.nSpeedMin = 0;														// 最小速度
	Effectinfo.nAuxiliaryAxisSpeedMax = 0;											// 最大補助軸（2D→3Dで、立体感を出すためのもの）
	Effectinfo.nAuxiliaryAxisSpeedMin = 0;											// 最小補助軸（2D→3Dで、立体感を出すためのもの）

	Effectinfo.fAcceleration = 0.0f;												// 加速度(1.0最大)
	Effectinfo.fGravity = 0.0f;														// 重力
	Effectinfo.fGravityLimit = 0.0f;												// 重力制限

	Effectinfo.nLayRot = 0;														// ポリゴンそのものを横にするかどうか（trueならビルボード解除）
	Effectinfo.nRotSpeedMax = 0;													// 最大回転速度
	Effectinfo.nRotSpeedMin = 0;													// 最小回転速度

	memset(Effectinfo.col, NULL, sizeof(Effectinfo.col));							// 色
	memset(Effectinfo.colChangeRate, NULL, sizeof(Effectinfo.colChangeRate));		// 色変化割合
	Effectinfo.fFadeOutRate = 0.0f;													// フェードアウトの割合
	Effectinfo.nCntFadeOutLock = 0;													// α値の変化をロックするカウンタ

	Effectinfo.nSizeMax = 0;														// 最大大きさ
	Effectinfo.nSizeMin = 0;														// 最小大きさ
	memset(Effectinfo.fSizeChangeRate, NULL, sizeof(Effectinfo.fSizeChangeRate));	// 拡大/縮小割合	
	Effectinfo.nCntSizeChangeLock = 0;												// 大きさをロックするカウンタ
	Effectinfo.bSizeChangeStartOrFinish = false;									// 大きさを変えるのが始まるか、終わるのか

	Effectinfo.bUseAdditiveSynthesis = false;										// 加算合成するかどうか
	Effectinfo.bUseZBuffer = false;													// Zバッファを使うか
	Effectinfo.nAlphaTestBorder = 0;												// αテストのボーダー

	return Effectinfo;
}

//===============================================
// エフェクトツールのエフェクト情報セット処理
// Author : 伊藤陽梧
//===============================================
void CEffectTool::SetEffectInfo(int nEffectType)
{
	// エフェクトの生成情報を取得
	CEffectData *pEffectData = CManager::GetEffectData();
	CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nEffectType);

	m_EffectInfo.nTexType = pCreateInfo->nTexType;								// 使うテクスチャの種類
	m_EffectInfo.nNumOnce = pCreateInfo->nNumOnce;								// 同時発生数
	m_EffectInfo.nPlane = pCreateInfo->plane;									// 平面

	m_EffectInfo.bOnCircleEdge = pCreateInfo->bOnCircleEdge;					// 円周上にするかどうか
	m_EffectInfo.bEmitVectorInside = pCreateInfo->bEmitVectorInside;			// 放出方向を内側にするかどうか
	m_EffectInfo.nScatterAngle = pCreateInfo->nScatterAngle;					// 飛散角度（0～314）
	m_EffectInfo.nScatterWidth = pCreateInfo->nScatterWidth;					// 飛散幅（0～314）
	m_EffectInfo.fEmitRadius = pCreateInfo->fEmitRadius;						// 半径の大きさ

	m_EffectInfo.nSpeedMax = pCreateInfo->nSpeedMax;							// 最大速度
	m_EffectInfo.nSpeedMin = pCreateInfo->nSpeedMin;							// 最小速度
	m_EffectInfo.nAuxiliaryAxisSpeedMax = pCreateInfo->nAuxiliaryAxisSpeedMax;	// 最大補助軸（2D→3Dで、立体感を出すためのもの）
	m_EffectInfo.nAuxiliaryAxisSpeedMin = pCreateInfo->nAuxiliaryAxisSpeedMin;	// 最小補助軸（2D→3Dで、立体感を出すためのもの）

	m_EffectInfo.fAcceleration = pCreateInfo->fAcceleration;					// 加速度(1.0最大)
	m_EffectInfo.fGravity = pCreateInfo->fGravity;								// 重力
	m_EffectInfo.fGravityLimit = pCreateInfo->fGravityLimit;					// 重力制限

	m_EffectInfo.nLayRot = pCreateInfo->nLayRot;								// ポリゴンそのものを横にするかどうか（trueならビルボード解除）
	m_EffectInfo.bRotEqualMoveAngle = pCreateInfo->bRotEqualMoveAngle;			// 回転を移動の向きに合わせるか
	m_EffectInfo.nRotSpeedMax = pCreateInfo->nRotSpeedMax;						// 最大回転速度
	m_EffectInfo.nRotSpeedMin = pCreateInfo->nRotSpeedMin;						// 最小回転速度
																					
	m_EffectInfo.col[0] = pCreateInfo->col.r;									// 色
	m_EffectInfo.col[1] = pCreateInfo->col.g;										
	m_EffectInfo.col[2] = pCreateInfo->col.b;										
	m_EffectInfo.col[3] = pCreateInfo->col.a;										
	m_EffectInfo.colChangeRate[0] = pCreateInfo->colChangeRate.r;				// 色変化割合
	m_EffectInfo.colChangeRate[1] = pCreateInfo->colChangeRate.g;					
	m_EffectInfo.colChangeRate[2] = pCreateInfo->colChangeRate.b;
	m_EffectInfo.colChangeRate[3] = pCreateInfo->colChangeRate.a;					
	m_EffectInfo.fFadeOutRate = pCreateInfo->fFadeOutRate;						// フェードアウトの割合
	m_EffectInfo.nCntFadeOutLock = pCreateInfo->nCntFadeOutLock;				// α値の変化をロックするカウンタ

	m_EffectInfo.nSizeMax = pCreateInfo->nSizeMax;								// 最大大きさ
	m_EffectInfo.nSizeMin = pCreateInfo->nSizeMin;								// 最小大きさ
	m_EffectInfo.fSizeChangeRate[0] = pCreateInfo->sizeChangeRate.x;			// 拡大/縮小割合
	m_EffectInfo.fSizeChangeRate[1] = pCreateInfo->sizeChangeRate.y;				
	m_EffectInfo.fSizeChangeRate[2] = pCreateInfo->sizeChangeRate.z;				
	m_EffectInfo.nCntSizeChangeLock = pCreateInfo->nCntSizeChangeLock;				// 大きさをロックするカウンタ
	m_EffectInfo.bSizeChangeStartOrFinish = pCreateInfo->bSizeChangeStartOrFinish;	// 大きさを変えるのが始まるか、終わるのか

	m_EffectInfo.bUseAdditiveSynthesis = pCreateInfo->bUseAdditiveSynthesis;	// 加算合成するかどうか
	m_EffectInfo.bUseZBuffer = pCreateInfo->bUseZBuffer;						// Zバッファを使うか
	m_EffectInfo.nAlphaTestBorder = pCreateInfo->nAlphaTestBorder;				// αテストのボーダー
}

//===============================================
// エフェクトツールのエフェクト情報取得処理
// Author : 伊藤陽梧
//===============================================
CEffectTool::EffectInfo * CEffectTool::GetEffectInfo(int nEffectType)
{
	return &m_EffectInfo;
}
