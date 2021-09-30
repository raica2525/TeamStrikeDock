//===============================================
//
// �G�t�F�N�g2D���� (effect2d.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "effect2d.h"
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
CEffect2D::CEffect2D(CScene::OBJTYPE objtype) :CScene2D(objtype)
{
    m_nTexType = 0;

    m_move = DEFAULT_VECTOR;
    m_fAcceleration = 1.0f;
    m_fGravity = 0.0f;
    m_fGravityLimit = 0.0f;

    m_fRotAngle = 0.0f;
    m_fRotSpeed = 0.0f;

    m_col = DEFAULT_COLOR;
    m_colChangeRate = DEFAULT_COLOR_NONE;
    m_fFadeOutRate = 0.0f;
    m_nCntFadeOutLock = 0;

    m_sizeChangeRate = DEFAULT_VECTOR;
    m_nCntSizeChangeLock = 0;
    m_bSizeChangeStartOrFinish = false;

    m_bUseAdditiveSynthesis = false;

    m_bOneRoundAnim = false;
    m_nAnimParagraph = 0;
    m_nAnimPattern = 0;
    m_nAnimSpeed = 0;
    m_bRepeat = false;

    m_bUseUpdate = true;
    m_pPlayer = NULL;
}

//=============================================================================
// �f�X�g���N�^
// Author : �㓡�T�V��
//=============================================================================
CEffect2D::~CEffect2D()
{

}

//=============================================================================
// ����������
// Author : �㓡�T�V��
//=============================================================================
HRESULT CEffect2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�����o�C���h
    BindTexture(m_nTexType);

    return S_OK;
}

