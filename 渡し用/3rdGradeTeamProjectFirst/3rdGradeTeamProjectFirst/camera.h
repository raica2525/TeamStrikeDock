//========================================================================================================================
//
// �J�������� [camera.h]
// Author : �㓡�T�V��
//
//========================================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//================================================
// �}�N����`
//================================================

#define CAMERA_DEFAULT_ROT D3DXToRadian(90.0f)

// ���b�N�I�����̃J�����̈ʒu�̔�����
#define CAMERA_LOCK_ON_POS_ADJUST D3DXVECTOR3(0.0f, 400.0f, -1000.0f)
#define CAMERA_LOCK_ON_OFFSET -750.0f

// ���s���̃J�����̈ʒu������
#define CAMERA_FAILD_POS_ADJUST D3DXVECTOR3(0.0f, 200.0f, 0.0f)
#define CAMERA_FAILD_OFFSET -600.0f
#define CAMERA_FAILD_HEIGHT D3DXToRadian(75.0f)
#define CAMERA_FAILD_ROT_SPEED D3DXToRadian(1.0f)

// �J�����̈ړ����x
#define CAMERA_MOVE_RATE 0.1f                                     // �J�����̕�Ԓl

// �J�����̉�]���x                                               
#define CAMERA_ROT_RIGHT_LEFT_SPEED D3DXToRadian(2.0f)            // �R���g���[���̍��E
#define CAMERA_ROT_UP_DOWN_SPEED D3DXToRadian(1.5f)               // �R���g���[���̏㉺
#define CAMERA_SENSITIVITY (0.03f)                                // �}�E�X���x

// �J�����̉�]�͈͐���                                           
#define CAMERA_ROT_DOWN_LIMIT D3DXToRadian(160.0f)                // �J�����̉���
#define CAMERA_ROT_UP_LIMIT D3DXToRadian(32.5f)                   // �J�����̏��

// �J�����̃V�[�^�ŕԂ��l�̐���
#define CAMERA_THETA_DOWN_LIMIT D3DXToRadian(155.0f)              // ����������
#define CAMERA_THETA_DOWN_LIMIT_2 D3DXToRadian(132.5f)            // �ڂ̑O����≺��������132.5
#define CAMERA_THETA_ADJUST_IN_AIR D3DXToRadian(115.0f)           // �󒆂ŉ������Ƃ��̕␳
#define CAMERA_THETA_DOWN_LIMIT_3 D3DXToRadian(97.5f)             // �ڂ̑O��_������
#define CAMERA_THETA_DOWN_ACCURATE_LIMIT D3DXToRadian(92.5f)      // 92.5�����m�ɑ_������

#define CAMERA_THETA_DOWN_LIMIT_VALUE D3DXToRadian(179.0f)        // �������Ƃ��̒l

// �����_���Ȋg�U�̒l(��100���l��)
#define RAND_HORIZON_DIFFUSION_VALUE 250                          // ����
#define RAND_VERTICAL_DIFFUSION_VALUE 250                         // ����

// ����
#define CAMERA_VIEW_RANGE 25000.0f

// ����p
#define CAMERA_VIEW_ANGLE D3DXToRadian(45.0f)

//================================================
// �J�����N���X�錾
//================================================
class CCamera
{
public:
    CCamera();
    ~CCamera();

    // �J�����̏��
    typedef enum
    {
        STATE_NONE = 0,         // �Ȃ�
        STATE_HORIZON_MOVE,     // ���ړ�
        STATE_FINISH_ROTATION,  // �������̉�]
        STATE_FINISH_EACH,      // �������̊e������
    }STATE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    CCamera *Create(void);
    void SetLookAt(D3DXVECTOR3 pos) { m_posRDest = pos; }
    void ResetGameCamera(D3DXVECTOR3 pos, float fRot);                  // �Q�[���J�ڎ��̃J�����̃��Z�b�g
    STATE GetState(void) { return m_state; }
    void SetState(STATE state) { m_nCntState = 0; m_state = state; }    // ��Ԃ��Z�b�g����Ƃ��́A�J�E���^�������Ƀ��Z�b�g
    float GetAngle(void);                                               // �J�����̉��̌������擾
    float GetPlayerRot(void);                                           // �v���C���[�̌����������擾
    float GetShotAngle(bool bUseSpread);                                // �ˌ��̉��̊p�x�擾
    float GetShotTheta(bool bUseSpread);                                // �n��Ŏˌ��̏c�̊p�x�擾
    float GetShotThetaInAir(bool bUseSpread);                           // �󒆂Ŏˌ��̏c�̊p�x�擾
    D3DXVECTOR3 GetPos(void) { return m_posV; }

    void ResetMousePos(void);
    void MoveViewpoint(void);

private:
    D3DXVECTOR3	m_posRDest;         // �J�����̒����_�i�ړI�l�j
    D3DXVECTOR3	m_posVDest;         // �J�����̍��W�i�ړI�l�j
    STATE m_state;                  // �J�����̏��
    D3DXVECTOR3	m_posV;             // �J�����̍��W
    D3DXVECTOR3	m_pos;              // �J�����̍��W
    D3DXVECTOR3	m_posR;             // �����_
    D3DXVECTOR3	m_vecU;             // ������̃x�N�g��
    D3DXMATRIX	m_mtxProjection;    // �v���W�F�N�V�����}�g���N�X
    D3DXMATRIX	m_mtxView;          // �r���[�}�g���N�X
    float		m_fDistance;        // ����
    float		m_fPhi;             // �t�@�C
    float		m_fTheta;           // �V�[�^
    int m_nCntState;                // ��ԃJ�E���^

    D3DXVECTOR3 m_AmountMovement;   // �}�E�X�̈ړ���
    D3DXVECTOR3 m_StartMousePos;    // �}�E�X�̊J�n�ʒu
};

#endif