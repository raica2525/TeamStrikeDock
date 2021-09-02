//===============================================
//
// �l�X�ȏ������܂Ƃ߂��t�@�C�� (library.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "library.h"
#include "manager.h"
#include "camera.h"

//===========================================
// ��ʊO���ǂ���(�񎟌�)
// Author : �㓡�T�V��
//===========================================
bool IsOutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize)
{
    // �ϐ��錾
    bool bOutScreen = false;    // ��ʊO���ǂ���

                                // ��ʂ̒[����o�Ă��邩�ǂ���
    if (pPos->x < -(pSize->x / 2) ||
        pPos->x > SCREEN_WIDTH + pSize->x / 2 ||
        pPos->y < -(pSize->y / 2) ||
        pPos->y > SCREEN_HEIGHT + pSize->y / 2)
    {
        // ��ʊO�̃t���O��true��
        bOutScreen = true;
    }

    return bOutScreen;
}

//===========================================
// �Q�[���O���ǂ���(�񎟌�)
// Author : �㓡�T�V��
//===========================================
bool IsOutGame2D(const D3DXVECTOR3 * pPos, const D3DXVECTOR3 * pSize)
{
    // �ϐ��錾
    bool bOutGame = false;    // �Q�[���O���ǂ���

                              // ��ʂ̒[����2�{�̂Ƃ��납��o�Ă��邩�ǂ���
    if (pPos->x < -SCREEN_WIDTH - (pSize->x / 2) ||
        pPos->x >(SCREEN_WIDTH * 2) + pSize->x / 2 ||
        pPos->y < -SCREEN_HEIGHT - (pSize->y / 2) ||
        pPos->y >(SCREEN_HEIGHT * 2) + pSize->y / 2)
    {
        // �Q�[���O�̃t���O��true��
        bOutGame = true;
    }

    return bOutGame;
}

//===========================================
// ��ʓ��ɂƂǂ܂点��(�񎟌�)
// Author : �㓡�T�V��
//===========================================
bool IsSteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pSize)
{
    // �ϐ��錾
    bool bScreenEdge = false;    // ��ʒ[���ǂ���

                                 // ��ʂ̍�
    if (pPos->x < pSize->x / 2)
    {
        // �Ƃǂ܂点��
        pPos->x = pSize->x / 2;

        // ��ʒ[�̃t���O��true��
        bScreenEdge = true;
    }

    // ��ʂ̉E
    if (pPos->x > SCREEN_WIDTH - pSize->x / 2)
    {
        // �Ƃǂ܂点��
        pPos->x = SCREEN_WIDTH - pSize->x / 2;

        // ��ʒ[�̃t���O��true��
        bScreenEdge = true;
    }

    // ��ʂ̏�
    if (pPos->y < pSize->y / 2)
    {
        // �Ƃǂ܂点��
        pPos->y = pSize->y / 2;

        // ��ʒ[�̃t���O��true��
        bScreenEdge = true;
    }

    // ��ʂ̉�
    if (pPos->y > SCREEN_HEIGHT - pSize->y / 2)
    {
        // �Ƃǂ܂点��
        pPos->y = SCREEN_HEIGHT - pSize->y / 2;

        // ��ʒ[�̃t���O��true��
        bScreenEdge = true;
    }

    return bScreenEdge;
}

//===========================================
// ��`�̓����蔻��2D
// Author : �㓡�T�V��
//===========================================
bool IsCollisionRectangle2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // �ϐ��錾
    bool bHit = false;    // �����������ǂ���

                          // ��̂��̂̓����蔻����v�Z
    if (pPos1->x - pSize1->x / 2 <= pPos2->x + pSize2->x / 2 &&
        pPos1->x + pSize1->x / 2 >= pPos2->x - pSize2->x / 2 &&
        pPos1->y - pSize1->y / 2 <= pPos2->y + pSize2->y / 2 &&
        pPos1->y + pSize1->y / 2 >= pPos2->y - pSize2->y / 2)
    {
        // ���������t���O��true��
        bHit = true;
    }

    return bHit;
}

