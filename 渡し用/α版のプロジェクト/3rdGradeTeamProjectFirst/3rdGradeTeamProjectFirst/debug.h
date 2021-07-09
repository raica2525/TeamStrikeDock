//====================================================================
//
// �f�o�b�O����̏��� (debug.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"
#include "ui.h"

//================================================
// �}�N����`
//================================================

#ifdef _DEBUG
// �����蔻��𐶐�����
#define COLLISION_TEST
#endif

//================================================
// �N���X�錾
//================================================

// �f�o�b�N�N���X
class CDebug : public CScene3D
{
public:

    // �f�o�b�N�̎��
    typedef enum
    {
        TYPE_PERMANENT = 0,	    // �i��
        TYPE_MOMENT,	        // �u��
        TYPE_MAX			    // ��ނ̍ő吔
    }TYPE;

    CDebug();
    ~CDebug();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CDebug *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);

#ifdef _DEBUG
    static void ReloadUI(CUI::SET set);     // �e�N�X�`���̍ēǂݍ���
    static void CommonCommand(void);        // ���ʂ̃f�o�b�O�R�}���h
    static void TitleCommand(void);         // �^�C�g�����̃f�o�b�O�R�}���h
    static void ManualCommand(void);        // �V�ѕ����̃f�o�b�O�R�}���h
    static void GameCommand(void);          // �Q�[�����̃f�o�b�O�R�}���h
    static void ResultCommand(void);        // ���U���g���̃f�o�b�O�R�}���h
#endif

private:
    int m_nLife;    // ����
    TYPE m_type;    // ���
};

#endif