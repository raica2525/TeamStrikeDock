//=============================================================================
//
// �Q�[������ [game.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "mode.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;
class CBall;
class CScore;
class CPause;
class CUI;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �Q�[����Ԃ̊Ǘ��t���[��
#define COUNT_WAIT_FINISH 300	    // �Q�[���I�����ɁA�^�C�g����ʂ֍s���܂ł̎���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CMode
{
public:

    // �Q�[���̏�ԁi���j
    typedef enum
    {
        STATE_ROUND_START = 0,     // �J�n
        STATE_BUTTLE,              // �o�g����
        STATE_FINISH_ROUND,	       // ���E���h������
        STATE_FINISH_GAME,         // �Q�[��������
        STATE_PAUSE,	           // �|�[�Y
        STATE_MAX			       // ��Ԃ̍ő吔
    }STATE;

    // �}�b�v����
    typedef struct
    {
        float fHeight;
        float fWidth;
    }MAP_LIMIT;

    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

    void Judgment(void);                                                            // ���s����
    void ManageState(void);                                                         // �Q�[����Ԃ̊Ǘ�

    void Buttle(void);

    /* �Z�b�^�[*/
    static void SetState(STATE state) { m_state = state; }                          // �Q�[���̏�Ԃ�ݒ�
    static void SetAddNumCurrentPlayer(void) { m_nNumCurrentPlayer++; }             // ���݂̃v���C���[�l���𑝂₷

    /* �Q�b�^�[*/
    static STATE GetState(void) { return m_state; }                                 // �Q�[���̏�Ԃ��擾
    static CPlayer* GetPlayer(const int nNum) { return m_apPlayer[nNum]; }          // �v���C���[���擾
    static CBall* GetBall(void) { return m_pBall; }                                 // �{�[�����擾
    static CScore *GetScore(void) { return m_pScore; }                              // �X�R�A���擾
    static int GetNumCurrentPlayer(void) { return m_nNumCurrentPlayer; }            // ���݂̃v���C���[�l�����擾
    static MAP_LIMIT GetMapLimit(void) { return m_mapLimit; }                       // �}�b�v�������擾

    /* �֗��Ȋ֐�*/
    static float GetAngleToClosestPlayer(int playable, D3DXVECTOR3 myPos);          // ��ԋ߂��v���C���[�ւ̊p�x�����߂�
    //static D3DXVECTOR3 GetPosToClosestEnemy(D3DXVECTOR3 myPos);                   // ��ԋ߂��G�̈ʒu�����߂�

private:

    static CScore *m_pScore;                            // �X�R�A�̃|�C���^
    static CPlayer *m_apPlayer[MAX_PLAYER];             // �v���C���[�̃|�C���^
    static CBall *m_pBall;                              // �{�[���̃|�C���^
    static CPause *m_pPause;                            // �|�[�Y�̃|�C���^

    static STATE m_state;                               // ���
    static MAP_LIMIT m_mapLimit;                        // �}�b�v����
    static int m_nNumCurrentPlayer;                     // ���݂̃v���C���[�l��

    int m_nCntGameTime;                                 // �Q�[�����Ԃ̃J�E���^
};

#endif