//===============================================
//
// �V�[�����2D�|���S������ (scene2d.cpp)
// Author : �㓡�T�V���A�r�c�I��i�e�N�X�`���u�����f�B���O�Ή��j
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
bool CScene2D::m_bAdditiveSynthesis = false;
bool CScene2D::m_bNega = false;

//===========================================
// �e���v���[�g�֐�                       
//===========================================

//===========================================
// ����Z
//
// ����   :data0,data1�̓f�[�^
// �߂�l :�v�Z����
//===========================================
template<class T> T Divide(const T data0, const T data1)
{
    // �ϐ��錾
    T answer;   // ����

                // ����
    answer = data0 / data1;

    return answer;
}

//============================================================
// �V�[�����2D�|���S���̃I�[�o�[���C�h���ꂽ�R���X�g���N�^
// Author : �㓡�T�V��
//============================================================
CScene2D::CScene2D(OBJTYPE objType) :CScene(objType)
{
    ZeroMemory(m_apTexture, sizeof(m_apTexture));
    m_pVtxBuff = NULL;
    m_pos = DEFAULT_VECTOR;
    m_size = DEFAULT_VECTOR;
    memset(m_anCounterAnim, 0, sizeof(m_anCounterAnim));
    memset(m_anPatternAnim, 0, sizeof(m_anPatternAnim));

    m_nNumTexture = 0;
    memset(m_aBrend, BREND_NORMAL, sizeof(m_aBrend));

    m_nAlphaTestBorder = DEFAULT_ALPHATEST_BORDER_2D;
}

//============================================================
// �V�[�����2D�|���S���̃f�X�g���N�^
// Author : �㓡�T�V��
//============================================================
CScene2D::~CScene2D()
{

}

//============================================================
// �V�[�����2D�|���S���̏���������
// Author : �㓡�T�V��
//============================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �ʒu�����т���
    m_pos = pos;

    // �傫�������т���
    m_size = size;

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

    // ���_���W�̐ݒ�i�E���Őݒ肷��j
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // rhw�̐ݒ�i�l��1.0�Őݒ肷��j
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // ���_�J���[��ݒ�i0.0f�`1.0f�̒l�Őݒ肷��j
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //�e�N�X�`�����W�̐ݒ�
    for (int nCount = 0; nCount < MAX_BREND_TEXTURE; nCount++)
    {
        pVtx[0].tex[nCount] = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex[nCount] = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex[nCount] = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex[nCount] = D3DXVECTOR2(1.0f, 1.0f);
    }

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return S_OK;
}