//===========================================
// ��`�̓����蔻��3D
// Author : �㓡�T�V��
//===========================================
bool IsCollisionRectangle3D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // �ϐ��錾
    bool bHit = false;  // �����������ǂ���

    D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;    //�Ԃ��鑤�̍ő�l
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, 0.0f, -pSize1->z / 2) + *pPos1;       //�Ԃ��鑤�̍ŏ��l
    D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;    //�Ԃ����鑤�̍ő�l
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;       //�Ԃ����鑤�̍ŏ��l

                                                                                            // �����蔻����v�Z
    if (box1Max.y > box2Min.y&&
        box1Min.y < box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        bHit = true;
    }

    return bHit;
}

//===========================================
// �u���b�N�̓����蔻��3D
// Author : �㓡�T�V��
//===========================================
BLOCK_HIT_INFO IsBlockCollision3D(D3DXVECTOR3 * pPos1, D3DXVECTOR3 * pPos1Old, const D3DXVECTOR3 * pPos2, const D3DXVECTOR3 * pSize1, const D3DXVECTOR3 * pSize2)
{
    // �u���b�N�ɓ����������
    BLOCK_HIT_INFO blockHitInfo;
    blockHitInfo.hitSurface = HIT_SURFACE_NONE;
    blockHitInfo.pos = *pPos1;

    D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;      //�Ԃ��鑤�̍ő�l
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, 0.0f, -pSize1->z / 2) + *pPos1;         //�Ԃ��鑤�̍ŏ��l
    D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;      //�Ԃ����鑤�̍ő�l
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;         //�Ԃ����鑤�̍ŏ��l

                                                                                              // �����蔻����v�Z
    if (box1Max.y > box2Min.y&&
        box1Min.y <= box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        if (box1Max.y > box2Min.y&&
            pPos1Old->y + pSize1->y / 2 < box2Min.y)
        {// Y���̉�
            blockHitInfo.hitSurface = HIT_SURFACE_BOTTOM;
            blockHitInfo.pos.y = box2Min.y - pSize1->y;
        }
        if (box1Min.y <= box2Max.y&&
            pPos1Old->y >= box2Max.y)
        {// Y���̏�
            blockHitInfo.hitSurface = HIT_SURFACE_TOP;
            blockHitInfo.pos.y = box2Max.y;
        }
        if (box1Max.x > box2Min.x&&
            pPos1Old->x + pSize1->x / 2 <= box2Min.x)
        {// X���̍�
            blockHitInfo.hitSurface = HIT_SURFACE_LEFT;
            blockHitInfo.pos.x = box2Min.x - pSize1->x / 2;
        }
        if (box1Min.x < box2Max.x&&
            pPos1Old->x - pSize1->x / 2 >= box2Max.x)
        {// X���̉E
            blockHitInfo.hitSurface = HIT_SURFACE_RIGHT;
            blockHitInfo.pos.x = box2Max.x + pSize1->x / 2;
        }
        if (box1Max.z > box2Min.z&&
            pPos1Old->z + pSize1->z / 2 <= box2Min.z)
        {// Z���̎�O
            blockHitInfo.hitSurface = HIT_SURFACE_FRONT;
            blockHitInfo.pos.z = box2Min.z - pSize1->z / 2;
        }
        if (box1Min.z < box2Max.z&&
            pPos1Old->z - pSize1->z / 2 >= box2Max.z)
        {// Z���̉�
            blockHitInfo.hitSurface = HIT_SURFACE_BACK;
            blockHitInfo.pos.z = box2Max.z + pSize1->z / 2;
        }
    }

    return blockHitInfo;
}

//=============================================================================
// �~�̓����蔻��2D
// Author : �㓡�T�V��
//=============================================================================
bool IsCollisionCircle2D(const D3DXVECTOR3 pos1, const float fSize1, const D3DXVECTOR3 pos2, const float fSize2)
{
    // ���������߂�
    float fDistance = sqrtf(powf(pos1.x - pos2.x, 2.0f) + powf(pos1.y - pos2.y, 2.0f));

    // 2�̔��a�𑫂������̂����߂�
    float fSumRadius = (fSize1 + fSize2) / 2;

    return (fDistance <= fSumRadius);
}

//===========================================
// �͈͓��ŁA�����_���Ȑ����𓾂�i+��-���܂����̂ɂ͔�Ή��j
// Author : �㓡�T�V��
//===========================================
int GetRandNum(const int nMax, const int nMin)
{
    int nMaxNum = nMax;
    int nMinNum = nMin;
    bool bReverse = false;

    // �ő傪�}�C�i�X�Ȃ�
    if (nMax < 0)
    {
        nMaxNum = (int)fabs(nMin);
        nMinNum = (int)fabs(nMax);
        bReverse = true;
    }

    int nRandNum = rand() % (nMaxNum - nMinNum + 1) + nMinNum;

    if (bReverse)
    {
        nRandNum *= -1;
    }

    return nRandNum;
}

