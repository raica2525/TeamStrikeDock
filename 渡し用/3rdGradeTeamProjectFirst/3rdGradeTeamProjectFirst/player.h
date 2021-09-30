//======================================================================================
//
// �v���C���[���� (player.h)
// Author : �㓡�T�V���A�r�c�I��i���U���g��J�X�^�}�C�Y�̃N���b�s���O�}�X�N�Ή��j
//
//======================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "character.h"
#include "model.h"
#include "cliping_musk.h"

//================================================
// �}�N����`
//================================================

// �v���C���[�̈ړ�����
#define PLAYER_ROT_RIGHT D3DXToRadian(270.0f)
#define PLAYER_ROT_LEFT D3DXToRadian(90.0f)
#define PLAYER_BASE_MINIMUM_SPEED 7.25f                     // ��{�ŏ����x
#define PLAYER_ADD_BASE_SPEED_RATE 0.01f                    // ��{���x�ɑ�������
#define PLAYER_BASE_MINIMUM_GRAVITY 0.7f                    // ��{�ŏ��d��
#define PLAYER_ADD_BASE_GRAVITY_RATE 0.0008f                // ��{�d�͂ɑ�������
#define PLAYER_DIVE_SPEED_RATE 2.0f                         // �}�~���̑��x�㏸����
#define PLAYER_JUMP_FIRST_RATE 23.5f                        // �W�����v�̍ŏ��̋�������
#define PLAYER_NEXT_JUMP_DOWN_RATE 0.8f                     // ���̃W�����v���ǂ�قǉ����邩�̊���
#define PLAYER_JUMP_ADD_FRAME 14                            // �W�����v�����Z�ł���t���[����
#define PLAYER_JUMP_KEEP_FRAME (8 + PLAYER_JUMP_ADD_FRAME)  // �W�����v���L�[�v�ł���t���[����
#define PLAYER_FIRST_JUMP_MOTION_TO_GLIDE 25                // �ŏ��̃W�����v���[�V����������ɂȂ�܂ł̃t���[����
#define PLAYER_NEXT_JUMP_MOTION_TO_GLIDE 16                 // ���̃W�����v���[�V����������ɂȂ�܂ł̃t���[����
#define PLAYER_CONTROL_MOVE_ON_GROUND 0.85f                 // �n��ł̈ړ��ʐ���
#define PLAYER_CONTROL_MOVE_IN_AIR 0.95f                    // �󒆂ł̈ړ��ʐ���
#define PLAYER_CONTROL_MOVE_TAKE_DAMAGE 0.975f              // �������̈ړ��ʐ���
#define PLAYER_CONTROL_MOVE_SQUAT 0.965f                    // ���Ⴊ�ݒ��̈ړ��ʐ���
#define PLAYER_CONTROL_MOVE_DEATH 0.98f                     // ���S���̈ړ��ʐ���
#define PLAYER_TURN_FRAME 4                                 // �U������ɕK�v�ȃt���[����
#define PLAYER_LANDING_TIME 5                               // ���n�t���[��
#define PLAYER_ATTACK_ANIM_MIN_FRAME 10                     // �Œ�ۏ؂̍U���A�j���[�V�����Đ��i�q�b�g�X�g�b�v���A�ςȊi�D�Ŏ~�߂Ȃ����߁j�i�e�U���t���[�����Z���j
#define PLAYER_SQUAT_CUT_COLLISION_SIZE 0.6f                // ���Ⴊ�݂������蔻�����銄��
#define STICK_SQUAT_RIGHT(angle) angle > D3DXToRadian(135.0f) && angle <= D3DXToRadian(170.0f)  // ���Ⴊ�݂Ȃ��������ς���E
#define STICK_SQUAT_LEFT(angle) angle < D3DXToRadian(-135.0f) && angle >= D3DXToRadian(-170.0f)  // ���Ⴊ�݂Ȃ��������ς��鍶
#define PLAYER_DEATH_STOP_POS_Y -99999.9f                   // ���S���Ɏ~�߂Ă�������
#define PLAYER_DEATH_STOP_FRAME 45                          // ���S���ɁA�~�܂�t���[����
#define PLAYER_SET_WALK_SMOKE_VALUE 6.0f                    // �v���C���[�������𔭐�������l
#define PLAYER_WEAPON_LOOK_FRAME 250                        // �J�X�^�}�C�Y��ʂŕ��������t���[����
#define PLAYER_VICTORY_WAIT_START_FRAME 120                 // ���U���g��ʂőҋ@��ԂɂȂ�t���[����
#define PLAYER_SP_GAUGE_MAX_EFFECT_INTERVAL 30              // SP�Q�[�W�G�t�F�N�g�̃C���^�[�o��
#define PLAYER_VICTORY_VOICE_FRAME 60                       // �����{�C�X�t���[��
#define PLAYER_SP_WHOLE_FRAME 120                           // �K�E�Z�S�̃t���[��

