//=============================================================================
//
// �J�X�^�}�C�Y��ʏ��� [custom.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _CUSTOM_H_
#define _CUSTOM_H_

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#include "main.h"
#include "mode.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_EACH_PARTS 32   // �e�p�[�c�̍ő吔

//=============================================================================
// �O���錾
//=============================================================================
class CPlayer;
class CUI;
class CText;

//=============================================================================
// �N���X�錾
//=============================================================================
class CCustom : public CMode
{
public:

    // �J�X�^�}�C�Y��ʂł̑I��
    typedef enum
    {
        SELECT_1P_HEAD = 0,
        SELECT_1P_UP,
        SELECT_1P_DOWN,
        SELECT_1P_WEP,
        SELECT_MAX
    }SELECT;

    // �G���g���[���
    typedef struct
    {
        CUI *pUI_Cursor;        // �J�[�\��
        int select;             // �I��
        int nNumSelectHead;     // ���p�[�c�I��
        int nNumSelectUp;       // �㔼�g�p�[�c�I��
        int nNumSelectDown;     // �����g�p�[�c�I��
        int nNumSelectWep;      // ����p�[�c�I��
        CText *pText_Head;      // ���p�[�c�̖��O
        CText *pText_Up;        // �㔼�g�p�[�c�̖��O
        CText *pText_Down;      // �����g�p�[�c�̖��O
        CText *pText_Wep;       // ����p�[�c�̖��O
        bool bUseCursor;        // �J�[�\�����g���Ă��邩�ǂ���
    }ENTRY_INFO;

    CCustom();
    ~CCustom();
    HRESULT Init(void);
    void BindHaveParts(void);                                                  // �������Ă���p�[�c�������т���
    void Uninit(void);
    void Update(void);

    void MoveCursor(void);  // �J�[�\���ړ�

    void SaveCustom(int nNumSaveWho, int nNumSaveWhere, int nNumSaveParts);    // �N�̂ǂ������̃p�[�c�ɂ��ĕۑ����邩

private:
    CPlayer *m_apPlayer[MAX_PLAYER];    // �v���C���[�̃|�C���^
    int m_anPartsHead[MAX_EACH_PARTS];  // ���p�[�c
    int m_anPartsUp[MAX_EACH_PARTS];    // �㔼�g�p�[�c
    int m_anPartsDown[MAX_EACH_PARTS];  // �����g�p�[�c
    int m_anPartsWep[MAX_EACH_PARTS];   // ����p�[�c

    ENTRY_INFO m_aEntryInfo[MAX_PLAYER];// �G���g���[���
};

#endif