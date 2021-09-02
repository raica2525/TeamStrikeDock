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
#include "mode.h"

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
    static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);  // �߂�l�͌Ăяo����Ŏg���K�v���Ȃ����ߖ���

#ifdef _DEBUG
    static void ReloadUI(CUI::SET set);     // �e�N�X�`���̍ēǂݍ���
    static void CommonCommand(void);        // ���ʂ̃f�o�b�O�R�}���h
#endif

private:
    int m_nLife;    // ����
    TYPE m_type;    // ���
};

// �f�o�b�N���j���[�N���X
class CDebugMenu : public CMode
{
public:

    // �f�o�b�N���j���[�̑I��
    typedef enum
    {
        SELECT_TRAINING = 0,	  // �g���[�j���O
        SELECT_CUSTOMIZE,	      // �J�X�^�}�C�Y
        SELECT_TITLE,             // �^�C�g��
        SELECT_ARENA_1,           // �A���[�i1
        SELECT_ARENA_2,           // �A���[�i2
        SELECT_ARENA_3,           // �A���[�i3
        SELECT_ARENA_4,           // �A���[�i4
        SELECT_ARENA_5,           // �A���[�i5
        SELECT_ARENA_6,           // �A���[�i6
        SELECT_MAX			      // �I���̍ő吔
    }SELECT;

    CDebugMenu();
    ~CDebugMenu();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

private:
    CUI *m_pUI_Cursor;      // �J�[�\��
    int m_nSelectCoolTime;  // �I���̃N�[���^�C��
    int m_select;           // �I��
};

#endif