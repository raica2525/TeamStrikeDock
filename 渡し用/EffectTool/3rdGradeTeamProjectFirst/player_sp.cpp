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

//=============================================================================
// �K�E�Z�̑ł���������
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::EmitSpShot(void)
{
    // �K�E�Z���Ƃ̑ł���������
    switch (m_spShot)
    {
    case SP_SHOT_HEAL:
        SpHeal();
        break;
    default:

        break;
    }

    // �Q�[�W�S����
    m_fSpGaugeCurrent = 0.0f;
}

//=============================================================================
// �K�E�Z:�񕜂̏���
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::SpHeal(void)
{
    // ��`
    const float HEAL_VALUE = 0.34f; // �񕜊���

    // ��
    m_fLife += m_fDef * HEAL_VALUE;

    // �񕜂̏��
    if (m_fLife > m_fDef)
    {
        m_fLife = m_fDef;
    }

    // �G�t�F�N�g����
    CEffect3D::Emit(CEffectData::TYPE_CIRCLEABSORPTION, GetPos(), GetPos(), GetActiveCollisionSize().x);
}