//===============================================
//
// �{�[���̏��� (ball.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "ball.h"
#include "manager.h"
#include "renderer.h"
#include "debug.h"
#include "game.h"
#include "score.h"
//#include "effect3d.h"
#include "library.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBall::CBall() :CScene3D(CScene::OBJTYPE_BALL)
{
    m_shootFlag = SHOOT_FLAG_NONE;
    m_moveAngle = BALL_FIRST_MOVE_ANGLE;
    m_fSpeed = BALL_FIRST_SPEED;

    m_nCntStopTime = 0;
    m_bUseCollision = false;
    m_bStartButtle = true;

    m_nWhoShooting = BALL_NOT_ANYONE;
    m_pPlayer = NULL;

    m_bAfterThrow = false;
    m_nCntBuntTime = 0;

    memset(m_aCornerPos, 0, sizeof(m_aCornerPos));
    m_posOld = DEFAULT_VECTOR;

    m_absorbFlag = ABSORB_FLAG_NONE;
    m_nWhoAbsorbing = BALL_NOT_ANYONE;
    m_nCntAbsorbTime = 0;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBall::~CBall()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CBall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene3D::Init(pos, size);

    // �X�P�[����ݒ�
    SetScale(size);

    // ���f���f�[�^���o�C���h
    BindModelData(0);   // �{�[����0��

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CBall::Uninit(void)
{
    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CBall::Update(void)
{
    // 1F�O�̈ʒu���L��
    m_posOld = GetPos();

    // �����Ȃ�
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_THROW))
    {
        Throw();
    }
    else
    {
        // �ʒu�A�����擾
        D3DXVECTOR3 pos = GetPos();
        D3DXVECTOR3 rot = GetRot();

        // �z����ԂłȂ��Ȃ�A�ړ�����
        pos = Move(pos);

        // �o�g���J�n��ԂȂ�
        if (m_bStartButtle)
        {
            pos = StartButtleMove(pos);
        }

        // �z����ԂȂ�
        if (m_nWhoAbsorbing != BALL_NOT_ANYONE)
        {
            pos = Absorb();
        }

        // �}�b�v����
        CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
        if (pos.x + (BALL_COLLISION_SIZE_SIDE / 2) > mapLimit.fWidth)
        {
            HitWall(HIT_WALL_RIGHT);
            pos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
            m_moveAngle.x *= -1;
        }
        else if (pos.x - (BALL_COLLISION_SIZE_SIDE / 2) < -mapLimit.fWidth)
        {
            HitWall(HIT_WALL_LEFT);
            pos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
            m_moveAngle.x *= -1;
        }
        if (pos.y + BALL_COLLISION_SIZE_SIDE > mapLimit.fHeight)
        {
            HitWall(HIT_WALL_UP);
            pos.y = mapLimit.fHeight - BALL_COLLISION_SIZE_SIDE;
            m_moveAngle.y *= -1;
        }
        else if (pos.y - (BALL_COLLISION_SIZE_SIDE / 2) < 0)
        {
            HitWall(HIT_WALL_DOWN);
            pos.y = (BALL_COLLISION_SIZE_SIDE / 2);
            m_moveAngle.y *= -1;
        }

        // �l�p�̈ʒu���X�V
        UpdateCornerPos(pos);

        // �ʒu�A�����𔽉f
        SetPos(pos);
        SetRot(rot);

#ifdef COLLISION_TEST
        if (m_bUseCollision)
        {
            CDebug::Create(pos + D3DXVECTOR3(0.0f, 0.0f, -50.0f), BALL_COLLISION_SIZE, CDebug::TYPE_MOMENT);
        }
#endif // COLLISION_TEST

        //// ���C�t��0�ȉ��Ȃ�A���g�p��
        //if (m_nLife <= 0)
        //{
        //    // �I������
        //    Uninit();
        //}
    }
}

