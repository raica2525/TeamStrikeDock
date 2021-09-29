//===============================================
//
// �{�[���̏��� (ball.cpp)
// Author : �㓡�T�V���A�ɓ��z��i�O�Ձj
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
#include "effect3d.h"
#include "library.h"
#include "player.h"
#include "wave.h"
#include "effect2d.h"
#include "number_array.h"
#include "outline.h"
#include "input.h"
#include "locus.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBall::CBall() :CScene3D(CScene::OBJTYPE_BALL)
{
    // �A�E�g���C���̃|�C���^�̓R���X�g���N�^�ł̂݃��Z�b�g
    m_pOutline = NULL;

    // �����o�ϐ��̃��Z�b�g
    ResetMenberVariables();
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CBall::~CBall()
{

}

//=============================================================================
// �����o�ϐ��̃��Z�b�g
// Author : �㓡�T�V��
//=============================================================================
void CBall::ResetMenberVariables(void)
{
    m_shootFlag = SHOOT_FLAG_NONE;
    m_moveAngle = BALL_FIRST_MOVE_ANGLE;
    m_fSpeed = BALL_FIRST_SPEED;

    m_nCntCurrentStopTime = 0;
    m_nStopTimeAtShoot = 0;
    m_bUseCollision = false;
    m_bNeverUseCollision = false;
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

    m_bDisp = false;
    m_nCntDispingTime = 0;
    m_nCntNoLineStopTime = 0;

    //===================================
    // �K�E�Z�Ή�����
    //===================================
    m_nCntHitWall = 0;
    m_nCntHitWallMax = 0;

    m_bWallThrough = false;
    m_nCntWallThrough = 0;
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

    // �A�E�g���C����ݒ�
    if (!m_pOutline)
    {
        m_pOutline = COutline::Create(pos, D3DXVECTOR3(500.0f, 500.0f, 0.0f), DEFAULT_VECTOR, PLAYER_COLOR_NONE);
    }

    return S_OK;
}

//=============================================================================
// �Đݒ菈��
// Author : �㓡�T�V��
//=============================================================================
void CBall::Reset(D3DXVECTOR3 pos)
{
    // �e�����o�ϐ��̃��Z�b�g
    ResetMenberVariables();

    // �ʒu�����߁A��]�ƃX�P�[�������Z�b�g
    SetPos(pos);
    m_posOld = pos;
    SetRot(DEFAULT_VECTOR);
    SetScale(DEFAULT_VECTOR);
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CBall::Uninit(void)
{
    // �A�E�g���C�����g�p���Ȃ�
    if (m_pOutline != NULL)
    {
        m_pOutline->Unable();
    }

    CScene3D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CBall::Update(void)
{
    // ���S�̍d�����Ԃ��J�E���g
    if (m_nCntNoLineStopTime > 0)
    {
        m_nCntNoLineStopTime--;
    }

    // �\�����Ă���Ȃ�
    if (m_bDisp)
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
            // �ʒu���擾
            D3DXVECTOR3 pos = GetPos();

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
                pos = HitWall(pos, HIT_WALL_RIGHT);
            }
            else if (pos.x - (BALL_COLLISION_SIZE_SIDE / 2) < -mapLimit.fWidth)
            {
                pos = HitWall(pos, HIT_WALL_LEFT);
            }
            if (pos.y + (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y  > mapLimit.fHeight)
            {
                pos = HitWall(pos, HIT_WALL_UP);
            }
            else if (pos.y - (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y < 0)
            {
                pos = HitWall(pos, HIT_WALL_DOWN);
            }

            // �l�p�̈ʒu���X�V
            UpdateCornerPos(pos);

            // �ʒu�𔽉f(Z���͌Œ�)
            SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));

#ifdef COLLISION_TEST
            if (m_bUseCollision)
            {
                CDebug::Create(pos + D3DXVECTOR3(0.0f, 0.0f, -50.0f) + BALL_COLLISION_POS_SLIDE, BALL_COLLISION_SIZE, CDebug::TYPE_MOMENT);
            }
#endif // COLLISION_TEST
        }

        // �G�t�F�N�g����
        LocusEffect();
        AuraEffect();

        // �X�P�[���ƌ�����ς���
        ChangeScaleAndRot();

        // ��΂ɓ����蔻����g��Ȃ��U���̃{�[���Ȃ�A�I�����������邩�ǂ�������
        if (m_bNeverUseCollision)
        {
            if (m_nCntHitWall >= m_nCntHitWallMax)
            {
                CBall::Uninit();
            }
        }
    }
    else
    {
        // �����蔻����g�p���Ȃ�
        m_bUseCollision = false;

        // �Q�[�����Ȃ�
        if (CGame::GetState() == CGame::STATE_BUTTLE)
        {
            // �{�[���z�u��
            if (m_nCntDispingTime <= 0)
            {
                CManager::SoundPlay(CSound::LABEL_SE_BALL_DISPING);
            }

            // �o�����Ԃ��J�E���g�A�b�v
            m_nCntDispingTime++;

            // �o���G�t�F�N�g
            AppearEffect();

            // �z�u���ԂɒB������A�g��J�n
            if (m_nCntDispingTime >= BALL_DISPING_TIME)
            {
                D3DXVECTOR3 scale = GetScale();
                scale.x += BALL_DISPING_SCALE_UP_RATE;
                scale.y += BALL_DISPING_SCALE_UP_RATE;
                scale.z += BALL_DISPING_SCALE_UP_RATE;

                // �傫�������{�ɂȂ�����A�Œ肵�z�u����
                if (scale.x > 1.0f)
                {
                    m_nCntDispingTime = 0;
                    scale.x = 1.0f;
                    scale.y = 1.0f;
                    scale.z = 1.0f;
                    m_bDisp = true;
                    m_bUseCollision = true;
                    CWave::Create(GetPos(), BALL_WAVE_FIRST_SIZE);
                }

                // �X�P�[����ݒ�
                SetScale(scale);
            }
        }
    }

    // �A�E�g���C�����X�V
    if (m_pOutline != NULL)
    {
        // �ʒu���X�V
        D3DXVECTOR3 outlinePos = GetPos();
        m_pOutline->SetPos(outlinePos);

        // �F���v���C���[�ɂ���ĕς���
        D3DXCOLOR outlineColor = PLAYER_COLOR_NONE;
        if (m_pPlayer)
        {
            if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
            {
                const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
                if (nWhoColor == PLAYER_1)
                {
                    outlineColor = PLAYER_COLOR_1;
                }
                else if (nWhoColor == PLAYER_2)
                {
                    outlineColor = PLAYER_COLOR_2;
                }
                else if (nWhoColor == PLAYER_3)
                {
                    outlineColor = PLAYER_COLOR_3;
                }
                else if (nWhoColor == PLAYER_4)
                {
                    outlineColor = PLAYER_COLOR_4;
                }
            }
        }

        m_pOutline->SetCol(outlineColor);
    }
}

