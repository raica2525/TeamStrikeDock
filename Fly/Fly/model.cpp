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
#include "model.h"
#include "shadow.h"
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	//���f���f�[�^�̏�����
	ZeroMemory(&m_ModelData, sizeof(m_ModelData));	
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
// 3D�I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CModel::Init(void)
{

	CScene3d::Init();
	return S_OK;
}

//=============================================================================
// 3D�I�u�W�F�N�g�̏I������
//=============================================================================
void CModel::Uninit(void)
{
	CScene3d::Uninit();
	Release();
}

//=============================================================================
// 3D�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// 3D�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL*pMat;
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
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
		pDevice->SetTexture(0, m_ModelData.pTexture[nCntMat]);
		m_ModelData.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetTexture(0, NULL);
	//�}�e���A�����̃��Z�b�g
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// 3D�I�u�W�F�N�g�̐���
//=============================================================================
CModel* CModel::Create(CModel* pParentModel,MODELDATA modeldata,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{

	// �������̊m��
	CModel* pModel;
	pModel = new CModel;
	//�e����̐ݒ�
	pModel->SetPosition(pos);
	pModel->SetRotation(rot);
	pModel->SetScale(size);
	pModel->m_pParentModel = pParentModel;
	pModel->m_ModelData = modeldata;

	CShadow::Create(pModel, modeldata, pos, rot, size);
	// ����������
	pModel->Init();

	return pModel;
}

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//���f���̊���
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
void CModel::BindModel(MODELDATA modeldata)
{
	m_ModelData = modeldata;
}
