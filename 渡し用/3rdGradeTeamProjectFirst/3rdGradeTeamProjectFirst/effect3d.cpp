//===============================================
//
// �G�t�F�N�g3D���� (effect3d.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "effect3d.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "game.h"
#include "library.h"
#include "sound.h"
#include "texture.h"

//=============================================================================
// �R���X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CEffect3D::CEffect3D(CScene::OBJTYPE objtype) :CBillboard(objtype)
{
    m_nTexType = 0;

    m_move = DEFAULT_VECTOR;
    m_fAcceleration = 1.0f;
    m_fGravity = 0.0f;
    m_fGravityLimit = 0.0f;

    m_fRotSpeed = 0.0f;

    m_col = DEFAULT_COLOR;
    m_colChangeRate = DEFAULT_COLOR_NONE;
    m_fFadeOutRate = 0.0f;
    m_nCntFadeOutLock = 0;

    m_sizeChangeRate = DEFAULT_VECTOR;
    m_nCntSizeChangeLock = 0;
    m_bSizeChangeStartOrFinish = false;

    m_bUseAdditiveSynthesis = false;
    m_bUseZBuffer = true;

    m_nCntAnimTime = 0;
    m_bOneRoundAnim = false;
    m_nAnimPattern = 0;
    m_nAnimSpeed = 0;
    m_bRepeat = false;

    m_bDisp = true;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �r���{�[�h�̏�����
    CBillboard::Init(pos, size);

    // �e�N�X�`�����o�C���h
    BindTexture(m_nTexType);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::Uninit(void)
{
    CBillboard::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::Update(void)
{
    // �ʒu�Ƒ傫�����擾
    D3DXVECTOR3 pos = GetPos();
    D3DXVECTOR3 size = GetSize();

    // �ʒu���X�V�i�d�͂�����x���l���j
    pos += m_move;
    m_move *= m_fAcceleration;
    m_move.y -= m_fGravity;
    if (m_fGravityLimit > 0.0f)
    {
        // �d�͐���
        if (m_move.y < -m_fGravityLimit)
        {
            m_move.y = -m_fGravityLimit;
        }
    }

    // �傫�������b�N����J�E���^
    if (m_nCntSizeChangeLock > 0)
    {
        m_nCntSizeChangeLock--;
    }

    // �傫�����A�J�n�ƏI���ɏꍇ�������Ȃ���X�V
    if (!m_bSizeChangeStartOrFinish)
    {
        // �J�n���A�J�E���^���Ȃ��Ȃ�傫���X�V
        if (m_nCntSizeChangeLock <= 0)
        {
            size += m_sizeChangeRate;
        }
    }
    else
    {
        // �I���Ȃ�A�J�E���^���Ȃ��Ȃ�܂ő傫���X�V
        if (m_nCntSizeChangeLock > 0)
        {
            size += m_sizeChangeRate;
        }
    }

    // �F���X�V
    m_col += m_colChangeRate;

    // ���l�����b�N����J�E���^
    if (m_nCntFadeOutLock > 0)
    {
        m_nCntFadeOutLock--;
    }

    // �A�j���[�V�������g���Ȃ�i�t�F�[�h�A�E�g���肪�ς��j
    if (m_nAnimPattern > 1)
    {
        // ������Ă��Ȃ��Ȃ�e�N�X�`���A�j���[�V�������X�V
        if (!m_bOneRoundAnim)
        {
            // �J�E���g�A�b�v
            m_nCntAnimTime++;

            // �e�N�X�`�����W���X�V
            if (m_nCntAnimTime == m_nAnimSpeed)
            {
                // ���s�[�g����Ȃ�A����̃t���O�ƌ��т��Ȃ�
                if (m_bRepeat)
                {
                    SetUpdateTexLeftToRight(m_nAnimPattern);
                }
                else
                {
                    m_bOneRoundAnim = SetUpdateTexLeftToRight(m_nAnimPattern);
                }

                // �J�E���^���Z�b�g
                m_nCntAnimTime = 0;
            }
        }
        else
        {
            // ���l�̃��b�N�������ꂽ��
            if (m_nCntFadeOutLock <= 0)
            {
                // ���������t�F�[�h�A�E�g�J�n
                m_col.a -= m_fFadeOutRate;
            }
        }
    }
    else
    {
        // ���l�̃��b�N�������ꂽ��
        if (m_nCntFadeOutLock <= 0)
        {
            // �A�j���[�V�������g��Ȃ��Ȃ�t�F�[�h�A�E�g�J�n
            m_col.a -= m_fFadeOutRate;
        }
    }

    // �ʒu�A�傫���A�F���r���{�[�h�ɔ��f
    CBillboard::SetPos(pos);
    CBillboard::SetSize(size);
    CBillboard::SetCol(m_col);

    // �r���{�[�h���X�V�i�p�x�����f�j
    float fRotAngle = GetRotAngle();
    fRotAngle += m_fRotSpeed;
    if (fRotAngle > D3DXToRadian(180.0f) || fRotAngle < D3DXToRadian(-180.0f))
    {
        fRotAngle *= -1;
    }
    CBillboard::SetRotAngle(fRotAngle);
    CBillboard::Update();

    // �����A�傫�����Ȃ��Ȃ�I������
    if (m_col.a <= 0.0f || size.x < 0.0f || size.y < 0.0f)
    {
        Uninit();
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::Draw(void)
{
    if (m_bDisp)
    {
        // ���Z����
        if (m_bUseAdditiveSynthesis)
        {
            CBillboard::SetAdditiveSynthesis();
        }

        // Z�o�b�t�@�𖳎����邩�ǂ���
        if (!m_bUseZBuffer)
        {
            CBillboard::SetZBufferNone();
        }

        CBillboard::Draw();
    }
}

//=============================================================================
// �G�t�F�N�g���o�����i��ԍŏ��ɌĂяo���j
// Author : �㓡�T�V��
//=============================================================================
CEffect3D* CEffect3D::Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius)
{
    // ��������G�t�F�N�g�̏��
    CEffect3D *pEffect3D = NULL;

    // �G�t�F�N�g�̐��������擾
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // ���o���a���w�肳��Ă���G�t�F�N�g�Ȃ�A�h��ւ���
    if (pCreateInfo->fEmitRadius > 0)
    {
        fEmitRadius = pCreateInfo->fEmitRadius;
    }

    // �������������A��������
    for (int nCnt = 0; nCnt < pCreateInfo->nNumOnce; nCnt++)
    {
        // �������a��0�ȏ�Ȃ�
        if (fEmitRadius > 0.0f)
        {
            // �p�x���o��
            float fAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;

            // ���o���̔��a���ǂ����邩
            D3DXVECTOR3 effectPos = pos;
            float fFinalEmitRadius = fEmitRadius;   // �~����
            if (!pCreateInfo->bOnCircleEdge)
            {
                // �~����
                int nEmitRadiusMax = int(fEmitRadius * EFFECT_FLOATING_POINT);
                fFinalEmitRadius = float(GetRandNum(nEmitRadiusMax, 0)) / EFFECT_FLOATING_POINT;
            }

            // ���ʂɂ���Ĉʒu�Ɣ�U�p�x���ꍇ����
            float fScatterAngle = 0.0f;
            if (pCreateInfo->plane == CEffectData::PLANE_XY)
            {
                effectPos.x += sinf(fAngle)*fFinalEmitRadius;
                effectPos.y += cosf(fAngle)*fFinalEmitRadius;

                // XZ�Ƃ͋t
                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((pos.x - effectPos.x), (pos.y - effectPos.y));
                }
                else
                {
                    fScatterAngle = atan2((effectPos.x - pos.x), (effectPos.y - pos.y));
                }
            }
            else if (pCreateInfo->plane == CEffectData::PLANE_XZ)
            {
                effectPos.x += -sinf(fAngle)*fFinalEmitRadius;
                effectPos.z += -cosf(fAngle)*fFinalEmitRadius;

                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((effectPos.x - pos.x), (effectPos.z - pos.z));
                }
                else
                {
                    fScatterAngle = atan2((pos.x - effectPos.x), (pos.z - effectPos.z));
                }
            }

            // ����
            if (!pEffect3D)
            {
                pEffect3D = Create(nType, effectPos, fScatterAngle);
            }
            else
            {
                Create(nType, effectPos, fScatterAngle);
            }
        }
        else
        {
            // ���ʂɂ���Ĕ�U�p�x���ꍇ����
            float fScatterAngle = 0.0f;
            if (pCreateInfo->plane == CEffectData::PLANE_XY)
            {
                // XZ�Ƃ͋t
                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((pos.x - posOld.x), (pos.y - posOld.y));
                }
                else
                {
                    fScatterAngle = atan2((posOld.x - pos.x), (posOld.y - pos.y));
                }
            }
            else if (pCreateInfo->plane == CEffectData::PLANE_XZ)
            {
                if (pCreateInfo->bEmitVectorInside)
                {
                    fScatterAngle = atan2((posOld.x - pos.x), (posOld.z - pos.z));
                }
                else
                {
                    fScatterAngle = atan2((pos.x - posOld.x), (pos.z - posOld.z));
                }
            }

            // ����
            if (!pEffect3D)
            {
                pEffect3D = Create(nType, pos, fScatterAngle);
            }
            else
            {
                Create(nType, pos, fScatterAngle);
            }
        }
    }

    // �ŏ��ɐ����������Ԃ�
    return pEffect3D;
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CEffect3D* CEffect3D::Create(const int nType, D3DXVECTOR3 pos, float fScatterAngle)
{
    // �G�t�F�N�g�̐��������擾
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // �������̊m��
    CEffect3D *pEffect3D = NULL;

    // �^�C�v���ƂɁAZ�o�b�t�@�𖳌�������G�t�F�N�g���ǂ��������߂�
    if (pCreateInfo->bUseZBuffer)
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D_Z_NONE);
    }
    else
    {
        pEffect3D = new CEffect3D(CScene::OBJTYPE_EFFECT3D);
    }

    // ��U�p�x�����ɐݒ肳��Ă���G�t�F�N�g�Ȃ�A�u��������
    if (pCreateInfo->nScatterAngle > 0)
    {
        fScatterAngle = ((float)pCreateInfo->nScatterAngle / EFFECT_FLOATING_POINT) * 2;
    }

    // ���������m�ۏo������
    if (pEffect3D != NULL)
    {
        // �e�N�X�`����񂩂�A�A�j���[�V�����̗L�����擾
        CTexture *pTexture = CManager::GetTexture();
        pEffect3D->m_nAnimPattern = pTexture->GetInfo(pCreateInfo->nTexType)->nPattern;
        pEffect3D->m_nAnimSpeed = pTexture->GetInfo(pCreateInfo->nTexType)->nSpeed;
        pEffect3D->m_bRepeat = pTexture->GetInfo(pCreateInfo->nTexType)->bRepeat;
        if (pEffect3D->m_nAnimPattern > 1)
        {
            pEffect3D->CBillboard::SetTexUV(0.0f, 1.0f / pEffect3D->m_nAnimPattern, 0.0f, 1.0f);
        }

        // �����̉�]�p�x�́A�ő��]���x��0���傫���Ȃ�ς���
        float fRotAngle = 0.0f;
        if (pCreateInfo->nRotSpeedMax > 0)
        {
            fRotAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;
            pEffect3D->m_fRotSpeed = D3DXToRadian(float(GetRandNum(pCreateInfo->nRotSpeedMax, pCreateInfo->nRotSpeedMin)) / EFFECT_FLOATING_POINT
                - float(GetRandNum(pCreateInfo->nRotSpeedMax, pCreateInfo->nRotSpeedMin)) / EFFECT_FLOATING_POINT);
        }
        pEffect3D->CBillboard::SetRotAngle(fRotAngle);

        // ��ނ����т��Ă���
        pEffect3D->m_nTexType = pCreateInfo->nTexType;

        // ������
        float fSize = 0.0f;
        if (pCreateInfo->nSizeMax > 0)
        {
            fSize = float(GetRandNum(pCreateInfo->nSizeMax, pCreateInfo->nSizeMin)) / EFFECT_FLOATING_POINT;
        }
        pEffect3D->Init(pos, D3DXVECTOR3(fSize, fSize, 0.0f));

        // �r���{�[�h�֐ݒ�𔽉f
        D3DXVECTOR3 rot = DEFAULT_VECTOR;
        switch (pCreateInfo->nLayRot)
        {
        case 1:
            rot = LAY_ROT_BOT;
            break;
        case 2:
            rot = LAY_ROT_LEFT;
            break;
        case 3:
            rot = LAY_ROT_TOP;
            break;
        case 4:
            rot = LAY_ROT_RIGHT;
            break;
        }
        pEffect3D->CBillboard::SetRot(rot);
        pEffect3D->CBillboard::SetColorVertex(pCreateInfo->col);
        pEffect3D->CBillboard::SetAlphaTestBorder(pCreateInfo->nAlphaTestBorder);

        // �����o�ϐ��Ɋe�l�����т��Ă���
        pEffect3D->m_fAcceleration = pCreateInfo->fAcceleration;
        pEffect3D->m_fGravity = pCreateInfo->fGravity;
        pEffect3D->m_fGravityLimit = pCreateInfo->fGravityLimit;

        pEffect3D->m_col = pCreateInfo->col;
        pEffect3D->m_colChangeRate = pCreateInfo->colChangeRate;
        pEffect3D->m_fFadeOutRate = pCreateInfo->fFadeOutRate;
        pEffect3D->m_nCntFadeOutLock = pCreateInfo->nCntFadeOutLock;

        pEffect3D->m_sizeChangeRate = pCreateInfo->sizeChangeRate;
        pEffect3D->m_nCntSizeChangeLock = pCreateInfo->nCntSizeChangeLock;
        pEffect3D->m_bSizeChangeStartOrFinish = pCreateInfo->bSizeChangeStartOrFinish;

        pEffect3D->m_bUseAdditiveSynthesis = pCreateInfo->bUseAdditiveSynthesis;
        pEffect3D->m_bUseZBuffer = pCreateInfo->bUseZBuffer;

        // �ړ��ʂ͍Ō�Ɍ���i�ړ��̌����ɉ����āA�����̉�]�p�x���ς��\�������邽�߁j
        pEffect3D->MoveCalculation(nType, fScatterAngle);
    }

    return pEffect3D;
}

//=============================================================================
// �ړ��ʂ��Z�o����
// Author : �㓡�T�V��
//=============================================================================
void CEffect3D::MoveCalculation(const int nType, const float fScatterAngle)
{
    // �G�t�F�N�g�̐��������擾
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // �������o��
    float fSpeed = float(GetRandNum(pCreateInfo->nSpeedMax, pCreateInfo->nSpeedMin)) / EFFECT_FLOATING_POINT;
    float fAuxiliaryAxisSpeed = float(GetRandNum(pCreateInfo->nAuxiliaryAxisSpeedMax, pCreateInfo->nAuxiliaryAxisSpeedMin)) / EFFECT_FLOATING_POINT;

    // �ړ��̊p�x�����߂�i��U���{��U�p�x�j
    float fScatterWidth = 0;
    if (pCreateInfo->nScatterWidth > 0)
    {
        fScatterWidth = float(rand() % pCreateInfo->nScatterWidth) / EFFECT_FLOATING_POINT - float(rand() % pCreateInfo->nScatterWidth) / EFFECT_FLOATING_POINT;
    }
    float fMoveAngle = fScatterAngle + fScatterWidth;

    // �����̉�]���ړ��̌����ɍ��킹�邩�ǂ���
    if (pCreateInfo->bRotEqualMoveAngle)
    {
        CBillboard::SetRotAngle(-fMoveAngle);  // �r���{�[�h�̕��ƍ��킹�邽�߂Ƀ}�C�i�X��������
    }

    // ���ʂɂ���Ĉړ��ʂ̌������ꍇ����
    if (pCreateInfo->plane == CEffectData::PLANE_XY)
    {
        m_move = D3DXVECTOR3(sinf(fMoveAngle) * fSpeed, cosf(fMoveAngle)* fSpeed, fAuxiliaryAxisSpeed);
    }
    else if (pCreateInfo->plane == CEffectData::PLANE_XZ)
    {
        m_move = D3DXVECTOR3(-sinf(fMoveAngle) * fSpeed, fAuxiliaryAxisSpeed, -cosf(fMoveAngle)* fSpeed);
    }
}