//=============================================================================
// �l�p�̈ʒu���X�V
// Author : �㓡�T�V��
//=============================================================================
void CBall::UpdateCornerPos(D3DXVECTOR3 pos)
{
    // �^�񒆂̍��W���o��
    D3DXVECTOR3 centerPos = (pos + m_posOld) / 2;

    // �ӂ̒��������߂�
    float fLengthX = fabsf(pos.x - m_posOld.x);
    float fLengthY = fabsf(pos.y - m_posOld.y);

    // ��]�O�̎l�p�̈ʒu�����߂�
    if (fLengthX > fLengthY)
    {
        // ��]�O�̎l�p�̈ʒu���o���i�����j
        m_aCornerPos[0] = centerPos + D3DXVECTOR3(-(fLengthX / 2) - (BALL_COLLISION_SIZE_SIDE / 2), (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[1] = centerPos + D3DXVECTOR3(-(fLengthX / 2) - (BALL_COLLISION_SIZE_SIDE / 2), -(BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[2] = centerPos + D3DXVECTOR3((fLengthX / 2) + (BALL_COLLISION_SIZE_SIDE / 2), -(BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[3] = centerPos + D3DXVECTOR3((fLengthX / 2) + (BALL_COLLISION_SIZE_SIDE / 2), (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
    }
    else
    {
        // ��]�O�̎l�p�̈ʒu���o���i�c���j
        m_aCornerPos[0] = centerPos + D3DXVECTOR3(-(BALL_COLLISION_SIZE_SIDE / 2), (fLengthY / 2) + (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[1] = centerPos + D3DXVECTOR3(-(BALL_COLLISION_SIZE_SIDE / 2), -(fLengthY / 2) - (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[2] = centerPos + D3DXVECTOR3((BALL_COLLISION_SIZE_SIDE / 2), -(fLengthY / 2) - (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
        m_aCornerPos[3] = centerPos + D3DXVECTOR3((BALL_COLLISION_SIZE_SIDE / 2), (fLengthY / 2) + (BALL_COLLISION_SIZE_SIDE / 2), 0.0f);
    }

    // �ړ������p�x���o��
    float fMoveAngle = atan2((pos.x - m_posOld.x), (pos.y - m_posOld.y));

    // ��]��̎l�p�̈ʒu���o���i����A�����A�E���A�E��̏��j
    m_aCornerPos[0] = caclRotate(m_aCornerPos[0], centerPos, fMoveAngle);
    m_aCornerPos[1] = caclRotate(m_aCornerPos[1], centerPos, fMoveAngle);
    m_aCornerPos[2] = caclRotate(m_aCornerPos[2], centerPos, fMoveAngle);
    m_aCornerPos[3] = caclRotate(m_aCornerPos[3], centerPos, fMoveAngle);
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CBall::Draw(void)
{
    CScene3D::Draw();
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CBall * CBall::Create(D3DXVECTOR3 pos, bool bUseCollision)
{
    // �|�C���^��p��
    CBall *pBall = NULL;

    // �������m��
    pBall = new CBall;

    // ������
    pBall->Init(pos, DEFAULT_SCALE);

    // �����o�ϐ������т���
    pBall->m_bUseCollision = bUseCollision;

    return pBall;
}

//=============================================================================
// �o�g���J�n���̈ړ�����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CBall::StartButtleMove(D3DXVECTOR3 pos)
{
    // �ϐ��錾
    D3DXVECTOR3 returnPos = pos;

    // �ǂ��Ŏ~�߂邩
    if (pos.y < BALL_START_BUTTLE_STOP_POS_Y)
    {
        pos.y = BALL_START_BUTTLE_STOP_POS_Y;
        m_moveAngle = DEFAULT_VECTOR;
    }

    return returnPos;
}

//=============================================================================
// �ړ�����
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CBall::Move(D3DXVECTOR3 pos)
{
    // �ϐ��錾
    D3DXVECTOR3 returnPos = pos;
    D3DXVECTOR3 moveAngle = m_moveAngle;    // ����̈ړ��p�x
    D3DXVECTOR3 move = DEFAULT_VECTOR;      // ����̈ړ���
    float fSpeed = m_fSpeed;                // ����̑��x
    float fGravity = 0.0f;                  // �d��

    // �e���ꏈ��
    if (m_bAfterThrow)
    {
        // ��������͈�葬�x�ȉ��Ȃ�A���̑��x��
        if (fSpeed < BALL_THROW_FIRST_SPEED)
        {
            fSpeed = BALL_THROW_FIRST_SPEED;
        }
    }
    else if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // �o���g�{�[���͈�葬�x���d�ʂ̉e�����󂯂�
        fSpeed = BALL_BUNT_SPEED;
        m_nCntBuntTime++;
        fGravity = m_nCntBuntTime * BALL_BUNT_GRAVITY;
    }

    // �d�����Ԃ̃J�E���^
    if (m_nCntStopTime > 0)
    {
        m_nCntStopTime--;

        // �Ō�̃t���[���ŁA�����蔻��𕜊�������
        if (m_nCntStopTime == 0)
        {
            m_bUseCollision = true;
        }
    }
    else
    {
        // �ړ��ʂ�����
        move = (moveAngle * fSpeed) - D3DXVECTOR3(0.0f, fGravity, 0.0f);

        // �o���g�{�[���Ȃ�Ay���ړ��ɏ��������
        if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
        {
            if (move.y < BALL_BUNT_MAX_FALL_SPEED)
            {
                move.y = BALL_BUNT_MAX_FALL_SPEED;
            }
        }

        // �ʒu�Ɉړ��ʂ����Z
        returnPos += move;
    }

    return returnPos;
}

//=============================================================================
// �z������
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CBall::Absorb(void)
{
    // �ϐ��錾
    D3DXVECTOR3 returnPos = GetPos();   // ������O�̏����̈ʒu�ɍ��E����Ȃ�

    // �v���C���[�̃|�C���^�������Ă���Ȃ�
    if (m_pPlayer)
    {
        // �ړ����Ȃ�
        if (IS_BITON(m_absorbFlag, ABSORB_FLAG_MOVING))
        {
            // �ړI�n�ֈʒu���߂Â���
            D3DXVECTOR3 targetPos = m_pPlayer->GetPos() + D3DXVECTOR3(m_pPlayer->GetActiveCollisionSize().x / 2 + BALL_ABSORB_POS_X,
                m_pPlayer->GetActiveCollisionSize().y / 2, 0.0f);
            float fAngleToTarget = GetAngleToTarget3D2D(returnPos, targetPos);
            D3DXVECTOR3 move = DEFAULT_VECTOR;
            move.x = sinf(fAngleToTarget) * BALL_ABSORB_SPEED;
            move.y = cosf(fAngleToTarget) * BALL_ABSORB_SPEED;
            returnPos += move;

            // �ړI�n�ɋ߂��Ȃ�
            float fDistance = GetDistance2D(returnPos, targetPos);
            if (fDistance < BALL_ABSORB_CLOSE_PLAYER)
            {
                // �ړ����瓞����
                BITOFF(m_absorbFlag, ABSORB_FLAG_MOVING);
                BITON(m_absorbFlag, ABSORB_FLAG_REACHED);
            }
        }
        else if (IS_BITON(m_absorbFlag, ABSORB_FLAG_REACHED))
        {
            // �������Ă���Ȃ�
            // �E�����Ȃ�
            if (m_pPlayer->GetRot().y == PLAYER_ROT_RIGHT)
            {
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(m_pPlayer->GetActiveCollisionSize().x / 2 + BALL_ABSORB_POS_X,
                    m_pPlayer->GetActiveCollisionSize().y / 2, 0.0f);
            }
            else if (m_pPlayer->GetRot().y == PLAYER_ROT_LEFT)
            {
                // �������Ȃ�
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(-(m_pPlayer->GetActiveCollisionSize().x / 2 + BALL_ABSORB_POS_X),
                    m_pPlayer->GetActiveCollisionSize().y / 2, 0.0f);
            }
        }
    }

    // �z�����Ԃ̃J�E���g�_�E��
    if (m_nCntAbsorbTime > 0)
    {
        m_nCntAbsorbTime--;

        // �Ō�̃t���[���őł��グ��
        if (m_nCntAbsorbTime == 0)
        {
            Launch(returnPos);
        }
    }

    return returnPos;
}

//=============================================================================
// �ǂɓ����������̋��ʏ���
// Author : �㓡�T�V��
//=============================================================================
void CBall::HitWall(HIT_WALL hitWall)
{
    // �����o�ϐ��̒���
    m_bAfterThrow = false;

    // �o���g�{�[����ԂȂ�
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // ��ǈȊO�Ȃ�
        if (hitWall != HIT_WALL_UP)
        {
            // �o���g�{�[����Ԃ��������A�Ō�ɑł����l�̃{�[���ɂ��āA�c�ړ���
            BITOFF(m_shootFlag, SHOOT_FLAG_BUNT);
            m_nCntBuntTime = 0;
            m_moveAngle = BALL_FIRST_MOVE_ANGLE;
            if (m_pPlayer)
            {
                m_nWhoShooting = m_pPlayer->GetPlayable();
            }
        }
        else
        {
            // ��ǂȂ�ړ��̌����ɉe����^���Ȃ�
            m_moveAngle.y *= -1;
        }
    }
    else
    {
        // �o���g�{�[���ȊO�Ȃ�A�ǂɓ����������ɂ킸���ɍd������
        m_nCntStopTime = BALL_SHOOT_STOP_REFRECT_FRAME;
    }
}

//=============================================================================
// �Q�[�����ɂƂǂ߂�
// Author : �㓡�T�V��
//=============================================================================
D3DXVECTOR3 CBall::StayInGame(D3DXVECTOR3 pos)
{
    D3DXVECTOR3 returnPos = pos;

    // �}�b�v����
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    if (returnPos.x + (BALL_COLLISION_SIZE_SIDE / 2) > mapLimit.fWidth)
    {
        returnPos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
    }
    else if (returnPos.x - (BALL_COLLISION_SIZE_SIDE / 2) < -mapLimit.fWidth)
    {
        returnPos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
    }
    if (returnPos.y + BALL_COLLISION_SIZE_SIDE > mapLimit.fHeight)
    {
        returnPos.y = mapLimit.fHeight - BALL_COLLISION_SIZE_SIDE;
    }
    else if (returnPos.y - (BALL_COLLISION_SIZE_SIDE / 2) < 0)
    {
        returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2);
    }

    return returnPos;
}

//=============================================================================
// �����̏���
// Author : �㓡�T�V��
//=============================================================================
void CBall::Throw(void)
{
    // �ʒu���擾
    D3DXVECTOR3 pos = GetPos();

    // �v���C���[�̏��������Ă���Ȃ�
    if (m_pPlayer)
    {
        // �v���C���[�̍���ɌŒ�
        pos = D3DXVECTOR3(m_pPlayer->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._41,
            m_pPlayer->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._42,
            m_pPlayer->GetParts(CPlayer::PARTS_LHAND)->GetWorldMtx()._43);

        // �v���C���[�̓������I�������A������Ԃ�����
        if (m_pPlayer->GetAttackTime() == ATTACK_THROW_END_FRAME)
        {
            // �������㔻���true��
            m_bAfterThrow = true;

            // ������Ԃ�����
            BITOFF(m_shootFlag, SHOOT_FLAG_THROW);

            // �����蔻��𕜊�������
            m_bUseCollision = true;

            // �U���p�x���o��
            float fAttackAngle = 90.0f;

            // �v���C���[�̌������������Ȃ�A��΂��������t��
            if (m_pPlayer->GetRot().y == PLAYER_ROT_LEFT)
            {
                fAttackAngle *= -1;
            }

            // �X�e�B�b�N�̊p�x�ɂ���āA�U���p�x��ς���
            if (m_pPlayer->GetControlInput()->bTiltedLeftStick)
            {
                // �X�e�B�b�N���ォ���������Ă���Ȃ�A�^����
                if (STICK_SPIKE_VERTICAL(m_pPlayer->GetControlInput()->fLeftStickAngle))
                {
                    fAttackAngle = 180.0f;
                }
            }

            // ��΂�����������
            m_moveAngle.x = sinf(D3DXToRadian(fAttackAngle));
            m_moveAngle.y = cosf(D3DXToRadian(fAttackAngle));
        }
    }

    // �ʒu�𔽉f
    SetPos(pos);
}

//=============================================================================
// ���Ƃ��̏���
// Author : �㓡�T�V��
//=============================================================================
int CBall::Shoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, CPlayer*pPlayer)
{
    // �������̃����o�ϐ��𒲐�
    ResetByShoot();

    // �o���g�{�[����ԂȂ�
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // �o���g�{�[����Ԃ��������A���ł�
        BITOFF(m_shootFlag, SHOOT_FLAG_BUNT);
        BITON(m_shootFlag, SHOOT_FLAG_QUICK);
    }

    // �t�@�[�q�b�g�Ȃ�
    if (IS_BITON(flag, SHOOT_FLAG_FAR_HIT))
    {
        // �ʒu���U�������ʒu�Ɍ��т���
        attackCenterPos = StayInGame(attackCenterPos);
        SetPos(attackCenterPos);
    }

    // �v���C���[�̏����擾���A�N�������������L�^
    m_pPlayer = pPlayer;
    m_nWhoShooting = m_pPlayer->GetPlayable();

    // �󂯎�����V���[�g�t���O�̍l��
    if (IS_BITON(flag, SHOOT_FLAG_THROW))
    {
        // �����蔻���������
        m_bUseCollision = false;

        // �����̃t���O���I���ɂ���
        BITON(m_shootFlag, SHOOT_FLAG_THROW);

        // ���ł��t���O�������i�ł������ƂɂȂ邽�߁j
        BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);

        // �֐��𔲂���
        return 0;
    }
    else if (IS_BITON(flag, SHOOT_FLAG_BUNT))
    {
        // �o���g�̃t���O���I���ɂ���
        BITON(m_shootFlag, SHOOT_FLAG_BUNT);

        // �ړ��p�x���󂯎��
        m_moveAngle = moveAngle;

        // �N�̃{�[���ł��Ȃ��Ȃ�
        m_nWhoShooting = BALL_NOT_ANYONE;

        // �֐��𔲂���
        return 0;
    }
    else if (IS_BITON(flag, SHOOT_FLAG_ABSORB))
    {
        // �z���̐ݒ�
        SetAbsorb(pPlayer);

        // �֐��𔲂���
        return 0;
    }
    else
    {
        // �ŏ��̐ڐG�Ȃ�
        if (bFirstCollision)
        {
            // �����蔻���������
            m_bUseCollision = false;

            // �d�����Ԃ����߂�
            m_nCntStopTime = (int)(m_fSpeed * BALL_SHOOT_STOP_TIME_RATE);

            // ���������ł��Ȃ�
            if (IS_BITON(flag, SHOOT_FLAG_QUICK))
            {
                BITON(m_shootFlag, SHOOT_FLAG_QUICK);
            }

            // �����o�ϐ������ł��Ȃ�
            if (IS_BITON(m_shootFlag, SHOOT_FLAG_QUICK))
            {
                // ���ł��t���O���������A�d�����Ԃ��܂�������
                BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);
                m_nCntStopTime /= 2;

                // �ő�d�����Ԃ𒲐�
                if (m_nCntStopTime > BALL_QUICK_SHOOT_MAX_STOP_TIME)
                {
                    m_nCntStopTime = BALL_QUICK_SHOOT_MAX_STOP_TIME;
                }
            }

            // �d�����Ԃ̍ő�
            if (m_nCntStopTime > BALL_SHOOT_MAX_STOP_TIME)
            {
                m_nCntStopTime = BALL_SHOOT_MAX_STOP_TIME;
            }
        }
        else
        {
            // �ړ��p�x�Ɨ͂����т���
            m_moveAngle = moveAngle;
            m_fSpeed += fPower;
        }
    }

    // ��葬�x�����Ȃ�A�d�����Ԃ�1
    if (m_fSpeed < BALL_SHOOT_STOP_START_SPEED)
    {
        m_nCntStopTime = 1;
    }

    return m_nCntStopTime;
}

//=============================================================================
// �V���[�g�ɂ���ă��Z�b�g����ϐ�
// Author : �㓡�T�V��
//=============================================================================
void CBall::ResetByShoot(void)
{
    m_bAfterThrow = false;
    m_nCntBuntTime = 0;
    m_bStartButtle = false;
    m_absorbFlag = ABSORB_FLAG_NONE;
    m_nWhoAbsorbing = BALL_NOT_ANYONE;
    m_nCntAbsorbTime = 0;
}

//=============================================================================
// ���b�L�[�K�[�h��z�����Ԑ؂�őł��グ��Ƃ��̏���
// Author : �㓡�T�V��
//=============================================================================
int CBall::Launch(D3DXVECTOR3 startPos, int nCntStopTime)
{
    // �������̃����o�ϐ��𒲐�
    ResetByShoot();

    // �ʒu���J�n�ʒu�ɍ��킹��
    SetPos(startPos);

    // �o���g�̃t���O���I���ɂ���
    BITON(m_shootFlag, SHOOT_FLAG_BUNT);

    // �ړ��p�x��^��ɂ���
    m_moveAngle.x = sinf(D3DXToRadian(0.0f));
    m_moveAngle.y = cosf(D3DXToRadian(0.0f));

    // �N�̃{�[���ł��Ȃ��Ȃ�
    m_nWhoShooting = BALL_NOT_ANYONE;

    // �d�����Ԃ����т���
    SetStopTime(nCntStopTime);

    return m_nCntStopTime;
}

//=============================================================================
// �d�����Ԃ����߂�
// Author : �㓡�T�V��
//=============================================================================
void CBall::SetStopTime(const int nCntStopTime)
{
    m_nCntStopTime = nCntStopTime;

    // �d�����Ă���Ȃ�
    if (m_nCntStopTime > 0)
    {
        // �����蔻���������
        m_bUseCollision = false;
    }
}

//=============================================================================
// �z����ݒ肷��
// Author : �㓡�T�V��
//=============================================================================
void CBall::SetAbsorb(CPlayer *pPlayer)
{
    BITON(m_absorbFlag, ABSORB_FLAG_MOVING);
    m_pPlayer = pPlayer;
    m_nWhoShooting = BALL_NOT_ANYONE;           // �v���C���[�ւ̓����蔻�����������
    m_nWhoAbsorbing = pPlayer->GetPlayable();   // �����̋z�������v���C���[�������{�[���ɐG���悤�ɂ��邽��
    m_nCntAbsorbTime = BALL_ABSORB_MAX_TIME;    // �z�����Ԃ̃J�E���g�_�E�����X�^�[�g
}
