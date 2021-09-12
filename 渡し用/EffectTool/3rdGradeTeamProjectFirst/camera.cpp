//==============================================================================================================================
//
// カメラ処理 (camera.cpp)
// Author : 後藤慎之助
//
//==============================================================================================================================

//========================
// インクルードファイル
//========================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "character.h"
#include "player.h"

//========================
// 静的メンバ変数宣言
//========================

//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CCamera::CCamera()
{
    // ビューマトリックスの初期化
    D3DXMatrixIdentity(&m_mtxView);

    // プロジェクションマトリックスの初期化
    D3DXMatrixIdentity(&m_mtxProjection);

    m_posRDest = DEFAULT_VECTOR;
    m_posVDest = DEFAULT_VECTOR;
    m_pos = DEFAULT_VECTOR;
    m_posV = DEFAULT_VECTOR;
    m_posR = DEFAULT_VECTOR;
    m_fDistance = CAMERA_LOCK_ON_OFFSET;

    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_state = STATE_NONE;
    m_fPhi = 0.0f;
    m_fTheta = 1;
    m_nCntState = 0;

    m_StartMousePos = DEFAULT_VECTOR;
    m_AmountMovement = DEFAULT_VECTOR;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// ゲーム遷移時のカメラのリセット
// Author : 後藤慎之助
//=============================================================================
void CCamera::ResetGameCamera(D3DXVECTOR3 pos, float fRot)
{
    m_posRDest = pos + CAMERA_LOCK_ON_POS_ADJUST;
    m_posVDest = DEFAULT_VECTOR;
    m_pos = pos + CAMERA_LOCK_ON_POS_ADJUST;
    m_posV = pos + CAMERA_LOCK_ON_POS_ADJUST;
    m_posR = pos + CAMERA_LOCK_ON_POS_ADJUST;
    m_fDistance = CAMERA_LOCK_ON_OFFSET;

    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_state = STATE_NONE;
    m_fPhi = fRot;
    m_fTheta = 1.4f;
    m_nCntState = 0;

    // マウスの情報を初期化
    ResetMousePos();
    POINT Point;
    GetCursorPos(&Point);
    m_StartMousePos = D3DXVECTOR3(Point.x, Point.y, 0.0f);
    m_AmountMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// マウスの位置リセット処理
// Author : 後藤慎之助
//=============================================================================
void CCamera::ResetMousePos(void)
{
    HWND hDesktop = GetDesktopWindow();

    WINDOWINFO windowInfo;
    LONG pos_x, pos_y;
    windowInfo.cbSize = sizeof(WINDOWINFO);

    GetWindowInfo(hDesktop, &windowInfo);
    pos_x = windowInfo.rcWindow.left;
    pos_y = windowInfo.rcWindow.top;
    SetCursorPos(SCREEN_WIDTH / 2 + pos_x, SCREEN_HEIGHT / 2 + pos_y);
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助
//=============================================================================
HRESULT CCamera::Init(void)
{
    // デバイスを取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 仮に、ステージ1開始時のプレイヤーの位置に合わせている
    m_posRDest = DEFAULT_VECTOR;
    m_posVDest = DEFAULT_VECTOR;
    m_pos = DEFAULT_VECTOR + CAMERA_LOCK_ON_POS_ADJUST;
    m_posV = DEFAULT_VECTOR + CAMERA_LOCK_ON_POS_ADJUST;
    m_posR = DEFAULT_VECTOR + CAMERA_LOCK_ON_POS_ADJUST;
    m_pos = DEFAULT_VECTOR;
    m_posV = DEFAULT_VECTOR;
    m_posR = DEFAULT_VECTOR;
    m_fDistance = CAMERA_LOCK_ON_OFFSET;

    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_state = STATE_NONE;
    m_fPhi = 0.0f;
    m_fTheta = 1;
    m_nCntState = 0;

    // マウスの情報を初期化
    ResetMousePos();
    POINT Point;
    GetCursorPos(&Point);
    m_StartMousePos = D3DXVECTOR3(Point.x, Point.y, 0.0f);
    m_AmountMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // ビューマトリックスの作成
    D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    // プロジェクションマトリックスの作成
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, CAMERA_VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新処理
// Author : 後藤慎之助
//=============================================================================
void CCamera::Update(void)
{
    //  カメラ位置修正処理
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    switch (m_state)
    {
    case STATE_NONE:

        break;

    case STATE_HORIZON_MOVE:

        //// 自機にロックオン
        //m_pos = CGame::GetPlayer()->GetPos() + CAMERA_LOCK_ON_POS_ADJUST;
        //m_posRDest = m_pos;

        // カメラと自身の距離
        m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // 視点移動
        MoveViewpoint();

        // 位置の目的地を更新(球面座標の公式)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // カメラの位置と注視点を更新
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;

    //case STATE_IN_FADE:

    //    //// 自機にロックオン
    //    //m_pos = CGame::GetPlayer()->GetPos() + CAMERA_LOCK_ON_POS_ADJUST;
    //    //m_posRDest = m_pos;

    //    //// カメラと自身の距離
    //    //m_fDistance = CAMERA_LOCK_ON_OFFSET;

    //    //// 位置の目的地を更新(球面座標の公式)
    //    //m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
    //    //m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
    //    //m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

    //    //// カメラの位置と注視点を更新
    //    //m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
    //    //m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

    //    break;

    //case STATE_FAILD:

    //    //// 自機にロックオン
    //    //m_pos = CGame::GetPlayer()->GetPos() + CAMERA_FAILD_POS_ADJUST;
    //    //m_posRDest = m_pos;

    //    //// カメラと自身の距離
    //    //m_fDistance = CAMERA_FAILD_OFFSET;

    //    //// 横回転
    //    //m_fPhi += CAMERA_FAILD_ROT_SPEED;

    //    //// 縦の位置
    //    //m_fTheta = CAMERA_FAILD_HEIGHT;

    //    //// 位置の目的地を更新(球面座標の公式)
    //    //m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
    //    //m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
    //    //m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

    //    //// カメラの位置と注視点を更新
    //    //m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
    //    //m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

    //    break;
    }

    // ビューマトリックスの作成
    D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    // プロジェクションマトリックスの作成
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, CAMERA_VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//=============================================================================
// インスタンス生成処理
// Author : 後藤慎之助
//=============================================================================
CCamera * CCamera::Create(void)
{
    CCamera *pCamera = NULL;
    pCamera = new CCamera;
    pCamera->Init();
    return pCamera;
}

//=============================================================================
// 横の角度取得処理
// Author : 後藤慎之助
//=============================================================================
float CCamera::GetAngle(void)
{
    // 返す角度
    float fReturnAngle = 0.0f;

    // 角度を決める(カメラの位置から、注視点を見た時の角度)
    fReturnAngle = atan2f((m_posR.x - m_posV.x), (m_posR.z - m_posV.z));

    return fReturnAngle;
}

//=============================================================================
// プレイヤーの角度取得処理
// Author : 後藤慎之助
//=============================================================================
float CCamera::GetPlayerRot(void)
{
    // 返す角度
    float fReturnAngle = 0.0f;

    // 角度を決める(カメラの位置から、注視点を見た時の角度)
    fReturnAngle = atan2f((m_posR.x - m_posV.x)*-1, (m_posR.z - m_posV.z)*-1);

    return fReturnAngle;
}

//=============================================================================
// 弾の発射角度取得処理
// Author : 後藤慎之助
//=============================================================================
float CCamera::GetShotAngle(bool bUseSpread)
{
    // 返す角度
    float fReturnAngle = 0.0f;

    // 角度を決める(カメラの位置から、注視点を見た時の角度)
    fReturnAngle = atan2f((m_posR.x - m_posV.x), (m_posR.z - m_posV.z));

    // 拡散させるなら
    if (bUseSpread == true)
    {
        // ランダムな角度を足して、拡散させる
        float fRandAngle = float(rand() % RAND_HORIZON_DIFFUSION_VALUE) / 100.0f - float(rand() % RAND_HORIZON_DIFFUSION_VALUE) / 100.0f;
        fReturnAngle += D3DXToRadian(fRandAngle);
    }

    return fReturnAngle;
}

//=============================================================================
// 縦の角度取得処理(銃を撃つときに必要)
// Author : 後藤慎之助
//=============================================================================
float CCamera::GetShotTheta(bool bUseSpread)
{
    // 返す角度
    float fReturnTheta = m_fTheta;

    // 上下範囲調整
    if (fReturnTheta >= CAMERA_THETA_DOWN_ACCURATE_LIMIT)
    {
        if (fReturnTheta >= CAMERA_THETA_DOWN_LIMIT_3)
        {
            if (fReturnTheta >= CAMERA_THETA_DOWN_LIMIT_2)
            {
                if (fReturnTheta >= CAMERA_THETA_DOWN_LIMIT)
                {
                    fReturnTheta = CAMERA_THETA_DOWN_LIMIT_VALUE;
                }
                else
                {
                    fReturnTheta = CAMERA_THETA_DOWN_LIMIT_2;
                }
            }
            else
            {
                fReturnTheta = CAMERA_THETA_DOWN_LIMIT_3;
            }
        }
        else
        {
            fReturnTheta = CAMERA_THETA_DOWN_ACCURATE_LIMIT;
        }
    }

    // 拡散させるなら
    if (bUseSpread == true)
    {
        // ランダムな角度を足して、拡散させる
        float fRandAngle = float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f - float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f;
        fReturnTheta += D3DXToRadian(fRandAngle);
    }

    return fReturnTheta;
}

//=============================================================================
// 空中で縦の角度取得処理(銃を撃つときに必要)
// Author : 後藤慎之助
//=============================================================================
float CCamera::GetShotThetaInAir(bool bUseSpread)
{
    // 返す角度
    float fReturnTheta = m_fTheta;

    // 上下範囲調整
    if (fReturnTheta >= CAMERA_THETA_DOWN_ACCURATE_LIMIT)
    {
        if (fReturnTheta >= CAMERA_THETA_DOWN_LIMIT_3)
        {
            if (fReturnTheta >= CAMERA_THETA_ADJUST_IN_AIR)
            {
                if (fReturnTheta >= CAMERA_THETA_DOWN_LIMIT_2)
                {
                    if (fReturnTheta >= CAMERA_THETA_DOWN_LIMIT)
                    {
                        fReturnTheta = CAMERA_THETA_DOWN_LIMIT_VALUE;
                    }
                    else
                    {
                        fReturnTheta = CAMERA_THETA_DOWN_LIMIT_2;
                    }
                }
                else
                {
                    fReturnTheta = CAMERA_THETA_ADJUST_IN_AIR;
                }
            }
            else
            {
                fReturnTheta = CAMERA_THETA_DOWN_LIMIT_3;
            }
        }
        else
        {
            fReturnTheta = CAMERA_THETA_DOWN_ACCURATE_LIMIT;
        }
    }

    // 拡散させるなら
    if (bUseSpread == true)
    {
        // ランダムな角度を足して、拡散させる
        float fRandAngle = float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f - float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f;
        fReturnTheta += D3DXToRadian(fRandAngle);
    }

    return fReturnTheta;
}

//=============================================================================
// カメラの視点移動処理
// Author : 後藤慎之助
//=============================================================================
void CCamera::MoveViewpoint(void)
{
    //// マウスの座標取得
    //POINT Point;
    //GetCursorPos(&Point);
    //D3DXVECTOR3 MousePos = D3DXVECTOR3(Point.x, Point.y, 0);

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 右スティックが倒れているなら
    if (Controller.lZ != 0 || Controller.lRz != 0)
    {
        // 右移動
        if (Controller.lZ > 0)
        {
            m_fPhi -= CAMERA_ROT_RIGHT_LEFT_SPEED;
        }
        // 左移動
        else if (Controller.lZ < 0)
        {
            m_fPhi += CAMERA_ROT_RIGHT_LEFT_SPEED;
        }

        // 上移動
        if (Controller.lRz > 0)
        {
            m_fTheta += CAMERA_ROT_UP_DOWN_SPEED;
        }
        // 下移動
        else if (Controller.lRz < 0)
        {
            m_fTheta -= CAMERA_ROT_UP_DOWN_SPEED;
        }
    }
    //// 右スティックが倒れていないなら、マウスを使える
    //else
    //{
    //    // マウスの移動量を計算
    //    m_AmountMovement.y = (m_StartMousePos.y - MousePos.y) * CAMERA_SENSITIVITY;
    //    m_AmountMovement.x = (MousePos.x - m_StartMousePos.x) * CAMERA_SENSITIVITY;

    //    m_fPhi -= D3DXToRadian(m_AmountMovement.x);
    //    m_fTheta -= D3DXToRadian(m_AmountMovement.y);
    //}

    // 左右範囲調整
    if (m_fPhi >= D3DXToRadian(360.0f))
    {
        m_fPhi = 0.0f;
    }
    else if (m_fPhi <= D3DXToRadian(-360.0f))
    {
        m_fPhi = 0.0f;
    }

    // 上下範囲調整
    if (m_fTheta >= CAMERA_ROT_DOWN_LIMIT)
    {
        m_fTheta = CAMERA_ROT_DOWN_LIMIT;
    }
    else if (m_fTheta <= CAMERA_ROT_UP_LIMIT)
    {
        m_fTheta = CAMERA_ROT_UP_LIMIT;
    }

    //// マウスの位置をリセット
    //ResetMousePos();
}
