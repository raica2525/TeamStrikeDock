//=============================================================================
//
// �G�t�F�N�g3D���� [effect3d.h]
// Author : �ɓ��z��
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "billboard.h"
#include "effectData.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �G�t�F�N�g3D�N���X��`
//=============================================================================
class CEffect3D :public CBillboard
{
public:

    CEffect3D(CScene::OBJTYPE objtype);
    ~CEffect3D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static void Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius = 0.0f);
    static CEffect3D * Create(const int nType, D3DXVECTOR3 pos, const float fScatterAngle);
    void MoveCalculation(const int nType, const float fScatterAngle);
	void UpdateMove(D3DXVECTOR3 &pos);
	void UpdateSize(D3DXVECTOR3 &size);
	void UpdateColor(void);
	void UpdateAnim(void);
	void UpdateAngle(float &fAngle);
private:
    int m_nTexType;                 // �g���e�N�X�`���̎��

    D3DXVECTOR3 m_move;             // �ړ���
    float m_fAcceleration;          // �����x�i1.0��j
    float m_fGravity;               // �d��
	float m_fGravityLimit;			// �d�͐���

    float m_fRotSpeed;              // ��]���x

    D3DXCOLOR m_col;                // �F
    D3DXCOLOR m_colChangeRate;      // �F�ω�����
    float m_fFadeOutRate;           // ���l�������Ă��������i�F�ω������ł��\�����A�����I�ɗp�ӂ����j
	int m_nCntFadeOutLock;			// ���l�����b�N����J�E���^

    D3DXVECTOR3 m_sizeChangeRate;   // �g��/�k������
	int m_nCntSizeChangeLock;       // �傫�������b�N����J�E���^
	bool m_bSizeChangeStartOrFinish;// �傫���ύX���J�n���邩�I�����邩�i���b�N�̃J�E���^�ƕ��p�j

    bool m_bUseAdditiveSynthesis;   // ���Z�����ɂ��邩�ǂ���
    bool m_bUseZBuffer;             // Z�o�b�t�@���������ǂ���

    int m_nCntAnimTime;             // �A�j���[�V�����p�J�E���^
    bool m_bOneRoundAnim;           // �A�j���[�V����������������ǂ���
    int m_nAnimPattern;             // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
    int m_nAnimSpeed;               // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
	bool m_bRepeat;					// �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j

	int m_nLife;					// �����i�c�[����̂݁j
};
#endif
