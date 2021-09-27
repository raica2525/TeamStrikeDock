//====================================================================
//
// �{�[���̏��� (ball.h)
// Author : �㓡�T�V���A�ɓ��z��i�O�Ձj
//
//====================================================================
#ifndef _BALL_H_
#define _BALL_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// �O���錾
//================================================
class CPlayer;
class COutline;

//================================================
// �}�N����`
//================================================

// ��{���
#define BALL_COLLISION_SIZE_SIDE 60.0f
#define BALL_COLLISION_SIZE D3DXVECTOR3(BALL_COLLISION_SIZE_SIDE, BALL_COLLISION_SIZE_SIDE, BALL_COLLISION_SIZE_SIDE)
#define BALL_COLLISION_POS_SLIDE_Y -30.0f
#define BALL_COLLISION_POS_SLIDE D3DXVECTOR3(0.0f, BALL_COLLISION_POS_SLIDE_Y, 0.0f)
#define BALL_ROT_SPEED D3DXToRadian(10.0f)        // ��]���x
#define BALL_FIRST_MOVE_ANGLE D3DXVECTOR3(sinf(D3DXToRadian(180.0f)), cosf(D3DXToRadian(180.0f)), 0.0f)
#define BALL_FIRST_SPEED 16.0f                    // �J�n���̑���
#define BALL_MAX_SPEED 999.0f                     // �{�[���̍ő呬�x
#define BALL_START_BUTTLE_STOP_POS_Y 180.0f       // �J�n���ɂǂ̍����Ŏ~�߂邩
#define BALL_DISPING_TIME 170                     // �J�n���̔z�u����
#define BALL_DISPING_SCALE_UP_RATE 0.05f          // �J�n���̃X�P�[���g�劄��
#define BALL_WAVE_FIRST_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)
#define BALL_MOVE_Y_DONT_MOVE_BORDER 1.0f         // �����Ă��Ȃ��Ƃ݂Ȃ��c���̒l�i�{�[����ό`������ۂɎg���j
#define BALL_OUTLINE_RATE 1.2f                    // �A�E�g���C���̊g�劄��

// �U������
#define BALL_SHOOT_STOP_LEAST_FRAME 8        // �ŏ��d�����ԁi�v�����j
#define BALL_SHOOT_STOP_REFRECT_FRAME 2      // �ǂɔ��˂��Ă�Ƃ��ɁA�~�܂�t���[����
#define BALL_SHOOT_STOP_LUCKY_GUARD_FRAME 30 // ���b�L�[�K�[�h���ɁA�~�܂�t���[����
#define BALL_SHOOT_STOP_TIME_RATE 0.5f       // ���݂̃X�s�[�h�ɂ����čd�����Ԃ��o�����߂̊���
#define BALL_SHOOT_BIG_HIT_SPEED 50.0f       // ��q�b�g�X�s�[�h�i�d�����Ԃ��n�܂�j
#define BALL_SHOOT_ULTRA_HIT_SPEED 100.0f    // �E���g���q�b�g�X�s�[�h�i���]�������n�܂�j
#define BALL_SHOOT_MAX_STOP_TIME 150         // �ő�d������
#define BALL_QUICK_SHOOT_MAX_STOP_TIME 15    // �������̍ő�d������
#define BALL_NOT_ANYONE -1                   // �{�[�����N�̂��̂ł��Ȃ����
#define BALL_THROW_FIRST_SPEED 100.0f        // �����̏���
#define BALL_BUNT_SPEED 42.5f                // �o���g�{�[���̑��x
#define BALL_BUNT_GRAVITY 1.5f               // �o���g�{�[���̏d��
#define BALL_BUNT_MAX_FALL_SPEED -25.0f      // �o���g�{�[���̍ő嗎�����x
#define BALL_ABSORB_POS_X 100.0f             // �z�������{�[�����ǂꂾ���O�ɏo����
#define BALL_ABSORB_SPEED 20.0f              // �z�����x
#define BALL_ABSORB_CLOSE_PLAYER 150.0f      // �z���Ńv���C���[�ɋ߂��Ƃ݂Ȃ��l
#define BALL_ABSORB_MAX_TIME 300             // �z�����Ԃ̍ő�J�E���^
#define BALL_SHOOT_EFFECT_LEAST_FRAME 10     // �ŏ��R���g���[���̐U���t���[����
#define BALL_UPDATE_METER_MIN_STOP_FRAME 4   // �{�[�����˃Q�[�W���X�V����Œ�d���t���[����

//================================================
// �N���X�錾
//================================================

// �{�[���N���X
class CBall : public CScene3D
{
public:

    // �V���[�g�̃t���O
    typedef enum
    {
        SHOOT_FLAG_NONE = 0,                  // �Ȃ�
        SHOOT_FLAG_QUICK = 0x001 << 0,        // ���ł�
        SHOOT_FLAG_THROW = 0x001 << 1,        // ����
        SHOOT_FLAG_BUNT = 0x001 << 2,         // �o���g
        SHOOT_FLAG_FAR_HIT = 0x001 << 3,      // �t�@�[�q�b�g
        SHOOT_FLAG_ABSORB = 0x001 << 4,       // �z��
    }SHOOT_FLAG;

    // �ǂɓ����������̌���
    typedef enum
    {
        HIT_WALL_NONE = 0,
        HIT_WALL_UP,
        HIT_WALL_DOWN,
        HIT_WALL_RIGHT,
        HIT_WALL_LEFT
    }HIT_WALL;

    // �z���̃t���O
    typedef enum
    {
        ABSORB_FLAG_NONE = 0,                  // �Ȃ�
        ABSORB_FLAG_MOVING = 0x001 << 0,       // �ړ���
        ABSORB_FLAG_REACHED = 0x001 << 1,      // ������
    }ABSORB_FLAG;

