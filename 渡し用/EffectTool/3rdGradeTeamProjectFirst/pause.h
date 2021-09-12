//====================================================================
//
// �|�[�Y�̏���(�I�u�W�F�N�g���p�����Ă��Ȃ�) (pause.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �}�N����`
//================================================

#define PAUSE_BG_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))                   // �|�[�Y�̔w�i�̈ʒu
#define PAUSE_CONTINUE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 75.0f, 0.0f))     // �|�[�Y�̃R���e�B�j���[�̈ʒu
#define PAUSE_QUIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 75.0f, 0.0f))         // �|�[�Y�̂�߂�̈ʒu

#define PAUSE_MENU_SIZE_X 300.0f                                                                // �|�[�Y�̊e���j���[�̏c�T�C�Y 
#define PAUSE_MENU_SIZE_Y 80.0f                                                                 // �|�[�Y�̊e���j���[�̉��T�C�Y

#define PAUSE_MENU_INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)                       // �|�[�Y�̊e���j���[�̔�A�N�e�B�u�F
#define PAUSE_MENU_FADE_RATE 0.01f                                                              // �|�[�Y�̓_�Ŋ���
#define PAUSE_MENU_FADE_OUT_MIN 0.6f                                                            // �|�[�Y�̃t�F�[�h�A�E�g�̍ŏ��l
#define PAUSE_MENU_SELECT_COOL_TIME 12                                                          // �|�[�Y�̑I���̃N�[���^�C��

//================================================
// �N���X�錾
//================================================

// �|�[�Y�N���X
class CPause
{
public:

    // �|�[�Y�̎��
    typedef enum
    {
        TYPE_BG = 0,            // �w�i
        TYPE_CONTINUE,          // �R���e�B�j���[
        TYPE_QUIT,	            // ��߂�
        TYPE_MAX		        // ��ނ̍ő吔
    }TYPE;

    // �|�[�Y�̏��
    typedef enum
    {
        STATE_NONE = 0,     // �Ȃ�
        STATE_PAUSE,        // �|�[�Y
        STATE_MAX		    // ��ނ̍ő吔
    }STATE;

    CPause();
    ~CPause();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    static void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CPause *Create(void);
    void SelectPauseMenu(void);

    static void SetState(STATE state) { m_state = state; }  // ��Ԃ�ݒ�

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // �ǂݍ��񂾃e�N�X�`����ۑ�����
    static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		   // ���_�o�b�t�@�ւ̃|�C���^
    static STATE m_state;                              // ���
    TYPE m_typePauseSelect;                            // �|�[�Y�őI�𒆂̃^�C�v
    D3DXCOLOR m_colSelect;                             // �I�𒆂̐F
    bool m_bAlphaDown;                                 // �����x�������邩�グ�邩
    int m_nSelectCoolTime;                             // �I���̃N�[���^�C��
};

#endif