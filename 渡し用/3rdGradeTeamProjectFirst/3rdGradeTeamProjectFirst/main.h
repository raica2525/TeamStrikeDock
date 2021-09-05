//=============================================================================
//
// メイン処理 [main.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <WinSock2.h>
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xaudio2.h>
#include <thread>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")    // 描画処理に必要
#pragma comment(lib, "d3dx9.lib")   // 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")  // コンポーネント使用に必要
#pragma comment(lib,"dinput8.lib")  // 入力処理に必要
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// ウインドウの位置
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)

// 画面の幅
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

// 頂点とポリゴン
#define NUM_VERTEX	(4)
#define NUM_POLYGON	(2)

// テクスチャブレンドの最大数
#define MAX_BREND_TEXTURE (4)

// 頂点フォーマット		(頂点座標|頂点カラー|テクスチャ座標 (* テクスチャブレンドの最大数))
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 * MAX_BREND_TEXTURE)
#define FVF_VERTEX_3D   (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// デフォルト設定
#define DEFAULT_COLOR (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define DEFAULT_COLOR_NONE (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define DEFAULT_VECTOR (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define DEFAULT_SCALE (D3DXVECTOR3(1.0f,1.0f,1.0f))
#define DEFAULT_NOR (D3DXVECTOR3(0.0f,1.0f,0.0f))
#define DEFAULT_ALPHATEST_BORDER_2D 50
#define DEFAULT_ALPHATEST_BORDER_3D 100

// プレイヤーの番号
#define MAX_PLAYER 4     // つなぐコントローラーの最大数
#define PLAYER_1 (0)     // プレイヤーのコントローラー番号 (1P)
#define PLAYER_2 (1)     // プレイヤーのコントローラー番号 (2P)
#define PLAYER_3 (2)     // プレイヤーのコントローラー番号 (3P)
#define PLAYER_4 (3)     // プレイヤーのコントローラー番号 (4P)

// 各プレイヤーの色
#define PLAYER_COLOR_1 D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define PLAYER_COLOR_2 D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)
#define PLAYER_COLOR_3 D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)
#define PLAYER_COLOR_4 D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)

// ビット演算
#define BITON(flag,bit) (flag)=(flag)|(bit)
#define BITOFF(flag,bit) (flag)=(flag)&(~(bit))
#define IS_BITON(flag,bit) ((flag)&(bit))
#define IS_BITOFF(flag,bit) ((~(flag)&(bit)))

// メニュー周り
#define MENU_SELECT_COOL_TIME 12    // 選択のクールタイム

// 0で初期化したくないとき用
#define NOT_EXIST -1

//*****************************************************************************
// 構造体定義
//*****************************************************************************

typedef struct
{
    D3DXVECTOR3 pos;
    float rhw;
    D3DCOLOR col;
    D3DXVECTOR2 tex[MAX_BREND_TEXTURE];
}VERTEX_2D;

typedef struct
{
    D3DXVECTOR3 pos;
    D3DXVECTOR3 nor;
    D3DCOLOR    col;
    D3DXVECTOR2 tex;
}VERTEX_3D;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#ifdef _DEBUG
int GetFPS(void);    // FPS取得
#endif

#endif