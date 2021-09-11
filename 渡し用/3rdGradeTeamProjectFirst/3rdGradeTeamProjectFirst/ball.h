//====================================================================
//
// ボールの処理 (ball.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BALL_H_
#define _BALL_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// 前方宣言
//================================================
class CPlayer;
class COutline;

//================================================
// マクロ定義
//================================================

// 基本情報
#define BALL_COLLISION_SIZE_SIDE 60.0f
#define BALL_COLLISION_SIZE D3DXVECTOR3(BALL_COLLISION_SIZE_SIDE, BALL_COLLISION_SIZE_SIDE, BALL_COLLISION_SIZE_SIDE)
#define BALL_COLLISION_POS_SLIDE_Y -30.0f
#define BALL_COLLISION_POS_SLIDE D3DXVECTOR3(0.0f, BALL_COLLISION_POS_SLIDE_Y, 0.0f)
#define BALL_ROT_SPEED D3DXToRadian(10.0f)        // 回転速度
#define BALL_FIRST_MOVE_ANGLE D3DXVECTOR3(sinf(D3DXToRadian(180.0f)), cosf(D3DXToRadian(180.0f)), 0.0f)
#define BALL_FIRST_SPEED 16.0f                    // 開始時の速さ
#define BALL_MAX_SPEED 999.0f                     // ボールの最大速度
#define BALL_START_BUTTLE_STOP_POS_Y 180.0f       // 開始時にどの高さで止めるか
#define BALL_DISPING_TIME 170                     // 開始時の配置時間
#define BALL_DISPING_SCALE_UP_RATE 0.05f          // 開始時のスケール拡大割合
#define BALL_WAVE_FIRST_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)
#define BALL_MOVE_Y_DONT_MOVE_BORDER 1.0f         // 動いていないとみなす縦軸の値（ボールを変形させる際に使う）
#define BALL_OUTLINE_RATE 1.2f                    // アウトラインの拡大割合

// 攻撃周り
#define BALL_SHOOT_STOP_LEAST_FRAME 8        // 最小硬直時間（要調整）
#define BALL_SHOOT_STOP_REFRECT_FRAME 2      // 壁に反射してるときに、止まるフレーム数
#define BALL_SHOOT_STOP_LUCKY_GUARD_FRAME 30 // ラッキーガード時に、止まるフレーム数
#define BALL_SHOOT_STOP_TIME_RATE 0.5f       // 現在のスピードにかけて硬直時間を出すための割合
#define BALL_SHOOT_BIG_HIT_SPEED 50.0f       // 大ヒットスピード（硬直時間が始まる）
#define BALL_SHOOT_ULTRA_HIT_SPEED 100.0f    // ウルトラヒットスピード（反転合成が始まる）
#define BALL_SHOOT_MAX_STOP_TIME 150         // 最大硬直時間
#define BALL_QUICK_SHOOT_MAX_STOP_TIME 15    // 早撃ちの最大硬直時間
#define BALL_NOT_ANYONE -1                   // ボールが誰のものでもない状態
#define BALL_THROW_FIRST_SPEED 100.0f        // 投げの初速
#define BALL_BUNT_SPEED 42.5f                // バントボールの速度
#define BALL_BUNT_GRAVITY 1.5f               // バントボールの重力
#define BALL_BUNT_MAX_FALL_SPEED -25.0f      // バントボールの最大落下速度
#define BALL_ABSORB_POS_X 100.0f             // 吸収したボールをどれだけ前に出すか
#define BALL_ABSORB_SPEED 20.0f              // 吸収速度
#define BALL_ABSORB_CLOSE_PLAYER 150.0f      // 吸収でプレイヤーに近いとみなす値
#define BALL_ABSORB_MAX_TIME 300             // 吸収時間の最大カウンタ

//================================================
// クラス宣言
//================================================

// ボールクラス
class CBall : public CScene3D
{
public:

    // シュートのフラグ
    typedef enum
    {
        SHOOT_FLAG_NONE = 0,                  // なし
        SHOOT_FLAG_QUICK = 0x001 << 0,        // 即打ち
        SHOOT_FLAG_THROW = 0x001 << 1,        // 投げ
        SHOOT_FLAG_BUNT = 0x001 << 2,         // バント
        SHOOT_FLAG_FAR_HIT = 0x001 << 3,      // ファーヒット
        SHOOT_FLAG_ABSORB = 0x001 << 4,       // 吸収
    }SHOOT_FLAG;

    // 壁に当たった時の向き
    typedef enum
    {
        HIT_WALL_NONE = 0,
        HIT_WALL_UP,
        HIT_WALL_DOWN,
        HIT_WALL_RIGHT,
        HIT_WALL_LEFT
    }HIT_WALL;

    // 吸収のフラグ
    typedef enum
    {
        ABSORB_FLAG_NONE = 0,                  // なし
        ABSORB_FLAG_MOVING = 0x001 << 0,       // 移動中
        ABSORB_FLAG_REACHED = 0x001 << 1,      // 到着後
    }ABSORB_FLAG;

