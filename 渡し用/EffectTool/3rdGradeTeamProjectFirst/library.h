//===============================================
//
// �l�X�ȏ������܂Ƃ߂��t�@�C�� [library.h]
// Author : �㓡�T�V��
//
//===============================================
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

//========================
// �C���N���[�h�t�@�C��
//========================
#include "main.h"

//========================
// �}�N����`
//========================

// �Q�[�����̈ړ��͈�
#define GAME_LIMIT_WIDTH 1000.0f    // 2�{�ɂȂ�
#define GAME_LIMIT_HEIGHT 1000.0f   // ���ł킩��₷���悤�ɖ{�Ƃ�2�{�̃T�C�Y�̃X�e�[�W

//========================
// �񋓌^��`
//========================
// �u���b�N�ɓ����������̖�
typedef enum
{
    HIT_SURFACE_NONE = 0,     // �������Ă��Ȃ�
    HIT_SURFACE_RIGHT,        // �E
    HIT_SURFACE_LEFT,         // ��
    HIT_SURFACE_FRONT,        // ��O
    HIT_SURFACE_BACK,         // ��
    HIT_SURFACE_TOP,          // ���
    HIT_SURFACE_BOTTOM,       // ��
}HIT_SURFACE;

//========================
// �\���̒�`
//========================
// �u���b�N�ɓ����������̏��
typedef struct
{
    HIT_SURFACE hitSurface;     // ����������
    D3DXVECTOR3 pos;            // �ʒu
}BLOCK_HIT_INFO;

//===========================================
// �v���g�^�C�v�錾
//===========================================
bool IsOutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize);
bool IsOutGame2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize);
bool IsSteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pSize);
bool IsCollisionRectangle2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
bool IsCollisionRectangle3D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
BLOCK_HIT_INFO IsBlockCollision3D(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos1Old, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
bool IsCollisionCircle2D(const D3DXVECTOR3 pos1, const float fSize1, const D3DXVECTOR3 pos2, const float fSize2);
int GetRandNum(const int nMax, const int nMin);
float GetAngleToTarget2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos);
float GetAngleToTarget3D2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos);
float GetDistance2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos);
D3DXVECTOR3 caclRotate(const D3DXVECTOR3 p1, const D3DXVECTOR3 cp, float deg);
bool IsCollisionSide2D(const D3DXVECTOR3 r1, const D3DXVECTOR3 r2, const D3DXVECTOR3 p1, const D3DXVECTOR3 p2);
bool IsCollisionToRotationRect(const D3DXVECTOR3 myCenterPos, const D3DXVECTOR3 mySize, const D3DXVECTOR3 targetPos1, const D3DXVECTOR3 targetPos2, const D3DXVECTOR3 targetPos3, const D3DXVECTOR3 targetPos4);

#endif