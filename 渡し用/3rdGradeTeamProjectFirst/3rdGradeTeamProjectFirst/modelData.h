//=============================================================================
//
// ���f���f�[�^�̏��� [modelData.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_MODEL_DATA (256)    // ���f�����̂��̂̃f�[�^
#define MAX_PARTS_LIST (128)    // �p�[�c���X�g
#define MAX_MODEL_POS_DEF (64)  // ���f���̏����ʒu�f�[�^
#define MAX_MODEL_PARTS_SET (8) // ���f���p�[�c�̍ő�Z�b�g���i�㔼�g�Ɖ����g�̊e�p�[�c�̍ő�j
#define MAX_PARAM (8)           // �ėp�̃p�����[�^
#define MAX_PARTS (4)           // �p�[�c�̍ő吔
#define MAX_MATERIAL (32)       // �}�e���A���̍ő吔

//=============================================================================
// ���f���f�[�^�N���X
//=============================================================================
class CModelData
{
public:

    typedef struct
    {
        //int nNumTexture;                              // �ǂݍ��ރe�N�X�`���̔ԍ�  �ix�t�@�C�����璼�ړǂݍ��ނ��ߍ폜�j
        LPDIRECT3DTEXTURE9	apTexMat[MAX_MATERIAL];     // �ǂݍ��ރe�N�X�`����x�t�@�C������}�e���A�������擾
        LPD3DXMESH pMesh;                               // �ǂݍ��ރ��b�V��
        LPD3DXBUFFER pBuffMat;                          // �ǂݍ��ރ}�e���A���̃o�b�t�@
        DWORD nNumMat;                                  // �ǂݍ��ރ}�e���A����
    }ModelData; // ���f���f�[�^

    typedef struct
    {
        int nIndex[MAX_MODEL_PARTS_SET];                // �C���f�b�N�X
        int nParent[MAX_MODEL_PARTS_SET];               // �e
        D3DXVECTOR3 pos[MAX_MODEL_PARTS_SET];           // �ʒu
    }ModelPosDef; // ���f���̏����ʒu�f�[�^

    typedef struct
    {
        float fAtk;                    // �U��
        float fDef;                    // �h��
        float fSpd;                    // ����
        float fWei;                    // �d��
        int nEx;                       // ����\�͔ԍ�
        float afParam[MAX_PARAM];      // �ėp�p�����[�^
    }PartsParam;    // �p�[�c�p�����[�^

    typedef struct
    {
        float fAtkRate;                // �U������
        float fDefRate;                // �h�䊄��
        float fSpdRate;                // ��������
        float fWeiRate;                // �d������
    }PartsRate;    // �p�[�c���[�g

    CModelData();	// �R���X�g���N�^
    ~CModelData();	// �f�X�g���N�^

    HRESULT Init(void);
    void LoadModelPosDef(void);
    void LoadPartsList(void);
    void LoadPartsRate(void);
    void Uninit(void);

    ModelData* GetModelData(const int nNum);    // ���f�����
    ModelPosDef* GetPosDef(const int nNum);     // ���f���̏����ʒu���
    PartsParam* GetPartsList(const int nNum);   // �p�[�c���X�g���
    PartsRate* GetPartsRate(const int nNum);    // �p�[�c���[�g���

private:
    ModelData m_aModelData[MAX_MODEL_DATA];         // ���f���f�[�^
    ModelPosDef m_aModelPosDef[MAX_MODEL_POS_DEF];  // ���f���̏����ʒu�f�[�^
    PartsParam m_aPartsList[MAX_PARTS_LIST];        // �p�[�c���X�g
    PartsRate m_aPartsRate[MAX_PARTS];              // �p�[�c���[�g
};
#endif