    CBall();
    ~CBall();
    void ResetMenberVariables(void);                                // メンバ変数のリセット
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Reset(D3DXVECTOR3 pos);                                    // ボールの再設定
    void Uninit(void);
    void Update(void);
    void UpdateCornerPos(D3DXVECTOR3 pos);                          // 斜め当たり判定用の、四つ角更新
    void AuraEffect(void);                                          // ボールのオーラエフェクト
    void AppearEffect(void);                                        // ボールの出現エフェクト
    void ChangeScaleAndRot(void);                                   // スケールと回転の変更
    void Draw(void);
    static CBall *Create(D3DXVECTOR3 pos, bool bUseCollision);
    D3DXVECTOR3 StartButtleMove(D3DXVECTOR3 pos);                   // バトル開始時の移動（最初の落下or負けた人への追従）
    D3DXVECTOR3 Move(D3DXVECTOR3 pos);                              // 移動
    D3DXVECTOR3 Absorb(void);                                       // 吸収
    D3DXVECTOR3 HitWall(D3DXVECTOR3 pos, HIT_WALL hitWall);         // 壁に当たった時の共通処理
    D3DXVECTOR3 StayInGame(D3DXVECTOR3 pos);                        // ゲーム内にとどめる
    void Throw(void);                                               // 投げられ中

    int Shoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, CPlayer*pPlayer);    // 打たれる時の処理
    void ResetByShoot(void);                                        // シュートによってリセットする変数
    int Launch(D3DXVECTOR3 startPos, int nCntStopTime = 0);         // ボールを打ち上げる

    /*========================================================
    // セッター
    //======================================================*/
    void SetStopTime(const int nCntStopTime);   // 硬直時間を決める
    void SetAbsorb(CPlayer *pPlayer);           // 吸収を設定
    void SetMoveAngle(D3DXVECTOR3 moveAngle) { m_moveAngle = moveAngle; }
    void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
    void SetStartButtle(bool bStartButtle) { m_bStartButtle = bStartButtle; }
    void SetWhoShooting(int nWhoShooting) { m_nWhoShooting = nWhoShooting; }
    void SetHitWallMax(int nHitWallMax) { m_nCntHitWallMax = nHitWallMax; }
    void SetWallThrough(int nWallThrough) { m_bWallThrough = true; m_nCntWallThrough = nWallThrough; }
    void SetDispOff(void) { m_bDisp = false;  m_nCntDispingTime = 0; m_bUseCollision = false; }

    /*========================================================
    // ゲッター
    //======================================================*/
    bool GetUseCollision(void) { return m_bUseCollision; }
    float GetSpeed(void) { return m_fSpeed; }
    int GetWhoShooting(void) { return m_nWhoShooting; }
    D3DXVECTOR3 GetCornerPos(int nCnt) { return m_aCornerPos[nCnt]; }
    int GetAbsorbFlag(void) { return m_absorbFlag; }
    int GetStopTime(void) { return m_nCntStopTime; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
    int GetWhoAbsorbing(void) { return m_nWhoAbsorbing; }
    D3DXVECTOR3 GetCollisionPos(void) { return GetPos() + BALL_COLLISION_POS_SLIDE; }

private:
    COutline *m_pOutline;       // アウトライン
    int m_shootFlag;            // シュートのフラグ
    D3DXVECTOR3 m_moveAngle;    // 移動する角度
    float m_fSpeed;             // 速さ
    
    int m_nCntStopTime;         // 硬直時間のカウンタ
    bool m_bUseCollision;       // 当たり判定を使うかどうか
    bool m_bNeverUseCollision;  // 当たり判定を絶対に使わないかどうか
    bool m_bStartButtle;        // バトル開始直後かどうか

    int m_nWhoShooting;         // 誰が打ったボールか
    CPlayer *m_pPlayer;         // プレイヤーのポインタ

    bool m_bAfterThrow;         // 投げ出された直後か
    int m_nCntBuntTime;         // バント時間のカウンタ

    D3DXVECTOR3 m_aCornerPos[4];// 四つ角の位置
    D3DXVECTOR3 m_posOld;       // 1F前の位置

    int m_absorbFlag;           // 吸収のフラグ
    int m_nWhoAbsorbing;        // 誰が現在吸収しているか
    int m_nCntAbsorbTime;       // 吸収時間のカウンタ

    bool m_bDisp;               // 表示しているかどうか
    int m_nCntDispingTime;      // 配置中のカウンタ
    int m_nCntNoLineStopTime;   // 死亡や相殺、ラッキーガードの硬直時間をカウント（集中線を止める用）

    //===================================
    // 必殺技対応周り
    //===================================
    int m_nCntHitWall;          // 壁に当たった回数
    int m_nCntHitWallMax;       // 壁に当たっていい最大数

    bool m_bWallThrough;        // 壁を貫通するかどうか
    int m_nCntWallThrough;      // あと何回壁を貫通するかどうか
};

#endif