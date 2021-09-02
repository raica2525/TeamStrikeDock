//===============================================
//
// 様々な処理をまとめたファイル (library.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "library.h"
#include "manager.h"
#include "camera.h"

//===========================================
// 画面外かどうか(二次元)
// Author : 後藤慎之助
//===========================================
bool IsOutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize)
{
    // 変数宣言
    bool bOutScreen = false;    // 画面外かどうか

                                // 画面の端から出ているかどうか
    if (pPos->x < -(pSize->x / 2) ||
        pPos->x > SCREEN_WIDTH + pSize->x / 2 ||
        pPos->y < -(pSize->y / 2) ||
        pPos->y > SCREEN_HEIGHT + pSize->y / 2)
    {
        // 画面外のフラグをtrueに
        bOutScreen = true;
    }

    return bOutScreen;
}

//===========================================
// ゲーム外かどうか(二次元)
// Author : 後藤慎之助
//===========================================
bool IsOutGame2D(const D3DXVECTOR3 * pPos, const D3DXVECTOR3 * pSize)
{
    // 変数宣言
    bool bOutGame = false;    // ゲーム外かどうか

                              // 画面の端から2倍のところから出ているかどうか
    if (pPos->x < -SCREEN_WIDTH - (pSize->x / 2) ||
        pPos->x >(SCREEN_WIDTH * 2) + pSize->x / 2 ||
        pPos->y < -SCREEN_HEIGHT - (pSize->y / 2) ||
        pPos->y >(SCREEN_HEIGHT * 2) + pSize->y / 2)
    {
        // ゲーム外のフラグをtrueに
        bOutGame = true;
    }

    return bOutGame;
}

