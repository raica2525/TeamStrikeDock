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
        SELECT_1P_READY,
        SELECT_2P_HEAD_RIGHT,
        SELECT_2P_HEAD_LEFT,
        SELECT_2P_UP_RIGHT,
        SELECT_2P_UP_LEFT,
        SELECT_2P_DOWN_RIGHT,
        SELECT_2P_DOWN_LEFT,
        SELECT_2P_WEP_RIGHT,
        SELECT_2P_WEP_LEFT,
        SELECT_2P_READY,
        SELECT_3P_HEAD_RIGHT,
        SELECT_3P_HEAD_LEFT,
        SELECT_3P_UP_RIGHT,
        SELECT_3P_UP_LEFT,
        SELECT_3P_DOWN_RIGHT,
        SELECT_3P_DOWN_LEFT,
        SELECT_3P_WEP_RIGHT,
        SELECT_3P_WEP_LEFT,
        SELECT_3P_READY,
        SELECT_4P_HEAD_RIGHT,
        SELECT_4P_HEAD_LEFT,
        SELECT_4P_UP_RIGHT,
        SELECT_4P_UP_LEFT,
        SELECT_4P_DOWN_RIGHT,
        SELECT_4P_DOWN_LEFT,
        SELECT_4P_WEP_RIGHT,
        SELECT_4P_WEP_LEFT,
        SELECT_4P_READY,
        SELECT_1P_CHANGE,
        SELECT_2P_CHANGE,
        SELECT_3P_CHANGE,
        SELECT_4P_CHANGE,
        SELECT_1P_OSUSUME,
        SELECT_2P_OSUSUME,
        SELECT_3P_OSUSUME,
        SELECT_4P_OSUSUME,
        SELECT_1P_DETAIL,
        SELECT_2P_DETAIL,
        SELECT_3P_DETAIL,
        SELECT_4P_DETAIL,
        SELECT_READY_TO_FIGHT,
        SELECT_RETURN,
        SELECT_STOCK,
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
        CLICK_TYPE_PARTS = 0,        // �p�[�c
        CLICK_TYPE_READY,            // ��������
        CLICK_TYPE_CHANGE,           // �`�F���W
        CLICK_TYPE_OSUSUME,          // �I�X�X��
        CLICK_TYPE_DETAIL,           // �ڍ�
        CLICK_TYPE_READY_TO_FIGHT,   // �킢��
        CLICK_TYPE_RETURN,           // �߂�
        CLICK_TYPE_STOCK,            // �X�g�b�N
    }CLICK_TYPE;

    // �G���g���[���
    typedef enum
    {
        ENTRY_STATUS_WAITING = 0,   // �ҋ@��
        ENTRY_STATUS_PLAYER,        // �v���C���[
        ENTRY_STATUS_CP_LEVEL_1,    // CP��킢
        ENTRY_STATUS_CP_LEVEL_2,    // CP�ӂ�
        ENTRY_STATUS_CP_LEVEL_3,    // CP�悢
    }ENTRY_STATUS;

    // �G���g���[���
    typedef struct
    {
        CPlayer *pPlayer;               // �v���C���[
        CUI *pUI_Cursor;                // �J�[�\��
        CUI *pUI_Bg_Wait;               // �ҋ@���̔w�i
        CUI *pUI_Bg_Select;             // �I�𒆂̔w�i
        CUI *pUI_Bg_Select_Out_Frame;   // �O���̑I��g�i�F�t���j
        CUI *pUI_Bg_Select_In_Frame;    // �����̑I��g
        CUI *pUI_Bg_Select_Status;      // �e�X�e�[�^�X�̕���
        CUI *pUI_Bg_Ready;              // ���������̔w�i
        int nNumSelectHead;             // ���p�[�c�I��
        int nNumSelectUp;               // �㔼�g�p�[�c�I��
        int nNumSelectDown;             // �����g�p�[�c�I��
        int nNumSelectWep;              // ����p�[�c�I��
		int nOsusumeOld;				// �O��̂������߂̏��
        CText *pText_Head;              // ���p�[�c�̖��O�\��
        CText *pText_Up;                // �㔼�g�p�[�c�̖��O�\��
        CText *pText_Down;              // �����g�p�[�c�̖��O�\��
        CText *pText_Wep;               // ����p�[�c�̖��O�\��
        bool bUseCursor;                // �J�[�\�����g���Ă��邩�ǂ���
        int nNumSelectUIOld;            // 1F�O�̑I����
        bool bReady;                    // �����������ǂ���
        ENTRY_STATUS status;            // ���
    }ENTRY_INFO;

    CCustom();
    ~CCustom();
    HRESULT Init(void);
    void BindHaveParts(void);                                                  // �������Ă���p�[�c�������т���
    void Uninit(void);
    void Update(void);

    /*========================================================
    // �J�[�\������
    //======================================================*/
    void MoveCursorByKeyboard(float& fStickAngle, bool& bTiltedStick, float& fTiltedStickValue); // �L�[�{�[�h����
    void MoveCursor(void);                                                     // �J�[�\���ړ�
    void RimitMoveCursor(D3DXVECTOR3& cursorPos);                              // �J�[�\���ړ��̐���
    void PressFire(int nNumWho);                                               // �v���X��������ƁA�����o��
    void CollisionSelect(int nNumWho, D3DXVECTOR3 cursorPos);                  // �I�����Ƃ̓����蔻��
    void ClickSelect(int nNumWho, CUI* pSelectUI, D3DXVECTOR3 cursorPos);      // �I�����̃N���b�N

    /*========================================================
    // �G���g���[����
    //======================================================*/
    void ChangeEntryStatus(int nNumWho, ENTRY_STATUS nextEntryStatus);         // �G���g���[��Ԃ̃`�F���W
    void ToggleReady(int nNumWho);                                             // ���������̃g�O��
    void JudgmentReadyToFight(void);                                           // ���������ł��Ă��邩�̔��f

    /*========================================================
    // �p�[�c�I������
    //======================================================*/
    void SelectParts(int nNumWho, int nNumWhere, bool bRight);                 // �J�X�^�}�C�Y����p�[�c��I��
    void SaveCustom(int nNumSaveWho, int nNumSaveWhere, int nNumSaveParts);    // �N�̂ǂ������̃p�[�c�ɂ��ĕۑ����邩
    void DoOsusume(int nNumWho);                                               // �I�X�X������

private:
    int m_anMemoryPartsHead[MAX_EACH_PARTS];  // �ێ��������p�[�c
    int m_anMemoryPartsUp[MAX_EACH_PARTS];    // �ێ������㔼�g�p�[�c
    int m_anMemoryPartsDown[MAX_EACH_PARTS];  // �ێ����������g�p�[�c
    int m_anMemoryPartsWep[MAX_EACH_PARTS];   // �ێ���������p�[�c

    ENTRY_INFO m_aEntryInfo[MAX_PLAYER];      // �G���g���[���
    
    bool m_bClickReadyToFight;                // ReadyToFight�������ꂽ���ǂ���
    bool m_bDispReadyToFight;                 // ReadyToFight��\�����邩�ǂ���
};

#endif