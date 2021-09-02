//=============================================================================
//
// エフェクト2D処理 [effect2d.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene2d.h"
#include "effectData.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// エフェクト2Dクラス定義
//=============================================================================
class CEffect2D :public CScene2D
{
public:

    CEffect2D(CScene::OBJTYPE objtype);
    ~CEffect2D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static CEffect2D * Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius = 0.0f);
    static CEffect2D * Create(const int nType, D3DXVECTOR3 pos, float fScatterAngle = 0.0f, const bool bIn3D = false);
    void MoveCalculation(const int nType, const float fScatterAngle);

    /*========================================================
    // セッター
    //======================================================*/
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetColChangeRate(D3DXCOLOR colChangeRate) { m_colChangeRate = colChangeRate; }
    void SetUseUpdate(bool bUseUpdate) { m_bUseUpdate = bUseUpdate; }

private:
    int m_nTexType;                 // 使うテクスチャの種類

    D3DXVECTOR3 m_move;             // 移動量
    float m_fAcceleration;          // 加速度（1.0基準）
    float m_fGravity;               // 重力
    float m_fGravityLimit;          // 重力制限

    float m_fRotAngle;              // 回転角度（effect3dから持ってきた際に追加）
    float m_fRotSpeed;              // 回転速度

    D3DXCOLOR m_col;                // 色
    D3DXCOLOR m_colChangeRate;      // 色変化割合
    float m_fFadeOutRate;           // α値を下げていく割合（色変化割合でも可能だが、明示的に用意した）
    int m_nCntFadeOutLock;          // α値をロックするカウンタ

    D3DXVECTOR3 m_sizeChangeRate;   // 拡大/縮小割合
    int m_nCntSizeChangeLock;       // 大きさをロックするカウンタ
    bool m_bSizeChangeStartOrFinish;// 大きさ変更を開始するか終了するか（ロックのカウンタと併用）

    bool m_bUseAdditiveSynthesis;   // 加算合成にするかどうか
    //bool m_bUseZBuffer;           // Zバッファをつかうかどうか（effect3dから持ってきた際に削除）

    //int m_nCntAnimTime;           // アニメーション用カウンタ（effect3dから持ってきた際に削除、scene2dに元々あるため）
    bool m_bOneRoundAnim;           // アニメーションが一周したかどうか
    int m_nAnimPattern;             // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）
    int m_nAnimSpeed;               // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）
    bool m_bRepeat;                 // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）

    bool m_bUseUpdate;              // 更新処理をするかどうか（反転合成の時に使ったりする）
};
#endif
