#pragma once
//====================================================================
//
// �N���b�s���O�}�X�N����(cliping_musk.h)
// Author : �r�c�I��
//
//====================================================================
#ifndef _CLIPING_MUSK_H_
#define _CLIPING_MUSK_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "library.h"

//================================================
// �}�N����`
//================================================
#define MAX_REFERENCE_VALUE 0xff

//================================================
// �N���X�錾
//================================================
class CClipingMusk
{
public:

	CClipingMusk();
	~CClipingMusk();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(const int nNumTexture);																	// �e�N�X�`�������蓖�Ă�
	void SetVertex(void);                                                                                       // ���_���W��ݒ�
	void SetRotVertex(float fAngle);                                                                            // ��]���钸�_���W��ݒ�
	void SetVisualVertex(D3DXVECTOR3 posVisual, D3DXVECTOR3 sizeVisual);                                        // ��������̒��_���W��ݒ�
	static CClipingMusk* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumTexture);
	static CClipingMusk*Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	//========================================================
	// �Z�b�^�[
	//========================================================
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosTo2D(D3DXVECTOR3 pos3D) { m_pos = ConvertScreenPos(pos3D); }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetAlphaTestBorder(int nValue) { m_nAlphaTestBorder = nValue; }

	//========================================================
	// �Q�b�^�[
	//========================================================
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	DWORD GetReferenceValue(void) { return m_dwReferenceValue; }
private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		                // ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;	                        // �ʒu
	D3DXVECTOR3             m_size;                         // �傫��

	static bool m_abUsingReferenceValue[MAX_REFERENCE_VALUE];	// �Q�ƒl���g�p���Ă��邩�ǂ���
	DWORD m_dwReferenceValue;									// �Q�ƒl�ۑ��p�ϐ�
	int m_nAlphaTestBorder;										// �A���t�@�e�X�g�̋��E�l
};

#endif