    CBall();
    ~CBall();
    void ResetMenberVariables(void);                                // �����o�ϐ��̃��Z�b�g
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Reset(D3DXVECTOR3 pos);                                    // �{�[���̍Đݒ�
    void Uninit(void);
    void Update(void);
    void UpdateCornerPos(D3DXVECTOR3 pos);                          // �΂ߓ����蔻��p�́A�l�p�X�V
    void AuraEffect(void);                                          // �{�[���̃I�[���G�t�F�N�g
    void AppearEffect(void);                                        // �{�[���̏o���G�t�F�N�g
    void LocusEffect(void);											// �{�[���̋O�ՃG�t�F�N�g
    void ChangeScaleAndRot(void);                                   // �X�P�[���Ɖ�]�̕ύX
    void Draw(void);
    static CBall *Create(D3DXVECTOR3 pos, bool bUseCollision);
    D3DXVECTOR3 StartButtleMove(D3DXVECTOR3 pos);                   // �o�g���J�n���̈ړ��i�ŏ��̗���or�������l�ւ̒Ǐ]�j
    D3DXVECTOR3 Move(D3DXVECTOR3 pos);                              // �ړ�
    D3DXVECTOR3 Absorb(void);                                       // �z��
    D3DXVECTOR3 HitWall(D3DXVECTOR3 pos, HIT_WALL hitWall);         // �ǂɓ����������̋��ʏ���
    D3DXVECTOR3 StayInGame(D3DXVECTOR3 pos);                        // �Q�[�����ɂƂǂ߂�
    void Throw(void);                                               // ������ꒆ

    int Shoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, CPlayer*pPlayer);    // �ł���鎞�̏���
    void ResetByShoot(void);                                        // �V���[�g�ɂ���ă��Z�b�g����ϐ�
    int Launch(D3DXVECTOR3 startPos, int nCntStopTime = 0);         // �{�[����ł��グ��

    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    void SetStopTime(const int nCntStopTime);   // �d�����Ԃ����߂�
    void SetAbsorb(CPlayer *pPlayer);           // �z����ݒ�
    void SetMoveAngle(D3DXVECTOR3 moveAngle) { m_moveAngle = moveAngle; }
    void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
    void SetStartButtle(bool bStartButtle) { m_bStartButtle = bStartButtle; }
    void SetWhoShooting(int nWhoShooting) { m_nWhoShooting = nWhoShooting; }
    void SetHitWallMax(int nHitWallMax) { m_nCntHitWallMax = nHitWallMax; }
    void SetWallThrough(int nWallThrough) { m_bWallThrough = true; m_nCntWallThrough = nWallThrough; }
    void SetDispOff(void) { m_bDisp = false;  m_nCntDispingTime = 0; m_bUseCollision = false; }
    void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }

    /*========================================================
    // �Q�b�^�[
    //======================================================*/
    bool GetUseCollision(void) { return m_bUseCollision; }
    float GetSpeed(void) { return m_fSpeed; }
    int GetWhoShooting(void) { return m_nWhoShooting; }
    D3DXVECTOR3 GetCornerPos(int nCnt) { return m_aCornerPos[nCnt]; }
    int GetAbsorbFlag(void) { return m_absorbFlag; }
    int GetStopTime(void) { return m_nCntCurrentStopTime; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
    int GetWhoAbsorbing(void) { return m_nWhoAbsorbing; }
    D3DXVECTOR3 GetCollisionPos(void) { return GetPos() + BALL_COLLISION_POS_SLIDE; }
    int GetDisp(void) { return m_bDisp; }

private:
    COutline *m_pOutline;       // �A�E�g���C��
    int m_shootFlag;            // �V���[�g�̃t���O
    D3DXVECTOR3 m_moveAngle;    // �ړ�����p�x
    float m_fSpeed;             // ����
    
    int m_nCntCurrentStopTime;  // ���݂̍d�����Ԃ̃J�E���^
    int m_nStopTimeAtShoot;     // ���ˎ��̍d������
    bool m_bUseCollision;       // �����蔻����g�����ǂ���
    bool m_bNeverUseCollision;  // �����蔻����΂Ɏg��Ȃ����ǂ���
    bool m_bStartButtle;        // �o�g���J�n���ォ�ǂ���

    int m_nWhoShooting;         // �N���ł����{�[����
    CPlayer *m_pPlayer;         // �v���C���[�̃|�C���^

    bool m_bAfterThrow;         // �����o���ꂽ���ォ
    int m_nCntBuntTime;         // �o���g���Ԃ̃J�E���^

    D3DXVECTOR3 m_aCornerPos[4];// �l�p�̈ʒu
    D3DXVECTOR3 m_posOld;       // 1F�O�̈ʒu

    int m_absorbFlag;           // �z���̃t���O
    int m_nWhoAbsorbing;        // �N�����݋z�����Ă��邩
    int m_nCntAbsorbTime;       // �z�����Ԃ̃J�E���^

    bool m_bDisp;               // �\�����Ă��邩�ǂ���
    int m_nCntDispingTime;      // �z�u���̃J�E���^
    int m_nCntNoLineStopTime;   // ���S�⑊�E�A���b�L�[�K�[�h�̍d�����Ԃ��J�E���g�i�W�������~�߂�p�j

    //===================================
    // �K�E�Z�Ή�����
    //===================================
    int m_nCntHitWall;          // �ǂɓ���������
    int m_nCntHitWallMax;       // �ǂɓ������Ă����ő吔

    bool m_bWallThrough;        // �ǂ��ђʂ��邩�ǂ���
    int m_nCntWallThrough;      // ���Ɖ���ǂ��ђʂ��邩�ǂ���
};

#endif