//===========================================
// 画面内にとどまらせる(二次元)
// Author : 後藤慎之助
//===========================================
bool IsSteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pSize)
{
    // 変数宣言
    bool bScreenEdge = false;    // 画面端かどうか

                                 // 画面の左
    if (pPos->x < pSize->x / 2)
    {
        // とどまらせる
        pPos->x = pSize->x / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の右
    if (pPos->x > SCREEN_WIDTH - pSize->x / 2)
    {
        // とどまらせる
        pPos->x = SCREEN_WIDTH - pSize->x / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の上
    if (pPos->y < pSize->y / 2)
    {
        // とどまらせる
        pPos->y = pSize->y / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の下
    if (pPos->y > SCREEN_HEIGHT - pSize->y / 2)
    {
        // とどまらせる
        pPos->y = SCREEN_HEIGHT - pSize->y / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    return bScreenEdge;
}

//===========================================
// 矩形の当たり判定2D
// Author : 後藤慎之助
//===========================================
bool IsCollisionRectangle2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // 変数宣言
    bool bHit = false;    // 当たったかどうか

                          // 二つのものの当たり判定を計算
    if (pPos1->x - pSize1->x / 2 <= pPos2->x + pSize2->x / 2 &&
        pPos1->x + pSize1->x / 2 >= pPos2->x - pSize2->x / 2 &&
        pPos1->y - pSize1->y / 2 <= pPos2->y + pSize2->y / 2 &&
        pPos1->y + pSize1->y / 2 >= pPos2->y - pSize2->y / 2)
    {
        // 当たったフラグをtrueに
        bHit = true;
    }

    return bHit;
}

//===========================================
// 矩形の当たり判定3D
// Author : 後藤慎之助
//===========================================
bool IsCollisionRectangle3D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // 変数宣言
    bool bHit = false;  // 当たったかどうか

    D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;    //ぶつかる側の最大値
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, 0.0f, -pSize1->z / 2) + *pPos1;       //ぶつかる側の最小値
    D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;    //ぶつかられる側の最大値
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;       //ぶつかられる側の最小値

                                                                                            // 当たり判定を計算
    if (box1Max.y > box2Min.y&&
        box1Min.y < box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        bHit = true;
    }

    return bHit;
}

//===========================================
// ブロックの当たり判定3D
// Author : 後藤慎之助
//===========================================
BLOCK_HIT_INFO IsBlockCollision3D(D3DXVECTOR3 * pPos1, D3DXVECTOR3 * pPos1Old, const D3DXVECTOR3 * pPos2, const D3DXVECTOR3 * pSize1, const D3DXVECTOR3 * pSize2)
{
    // ブロックに当たった情報
    BLOCK_HIT_INFO blockHitInfo;
    blockHitInfo.hitSurface = HIT_SURFACE_NONE;
    blockHitInfo.pos = *pPos1;

    D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;      //ぶつかる側の最大値
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, 0.0f, -pSize1->z / 2) + *pPos1;         //ぶつかる側の最小値
    D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;      //ぶつかられる側の最大値
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;         //ぶつかられる側の最小値

                                                                                              // 当たり判定を計算
    if (box1Max.y > box2Min.y&&
        box1Min.y <= box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        if (box1Max.y > box2Min.y&&
            pPos1Old->y + pSize1->y / 2 < box2Min.y)
        {// Y軸の下
            blockHitInfo.hitSurface = HIT_SURFACE_BOTTOM;
            blockHitInfo.pos.y = box2Min.y - pSize1->y;
        }
        if (box1Min.y <= box2Max.y&&
            pPos1Old->y >= box2Max.y)
        {// Y軸の上
            blockHitInfo.hitSurface = HIT_SURFACE_TOP;
            blockHitInfo.pos.y = box2Max.y;
        }
        if (box1Max.x > box2Min.x&&
            pPos1Old->x + pSize1->x / 2 <= box2Min.x)
        {// X軸の左
            blockHitInfo.hitSurface = HIT_SURFACE_LEFT;
            blockHitInfo.pos.x = box2Min.x - pSize1->x / 2;
        }
        if (box1Min.x < box2Max.x&&
            pPos1Old->x - pSize1->x / 2 >= box2Max.x)
        {// X軸の右
            blockHitInfo.hitSurface = HIT_SURFACE_RIGHT;
            blockHitInfo.pos.x = box2Max.x + pSize1->x / 2;
        }
        if (box1Max.z > box2Min.z&&
            pPos1Old->z + pSize1->z / 2 <= box2Min.z)
        {// Z軸の手前
            blockHitInfo.hitSurface = HIT_SURFACE_FRONT;
            blockHitInfo.pos.z = box2Min.z - pSize1->z / 2;
        }
        if (box1Min.z < box2Max.z&&
            pPos1Old->z - pSize1->z / 2 >= box2Max.z)
        {// Z軸の奥
            blockHitInfo.hitSurface = HIT_SURFACE_BACK;
            blockHitInfo.pos.z = box2Max.z + pSize1->z / 2;
        }
    }

    return blockHitInfo;
}

//=============================================================================
// 円の当たり判定2D
// Author : 後藤慎之助
//=============================================================================
bool IsCollisionCircle2D(const D3DXVECTOR3 pos1, const float fSize1, const D3DXVECTOR3 pos2, const float fSize2)
{
    // 距離を求める
    float fDistance = sqrtf(powf(pos1.x - pos2.x, 2.0f) + powf(pos1.y - pos2.y, 2.0f));

    // 2つの半径を足したものを求める
    float fSumRadius = (fSize1 + fSize2) / 2;

    return (fDistance <= fSumRadius);
}

//===========================================
// 範囲内で、ランダムな数字を得る（+と-をまたぐのには非対応）
// Author : 後藤慎之助
//===========================================
int GetRandNum(const int nMax, const int nMin)
{
    int nMaxNum = nMax;
    int nMinNum = nMin;
    bool bReverse = false;

    // 最大がマイナスなら
    if (nMax < 0)
    {
        nMaxNum = (int)fabs(nMin);
        nMinNum = (int)fabs(nMax);
        bReverse = true;
    }

    int nRandNum = rand() % (nMaxNum - nMinNum + 1) + nMinNum;

    if (bReverse)
    {
        nRandNum *= -1;
    }

    return nRandNum;
}

