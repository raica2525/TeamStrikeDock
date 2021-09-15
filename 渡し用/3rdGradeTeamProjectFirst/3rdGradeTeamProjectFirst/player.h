//======================================================================================
//
// プレイヤー処理 (player.h)
// Author : 後藤慎之助
//
//======================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "character.h"
#include "model.h"
#include "cliping_musk.h"

//================================================
// マクロ定義
//================================================

// プレイヤーの移動周り
#define PLAYER_ROT_RIGHT D3DXToRadian(270.0f)
#define PLAYER_ROT_LEFT D3DXToRadian(90.0f)
#define PLAYER_BASE_MINIMUM_SPEED 7.25f                     // 基本最小速度
#define PLAYER_ADD_BASE_SPEED_RATE 0.01f                    // 基本速度に足す割合
#define PLAYER_BASE_MINIMUM_GRAVITY 0.7f                    // 基本最小重力
#define PLAYER_ADD_BASE_GRAVITY_RATE 0.0008f                // 基本重力に足す割合
#define PLAYER_DIVE_SPEED_RATE 2.0f                         // 急降下の速度上昇割合
#define PLAYER_JUMP_FIRST_RATE 23.5f                        // ジャンプの最初の強さ割合
#define PLAYER_NEXT_JUMP_DOWN_RATE 0.8f                     // 次のジャンプがどれほど下がるかの割合
#define PLAYER_JUMP_ADD_FRAME 14                            // ジャンプを加算できるフレーム数
#define PLAYER_JUMP_KEEP_FRAME (8 + PLAYER_JUMP_ADD_FRAME)  // ジャンプをキープできるフレーム数
#define PLAYER_FIRST_JUMP_MOTION_TO_GLIDE 25                // 最初のジャンプモーションが滑空になるまでのフレーム数
#define PLAYER_NEXT_JUMP_MOTION_TO_GLIDE 16                 // 次のジャンプモーションが滑空になるまでのフレーム数
#define PLAYER_CONTROL_MOVE_ON_GROUND 0.85f                 // 地上での移動量制御
#define PLAYER_CONTROL_MOVE_IN_AIR 0.95f                    // 空中での移動量制御
#define PLAYER_CONTROL_MOVE_TAKE_DAMAGE 0.975f              // 負傷中の移動量制御
#define PLAYER_CONTROL_MOVE_SQUAT 0.965f                    // しゃがみ中の移動量制御
#define PLAYER_CONTROL_MOVE_DEATH 0.98f                     // 死亡中の移動量制御
#define PLAYER_TURN_FRAME 4                                 // 振り向きに必要なフレーム数
#define PLAYER_LANDING_TIME 5                               // 着地フレーム
#define PLAYER_ATTACK_ANIM_MIN_FRAME 10                     // 最低保証の攻撃アニメーション再生（ヒットストップ時、変な格好で止めないため）（各攻撃フレームより短く）
#define PLAYER_SQUAT_CUT_COLLISION_SIZE 0.6f                // しゃがみが当たり判定を削る割合
#define STICK_SQUAT_RIGHT(angle) angle > D3DXToRadian(135.0f) && angle <= D3DXToRadian(170.0f)  // しゃがみながら向きを変える右
#define STICK_SQUAT_LEFT(angle) angle < D3DXToRadian(-135.0f) && angle >= D3DXToRadian(-170.0f)  // しゃがみながら向きを変える左
#define PLAYER_DEATH_STOP_POS_Y -99999.9f                   // 死亡時に止めておく高さ
#define PLAYER_DEATH_STOP_FRAME 45                          // 死亡時に、止まるフレーム数
#define PLAYER_SET_WALK_SMOKE_VALUE 6.0f                    // プレイヤーが砂煙を発生させる値

