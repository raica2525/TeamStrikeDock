//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	glyph.cpp
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "glyph.h"
#include "manager.h"
#include "renderer.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�ÓI�����o�ϐ��錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
LPD3DXFONT CGlyph::m_pFont = NULL;

CGlyph::CGlyph()
{
	m_pChar = NULL;
}
CGlyph::~CGlyph()
{

}

HRESULT CGlyph::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	GLYPHMETRICS GM;
	int fontsize = 512;
	LOGFONT lf = { fontsize, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE,"�l�r ����"}; //�t�H���g�̎w��
	HFONT hFont = CreateFontIndirect(&lf); //�_���t�H���g����
	MAT2 Mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	HDC hdc = GetDC(FindWindow(CLASS_NAME, NULL));


	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
	UINT code = 0;
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);
	//�������镶�����}���`�o�C�g�������ۂ��Ŕ��ʂ���
	if (IsDBCSLeadByte(*m_pChar))
	{
		code = (BYTE)m_pChar[0] << 8 | (BYTE)m_pChar[1];
	}
	else
	{
		code = m_pChar[0];
	}

	int nSize = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	BYTE *ptr = new BYTE[nSize];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, nSize, ptr, &Mat);

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	D3DXCreateTexture(
		pDevice,
		GM.gmCellIncX,
		TM.tmHeight,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture);
	int iOfs_x = GM.gmptGlyphOrigin.x;
	int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	int iBmp_h = GM.gmBlackBoxY;
	int Level = 17;
	int x, y;
	DWORD Alpha, Color;
	D3DLOCKED_RECT lockrect;
	m_pTexture->LockRect(0, &lockrect, NULL, D3DLOCK_DISCARD);

	FillMemory(lockrect.pBits, lockrect.Pitch * TM.tmHeight, 0);
	for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
	{
		for (x = iOfs_x; x<iOfs_x + GM.gmBlackBoxX; x++)
		{
			Alpha = (255 * ptr[x - iOfs_x + iBmp_w*(y - iOfs_y)]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy((BYTE*)lockrect.pBits + lockrect.Pitch*y + 4 * x, &Color, sizeof(DWORD));
		}
	}
	// �T�[�t�F�C�X�A�����b�N
	m_pTexture->UnlockRect(0);

	CScene2d::Init();
	BindTexture(m_pTexture);

	return S_OK;
}
void CGlyph::Uninit(void)
{

}
void CGlyph::Update(void)
{

}
void CGlyph::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �A���t�@�e�X�g�̗L����
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// �s�����ɂ���l�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);
	CScene2d::Draw();
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

CGlyph* CGlyph::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TCHAR* chara)
{
	CGlyph* pGlyph = new CGlyph;
	pGlyph->SetPos(pos);
	pGlyph->SetSize(size);
	pGlyph->m_pChar = chara;
	pGlyph->Init();
	return pGlyph;
}

void CGlyph::Load(void)
{
	AddFontResourceEx("Data\\Fonts\\NikkyouSans-mLKax.ttf", FR_PRIVATE, 0);
}
void CGlyph::Unload(void)
{
	RemoveFontResourceEx("Data\\Fonts\\NikkyouSans-mLKax.ttf", FR_PRIVATE, 0);
}