//=============================================================================
// �l�p�̈ʒu���X�V
// Author : �㓡�T�V��
//=============================================================================
void CBall::UpdateCornerPos(D3DXVECTOR3 pos)
{
    // �^�񒆂̍��W���o��
    D3DXVECTOR3 centerPos = (pos + m_posOld) / 2 + BALL_COLLISION_POS_SLIDE;

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
// �G�t�F�N�g���o����
// Author : �㓡�T�V��
//=============================================================================
void CBall::AuraEffect(void)
{
    // ���݂̈ʒu���擾
    D3DXVECTOR3 posCurrent = GetPos();

    // ���݂̈ʒu����1F�O�̌������擾
    float fScatterAngle = atan2((posCurrent.x - m_posOld.x), (posCurrent.y - m_posOld.y));

    // �G�t�F�N�g����
    CEffect3D* pEffect = CEffect3D::Create(CEffectData::TYPE_BALL_AURA, posCurrent, fScatterAngle);

    // �G�t�F�N�g�������ł��Ă���Ȃ�
    if (pEffect)
    {
        // �F�����݃{�[���������Ă���l�ɍ��킹�ĕς���
        D3DXCOLOR col = DEFAULT_COLOR;
        D3DXCOLOR colChangeRate = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        if (m_pPlayer)
        {
            if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
            {
                const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
                if (nWhoColor == PLAYER_1)
                {
                    col = D3DXCOLOR(1.0f, 0.99f, 0.0f, 1.0f);
                    colChangeRate.g = -0.05f;
                }
                else if (nWhoColor == PLAYER_2)
                {
                    col = D3DXCOLOR(0.588f, 0.937f, 1.0f, 1.0f);
                    colChangeRate.g = -0.03f;
                    colChangeRate.r = -0.03f;
                }
                else if (nWhoColor == PLAYER_3)
                {
                    col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
                    colChangeRate.r = -0.05f;
                }
                else if (nWhoColor == PLAYER_4)
                {
                    col = D3DXCOLOR(0.933f, 0.478f, 0.02f, 1.0f);
                    colChangeRate.g = 0.01f;
                }
            }
            else
            {
                col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
                colChangeRate.g = -0.05f;
                colChangeRate.r = -0.025f;
            }
        }
        else
        {
            col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
            colChangeRate.g = -0.05f;
            colChangeRate.r = -0.025f;
        }

        // �F�ƐF�ω�������ݒ�
        pEffect->SetCol(col);
        pEffect->SetColChangeRate(colChangeRate);
    }
}

//=============================================================================
// �{�[���o���̃G�t�F�N�g����
// Author : �㓡�T�V��
//=============================================================================
void CBall::AppearEffect(void)
{
    // ���݂̈ʒu���擾
    D3DXVECTOR3 posCurrent = GetPos();

    // �G�t�F�N�g����
    CEffect3D* pEffect = CEffect3D::Emit(CEffectData::TYPE_BALL_APPEAR, posCurrent, posCurrent);

    // �G�t�F�N�g�������ł��Ă���Ȃ�
    if (pEffect)
    {
        // �F�����݃{�[���������Ă���l�ɍ��킹�ĕς���
        D3DXCOLOR col = DEFAULT_COLOR;
        D3DXCOLOR colChangeRate = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        if (m_pPlayer)
        {
            if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
            {
                const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
                if (nWhoColor == PLAYER_1)
                {
                    col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
                    colChangeRate.g = -0.05f;
                }
                else if (nWhoColor == PLAYER_2)
                {
                    col = D3DXCOLOR(0.588f, 0.937f, 1.0f, 1.0f);
                    colChangeRate.g = -0.03f;
                    colChangeRate.r = -0.03f;
                }
                else if (nWhoColor == PLAYER_3)
                {
                    col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
                    colChangeRate.r = -0.05f;
                }
                else if (nWhoColor == PLAYER_4)
                {
                    col = D3DXCOLOR(0.933f, 0.478f, 0.02f, 1.0f);
                    colChangeRate.g = 0.01f;
                }
            }
            else
            {
                col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
                colChangeRate.g = -0.05f;
                colChangeRate.r = -0.025f;
            }
        }
        else
        {
            col = D3DXCOLOR(0.835f, 0.901f, 1.0f, 1.0f);
            colChangeRate.g = -0.05f;
            colChangeRate.r = -0.025f;
        }

        // �F��ݒ�
        pEffect->SetCol(col);
        pEffect->SetColChangeRate(colChangeRate);
    }
}

//=============================================================================
// �{�[���̋O�Ղ̏���
// Author : �ɓ��z��
//=============================================================================
void CBall::LocusEffect(void)
{
    const float LOCUS_SCALE = 50.0f;    // �O�Ղ̑傫��
    const float SHRINK_SPEED = 1.0f;    // �O�Ղ̏k�ޑ���
    const float LOCUS_POS_Z = -1.0f;    // �O�Ղ̈ʒuZ�i���̃G�t�F�N�g����O�j
    D3DXVECTOR3 pos = GetPos();

    // �{�[���̐i�ފp�x�ɍ��킹�ďo��悤�ɂ���
    D3DXVECTOR3 posOld1 = D3DXVECTOR3(m_posOld.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(90.0f))),
        m_posOld.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(90.0f))), LOCUS_POS_Z);
    D3DXVECTOR3 posOld2 = D3DXVECTOR3(m_posOld.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(-90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(-90.0f))),
        m_posOld.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(-90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(-90.0f))), LOCUS_POS_Z);
    D3DXVECTOR3 pos1 = D3DXVECTOR3(pos.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(90.0f))),
        pos.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(90.0f))), LOCUS_POS_Z);
    D3DXVECTOR3 pos2 = D3DXVECTOR3(pos.x + (LOCUS_SCALE * m_moveAngle.x * cosf(D3DXToRadian(-90.0f))) - (LOCUS_SCALE * m_moveAngle.y * sinf(D3DXToRadian(-90.0f))),
        pos.y + (LOCUS_SCALE * m_moveAngle.x * sinf(D3DXToRadian(-90.0f))) + (LOCUS_SCALE * m_moveAngle.y * cosf(D3DXToRadian(-90.0f))), LOCUS_POS_Z);

    // �F���v���C���[�ɂ���ĕς���
    D3DXCOLOR locusColor = PLAYER_COLOR_NONE;
    if (m_pPlayer)
    {
        if (IS_BITOFF(m_shootFlag, SHOOT_FLAG_BUNT))
        {
            const int nWhoColor = m_pPlayer->GetIdxControlAndColor();
            if (nWhoColor == PLAYER_1)
            {
                locusColor = PLAYER_COLOR_1;
            }
            else if (nWhoColor == PLAYER_2)
            {
                locusColor = PLAYER_COLOR_2;
            }
            else if (nWhoColor == PLAYER_3)
            {
                locusColor = PLAYER_COLOR_3;
            }
            else if (nWhoColor == PLAYER_4)
            {
                locusColor = PLAYER_COLOR_4;
            }
        }
    }

    // �O�Ղ̐���
    CLocus::Create(pos1, pos2, posOld1, posOld2, locusColor, CLocus::TYPE_SHRINK, SHRINK_SPEED);
}

