//==============================================================================================================================
//
// �J�������� (camera.cpp)
// Author : �㓡�T�V��
//
//==============================================================================================================================

//========================
// �C���N���[�h�t�@�C��
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
// �ÓI�����o�ϐ��錾
//========================

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCamera::CCamera()
{
    // �r���[�}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&m_mtxView);

    // �v���W�F�N�V�����}�g���b�N�X�̏�����
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
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// �Q�[���J�ڎ��̃J�����̃��Z�b�g
// Author : �㓡�T�V��
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
        m_state = STATE_BUTTLE;   // �o�g�����̃J�����ɂ���
        m_fTheta = 1.4f;
        break;
    case SETTING_CUSTOM:
        m_posRDest = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_pos = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_posV = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_posR = pos + CAMERA_LOCK_ON_POS_ADJUST;
        m_fDistance = CAMERA_LOCK_ON_OFFSET;
        m_state = STATE_OUT_GAME;   // �A�E�g�Q�[���̃J�����ɂ���
        m_fTheta = 1.4f;
        break;
    }

    m_posVDest = DEFAULT_VECTOR;
    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_fPhi = fRot;
    m_nCntState = 0;
    m_shakePhase = SHAKE_PHASE_NONE;

    // �ʒu���X�V(���ʍ��W�̌���)
    m_posV.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
    m_posV.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
    m_posV.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;
}

//=============================================================================
// �J�����̐U������
// Author : �㓡�T�V��
//=============================================================================
void CCamera::Shake(void)
{
    // �J�E���^�����Z
    m_nCntState++;

    // �ʒu��ς���
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

    // ���̐U���i�K��
    if (m_nCntState >= CHANGE_FRAME)
    {
        m_nCntState = 0;
        m_shakePhase++;
        if (m_shakePhase >= SHAKE_PHASE_MAX)
        {
            m_shakePhase = SHAKE_PHASE_NONE;
        }
    }

    // �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
    m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + cameraPos.x;
    m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + cameraPos.y;
    m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + cameraPos.z;

    // �J�����̈ʒu�ƒ����_���X�V
    m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
    m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;
}

//=============================================================================
// �J�����U���ݒ�
// Author : �㓡�T�V��
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
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CCamera::Init(void)
{
    // �f�o�C�X���擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���ɁA�X�e�[�W1�J�n���̃v���C���[�̈ʒu�ɍ��킹�Ă���
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

    // �r���[�}�g���b�N�X�̍쐬
    D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    // �v���W�F�N�V�����}�g���b�N�X�̍쐬
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, CAMERA_VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CCamera::Update(void)
{
    //  �J�����ʒu�C������
    bool bResetCamera = false;
    switch (m_state)
    {
    case STATE_OUT_GAME:

        // �J�����Ǝ��g�̋���
        m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // �J�����̈ʒu�ƒ����_���X�V
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;

    case STATE_BUTTLE:

        // �J�����Ǝ��g�̋���
        m_fDistance = CAMERA_LOCK_ON_OFFSET;

        if (m_shakePhase == SHAKE_PHASE_NONE)
        {
            // �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
            m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
            m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
            m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

            // �J�����̈ʒu�ƒ����_���X�V
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
        // �J�E���^���Z
        m_nCntState++;

        if (m_nCntState < CAMERA_FINISH_NEXT_PLAYER_FRAME)
        {
            // �ŏ��͔s�҂Ƀ��b�N�I��
            CPlayer *pLoser = NULL;
            for (int nCntPlayer = 0; nCntPlayer < CGame::GetNumAllPlayer(); nCntPlayer++)
            {
                CPlayer *pPlayer = CGame::GetPlayer(nCntPlayer);
                if (pPlayer)
                {
                    // �s�҂��L��
                    if (pPlayer->GetIdxControlAndColor() == CGame::GetPlayerRankInThisRound(CPlayer::RANK_2))
                    {
                        pLoser = pPlayer;
                    }
                }
            }

            // �s�҂����݂���Ȃ�
            if (pLoser)
            {
                m_fPhi += CAMERA_FINISH_ROT_SPEED;
                m_pos = pLoser->GetPos() + D3DXVECTOR3(0.0f, pLoser->GetCollisionSizeDeffence().y / 2, 0.0f);
                m_posRDest = m_pos;
            }
        }
        else
        {
            // ���ɏ��҂փ��b�N�I��
            CPlayer *pWinner = NULL;
            for (int nCntPlayer = 0; nCntPlayer < CGame::GetNumAllPlayer(); nCntPlayer++)
            {
                CPlayer *pPlayer = CGame::GetPlayer(nCntPlayer);
                if (pPlayer)
                {
                    // ���҂��L��
                    if (pPlayer->GetIdxControlAndColor() == CGame::GetPlayerRankInThisRound(CPlayer::RANK_1))
                    {
                        pWinner = pPlayer;
                    }
                }
            }

            // �s�҂����݂���Ȃ�
            if (pWinner)
            {
                m_fPhi -= CAMERA_FINISH_ROT_SPEED;
                m_pos = pWinner->GetPos() + D3DXVECTOR3(0.0f, pWinner->GetCollisionSizeDeffence().y / 2, 0.0f);
                m_posRDest = m_pos;
            }
        }

        //// �J�����Ǝ��g�̋���
        //m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // �J�����̈ʒu�ƒ����_���X�V
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;
    }

    case STATE_SP:
    {
        // �J�E���^���Z���A�K�E���o���Ԃ��Ǘ�
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

        // �K�E�Z�g�p�҂Ƀ��b�N�I��
        CPlayer *pUser = CGame::GetSpPlayer();
        if (pUser)
        {
            m_pos = pUser->GetPartsPos(CPlayer::PARTS_HEAD);
            m_posRDest = m_pos;
        }

        //// �J�����Ǝ��g�̋���
        //m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // �J�����̈ʒu�ƒ����_���X�V
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;
    }

    }

    // �r���[�}�g���b�N�X�̍쐬
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    // �v���W�F�N�V�����}�g���b�N�X�̍쐬
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, CAMERA_VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

    // ���Z�b�g����t���O����������A�J���������Z�b�g
    if (bResetCamera)
    {
        CCamera::ResetCamera(DEFAULT_VECTOR, CAMERA_DEFAULT_ROT, CCamera::SETTING_GAME);
    }
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CCamera * CCamera::Create(void)
{
    CCamera *pCamera = NULL;
    pCamera = new CCamera;
    pCamera->Init();
    return pCamera;
}