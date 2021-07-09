//=============================================================================
//
// ���C������ [main.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include <WinSock2.h>
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xaudio2.h>
#include <thread>

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")    // �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")   // �g�����C�u����
#pragma comment(lib, "dxguid.lib")  // �R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib,"dinput8.lib")  // ���͏����ɕK�v
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �E�C���h�E�̈ʒu
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)

// ��ʂ̕�
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

// ���_�ƃ|���S��
#define NUM_VERTEX	(4)
#define NUM_POLYGON	(2)

//���_�t�H�[�}�b�g		(���_���W|���_�J���[|�e�N�X�`�����W)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D   (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// �f�t�H���g�ݒ�
#define DEFAULT_COLOR (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define DEFAULT_VECTOR (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define DEFAULT_SCALE (D3DXVECTOR3(1.0f,1.0f,1.0f))
#define DEFAULT_NOR (D3DXVECTOR3(0.0f,1.0f,0.0f))

// �v���C���[�̔ԍ�
#define MAX_PLAYER 4     // �Ȃ��R���g���[���[�̍ő吔
#define PLAYER_1 (0)     // �v���C���[�̃R���g���[���[�ԍ� (1P)
#define PLAYER_2 (1)     // �v���C���[�̃R���g���[���[�ԍ� (2P)
#define PLAYER_3 (2)     // �v���C���[�̃R���g���[���[�ԍ� (3P)
#define PLAYER_4 (3)     // �v���C���[�̃R���g���[���[�ԍ� (4P)

// �r�b�g���Z
#define BITON(flag,bit) (flag)=(flag)|(bit)
#define BITOFF(flag,bit) (flag)=(flag)&(~(bit))
#define IS_BITON(flag,bit) ((flag)&(bit))
#define IS_BITOFF(flag,bit) ((~(flag)&(bit)))

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

typedef struct
{
    D3DXVECTOR3 pos;
    float rhw;
    D3DCOLOR col;
    D3DXVECTOR2 tex;
}VERTEX_2D;

typedef struct
{
    D3DXVECTOR3 pos;
    D3DXVECTOR3 nor;
    D3DCOLOR    col;
    D3DXVECTOR2 tex;
}VERTEX_3D;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#ifdef _DEBUG
int GetFPS(void);    // FPS�擾
#endif

#endif