// �v���C���[�̖h�����
#define PLAYER_MAX_STOCK 6                                  // �X�g�b�N�̍ő吔
#define PLAYER_LUCKY_GUARD_MAX 10000                        // ���b�L�[�K�[�h�̍ő�l�i���ꕪ�̖h���(���������h��͂�1000���炢�H)�j
#define PLAYER_TAKE_DAMAGE_RATE_FROM_BALL 10.0f             // �{�[������_���[�W���󂯂�Ƃ��A���x�ɂ����銄��
#define PLAYER_TAKE_DAMAGE_BORDER_DAMAGE 500.0f             // �����ꂩ�����̋��ڃ_���[�W
#define PLAYER_TAKE_DAMAGE_SMALL_FRAME 30                   // �������t���[����
#define PLAYER_TAKE_DAMAGE_BIG_FRAME 180                    // ������t���[����
#define PLAYER_TAKE_DAMAGE_LUCKY_GUARD_FRAME 40             // �������b�L�[�K�[�h�t���[����
#define PLAYER_KNOCK_BACK_SMALL_IN_AIR 10.0f                // �m�b�N�o�b�N�ʁi�󒆕������j
#define PLAYER_KNOCK_BACK_BIG 22.5f                         // �m�b�N�o�b�N�ʁi������j
#define PLAYER_KNOCK_BACK_LUCKY_GUARD 25.0f                 // �m�b�N�o�b�N�ʁi���b�L�[�K�[�h�j
#define PLAYER_KNOCK_BACK_DEATH_X 25.0f                     // �m�b�N�o�b�N�ʁi���S��X�j
#define PLAYER_KNOCK_BACK_DEATH_Y 12.5f                     // �m�b�N�o�b�N�ʁi���S��Y�j
#define PLAYER_KNOCK_BACK_STOP_BORDER 7.5f                  // �m�b�N�o�b�N���~�܂����Ƃ݂Ȃ����E�l
#define PLAYER_TAKE_DAMAGE_BLOWN_GRAVITY 0.5f               // ������΂��ꒆ�ɂ�����d��
#define PLAYER_TAKE_DAMAGE_STAND_UP_FRAME 18                // �N���オ��̃t���[����
#define PLAYER_TAKE_DAMAGE_STAND_UP_INVINCIBLE_FRAME (PLAYER_TAKE_DAMAGE_STAND_UP_FRAME - 13) // �N���オ�蒆�̖��G�t���[��
#define PLAYER_TAKE_DAMAGE_START_QUICK_STAND_UP (PLAYER_TAKE_DAMAGE_BIG_FRAME - 60) // �����ɋN���オ��̂��\�ɂȂ�t���[��
#define PLAYER_TAKE_DAMAGE_SMALL_EFFECT_FRAME 30            // �������R���g���[���̐U���t���[����
#define PLAYER_TAKE_DAMAGE_BIG_EFFECT_FRAME 60              // ������R���g���[���̐U���t���[����
#define PLAYER_TAKE_DAMAGE_DEATH_EFFECT_FRAME 90            // �������S�R���g���[���̐U���t���[����

// �v���C���[�̍U������(�S�ĉE�����z��)
//==========================
// �X�C���O
//==========================
// �З�
#define ATTACK_SWING_BASE_POWER 1.0f
#define ATTACK_SWING_ADD_BASE_POWER_RATE 0.001f
// �S�̃t���[���A�U���I���t���[��
#define ATTACK_SWING_WHOLE_FRAME 21
#define ATTACK_SWING_END_FRAME (ATTACK_SWING_WHOLE_FRAME - 11)
// ���̑�
#define ATTACK_SWING_CHARGE_MAX_FRAME_NORMAL 35            // �`���[�W�ő�t���[�����i�ʏ�j
#define ATTACK_SWING_CHARGE_MAX_FRAME_FAST 20              // �`���[�W�ő�t���[�����i���ߒZ�k�j
#define ATTACK_SWING_CHARGE_EMIT_EFFECT_FRAME 10           // �`���[�W�G�t�F�N�g�����t���[��
#define ATTACK_SWING_CHARGE_ATTACK_RATE 0.03f              // �`���[�W���Ԃɂ��p���[�������� (�ő�`���[�W�̓X�}�b�V���Ɠ���)
#define STICK_SWING_UP(angle) angle < D3DXToRadian(75.0f) && angle > D3DXToRadian(-75.0f)
#define STICK_SWING_DOWN(angle) angle >= D3DXToRadian(105.0f) && angle <= D3DXToRadian(180.0f) || angle <= D3DXToRadian(-105.0f) && angle > D3DXToRadian(-180.0f)
#define STICK_SWING_HORIZON(angle) angle >= D3DXToRadian(75.0f) && angle < D3DXToRadian(105.0f) || angle <= D3DXToRadian(-75.0f) && angle > D3DXToRadian(-105.0f)

//==========================
// �X�}�b�V��
//==========================
// �З�
#define ATTACK_SMASH_BASE_POWER 0.2f                // �X�}�b�V���̏ꍇ�A�{�[���̌��݂̑����ɂ�����l�ɕς��
#define ATTACK_SMASH_ADD_BASE_POWER_RATE 0.00035f
// �S�̃t���[���A�U���I���t���[��
#define ATTACK_SMASH_WHOLE_FRAME 25
#define ATTACK_SMASH_END_FRAME (ATTACK_SMASH_WHOLE_FRAME - 11)
// ���̑��i�łu�ԂɁA�󒆂ł������]�����ł���j
#define ATTACK_SMASH_SPEED_RATE 0.1f                // ���݂̃{�[���̑����ɂ����銄��

//==========================
// �X�p�C�N
//==========================
// �З�
#define ATTACK_SPIKE_BASE_POWER 1.0f
#define ATTACK_SPIKE_ADD_BASE_POWER_RATE 0.001f
// �����蔻��̑傫��
#define ATTACK_SPIKE_SIZE_RATE_X 1.5f
// �S�̃t���[���A�U���I���t���[��
#define ATTACK_SPIKE_WHOLE_FRAME 21
#define ATTACK_SPIKE_END_FRAME (ATTACK_SPIKE_WHOLE_FRAME - 11)
// ���̑�
#define STICK_SPIKE_RIGHT(angle) angle >= D3DXToRadian(15.0f) && angle <= D3DXToRadian(165.0f)
#define STICK_SPIKE_LEFT(angle) angle <= D3DXToRadian(-15.0f) && angle >= D3DXToRadian(-165.0f)
#define STICK_SPIKE_VERTICAL(angle) angle < D3DXToRadian(15.0f) && angle > D3DXToRadian(-15.0f) || angle > D3DXToRadian(165.0f) && angle <= D3DXToRadian(180.0f) || angle < D3DXToRadian(-165.0f) && angle > D3DXToRadian(-180.0f)

//==========================
// �L���b�`�̍\��
//==========================
// �ʒu�X���C�h
#define ATTACK_CATCH_READY_SLIDE_POS_X_RATE 0.65f
// �S�̃t���[���A�U���I���t���[��
#define ATTACK_CATCH_READY_WHOLE_FRAME 21
#define ATTACK_CATCH_READY_END_FRAME (ATTACK_SWING_WHOLE_FRAME - 11)
// ���̑�
#define ATTACK_CATCH_READY_EFFECT_FRAME 15  // �R���g���[���̐U���t���[����

//==========================
// ����
//==========================
// �S�̃t���[���A�U���I���t���[��
#define ATTACK_THROW_WHOLE_FRAME 55
#define ATTACK_THROW_END_FRAME (ATTACK_THROW_WHOLE_FRAME - 35)
// ���̑�
#define ATTACK_THROW_SLOW_SPEED_RATE 0.2f   // �������A�ǂꂭ�炢�������ɂ��邩�i�ʏ푬�x�ł��ʔ������j