// プレイヤーの防御周り
#define PLAYER_MAX_STOCK 8                                  // ストックの最大数
#define PLAYER_LUCKY_GUARD_MAX 10000                        // ラッキーガードの最大値（これ分の防御力(だいたい防御力は1000くらい？)）
#define PLAYER_TAKE_DAMAGE_RATE_FROM_BALL 10.0f             // ボールからダメージを受けるとき、速度にかける割合
#define PLAYER_TAKE_DAMAGE_BORDER_DAMAGE 500.0f             // 小やられか大やられの境目ダメージ
#define PLAYER_TAKE_DAMAGE_SMALL_FRAME 30                   // 負傷小フレーム数
#define PLAYER_TAKE_DAMAGE_BIG_FRAME 180                    // 負傷大フレーム数
#define PLAYER_TAKE_DAMAGE_LUCKY_GUARD_FRAME 40             // 負傷ラッキーガードフレーム数
#define PLAYER_KNOCK_BACK_SMALL_IN_AIR 10.0f                // ノックバック量（空中負傷小）
#define PLAYER_KNOCK_BACK_BIG 22.5f                         // ノックバック量（負傷大）
#define PLAYER_KNOCK_BACK_LUCKY_GUARD 25.0f                 // ノックバック量（ラッキーガード）
#define PLAYER_KNOCK_BACK_DEATH_X 25.0f                     // ノックバック量（死亡時X）
#define PLAYER_KNOCK_BACK_DEATH_Y 12.5f                     // ノックバック量（死亡時Y）
#define PLAYER_KNOCK_BACK_STOP_BORDER 7.5f                  // ノックバックが止まったとみなす境界値
#define PLAYER_TAKE_DAMAGE_BLOWN_GRAVITY 0.5f               // 吹っ飛ばされ中にかかる重力
#define PLAYER_TAKE_DAMAGE_STAND_UP_FRAME 18                // 起き上がりのフレーム数
#define PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME (PLAYER_TAKE_DAMAGE_STAND_UP_FRAME - 13) // 起き上がり中の無敵フレーム
#define PLAYER_TAKE_DAMAGE_START_QUICK_STAND_UP (PLAYER_TAKE_DAMAGE_BIG_FRAME - 60) // 即座に起き上がるのが可能になるフレーム
#define PLAYER_TAKE_DAMAGE_SMALL_EFFECT_FRAME 30            // 負傷小コントローラの振動フレーム数
#define PLAYER_TAKE_DAMAGE_BIG_EFFECT_FRAME 60              // 負傷大コントローラの振動フレーム数
#define PLAYER_TAKE_DAMAGE_DEATH_EFFECT_FRAME 90            // 負傷死亡コントローラの振動フレーム数

// プレイヤーの攻撃周り(全て右向き想定)
//==========================
// スイング
//==========================
// 威力
#define ATTACK_SWING_BASE_POWER 1.0f
#define ATTACK_SWING_ADD_BASE_POWER_RATE 0.001f
// 全体フレーム、攻撃終了フレーム
#define ATTACK_SWING_WHOLE_FRAME 21
#define ATTACK_SWING_END_FRAME (ATTACK_SWING_WHOLE_FRAME - 11)
// その他
#define ATTACK_SWING_CHARGE_MAX_FRAME_NORMAL 35            // チャージ最大フレーム数（通常）
#define ATTACK_SWING_CHARGE_MAX_FRAME_FAST 20              // チャージ最大フレーム数（溜め短縮）
#define ATTACK_SWING_CHARGE_ATTACK_RATE 0.03f              // チャージ時間によるパワー増加割合 (最大チャージはスマッシュと同じ)
#define STICK_SWING_UP(angle) angle < D3DXToRadian(75.0f) && angle > D3DXToRadian(-75.0f)
#define STICK_SWING_DOWN(angle) angle >= D3DXToRadian(105.0f) && angle <= D3DXToRadian(180.0f) || angle <= D3DXToRadian(-105.0f) && angle > D3DXToRadian(-180.0f)
#define STICK_SWING_HORIZON(angle) angle >= D3DXToRadian(75.0f) && angle < D3DXToRadian(105.0f) || angle <= D3DXToRadian(-75.0f) && angle > D3DXToRadian(-105.0f)

