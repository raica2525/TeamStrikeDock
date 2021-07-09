//====================================================================
//
// サウンド処理 (sound.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// クラス宣言
//================================================

// サウンドのクラス
class CSound
{
public:
    typedef enum
    {
        // SE
        LABEL_SE_HIT = 0,		// ヒット音
        LABEL_SE_SWISH,         // 素振り音
        LABEL_SE_BOUND,         // バウンド音
        LABEL_SE_CATCH,         // キャッチ音
        LABEL_SE_THROW,         // 投げ音
        LABEL_SE_KO,            // KO音
        LABEL_SE_BUNT,          // バント音
        LABEL_SE_DAMAGE,        // ダメージ音
        LABEL_SE_OFFSET,        // 相殺音
        LABEL_SE_HIT_BIG,       // 大ヒット音
        LABEL_SE_BALL_DISPING,  // ボール配置音
        LABEL_SE_JUMP,          // ジャンプ音
        LABEL_MAX,
    } LABEL;

    CSound();
    ~CSound();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT Play(LABEL label);
    void Stop(LABEL label);
    void StopAll(void);

private:
    typedef struct
    {
        char *pFilename;	// ファイル名
        int nCntLoop;		// ループカウント (-1でループ、0でループなし)
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
    BYTE *m_apDataAudio[LABEL_MAX];					    // オーディオデータ
    DWORD m_aSizeAudio[LABEL_MAX];					    // オーディオデータサイズ
    static PARAM m_aParam[LABEL_MAX];                   // 各音素材のパラメータ
};

#endif