//==========================
// �o���g
//==========================
// �S�̃t���[���A�U���I���t���[��
#define ATTACK_BUNT_WHOLE_FRAME 21
#define ATTACK_BUNT_END_FRAME (ATTACK_BUNT_WHOLE_FRAME - 11)
// ���̑�
#define ATTACK_BUNT_ANGLE 15.0f

//==========================
// �z��
//==========================
// �����蔻��̑傫��
#define ATTACK_ABSORB_SIZE_RATE 1.75f
// �S�̃t���[���A�U���I���t���[��
#define ATTACK_ABSORB_WHOLE_FRAME 80
#define ATTACK_ABSORB_END_FRAME (ATTACK_ABSORB_WHOLE_FRAME - 20)

//==========================
// �K�E�Z
//==========================
// �K�E�Z�Q�[�W�㏸�ʁi�L���b�`���͂���ɔ{�����オ��j
#define ATTACK_SP_GAUGE_BASE_GAIN 40.0f
#define ATTACK_SP_GAUGE_ADD_BASE_GAIN_RATE 0.08f
#define ATTACK_SP_GAUGE_EX_ADD_RATE 1.5f
// �e�K�E�Z�ׂ̍����l�́A�e�֐����Œ�`���Ă���

//================================================
// �O���錾
//================================================
class CAi;
class CUI;
class CText;
class CNumberArray;
class CEffect3D;

//================================================
// �N���X�錾
//================================================

// �v���C���[�N���X
class CPlayer : public CCharacter
{
public:
    CPlayer();
    ~CPlayer();

    // �A�j���[�V�����̎��
    typedef enum
    {
        ANIM_IDLE = 0,      // �ҋ@
        ANIM_MOVE,          // �ړ�
        ANIM_JUMP,          // �W�����v
        ANIM_LANDING,       // ���n
        ANIM_SWING_CHARGE,  // �X�C���O�̃`���[�W
        ANIM_SWING,         // �X�C���O
        ANIM_SMASH,         // �X�}�b�V��
        ANIM_SPIKE,         // �X�p�C�N
        ANIM_CATCH_READY,   // �L���b�`�̍\��
        ANIM_THROW,         // ����
        ANIM_BUNT,          // �o���g
        ANIM_GLIDE,         // ����
        ANIM_SECOND_JUMP,   // 2�i�W�����v
        ANIM_SQUAT,         // ���Ⴊ��
        ANIM_DAMAGE_SMALL,  // �̂�����
        ANIM_DAMAGE_BIG,    // �_�E��
        ANIM_STAND_UP,      // �N���オ��
        ANIM_ABSORB,        // �z���i�A�s�[���j
        ANIM_BLOWN,         // ������΂���
        ANIM_THIRD_JUMP,    // 3�i�W�����v
        ANIM_CUSTOM_IDLE,   // �J�X�^�}�C�Y��ʂł̑ҋ@
        ANIM_WEAPON_LOOK,   // �J�X�^�}�C�Y��ʂł̕��������
        ANIM_FIRST,         // 1��
        ANIM_SECOND,        // 2��
        ANIM_THIRD,         // 3��
        ANIM_FOURTH,        // 4��
        ANIM_FIRST_WAIT,    // 1�ʑҋ@
        ANIM_MAX,
    }ANIMATION;

    // ���f���̊e�p�[�c
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

    // ���f���̃J�X�^�}�C�Y�p�[�c
    typedef enum
    {
        CUSTOM_PARTS_HEAD = 0,   // ��
        CUSTOM_PARTS_UP,         // �㔼�g
        CUSTOM_PARTS_DOWN,       // �����g
        CUSTOM_PARTS_WEP,        // ����
        CUSTOM_PARTS_MAX         // �J�X�^�}�C�Y�p�[�c�̍ő吔
    }CUSTOM_PARTS;

    // �U�����
    typedef enum
    {
        ATTACK_STATE_NONE = 0,      // �Ȃ�
        ATTACK_STATE_SWING_CHARGE,  // �X�C���O�`���[�W
        ATTACK_STATE_SWING,         // �X�C���O
        ATTACK_STATE_SMASH,         // �X�}�b�V��
        ATTACK_STATE_SPIKE,         // �X�p�C�N
        ATTACK_STATE_CATCH_READY,   // �L���b�`�̍\��
        ATTACK_STATE_THROW,         // ����
        ATTACK_STATE_BUNT,          // �o���g
        ATTACK_STATE_ABSORB,        // �z��
        ATTACK_STATE_MAX,
    }ATTACK_STATE;

