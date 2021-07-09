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
        STATE_BUTTLE,           // バトル中
        STATE_FINISH_ROTATION,  // 決着時の回転
        STATE_FINISH_EACH,      // 決着時の各自注視
    }STATE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    CCamera *Create(void);
    void ResetGameCamera(D3DXVECTOR3 pos, float fRot);                  // ゲーム遷移時のカメラのリセット

    //===============================
    // セッター
    //===============================
    void SetState(STATE state) { m_nCntState = 0; m_state = state; }    // 状態をセットするときは、カウンタも同時にリセット
    void SetLookAt(D3DXVECTOR3 pos) { m_posRDest = pos; }

    //===============================
    // ゲッター
    //===============================
    STATE GetState(void) { return m_state; }
    D3DXVECTOR3 GetPos(void) { return m_posV; }
    D3DXMATRIX GetProjectionMtx(void) { return m_mtxProjection; }
    D3DXMATRIX GetViewMtx(void) { return m_mtxView; }

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
};

#endif