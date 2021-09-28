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
#define CHARCTER_PARTS_MAX 18       // �L�����N�^�[�̃p�[�c�̍ő吔

//================================================
// �O���錾 
//================================================
class CModel;
class CAnimation;

//================================================
// �N���X�錾
//================================================

// �L�����N�^�[�N���X
class CCharacter : public CScene
{
public:
    CCharacter(OBJTYPE objType);
    ~CCharacter();

    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void DeathDraw(void);                                         // ���ꂽ���̕`��
    void LoadModelData(char* cFilePass);                          // �e�L�X�g�t�@�C�����璼�ړǂݍ��ގ��Ɏg���֐�
    void LoadModelData(int nModelPosDefUp, int nModelPosDefDown); // ���f���̏����ʒu�f�[�^����ǂݍ��ނƂ��Ɏg���֐�

    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    void SetPartPos(int index, D3DXVECTOR3 pos) { m_apModel[index]->SetPos(pos); }
    void SetPartRot(int index, D3DXVECTOR3 rot) { m_apModel[index]->SetRot(rot); }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetPartNum(int nPartNum) { m_nPartsNum = nPartNum; }                       // �p�[�c�̍ő吔��ݒ�
    void SetPosOld(const D3DXVECTOR3 posOld) { m_posOld = posOld; }
    void SetAnimFilePass(char* cFilePass) { m_cAnimFilePass = cFilePass; }

    void BindParts(int nPartsIndex, int nPartsType) { m_aPartsType[nPartsIndex] = nPartsType; }
    void SetDrawWeapon(bool bDrawWeapon) { m_bDrawWeapon = bDrawWeapon; }

    /*========================================================
    // �Q�b�^�[
    //======================================================*/
    D3DXVECTOR3 GetDefaultPos(int index) { return m_aPosDefault[index]; }
    D3DXVECTOR3 GetPartsPos(int index) { return D3DXVECTOR3(m_apModel[index]->GetWorldMtx()._41, m_apModel[index]->GetWorldMtx()._42, m_apModel[index]->GetWorldMtx()._43); }
    D3DXVECTOR3 GetPartsRot(int index) { return m_apModel[index]->GetRot(); }
    CModel * GetParts(int index) { return m_apModel[index]; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    CAnimation * GetAnimation(void) { return m_pAnimation; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

    // �A�j���[�V�������g�p���Ȃ��ꍇ�̃Z�b�^�[
    void SetUnableAnimation(void) { m_bUseAnimation = false; }

private:
    int                     m_aPartsType[CHARCTER_PARTS_MAX];           // �p�[�c�^�C�v
    D3DXVECTOR3				m_aPosDefault[CHARCTER_PARTS_MAX];          // �����ʒu
    int						m_anIndexParent[CHARCTER_PARTS_MAX];        // �e�ݒ�

    D3DXVECTOR3		m_pos;                                              // �S�Ă̐e�̈ʒu
    D3DXVECTOR3		m_rot;                                              // �S�Ă̐e�̌���
    int				m_nPartsNum;                                        // �p�[�c��

    CModel			*m_apModel[CHARCTER_PARTS_MAX];                     // ���f���ւ̃|�C���^
    CAnimation		*m_pAnimation;                                      // �A�j���[�V�����ւ̃|�C���^

    D3DXVECTOR3		m_posOld;                                           // 1F�O�̈ʒu

    char* m_cAnimFilePass;                                              // �A�j���[�V�����t�@�C���̃p�X

    bool m_bUseAnimation;												// �A�j���[�V���������邩�ǂ���

    bool m_bDrawWeapon;                                                 // �����`�悷�邩�ǂ���
};

#endif