//============================================================
// �V�[�����2D�|���S���̏I������
// Author : �㓡�T�V��
//============================================================
void CScene2D::Uninit(void)
{
    // ���_�o�b�t�@�̔j��
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//============================================================
// �V�[�����2D�|���S���̍X�V����
// Author : �㓡�T�V��
//============================================================
void CScene2D::Update(void)
{

}

//=============================================================
// �V�[�����2D�|���S���̕`�揈��
// Author : �r�c�I��i�e�N�X�`���u�����f�B���O�Ή��j�A�㓡�T�V��
//=============================================================
void CScene2D::Draw(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    // �A���t�@�e�X�g�i��⋭���j
    if (m_nAlphaTestBorder != DEFAULT_ALPHATEST_BORDER_2D)
    {
        //�A���t�@�e�X�g��L����
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        //�A���t�@�e�X�g��l�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaTestBorder);
        //�A���t�@�e�X�g�̔�r���@�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    }

    // ���Z����
    if (m_bAdditiveSynthesis)
    {
        // �����_�[�X�e�[�g(���Z�����ɂ���)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    // ���]����
    if (m_bNega)
    {
        // ���]�����ōl���ł��Ȃ����l���A�A���t�@�e�X�g�Ő��䂷��
        // �A���t�@�e�X�g��L����
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        // �A���t�@�e�X�g��l�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaTestBorder);
        // �A���t�@�e�X�g�̔�r���@�̐ݒ�
        pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

        // ���]����
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
    }

    // �u�����h���@�ݒ�@//�r�c�ǉ�
    for (int nCount = 0; nCount < m_nNumTexture; nCount++)
    {
		if (m_aBrend[nCount] == BREND_IGNORE_INFO)
		{
			pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		}
		else
		{
			switch (nCount)
			{
			case 0:
                pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
                pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_MODULATE);
				pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // �A���t�@�u�����f�B���O����
				break;
            default:
				switch (m_aBrend[nCount])
				{
				case BREND_NORMAL:
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_CURRENT);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					break;

				case BREND_SEAL:
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_ADD);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
					break;
				case BREND_APPLY_INFO:
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLOROP, D3DTOP_MODULATE);
					pDevice->SetTextureStageState(nCount, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
					pDevice->SetTextureStageState(nCount, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                    break;
				}
				break;
			}
		}
    }

    // �e�N�X�`���̐ݒ� //�r�c�ύX
    for (int nCount = 0; nCount < m_nNumTexture; nCount++)
    {
        pDevice->SetTexture(nCount, m_apTexture[nCount]);
    }

    // �|���S���̕`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    // ���]������߂�
    if (m_bNega)
    {
        // �A���t�@�e�X�g�𖳌���
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

        // ���]������߂�
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        m_bNega = false;
    }

    // ���Z������߂�
    if (m_bAdditiveSynthesis)
    {
        // �����_�[�X�e�[�g(���Z������߂�)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // ���Z�����̃t���O��false�ɖ߂�
        m_bAdditiveSynthesis = false;
    }

    // �A���t�@�e�X�g��߂�
    if (m_nAlphaTestBorder != DEFAULT_ALPHATEST_BORDER_2D)
    {
        //�A���t�@�e�X�g�𖳌���
        pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }

    // �e�N�X�`���̐ݒ�̉����@//�r�c�ύX
    for (int nCount = 0; nCount < MAX_BREND_TEXTURE; nCount++)
    {
        pDevice->SetTexture(nCount, NULL);
    }

    // �u�����h���@��߂�
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

//=============================================================
// �V�[�����2D�|���S���̃e�N�X�`�������蓖��
// Author : �㓡�T�V���A�r�c�I��
//=============================================================
int CScene2D::BindTexture(const int nNumTexture, const BREND brend)
{
    // �߂�l�ŁA���ݎg���Ă���e�N�X�`������Ԃ��p
    int nCurrentNumTex = m_nNumTexture;

    // �e�N�X�`���������Ă��āA�u�����h�̕��@�����߂��ݎg���Ă���e�N�X�`���������Z
    CTexture *pTexture = CManager::GetTexture();
    m_apTexture[m_nNumTexture] = pTexture->GetInfo(nNumTexture)->pTexture;
    m_aBrend[m_nNumTexture++] = brend;

    return nCurrentNumTex;
}

//=============================================================
// �V�[�����2D�|���S���̒��_���W��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetVertex(void)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(+(m_size.x / 2), +(m_size.y / 2), 0.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �V�[�����2D�|���S���̉�]���钸�_���W��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetRotVertex(float fAngle)
{
    // �ϐ��錾
    // �e���_
    D3DXVECTOR3 vertex1 = DEFAULT_VECTOR;
    D3DXVECTOR3 vertex2= DEFAULT_VECTOR;
    D3DXVECTOR3 vertex3= DEFAULT_VECTOR;
    D3DXVECTOR3 vertex4= DEFAULT_VECTOR;

    //==========================================================================================================
    // �摜���A�摜�̒��S�����ɉ�]������
    // ����̒��_
    vertex1.x = -(m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex1.y = -(m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);

    // �E��̒��_
    vertex2.x = (m_size.x / 2)*cosf(fAngle)
        - (-(m_size.y / 2))*sinf(fAngle);
    vertex2.y = (m_size.x / 2)*sinf(fAngle)
        + (-(m_size.y / 2))*cosf(fAngle);

    // �����̒��_
    vertex3.x = -(m_size.x / 2)*cosf(fAngle)
        - (m_size.y / 2)*sinf(fAngle);
    vertex3.y = -(m_size.x / 2)*sinf(fAngle)
        + (m_size.y / 2)*cosf(fAngle);

    // �E���̒��_
    vertex4.x = (m_size.x / 2)*cosf(fAngle)
        - (m_size.y / 2)*sinf(fAngle);
    vertex4.y = (m_size.x / 2)*sinf(fAngle)
        + (m_size.y / 2)*cosf(fAngle);
    //==========================================================================================================

    // ���_���ւ̃|�C���^
    VERTEX_2D *pVtx = NULL;

    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // ���_���W�̐ݒ�
    pVtx[0].pos = m_pos + vertex1;
    pVtx[1].pos = m_pos + vertex2;
    pVtx[2].pos = m_pos + vertex3;
    pVtx[3].pos = m_pos + vertex4;

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �V�[�����2D�|���S���̌�������̒��_���W��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // ���_���W�̐ݒ�
    pVtx[0].pos = posVisual + D3DXVECTOR3(-(sizeVisual.x / 2), -(sizeVisual.y / 2), 0.0f);
    pVtx[1].pos = posVisual + D3DXVECTOR3(+(sizeVisual.x / 2), -(sizeVisual.y / 2), 0.0f);
    pVtx[2].pos = posVisual + D3DXVECTOR3(-(sizeVisual.x / 2), +(sizeVisual.y / 2), 0.0f);
    pVtx[3].pos = posVisual + D3DXVECTOR3(+(sizeVisual.x / 2), +(sizeVisual.y / 2), 0.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �V�[�����2D�|���S���̃A�j���[�V������ݒ�
// Author : �㓡�T�V��
//=============================================================
bool CScene2D::SetAnimation(int nSpeed, int nPattern, int nTex)
{
    // �ϐ��錾
    bool bOneRound = false;   // �A�j���[�V����������������ǂ���

    // �A�j���[�V����
    m_anCounterAnim[nTex]++;	//�J�E���^���Z
    if (m_anCounterAnim[nTex] == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_anCounterAnim[nTex] = 0;  // �J�E���^��0�ɖ߂�

        // �A�j���[�V���������������
        if ((m_anPatternAnim[nTex] + 1) % nPattern == 0)
        {
            // ����̃t���O��true��
            bOneRound = true;
        }

        // �A�j���[�V������؂�ւ���
        m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // ����
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

                                   // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// �V�[�����2D�|���S���̋t�����̃A�j���[�V������ݒ�
// Author : �㓡�T�V��
//=============================================================
bool CScene2D::SetReverseAnimation(int nSpeed, int nPattern, int nTex)
{
    // �ϐ��錾
    bool bOneRound = false;   // �A�j���[�V����������������ǂ���

    // �A�j���[�V����
    m_anCounterAnim[nTex]++;	//�J�E���^���Z
    if (m_anCounterAnim[nTex] == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_anCounterAnim[nTex] = 0;  // �J�E���^��0�ɖ߂�

        // �A�j���[�V���������������
        if ((m_anPatternAnim[nTex] + 1) % nPattern == 0)
        {
            // ����̃t���O��true��
            bOneRound = true;
        }

        // �A�j���[�V������؂�ւ���
        m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // ����
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

                                   // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + fEqualDivision, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// �V�[�����2D�|���S���̃A�j���[�V������ݒ�
// Author : �㓡�T�V��
//=============================================================
bool CScene2D::SetFlowingAnimation(int nSpeed, int nPattern, bool bRightToLeft, DIRECT direct, int nTex)
{
    // �ϐ��錾
    bool bOneRound = false;   // �A�j���[�V����������������ǂ���

    // �A�j���[�V����
    m_anCounterAnim[nTex]++;	//�J�E���^���Z
    if (m_anCounterAnim[nTex] == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_anCounterAnim[nTex] = 0;  // �J�E���^��0�ɖ߂�

        // �A�j���[�V���������������
        if ((m_anPatternAnim[nTex] + 1) % nPattern == 0)
        {
            // ����̃t���O��true��
            bOneRound = true;
        }

        if (bRightToLeft)
        {
            // �A�j���[�V������؂�ւ���
            m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // ����
        }
        else
        {
            // �A�j���[�V������؂�ւ���
            m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] - 1) % nPattern;  // ����
        }
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

    // �e�N�X�`�����W�̐ݒ�
    // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

    // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    switch (direct)
    {
    case CScene2D::DIRECT_VERTICAL:
        // �c
        pVtx[0].tex[nTex] = D3DXVECTOR2(0.0f, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[1].tex[nTex] = D3DXVECTOR2(1.0f, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[2].tex[nTex] = D3DXVECTOR2(0.0f, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(1.0f, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        break;

    case CScene2D::DIRECT_HORIZON:
        // ��
        pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 0.0f);
        pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, 0.0f);
        pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, 1.0f);
        break;

    case CScene2D::DIRECT_RIGHT_UP:
        // �E���オ��
        pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * (fEqualDivision * (-1)));
        pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * (fEqualDivision * (-1)));
        pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * (fEqualDivision * (-1)) + 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * (fEqualDivision * (-1)) + 1.0f);

        break;

    case CScene2D::DIRECT_RIGHT_DOWN:
        // �E��������
        pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * fEqualDivision);
        pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivision + 1.0f, m_anPatternAnim[nTex] * fEqualDivision + 1.0f);
        break;
    }

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();

    return bOneRound;
}

//=============================================================
// �e�N�X�`���̕`��͈͂̐ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetTextureRange(int nRange, int nPattern, int nTex)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

                                   // ���������邩�v�Z
    fEqualDivision = Divide((float)nRange, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex[nTex] = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(fEqualDivision, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fEqualDivision, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �i���̂���A�j���[�V�����̐ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetParagraphAnimation(int nParagraph, int nMaxParagraph, int nSpeed, int nPattern, int nTex)
{
    // �A�j���[�V����
    m_anCounterAnim[nTex]++;	//�J�E���^���Z
    if (m_anCounterAnim[nTex] == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_anCounterAnim[nTex] = 0;  // �J�E���^��0�ɖ߂�

        // �A�j���[�V������؂�ւ���
        m_anPatternAnim[nTex] = (m_anPatternAnim[nTex] + 1) % nPattern;  // ����
    }

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivisionX = 0.0f;   // �e�N�X�`���𓙕�����i�c�j
    float fEqualDivisionY = 0.0f;   // �e�N�X�`���𓙕�����i���j

                                    // ���������邩�v�Z
    fEqualDivisionX = Divide(1.0f, (float)nPattern);
    fEqualDivisionY = Divide(1.0f, (float)nMaxParagraph);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX, fEqualDivisionY * (nParagraph - 1));
    pVtx[1].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX + fEqualDivisionX, fEqualDivisionY * (nParagraph - 1));
    pVtx[2].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX, fEqualDivisionY * nParagraph);
    pVtx[3].tex[nTex] = D3DXVECTOR2(m_anPatternAnim[nTex] * fEqualDivisionX + fEqualDivisionX, fEqualDivisionY * nParagraph);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �e�N�X�`���̕`��ꏊ�����߂�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetTexturePlace(int nPlace, int nPattern, int nTex)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

    // �e�N�X�`�����W�̐ݒ�
    // �ϐ��錾
    float fEqualDivision = 0.0f;   // �e�N�X�`���𓙕�����

    // ���������邩�v�Z
    fEqualDivision = Divide(1.0f, (float)nPattern);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex[nTex] = D3DXVECTOR2((fEqualDivision * nPlace), 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(fEqualDivision + (fEqualDivision * nPlace), 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2((fEqualDivision * nPlace), 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fEqualDivision + (fEqualDivision * nPlace), 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �i��������e�N�X�`���̕`��ꏊ�����߂�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetParagraphTexturePlace(int nPlace, int nParagraph, int nMaxParagraph, int nPattern, int nTex)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

                                                // �e�N�X�`�����W�̐ݒ�
                                                // �ϐ��錾
    float fEqualDivisionX = 0.0f;   // �e�N�X�`���𓙕�����i�c�j
    float fEqualDivisionY = 0.0f;   // �e�N�X�`���𓙕�����i���j

                                    // ���������邩�v�Z
    fEqualDivisionX = Divide(1.0f, (float)nPattern);
    fEqualDivisionY = Divide(1.0f, (float)nMaxParagraph);

    // �e�N�X�`���̍��W�𔽉f
    pVtx[0].tex[nTex] = D3DXVECTOR2((fEqualDivisionX * nPlace), fEqualDivisionY * (nParagraph - 1));
    pVtx[1].tex[nTex] = D3DXVECTOR2(fEqualDivisionX + (fEqualDivisionX * nPlace), fEqualDivisionY * (nParagraph - 1));
    pVtx[2].tex[nTex] = D3DXVECTOR2((fEqualDivisionX * nPlace), fEqualDivisionY * nParagraph);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fEqualDivisionX + (fEqualDivisionX * nPlace), fEqualDivisionY * nParagraph);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �A�j���[�V�����̃J�E���^�𗘗p����Ƃ��Ɏg��
// Author : �㓡�T�V��
//=============================================================
int CScene2D::CountAnimation(int nSpeed, int nPattern)
{
    // �A�j���[�V����
    m_anCounterAnim[0]++;	//�J�E���^���Z
    if (m_anCounterAnim[0] == nSpeed)//����
    {
        // �I�[�o�[�t���[�h�~
        m_anCounterAnim[0] = 0;  // �J�E���^��0�ɖ߂�

                             // �A�j���[�V������؂�ւ���
        m_anPatternAnim[0] = (m_anPatternAnim[0] + 1) % nPattern;  // ����
    }

    // �A�j���[�V�����̃p�^�[���̃J�E���^��Ԃ�
    return m_anPatternAnim[0];
}

//=============================================================
// �V�[�����2D�|���S���̐F��ς���
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // ���_�J���[��ݒ�i0.0f�`1.0f�̒l�Őݒ肷��j
    pVtx[0].col = col;
    pVtx[1].col = col;
    pVtx[2].col = col;
    pVtx[3].col = col;

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �����Q�[�W�̃T�C�Y��ݒ�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetLeftToRightGauge(float fMax, float fNow, int nTex)
{
    // �d��
    float fWeight = fNow / fMax;

    VERTEX_2D *pVtx = NULL;	// ���_���ւ̃|�C���^

                            // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // ���_���W�̐ݒ�(���ɑ����Ă���A�E�ɐL�΂��C���[�W)
    pVtx[0].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), -(m_size.y / 2), 0.0f);
    pVtx[1].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2) + (m_size.x * fWeight), -(m_size.y / 2), 0.0f);
    pVtx[2].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2), +(m_size.y / 2), 0.0f);
    pVtx[3].pos = m_pos + D3DXVECTOR3(-(m_size.x / 2) + (m_size.x * fWeight), +(m_size.y / 2), 0.0f);

    // �e�N�X�`�����W���X�V
    pVtx[0].tex[nTex] = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex[nTex] = D3DXVECTOR2(fWeight, 0.0f);
    pVtx[2].tex[nTex] = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex[nTex] = D3DXVECTOR2(fWeight, 1.0f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}

//=============================================================
// �[��1�s�N�Z�������Α��ɍs�����ۂ���������֐�
// Author : �㓡�T�V��
//=============================================================
void CScene2D::SetShaveTex(void)
{
    // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    VERTEX_2D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 1�s�N�Z�����炢���
    pVtx[0].tex[0] = D3DXVECTOR2(0.01f, 0.01f);
    pVtx[1].tex[0] = D3DXVECTOR2(0.99f, 0.01f);
    pVtx[2].tex[0] = D3DXVECTOR2(0.01f, 0.99f);
    pVtx[3].tex[0] = D3DXVECTOR2(0.99f, 0.99f);

    //���_�f�[�^���A�����b�N����
    m_pVtxBuff->Unlock();
}
