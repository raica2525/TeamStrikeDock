//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	text.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _TEXT_H_
#define _TEXT_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CText : public CScene
{
public:
    //列挙型宣言
    typedef enum
    {
        ALIGN_LEFT = 0,
        ALIGN_CENTER,
        ALIGN_RIGHT
    }ALIGN;

    CText();	//コンストラクタ
    ~CText();	//デストラクタ
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	//初期化処理
    void Uninit(void);	//終了処理
    void Update(void);	//更新処理
    void Draw(void);	//描画処理
    static CText* Create(D3DXVECTOR3 pos, int nSize, char* pStr, ALIGN align, char* pFontName, D3DCOLOR col, int nLimWidth = 0);	//生成処理
    static void Load(void);	//フォントファイルの読み込み
    static void Unload(void);	//フォントデータの破棄

    void SetText(char* pStr);	//テキスト変更関数

private:
    LPD3DXFONT m_pFont;	//フォント保存用変数
    D3DXVECTOR3 m_pos;	//座標
    int m_nSize;	//文字のサイズ
    int m_nLimWidth;	//文字列の幅の上限
    D3DCOLOR m_col;	//色
    ALIGN m_align;	//文字の整列の方法
    DWORD m_format;	//整列方法変換用変数
    char m_fontName[128];	//フォントの名前
    char m_str[512];	//表示する文字列
};
#endif