    // �������
    typedef enum
    {
        DAMAGE_STATE_NONE = 0,      // �Ȃ�
        DAMAGE_STATE_SMALL,         // �̂�����
        DAMAGE_STATE_BIG,           // �_�E��
        DAMAGE_STATE_STAND_UP,      // �N���オ��
        DAMAGE_STATE_BLOWN,         // ������΂���
        DAMAGE_STATE_MAX,
    }DAMAGE_STATE;

    // �ėp�p�����[�^�̓���
    typedef enum
    {
        PARAM_SWING_UP = 0,             // �X�C���O��
        PARAM_SWING_DOWN,               // �X�C���O��
        PARAM_SMASH,                    // �X�}�b�V��
        PARAM_SPIKE_RIGHT,              // �X�p�C�N�E
        PARAM_SPIKE_LEFT,               // �X�p�C�N��
        PARAM_5_WEAPON_SP,              // ����̕K�E�Z�⏕�l3��
        PARAM_6_WEAPON_SP,              // ����̕K�E�Z�⏕�l2��
        PARAM_7_WEAPON_SP,              // ����̕K�E�Z�⏕�l1��
        PARAM_MAX
    }PARAM;

    typedef struct
    {
        bool bTiltedLeftStick;          // ���X�e�B�b�N���X�������ǂ���
        float fLeftStickAngle;          // ���X�e�B�b�N�p�x
        bool bTiltedRightStick;         // �E�X�e�B�b�N���X�������ǂ���
        float fRightStickAngle;         // �E�X�e�B�b�N�p�x
        bool bTriggerA;                 // A�{�^���g���K�[
        bool bPressA;                   // A�{�^���v���X
        bool bTriggerX;                 // X�{�^���g���K�[
        bool bPressX;                   // X�{�^���v���X
        bool bReleaseX;                 // X�{�^�������[�X
        bool bTriggerY;                 // Y�{�^���g���K�[
        bool bTriggerB;                 // B�{�^���g���K�[
    }ControlInput;    // ���͐���

    // AI
    typedef enum
    {
        AI_LEVEL_NONE = 0,    // AI�Ȃ�
        AI_LEVEL_1,           // ��킢
        AI_LEVEL_2,           // �ӂ�
        AI_LEVEL_3            // �悢
    }AI_LEVEL;

    // �{�C�X�Z�b�g
    typedef enum
    {
        VOICE_SET_ROBO = 0,   // ���{
        VOICE_SET_WOMAN,      // ����
        VOICE_SET_GHOST,      // �S�[�X�g
        VOICE_SET_OOO,        // �I�[�Y
        VOICE_SET_MAN,        // �j��
        VOICE_SET_BOY,        // ���N
    }VOICE_SET;

    // ����\�́i�A�r���e�B�j�̃t���O
    typedef enum
    {
        EX_FLAG_NONE = 0,                      // �Ȃ�
        EX_FLAG_THREE_JUMP = 0x001 << 1,       // �O�i�W�����v
        EX_FLAG_FAST_CHARGE = 0x001 << 2,      // ���ߒZ�k
        EX_FLAG_DONT_CATCH = 0x001 << 3,       // �L���b�`�g�p�s��
        EX_FLAG_FIRST_HIT_GUARD = 0x001 << 4,  // �t�@�[�X�g�q�b�g�K�[�h
        EX_FLAG_TRAIL_GREEN = 0x001 << 5,      // ����O�ՐF:��
        EX_FLAG_TRAIL_PURPLE = 0x001 << 6,     // ����O�ՐF:��
        EX_FLAG_SAVE_SP_GAUGE = 0x001 << 7,    // �K�E�Q�[�W�ێ�
        EX_FLAG_ROUND_HEAL = 0x001 << 8,       // ���E���h���W�F�l
    }EX_FLAG;

    // �K�E�Z�iSP�V���b�g�j�̎��
    typedef enum
    {
        SP_SHOT_HEAL = 0,       // �q�[���i�񕜁j
        SP_SHOT_DECOY,          // �Ԃ񂵂�{�[��
        SP_SHOT_WALL_THROUGH,   // �E�H�[���X���[�i�ǃ��[�v�j
		SP_SHOT_BARRIER			// �o���A
    }SP_SHOT;

