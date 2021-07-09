//====================================================================
//
// �T�E���h���� (sound.h)
// Author : �㓡�T�V��
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
        LABEL_MAX,
    } LABEL;

    CSound();
    ~CSound();  // �e�N���X�̃f�X�g���N�^��virtual�����邱�ƂŁA���������[�N��}����
    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT Play(LABEL label);
    void Stop(LABEL label);
    void StopAll(void);

private:
    typedef struct
    {
        char *pFilename;	// �t�@�C����
        int nCntLoop;		// ���[�v�J�E���g (-1�Ń��[�v�A0�Ń��[�v�Ȃ�)
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
    IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
    BYTE *m_apDataAudio[LABEL_MAX];					    // �I�[�f�B�I�f�[�^
    DWORD m_aSizeAudio[LABEL_MAX];					    // �I�[�f�B�I�f�[�^�T�C�Y
    static PARAM m_aParam[LABEL_MAX];                   // �e���f�ނ̃p�����[�^
};

#endif