//==========================
// スマッシュ
//==========================
// 威力
#define ATTACK_SMASH_BASE_POWER 0.2f                // スマッシュの場合、ボールの現在の速さにかける値に変わる
#define ATTACK_SMASH_ADD_BASE_POWER_RATE 0.00035f
// 全体フレーム、攻撃終了フレーム
#define ATTACK_SMASH_WHOLE_FRAME 25
#define ATTACK_SMASH_END_FRAME (ATTACK_SMASH_WHOLE_FRAME - 11)
// その他（打つ瞬間に、空中でも方向転換ができる）
#define ATTACK_SMASH_SPEED_RATE 0.1f                // 現在のボールの速さにかける割合

//==========================
// スパイク
//==========================
// 威力
#define ATTACK_SPIKE_BASE_POWER 1.0f
#define ATTACK_SPIKE_ADD_BASE_POWER_RATE 0.001f
// 当たり判定の大きさ
#define ATTACK_SPIKE_SIZE_RATE_X 1.5f
// 全体フレーム、攻撃終了フレーム
#define ATTACK_SPIKE_WHOLE_FRAME 21
#define ATTACK_SPIKE_END_FRAME (ATTACK_SPIKE_WHOLE_FRAME - 11)
// その他
#define STICK_SPIKE_RIGHT(angle) angle >= D3DXToRadian(15.0f) && angle <= D3DXToRadian(165.0f)
#define STICK_SPIKE_LEFT(angle) angle <= D3DXToRadian(-15.0f) && angle >= D3DXToRadian(-165.0f)
#define STICK_SPIKE_VERTICAL(angle) angle < D3DXToRadian(15.0f) && angle > D3DXToRadian(-15.0f) || angle > D3DXToRadian(165.0f) && angle <= D3DXToRadian(180.0f) || angle < D3DXToRadian(-165.0f) && angle > D3DXToRadian(-180.0f)

//==========================
// キャッチの構え
//==========================
// 位置スライド
#define ATTACK_CATCH_READY_SLIDE_POS_X_RATE 0.65f
// 全体フレーム、攻撃終了フレーム
#define ATTACK_CATCH_READY_WHOLE_FRAME 21
#define ATTACK_CATCH_READY_END_FRAME (ATTACK_SWING_WHOLE_FRAME - 11)
// その他
#define ATTACK_CATCH_READY_EFFECT_FRAME 15  // コントローラの振動フレーム数

//==========================
// 投げ
//==========================
// 全体フレーム、攻撃終了フレーム
#define ATTACK_THROW_WHOLE_FRAME 55
#define ATTACK_THROW_END_FRAME (ATTACK_THROW_WHOLE_FRAME - 35)
// その他
#define ATTACK_THROW_SLOW_SPEED_RATE 0.2f   // 投げ中、どれくらいゆっくりにするか（通常速度でも面白そう）

//==========================
// バント
//==========================
// 全体フレーム、攻撃終了フレーム
#define ATTACK_BUNT_WHOLE_FRAME 21
#define ATTACK_BUNT_END_FRAME (ATTACK_BUNT_WHOLE_FRAME - 11)
// その他
#define ATTACK_BUNT_ANGLE 15.0f

//==========================
// 吸収
//==========================
// 当たり判定の大きさ
#define ATTACK_ABSORB_SIZE_RATE 1.75f
// 全体フレーム、攻撃終了フレーム
#define ATTACK_ABSORB_WHOLE_FRAME 80
#define ATTACK_ABSORB_END_FRAME (ATTACK_ABSORB_WHOLE_FRAME - 20)

//==========================
// 必殺技
//==========================
// 必殺技ゲージ上昇量（キャッチ時はさらに倍率が上がる）
#define ATTACK_SP_GAUGE_BASE_GAIN 40.0f
#define ATTACK_SP_GAUGE_ADD_BASE_GAIN_RATE 0.08f
#define ATTACK_SP_GAUGE_EX_ADD_RATE 1.5f
// 各必殺技の細かい値は、各関数内で定義してある

//================================================
// 前方宣言
//================================================
class CAi;
class CUI;
class CText;

//================================================
// クラス宣言
//================================================

// プレイヤークラス
class CPlayer : public CCharacter
{
public:
    CPlayer();
    ~CPlayer();

