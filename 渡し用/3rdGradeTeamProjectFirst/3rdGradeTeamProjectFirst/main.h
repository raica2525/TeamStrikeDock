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

// �E�C���h�E�̃N���X���ƃL���v�V������
#define CLASS_NAME		"AppClass"
#define WINDOW_NAME		"BLAZE_SMASH"

// �E�C���h�E�̈ʒu
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)

// ��ʂ̕�
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

// ���_�ƃ|���S��
#define NUM_VERTEX	(4)
#define NUM_POLYGON	(2)

// �e�N�X�`���u�����h�̍ő吔    //�r�c�ǉ�
#define MAX_BREND_TEXTURE (4)

// ���_�t�H�[�}�b�g		(���_���W|���_�J���[|�e�N�X�`�����W (* �e�N�X�`���u�����h�̍ő吔))  //�r�c�ǉ�
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 * MAX_BREND_TEXTURE)
#define FVF_VERTEX_3D   (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// �f�t�H���g�ݒ�
#define DEFAULT_COLOR (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define DEFAULT_COLOR_NONE (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define DEFAULT_COLOR_NONE_ALPHA (D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))
#define DEFAULT_VECTOR (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define DEFAULT_SCALE (D3DXVECTOR3(1.0f,1.0f,1.0f))
#define DEFAULT_NOR (D3DXVECTOR3(0.0f,1.0f,0.0f))
#define DEFAULT_ALPHATEST_BORDER_2D 50
#define DEFAULT_ALPHATEST_BORDER_3D 100

// �v���C���[�̔ԍ�
#define MAX_PLAYER 4     // �Ȃ��R���g���[���[�̍ő吔
#define PLAYER_1 (0)     // �v���C���[�̃R���g���[���[�ԍ� (1P)
#define PLAYER_2 (1)     // �v���C���[�̃R���g���[���[�ԍ� (2P)
#define PLAYER_3 (2)     // �v���C���[�̃R���g���[���[�ԍ� (3P)
#define PLAYER_4 (3)     // �v���C���[�̃R���g���[���[�ԍ� (4P)

// �e�v���C���[�̐F
#define PLAYER_COLOR_NONE D3DXCOLOR(0.584f, 0.333f, 0.96f, 1.0f)
#define PLAYER_COLOR_1 D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define PLAYER_COLOR_2 D3DXCOLOR(0.333f, 0.682f, 0.949f, 1.0f)
#define PLAYER_COLOR_3 D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define PLAYER_COLOR_4 D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)

// �r�b�g���Z
#define BITON(flag,bit) (flag)=(flag)|(bit)
#define BITOFF(flag,bit) (flag)=(flag)&(~(bit))
#define IS_BITON(flag,bit) ((flag)&(bit))
#define IS_BITOFF(flag,bit) ((~(flag)&(bit)))

// ���j���[����
#define MENU_SELECT_COOL_TIME 12    // �I���̃N�[���^�C��

// 0�ŏ������������Ȃ��Ƃ��p
#define NOT_EXIST -1

// �e�L�X�g����
#define TEXT_EXIST_COLOR D3DCOLOR_RGBA(255, 255, 255, 255)
#define TEXT_NOT_EXIST_COLOR D3DCOLOR_RGBA(255, 255, 255, 0)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

typedef struct
{
    D3DXVECTOR3 pos;
    float rhw;
    D3DCOLOR col;
    D3DXVECTOR2 tex[MAX_BREND_TEXTURE];
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