    // ����
    typedef enum
    {
        RANK_1 = 0,
        RANK_2,
        RANK_3,
        RANK_4
    }RANK;

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void LoadCustom(void);                                                              // �J�X�^�}�C�Y�ǂݍ���
    void Uninit(void);                                                                  // �I������
    void Input(void);                                                                   // ���͏���
    void Update(void);                                                                  // �X�V����
    void UpdateMannequin(void);                                                         // �}�l�L�����[�h�X�V����
    void UpdateGameUI(void);                                                            // �Q�[��UI�̍X�V����
    void DeadMove(void);                                                                // ���ꂽ���̋���
    void ResetOnGround(void);                                                           // ���n���Ƀ��Z�b�g����ϐ�
    void ResetAttack(void);                                                             // �U������Ń��Z�b�g����ϐ�
    void Respawn(void);                                                                 // ���X�|�[��
    void Draw(void);                                                                    // �`�揈��
    static CPlayer *CreateInGame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nStock, int nIdxCreate, int nIdxControlAndColor,
        AI_LEVEL AIlevel, bool bUseKeyboard = false);   // �Q�[�����ł̐�������
    static CPlayer *CreateInCustom(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxControlAndColor, bool bDisp); // �J�X�^�}�C�Y��ʂł̐���
    static CPlayer *CreateInResult(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxControlAndColor, RANK rank);  // ���U���g��ʂł̐���    

    void Movement(float fSpeed);                                                        // �s��
    void MoveMotion(void);                                                              // �ړ����[�V�����̊Ǘ�
    void CollisionBall(D3DXVECTOR3 playerPos);                                          // �{�[���ɓ����������ǂ���
    void TakeDamage(float fDamage, int nWho, D3DXVECTOR3 damagePos, D3DXVECTOR3 damageOldPos, bool bUseLuckyGuard, bool bSetOff = false);   // �_���[�W�����炤
    void Control(float fSpeed);                                                         // ����
    void Jump(void);                                                                    // �W�����v
    void AttackUpdate(D3DXVECTOR3 pos, bool bFirstCollision);                           // �U���X�V����
    void AttackGenerator(D3DXVECTOR3 pos);                                              // �U����������
    void SwingCharge(void);                                                             // �X�C���O�̃`���[�W
    void Swing(D3DXVECTOR3 playerPos, bool bFirstCollision);                            // �X�C���O
    void Smash(D3DXVECTOR3 playerPos, bool bFirstCollision);                            // �X�}�b�V��
    void Spike(D3DXVECTOR3 playerPos, bool bFirstCollision);                            // �X�p�C�N
    void CatchReady(D3DXVECTOR3 playerPos);                                             // �L���b�`�̍\��
    void Bunt(D3DXVECTOR3 playerPos);                                                   // �o���g
    void Absorb(D3DXVECTOR3 playerPos);                                                 // �z��
    bool IsAttackBall(D3DXVECTOR3 attackPos, D3DXVECTOR3 attackSize, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag);   // �U���̓����蔻��
    void LeaveWepAfterimage(void);                                                      // ����̎c�����c��
    void RotControl();                                                                  // ��]����
    void AttackMotion(void);                                                            // �U�����[�V�����̊Ǘ�
    void DamageUpdate(D3DXVECTOR3 pos);                                                 // �����X�V����
    void DamageMotion(void);                                                            // �������[�V�����̊Ǘ�
    void GainSpGauge(bool bExAdd = false);                                              // �K�E�Q�[�W�㏸

	void ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumTexture);					// �N���b�s���O�}�X�N�K�p //�r�c�ǉ�
	void ApplyMusk(D3DXVECTOR3 pos, D3DXVECTOR3 size);									// �e�N�X�`���Ȃ��N���b�s���O�}�X�N�K�p //�r�c�ǉ�

    /*========================================================
    // �J�X�^�}�C�Y�̕��������
    //======================================================*/
    static void CustomPartsName(char* pName, int nPartsType);
    void CustomExName(char* pName, int exFlag);
    void CustomSpName(char* pName);

    /*========================================================
    // �K�E�Z
    //======================================================*/
    void EmitSpShot(void);  // ���̒��ŕ��򂵂āA�e�K�E�Z�̑ł������ۂ̏���������  
    void SpHeal(void);
    void SpDecoy(void);
    void SpWallThrough(void);
	void SpBarrier(void);
    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    void SetAddPoint(void) { m_nPoint++; }
    void SetStopTime(int nStopTime) { m_nCntStopTime = nStopTime; }
    void SetSpGaugeCurrent(float fSpGauge) { m_fSpGaugeCurrent = fSpGauge; }
    void SetDisp(bool bDisp) { m_bDisp = bDisp; }
    void ResetStatusEveryRound(void);   // �����E���h���Z�b�g����X�e�[�^�X
    void SetDispAbility(bool bDisp) { m_bDispAbility = bDisp; }
    void SetCustomWeaponLook(void) { if (m_nCntAttackAnimTime <= 0)m_nCntAttackAnimTime = PLAYER_WEAPON_LOOK_FRAME; }
    void SetRank(RANK rank) { m_rank = rank; }

    /*========================================================
    // �Q�b�^�[
    //======================================================*/
    bool GetDisp(void) { return m_bDisp; }
    float GetAtk(void) { return m_fAtk; }
    float GetDef(void) { return m_fDef; }
    float GetSpd(void) { return m_fSpd; }
    float GetWei(void) { return m_fWei; }
    ControlInput *GetControlInput(void) { return &m_controlInput; }
    int GetIdxCreate(void) { return m_nIdxCreate; }
    int GetIdxControlAndColor(void) { return m_nIdxControlAndColor; }
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
    bool GetUseControllerEffect(void);       // �R���g���[���̐U�����g�p���邩�ǂ���
    bool GetDispAbility(void) { return m_bDispAbility; }

