//=============================================================================
//
// ���C������ [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS
#define DIRECTINPUT_VERSION (0x0800)

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************

#include <windows.h>
#include <dinput.h>
#include "d3dx9.h"
#include <time.h>

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")//�V�X�e�������擾
#pragma comment(lib, "dinput8.lib")
//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define SCREEN_WIDTH (1920) //��ʂ̉𑜓x�iX����)
#define SCREEN_HEIGHT (1080)	//��ʂ̉𑜓x�iY�����j
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define NUM_POLYGON		(2)
#define NUM_VERTEX		(4)
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define ZERO_VEC		D3DXVECTOR3(0.0f,0.0f,0.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

typedef struct 
{
	D3DXVECTOR3 pos;//���_��
	float rhw;//���W�ϊ��p�W��(1.0f�Œ�)
	D3DCOLOR col;//���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
} VERTEX_3D;
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�O���錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CManager;
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�v���g�^�C�v�錾
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

int GetFPS(void);

#endif