    // アニメーションの種類
    typedef enum
    {
        ANIM_IDLE = 0,      // 待機
        ANIM_MOVE,          // 移動
        ANIM_JUMP,          // ジャンプ
        ANIM_LANDING,       // 着地
        ANIM_SWING_CHARGE,  // スイングのチャージ
        ANIM_SWING,         // スイング
        ANIM_SMASH,         // スマッシュ
        ANIM_SPIKE,         // スパイク
        ANIM_CATCH_READY,   // キャッチの構え
        ANIM_THROW,         // 投げ
        ANIM_BUNT,          // バント
        ANIM_GLIDE,         // 滑空
        ANIM_SECOND_JUMP,   // 2段ジャンプ
        ANIM_SQUAT,         // しゃがみ
        ANIM_DAMAGE_SMALL,  // のけぞり
        ANIM_DAMAGE_BIG,    // ダウン
        ANIM_STAND_UP,      // 起き上がり
        ANIM_ABSORB,        // 吸収（アピール）
        ANIM_BLOWN,         // 吹き飛ばされ
        ANIM_THIRD_JUMP,    // 3段ジャンプ
        ANIM_MAX,
    }ANIMATION;

    // プレイアブルキャラがどのコントローラを使うか
    typedef enum
    {
        PLAYABLE_001 = 0,
        PLAYABLE_002,
        PLAYABLE_003,
        PLAYABLE_004,
        PLAYABLE_MAX,
    }PLAYABLE;

    // モデルの各パーツ
    typedef enum
    {
        PARTS_HIP = 0,
        PARTS_BODY,
        PARTS_HEAD,
        PARTS_RSHOULDER,
        PARTS_RARM,
        PARTS_RHAND,
        PARTS_LSHOULDER,
        PARTS_LARM,
        PARTS_LHAND,
        PARTS_RTHIGH,
        PARTS_RLEG,
        PARTS_RFOOT,
        PARTS_LTHIGH,
        PARTS_LLEG,
        PARTS_LFOOT,
        PARTS_WEP,
        PARTS_MAX,
    }PARTS;

    // モデルのカスタマイズパーツ
    typedef enum
    {
        CUSTOM_PARTS_HEAD = 0,   // 頭
        CUSTOM_PARTS_UP,         // 上半身
        CUSTOM_PARTS_DOWN,       // 下半身
        CUSTOM_PARTS_WEP,        // 武器
        CUSTOM_PARTS_MAX         // カスタマイズパーツの最大数
    }CUSTOM_PARTS;

    // 攻撃状態
    typedef enum
    {
        ATTACK_STATE_NONE = 0,      // なし
        ATTACK_STATE_SWING_CHARGE,  // スイングチャージ
        ATTACK_STATE_SWING,         // スイング
        ATTACK_STATE_SMASH,         // スマッシュ
        ATTACK_STATE_SPIKE,         // スパイク
        ATTACK_STATE_CATCH_READY,   // キャッチの構え
        ATTACK_STATE_THROW,         // 投げ
        ATTACK_STATE_BUNT,          // バント
        ATTACK_STATE_ABSORB,        // 吸収
        ATTACK_STATE_MAX,
    }ATTACK_STATE;

    // 負傷状態
    typedef enum
    {
        DAMAGE_STATE_NONE = 0,      // なし
        DAMAGE_STATE_SMALL,         // のけぞり
        DAMAGE_STATE_BIG,           // ダウン
        DAMAGE_STATE_STAND_UP,      // 起き上がり
        DAMAGE_STATE_BLOWN,         // 吹っ飛ばされ
        DAMAGE_STATE_MAX,
    }DAMAGE_STATE;

    // 汎用パラメータの内訳
    typedef enum
    {
        PARAM_SWING_UP = 0,             // スイング上
        PARAM_SWING_DOWN,               // スイング下
        PARAM_SMASH,                    // スマッシュ
        PARAM_SPIKE_RIGHT,              // スパイク右
        PARAM_SPIKE_LEFT,               // スパイク左
        PARAM_5_WEAPON_SP,              // 武器の必殺技補助値3番
        PARAM_6_WEAPON_SP,              // 武器の必殺技補助値2番
        PARAM_7_WEAPON_SP,              // 武器の必殺技補助値1番
        PARAM_MAX
    }PARAM;

