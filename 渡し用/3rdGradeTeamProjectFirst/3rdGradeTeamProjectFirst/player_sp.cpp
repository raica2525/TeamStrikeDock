//======================================================================================
//
// �v���C���[�̕K�E�Z�̏��� (player_sp.cpp)
// Author : �㓡�T�V��
//
//======================================================================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "player.h"
#include "effect3d.h"
#include "ball.h"
#include "game.h"
#include "effect2d.h"
#include "manager.h"
#include "camera.h"
#include "text.h"

//=============================================================================
// �K�E�Z�̑ł���������
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::EmitSpShot(void)
{
    // �X�C���O�A�X�}�b�V���A�X�p�C�N�̂����ꂩ�Ȃ�
    if (m_attackState == ATTACK_STATE_SWING || m_attackState == ATTACK_STATE_SMASH || m_attackState == ATTACK_STATE_SPIKE)
    {
        // �K�E�Z�g�p�҂�o�^
        CGame::SetSpPlayer(this);
        CManager::GetCamera()->SetState(CCamera::STATE_SP);

        // �G�t�F�N�g�ƕ������o��
        CEffect2D *pBg = CEffect2D::Create(CEffectData::TYPE_SS_BG, D3DXVECTOR3(640.0f, 210.0f, 0.0f));
        pBg->SetSize(D3DXVECTOR3(1280.0f, 300.0f, 0.0f));
        CEffect2D *pText = CEffect2D::Create(CEffectData::TYPE_SS_TEXT, D3DXVECTOR3(640.0f, 210.0f, 0.0f));
        pText->SetSize(D3DXVECTOR3(550.0f, 550.0f, 0.0f));
        CText *pSpName = CGame::GetSpText();
        if (pSpName)
        {
            char cSpName[256];
            CustomSpName(cSpName);
            pSpName->SetText(cSpName);
            pSpName->SetColor(TEXT_EXIST_COLOR);
        }

        D3DXVECTOR3 hipPos = GetPartsPos(PARTS_HIP);
        CEffect3D::Create(CEffectData::TYPE_SS_1, hipPos);
        CEffect3D::Create(CEffectData::TYPE_SS_2, hipPos);
        CEffect3D::Create(CEffectData::TYPE_SS_3, hipPos);

        // �K�E�{�C�X
        switch (m_voiceSet)
        {
        case VOICE_SET_ROBO:
            CManager::SoundPlay(CSound::LABEL_VOICE_SP_ICARUS);
            break;
        case VOICE_SET_WOMAN:
            CManager::SoundPlay(CSound::LABEL_VOICE_SP_KLEINOD);
            break;
        case VOICE_SET_GHOST:
            CManager::SoundPlay(CSound::LABEL_VOICE_SP_KNIGHT);
            break;
        case VOICE_SET_OOO:
            CManager::SoundPlay(CSound::LABEL_VOICE_SP_OOO);
            break;
        case VOICE_SET_MAN:
            CManager::SoundPlay(CSound::LABEL_VOICE_SP_RANGER);
            break;
        case VOICE_SET_BOY:
            CManager::SoundPlay(CSound::LABEL_VOICE_SP_X);
            break;
        }

        // �K�E�Z���Ƃ̑ł���������
        switch (m_spShot)
        {
        case SP_SHOT_HEAL:
            SpHeal();
            break;
        case SP_SHOT_DECOY:
            SpDecoy();
            break;
        case SP_SHOT_WALL_THROUGH:
            SpWallThrough();
            break;
		case SP_SHOT_BARRIER:
			SpBarrier();
			break;
        default:

            break;
        }

        // �Q�[�W�S����
        m_fSpGaugeCurrent = 0.0f;
    }
}

//=============================================================================
// �K�E�Z:�񕜂̏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SpHeal(void)
{
    // ��`
    const float HEAL_VALUE = m_afParam[PARAM_7_WEAPON_SP]; // �񕜊�����⏕�l����󂯎��

    // ��
    m_fLife += m_fDef * HEAL_VALUE;

    // �񕜂̏��
    if (m_fLife > m_fDef)
    {
        m_fLife = m_fDef;
    }

    // �G�t�F�N�g����
    CEffect3D::Emit(CEffectData::TYPE_HEAL, GetPos(), GetPos(), m_collisionSizeDeffence.x);
}

//=============================================================================
// �K�E�Z:�Ԃ񂵂�{�[���̏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SpDecoy(void)
{
    // �U���p�x
    float fAttackAngle = D3DXToRadian(180.0f);    // �j���[�g�����͐^��

    // �X�e�B�b�N���|��Ă���Ȃ炻�̕����ɔ�΂�
    if (m_controlInput.bTiltedLeftStick)
    {
        fAttackAngle = m_controlInput.fLeftStickAngle;
    }

    // �ړ��p�x����
    D3DXVECTOR3 moveAngle = DEFAULT_VECTOR;
    moveAngle.x = sinf(fAttackAngle);
    moveAngle.y = cosf(fAttackAngle);

    // �{�[���𐶐��i���݃{�[���ʒu����A�����蔻��̂Ȃ����̂𐶐��j
    CBall*pBall = CBall::Create(CGame::GetBall()->GetPos(), false);

    // ���������{�[���ɕK�v�ȏ������т���
    pBall->SetMoveAngle(moveAngle);
    pBall->SetSpeed(CGame::GetBall()->GetSpeed());
    pBall->SetStartButtle(false);
    pBall->SetWhoShooting(m_nIdxCreate);
    pBall->SetPlayer(this);
    pBall->SetHitWallMax((int)m_afParam[PARAM_7_WEAPON_SP]);    // �⏕�l����ǂɓ��������������ő吔��ݒ�

    // �ǉ��Ő�������{�[��
    if ((int)m_afParam[PARAM_6_WEAPON_SP] > 0)
    {
        for (int nCnt = 0; nCnt < (int)m_afParam[PARAM_6_WEAPON_SP]; nCnt++)
        {
            // �U���p�x�������_���Ɍ��߂�
            fAttackAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;
            moveAngle.x = sinf(fAttackAngle);
            moveAngle.y = cosf(fAttackAngle);

            // �{�[���𐶐�
            pBall = CBall::Create(CGame::GetBall()->GetPos(), false);
            pBall->SetMoveAngle(moveAngle);
            pBall->SetSpeed(CGame::GetBall()->GetSpeed());
            pBall->SetStartButtle(false);
            pBall->SetWhoShooting(m_nIdxCreate);
            pBall->SetPlayer(this);
            pBall->SetHitWallMax((int)m_afParam[PARAM_7_WEAPON_SP]);
        }
    }
}

//=============================================================================
// �K�E�Z:�E�H�[���X���[�̏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SpWallThrough(void)
{
    CGame::GetBall()->SetWallThrough((int)m_afParam[PARAM_7_WEAPON_SP]);
}

//=============================================================================
// �K�E�Z:�o���A�̏���
// Author : �r�c�I��
//=============================================================================
void CPlayer::SpBarrier(void)
{
	m_bSpBarrier = true;
	
    CEffect2D *pBarrier = CEffect2D::Create(CEffectData::TYPE_BARRIER, ConvertScreenPos(GetPos() + D3DXVECTOR3(0.0f, m_collisionSizeDeffence.y / 2, 0.0f)));
    pBarrier->SetPlayer(this);
}