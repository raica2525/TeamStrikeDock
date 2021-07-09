//=============================================================================
//
// エフェクト3D処理 [effect3d.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "billboard.h"
#include "effectData.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// エフェクト3Dクラス定義
//=============================================================================
class CEffect3D :public CBillboard
{
public:

    CEffect3D(CScene::OBJTYPE objtype);
    ~CEffect3D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static CEffect3D * Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius = 0.0f);
    static CEffect3D * Create(const int nType, D3DXVECTOR3 pos, const float fScatterAngle);
    void MoveCalculation(const int nType, const float fScatterAngle);

    // セッター
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetColChangeRate(D3DXCOLOR colChangeRate) { m_colChangeRate = colChangeRate; }

private:
    int m_nTexType;                 // 使うテクスチャの種類

    D3DXVECTOR3 m_move;             // 移動量
    float m_fAcceleration;          // 加速度（1.0基準）
    float m_fGravity;               // 重力

    float m_fRotSpeed;              // 回転速度

    D3DXCOLOR m_col;                // 色
    D3DXCOLOR m_colChangeRate;      // 色変化割合
    float m_fFadeOutRate;           // α値を下げていく割合（色変化割合でも可能だが、明示的に用意した）

    D3DXVECTOR3 m_sizeChangeRate;   // 拡大/縮小割合

    bool m_bUseAdditiveSynthesis;   // 加算合成にするかどうか
    bool m_bUseZBuffer;             // Zバッファをつかうかどうか

    int m_nCntAnimTime;             // アニメーション用カウンタ
    bool m_bOneRoundAnim;           // アニメーションが一周したかどうか
    int m_nAnimPattern;             // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）
    int m_nAnimSpeed;               // テクスチャ情報保持用（updateで毎回取得するのを防ぐ）
};
#endif