    typedef struct
    {
        bool bTiltedLeftStick;          // 左スティックが傾いたかどうか
        float fLeftStickAngle;          // 左スティック角度
        bool bTiltedRightStick;         // 右スティックが傾いたかどうか
        float fRightStickAngle;         // 右スティック角度
        bool bTriggerA;                 // Aボタントリガー
        bool bPressA;                   // Aボタンプレス
        bool bTriggerX;                 // Xボタントリガー
        bool bPressX;                   // Xボタンプレス
        bool bReleaseX;                 // Xボタンリリース
        bool bTriggerY;                 // Yボタントリガー
        bool bTriggerB;                 // Bボタントリガー
    }ControlInput;    // 入力制御

    // AI
    typedef enum
    {
        AI_LEVEL_NONE = 0,    // AIなし
        AI_LEVEL_1,           // よわい
        AI_LEVEL_2,           // ふつう
        AI_LEVEL_3            // つよい
    }AI_LEVEL;

    // ボイスセット
    typedef enum
    {
        VOICE_SET_0 = 0,      // 
        VOICE_SET_1,          // 
        VOICE_SET_2,          // 
        VOICE_SET_3           // 
    }VOICE_SET;

    // 特殊能力（アビリティ）のフラグ
    typedef enum
    {
        EX_FLAG_NONE = 0,                      // なし
        EX_FLAG_THREE_JUMP = 0x001 << 1,       // 三段ジャンプ
        EX_FLAG_FAST_CHARGE = 0x001 << 2,      // 溜め短縮
        EX_FLAG_DONT_CATCH = 0x001 << 3,       // キャッチ使用不可
        EX_FLAG_FIRST_HIT_GUARD = 0x001 << 4,  // ファーストヒットガード
    }EX_FLAG;

