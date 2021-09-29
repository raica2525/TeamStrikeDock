//====================================================================
//
// エフェクトデータ管理 (effectData.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _EFFECT_DATA_H_
#define _EFFECT_DATA_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define EFFECT_PI 314                   // 円の表現の際に使う値（これをエフェクトの浮動小数点で割る）
#define EFFECT_FLOATING_POINT 100.0f    // エフェクトの浮動小数点

// ブロック等に当たった際、板ポリのエフェクトを置くための回転量（ブロックの中から見ているので注意）
#define LAY_ROT_TOP D3DXVECTOR3(D3DXToRadian(-90.0f), 0.0f, 0.0f)    // 天井
#define LAY_ROT_LEFT D3DXVECTOR3(0.0f, D3DXToRadian(270.0f), 0.0f)  // 左壁
#define LAY_ROT_BOT D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f)    // 地面
#define LAY_ROT_RIGHT D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f)  // 右壁
#define LAY_ROT_BACK D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f)
#define LAY_ROT_FRONT D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(0.0001f))

#define MAX_EFFECT_TYPE 64

//================================================
// クラス宣言
//================================================

// エフェクトデータの管理クラス
class CEffectData
{
public:
    CEffectData();
    ~CEffectData();

    typedef enum
    {
        TYPE_HEAL = 0,              // 回復
        TYPE_BALL_AURA,             // ボールオーラ
        TYPE_BALL_APPEAR,           // ボール出現
        TYPE_NEGA,                  // 反転合成
        TYPE_WALK_SMOKE_RIGHT,      // 歩き始めの砂煙右
        TYPE_JUMP_SMOKE_RIGHT,      // ジャンプの砂煙右
        TYPE_GROUND_SMOKE,          // 着地時の砂煙
        TYPE_SECOND_JUMP_AIR_RING,  // 二段ジャンプの空気の輪
        TYPE_SECOND_JUMP_SMOKE,     // 二段ジャンプの砂煙
        TYPE_WALK_SMOKE_LEFT,       // 歩き始めの砂煙左
        TYPE_JUMP_SMOKE_LEFT,       // ジャンプの砂煙左
        TYPE_SHOOT_CIRCLE_1P,       // シュート時の円1P
        TYPE_SHOOT_CIRCLE_2P,       // シュート時の円2P
        TYPE_SHOOT_CIRCLE_3P,       // シュート時の円3P
        TYPE_SHOOT_CIRCLE_4P,       // シュート時の円4P
        TYPE_SHOOT_CIRCLE_1P_FAST,  // シュート時の円1P改
        TYPE_SHOOT_CIRCLE_2P_FAST,  // シュート時の円2P改
        TYPE_SHOOT_CIRCLE_3P_FAST,  // シュート時の円3P改
        TYPE_SHOOT_CIRCLE_4P_FAST,  // シュート時の円4P改
        TYPE_SHOOT_ABSORB,          // 吸収
        TYPE_HIT_BALL,              // ボールが当たった時
        TYPE_LUCKY_GUARD_FIRST,     // ラッキーガード時に最初に出るエフェクト
        TYPE_LUCKY_GUARD_SECOND,    // ラッキーガード時に2番目に出るエフェクト
        TYPE_TITLE_FIRE_RED,        // タイトルの赤い炎
        TYPE_TITLE_FIRE_BLUE,       // タイトルの青い炎
        TYPE_KIRA,                  // カーソルクリック時のキラキラ
        TYPE_SHADOW,                // 影
        TYPE_WARP_SPLASH,           // ワープ時の飛沫
        TYPE_WARP_SPLASH2,          // ワープ時の飛沫2
        TYPE_WARP_UP,               // ワープ上
        TYPE_WARP_DOWN,             // ワープ下
        TYPE_WARP_RIGHT,            // ワープ右
        TYPE_WARP_LEFT,             // ワープ左
        TYPE_DIVING,                // 急降下
        TYPE_BLOWN,                 // 吹っ飛ばされた時
        TYPE_RED_WING,              // 赤い翼
        TYPE_BLUE_WING,             // 青い翼
        TYPE_WALL_HIT_SMOKE,        // 壁バウンド煙
        TYPE_WALL_HIT_SHOCK,        // 壁バウンド衝撃波
        TYPE_ABSORB,                // 立ち上がり吸収
        TYPE_READY_TO_FIGHT1,       // 準備完了1
        TYPE_READY_TO_FIGHT2,       // 準備完了2
        TYPE_READY_TO_FIGHT3,       // 準備完了3
        TYPE_READY_TO_FIGHT4,       // 準備完了4
        TYPE_CHARGE_SWING_LEFT,     // チャージスイングエフェクト左
        TYPE_CHARGE_SWING_RIGHT,    // チャージスイングエフェクト右
        TYPE_CHARGE_LIGHT,          // チャージ時の光
    }TYPE;  // 生成するエフェクトの種類

    typedef enum
    {
        PLANE_XY = 0,   // XY平面
        PLANE_XZ,       // XZ平面
    }PLANE;  // 平面の情報

    typedef struct
    {
        int nTexType;                 // 使うテクスチャの種類
        int nNumOnce;                 // 同時発生数
        int plane;                    // 平面

        bool bOnCircleEdge;           // 円の縁から放出するかどうか
        bool bEmitVectorInside;       // 放出方向を内側にするかどうか
        int nScatterAngle;            // 飛散角度（0～314）
        int nScatterWidth;            // 飛散幅（0～314）
        int nSpeedMax;                // 最大速度
        int nSpeedMin;                // 最小速度
        int nAuxiliaryAxisSpeedMax;   // 補助軸の最大速度（2D→3Dで、立体感を出すためのもの）
        int nAuxiliaryAxisSpeedMin;   // 補助軸の最小速度（2D→3Dで、立体感を出すためのもの）

        float fAcceleration;          // 加速度（1.0基準）
        float fGravity;               // 重力
        float fGravityLimit;          // 重力制限

        int nLayRot;                  // ポリゴンそのものを横にするかどうか（1下、2左、3上、4右）
        bool bRotEqualMoveAngle;      // 回転を移動の向きに合わせるか
        int nRotSpeedMax;             // 最大回転速度
        int nRotSpeedMin;             // 最小回転速度

        D3DXCOLOR col;                // 色
        D3DXCOLOR colChangeRate;      // 色変化割合
        float fFadeOutRate;           // α値を下げていく割合（色変化割合でも可能だが、明示的に用意した）
        int nCntFadeOutLock;          // α値をロックするカウンタ

        int nSizeMax;                 // 最大大きさ
        int nSizeMin;                 // 最小大きさ
        D3DXVECTOR3 sizeChangeRate;   // 拡大/縮小割合
        int nCntSizeChangeLock;       // 大きさをロックするカウンタ
        bool bSizeChangeStartOrFinish;// 大きさを変えるのが始まるか、終わるのか
        float fEmitRadius;            // 放出半径

        bool bUseAdditiveSynthesis;   // 加算合成にするかどうか
        bool bUseZBuffer;             // Zバッファをつかうかどうか
        int nAlphaTestBorder;         // αテストのボーダー
    }CreateInfo;    // 生成情報

    HRESULT Init(void);
    void Uninit(void);

    CreateInfo* GetCreateInfo(const int nNum);

private:
    CreateInfo m_aCreateInfo[MAX_EFFECT_TYPE];    // 生成情報
};

#endif