//===========================================
// �Ώۂւ̊p�x�𓾂�2D
// Author : �㓡�T�V��
//===========================================
float GetAngleToTarget2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos)
{
    // �ϐ��錾
    float fAngle = 0.0f;                    // �Ԃ��p�x

    // �p�x�����߂�
    fAngle = atan2((targetPos.y - myPos.y), (targetPos.x - myPos.x));

    return fAngle;
}

//===========================================
// �Ώۂւ̊p�x�𓾂�2.5D
// Author : �㓡�T�V��
//===========================================
float GetAngleToTarget3D2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos)
{
    // �ϐ��錾
    float fAngle = 0.0f;                    // �Ԃ��p�x

    // �p�x�����߂�
    fAngle = atan2((targetPos.x - myPos.x), (targetPos.y - myPos.y));

    return fAngle;
}

//===========================================
// �Ώۂւ̋����𓾂�2D
// Author : �㓡�T�V��
//===========================================
float GetDistance2D(const D3DXVECTOR3 myPos, const D3DXVECTOR3 targetPos)
{
    float fDistance = 0.0f;

    // ���������߂�
    fDistance = sqrtf(
        powf((myPos.x - targetPos.x), 2) +
        powf((myPos.y - targetPos.y), 2));

    return fDistance;
}

//===========================================
// ���@�藝��]���W�v�Z�֐�
//-------------------------------------------------
// ��]����_���W p1
// ��]�������W   cp
// ��]�p�x       deg
//===========================================
D3DXVECTOR3 caclRotate(const D3DXVECTOR3 p1, const D3DXVECTOR3 cp, float deg) 
{
    float  cx, cy;

    cx = (cp.x - p1.x)*cos(D3DXToRadian(deg)) - (cp.y - p1.y)*sin(D3DXToRadian(deg)) + cp.x;
    cy = (cp.x - p1.x)*sin(D3DXToRadian(deg)) + (cp.y - p1.y)*cos(D3DXToRadian(deg)) + cp.y;

    return D3DXVECTOR3(cx, cy, 0.0f);
}

//===========================================
// �Փ˔���֐��i�O�p�j
//-------------------------------------------
// ���� r1-r2�ƁA��p1-p2���N���X���邩���肷��
// �N���X����ꍇ�� true. ���Ȃ��ꍇ�� false.
// http://mslabo.sakura.ne.jp/WordPress/make/processing%E3%80%80%E9%80%86%E5%BC%95%E3%81%8D%E3%83%AA%E3%83%95%E3%82%A1%E3%83%AC%E3%83%B3%E3%82%B9/%E5%A4%9A%E8%A7%92%E5%BD%A2%E5%90%8C%E5%A3%AB%E3%81%AE%E5%BD%93%E3%81%9F%E3%82%8A%E5%88%A4%E5%AE%9A%E3%82%92%E8%A1%8C%E3%81%86%E3%81%AB%E3%81%AF/
//===========================================
bool IsCollisionSide2D(const D3DXVECTOR3 r1, const D3DXVECTOR3 r2, const D3DXVECTOR3 p1, const D3DXVECTOR3 p2)
{
    float t1, t2;

    // 2�_r1-r2��ʂ钼���ɑ΂��Ap1�Ap2���ǂ̈ʒu�ɂ��邩�v�Z����
    t1 = (r1.x - r2.x)*(p1.y - r1.y) + (r1.y - r2.y)*(r1.x - p1.x);
    t2 = (r1.x - r2.x)*(p2.y - r1.y) + (r1.y - r2.y)*(r1.x - p2.x);

    // ���ꂼ��̐������قȂ�i�ς����ɂȂ�j�Ȃ�N���X���Ă���i�_���l������ƁA�S�����������ŕ��񂾎���true��ʂ�̂ō폜�j
    if (t1*t2 < 0) 
    {
        return(true); // �N���X���Ă���
    }

    return(false); // �N���X���Ă��Ȃ�
}

