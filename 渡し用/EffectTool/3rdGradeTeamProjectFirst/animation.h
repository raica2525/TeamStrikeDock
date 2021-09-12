//====================================================================
//
// アニメーション処理 (animation.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "character.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define KEY_MAX 10          // キーセットの最大数
#define ANIMATION_MAX 24    // 持てるアニメーションの最大数(0番目は、必ず待機モーションにする)

//================================================
// クラス宣言
//================================================

//アニメーションクラス
class CAnimation
{
public:
    CAnimation();
    ~CAnimation();

    // 列挙型宣言(animation.hにも追加が必要)
    typedef enum
    {
        TYPE_NONE = 0,
        TYPE_HUMAN,		// プレイヤー
        TYPE_SPIDER,	// 蜘蛛
        TYPE_ANT,		// 蟻
        TYPE_FORCE,		// 防衛軍
        TYPE_UFO,		// UFO
        TYPE_TOWER,		// タワー
        TYPE_WYVERN,	// ワイバーン
        TYPE_MAX		// 最大値
    }ANIM_TYPE;

    // 構造体宣言
    typedef struct
    {
        D3DXVECTOR3 pos[KEY_MAX];
        D3DXVECTOR3 rot[KEY_MAX];
    }PARTS_INFO;

    typedef struct
    {
        PARTS_INFO  parts[CHARCTER_PARTS_MAX];
        int			nFrame[KEY_MAX];
        int			nLoop;
        int			nKeyNum;
    }KEY_INFO;

    HRESULT Init(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type);									// 初期化処理
    void Uninit(void);																						// 終了処理
    void Update(void);																						// 更新処理
    void Draw(void);																						// 描画処理
    static CAnimation * Create(CCharacter *pCharacter, int nPartsNum, ANIM_TYPE type, char* cFilePass);		// インスタンス生成処理
    void LoadAnimation(ANIM_TYPE type, char* pass);															// アニメーションの読み込み処理
    void SetAnimation(int anim) { m_nAnimation = anim; }													// アニメーション設定処理
    int GetAnimation(void) { return m_nAnimation; }															// インスタンス取得用

private:
    KEY_INFO				m_keyInfo[TYPE_MAX][ANIMATION_MAX];	// キーアニメーションデータ保持用		
    int				        m_nAnimation;						// 現在のアニメーション
    int				        m_nAnimationOld;					// 前回のアニメーション
    ANIM_TYPE				m_type;								// アニメーションのタイプ
    D3DXVECTOR3				m_pos[CHARCTER_PARTS_MAX];			// 座標
    D3DXVECTOR3				m_posDest[CHARCTER_PARTS_MAX];		// 座標移動量
    D3DXVECTOR3				m_rot[CHARCTER_PARTS_MAX];			// 回転
    D3DXVECTOR3				m_rotDest[CHARCTER_PARTS_MAX];		// 回転移動量
    CCharacter				*m_pCharacter;						// キャラクターのインスタンス
    int						m_nFrame;							// フレーム数
    int						m_nKey;								// キー数
    int						m_nPartsNum;						// モデルのパーツ数
    bool					m_bAdjustment;						// 角度を修正したか
};

#endif