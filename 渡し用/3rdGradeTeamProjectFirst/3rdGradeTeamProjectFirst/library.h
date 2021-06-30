//===============================================
//
// 様々な処理をまとめたファイル [library.h]
// Author : 後藤慎之助
//
//===============================================
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

//========================
// インクルードファイル
//========================
#include "main.h"

//========================
// マクロ定義
//========================

// ゲーム内の移動範囲
#define GAME_LIMIT_WIDTH 1000.0f    // 2倍になる
#define GAME_LIMIT_HEIGHT 1000.0f   // 仮でわかりやすいように本家の2倍のサイズのステージ

//========================
// 列挙型定義
//========================
// ブロックに当たった時の面
typedef enum
{
    HIT_SURFACE_NONE = 0,     // 当たっていない
    HIT_SURFACE_RIGHT,        // 右
    HIT_SURFACE_LEFT,         // 左
    HIT_SURFACE_FRONT,        // 手前
    HIT_SURFACE_BACK,         // 奥
    HIT_SURFACE_TOP,          // 上面
    HIT_SURFACE_BOTTOM,       // 底
}HIT_SURFACE;

//========================
// 構造体定義
//========================
// ブロックに当たった時の情報
typedef struct
{
    HIT_SURFACE hitSurface;     // 当たった面
    D3DXVECTOR3 pos;            // 位置
}BLOCK_HIT_INFO;

//===========================================
// プロトタイプ宣言
//===========================================
bool IsOutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize);
bool IsOutGame2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize);
bool IsSteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pSize);
bool IsCollisionRectangle2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
bool IsCollisionRectangle3D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
BLOCK_HIT_INFO IsBlockCollision3D(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos1Old, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
bool IsCollisionCircle2D(const D3DXVECTOR3 pos1, const float fSize1, const D3DXVECTOR3 pos2, const float fSize2);
int GetRandNum(const int nMax, const int nMin);
float GetAngleToTarget2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos);
float GetAngleToTarget3D2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos);
float GetDistance2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos);
D3DXVECTOR3 caclRotate(const D3DXVECTOR3 p1, const D3DXVECTOR3 cp, float deg);
bool IsCollisionSide2D(const D3DXVECTOR3 r1, const D3DXVECTOR3 r2, const D3DXVECTOR3 p1, const D3DXVECTOR3 p2);
bool IsCollisionToRotationRect(const D3DXVECTOR3 myCenterPos, const D3DXVECTOR3 mySize, const D3DXVECTOR3 targetPos1, const D3DXVECTOR3 targetPos2, const D3DXVECTOR3 targetPos3, const D3DXVECTOR3 targetPos4);

#endif