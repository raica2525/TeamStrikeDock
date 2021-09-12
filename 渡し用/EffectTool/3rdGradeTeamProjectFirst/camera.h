//========================================================================================================================
//
// カメラ処理 [camera.h]
// Author : 後藤慎之助
//
//========================================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//================================================
// マクロ定義
//================================================

#define CAMERA_DEFAULT_ROT D3DXToRadian(90.0f)

// ロックオン時のカメラの位置の微調整
#define CAMERA_LOCK_ON_POS_ADJUST D3DXVECTOR3(0.0f, 400.0f, -1000.0f)
#define CAMERA_LOCK_ON_OFFSET -750.0f

// 失敗時のカメラの位置微調整
#define CAMERA_FAILD_POS_ADJUST D3DXVECTOR3(0.0f, 200.0f, 0.0f)
#define CAMERA_FAILD_OFFSET -600.0f
#define CAMERA_FAILD_HEIGHT D3DXToRadian(75.0f)
#define CAMERA_FAILD_ROT_SPEED D3DXToRadian(1.0f)

// カメラの移動速度
#define CAMERA_MOVE_RATE 0.1f                                     // カメラの補間値

// カメラの回転速度                                               
#define CAMERA_ROT_RIGHT_LEFT_SPEED D3DXToRadian(2.0f)            // コントローラの左右
#define CAMERA_ROT_UP_DOWN_SPEED D3DXToRadian(1.5f)               // コントローラの上下
#define CAMERA_SENSITIVITY (0.03f)                                // マウス感度

// カメラの回転範囲制限                                           
#define CAMERA_ROT_DOWN_LIMIT D3DXToRadian(160.0f)                // カメラの下限
#define CAMERA_ROT_UP_LIMIT D3DXToRadian(32.5f)                   // カメラの上限

// カメラのシータで返す値の制限
#define CAMERA_THETA_DOWN_LIMIT D3DXToRadian(155.0f)              // 床を撃つ感じ
#define CAMERA_THETA_DOWN_LIMIT_2 D3DXToRadian(132.5f)            // 目の前よりやや下を撃つ感じ132.5
#define CAMERA_THETA_ADJUST_IN_AIR D3DXToRadian(115.0f)           // 空中で下を撃つときの補正
#define CAMERA_THETA_DOWN_LIMIT_3 D3DXToRadian(97.5f)             // 目の前を狙う感じ
#define CAMERA_THETA_DOWN_ACCURATE_LIMIT D3DXToRadian(92.5f)      // 92.5が正確に狙える上限

#define CAMERA_THETA_DOWN_LIMIT_VALUE D3DXToRadian(179.0f)        // 床を撃つときの値

// ランダムな拡散の値(÷100を考慮)
#define RAND_HORIZON_DIFFUSION_VALUE 250                          // 水平
#define RAND_VERTICAL_DIFFUSION_VALUE 250                         // 垂直

// 視野
#define CAMERA_VIEW_RANGE 25000.0f

// 視野角
#define CAMERA_VIEW_ANGLE D3DXToRadian(45.0f)

//================================================
// カメラクラス宣言
//================================================
class CCamera
{
public:
    CCamera();
    ~CCamera();

    // カメラの状態
    typedef enum
    {
        STATE_NONE = 0,         // なし
        STATE_HORIZON_MOVE,     // 横移動
        STATE_FINISH_ROTATION,  // 決着時の回転
        STATE_FINISH_EACH,      // 決着時の各自注視
    }STATE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    CCamera *Create(void);
    void SetLookAt(D3DXVECTOR3 pos) { m_posRDest = pos; }
    void ResetGameCamera(D3DXVECTOR3 pos, float fRot);                  // ゲーム遷移時のカメラのリセット
    STATE GetState(void) { return m_state; }
    void SetState(STATE state) { m_nCntState = 0; m_state = state; }    // 状態をセットするときは、カウンタも同時にリセット
    float GetAngle(void);                                               // カメラの横の向きを取得
    float GetPlayerRot(void);                                           // プレイヤーの向く方向を取得
    float GetShotAngle(bool bUseSpread);                                // 射撃の横の角度取得
    float GetShotTheta(bool bUseSpread);                                // 地上で射撃の縦の角度取得
    float GetShotThetaInAir(bool bUseSpread);                           // 空中で射撃の縦の角度取得
    D3DXVECTOR3 GetPos(void) { return m_posV; }

    void ResetMousePos(void);
    void MoveViewpoint(void);

private:
    D3DXVECTOR3	m_posRDest;         // カメラの注視点（目的値）
    D3DXVECTOR3	m_posVDest;         // カメラの座標（目的値）
    STATE m_state;                  // カメラの状態
    D3DXVECTOR3	m_posV;             // カメラの座標
    D3DXVECTOR3	m_pos;              // カメラの座標
    D3DXVECTOR3	m_posR;             // 注視点
    D3DXVECTOR3	m_vecU;             // 上方向のベクトル
    D3DXMATRIX	m_mtxProjection;    // プロジェクションマトリクス
    D3DXMATRIX	m_mtxView;          // ビューマトリクス
    float		m_fDistance;        // 距離
    float		m_fPhi;             // ファイ
    float		m_fTheta;           // シータ
    int m_nCntState;                // 状態カウンタ

    D3DXVECTOR3 m_AmountMovement;   // マウスの移動量
    D3DXVECTOR3 m_StartMousePos;    // マウスの開始位置
};

#endif