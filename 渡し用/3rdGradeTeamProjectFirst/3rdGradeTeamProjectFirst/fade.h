//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "manager.h"

//================================================
// �N���X�錾
//================================================
class CFade
{
public:

    // �t�F�[�h�̎��
    typedef enum
    {
        FADE_NONE = 0,
        FADE_OUT,
        FADE_IN,
        FADE_MAX
    }FADE;

    CFade();										// �R���X�g���N�^
    ~CFade();										// �f�X�g���N�^
    HRESULT Init(CManager::MODE mode);				// ����������
    void Uninit(void);								// �I������
    void Update(void);								// �X�V����
    static void Draw(void);							// �`�揈��
    static void SetFade(CManager::MODE mode);		// �t�F�[�h�C������	
    static CFade *Create(CManager::MODE mode);		// �C���X�^���X����
    static FADE GetFade(void) { return m_fade; }    // �t�F�[�h�̏�Ԃ��擾

private:
    static D3DXCOLOR		m_col;			        // ���_�J���[
    static FADE				m_fade;                 // �t�F�[�h�̏��
    static CManager::MODE	m_modeNext;             // ���ɐݒ肷�郂�[�h
    static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
};

#endif