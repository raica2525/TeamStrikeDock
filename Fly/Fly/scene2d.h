//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	scene2d.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

#ifndef _SCENE2D_H_
#define _SCENE2D_H_
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "main.h"
#include "scene.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�N���X��`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CScene2d : public CScene
{
public:

	CScene2d(int nPriority = 1);		//�R���X�g���N�^
	virtual ~CScene2d();	//�f�X�g���N�^

	 virtual HRESULT Init(void);	//����������
	 virtual void Uninit(void);		//�I������
	 virtual void Update(void);		//�X�V����
	 virtual void Draw(void);		//�`�揈��

	 void SetPos(D3DXVECTOR3 pos);			//�ʒu�ݒ菈��
	 D3DXVECTOR3 GetPos(void);

	 void SetMove(D3DXVECTOR3 move);			//�ʒu�ݒ菈��
	 D3DXVECTOR3 GetMove(void);

	 void SetSize(D3DXVECTOR3 size);
	 void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	 void SetTexPos(D3DXVECTOR2* pTexPos);
	 static CScene2d* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nPriority);	//�|���S����������

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_move;					//�|���S���̈ړ���
	D3DXVECTOR3				m_size;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^

};

#endif