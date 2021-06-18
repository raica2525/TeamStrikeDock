//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	model.cpp
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "shadow.h"
#include "model.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CShadow::CShadow()
{
	//���f���f�[�^�̏�����
	ZeroMemory(&m_ModelData, sizeof(m_ModelData));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{
}

//=============================================================================
// 3D�I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CShadow::Init(void)
{

	CScene3d::Init();
	return S_OK;
}

//=============================================================================
// 3D�I�u�W�F�N�g�̏I������
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3d::Uninit();
	Release();
}

//=============================================================================
// 3D�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CShadow::Update(void)
{
	D3DXVECTOR3 pos = m_pParentModel->GetPosition();
	D3DXVECTOR3 rot = m_pParentModel->GetRotation();
	D3DXVECTOR3 size = m_pParentModel->GetScale();
	pos.y = 0;
	rot.x *= -1;
	SetPosition(pos);
	SetRotation(rot);
	SetScale(size);
}

//=============================================================================
// 3D�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	if (CManager::GetUseShadow())
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
		D3DMATERIAL9 matDef;
		D3DXMATERIAL*pMat;

		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

		//���[���h�ϊ��s��̏�����
		D3DXMatrixIdentity(&mtxWorld);
		//��]�s��̐����ƌv�Z
		D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRotation().y, GetRotation().x, GetRotation().z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//�g��k���s��̐����ƌv�Z
		D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		//���s�ړ��s��̐����ƌv�Z
		D3DXMatrixTranslation(&mtxTrans, GetPosition().x, GetPosition().y, GetPosition().z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);


		//�v�Z���ʂ�K�p
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//�}�e���A�����̎擾
		pDevice->GetMaterial(&matDef);
		//���f���f�[�^�ǂݍ��ݎ��s���̏���
		if (m_ModelData.pBuffMat == NULL)
		{
			MessageBox(FindWindow(CLASS_NAME, NULL), "���f���f�[�^�ǂݍ��ݎ��s", "�x��", MB_ICONWARNING);
			exit(EXIT_FAILURE);
		}
		pMat = (D3DXMATERIAL*)m_ModelData.pBuffMat->GetBufferPointer();

		//�}�e���A�����̐ݒ�ƃ}�e���A�����Ƃɕ`��
		for (int nCntMat = 0; nCntMat < (int)m_ModelData.nNumMat; nCntMat++)
		{
			pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			if (m_ModelData.pTexture[nCntMat] != NULL)
			{
				pDevice->SetTexture(0, m_ModelData.pTexture[nCntMat]);
			}
			m_ModelData.pMesh->DrawSubset(nCntMat);
		}
		//�}�e���A�����̃��Z�b�g
		pDevice->SetMaterial(&matDef);

		// Z�o�b�t�@�ݒ� => �L��
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		// ZBUFFER��r�ݒ�ύX => (�Q�ƒl <= �o�b�t�@�l)
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}


}

//=============================================================================
// 3D�I�u�W�F�N�g�̐���
//=============================================================================
CShadow* CShadow::Create(CModel *pParentModel, CModel::MODELDATA modeldata, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	// �������̊m��
	CShadow* pShadow;
	pShadow = new CShadow;
	//�e����̐ݒ�
	pShadow->SetPosition(pos);
	pShadow->SetRotation(rot);
	pShadow->SetScale(size);
	pShadow->m_pParentModel = pParentModel;
	pShadow->m_ModelData = modeldata;
	// ����������
	pShadow->Init();

	return pShadow;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//���f���̊���
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CShadow::BindModel(CModel::MODELDATA modeldata)
{
	m_ModelData = modeldata;
}
