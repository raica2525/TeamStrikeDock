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

// �������̃J�����̈ʒu������
#define CAMERA_FINISH_NEXT_PLAYER_FRAME 90
#define CAMERA_FINISH_ROT_SPEED D3DXToRadian(3.0f)
//#define CAMERA_FINISH_OFFSET -600.0f
//#define CAMERA_FINISH_HEIGHT D3DXToRadian(75.0f)

// �J�����̈ړ����x
#define CAMERA_MOVE_RATE 0.1f                                     // �J�����̕�Ԓl

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
        STATE_OUT_GAME,         // �A�E�g�Q�[��
        STATE_BUTTLE,           // �o�g����
        STATE_FINISH_EACH,      // �������̊e������
        STATE_SP,               // �K�E�Z��
    }STATE;

    // �J�����̎g�p���
    typedef enum
    {
        SETTING_GAME = 0,         // �Q�[����
        SETTING_CUSTOM,           // �J�X�^�}�C�Y��
    }SETTING;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    CCamera *Create(void);
    void ResetCamera(D3DXVECTOR3 pos, float fRot, SETTING setting);     // �J�����̃��Z�b�g

    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    void SetState(STATE state) { m_nCntState = 0; m_state = state; }    // ��Ԃ��Z�b�g����Ƃ��́A�J�E���^�������Ƀ��Z�b�g
    void SetLookAt(D3DXVECTOR3 pos) { m_posRDest = pos; }

    /*========================================================
    // �Q�b�^�[
    //======================================================*/
    STATE GetState(void) { return m_state; }
    D3DXVECTOR3 GetPos(void) { return m_posV; }
    D3DXMATRIX GetProjectionMtx(void) { return m_mtxProjection; }
    D3DXMATRIX GetViewMtx(void) { return m_mtxView; }

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
};

#endif