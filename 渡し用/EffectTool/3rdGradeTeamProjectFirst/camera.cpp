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

    m_StartMousePos = DEFAULT_VECTOR;
    m_AmountMovement = DEFAULT_VECTOR;
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

    // �}�E�X�̏���������
    ResetMousePos();
    POINT Point;
    GetCursorPos(&Point);
    m_StartMousePos = D3DXVECTOR3(Point.x, Point.y, 0.0f);
    m_AmountMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �}�E�X�̈ʒu���Z�b�g����
// Author : �㓡�T�V��
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

    // �}�E�X�̏���������
    ResetMousePos();
    POINT Point;
    GetCursorPos(&Point);
    m_StartMousePos = D3DXVECTOR3(Point.x, Point.y, 0.0f);
    m_AmountMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    switch (m_state)
    {
    case STATE_NONE:

        break;

    case STATE_HORIZON_MOVE:

        //// ���@�Ƀ��b�N�I��
        //m_pos = CGame::GetPlayer()->GetPos() + CAMERA_LOCK_ON_POS_ADJUST;
        //m_posRDest = m_pos;

        // �J�����Ǝ��g�̋���
        m_fDistance = CAMERA_LOCK_ON_OFFSET;

        // ���_�ړ�
        MoveViewpoint();

        // �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
        m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
        m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
        m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

        // �J�����̈ʒu�ƒ����_���X�V
        m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
        m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

        break;

    //case STATE_IN_FADE:

    //    //// ���@�Ƀ��b�N�I��
    //    //m_pos = CGame::GetPlayer()->GetPos() + CAMERA_LOCK_ON_POS_ADJUST;
    //    //m_posRDest = m_pos;

    //    //// �J�����Ǝ��g�̋���
    //    //m_fDistance = CAMERA_LOCK_ON_OFFSET;

    //    //// �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
    //    //m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
    //    //m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
    //    //m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

    //    //// �J�����̈ʒu�ƒ����_���X�V
    //    //m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
    //    //m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

    //    break;

    //case STATE_FAILD:

    //    //// ���@�Ƀ��b�N�I��
    //    //m_pos = CGame::GetPlayer()->GetPos() + CAMERA_FAILD_POS_ADJUST;
    //    //m_posRDest = m_pos;

    //    //// �J�����Ǝ��g�̋���
    //    //m_fDistance = CAMERA_FAILD_OFFSET;

    //    //// ����]
    //    //m_fPhi += CAMERA_FAILD_ROT_SPEED;

    //    //// �c�̈ʒu
    //    //m_fTheta = CAMERA_FAILD_HEIGHT;

    //    //// �ʒu�̖ړI�n���X�V(���ʍ��W�̌���)
    //    //m_posVDest.x = m_fDistance * (sin(m_fTheta) * cos(m_fPhi)) + m_pos.x;
    //    //m_posVDest.y = (m_fDistance / 2.0f) * cos(m_fTheta) + m_pos.y;
    //    //m_posVDest.z = m_fDistance * (sin(m_fTheta) * sin(m_fPhi)) + m_pos.z;

    //    //// �J�����̈ʒu�ƒ����_���X�V
    //    //m_posR += (m_posRDest - m_posR) * CAMERA_MOVE_RATE;
    //    //m_posV += (m_posVDest - m_posV) * CAMERA_MOVE_RATE;

    //    break;
    }

    // �r���[�}�g���b�N�X�̍쐬
    D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    // �v���W�F�N�V�����}�g���b�N�X�̍쐬
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, CAMERA_VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_VIEW_RANGE);
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
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

//=============================================================================
// ���̊p�x�擾����
// Author : �㓡�T�V��
//=============================================================================
float CCamera::GetAngle(void)
{
    // �Ԃ��p�x
    float fReturnAngle = 0.0f;

    // �p�x�����߂�(�J�����̈ʒu����A�����_���������̊p�x)
    fReturnAngle = atan2f((m_posR.x - m_posV.x), (m_posR.z - m_posV.z));

    return fReturnAngle;
}

//=============================================================================
// �v���C���[�̊p�x�擾����
// Author : �㓡�T�V��
//=============================================================================
float CCamera::GetPlayerRot(void)
{
    // �Ԃ��p�x
    float fReturnAngle = 0.0f;

    // �p�x�����߂�(�J�����̈ʒu����A�����_���������̊p�x)
    fReturnAngle = atan2f((m_posR.x - m_posV.x)*-1, (m_posR.z - m_posV.z)*-1);

    return fReturnAngle;
}