private:
    bool m_bMannequin;                       // �}�l�L�����ǂ���
    ControlInput m_controlInput;             // ���͊Ǘ�
    bool m_bUseKeyboard;                     // �L�[�{�[�h���g�����ǂ���
    AI_LEVEL m_AIlevel;                      // AI���x��
    CAi *m_pAI;                              // AI�ւ̃|�C���^
    CUI *m_pUI_HP;                           // HP�Q�[�W�ւ̃|�C���^
    CUI *m_pUI_HP_red;                       // ��HP�Q�[�W�ւ̃|�C���^
    CUI *m_pUI_SP;                           // SP�Q�[�W�ւ̃|�C���^
    CUI *m_apUI_Stock[PLAYER_MAX_STOCK];     // �X�g�b�N�ւ̃|�C���^
    CUI *m_pUI_Playable;                     // �v���C���[�\���ւ̃|�C���^
    CNumberArray *m_pNumArray_SP;            // SP�p�[�Z���g�ւ̃|�C���^
    CEffect3D *m_pEffect3d_Shadow;           // �e�ւ̃|�C���^
    CUI *m_pUI_Custom_Atk;                   // �J�X�^�}�C�Y���_�U���Q�[�W
    CUI *m_pUI_Custom_Def;                   // �J�X�^�}�C�Y���_�h��Q�[�W
    CUI *m_pUI_Custom_Spd;                   // �J�X�^�}�C�Y���_�����Q�[�W
    CUI *m_pUI_Custom_Wei;                   // �J�X�^�}�C�Y���_�d���Q�[�W
    CUI *m_pUI_Custom_Ability;               // �J�X�^�}�C�Y���_�A�r���e�B
    CText *m_pText_Custom_Ex_Head;           // �J�X�^�}�C�Y���_����\�͖�_��
    CText *m_pText_Custom_Ex_Up;             // �J�X�^�}�C�Y���_����\�͖�_��
    CText *m_pText_Custom_Ex_Down;           // �J�X�^�}�C�Y���_����\�͖�_�r
    CText *m_pText_Custom_Ex_Wep;            // �J�X�^�}�C�Y���_����\�͖�_����
    CText *m_pText_Custom_Sp;                // �J�X�^�}�C�Y���_�K�E�Z��
    bool m_bDispAbility;                     // �J�X�^�}�C�Y��ʂŏڍׂ�\�����邩�ǂ���
                        
    int m_nIdxCreate;                        // �����̃C���f�b�N�X
    int m_nIdxControlAndColor;               // �R���g���[���ƃJ���[�̃C���f�b�N�X
    int m_nPoint;                            // �|�C���g
    int m_nStock;                            // �X�g�b�N��
    D3DXVECTOR3 m_startPos;                  // �J�n�ʒu��ێ�
    D3DXVECTOR3 m_startRot;                  // �J�n������ێ�
                                             
    D3DXVECTOR3 m_move;                      // �ړ���
    D3DXVECTOR3 m_moveOld;                   // 1F�O�̈ړ���
    int m_exFlag;                            // ����\�̓t���O
    ATTACK_STATE m_attackState;              // �U�����
                                             
    int m_nCntAttackTime;                    // �U�����Ԃ��J�E���g
    bool m_bGround;                          // �n�ʂɂ��邩�ǂ���
                                             
    bool m_bDisp;                            // �\���t���O
    int m_nCntTakeDamageTime;                // �U�����󂯂Ă��鎞��
    float m_fLife;                           // �̗�
    float m_fLife_red;                       // �ԑ̗�
    bool m_bUsedLuckyGuard;                  // ���b�L�[�K�[�h���g�������ǂ���
    DAMAGE_STATE m_damageState;              // �������
                                             
    int m_nCntLandingTime;                   // ���n����
    bool m_bGroundOld;                       // 1F�O�A�n�ʂɂ������ǂ���

    int m_anNumCustomParts[CUSTOM_PARTS_MAX];// �J�X�^�}�C�Y�p�[�c�̔ԍ�
    float m_fAtk;                            // �U��
    float m_fDef;                            // �h��i�ő�HP�ɂ��Ȃ�j
    float m_fSpd;                            // ����
    float m_fWei;                            // �d��
    float m_afParam[PARAM_MAX];              // �ėp�f�[�^
    int m_nModelPosDefUp;                    // ���f���̏����ʒu�f�[�^�i�㔼�g�j
    int m_nModelPosDefDown;                  // ���f���̏����ʒu�f�[�^�i�����g�j
                                             
    int m_nCntTurnTime;                      // �����]������
    int m_nCntPressJump;                     // �W�����v������������
    bool m_bDiveCheck;                       // �}�~���ɂȂ邩�ǂ����̃`�F�b�N
    bool m_bDiving;                          // �}�~����
    bool m_bUsedSecondJump;                  // ��i�W�����v���g�������ǂ���
    bool m_bSquat;                           // ���Ⴊ��ł��邩�ǂ���
    int m_nCntStartGlide;                    // ���󂪎n�܂�܂ł̃J�E���^
                                             
    int m_nCntSwingCharge;                   // �X�C���O�̃`���[�W����
    bool m_bUseAvoidMultipleHits;            // ���i�q�b�g������g�����ǂ���
    bool m_bHitAttackThisFrame;              // ���̃t���[���ōU���������������ǂ���
    int m_nCntStopTime;                      // �d�����Ԃ̃J�E���^
    int m_nCntAttackAnimTime;                // �U���A�j���[�V�����̍Œ�ۏ�
                                             
    int m_spShot;                            // �K�E�Z
    float m_fSpGaugeCurrent;                 // ���݂̕K�E�Q�[�W��
    float m_fSpGaugeMax;                     // �ő�K�E�Q�[�W��
                                             
    D3DXVECTOR3 m_collisionSizeAttack;       // �U���Փ˃T�C�Y
    D3DXVECTOR3 m_collisionSizeDeffence;     // �h��Փ˃T�C�Y

	CClipingMusk* m_pClipingMusk;			 // �N���b�s���O�}�X�N
    int m_nNumWep;                           // ����̃��f���ԍ�
    int m_nCntStopRedLifeTime;               // �ԃQ�[�W��~���Ԃ̃J�E���^
    RANK m_rank;                             // ����
    D3DXVECTOR3 m_hipPosOld;                 // 1F�O�̍��̈ʒu

	bool m_bSpBarrier;						 // �K�E�Z�ɂ��o���A���l�����Ă��邩�ǂ���
    int m_nCntSpGaugeMaxTime;                // �K�E�Q�[�WMAX����
    int m_voiceSet;                          // �{�C�X�Z�b�g

    //===================================    
    // ����\�͑Ή�����                      
    //===================================    
    bool m_bUsedThreeJump;                   // �O�i�W�����v���g�������ǂ���
    int m_nSwingChargeMax;                   // �ő嗭�ߎ���
};

#endif