//====================================================================
//
// �V�[����̃I�u�W�F�N�g���� (scene.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �}�N����`
//================================================

//================================================
// �N���X�錾
//================================================

// �I�u�W�F�N�g�̃N���X
class CScene
{
public:
    // �I�u�W�F�N�g�̎��(�`�揇)
    typedef enum
    {
        OBJTYPE_NONE,               // �Ȃ�
        OBJTYPE_BG,                 // �w�i
        OBJTYPE_EFFECT3D_Z_NONE,    // Z�o�b�t�@�G�t�F�N�g3D
        OBJTYPE_BALL,               // �{�[��
        OBJTYPE_ENEMY,              // �G�l�~�[
        OBJTYPE_PLAYER,             // �v���C���[
        OBJTYPE_EFFECT3D_STENCIL,   // �X�e���V���G�t�F�N�g
        OBJTYPE_BLOCK,              // �u���b�N
        OBJTYPE_MODEL_EFFECT,       // ���f���G�t�F�N�g
        OBJTYPE_ITEM,               // �A�C�e��
        OBJTYPE_EFFECT3D,           // �G�t�F�N�g3D
        OBJTYPE_UI,                 // UI
        OBJTYPE_EFFECT2D,           // �G�t�F�N�g2D
        OBJTYPE_MAX                 // �I�u�W�F�N�g�̎�ނ̍ő吔
    }OBJTYPE;

    CScene();
    CScene(OBJTYPE objType);    // �C�j�V�����C�U�ŃI�u�W�F�N�g�^�C�v��R�Â�
    virtual ~CScene();  // �e�N���X�̃f�X�g���N�^��virtual�����邱�ƂŁA���������[�N��}����
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;
    virtual void Uninit(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    static void UpdateAll(void);                                                        // �S�čX�V
    static void DrawAll(void);                                                          // �S�ĕ`��
    static void QuickSort(CScene*apScene[], int left, int right);                       // �N�C�b�N�\�[�g
    static void ReleaseAll(void);                                                       // �S�ĊJ��
    static void ReleaseStage(void);                                                     // �S�Ă̔w�i�A�G�A�u���b�N�A�A�C�e�����J��
    static void ReleaseUI(void);                                                        // �S�Ă�UI���폜

    static int GetNumObjAll(void) { return m_nNumObjectAll; }                           // �V�[����̑S�ẴI�u�W�F�N�g��
    static int GetNumAll(OBJTYPE objtype) { return m_nNumAll[objtype]; }                // ���̎�ނ̑S�̂̐����擾
    static CScene*GetTopScene(OBJTYPE objtype) { return m_pTop[objtype]; }              // ���̎�ނ̐擪�̃I�u�W�F�N�g���擾
    CScene* GetNextScene(void) { return m_pNext; }                                      // ���̃I�u�W�F�N�g���擾

    /*============================================================================================================================
    // �I�u�W�F�N�g�̃^�C�v���ƂŃR���X�g���N�^�Ŏ��̃V�[�������L�����Ă��邽�߁A��΂ɃI�u�W�F�N�g�^�C�v�����������Ă͂����Ȃ�
    //void SetObjType(OBJTYPE objtype) { m_objType = objtype; }
    // �g�b�v�Ɏ�ނ����Ď擾�ł���̂ŁA�Q�b�^�[���g���@��Ȃ��Ȃ���
    //OBJTYPE GetObjType(void) { return m_objType; }
    ============================================================================================================================*/

protected:
    void Release(void);                 // �P�̂��J��

private:
    static CScene *m_pTop[OBJTYPE_MAX];  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
    static CScene *m_pCur[OBJTYPE_MAX];  // ���݂ɂ�����ŐV�̃I�u�W�F�N�g�ւ̃|�C���^
    CScene *m_pPrev;                     // �O�̃I�u�W�F�N�g�ւ̃|�C���^
    CScene *m_pNext;                     // ���̃I�u�W�F�N�g�ւ̃|�C���^

    static int m_nNumObjectAll;          // �V�[����̂��ׂẴI�u�W�F�N�g�̐�
    static int m_nNumAll[OBJTYPE_MAX];   // �V�[����̎�ނ��Ƃ̃I�u�W�F�N�g�̍ő吔
    OBJTYPE m_objType;                   // �I�u�W�F�N�g�̎��

    bool m_bUse;                         // �g�p���邩�ǂ���
};

#endif