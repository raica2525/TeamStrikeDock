//====================================================================
//
// �G�t�F�N�g�c�[���̏��� (effecttool.h)
// Author : �ɓ��@�z��
//
//====================================================================
#ifndef _EFFECTTOOL_H_
#define _EFFECTTOOL_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �}�N����`
//================================================
#define EFFECT_TYPE (20)

//================================================
// �G�t�F�N�g�c�[���N���X
//================================================
class CEffectTool
{
public:

	CEffectTool();
	~CEffectTool();

	typedef struct
	{
		float pos[3];					// ���W

		int nTexType;					// �g���e�N�X�`���̎��
		int nNumOnce;					// ����������
		int nPlane;						// ����

		bool bOnCircleEdge;				// �~����ɂ��邩�ǂ���
		bool bEmitVectorInside;			// ���o����������ɂ��邩�ǂ���
		int nScatterAngle;				// ��U�p�x�i0�`314�j
		int nScatterWidth;				// ��U���i0�`314�j
		float fEmitRadius;				// ���a�̑傫��

		int nSpeedMax;					// �ő呬�x
		int nSpeedMin;					// �ŏ����x
		int nAuxiliaryAxisSpeedMax;		// �ő�⏕���i2D��3D�ŁA���̊����o�����߂̂��́j
		int nAuxiliaryAxisSpeedMin;		// �ŏ��⏕���i2D��3D�ŁA���̊����o�����߂̂��́j

		float fAcceleration;			// �����x(1.0�ő�)
		float fGravity;					// �d��
		float fGravityLimit;			// �d�͐���

		int nLayRot;					// �|���S�����̂��̂����ɂ��邩�ǂ����i1,2�Ȃ�r���{�[�h�����j
		bool bRotEqualMoveAngle;		// ��]���ړ��̌����ɍ��킹�邩
		int nRotSpeedMax;				// �ő��]���x
		int nRotSpeedMin;				// �ŏ���]���x

		float col[4];					// �F
		float colChangeRate[4];			// �F�ω�����
		float fFadeOutRate;				// �t�F�[�h�A�E�g�̊���
		int nCntFadeOutLock;			// ���l�����b�N����J�E���^

		int nSizeMax;					// �ő�傫��
		int nSizeMin;					// �ŏ��傫��
		float fSizeChangeRate[3];		// �g��/�k������
		int nCntSizeChangeLock;			// �傫�������b�N����J�E���^
		bool bSizeChangeStartOrFinish;	// �傫����ς���̂��n�܂邩�A�I���̂�

		bool bUseAdditiveSynthesis;		// ���Z�������邩�ǂ���
		bool bUseZBuffer;				// Z�o�b�t�@���g����
		int nAlphaTestBorder;			// ���e�X�g�̃{�[�_�[
	}EffectInfo;    // �G�t�F�N�g�̏��

	HRESULT Init(HWND hWnd);	// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	EffectInfo InitEffectInfo(void);

	void SetEffectInfo(int nEffectType);
	EffectInfo* GetEffectInfo(int nEffectType);
private:
	EffectInfo m_EffectInfo;
	EffectInfo m_EffectInfoCopy;
	int m_nEffectType;
	float m_fCreateFlame;
	float m_fCountFlame;
	float m_fBarWidth;
	float m_fEmitRadius;
	bool m_bCreate[EFFECT_TYPE];
};

#endif