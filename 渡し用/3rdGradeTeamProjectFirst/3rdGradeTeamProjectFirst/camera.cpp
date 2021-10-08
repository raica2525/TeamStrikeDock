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
#include "text.h"

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
    m_shakePhase = SHAKE_PHASE_NONE;
    m_fShakeValue = 0.0f;
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
void CCamera::ResetCamera(D3DXVECTOR3 pos, float fRot, SETTING setting)
{
    switch (setting)
    {
    case SETTING_GAME:
        m_posRDest = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_pos = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_posV = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_posR = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_fDistance = CAMERA_LOCK_ON_OFFSET;
        m_state = STATE_BUTTLE;   // バトル中のカメラにする
        m_fTheta = 1.4f;
        break;
    case SETTING_CUSTOM:
        m_posRDest = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_pos = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_posV = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_posR = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_fDistance = CAMERA_LOCK_ON_OFFSET;
        m_state = STATE_OUT_GAME;   // アウトゲームのカメラにする
        m_fTheta = 1.4f;
        break;
    }

    m_posVDest = DEFAULT_VECTOR;
    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_fPhi = fRot;
    m_nCntState = 0;
    m_shakePhase = SHAKE_PHASE_NONE;

    // 位置を更新(球面座標の公式)
    m_posV.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
    m_posV.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
    m_posV.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;
}

//=============================================================================
// カメラの振動処理
// Author : 後藤慎之助
//=============================================================================
void CCamera::Shake(void)
{
    // カウンタを加算
    m_nCntState++;

    // 位置を変える
    const int CHANGE_FRAME = 3;
    D3DXVECTOR3 cameraPos = m_pos;
    switch (m_shakePhase)
    {
    case SHAKE_PHASE_1:
        cameraPos.x += m_fShakeValue;
        cameraPos.y += m_fShakeValue;
        break;
    case SHAKE_PHASE_2:
        cameraPos.x += -m_fShakeValue;
        cameraPos.y += -m_fShakeValue;
        break;
    case SHAKE_PHASE_3:
        cameraPos.x += m_fShakeValue;
        cameraPos.y += -m_fShakeValue;
        break;
    case SHAKE_PHASE_4:
        cameraPos.x += -m_fShakeValue;
        cameraPos.y += m_fShakeValue;
        break;
    case SHAKE_PHASE_5:
        cameraPos.x += m_fShakeValue / 2;
        cameraPos.y += m_fShakeValue / 2;
        break;
    case SHAKE_PHASE_6:
        cameraPos.x -= m_fShakeValue / 2;
        cameraPos.y -= m_fShakeValue / 2;
        break;
    }

    // 次の振動段階へ
    if (m_nCntState >= CHANGE_FRAME)
    {
        m_nCntState = 0;
        m_shakePhase++;
        if (m_shakePhase >= SHAKE_PHASE_MAX)
        {
            m_shakePhase = SHAKE_PHASE_NONE;
        }
    }

    // 位置の目的地を更新(球面座標の公式)
    m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + cameraPos.x;
    m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + cameraPos.y;
    m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + cameraPos.z;

    // カメラの位置と注視点を更新
    m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
    m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;
}