//=============================================================================
// �X�P�[���Ɗp�x�̕ύX����
// Author : �㓡�T�V��
//=============================================================================
void CBall::ChangeScaleAndRot(void)
{
    // �X�P�[���ƌ������擾
    D3DXVECTOR3 scale = GetScale();
    D3DXVECTOR3 rot = GetRot();

    if (m_nWhoShooting == BALL_NOT_ANYONE || IS_BITON(m_shootFlag, SHOOT_FLAG_THROW) || m_nCntCurrentStopTime > 0)
    {
        // �������N�̃{�[���ł��Ȃ������~�܂��Ă���Ƃ��͒ʏ�X�P�[��
        scale.x = 1.0f;
        scale.y = 1.0f;

        // ��]
        rot.x += BALL_ROT_SPEED;
    }
    else
    {
        // �q�b�g�X�g�b�v����������X�s�[�h�ȏ�Ȃ�
        if (m_fSpeed > BALL_SHOOT_BIG_HIT_SPEED)
        {
            // �X�P�[�����c���ɂ���
            scale.x = 0.5f;
            scale.y = 1.5f;

            // �ʒu����ړ��p�x���擾
            D3DXVECTOR3 pos = GetPos();
            float fMoveX = m_posOld.x - pos.x;
            float fMoveY = m_posOld.y - pos.y;
            float fAngle = atan2((fMoveX), (fMoveY));

            // �ړ��p�x�̌����ɉ�]�i�^���͗�O�j
            rot.x = sinf(fAngle);
            rot.y = cosf(fAngle);
            if (fabsf(fMoveY) <= BALL_MOVE_Y_DONT_MOVE_BORDER)
            {
                scale.x = 1.5f;
                scale.y = 0.5f;
            }
        }
        else
        {
            // ��]
            rot.x += BALL_ROT_SPEED;
        }
    }

    // ��]����
    if (rot.x > D3DXToRadian(180.0f))
    {
        rot.x = D3DXToRadian(-180.0f);
    }

    // �X�P�[���ƌ����𔽉f
    SetScale(scale);
    SetRot(rot);
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CBall::Draw(void)
{
    // �\�����Ă��邩�A�z�u���Ȃ�`��
    if (m_bDisp || m_nCntDispingTime >= BALL_DISPING_TIME)
    {
        // �X�e���V���o�b�t�@�ŃA�E�g���C����`��
        if (m_pOutline != NULL)
        {
            // �X�e���V���o�b�t�@���g��
            LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

            // Z�e�X�g�L��
            pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
            // Z�e�X�g��K�����s��
            pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

            // �X�e���V���e�X�g�L��
            pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
            // �X�e���V���o�b�t�@�֔��f����Q�ƒl
            pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
            // �Q�ƒl�}�X�N
            pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
            // �X�e���V���e�X�g��K��������
            pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
            // �X�e���V���e�X�g�̃e�X�g�ݒ�
            pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
            pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
            pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

            // �ʏ�̕`��̑O��ŁA�X�P�[����������
            D3DXVECTOR3 memoryScale = GetScale();
            SetScale(GetScale()*BALL_OUTLINE_RATE);
            CScene3D::Draw(false, false);
            SetScale(memoryScale);

            // �X�e���V���e�X�g����
            pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

            // ZBUFFER��r�ݒ�ύX => (�Q�ƒl <= �o�b�t�@�l)(�߂�)
            pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

            // �A�E�g���C����`��
            m_pOutline->Draw();
        }

        // �ʏ�̕`��
        CScene3D::Draw(false, false);
    }
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
    pBall->Init(pos, DEFAULT_VECTOR);

    // �����o�ϐ������т���
    pBall->m_bUseCollision = bUseCollision;

    // �����蔻����g��Ȃ��Ȃ�A��΂ɂ���ȍ~���g��Ȃ��A���z�u���̊T�O�͂Ȃ�
    if (!pBall->m_bUseCollision)
    {
        pBall->m_bNeverUseCollision = true;
        pBall->m_bDisp = true;
        pBall->SetScale(DEFAULT_SCALE);
    }

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
        if (m_nCntCurrentStopTime <= 0)
        {
            // �d�����́A�J�E���^�����Z���Ȃ�
            m_nCntBuntTime++;
        }
        fSpeed = BALL_BUNT_SPEED;
        fGravity = m_nCntBuntTime * BALL_BUNT_GRAVITY;
    }

    // �d�����Ԃ̃J�E���^
    if (m_nCntCurrentStopTime <= 0)
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
    else
    {
        // �W�������o���d�����Ԃ��ォ�A���S���̃J�E���g�_�E�������Ă��Ȃ��Ȃ�
        if (m_nCntCurrentStopTime > BALL_SHOOT_STOP_REFRECT_FRAME && m_nCntNoLineStopTime <= 0)
        {
            // �W����
            CEffect3D::Emit(CEffectData::TYPE_SHOOT_ABSORB, pos, pos);
        }

        // ��~���Ԃ��J�E���g�_�E��
        m_nCntCurrentStopTime--;

        // �����蔻����g���{�[���Ȃ�
        if (!m_bNeverUseCollision)
        {
            // �{�[�����˃Q�[�W���X�V����V���[�g���̍d�����Ԃ��A���S���̃J�E���g�_�E�������Ă��Ȃ��Ȃ�
            if (m_nStopTimeAtShoot > BALL_UPDATE_METER_MIN_STOP_FRAME && m_nCntNoLineStopTime <= 0)
            {
                // �{�[�����˃Q�[�W���X�V
                CGame::SetBallGauge(m_nStopTimeAtShoot, m_nStopTimeAtShoot - m_nCntCurrentStopTime);
            }
            else
            {
                // �����Ă���Œ��́A�{�[�����˃Q�[�W����ɖ��^����
                CGame::SetBallGauge(BALL_UPDATE_METER_MIN_STOP_FRAME, BALL_UPDATE_METER_MIN_STOP_FRAME);
            }

            // �Ō�̃t���[���ŁA�����蔻��𕜊������A�V���[�g���̍d�����Ԃ̋L�������Z�b�g
            if (m_nCntCurrentStopTime == 0)
            {
                m_bUseCollision = true;
                m_nStopTimeAtShoot = 0;
            }
        }
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
            D3DXVECTOR3 targetPos = m_pPlayer->GetPos() + D3DXVECTOR3(0.0f, m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);
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
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(m_pPlayer->GetCollisionSizeDeffence().x * 0.5f + BALL_ABSORB_POS_X,
                    m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);
            }
            else if (m_pPlayer->GetRot().y == PLAYER_ROT_LEFT)
            {
                // �������Ȃ�
                returnPos = m_pPlayer->GetPos() + D3DXVECTOR3(-(m_pPlayer->GetCollisionSizeDeffence().x * 0.5f + BALL_ABSORB_POS_X),
                    m_pPlayer->GetCollisionSizeDeffence().y * 0.5f, 0.0f);
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
D3DXVECTOR3 CBall::HitWall(D3DXVECTOR3 pos, HIT_WALL hitWall)
{
    // �ړ�����
    D3DXVECTOR3 returnPos = pos;
    CGame::MAP_LIMIT mapLimit = CGame::GetMapLimit();
    switch (hitWall)
    {
    case HIT_WALL_RIGHT:
        // �E�H�[���X���[���邩�ǂ���
        returnPos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.x *= -1;
        }
        else
        {
            // ���[�v��
            CManager::SoundPlay(CSound::LABEL_SE_WARP);

            // ���[�v�G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_RIGHT, returnPos);

            m_nCntWallThrough--;
            returnPos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);

            // ���[�v��G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_LEFT, returnPos);

            // �E�H�[���X���[�����Ȃ�A�O�̈ʒu�����킹��
            m_posOld.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
        }
        break;
    case HIT_WALL_LEFT:
        // �E�H�[���X���[���邩�ǂ���
        returnPos.x = -mapLimit.fWidth + (BALL_COLLISION_SIZE_SIDE / 2);
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.x *= -1;
        }
        else
        {
            // ���[�v��
            CManager::SoundPlay(CSound::LABEL_SE_WARP);

            // ���[�v�G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_LEFT, returnPos);

            m_nCntWallThrough--;
            returnPos.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);

            // ���[�v��G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_RIGHT, returnPos);

            // �E�H�[���X���[�����Ȃ�A�O�̈ʒu�����킹��
            m_posOld.x = mapLimit.fWidth - (BALL_COLLISION_SIZE_SIDE / 2);
        }
        break;
    case HIT_WALL_UP:
        // �E�H�[���X���[���邩�ǂ���
        returnPos.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.y *= -1;
        }
        else
        {
            // ���[�v��
            CManager::SoundPlay(CSound::LABEL_SE_WARP);

            // ���[�v�G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_UP, returnPos);

            m_nCntWallThrough--;
            returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;

            // ���[�v��G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_DOWN, returnPos);

            // �E�H�[���X���[�����Ȃ�A�O�̈ʒu�����킹��
            m_posOld.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        }
        break;
    case HIT_WALL_DOWN:
        // �E�H�[���X���[���邩�ǂ���
        returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        if (m_nCntWallThrough <= 0)
        {
            m_moveAngle.y *= -1;
        }
        else
        {
            // ���[�v��
            CManager::SoundPlay(CSound::LABEL_SE_WARP);

            // ���[�v�G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_DOWN, returnPos);

            m_nCntWallThrough--;
            returnPos.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;

            // ���[�v��G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WARP_SPLASH2, returnPos, m_posOld);
            CEffect3D::Create(CEffectData::TYPE_WARP_UP, returnPos);

            // �E�H�[���X���[�����Ȃ�A�O�̈ʒu�����킹��
            m_posOld.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
        }
        break;
    }

    // �����o�ϐ��̒���
    m_bAfterThrow = false;

    // �ǂɓ��������񐔂��֌W�̂����ԂȂ�A�J�E���g�A�b�v
    if (m_bNeverUseCollision)
    {
        m_nCntHitWall++;
    }

    // �o���g�{�[����ԂȂ�
    if (IS_BITON(m_shootFlag, SHOOT_FLAG_BUNT))
    {
        // ��ǈȊO�Ȃ�
        if (hitWall != HIT_WALL_UP)
        {
            // �o���g�{�[����Ԃ��������A�Ō�ɑł����l�̃{�[���ɂ��āA�c�ړ���
            BITOFF(m_shootFlag, SHOOT_FLAG_BUNT);
            BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);              // ���ł��t���O�������i�ł������ƂɂȂ邽�߁j
            m_nCntBuntTime = 0;
            m_moveAngle = BALL_FIRST_MOVE_ANGLE;
            if (m_pPlayer)
            {
                m_nWhoShooting = m_pPlayer->GetIdxCreate();
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
        // �z����ԂłȂ��Ȃ�
        if (m_nWhoAbsorbing == BALL_NOT_ANYONE)
        {
            // �o�E���h��
            CManager::SoundPlay(CSound::LABEL_SE_BOUND);

            // �o���g�{�[���ȊO�Ȃ�A�ǂɓ����������ɂ킸���ɍd������
            m_nCntCurrentStopTime = BALL_SHOOT_STOP_REFRECT_FRAME;

            // �ǃq�b�g�G�t�F�N�g
            CEffect3D::Emit(CEffectData::TYPE_WALL_HIT_SMOKE, returnPos, m_posOld);
            CEffect3D::Emit(CEffectData::TYPE_WALL_HIT_SHOCK, returnPos, m_posOld);
        }
    }

    return returnPos;
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
    if (returnPos.y + (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y > mapLimit.fHeight)
    {
        returnPos.y = mapLimit.fHeight - (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
    }
    else if (returnPos.y - (BALL_COLLISION_SIZE_SIDE / 2) + BALL_COLLISION_POS_SLIDE_Y < 0)
    {
        returnPos.y = (BALL_COLLISION_SIZE_SIDE / 2) - BALL_COLLISION_POS_SLIDE_Y;
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
            // ������
            CManager::SoundPlay(CSound::LABEL_SE_THROW);

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
    m_nWhoShooting = m_pPlayer->GetIdxCreate();

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
        // �o���g��
        CManager::SoundPlay(CSound::LABEL_SE_BUNT);

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

        // ���ł��t���O�������i�ł������ƂɂȂ邽�߁j
        BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);

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
            m_nCntCurrentStopTime = (int)(m_fSpeed * BALL_SHOOT_STOP_TIME_RATE);

            // ���������ł��Ȃ�
            if (IS_BITON(flag, SHOOT_FLAG_QUICK))
            {
                BITON(m_shootFlag, SHOOT_FLAG_QUICK);
            }

            // �����o�ϐ������ł��Ȃ�
            bool bQuickShoot = false;
            if (IS_BITON(m_shootFlag, SHOOT_FLAG_QUICK))
            {
                // ���ł��t���O���������A�d�����Ԃ��܂�������
                BITOFF(m_shootFlag, SHOOT_FLAG_QUICK);
                m_nCntCurrentStopTime /= 2;
                bQuickShoot = true;

                // �ő�d�����Ԃ𒲐�
                if (m_nCntCurrentStopTime > BALL_QUICK_SHOOT_MAX_STOP_TIME)
                {
                    m_nCntCurrentStopTime = BALL_QUICK_SHOOT_MAX_STOP_TIME;
                }
            }

            // �d�����Ԃ̍ő�
            if (m_nCntCurrentStopTime > BALL_SHOOT_MAX_STOP_TIME)
            {
                m_nCntCurrentStopTime = BALL_SHOOT_MAX_STOP_TIME;
            }

            // �d�����������鑬�x�ŁA�g������
            if (m_fSpeed >= BALL_SHOOT_BIG_HIT_SPEED)
            {
                // �E���g���q�b�g���ǂ���
                if (m_fSpeed >= BALL_SHOOT_ULTRA_HIT_SPEED)
                {
                    // �E���g���q�b�g�͔��]�����i�ݒ�ŕς����悤�Ɂj
                    CEffect2D* pNega = CGame::GetNega();
                    if (pNega)
                    {
                        pNega->SetPosTo2D(GetPos());
                        pNega->SetSize(DEFAULT_VECTOR);
                        pNega->SetUseUpdate(true);
                    }
                    CEffect2D* pPosi = CGame::GetPosi();
                    if (pPosi)
                    {
                        pPosi->SetPosTo2D(GetPos());
                        pPosi->SetSize(DEFAULT_VECTOR);
                        pPosi->SetUseUpdate(false);
                        pPosi->SetRotVertex(0.0f);  // �X�V���~�߂����ɁA���_�͂����Œ���
                    }
                    CManager::SoundPlay(CSound::LABEL_SE_HIT_ULTRA);
                }
                else
                {
                    // ��q�b�g�͔g������
                    CWave::Create(GetPos(), BALL_WAVE_FIRST_SIZE);
                    CManager::SoundPlay(CSound::LABEL_SE_HIT_BIG);
                }
            }
            else
            {
                // �q�b�g��
                CManager::SoundPlay(CSound::LABEL_SE_HIT);
            }

            // ��葬�x�����Ȃ�A�ŏ��d������
            if (m_fSpeed < BALL_SHOOT_BIG_HIT_SPEED)
            {
                m_nCntCurrentStopTime = BALL_SHOOT_STOP_LEAST_FRAME;

                // ���������l��
                if (bQuickShoot)
                {
                    m_nCntCurrentStopTime = BALL_SHOOT_STOP_LEAST_FRAME / 2;
                }
            }

            // �R���g���[���̐U��
            if (m_pPlayer)
            {
                if (m_pPlayer->GetUseControllerEffect())
                {
                    // �d�����Ԃ̔����h�炷�i�Œ�ۏ؂���j
                    int nEffectFrame = m_nCntCurrentStopTime / 2;
                    if (nEffectFrame < BALL_SHOOT_EFFECT_LEAST_FRAME)
                    {
                        nEffectFrame = BALL_SHOOT_EFFECT_LEAST_FRAME;
                    }
                    CManager::GetInputJoypad()->StartEffect(m_pPlayer->GetIdxControlAndColor(), nEffectFrame);
                }
            }

            // �{�[�����ˎ��̍d�����Ԃ��L��
            m_nStopTimeAtShoot = m_nCntCurrentStopTime;
            if (m_nStopTimeAtShoot > BALL_UPDATE_METER_MIN_STOP_FRAME)
            {
                // �{�[�����˃Q�[�W���X�V
                CGame::SetBallGauge(m_nStopTimeAtShoot, 0);
            }
        }
        else
        {
            // �E���g���q�b�g�Ȃ甽�]�����𔽓]���Ă����i���̐F�ɖ߂��Ă����j�i�ݒ�ŕς����悤�Ɂj
            if (m_fSpeed >= BALL_SHOOT_ULTRA_HIT_SPEED)
            {
                CEffect2D* pPosi = CGame::GetPosi();
                if (pPosi)
                {
                    pPosi->SetUseUpdate(true);
                }
            }

            // �ړ��p�x�Ɨ͂����т���
            m_moveAngle = moveAngle;
            m_fSpeed += fPower;

            // ���x�̏��
            if (m_fSpeed > BALL_MAX_SPEED)
            {
                m_fSpeed = BALL_MAX_SPEED;
            }

            // �{�[���X�s�[�h�\�����X�V
            CGame::GetNumArrayBallSpd()->SetDispNumber((int)m_fSpeed);
        }
    }

    return m_nCntCurrentStopTime;
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
    
    // �K�E�Z����
    if (!m_bWallThrough)
    {
        m_nCntWallThrough = 0;
    }
    else
    {
        // �ł��o���Ƃ��Ɉ�x������銴��
        m_bWallThrough = false;
    }
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

    return m_nCntCurrentStopTime;
}

//=============================================================================
// �d�����Ԃ����߂�
// Author : �㓡�T�V��
//=============================================================================
void CBall::SetStopTime(const int nCntStopTime)
{
    m_nCntCurrentStopTime = nCntStopTime;

    // �d�����Ă���Ȃ�
    if (m_nCntCurrentStopTime > 0)
    {
        // �����蔻���������
        m_bUseCollision = false;

        // �W����������
        m_nCntNoLineStopTime = nCntStopTime;
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
    m_nWhoAbsorbing = m_pPlayer->GetIdxCreate();   // �����̋z�������v���C���[�������{�[���ɐG���悤�ɂ��邽��
    m_nCntAbsorbTime = BALL_ABSORB_MAX_TIME;    // �z�����Ԃ̃J�E���g�_�E�����X�^�[�g
}