    // 必殺技（SPショット）の種類
    typedef enum
    {
        SP_SHOT_HEAL = 0,       // ヒール（回復）
        SP_SHOT_DECOY,          // ぶんしんボール
        SP_SHOT_WALL_THROUGH,   // ウォールスルー（壁ループ）
    }SP_SHOT;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void LoadCustom(void);                                                              // カスタマイズ読み込み
    void BindVoiceSet(int voiceSet);                                                    // ボイスセットを結びつける
    void Uninit(void);                                                                  // 終了処理
    void Input(void);                                                                   // 入力処理
    void Update(void);                                                                  // 更新処理
    void UpdateMannequin(void);                                                         // マネキンモード更新処理
    void UpdateGameUI(void);                                                            // ゲームUIの更新処理
    void DeadMove(void);                                                                // やられた時の挙動
    void ResetOnGround(void);                                                           // 着地時にリセットする変数
    void ResetAttack(void);                                                             // 攻撃周りでリセットする変数
    void Respawn(void);                                                                 // リスポーン
    void Draw(void);                                                                    // 描画処理
    static CPlayer *CreateInGame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nStock, int playable, AI_LEVEL AIlevel, bool bUseKeyboard = false);   // ゲーム内での生成処理
    static CPlayer *CreateInCustom(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int playable, bool bDisp);     // カスタマイズ画面での生成

    void Movement(float fSpeed);                                                        // 行動
    void MoveMotion(void);                                                              // 移動モーションの管理
    void CollisionBall(D3DXVECTOR3 playerPos);                                          // ボールに当たったかどうか
    void TakeDamage(float fDamage, int nWho, D3DXVECTOR3 damagePos, D3DXVECTOR3 damageOldPos, bool bUseLuckyGuard, bool bSetOff = false);   // ダメージをくらう
    void Control(float fSpeed);                                                         // 制御
    void Jump(void);                                                                    // ジャンプ
    void AttackUpdate(D3DXVECTOR3 pos, bool bFirstCollision);                           // 攻撃更新処理
    void AttackGenerator(D3DXVECTOR3 pos);                                              // 攻撃発生処理
    void SwingCharge(void);                                                             // スイングのチャージ
    void Swing(D3DXVECTOR3 playerPos, bool bFirstCollision);                            // スイング
    void Smash(D3DXVECTOR3 playerPos, bool bFirstCollision);                            // スマッシュ
    void Spike(D3DXVECTOR3 playerPos, bool bFirstCollision);                            // スパイク
    void CatchReady(D3DXVECTOR3 playerPos);                                             // キャッチの構え
    void Bunt(D3DXVECTOR3 playerPos);                                                   // バント
    void Absorb(D3DXVECTOR3 playerPos);                                                 // 吸収
    bool IsAttackBall(D3DXVECTOR3 attackPos, D3DXVECTOR3 attackSize, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag);   // 攻撃の当たり判定
    void RotControl();                                                                  // 回転制御
    void AttackMotion(void);                                                            // 攻撃モーションの管理
    void DamageUpdate(D3DXVECTOR3 pos);                                                 // 負傷更新処理
    void DamageMotion(void);                                                            // 負傷モーションの管理
    void GainSpGauge(bool bExAdd = false);                                              // 必殺ゲージ上昇

	void ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumTexture);					// クリッピングマスク適用 //池田追加
	void ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size);									// テクスチャなしクリッピングマスク適用 //池田追加

    /*========================================================
    // カスタマイズの文字列周り
    //======================================================*/
    static void CustomPartsName(char* pName, int nPartsType);
    void CustomExName(char* pName);
    void CustomSpName(char* pName);

    /*========================================================
    // 必殺技
    //======================================================*/
    void EmitSpShot(void);  // この中で分岐して、各必殺技の打ちだす際の処理をする  
    void SpHeal(void);
    void SpDecoy(void);
    void SpWallThrough(void);

    /*========================================================
    // セッター
    //======================================================*/
    void SetAddPoint(void) { m_nPoint++; }
    void SetStopTime(int nStopTime) { m_nCntStopTime = nStopTime; }
    void SetSpGaugeCurrent(float fSpGauge) { m_fSpGaugeCurrent = fSpGauge; }
    void SetDisp(bool bDisp) { m_bDisp = bDisp; }
    void ResetStatusEveryRound(void);   // 毎ラウンドリセットするステータス

    /*========================================================
    // ゲッター
    //======================================================*/
    bool GetDisp(void) { return m_bDisp; }
    float GetAtk(void) { return m_fAtk; }
    float GetDef(void) { return m_fDef; }
    float GetSpd(void) { return m_fSpd; }
    float GetWei(void) { return m_fWei; }
    ControlInput *GetControlInput(void) { return &m_controlInput; }
    int GetPlayable(void) { return m_playable; }
    int GetStopTime(void) { return m_nCntStopTime; }
    int GetAttackTime(void) { return m_nCntAttackTime; }
    int GetSwingCharge(void) { return m_nCntSwingCharge; }
    float GetLife(void) { return m_fLife; }
    int GetPoint(void) { return m_nPoint; }
    float GetSpGaugeCurrent(void) { return m_fSpGaugeCurrent; }
    bool GetGround(void) { return m_bGround; }
    int GetPressJumpTime(void) { return m_nCntPressJump; }
    int GetStock(void) { return m_nStock; }
    D3DXVECTOR3 GetCollisionSizeDeffence(void) { return m_collisionSizeDeffence; }
    AI_LEVEL GetAILevel(void) { return m_AIlevel; }
    int GetCustomPartsNum(int nParts) { return m_anNumCustomParts[nParts]; }
    bool GetUseControllerEffect(void);       // コントローラの振動を使用するかどうか

