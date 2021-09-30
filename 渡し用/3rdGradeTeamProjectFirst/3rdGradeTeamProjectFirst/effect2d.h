//=============================================================================
//
// �G�t�F�N�g2D���� [effect2d.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2d.h"
#include "effectData.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �G�t�F�N�g2D�N���X��`
//=============================================================================
class CEffect2D :public CScene2D
{
public:

    CEffect2D(CScene::OBJTYPE objtype);
    ~CEffect2D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static CEffect2D * Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius = 0.0f);
    static CEffect2D * Create(const int nType, D3DXVECTOR3 pos, float fScatterAngle = 0.0f, const bool bIn3D = false);
    void MoveCalculation(const int nType, const float fScatterAngle);

    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetColChangeRate(D3DXCOLOR colChangeRate) { m_colChangeRate = colChangeRate; }
    void SetUseUpdate(bool bUseUpdate) { m_bUseUpdate = bUseUpdate; }
    void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }

private:
    int m_nTexType;                 // �g���e�N�X�`���̎��

    D3DXVECTOR3 m_move;             // �ړ���
    float m_fAcceleration;          // �����x�i1.0��j
    float m_fGravity;               // �d��
    float m_fGravityLimit;          // �d�͐���

    float m_fRotAngle;              // ��]�p�x�ieffect3d���玝���Ă����ۂɒǉ��j
    float m_fRotSpeed;              // ��]���x

    D3DXCOLOR m_col;                // �F
    D3DXCOLOR m_colChangeRate;      // �F�ω�����
    float m_fFadeOutRate;           // ���l�������Ă��������i�F�ω������ł��\�����A�����I�ɗp�ӂ����j
    int m_nCntFadeOutLock;          // ���l�����b�N����J�E���^

    D3DXVECTOR3 m_sizeChangeRate;   // �g��/�k������
    int m_nCntSizeChangeLock;       // �傫�������b�N����J�E���^
    bool m_bSizeChangeStartOrFinish;// �傫���ύX���J�n���邩�I�����邩�i���b�N�̃J�E���^�ƕ��p�j

    bool m_bUseAdditiveSynthesis;   // ���Z�����ɂ��邩�ǂ���
    //bool m_bUseZBuffer;           // Z�o�b�t�@���������ǂ����ieffect3d���玝���Ă����ۂɍ폜�j

    //int m_nCntAnimTime;           // �A�j���[�V�����p�J�E���^�ieffect3d���玝���Ă����ۂɍ폜�Ascene2d�Ɍ��X���邽�߁j
    bool m_bOneRoundAnim;           // �A�j���[�V����������������ǂ���
    int m_nAnimParagraph;           // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
    int m_nAnimPattern;             // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
    int m_nAnimSpeed;               // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j
    bool m_bRepeat;                 // �e�N�X�`�����ێ��p�iupdate�Ŗ���擾����̂�h���j

    bool m_bUseUpdate;              // �X�V���������邩�ǂ����i���]�����̎��Ɏg�����肷��j
    CPlayer *m_pPlayer;             // �v���C���[
};
#endif
