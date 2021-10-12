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

// �u���b�N���ɓ��������ہA�|���̃G�t�F�N�g��u�����߂̉�]�ʁi�u���b�N�̒����猩�Ă���̂Œ��Ӂj
#define LAY_ROT_TOP D3DXVECTOR3(D3DXToRadian(-90.0f), 0.0f, 0.0f)    // �V��
#define LAY_ROT_LEFT D3DXVECTOR3(0.0f, D3DXToRadian(270.0f), 0.0f)   // ����
#define LAY_ROT_BOT D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f)     // �n��
#define LAY_ROT_RIGHT D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f)   // �E��
#define LAY_ROT_BACK D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f)
#define LAY_ROT_FRONT D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(0.0001f))

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
		TYPE_CIRCLEABSORPTION = 0,  // �~�̋z��
		TYPE_CIRCLE,				// �~
        TYPE_RUNFUGITIVEDUST,		// �����n�߂̍���
		T_3,
		T_4,
		T_5,
		T_6,
		T_7,
		T_8,
		T_9,
		T_10,
		T_11,
		T_12,
		T_13,
        TYPE_MAX
    }TYPE;  // ��������G�t�F�N�g�̎��

    typedef enum
    {
        PLANE_XY = 0,   // XY����
        PLANE_XZ,       // XZ����
    }PLANE;  // ���ʂ̏��

    typedef struct
    {
        int nTexType;					// �g���e�N�X�`���̎��
        int nNumOnce;					// ����������
        int plane;						// ����

        bool bOnCircleEdge;				// �~�̉�������o���邩�ǂ���
        bool bEmitVectorInside;			// ���o����������ɂ��邩�ǂ���
		int nScatterAngle;				// ��U�p�x�i0�`314�j
        int nScatterWidth;				// ��U���i0�`314�j

        int nSpeedMax;					// �ő呬�x
        int nSpeedMin;					// �ŏ����x
        int nAuxiliaryAxisSpeedMax;		// �⏕���̍ő呬�x�i2D��3D�ŁA���̊����o�����߂̂��́j
        int nAuxiliaryAxisSpeedMin;		// �⏕���̍ŏ����x�i2D��3D�ŁA���̊����o�����߂̂��́j
		float fEmitRadius;				// ���a�̑傫��

        float fAcceleration;			// �����x�i1.0��j
        float fGravity;					// �d��
		float fGravityLimit;			// �d�͐���

        int nLayRot;					// �|���S�����̂��̂����ɂ��邩�ǂ����i1,2�Ȃ�r���{�[�h�����j
        bool bRotEqualMoveAngle;		// ��]���ړ��̌����ɍ��킹�邩
        int nRotSpeedMax;				// �ő��]���x
        int nRotSpeedMin;				// �ŏ���]���x

        D3DXCOLOR col;					// �F
        D3DXCOLOR colChangeRate;		// �F�ω�����
        float fFadeOutRate;				// ���l�������Ă��������i�F�ω������ł��\�����A�����I�ɗp�ӂ����j
		int nCntFadeOutLock;			// ���l�̕ω������b�N����J�E���^

        int nSizeMax;					// �ő�傫��
        int nSizeMin;					// �ŏ��傫��
        D3DXVECTOR3 sizeChangeRate;		// �g��/�k������
		int nCntSizeChangeLock;			// �傫�������b�N����J�E���^
		bool bSizeChangeStartOrFinish;	// �傫����ς���̂��n�܂邩�A�I���̂�

        bool bUseAdditiveSynthesis;		// ���Z�����ɂ��邩�ǂ���
        bool bUseZBuffer;				// Z�o�b�t�@���������ǂ���
        int nAlphaTestBorder;			// ���e�X�g�̃{�[�_�[
    }CreateInfo;    // �������

    HRESULT Init(void);	// ����������
    void Uninit(void);	// �I������

	HRESULT UploadEffectInfo(const int nNum);

    CreateInfo* GetCreateInfo(const int nNum);
	void SetCreateInfo(const int nNum);
private:
    CreateInfo m_aCreateInfo[TYPE_MAX];    // �������
};

#endif