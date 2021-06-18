//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	polygon3d.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "polygon3d.h"

#include "manager.h"
#include "renderer.h"

CPolygon3d::CPolygon3d()
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
}
CPolygon3d::~CPolygon3d()
{
}

HRESULT CPolygon3d::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_3D* pVtx  = NULL;
    D3DXVECTOR3 size = GetScale();
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-size.x / 2, 0, size.y / 2);
	pVtx[1].pos = D3DXVECTOR3(size.x / 2, 0, size.y / 2);
	pVtx[2].pos = D3DXVECTOR3(-size.x / 2, 0, -size.y / 2);
	pVtx[3].pos = D3DXVECTOR3(size.x / 2, 0, -size.y / 2);

	pVtx[0].nor = D3DXVECTOR3(0, 1, 0);
	pVtx[1].nor = D3DXVECTOR3(0, 1, 0);
	pVtx[2].nor = D3DXVECTOR3(0, 1, 0);
	pVtx[3].nor = D3DXVECTOR3(0, 1, 0);

	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    m_pVtxBuff->Unlock();
	CScene3d::Init();
    return S_OK;
}
void CPolygon3d::Uninit(void)
{
    if(m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
	CScene3d::Uninit();
}
void CPolygon3d::Update(void)
{

}
void CPolygon3d::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxWorld;
	D3DMATERIAL9 matDef;
    D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 pos = GetPosition();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);

	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);				//両方bad
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);				//ステンシルだけok
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);			//両方ok

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//単位行列生成
    D3DXMatrixIdentity(&mtxWorld);
	//回転
    D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y), D3DXToRadian(rot.x), D3DXToRadian(rot.z));
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	//移動
    D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	//行列適用
    pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	ZeroMemory(&matDef, sizeof(matDef));
	matDef.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	pDevice->SetMaterial(&matDef);
    pDevice->SetTexture(0, m_pTexture);
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
    pDevice->SetFVF(FVF_VERTEX_3D);
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pDevice->SetTexture(0, NULL);
	ZeroMemory(&matDef, sizeof(matDef));
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

}

CPolygon3d* CPolygon3d::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
    CPolygon3d* pPolygon3D = NULL;
    pPolygon3D             = new CPolygon3d;
    pPolygon3D->SetPosition(pos);
    pPolygon3D->SetRotation(rot);
    pPolygon3D->SetScale(size);
    pPolygon3D->Init();
    return pPolygon3D;
}

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//テクスチャの割当
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
void CPolygon3d::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
