//==========================================================================================
//
// �L�����N�^�[�̏��� (character.h)
// Author : �㓡�T�V��
//
//==========================================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene.h"
#include "model.h"

//================================================
// �}�N����`
//================================================
#define CHARCTER_PARTS_MAX 25       // �L�����N�^�[�̃p�[�c�̍ő吔
#define CHARCTER_COLLISION_MAX 18   // �L�����N�^�[�̓����蔻��̍ő吔

//================================================
// �O���錾 
//================================================
class CModel;
class CAnimation;
class CShadow;

//================================================
// �N���X�錾
//================================================

// �L�����N�^�[�N���X
class CCharacter : public CScene
{
public:
    CCharacter();
    CCharacter(OBJTYPE objType);
    ~CCharacter();

    // �푰(animation.h�ɂ��ǉ����K�v)
    typedef enum
    {
        TRIBE_NONE = 0,
        TRIBE_PLAYER,			// �v���C���[
        TRIBE_MAX			    // ��ނ̍ő吔
    }TRIBE;

    // �����蔻��̏��
    typedef struct
    {
        D3DXVECTOR3 pos;        // �ʒu
        D3DXVECTOR3 size;       // ��`�̃T�C�Y
    }COLLISION;

    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void LoadModelData(char* cFilePass);                          // �e�L�X�g�t�@�C�����璼�ړǂݍ��ގ��Ɏg���֐�
    void LoadModelData(int nModelPosDefUp, int nModelPosDefDown); // ���f���̏����ʒu�f�[�^����ǂݍ��ނƂ��Ɏg���֐�

    // �Z�b�^�[
    void SetPartPos(int index, D3DXVECTOR3 pos) { m_apModel[index]->SetPos(pos); }
    void SetPartRot(int index, D3DXVECTOR3 rot) { m_apModel[index]->SetRot(rot); }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetTribe(TRIBE tribe) { m_tribe = tribe; }
    void SetPartNum(int nPartNum) { m_nPartsNum = nPartNum; }                       // �p�[�c�̍ő吔��ݒ�
    void SetPosOld(const D3DXVECTOR3 posOld) { m_posOld = posOld; }
    void SetAnimFilePass(char* cFilePass) { m_cAnimFilePass = cFilePass; }

    void BindParts(int nPartsIndex, int nPartsType) { m_aPartsType[nPartsIndex] = nPartsType; }     // �p�[�c�̎�ނ�ݒ�

    // �Q�b�^�[
    D3DXVECTOR3 GetDefaultPos(int index) { return m_posDefault[index]; }
    D3DXVECTOR3 GetPartsRot(int index) { return m_apModel[index]->GetRot(); }
    CModel * GetParts(int index) { return m_apModel[index]; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    TRIBE GetTribe(void) { return m_tribe; }
    CAnimation * GetAnimation(void) { return m_pAnimation; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

    // �����蔻��Ɋւ���Q�b�^�[��Z�b�^�[
    void SetActiveCollisionSize(const D3DXVECTOR3 collisionSize) { m_activeCollisionSize = collisionSize; }
    D3DXVECTOR3 GetActiveCollisionSize(void) { return m_activeCollisionSize; }
    COLLISION GetCollision(int nCnt) { return m_aCollision[nCnt]; }                         // �e�p�[�c�́A�Փ˂Ɋւ������Ԃ�
    void SetCollisionPos(int nCnt, D3DXVECTOR3 pos) { m_aCollision[nCnt].pos = pos; }       // �e�p�[�c�́A�Փ˂̈ʒu��ݒ�
    void SetCollisionSize(int nCnt, D3DXVECTOR3 size) { m_aCollision[nCnt].size = size; }   // �e�p�[�c�́A�Փ˂̃T�C�Y��ݒ�
    int GetCntCollision(void) { return m_nCntCollision; }                                   // �����蔻��̐���Ԃ�
    void SetCntCollision(int nCnt) { m_nCntCollision = nCnt; }                              // �����蔻��̐���ݒ�

    // �A�j���[�V�������g�p���Ȃ��ꍇ�̃Z�b�^�[
    void SetUnableAnimation(void) { m_bUseAnimation = false; }

private:
    int                     m_aPartsType[CHARCTER_PARTS_MAX];           // �p�[�c�^�C�v
    D3DXVECTOR3				m_posDefault[CHARCTER_PARTS_MAX];           // �����ʒu
    int						m_nIndexParent[CHARCTER_PARTS_MAX];         // �e�ݒ�

    D3DXVECTOR3		m_pos;                                              // �S�Ă̐e�̈ʒu
    D3DXVECTOR3		m_rot;                                              // �S�Ă̐e�̌���
    TRIBE           m_tribe;                                            // �푰
    int				m_nPartsNum;                                        // �p�[�c��

    CModel			*m_apModel[CHARCTER_PARTS_MAX];                     // ���f���ւ̃|�C���^
    CAnimation		*m_pAnimation;                                      // �A�j���[�V�����ւ̃|�C���^
    CShadow         *m_pShadow;                                         // �e�ւ̃|�C���^
    CShadow         *m_pFrame;                                          // �ӂ��ǂ�ւ̃|�C���^

    D3DXVECTOR3 m_activeCollisionSize;                                  // �\���I�ȓ����蔻��̑傫��(�ǂɑ΂��Ďg�����肷��)
    D3DXVECTOR3		m_posOld;                                           // 1F�O�̈ʒu

    char* m_cAnimFilePass;                                              // �A�j���[�V�����t�@�C���̃p�X

    COLLISION m_aCollision[CHARCTER_COLLISION_MAX];                     // �����蔻��̏��
    int m_nCntCollision;                                                // �����蔻��̐�

    bool m_bUseAnimation;												// �A�j���[�V���������邩�ǂ���
};

#endif