//===========================================
// ��]�����`�ւ̓����蔻��i��]���Ȃ����̂���A��]�����`�ցj
// Author : �㓡�T�V��
//===========================================
bool IsCollisionToRotationRect(const D3DXVECTOR3 myCenter, const D3DXVECTOR3 mySize, const D3DXVECTOR3 targetPos1, const D3DXVECTOR3 targetPos2, const D3DXVECTOR3 targetPos3, const D3DXVECTOR3 targetPos4)
{
    // �������̎l�p�̈ʒu���o��(����A�����A�E���A�E��̏�)
    D3DXVECTOR3 myPos1 = myCenter + D3DXVECTOR3(-mySize.x / 2, mySize.y, 0.0f);
    D3DXVECTOR3 myPos2 = myCenter + D3DXVECTOR3(-mySize.x / 2, 0.0f, 0.0f);
    D3DXVECTOR3 myPos3 = myCenter + D3DXVECTOR3(mySize.x / 2, 0.0f, 0.0f);
    D3DXVECTOR3 myPos4 = myCenter + D3DXVECTOR3(mySize.x / 2, mySize.y, 0.0f);

    // �Փ˔�����s��
    // �����l�p�� [0]-[1] �ƑΏێl�p�� [0]-[1]�A[1]-[2]�A[2]-[3]�A[3]-[0]
    // �����l�p�� [1]-[2] �ƑΏێl�p�� [0]-[1]�A[1]-[2]�A[2]-[3]�A[3]-[0]
    // �����l�p�� [2]-[3] �ƑΏێl�p�� [0]-[1]�A[1]-[2]�A[2]-[3]�A[3]-[0]
    // �����l�p�� [3]-[0] �ƑΏێl�p�� [0]-[1]�A[1]-[2]�A[2]-[3]�A[3]-[0] 
    // ���N���X���肷��

    // �ϐ��錾
    bool bCross1 = false;
    bool bCross2 = false;

    // �v�Z���₷���悤�Ɉ�x�z��Ɋm��
    D3DXVECTOR3 myRect[4] = { myPos1 ,myPos2 ,myPos3 ,myPos4 };
    D3DXVECTOR3 targetRect[4] = { targetPos1 ,targetPos2 ,targetPos3 ,targetPos4 };

    // i �͔��l�p��4�ӂ̃C���f�b�N�X
    for (int i = 0; i < 4; i++) 
    {
        // ���W�z��3-4�̔�r�Ȃ�A3-0�̔�r�ɒu��������
        int toi;
        toi = i + 1;
        if (toi > 3) toi = 0;

        // j �͐l�p��4�ӂ̃C���f�b�N�X
        for (int j = 0; j < 4; j++) 
        {
            // ���W�z��3-4�̔�r�Ȃ�A3-0�̔�r�ɒu��������
            int toj;
            toj = j + 1;
            if (toj > 3) toj = 0;

            // �N���X����
            bCross1 = IsCollisionSide2D(myRect[i], myRect[toi],
                targetRect[j], targetRect[toj]);

            bCross2 = IsCollisionSide2D(targetRect[j], targetRect[toj],
                myRect[i], myRect[toi]);

            // �ǂ�����N���X���Ă���Ȃ瓖�����Ă���
            if (bCross1 && bCross2)
            {
                return true;
            }
        }
    }

    return false;
}

//===========================================
// �X�N���[�����W�ւ̕ϊ�
// Author : �㓡�T�V��
//===========================================
D3DXVECTOR3 ConvertScreenPos(const D3DXVECTOR3 convertPos)
{
    // �J�������擾
    CCamera* pCamera = CManager::GetCamera();

    // 4�����ł̈ʒu���`
    D3DXVECTOR4* pPos = &D3DXVECTOR4(convertPos.x, convertPos.y, convertPos.z, 1.0f);

    // �r���[�ϊ�
    D3DXVECTOR4 viewPosition = *D3DXVec4Transform(pPos, pPos, &pCamera->GetViewMtx());

    // �v���W�F�N�V�����ϊ�
    D3DXVECTOR4 projectionPosition = *D3DXVec4Transform(&viewPosition, &viewPosition, &pCamera->GetProjectionMtx());

    // �������W���l��
    projectionPosition /= projectionPosition.w;

    // �X�N���[���ϊ�
    D3DXVECTOR3 screenPos = D3DXVECTOR3(
        (1.0f + projectionPosition.x) / 2.0f * SCREEN_WIDTH,
        (1.0f - projectionPosition.y) / 2.0f * SCREEN_HEIGHT,
        0.0f);

    return screenPos;
}