//===========================================
// 対象への角度を得る2D
// Author : 後藤慎之助
//===========================================
float GetAngleToTarget2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos)
{
    // 変数宣言
    float fAngle = 0.0f;                    // 返す角度

    // 角度を求める
    fAngle = atan2((targetPos.y - myPos.y), (targetPos.x - myPos.x));

    return fAngle;
}

//===========================================
// 対象への角度を得る2.5D
// Author : 後藤慎之助
//===========================================
float GetAngleToTarget3D2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos)
{
    // 変数宣言
    float fAngle = 0.0f;                    // 返す角度

    // 角度を求める
    fAngle = atan2((targetPos.x - myPos.x), (targetPos.y - myPos.y));

    return fAngle;
}

//===========================================
// 対象への距離を得る2D
// Author : 後藤慎之助
//===========================================
float GetDistance2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos)
{
    float fDistance = 0.0f;

    // 距離を求める
    fDistance = sqrtf(
        powf((myPos.x - targetPos.x), 2) +
        powf((myPos.y - targetPos.y), 2));

    return fDistance;
}

//===========================================
// 加法定理回転座標計算関数
//-------------------------------------------------
// 回転する点座標 p1
// 回転中央座標   cp
// 回転角度       deg
//===========================================
D3DXVECTOR3 caclRotate(const D3DXVECTOR3 p1, const D3DXVECTOR3 cp, float deg) 
{
    float  cx, cy;

    cx = (cp.x - p1.x)*cos(D3DXToRadian(deg)) - (cp.y - p1.y)*sin(D3DXToRadian(deg)) + cp.x;
    cy = (cp.x - p1.x)*sin(D3DXToRadian(deg)) + (cp.y - p1.y)*cos(D3DXToRadian(deg)) + cp.y;

    return D3DXVECTOR3(cx, cy, 0.0f);
}

//===========================================
// 衝突判定関数（外角）
//-------------------------------------------
// 直線 r1-r2と、辺p1-p2がクロスするか判定する
// クロスする場合は true. しない場合は false.
// http://mslabo.sakura.ne.jp/WordPress/make/processing%E3%80%80%E9%80%86%E5%BC%95%E3%81%8D%E3%83%AA%E3%83%95%E3%82%A1%E3%83%AC%E3%83%B3%E3%82%B9/%E5%A4%9A%E8%A7%92%E5%BD%A2%E5%90%8C%E5%A3%AB%E3%81%AE%E5%BD%93%E3%81%9F%E3%82%8A%E5%88%A4%E5%AE%9A%E3%82%92%E8%A1%8C%E3%81%86%E3%81%AB%E3%81%AF/
//===========================================
bool IsCollisionSide2D(const D3DXVECTOR3 r1, const D3DXVECTOR3 r2, const D3DXVECTOR3 p1, const D3DXVECTOR3 p2)
{
    float t1, t2;

    // 2点r1-r2を通る直線に対し、p1、p2がどの位置にあるか計算する
    t1 = (r1.x - r2.x)*(p1.y - r1.y) + (r1.y - r2.y)*(r1.x - p1.x);
    t2 = (r1.x - r2.x)*(p2.y - r1.y) + (r1.y - r2.y)*(r1.x - p2.x);

    // それぞれの正負が異なる（積が負になる）ならクロスしている（点を考慮すると、全く同じ向きで並んだ時にtrueを通るので削除）
    if (t1*t2 < 0) 
    {
        return(true); // クロスしている
    }

    return(false); // クロスしていない
}

