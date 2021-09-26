//====================================================================
//
// �G�t�F�N�g�f�[�^�Ǘ� (effectData.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _EFFECT_DATA_H_
#define _EFFECT_DATA_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define EFFECT_PI 314                   // �~�̕\���̍ۂɎg���l�i������G�t�F�N�g�̕��������_�Ŋ���j
#define EFFECT_FLOATING_POINT 100.0f    // �G�t�F�N�g�̕��������_

// �u���b�N���ɓ��������ہA�|���̃G�t�F�N�g��u�����߂̉�]��
#define LAY_ROT_TOP D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f)
#define LAY_ROT_LEFT D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f)
#define LAY_ROT_BACK D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f)
#define LAY_ROT_RIGHT D3DXVECTOR3(0.0f, D3DXToRadian(270.0f), 0.0f)
#define LAY_ROT_FRONT D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(0.0001f))

#define MAX_EFFECT_TYPE 64

//================================================
// �N���X�錾
//================================================

// �G�t�F�N�g�f�[�^�̊Ǘ��N���X
class CEffectData
{
public:
    CEffectData();
    ~CEffectData();

    typedef enum
    {
        TYPE_HEAL = 0,              // ��
        TYPE_BALL_AURA,             // �{�[���I�[��
        TYPE_BALL_APPEAR,           // �{�[���o��
        TYPE_NEGA,                  // ���]����
        TYPE_WALK_SMOKE_RIGHT,      // �����n�߂̍����E
        TYPE_JUMP_SMOKE_RIGHT,      // �W�����v�̍����E
        TYPE_GROUND_SMOKE,          // ���n���̍���
        TYPE_SECOND_JUMP_AIR_RING,  // ��i�W�����v�̋�C�̗�
        TYPE_SECOND_JUMP_SMOKE,     // ��i�W�����v�̍���
        TYPE_WALK_SMOKE_LEFT,       // �����n�߂̍�����
        TYPE_JUMP_SMOKE_LEFT,       // �W�����v�̍�����
        TYPE_SHOOT_CIRCLE_1P,       // �V���[�g���̉~1P
        TYPE_SHOOT_CIRCLE_2P,       // �V���[�g���̉~2P
        TYPE_SHOOT_CIRCLE_3P,       // �V���[�g���̉~3P
        TYPE_SHOOT_CIRCLE_4P,       // �V���[�g���̉~4P
        TYPE_SHOOT_CIRCLE_1P_FAST,  // �V���[�g���̉~1P��
        TYPE_SHOOT_CIRCLE_2P_FAST,  // �V���[�g���̉~2P��
        TYPE_SHOOT_CIRCLE_3P_FAST,  // �V���[�g���̉~3P��
        TYPE_SHOOT_CIRCLE_4P_FAST,  // �V���[�g���̉~4P��
        TYPE_SHOOT_ABSORB,          // �z��
        TYPE_HIT_BALL,              // �{�[��������������
        TYPE_LUCKY_GUARD_FIRST,     // ���b�L�[�K�[�h���ɍŏ��ɏo��G�t�F�N�g
        TYPE_LUCKY_GUARD_SECOND,    // ���b�L�[�K�[�h����2�Ԗڂɏo��G�t�F�N�g
        TYPE_TITLE_FIRE_RED,        // �^�C�g���̐Ԃ���
        TYPE_TITLE_FIRE_BLUE,       // �^�C�g���̐���
        TYPE_KIRA,                  // �J�[�\���N���b�N���̃L���L��
    }TYPE;  // ��������G�t�F�N�g�̎��

    typedef enum
    {
        PLANE_XY = 0,   // XY����
        PLANE_XZ,       // XZ����
    }PLANE;  // ���ʂ̏��

    typedef struct
    {
        int nTexType;                 // �g���e�N�X�`���̎��
        int nNumOnce;                 // ����������
        int plane;                    // ����

        bool bOnCircleEdge;           // �~�̉�������o���邩�ǂ���
        bool bEmitVectorInside;       // ���o����������ɂ��邩�ǂ���
        int nScatterAngle;            // ��U�p�x�i0�`314�j
        int nScatterWidth;            // ��U���i0�`314�j
        int nSpeedMax;                // �ő呬�x
        int nSpeedMin;                // �ŏ����x
        int nAuxiliaryAxisSpeedMax;   // �⏕���̍ő呬�x�i2D��3D�ŁA���̊����o�����߂̂��́j
        int nAuxiliaryAxisSpeedMin;   // �⏕���̍ŏ����x�i2D��3D�ŁA���̊����o�����߂̂��́j

        float fAcceleration;          // �����x�i1.0��j
        float fGravity;               // �d��
        float fGravityLimit;          // �d�͐���

        bool bLayRot;                 // �|���S�����̂��̂����ɂ��邩�ǂ����itrue�Ȃ�r���{�[�h�����j
        bool bRotEqualMoveAngle;      // ��]���ړ��̌����ɍ��킹�邩
        int nRotSpeedMax;             // �ő��]���x
        int nRotSpeedMin;             // �ŏ���]���x

        D3DXCOLOR col;                // �F
        D3DXCOLOR colChangeRate;      // �F�ω�����
        float fFadeOutRate;           // ���l�������Ă��������i�F�ω������ł��\�����A�����I�ɗp�ӂ����j
        int nCntFadeOutLock;          // ���l�����b�N����J�E���^

        int nSizeMax;                 // �ő�傫��
        int nSizeMin;                 // �ŏ��傫��
        D3DXVECTOR3 sizeChangeRate;   // �g��/�k������
        int nCntSizeChangeLock;       // �傫�������b�N����J�E���^
        bool bSizeChangeStartOrFinish;// �傫����ς���̂��n�܂邩�A�I���̂�
        float fEmitRadius;            // ���o���a

        bool bUseAdditiveSynthesis;   // ���Z�����ɂ��邩�ǂ���
        bool bUseZBuffer;             // Z�o�b�t�@���������ǂ���
        int nAlphaTestBorder;         // ���e�X�g�̃{�[�_�[
    }CreateInfo;    // �������

    HRESULT Init(void);
    void Uninit(void);

    CreateInfo* GetCreateInfo(const int nNum);

private:
    CreateInfo m_aCreateInfo[MAX_EFFECT_TYPE];    // �������
};

#endif