//=============================================================================
// �I������
// Author : �㓡�T�V��
//=============================================================================
void CEffect2D::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// �X�V����
// Author : �㓡�T�V��
//=============================================================================
void CEffect2D::Update(void)
{
    if (!m_pPlayer || !CGame::GetCurrentSpShot())
    {
        // �X�V����������Ȃ�
        if (m_bUseUpdate)
        {
            // �ʒu�Ƒ傫�����擾
            D3DXVECTOR3 pos = GetPosition();
            D3DXVECTOR3 size = GetSize();

            // �ʒu���X�V�i�d�͂�����x���l���j
            pos += m_move;
            m_move *= m_fAcceleration;
            m_move.y += m_fGravity; // ���Ӂi3D�Ƌt�j
            if (m_fGravityLimit > 0.0f)
            {
                // �d�͐����i3D�Ƌt�j
                if (m_move.y > m_fGravityLimit)
                {
                    m_move.y = m_fGravityLimit;
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
                    // ���s�[�g����Ȃ�A����̃t���O�ƌ��т��Ȃ�
                    if (m_bRepeat)
                    {
                        if (m_nAnimParagraph > 2)
                        {
                            SetAllParagraphAnimation(m_nAnimParagraph, m_nAnimSpeed, m_nAnimPattern);
                        }
                        else
                        {
                            SetAnimation(m_nAnimSpeed, m_nAnimPattern);
                        }
                    }
                    else
                    {
                        if (m_nAnimParagraph >= 2)
                        {
                            m_bOneRoundAnim = SetAllParagraphAnimation(m_nAnimParagraph, m_nAnimSpeed, m_nAnimPattern);
                        }
                        else
                        {
                            m_bOneRoundAnim = SetAnimation(m_nAnimSpeed, m_nAnimPattern);
                        }
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

            // �傫���̐����ieffect3d���玝���Ă����ۂɁA���]�����p�Œǉ��j
            if (size.x > SCREEN_WIDTH * 5)
            {
                size.x = SCREEN_WIDTH * 5;
            }

            if (size.y > SCREEN_HEIGHT * 5)
            {
                size.y = SCREEN_HEIGHT * 5;
            }

            // �v���C���[�̃|�C���^������Ȃ�A�ꏊ��Ǐ]
            if (m_pPlayer)
            {
                pos = ConvertScreenPos(m_pPlayer->GetPos() + D3DXVECTOR3(0.0f, m_pPlayer->GetCollisionSizeDeffence().y / 2, 0.0f));
            }

            // �ʒu�A�傫���A�F�𔽉f
            CScene2D::SetPosition(pos);
            CScene2D::SetSize(size);
            CScene2D::SetColor(m_col);

            // �e���_���X�V�i�p�x�����f�j
            m_fRotAngle += m_fRotSpeed;
            if (m_fRotAngle > D3DXToRadian(180.0f) || m_fRotAngle < D3DXToRadian(-180.0f))
            {
                m_fRotAngle *= -1;
            }
            CScene2D::SetRotVertex(m_fRotAngle);

            // �����A�傫�����Ȃ��Ȃ�I������
            if (m_col.a <= 0.0f || size.x < 0.0f || size.y < 0.0f)
            {
                Uninit();
            }
        }
    }
}

//=============================================================================
// �`�揈��
// Author : �㓡�T�V��
//=============================================================================
void CEffect2D::Draw(void)
{
    bool bDraw = true;
    if (m_pPlayer)
    {
        // �K�E�Z���g���Ă���Ƃ��ɁA�Ǐ]�G�t�F�N�g�͕`�悵�Ȃ�
        if (CGame::GetCurrentSpShot())
        {
            bDraw = false;
        }
    }

    if (bDraw)
    {
        // ���Z����
        if (m_bUseAdditiveSynthesis)
        {
            CScene2D::SetAdditiveSynthesis();
        }

        // ���]����
        if (m_nTexType == 20)
        {
            SetNega();
        }

        CScene2D::Draw();
    }
}

//=============================================================================
// �G�t�F�N�g���o�����i��ԍŏ��ɌĂяo���j
// Author : �㓡�T�V��
//=============================================================================
CEffect2D* CEffect2D::Emit(const int nType, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fEmitRadius)
{
    // ��������G�t�F�N�g�̏��
    CEffect2D *pEffect2D = NULL;

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
            if (!pEffect2D)
            {
                pEffect2D = Create(nType, effectPos, fScatterAngle);
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
            if (!pEffect2D)
            {
                pEffect2D = Create(nType, pos, fScatterAngle);
            }
            else
            {
                Create(nType, pos, fScatterAngle);
            }
        }
    }

    // �ŏ��ɐ����������Ԃ�
    return pEffect2D;
}

//=============================================================================
// �C���X�^���X��������
// Author : �㓡�T�V��
//=============================================================================
CEffect2D* CEffect2D::Create(const int nType, D3DXVECTOR3 pos, float fScatterAngle, const bool bIn3D)
{
    // �G�t�F�N�g�̐��������擾
    CEffectData *pEffectData = CManager::GetEffectData();
    CEffectData::CreateInfo *pCreateInfo = pEffectData->GetCreateInfo(nType);

    // �������̊m��
    CEffect2D *pEffect2D = NULL;

    // �^�C�v���ƂɁAUI�̎�O�����̃G�t�F�N�g���ǂ��������߂�
    if (pCreateInfo->bUseZBuffer)
    {
        pEffect2D = new CEffect2D(CScene::OBJTYPE_EFFECT2D_BACK_UI);
    }
    else
    {
        pEffect2D = new CEffect2D(CScene::OBJTYPE_EFFECT2D_FRONT_UI);
    }

    // ��U�p�x�����ɐݒ肳��Ă���G�t�F�N�g�Ȃ�A�u��������
    if (pCreateInfo->nScatterAngle > 0)
    {
        fScatterAngle = ((float)pCreateInfo->nScatterAngle / EFFECT_FLOATING_POINT) * 2;
    }

    // ���������m�ۏo������
    if (pEffect2D != NULL)
    {
        // �ݒ�𔽉f
        //pEffect2D->CScene2D::SetColor(pCreateInfo->col);�ieffect3d���玝���Ă����ۂɁAeffect2d�ł͐���1F��Ɍ�����悤�ɂȂ邽�߁A�폜�j
        pEffect2D->CScene2D::SetAlphaTestBorder(pCreateInfo->nAlphaTestBorder);

        // �e�N�X�`����񂩂�A�A�j���[�V�����̗L�����擾
        CTexture *pTexture = CManager::GetTexture();
        pEffect2D->m_nAnimParagraph = pTexture->GetInfo(pCreateInfo->nTexType)->nParagraph;
        pEffect2D->m_nAnimPattern = pTexture->GetInfo(pCreateInfo->nTexType)->nPattern;
        pEffect2D->m_nAnimSpeed = pTexture->GetInfo(pCreateInfo->nTexType)->nSpeed;
        pEffect2D->m_bRepeat = pTexture->GetInfo(pCreateInfo->nTexType)->bRepeat;

        // �����̉�]�p�x�́A�ő��]���x��0���傫���Ȃ�ς���
        float fRotAngle = 0.0f;
        if (pCreateInfo->nRotSpeedMax > 0)
        {
            fRotAngle = float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT - float(rand() % EFFECT_PI) / EFFECT_FLOATING_POINT;
            pEffect2D->m_fRotSpeed = D3DXToRadian(float(GetRandNum(pCreateInfo->nRotSpeedMax, pCreateInfo->nRotSpeedMin)) / EFFECT_FLOATING_POINT
                - float(GetRandNum(pCreateInfo->nRotSpeedMax, pCreateInfo->nRotSpeedMin)) / EFFECT_FLOATING_POINT);
        }
        pEffect2D->m_fRotAngle = fRotAngle;

        // ��ނ����т��Ă���
        pEffect2D->m_nTexType = pCreateInfo->nTexType;

        // �ʒu�ƃT�C�Y��������
        if (bIn3D)
        {
            pos = ConvertScreenPos(pos);    // 3D��Ȃ�A�X�N���[�����W�ɕϊ�
        }
        float fSize = 0.0f;
        if (pCreateInfo->nSizeMax > 0)
        {
            fSize = float(GetRandNum(pCreateInfo->nSizeMax, pCreateInfo->nSizeMin)) / EFFECT_FLOATING_POINT;
        }
        pEffect2D->Init(pos, D3DXVECTOR3(fSize, fSize, 0.0f));

        // �����o�ϐ��Ɋe�l�����т��Ă���
        pEffect2D->m_fAcceleration = pCreateInfo->fAcceleration;
        pEffect2D->m_fGravity = pCreateInfo->fGravity;
        pEffect2D->m_fGravityLimit = pCreateInfo->fGravityLimit;

        pEffect2D->m_col = pCreateInfo->col;
        pEffect2D->m_colChangeRate = pCreateInfo->colChangeRate;
        pEffect2D->m_fFadeOutRate = pCreateInfo->fFadeOutRate;
        pEffect2D->m_nCntFadeOutLock = pCreateInfo->nCntFadeOutLock;

        pEffect2D->m_sizeChangeRate = pCreateInfo->sizeChangeRate;
        pEffect2D->m_nCntSizeChangeLock = pCreateInfo->nCntSizeChangeLock;
        pEffect2D->m_bSizeChangeStartOrFinish = pCreateInfo->bSizeChangeStartOrFinish;

        pEffect2D->m_bUseAdditiveSynthesis = pCreateInfo->bUseAdditiveSynthesis;

        // �ړ��ʂ͍Ō�Ɍ���i�ړ��̌����ɉ����āA�����̉�]�p�x���ς��\�������邽�߁j
        pEffect2D->MoveCalculation(nType, fScatterAngle);
    }

    return pEffect2D;
}

//=============================================================================
// �ړ��ʂ��Z�o����
// Author : �㓡�T�V��
//=============================================================================
void CEffect2D::MoveCalculation(const int nType, const float fScatterAngle)
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
        m_fRotAngle = -fMoveAngle;
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