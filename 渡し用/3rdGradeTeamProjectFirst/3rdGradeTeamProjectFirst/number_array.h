//====================================================================
//
// 数字配列処理 (number_array.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _NUMBER_ARRAY_H_
#define _NUMBER_ARRAY_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CNumber;

//================================================
// マクロ定義
//================================================
#define MAX_NUMBER_ARRAY 8  // 数字配列の桁数

// 各場面の数字の大きさ
#define NUMBER_SIZE_X_BALL_SPD 20.0f
#define NUMBER_SIZE_X_DAMAGE 17.5f

//================================================
// クラス宣言
//================================================

// 数字配列のクラス
class CNumberArray : public CScene
{
public:
    CNumberArray();
    ~CNumberArray();
    static CNumberArray *Create(int nTexType, D3DXVECTOR3 pos, float fSizeX, D3DXCOLOR col, int nDispNumber, bool bDispUselessDigits, bool bMove = false);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // 数字情報開放
    void Update(void);
    void Draw(void);
    void SetDispNumber(int nDispNumber, bool bMove = false);  // 表示する数字を設定

private:
    int m_nTexType;                         // 使うテクスチャ番号
    CNumber *m_apNumber[MAX_NUMBER_ARRAY];  // 数字情報

    D3DXVECTOR3 m_createPos;                // 生成位置
    D3DXCOLOR m_createColor;                // 生成色
    bool m_bDispUselessDigits;              // 不要な桁数も描画するかどうか
    float m_fSizeX;                         // 横の大きさ
};

#endif