//=============================================================================
// カメラ振動設定
// Author : 後藤慎之助
//=============================================================================
void CCamera::SetShake(float fShakeValue, bool bResetShake)
{
    if (CGame::GetState() == CGame::STATE_BUTTLE)
    {
        m_fShakeValue = fShakeValue;
        if (m_shakePhase == SHAKE_PHASE_NONE)
        {
            m_nCntState = 0;
            m_shakePhase = SHAKE_PHASE_1;
        }
        else
        {
            if (bResetShake)
            {
                m_nCntState = 0;
                m_shakePhase = SHAKE_PHASE_1;
            }
        }
    }
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
    m_shakePhase = SHAKE_PHASE_NONE;

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
    bool bResetCamera = false;
    switch (m_state)
    {
    case STATE_OUT_GAME:

        // カメラと自身の距離
        m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // 位置の目的地を更新(球面座標の公式)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // カメラの位置と注視点を更新
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;

    case STATE_BUTTLE:

        // カメラと自身の距離
        m_fDistance = CAMERA_LOCK_ON_OFFSET;

        if (m_shakePhase == SHAKE_PHASE_NONE)
        {
            // 位置の目的地を更新(球面座標の公式)
            m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
            m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
            m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

            // カメラの位置と注視点を更新
            m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
            m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;
        }
        else
        {
            Shake();
        }

        break;

    case STATE_FINISH_EACH:
    {
        // カウンタ加算
        m_nCntState++;

        if (m_nCntState < CAMERA_FINISH_NEXT_PLAYER_FRAME)
        {
            // 最初は敗者にロックオン
            CPlayer *pLoser = NULL;
            for (int nCntPlayer = 0; nCntPlayer < CGame::GetNumAllPlayer(); nCntPlayer++)
            {
                CPlayer *pPlayer = CGame::GetPlayer(nCntPlayer);
                if (pPlayer)
                {
                    // 敗者を記憶
                    if (pPlayer->GetIdxControlAndColor() == CGame::GetPlayerRankInThisRound(CPlayer::RANK_2))
                    {
                        pLoser = pPlayer;
                    }
                }
            }

            // 敗者が存在するなら
            if (pLoser)
            {
                m_fPhi += CAMERA_FINISH_ROT_SPEED;
                m_pos = pLoser->GetPos() + D3DXVECTOR3(0.0f, pLoser->GetCollisionSizeDeffence().y / 2, 0.0f);
                m_posRDest = m_pos;
            }
        }
        else
        {
            // 次に勝者へロックオン
            CPlayer *pWinner = NULL;
            for (int nCntPlayer = 0; nCntPlayer < CGame::GetNumAllPlayer(); nCntPlayer++)
            {
                CPlayer *pPlayer = CGame::GetPlayer(nCntPlayer);
                if (pPlayer)
                {
                    // 勝者を記憶
                    if (pPlayer->GetIdxControlAndColor() == CGame::GetPlayerRankInThisRound(CPlayer::RANK_1))
                    {
                        pWinner = pPlayer;
                    }
                }
            }

            // 敗者が存在するなら
            if (pWinner)
            {
                m_fPhi -= CAMERA_FINISH_ROT_SPEED;
                m_pos = pWinner->GetPos() + D3DXVECTOR3(0.0f, pWinner->GetCollisionSizeDeffence().y / 2, 0.0f);
                m_posRDest = m_pos;
            }
        }

        //// カメラと自身の距離
        //m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // 位置の目的地を更新(球面座標の公式)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // カメラの位置と注視点を更新
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;
    }

    case STATE_SP:
    {
        // カウンタ加算し、必殺演出時間を管理
        m_nCntState++;
        if (m_nCntState >= PLAYER_SP_WHOLE_FRAME)
        {
            m_nCntState = 0;
            CGame::SetCurrentSpShot(false);
            bResetCamera = true;
        }
        else if (m_nCntState == 80)
        {
            CText *pSpName = CGame::GetSpText();
            if (pSpName)
            {
                pSpName->SetColor(TEXT_NOT_EXIST_COLOR);
            }
        }

        // 必殺技使用者にロックオン
        CPlayer *pUser = CGame::GetSpPlayer();
        if (pUser)
        {
            m_pos = pUser->GetPartsPos(CPlayer::PARTS_HEAD);
            m_posRDest = m_pos;
        }

        //// カメラと自身の距離
        //m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // 位置の目的地を更新(球面座標の公式)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // カメラの位置と注視点を更新
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;
    }

    }

    // ビューマトリックスの作成
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    // プロジェクションマトリックスの作成
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, CAMERA_VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

    // リセットするフラグがあったら、カメラをリセット
    if (bResetCamera)
    {
        CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_GAME);
    }
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