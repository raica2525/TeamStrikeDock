//====================================================================
//
// サウンド処理 (sound.h)
// Author : 池田悠希
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
        LABEL_SE_HIT_ULTRA,     // ウルトラヒット音
		LABEL_SE_SELECT,		// 決定音
		LABEL_SE_CUSTOM,		// カスタマイズ変更音
		LABEL_SE_CANCEL,		// キャンセル音
		LABEL_SE_ENTRY,			// エントリー音
		LABEL_SE_READY,			// READYボタン出現音
		LABEL_SE_HEAL,			// 回復音
		LABEL_SE_INFO,			// 詳細が出る時の音
		LABEL_SE_CURSOLON,		// ボタンにカーソルが乗った時の音
		LABEL_SE_WARP,			// ボールワープ音
		LABEL_SE_SKILL,			// 必殺技発動音
		LABEL_BGM_TITLE,		// タイトルBGM
		LABEL_BGM_MENU,			// メニュー&カスタマイズBGM
		LABEL_BGM_BATTLE00,		// バトルBGM00
		LABEL_BGM_BATTLE01,		// バトルBGM01
		LABEL_BGM_BATTLE02,		// バトルBGM02
		LABEL_VOICE_WIN_ICARUS,	// 勝利ボイス
		LABEL_VOICE_WIN_KLEINOD,
		LABEL_VOICE_WIN_KNIGHT,
		LABEL_VOICE_WIN_OOO,
		LABEL_VOICE_WIN_RANGER,
		LABEL_VOICE_WIN_X,
		LABEL_VOICE_SP_ICARUS,	// 必殺技ボイス
		LABEL_VOICE_SP_KLEINOD,
		LABEL_VOICE_SP_KNIGHT,
		LABEL_VOICE_SP_OOO,
		LABEL_VOICE_SP_RANGER,
		LABEL_VOICE_SP_X,
        LABEL_SE_WING,
        LABEL_SE_OK,
        LABEL_BGM_RESULT,		// リザルトBGM
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
    void SetVolume(LABEL label, int nVolume);				// 音量設定用関数
    void SetCategoryVolume(CATEGORY category, int nVolume); // カテゴリごとの音量設定用関数
private:
    typedef struct
    {
        char *pFilename;	// ファイル名
        int nCntLoop;		// ループカウント (-1でループ、0でループなし)
        CATEGORY category;	// 種類分け
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
    BYTE *m_apDataAudio[LABEL_MAX];					    // オーディオデータ
    DWORD m_aSizeAudio[LABEL_MAX];					    // オーディオデータサイズ
    static PARAM m_aParam[LABEL_MAX];                   // 各音素材のパラメータ
    int m_anVolume[CATEGORY_MAX];						// 各種別の音量
};

#endif