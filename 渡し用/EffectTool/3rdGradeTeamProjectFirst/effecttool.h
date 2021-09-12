//====================================================================
//
// エフェクトツールの処理 (effecttool.h)
// Author : 伊藤　陽梧
//
//====================================================================
#ifndef _EFFECTTOOL_H_
#define _EFFECTTOOL_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// マクロ定義
//================================================
#define EFFECT_TYPE (20)

//================================================
// エフェクトツールクラス
//================================================
class CEffectTool
{
public:

	CEffectTool();
	~CEffectTool();

	typedef struct
	{
		float pos[3];					// 座標

		int nTexType;					// 使うテクスチャの種類
		int nNumOnce;					// 同時発生数
		int nPlane;						// 平面

		bool bOnCircleEdge;				// 円周上にするかどうか
		bool bEmitVectorInside;			// 放出方向を内側にするかどうか
		int nScatterAngle;				// 飛散角度（0～314）
		int nScatterWidth;				// 飛散幅（0～314）
		float fEmitRadius;				// 半径の大きさ

		int nSpeedMax;					// 最大速度
		int nSpeedMin;					// 最小速度
		int nAuxiliaryAxisSpeedMax;		// 最大補助軸（2D→3Dで、立体感を出すためのもの）
		int nAuxiliaryAxisSpeedMin;		// 最小補助軸（2D→3Dで、立体感を出すためのもの）

		float fAcceleration;			// 加速度(1.0最大)
		float fGravity;					// 重力
		float fGravityLimit;			// 重力制限

		bool bLayRot;					// ポリゴンそのものを横にするかどうか（trueならビルボード解除）
		bool bRotEqualMoveAngle;		// 回転を移動の向きに合わせるか
		int nRotSpeedMax;				// 最大回転速度
		int nRotSpeedMin;				// 最小回転速度

		float col[4];					// 色
		float colChangeRate[4];			// 色変化割合
		float fFadeOutRate;				// フェードアウトの割合
		int nCntFadeOutLock;			// α値をロックするカウンタ

		int nSizeMax;					// 最大大きさ
		int nSizeMin;					// 最小大きさ
		float fSizeChangeRate[3];		// 拡大/縮小割合
		int nCntSizeChangeLock;			// 大きさをロックするカウンタ
		bool bSizeChangeStartOrFinish;	// 大きさを変えるのが始まるか、終わるのか

		bool bUseAdditiveSynthesis;		// 加算合成するかどうか
		bool bUseZBuffer;				// Zバッファを使うか
		int nAlphaTestBorder;			// αテストのボーダー
	}EffectInfo;    // エフェクトの情報

	HRESULT Init(HWND hWnd);	// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	EffectInfo InitEffectInfo(void);

	void SetEffectInfo(int nEffectType);
	EffectInfo* GetEffectInfo(int nEffectType);
private:
	EffectInfo m_EffectInfo;
	EffectInfo m_EffectInfoCopy;
	int m_nEffectType;
	float m_fCreateFlame;
	float m_fCountFlame;
	float m_fBarWidth;
	float m_fEmitRadius;
	bool m_bCreate[EFFECT_TYPE];
};

#endif