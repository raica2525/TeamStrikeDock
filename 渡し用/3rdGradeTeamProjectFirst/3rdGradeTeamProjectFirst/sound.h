//====================================================================
//
// �T�E���h���� (sound.h)
// Author : �r�c�I��
//
//====================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �N���X�錾
//================================================

// �T�E���h�̃N���X
class CSound
{
public:
    typedef enum
    {
        // SE
        LABEL_SE_HIT = 0,		// �q�b�g��
        LABEL_SE_SWISH,         // �f�U�艹
        LABEL_SE_BOUND,         // �o�E���h��
        LABEL_SE_CATCH,         // �L���b�`��
        LABEL_SE_THROW,         // ������
        LABEL_SE_KO,            // KO��
        LABEL_SE_BUNT,          // �o���g��
        LABEL_SE_DAMAGE,        // �_���[�W��
        LABEL_SE_OFFSET,        // ���E��
        LABEL_SE_HIT_BIG,       // ��q�b�g��
        LABEL_SE_BALL_DISPING,  // �{�[���z�u��
        LABEL_SE_JUMP,          // �W�����v��
        LABEL_SE_HIT_ULTRA,     // �E���g���q�b�g��
		LABEL_SE_SELECT,		// ���艹
		LABEL_SE_CUSTOM,		// �J�X�^�}�C�Y�ύX��
		LABEL_SE_CANCEL,		// �L�����Z����
		LABEL_SE_ENTRY,			// �G���g���[��
		LABEL_SE_READY,			// READY�{�^���o����
		LABEL_SE_HEAL,			// �񕜉�
		LABEL_SE_INFO,			// �ڍׂ��o�鎞�̉�
		LABEL_SE_CURSOLON,		// �{�^���ɃJ�[�\������������̉�
		LABEL_SE_WARP,			// �{�[�����[�v��
		LABEL_SE_SKILL,			// �K�E�Z������
		LABEL_BGM_TITLE,		// �^�C�g��BGM
		LABEL_BGM_MENU,			// ���j���[&�J�X�^�}�C�YBGM
		LABEL_BGM_BATTLE00,		// �o�g��BGM00
		LABEL_BGM_BATTLE01,		// �o�g��BGM01
		LABEL_BGM_BATTLE02,		// �o�g��BGM02
		LABEL_VOICE_WIN_ICARUS,	// �����{�C�X
		LABEL_VOICE_WIN_KLEINOD,
		LABEL_VOICE_WIN_KNIGHT,
		LABEL_VOICE_WIN_OOO,
		LABEL_VOICE_WIN_RANGER,
		LABEL_VOICE_WIN_X,
		LABEL_VOICE_SP_ICARUS,	// �K�E�Z�{�C�X
		LABEL_VOICE_SP_KLEINOD,
		LABEL_VOICE_SP_KNIGHT,
		LABEL_VOICE_SP_OOO,
		LABEL_VOICE_SP_RANGER,
		LABEL_VOICE_SP_X,
        LABEL_SE_WING,
        LABEL_SE_OK,
        LABEL_BGM_RESULT,		// ���U���gBGM
		LABEL_SE_FINISH,		// �t�B�j�b�V��SE
        LABEL_MAX,
    } LABEL;

    typedef enum
    {
        CATEGORY_SE = 0,
        CATEGORY_BGM,
        CATEGORY_MAX
    } CATEGORY;

    CSound();
    ~CSound();
    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT Play(LABEL label, float fPitch = 1.0f);
    void Stop(LABEL label);
    void StopAll(void);
    void SetVolume(LABEL label, int nVolume);				// ���ʐݒ�p�֐�
    void SetCategoryVolume(CATEGORY category, int nVolume); // �J�e�S�����Ƃ̉��ʐݒ�p�֐�
private:
    typedef struct
    {
        char *pFilename;	// �t�@�C����
        int nCntLoop;		// ���[�v�J�E���g (-1�Ń��[�v�A0�Ń��[�v�Ȃ�)
        CATEGORY category;	// ��ޕ���
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
    IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
    BYTE *m_apDataAudio[LABEL_MAX];					    // �I�[�f�B�I�f�[�^
    DWORD m_aSizeAudio[LABEL_MAX];					    // �I�[�f�B�I�f�[�^�T�C�Y
    static PARAM m_aParam[LABEL_MAX];                   // �e���f�ނ̃p�����[�^
    int m_anVolume[CATEGORY_MAX];						// �e��ʂ̉���
};

#endif