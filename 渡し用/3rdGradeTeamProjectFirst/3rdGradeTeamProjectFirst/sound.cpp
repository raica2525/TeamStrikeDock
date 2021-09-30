//=============================================================================
//
// サウンドの処理 [sound.cpp] (サウンド)
// Author : 池田悠希
//
//=============================================================================
#include "sound.h"

//================================================
// マクロ定義
//================================================
#define DEFAULT_VOLUME (100)	//音量の初期値

//============================================
// 静的メンバ変数宣言
//============================================
CSound::PARAM CSound::m_aParam[LABEL_MAX] =
{
    // SE
    { "data/SOUND/SE/hit1.wav", 0 , CATEGORY_SE },								// ヒット音
    { "data/SOUND/SE/swish.wav", 0 , CATEGORY_SE },								// 素振り音
    { "data/SOUND/SE/bound.wav", 0 , CATEGORY_SE },								// バウンド音
    { "data/SOUND/SE/catch.wav", 0 , CATEGORY_SE },								// キャッチ音
    { "data/SOUND/SE/throw.wav", 0 , CATEGORY_SE },								// 投げ音
    { "data/SOUND/SE/ko.wav", 0 , CATEGORY_SE },								// KO音
    { "data/SOUND/SE/bunt.wav", 0 , CATEGORY_SE },								// バント音
    { "data/SOUND/SE/damage.wav", 0 , CATEGORY_SE },							// ダメージ音
    { "data/SOUND/SE/offset.wav", 0 , CATEGORY_SE },							// 相殺音
    { "data/SOUND/SE/hit2.wav", 0 , CATEGORY_SE },								// 大ヒット音
    { "data/SOUND/SE/ball_disping.wav", 0 , CATEGORY_SE },						// ボール配置音
    { "data/SOUND/SE/jump.wav", 0 , CATEGORY_SE },								// ジャンプ音
    { "data/SOUND/SE/hit3.wav", 0 , CATEGORY_SE },								// ウルトラヒット音
	{ "data/SOUND/SE/select.wav", 0 , CATEGORY_SE },							// 決定音
	{ "data/SOUND/SE/custom.wav", 0 , CATEGORY_SE },							// カスタマイズ変更音
	{ "data/SOUND/SE/cancel.wav", 0 , CATEGORY_SE },							// キャンセル音
	{ "data/SOUND/SE/entry.wav", 0 , CATEGORY_SE },								// エントリー音
	{ "data/SOUND/SE/ready.wav", 0 , CATEGORY_SE },								// READYボタン出現音
	{ "data/SOUND/SE/heal.wav", 0 , CATEGORY_SE },								// 回復音
	{ "data/SOUND/SE/info.wav", 0 , CATEGORY_SE },								// 詳細が出る時の音
	{ "data/SOUND/SE/cursolon.wav", 0 , CATEGORY_SE },							// ボタンにカーソルが乗った時の音
	{ "data/SOUND/SE/warp.wav", 0 , CATEGORY_SE },								// ボールワープ音
	{ "data/SOUND/SE/skill.wav", 0 , CATEGORY_SE },								// 必殺技発動音
	{ "data/SOUND/BGM/title.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },		// タイトルBGM
	{ "data/SOUND/BGM/menu.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },		// メニュー&カスタマイズBGM
	{ "data/SOUND/BGM/battle00.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// バトルBGM00
	{ "data/SOUND/BGM/battle01.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// バトルBGM01
	{ "data/SOUND/BGM/battle02.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// バトルBGM02
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
    { "data/SOUND/BGM/result.wav", XAUDIO2_LOOP_INFINITE , CATEGORY_BGM },	// リザルトBGM
	{ "data/SOUND/SE/finish.wav", 0 , CATEGORY_SE },
};

//=============================================================================
// サウンドのデフォルトコンストラクタ
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
// サウンドのデストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// サウンド初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
    HRESULT hr;

    // COMライブラリの初期化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2オブジェクトの作成
    hr = XAudio2Create(&m_pXAudio2, 0);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // マスターボイスの生成
    hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

        if (m_pXAudio2)
        {
            // XAudio2オブジェクトの開放
            m_pXAudio2->Release();
            m_pXAudio2 = NULL;
        }

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    //音量の初期化
    for (int nCount = 0; nCount < CATEGORY_MAX; nCount++)
    {
        m_anVolume[nCount] = DEFAULT_VOLUME;
    }

    // サウンドデータの初期化
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        HANDLE hFile;
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        DWORD dwFiletype;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // バッファのクリア
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // サウンドデータファイルの生成
        hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// ファイルポインタを先頭に移動
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVEファイルのチェック
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        if (dwFiletype != 'EVAW')
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // フォーマットチェック
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // オーディオデータ読み込み
        hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
        hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // ソースボイスの生成
        hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
        if (FAILED(hr))
        {
            MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // バッファの値設定
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = m_aSizeAudio[nCntSound];
        buffer.pAudioData = m_apDataAudio[nCntSound];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

        // オーディオバッファの登録
        m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

        //音量を設定
        SetVolume((LABEL)nCntSound, m_anVolume[m_aParam[nCntSound].category]);

        // ファイルをクローズ
        CloseHandle(hFile);
    }

    return S_OK;
}

//=============================================================================
// サウンド終了処理
//=============================================================================
void CSound::Uninit(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // 一時停止
            m_apSourceVoice[nCntSound]->Stop(0);

            // ソースボイスの破棄
            m_apSourceVoice[nCntSound]->DestroyVoice();
            m_apSourceVoice[nCntSound] = NULL;

            // オーディオデータの開放
            free(m_apDataAudio[nCntSound]);
            m_apDataAudio[nCntSound] = NULL;
        }
    }

    // マスターボイスの破棄
    m_pMasteringVoice->DestroyVoice();
    m_pMasteringVoice = NULL;

    if (m_pXAudio2)
    {
        // XAudio2オブジェクトの開放
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    // COMライブラリの終了処理
    CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::Play(LABEL label, float fPitch)
{
    XAUDIO2_VOICE_STATE xa2state;
    XAUDIO2_BUFFER buffer;

    // バッファの値設定

    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = m_aSizeAudio[label];
    buffer.pAudioData = m_apDataAudio[label];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = m_aParam[label].nCntLoop;

    // 状態取得
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        m_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_apSourceVoice[label]->FlushSourceBuffers();
    }

    // オーディオバッファの登録
    m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);
    m_apSourceVoice[label]->SetFrequencyRatio(fPitch);
    // 再生
    m_apSourceVoice[label]->Start(0);

    return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void CSound::Stop(LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 状態取得
    m_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        m_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_apSourceVoice[label]->FlushSourceBuffers();
    }
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void CSound::StopAll(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
    {
        if (m_apSourceVoice[nCntSound])
        {
            // 一時停止
            m_apSourceVoice[nCntSound]->Stop(0);
        }
    }
}

//=============================================================================
// チャンクのチェック
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
    {// ファイルポインタを先頭に移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    while (hr == S_OK)
    {
        if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクデータの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch (dwChunkType)
        {
        case 'FFIR':
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
            {// ファイルタイプの読み込み
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// ファイルポインタをチャンクデータ分移動
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
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
    DWORD dwRead;

    if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを指定位置まで移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
    {// データの読み込み
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

//=============================================================================
// 音量設定
//=============================================================================
void CSound::SetVolume(LABEL label, int nVolume)
{
    m_apSourceVoice[label]->SetVolume(float(nVolume) / DEFAULT_VOLUME);
}

//=============================================================================
// カテゴリごとの音量設定
//=============================================================================
void CSound::SetCategoryVolume(CATEGORY category, int nVolume)
{
    m_anVolume[category] = nVolume;
    //設定された種別の音声の音量を変更する
    for (int nCount = 0; nCount < LABEL_MAX; nCount++)
    {
        if (m_aParam[nCount].category == category)
        {
            SetVolume((LABEL)nCount, nVolume);
        }
    }
}