//=============================================================================
// �e�̔��ˊp�x�擾����
// Author : �㓡�T�V��
//=============================================================================
float CCamera::GetShotAngle(bool bUseSpread)
{
    // �Ԃ��p�x
    float fReturnAngle = 0.0f;

    // �p�x�����߂�(�J�����̈ʒu����A�����_���������̊p�x)
    fReturnAngle = atan2f((m_posR.x - m_posV.x), (m_posR.z - m_posV.z));

    // �g�U������Ȃ�
    if (bUseSpread == true)
    {
        // �����_���Ȋp�x�𑫂��āA�g�U������
        float fRandAngle = float(rand() % RAND_HORIZON_DIFFUSION_VALUE) / 100.0f - float(rand() % RAND_HORIZON_DIFFUSION_VALUE) / 100.0f;
        fReturnAngle += D3DXToRadian(fRandAngle);
    }

    return fReturnAngle;
}

//=============================================================================
// �c�̊p�x�擾����(�e�����Ƃ��ɕK�v)
// Author : �㓡�T�V��
//=============================================================================
float CCamera::GetShotTheta(bool bUseSpread)
{
    // �Ԃ��p�x
    float fReturnTheta = m_fTheta;

    // �㉺�͈͒���
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

    // �g�U������Ȃ�
    if (bUseSpread == true)
    {
        // �����_���Ȋp�x�𑫂��āA�g�U������
        float fRandAngle = float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f - float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f;
        fReturnTheta += D3DXToRadian(fRandAngle);
    }

    return fReturnTheta;
}

//=============================================================================
// �󒆂ŏc�̊p�x�擾����(�e�����Ƃ��ɕK�v)
// Author : �㓡�T�V��
//=============================================================================
float CCamera::GetShotThetaInAir(bool bUseSpread)
{
    // �Ԃ��p�x
    float fReturnTheta = m_fTheta;

    // �㉺�͈͒���
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

    // �g�U������Ȃ�
    if (bUseSpread == true)
    {
        // �����_���Ȋp�x�𑫂��āA�g�U������
        float fRandAngle = float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f - float(rand() % RAND_VERTICAL_DIFFUSION_VALUE) / 100.0f;
        fReturnTheta += D3DXToRadian(fRandAngle);
    }

    return fReturnTheta;
}

//=============================================================================
// �J�����̎��_�ړ�����
// Author : �㓡�T�V��
//=============================================================================
void CCamera::MoveViewpoint(void)
{
    //// �}�E�X�̍��W�擾
    //POINT Point;
    //GetCursorPos(&Point);
    //D3DXVECTOR3 MousePos = D3DXVECTOR3(Point.x, Point.y, 0);

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // �E�X�e�B�b�N���|��Ă���Ȃ�
    if (Controller.lZ != 0 || Controller.lRz != 0)
    {
        // �E�ړ�
        if (Controller.lZ > 0)
        {
            m_fPhi -= CAMERA_ROT_RIGHT_LEFT_SPEED;
        }
        // ���ړ�
        else if (Controller.lZ < 0)
        {
            m_fPhi += CAMERA_ROT_RIGHT_LEFT_SPEED;
        }

        // ��ړ�
        if (Controller.lRz > 0)
        {
            m_fTheta += CAMERA_ROT_UP_DOWN_SPEED;
        }
        // ���ړ�
        else if (Controller.lRz < 0)
        {
            m_fTheta -= CAMERA_ROT_UP_DOWN_SPEED;
        }
    }
    //// �E�X�e�B�b�N���|��Ă��Ȃ��Ȃ�A�}�E�X���g����
    //else
    //{
    //    // �}�E�X�̈ړ��ʂ��v�Z
    //    m_AmountMovement.y = (m_StartMousePos.y - MousePos.y) * CAMERA_SENSITIVITY;
    //    m_AmountMovement.x = (MousePos.x - m_StartMousePos.x) * CAMERA_SENSITIVITY;

    //    m_fPhi -= D3DXToRadian(m_AmountMovement.x);
    //    m_fTheta -= D3DXToRadian(m_AmountMovement.y);
    //}

    // ���E�͈͒���
    if (m_fPhi >= D3DXToRadian(360.0f))
    {
        m_fPhi = 0.0f;
    }
    else if (m_fPhi <= D3DXToRadian(-360.0f))
    {
        m_fPhi = 0.0f;
    }

    // �㉺�͈͒���
    if (m_fTheta >= CAMERA_ROT_DOWN_LIMIT)
    {
        m_fTheta = CAMERA_ROT_DOWN_LIMIT;
    }
    else if (m_fTheta <= CAMERA_ROT_UP_LIMIT)
    {
        m_fTheta = CAMERA_ROT_UP_LIMIT;
    }

    //// �}�E�X�̈ʒu�����Z�b�g
    //ResetMousePos();
}
