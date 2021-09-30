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
        OBJTYPE_NONE_DRAW,          // �`�悵�Ȃ�
        OBJTYPE_BG,                 // �w�i
        OBJTYPE_EFFECT3D_Z_NONE,    // �G�t�F�N�g3D�iZ�o�b�t�@�𖳎��j
        OBJTYPE_BALL,               // �{�[��
        //OBJTYPE_ENEMY,            // �G�l�~�[�i����͂��Ȃ��A���̑���CPU������j
        OBJTYPE_PLAYER,             // �v���C���[
        //OBJTYPE_BLOCK,            // �u���b�N�i����͎g���Ă��Ȃ��j
        OBJTYPE_MODEL_EFFECT,       // ���f���G�t�F�N�g
        //OBJTYPE_ITEM,             // �A�C�e���i����͎g���Ă��Ȃ��j
        OBJTYPE_EFFECT3D,           // �G�t�F�N�g3D
        OBJTYPE_EFFECT2D_BACK_UI,   // �G�t�F�N�g2D�iUI�����j
        OBJTYPE_UI_BACK_TEXT,       // UI�i�e�L�X�g�����j
        OBJTYPE_TEXT,               // �e�L�X�g
        OBJTYPE_UI_FRONT_TEXT,      // UI�i�e�L�X�g����O�j
        OBJTYPE_EFFECT2D_FRONT_UI,  // �G�t�F�N�g2D�iUI����O�j
        OBJTYPE_WAVE,               // ��ʏ�ɏo��E�F�[�u
        OBJTYPE_MAX                 // �I�u�W�F�N�g�̎�ނ̍ő吔
    }OBJTYPE;

    CScene(OBJTYPE objType);    // �C�j�V�����C�U�ŃI�u�W�F�N�g�^�C�v��R�Â�
    virtual ~CScene();  // �e�N���X�̃f�X�g���N�^��virtual�����邱�ƂŁA���������[�N��}����
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;
    virtual void Uninit(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    static void UpdateAll(void);                                                        // �S�čX�V
    static void SpUpdate(void);                                                         // �K�E�Z�g�p���̃A�b�v�f�[�g
    static void DrawExceptWave(void);                                                   // �g�ȊO��`��
    static void DrawExceptWaveAndUI(void);                                              // �g��UI�ȊO��`��
    static void DrawWave(void);                                                         // �g��`��
    static void DrawPlayer(void);                                                       // �v���C���[��`��
    static void DrawInBlowMoment(void);                                                 // �����̈ꌂ���̕`��
    static void QuickSort(CScene*apScene[], int left, int right);                       // �N�C�b�N�\�[�g
    static void ReleaseAll(void);                                                       // �S�ĊJ��
#ifdef _DEBUG
    static void ReleaseReloadUI(void);                                                  // �����[�h����UI���폜
    void SetReloadUI(void) { m_bReloadUI = true; }                                      // �����[�h����UI��ݒ�
#endif

    static int GetNumObjAll(void) { return m_nNumObjectAll; }                           // �V�[����̑S�ẴI�u�W�F�N�g��
    static int GetNumAll(OBJTYPE objtype) { return m_nNumAll[objtype]; }                // ���̎�ނ̑S�̂̐����擾
    static CScene*GetTopScene(OBJTYPE objtype) { return m_apTop[objtype]; }              // ���̎�ނ̐擪�̃I�u�W�F�N�g���擾
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
    static CScene *m_apTop[OBJTYPE_MAX];  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
    static CScene *m_apCur[OBJTYPE_MAX];  // ���݂ɂ�����ŐV�̃I�u�W�F�N�g�ւ̃|�C���^
    CScene *m_pPrev;                      // �O�̃I�u�W�F�N�g�ւ̃|�C���^
    CScene *m_pNext;                      // ���̃I�u�W�F�N�g�ւ̃|�C���^

    static int m_nNumObjectAll;          // �V�[����̂��ׂẴI�u�W�F�N�g�̐�
    static int m_nNumAll[OBJTYPE_MAX];   // �V�[����̎�ނ��Ƃ̃I�u�W�F�N�g�̍ő吔
    OBJTYPE m_objType;                   // �I�u�W�F�N�g�̎��

    bool m_bUse;                         // �g�p���邩�ǂ���

#ifdef _DEBUG
    bool m_bReloadUI;                    // �����[�h����UI���ǂ���
#endif
};

#endif