//=============================================================================
//
// サウンド処理 [sound.h]
// Author : HOGE FUGA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#include <xaudio2.h>

class CSound
{
public:
	CSound();
	~CSound();

	typedef enum
	{
		SE_SHOT = 0,		// 弾発射音
		SE_EXPLOSION,			// ヒット
		MAX,
	} LABEL;

	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;

	// 各音素材のパラメータ
	static PARAM g_aParam[MAX];

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[MAX];	// ソースボイス
	BYTE *m_apDataAudio[MAX];					// オーディオデータ
	DWORD m_aSizeAudio[MAX];					// オーディオデータサイズ
};


#endif