private:
    bool m_bMannequin;                       // マネキンかどうか
    ControlInput m_controlInput;             // 入力管理
    bool m_bUseKeyboard;                     // キーボードを使うかどうか
    AI_LEVEL m_AIlevel;                      // AIレベル
    CAi *m_pAI;                              // AIへのポインタ
    CUI *m_pUI_HP;                           // HPゲージへのポインタ
    CUI *m_pUI_SP;                           // SPゲージへのポインタ
    CUI *m_apUI_Stock[PLAYER_MAX_STOCK];     // ストックへのポインタ
    CUI *m_pUI_Playable;                     // プレイヤー表示へのポインタ
    CUI *m_pUI_Custom_Atk;                   // カスタマイズ画面_攻撃ゲージ
    CUI *m_pUI_Custom_Def;                   // カスタマイズ画面_防御ゲージ
    CUI *m_pUI_Custom_Spd;                   // カスタマイズ画面_速さゲージ
    CUI *m_pUI_Custom_Wei;                   // カスタマイズ画面_重さゲージ
    CText *m_pText_Custom_Ex;                // カスタマイズ画面_特殊能力名
    CText *m_pText_Custom_Sp;                // カスタマイズ画面_必殺技名
                                             
    int m_playable;                          // プレイアブルキャラ
    int m_nPoint;                            // ポイント
    int m_nStock;                            // ストック数
    D3DXVECTOR3 m_startPos;                  // 開始位置を保持
    D3DXVECTOR3 m_startRot;                  // 開始向きを保持
                                             
    D3DXVECTOR3 m_move;                      // 移動量
    D3DXVECTOR3 m_moveOld;                   // 1F前の移動量
    int m_exFlag;                            // 特殊能力フラグ
    ATTACK_STATE m_attackState;              // 攻撃状態
                                             
    int m_nCntAttackTime;                    // 攻撃時間をカウント
    bool m_bGround;                          // 地面にいるかどうか
                                             
    bool m_bDisp;                            // 表示フラグ
    int m_nCntTakeDamageTime;                // 攻撃を受けている時間
    float m_fLife;                           // 体力
    bool m_bUsedLuckyGuard;                  // ラッキーガードを使ったかどうか
    DAMAGE_STATE m_damageState;              // 負傷状態
                                             
    int m_nCntLandingTime;                   // 着地時間
    bool m_bGroundOld;                       // 1F前、地面にいたかどうか

    int m_anNumCustomParts[CUSTOM_PARTS_MAX];// カスタマイズパーツの番号
    float m_fAtk;                            // 攻撃
    float m_fDef;                            // 防御（最大HPにもなる）
    float m_fSpd;                            // 速さ
    float m_fWei;                            // 重さ
    float m_afParam[PARAM_MAX];              // 汎用データ
    int m_nModelPosDefUp;                    // モデルの初期位置データ（上半身）
    int m_nModelPosDefDown;                  // モデルの初期位置データ（下半身）
                                             
    int m_nCntTurnTime;                      // 方向転換時間
    int m_nCntPressJump;                     // ジャンプを押した時間
    bool m_bDiveCheck;                       // 急降下になるかどうかのチェック
    bool m_bDiving;                          // 急降下中
    bool m_bUsedSecondJump;                  // 二段ジャンプを使ったかどうか
    bool m_bSquat;                           // しゃがんでいるかどうか
    int m_nCntStartGlide;                    // 滑空が始まるまでのカウンタ
                                             
    int m_nCntSwingCharge;                   // スイングのチャージ時間
    bool m_bUseAvoidMultipleHits;            // 多段ヒット回避を使うかどうか
    bool m_bHitAttackThisFrame;              // このフレームで攻撃が当たったかどうか
    int m_nCntStopTime;                      // 硬直時間のカウンタ
    int m_nCntAttackAnimTime;                // 攻撃アニメーションの最低保証
                                             
    int m_spShot;                            // 必殺技
    float m_fSpGaugeCurrent;                 // 現在の必殺ゲージ量
    float m_fSpGaugeMax;                     // 最大必殺ゲージ量
                                             
    D3DXVECTOR3 m_collisionSizeAttack;       // 攻撃衝突サイズ
    D3DXVECTOR3 m_collisionSizeDeffence;     // 防御衝突サイズ

	CClipingMusk* m_pClipingMusk;			 // クリッピングマスク
                                             
    //===================================    
    // 特殊能力対応周り                      
    //===================================    
    bool m_bUsedThreeJump;                   // 三段ジャンプを使ったかどうか
    int m_nSwingChargeMax;                   // 最大溜め時間
};

#endif