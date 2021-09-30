//=============================================================================
//
// �T�E���h�̏��� [sound.cpp] (�T�E���h)
// Author : �r�c�I��
//
//=============================================================================
#include "sound.h"

//================================================
// �}�N����`
//================================================
#define DEFAULT_VOLUME (100)	//���ʂ̏����l

//============================================
// �ÓI�����o�ϐ��錾
//============================================
CSound::PARAM CSound::m_aParam[LABEL_MAX] =
{
    // SE
    { "data/SOUND/SE/hit1.wav", 0 , CATEGORY_SE },								// �q�b�g��
    { "data/SOUND/SE/swish.wav", 0 , CATEGORY_SE },								// �f�U�艹
    { "data/SOUND/SE/bound.wav", 0 , CATEGORY_SE },								// �o�E���h��
    { "data/SOUND/SE/catch.wav", 0 , CATEGORY_SE },								// �L���b�`��
    { "data/SOUND/SE/throw.wav", 0 , CATEGORY_SE },								// ������
    { "data/SOUND/SE/ko.wav", 0 , CATEGORY_SE },								// KO��
    { "data/SOUND/SE/bunt.wav", 0 , CATEGORY_SE },								// �o���g��
    { "data/SOUND/SE/damage.wav", 0 , CATEGORY_SE },							// �_���[�W��
    { "data/SOUND/SE/offset.wav", 0 , CATEGORY_SE },							// ���E��
    { "data/SOUND/SE/hit2.wav", 0 , CATEGORY_SE },								// ��q�b�g��
    { "data/SOUND/SE/ball_disping.wav", 0 , CATEGORY_SE },						// �{�[���z�u��
    { "data/SOUND/SE/jump.wav", 0 , CATEGORY_SE },								// �W�����v��
    { "data/SOUND/SE/hit3.wav", 0 , CATEGORY_SE },								// �E���g���q�b�g��
	{ "data/SOUND/SE/select.wav", 0 , CATEGORY_SE },							// ���艹
	{ "data/SOUND/SE/custom.wav", 0 , CATEGORY_SE },							// �J�X�^�}�C�Y�ύX��
	{ "data/SOUND/SE/cancel.wav", 0 , CATEGORY_SE },							// �L�����Z����
	{ "data/SOUND/SE/entry.wav", 0 , CATEGORY_SE },								// �G���g���[��
	{ "data/SOUND/SE/ready.wav", 0 , CATEGORY_SE },								// READY�{�^���o����
	{ "data/SOUND/SE/heal.wav", 0 , CATEGORY_SE },								// �񕜉�
	{ "data/SOUND/SE/info.wav", 0 , CATEGORY_SE },								// �ڍׂ��o�鎞�̉�
	{ "data/SOUND/SE/cursolon.wav", 0 , CATEGORY_SE },							// �{�^���ɃJ�[�\������������̉�
	{ "data/SOUND/SE/warp.wav", 0 , CATEGORY_SE },								// �{�[�����[�v��
	{ "data/SOUND/SE/skill.wav", 0 , CATEGORY_SE },								// �K�E�Z������
	{ "data/SOUND/BGM/title.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },		// �^�C�g��BGM
	{ "data/SOUND/BGM/menu.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },		// ���j���[&�J�X�^�}�C�YBGM
	{ "data/SOUND/BGM/battle00.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// �o�g��BGM00
	{ "data/SOUND/BGM/battle01.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// �o�g��BGM01
	{ "data/SOUND/BGM/battle02.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// �o�g��BGM02
	{ "data/SOUND/VOICE/Icarus_WIN.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/Kleinod_WIN.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/Knight_WIN.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/OOO_WIN.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/Ranger_WIN.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/X_WIN.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/Icarus_SP.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/Kleinod_SP.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/Knight_SP.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/OOO_SP.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/Ranger_SP.wav", 0 , CATEGORY_SE },
	{ "data/SOUND/VOICE/X_SP.wav", 0 , CATEGORY_SE },
    { "data/SOUND/SE/wing.wav", 0 , CATEGORY_SE },
    { "data/SOUND/SE/ok.wav", 0 , CATEGORY_SE },
    { "data/SOUND/BGM/result.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// ���U���gBGM
	{ "data/SOUND/SE/finish.wav", 0 , CATEGORY_SE },
};

//=============================================================================
// �T�E���h�̃f�t�H���g�R���X�g���N�^
//=============================================================================
CSound::CSound()
{
    m_pXAudio2 = NULL;
    m_pMasteringVoice = NULL;
    memset(m_apSourceVoice, 0, sizeof(m_apSourceVoice));
    memset(m_apDataAudio, 0, sizeof(m_apDataAudio));
    memset(m_aSizeAudio, 0, sizeof(m_aSizeAudio));
    memset(m_anVolume, 0, sizeof(m_anVolume));
}

//=============================================================================
// �T�E���h�̃f�X�g���N�^
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// �T�E���h����������
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
    HRESULT hr;

    // COM���C�u�����̏�����
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2�I�u�W�F�N�g�̍쐬
    hr = XAudio2Create(&m_pXAudio2, 0);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

        // COM���C�u�����̏I������
        CoUninitialize();

        return E_FAIL;
    }

    // �}�X�^�[�{�C�X�̐���
    hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

        if (m_pXAudio2)
        {
            // XAudio2�I�u�W�F�N�g�̊J��
            m_pXAudio2->Release();
            m_pXAudio2 = NULL;
        }

        // COM���C�u�����̏I������
        CoUninitialize();

        return E_FAIL;
    }

    //���ʂ̏�����
    for (int nCount = 0; nCount < CATEGORY_MAX; nCount++)
    {
        m_anVolume[nCount] = DEFAULT_VOLUME;
    }

    // �T�E���h�f�[�^�̏�����
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        HANDLE hFile;
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        DWORD dwFiletype;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // �o�b�t�@�̃N���A
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // �T�E���h�f�[�^�t�@�C���̐���
        hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// �t�@�C���|�C���^��擪�Ɉړ�
            MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVE�t�@�C���̃`�F�b�N
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        if (dwFiletype != 'EVAW')
        {
            MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �t�H�[�}�b�g�`�F�b�N
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �I�[�f�B�I�f�[�^�ǂݍ���
        hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }
        m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
        hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �\�[�X�{�C�X�̐���
        hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
        if (FAILED(hr))
        {
            MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
            return S_FALSE;
        }

        // �o�b�t�@�̒l�ݒ�
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = m_aSizeAudio[nCntSound];
        buffer.pAudioData = m_apDataAudio[nCntSound];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

        // �I�[�f�B�I�o�b�t�@�̓o�^
        m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

        //���ʂ�ݒ�
        SetVolume((LABEL)nCntSound, m_anVolume[m_aParam[nCntSound].category]);

        // �t�@�C�����N���[�Y
        CloseHandle(hFile);
    }

    return S_OK;
}

//=============================================================================
// �T�E���h�I������
//=============================================================================
void CSound::Uninit(void)
{
    // �ꎞ��~
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // �ꎞ��~
            m_apSourceVoice[nCntSound]->Stop(0);

            // �\�[�X�{�C�X�̔j��
            m_apSourceVoice[nCntSound]->DestroyVoice();
            m_apSourceVoice[nCntSound] = NULL;

            // �I�[�f�B�I�f�[�^�̊J��
            free(m_apDataAudio[nCntSound]);
            m_apDataAudio[nCntSound] = NULL;
        }
    }

    // �}�X�^�[�{�C�X�̔j��
    m_pMasteringVoice->DestroyVoice();
    m_pMasteringVoice = NULL;

    if (m_pXAudio2)
    {
        // XAudio2�I�u�W�F�N�g�̊J��
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    // COM���C�u�����̏I������
    CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT CSound::Play(LABEL label, float fPitch)
{
    XAUDIO2_VOICE_STATE xa2state;
    XAUDIO2_BUFFER buffer;

    // �o�b�t�@�̒l�ݒ�

    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = m_aSizeAudio[label];
    buffer.pAudioData = m_apDataAudio[label];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = m_aParam[label].nCntLoop;

    // ��Ԏ擾
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// �Đ���
     // �ꎞ��~
        m_apSourceVoice[label]->Stop(0);

        // �I�[�f�B�I�o�b�t�@�̍폜
        m_apSourceVoice[label]->FlushSourceBuffers();
    }

    // �I�[�f�B�I�o�b�t�@�̓o�^
    m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);
    m_apSourceVoice[label]->SetFrequencyRatio(fPitch);
    // �Đ�
    m_apSourceVoice[label]->Start(0);

    return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void CSound::Stop(LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;

    // ��Ԏ擾
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// �Đ���
     // �ꎞ��~
        m_apSourceVoice[label]->Stop(0);

        // �I�[�f�B�I�o�b�t�@�̍폜
        m_apSourceVoice[label]->FlushSourceBuffers();
    }
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void CSound::StopAll(void)
{
    // �ꎞ��~
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // �ꎞ��~
            m_apSourceVoice[nCntSound]->Stop(0);
        }
    }
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
    HRESULT hr = S_OK;
    DWORD dwRead;
    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD dwBytesRead = 0;
    DWORD dwOffset = 0;

    if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// �t�@�C���|�C���^��擪�Ɉړ�
        return HRESULT_FROM_WIN32(GetLastError());
    }

    while (hr == S_OK)
    {
        if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
        {// �`�����N�̓ǂݍ���
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
        {// �`�����N�f�[�^�̓ǂݍ���
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch (dwChunkType)
        {
        case 'FFIR':
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
            {// �t�@�C���^�C�v�̓ǂݍ���
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
                return HRESULT_FROM_WIN32(GetLastError());
            }
        }

        dwOffset += sizeof(DWORD) * 2;
        if (dwChunkType == format)
        {
            *pChunkSize = dwChunkDataSize;
            *pChunkDataPosition = dwOffset;

            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        if (dwBytesRead >= dwRIFFDataSize)
        {
            return S_FALSE;
        }
    }

    return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
    DWORD dwRead;

    if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// �t�@�C���|�C���^���w��ʒu�܂ňړ�
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
    {// �f�[�^�̓ǂݍ���
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

//=============================================================================
// ���ʐݒ�
//=============================================================================
void CSound::SetVolume(LABEL label, int nVolume)
{
    m_apSourceVoice[label]->SetVolume(float(nVolume) / DEFAULT_VOLUME);
}

//=============================================================================
// �J�e�S�����Ƃ̉��ʐݒ�
//=============================================================================
void CSound::SetCategoryVolume(CATEGORY category, int nVolume)
{
    m_anVolume[category] = nVolume;
    //�ݒ肳�ꂽ��ʂ̉����̉��ʂ�ύX����
    for (int nCount = 0; nCount < LABEL_MAX; nCount++)
    {
        if (m_aParam[nCount].category == category)
        {
            SetVolume((LABEL)nCount, nVolume);
        }
    }
}