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
#include "ui.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_EACH_PARTS 32   // �e�p�[�c�̍ő吔

//=============================================================================
// �O���錾
//=============================================================================
class CPlayer;
class CText;

//=============================================================================
// �N���X�錾
//=============================================================================
class CCustom : public CMode
{
public:

    // �J�X�^�}�C�Y��ʂł̑I��(�A�N�Z�X�i���o�[)
    typedef enum
    {
        SELECT_1P_HEAD_RIGHT = 0,
        SELECT_1P_HEAD_LEFT,
        SELECT_1P_UP_RIGHT,
        SELECT_1P_UP_LEFT,
        SELECT_1P_DOWN_RIGHT,
        SELECT_1P_DOWN_LEFT,
        SELECT_1P_WEP_RIGHT,
        SELECT_1P_WEP_LEFT,
        SELECT_1P_REDAY,
        SELECT_2P_HEAD_RIGHT,
        SELECT_2P_HEAD_LEFT,
        SELECT_2P_UP_RIGHT,
        SELECT_2P_UP_LEFT,
        SELECT_2P_DOWN_RIGHT,
        SELECT_2P_DOWN_LEFT,
        SELECT_2P_WEP_RIGHT,
        SELECT_2P_WEP_LEFT,
        SELECT_2P_REDAY,
        SELECT_3P_HEAD_RIGHT,
        SELECT_3P_HEAD_LEFT,
        SELECT_3P_UP_RIGHT,
        SELECT_3P_UP_LEFT,
        SELECT_3P_DOWN_RIGHT,
        SELECT_3P_DOWN_LEFT,
        SELECT_3P_WEP_RIGHT,
        SELECT_3P_WEP_LEFT,
        SELECT_3P_REDAY,
        SELECT_4P_HEAD_RIGHT,
        SELECT_4P_HEAD_LEFT,
        SELECT_4P_UP_RIGHT,
        SELECT_4P_UP_LEFT,
        SELECT_4P_DOWN_RIGHT,
        SELECT_4P_DOWN_LEFT,
        SELECT_4P_WEP_RIGHT,
        SELECT_4P_WEP_LEFT,
        SELECT_4P_REDAY,
        SELECT_MAX
    }SELECT;

    // �⏕�l�̓���i�N���b�N�ҁj
    typedef enum
    {
        PARAM_CLICK_TYPE = 0,   // ���
        PARAM_CLICK_WHO,        // �N��
        PARAM_CLICK_WHERE,      // �ǂ���
        PARAM_CLICK_RIGHT,      // �E��
    }PARAM_CLICK;

    // �N���b�N�������̂̎��
    typedef enum
    {
        CLICK_TYPE_PARTS = 0,   // �p�[�c
        CLICK_TYPE_READY,       // ��������
    }CLICK_TYPE;

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
        int nNumSelectUIOld;    // 1F�O�̑I����
    }ENTRY_INFO;

    CCustom();
    ~CCustom();
    HRESULT Init(void);
    void BindHaveParts(void);                                                  // �������Ă���p�[�c�������т���
    void Uninit(void);
    void Update(void);

    void MoveCursor(void);                                                     // �J�[�\���ړ�
    void CollisionSelect(int nNumWho, D3DXVECTOR3 cursorPos);                  // �I�����Ƃ̓����蔻��
    void ClickSelect(int nNumWho, CUI* pSelectUI);                             // �I�����̃N���b�N
    void SelectParts(int nNumWho, int nNumWhere, bool bRight);                 // �J�X�^�}�C�Y����p�[�c��I��

    void SaveCustom(int nNumSaveWho, int nNumSaveWhere, int nNumSaveParts);    // �N�̂ǂ������̃p�[�c�ɂ��ĕۑ����邩

private:
    CPlayer *m_apPlayer[MAX_PLAYER];          // �v���C���[�̃|�C���^
    int m_anMemoryPartsHead[MAX_EACH_PARTS];  // �ێ��������p�[�c
    int m_anMemoryPartsUp[MAX_EACH_PARTS];    // �ێ������㔼�g�p�[�c
    int m_anMemoryPartsDown[MAX_EACH_PARTS];  // �ێ����������g�p�[�c
    int m_anMemoryPartsWep[MAX_EACH_PARTS];   // �ێ���������p�[�c

    ENTRY_INFO m_aEntryInfo[MAX_PLAYER];      // �G���g���[���
};

#endif