//===========================================
// 回転する矩形への当たり判定（回転しないものから、回転する矩形へ）
// Author : 後藤慎之助
//===========================================
bool IsCollisionToRotationRect(const D3DXVECTOR3 myCenter, const D3DXVECTOR3 mySize, const D3DXVECTOR3 targetPos1, const D3DXVECTOR3 targetPos2, const D3DXVECTOR3 targetPos3, const D3DXVECTOR3 targetPos4)
{
    // 自分側の四つ角の位置を出す(左上、左下、右下、右上の順)
    D3DXVECTOR3 myPos1 = myCenter + D3DXVECTOR3(-mySize.x / 2, mySize.y, 0.0f);
    D3DXVECTOR3 myPos2 = myCenter + D3DXVECTOR3(-mySize.x / 2, 0.0f, 0.0f);
    D3DXVECTOR3 myPos3 = myCenter + D3DXVECTOR3(mySize.x / 2, 0.0f, 0.0f);
    D3DXVECTOR3 myPos4 = myCenter + D3DXVECTOR3(mySize.x / 2, mySize.y, 0.0f);

    // 衝突判定を行う
    // 自分四角辺 [0]-[1] と対象四角辺 [0]-[1]、[1]-[2]、[2]-[3]、[3]-[0]
    // 自分四角辺 [1]-[2] と対象四角辺 [0]-[1]、[1]-[2]、[2]-[3]、[3]-[0]
    // 自分四角辺 [2]-[3] と対象四角辺 [0]-[1]、[1]-[2]、[2]-[3]、[3]-[0]
    // 自分四角辺 [3]-[0] と対象四角辺 [0]-[1]、[1]-[2]、[2]-[3]、[3]-[0] 
    // をクロス判定する

    // 変数宣言
    bool bCross1 = false;
    bool bCross2 = false;

    // 計算しやすいように一度配列に確保
    D3DXVECTOR3 myRect[4] = { myPos1 ,myPos2 ,myPos3 ,myPos4 };
    D3DXVECTOR3 targetRect[4] = { targetPos1 ,targetPos2 ,targetPos3 ,targetPos4 };

    // i は白四角の4辺のインデックス
    for (int i = 0; i < 4; i++) 
    {
        // 座標配列が3-4の比較なら、3-0の比較に置き換える
        int toi;
        toi = i + 1;
        if (toi > 3) toi = 0;

        // j は青四角の4辺のインデックス
        for (int j = 0; j < 4; j++) 
        {
            // 座標配列が3-4の比較なら、3-0の比較に置き換える
            int toj;
            toj = j + 1;
            if (toj > 3) toj = 0;

            // クロス判定
            bCross1 = IsCollisionSide2D(myRect[i], myRect[toi],
                targetRect[j], targetRect[toj]);

            bCross2 = IsCollisionSide2D(targetRect[j], targetRect[toj],
                myRect[i], myRect[toi]);

            // どちらもクロスしているなら当たっている
            if (bCross1 && bCross2)
            {
                return true;
            }
        }
    }

    return false;
}

//===========================================
// スクリーン座標への変換
// Author : 後藤慎之助
//===========================================
D3DXVECTOR3 ConvertScreenPos(const D3DXVECTOR3 convertPos)
{
    // カメラを取得
    CCamera* pCamera = CManager::GetCamera();

    // 4次元での位置を定義
    D3DXVECTOR4* pPos = &D3DXVECTOR4(convertPos.x, convertPos.y, convertPos.z, 1.0f);

    // ビュー変換
    D3DXVECTOR4 viewPosition = *D3DXVec4Transform(pPos, pPos, &pCamera->GetViewMtx());

    // プロジェクション変換
    D3DXVECTOR4 projectionPosition = *D3DXVec4Transform(&viewPosition, &viewPosition, &pCamera->GetProjectionMtx());

    // 同次座標を考慮
    projectionPosition /= projectionPosition.w;

    // スクリーン変換
    D3DXVECTOR3 screenPos = D3DXVECTOR3(
        (1.0f + projectionPosition.x) / 2.0f * SCREEN_WIDTH,
        (1.0f - projectionPosition.y) / 2.0f * SCREEN_HEIGHT,
        0.0f);

    return screenPos;
}