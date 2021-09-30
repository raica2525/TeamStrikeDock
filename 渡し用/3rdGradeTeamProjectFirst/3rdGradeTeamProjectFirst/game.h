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
#include "player.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBall;
class CPause;
class CUI;
class CEffect2D;
class CNumberArray;
class CText;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CMode
{
public:

    // �Q�[���̎��
    typedef enum
    {
        TYPE_TRAINING = 0,     // �g���[�j���O
        TYPE_ARENA,            // ���Z��
        TYPE_MINI,             // �~�j�Q�[��
        TYPE_MAX			   // ��Ԃ̍ő吔
    }TYPE;

    // �Q�[���̏�ԁi���j
    typedef enum
    {
        STATE_ROUND_START = 0,     // �J�n
        STATE_BUTTLE,              // �o�g����
        STATE_BLOW_MOMENT,         // �ꌂ�̏u��
        STATE_FINISH,              // ����
        STATE_PAUSE_MENU,	       // �|�[�Y���j���[
        STATE_MAX			       // ��Ԃ̍ő吔
    }STATE;

    // �}�b�v����
    typedef struct
    {
        float fHeight;
        float fWidth;
    }MAP_LIMIT;

    // �V���[�g�̗\��
    typedef struct
    {
        D3DXVECTOR3 attackCenterPos;    // �U���̒��S�ʒu
        D3DXVECTOR3 moveAngle;          // �ړ��p�x
        float fPower;                   // �p���[
        bool bFirstCollision;           // �d���O�̃t�@�[�X�g�q�b�g��
        int flag;                       // �{�[���ւ̃V���[�g�t���O
        bool bReserved;                 // �\�񒆂��ǂ���
    }RESERVE_SHOOT;

    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);

    void ManageState(void);     // �Q�[����Ԃ̊Ǘ�
    void RoundStart(void);      // ���E���h�J�n
    void InButtle(void);        // �o�g����
    void BlowMoment(void);      // �ꌂ�̏u��
    void JudgmentFinish(void);  // ���s����

    /*========================================================
    // �Z�b�^�[
    //======================================================*/
    static void SetNextGameInDebug(TYPE gameType, int nNumPlayer, int nNumStock, bool bUseKeyboard = false) 
    {
        m_type = gameType;  m_nNumAllPlayer = nNumPlayer; m_nNumStock = nNumStock; m_bUseKeyboard = bUseKeyboard;
    }   // �f�o�b�O���j���[�ŁA���̃Q�[����ݒ�
    static void SetNextGameInCustom(TYPE gameType, int nNumPlayer)
    {
        m_type = gameType;  m_nNumAllPlayer = nNumPlayer;
    }   // �J�X�^�}�C�Y��ʂŁA���̃Q�[����ݒ�
    static void SetIdxPlayer(int nNum, int nIdxPlayer) { m_anMemoryIdxPlayer[nNum] = nIdxPlayer; }  // �v���C���[�̃C���f�b�N�X
    static void SetAILevel(int nNum, CPlayer::AI_LEVEL level) { m_aMemoryAILevel[nNum] = level; }   // AI���x��
    static void SetAddNumDefeatPlayer(int nIdxPlayer) { SetPlayerRankInThisRound(nIdxPlayer); m_nNumDefeatPlayer++; }                       // ���ꂽ�v���C���[�l���𑝂₷
    static void SetWorstPlayer(int player) { m_nWhoWorstPlayer = player; }                  // ���[�X�g�̃v���C���[�����߂�
    static void SetAddNumDeathPlayer(int nIdxPlayer) { SetPlayerRankInThisRound(nIdxPlayer); m_anPlayerRank[m_nNumAllPlayer - m_nNumDeathPlayer - 1] = nIdxPlayer; m_nNumDeathPlayer++; } // ���񂾃v���C���[�ԍ����L�����A�J�E���g
    static void SetQuitPause(void) { m_state = STATE_BUTTLE; m_bStopObjUpdate = false; }    // �|�[�Y��Ԃ���߂�
    static void SetStock(int nStock) { m_nNumStock = nStock; }
    static void SetUseKeyboard(bool bUseKeyboard) { m_bUseKeyboard = bUseKeyboard; }
    static void SetPlayerRankInThisRound(int nIdxPlayer)
    { 
        int nRank = GetNumCurrentDispPlayer() - 1;
        if (nRank < CPlayer::RANK_1)
        {
            nRank = CPlayer::RANK_2;    // �O�̂��߁A�s���Ȕz����Q�Ƃ��Ȃ��悤�ɂ���
        }
        m_anPlayerRankInThisRound[nRank] = nIdxPlayer;
    }
    static void SetSpPlayer(CPlayer *pPlayer) { m_bCurrentSpShot = true; m_pSpPlayer = pPlayer; }
    static void SetCurrentSpShot(bool bStopUpdate) { m_bCurrentSpShot = bStopUpdate; }

    /*========================================================
    // �Q�b�^�[
    //======================================================*/
    static STATE GetState(void) { return m_state; }                                 // �Q�[���̏�Ԃ��擾
    static CPlayer* GetPlayer(const int nNum) { return m_apPlayer[nNum]; }          // �v���C���[���擾
    static CBall* GetBall(void) { return m_pBall; }                                 // �{�[�����擾
    static int GetNumAllPlayer(void) { return m_nNumAllPlayer; }                    // �S�̃v���C���[�l�����擾
    static int GetNumDefeatPlayer(void) { return m_nNumDefeatPlayer; }              // ���ꂽ�v���C���[�l�����擾
    static int GetWhoWorstPlayer(void) { return m_nWhoWorstPlayer; }                // ���[�X�g�̃v���C���[���擾
    static int GetNumDeathPlayer(void) { return m_nNumDeathPlayer; }                // ���񂾃v���C���[�l�����擾
    static MAP_LIMIT GetMapLimit(void) { return m_mapLimit; }                       // �}�b�v�������擾
    static TYPE GetType(void) { return m_type; }                                    // �Q�[����ނ��擾
    static bool GetStopObjUpdate(void) { return m_bStopObjUpdate; }                 // �I�u�W�F�N�g�̃A�b�v�f�[�g���~�߂邩�ǂ���
    static int GetNumCurrentDispPlayer(void) { return (m_nNumAllPlayer - m_nNumDeathPlayer - m_nNumDefeatPlayer); } // ���ݐ����Ă���v���C���[�l�����擾
    static CEffect2D* GetNega(void) { return m_pEffect2d_Nega; }                    // ���]�����̃G�t�F�N�g���擾
    static CEffect2D* GetPosi(void) { return m_pEffect2d_Posi; }                    // ���]������߂����߂̃G�t�F�N�g���擾
    static CNumberArray* GetNumArrayBallSpd(void) { return m_pNumArray_BallSpd; }   // �{�[���X�s�[�h�\�����擾
    static bool GetUseKeyboard(void) { return m_bUseKeyboard; }                     // �L�[�{�[�h���g�����ǂ����擾
    static int GetStock(void) { return m_nNumStock; }                               // �X�g�b�N�����擾
    static int GetPlayerRank(int nIdx) { return m_anPlayerRank[nIdx]; }             // �v���C���[�̏��ʂ��擾
    static int GetPlayerRankInThisRound(int nIdx) { return m_anPlayerRankInThisRound[nIdx]; }  // �����E���h�̃v���C���[�̏��ʂ��擾
    static CPlayer*GetSpPlayer(void) { return m_pSpPlayer; }
    static bool GetCurrentSpShot(void) { return m_bCurrentSpShot; }
    static CText *GetSpText(void) { return m_pSpText; }

    /*========================================================
    // �֗��Ȋ֐�
    //======================================================*/
    static void ReserveShoot(D3DXVECTOR3 attackCenterPos, D3DXVECTOR3 moveAngle, float fPower, bool bFirstCollision, int flag, int nWho);
    void JudgmentShoot(void);                                                       // �N���V���[�g��łĂ����̔���
    static float GetAngleToClosestPlayer(int nIdxPlayer, D3DXVECTOR3 myPos);        // ��ԋ߂��v���C���[�ւ̊p�x�����߂�
    static D3DXVECTOR3  GetPosToClosestPlayer(int nIdxPlayer, D3DXVECTOR3 myPos);   // ��ԋ߂��v���C���[�̈ʒu�����߂�
    static void SetBallGauge(int nMax, int nNow);                                   // �{�[�����˃Q�[�W

private:

    static bool m_bStopObjUpdate;                          // �I�u�W�F�N�g�̃A�b�v�f�[�g���~�߂邩�ǂ���
    static CPlayer *m_apPlayer[MAX_PLAYER];                // �v���C���[�̃|�C���^
    static int m_anPlayerRank[MAX_PLAYER];                 // �v���C���[�̃����L���O�i�z��̎Ⴂ������1�ʁA2�ʁA�A�A�j
    static int m_anPlayerRankInThisRound[MAX_PLAYER];      // ���̃��E���h���̃v���C���[�̃����L���O�i�z��̎Ⴂ������1�ʁA2�ʁA�A�A�j
    static CBall *m_pBall;                                 // �{�[���̃|�C���^
    static CPause *m_pPause;                               // �|�[�Y�̃|�C���^
    static CEffect2D *m_pEffect2d_Nega;                    // ���]�����ւ̃|�C���^
    static CEffect2D *m_pEffect2d_Posi;                    // ���]������߂����߂̃|�C���^
    static CNumberArray *m_pNumArray_BallSpd;              // �{�[���X�s�[�h�\���ւ̃|�C���^

    static TYPE m_type;                                    // �Q�[�����
    static int m_nNumAllPlayer;                            // �S�̃v���C���[�l��
    static int m_nNumStock;                                // �X�g�b�N��
    static bool m_bUseKeyboard;                            // �L�[�{�[�h���g�����ǂ���
    static int m_anMemoryIdxPlayer[MAX_PLAYER];            // �v���C���[�̃C���f�b�N�X���L��
    static CPlayer::AI_LEVEL m_aMemoryAILevel[MAX_PLAYER]; // AI���x�����L��
    static STATE m_state;                                  // ���
    static MAP_LIMIT m_mapLimit;                           // �}�b�v����
    static int m_nNumDefeatPlayer;                         // ���ꂽ�v���C���[�l��
    static int m_nWhoWorstPlayer;                          // ���[�X�g�̃v���C���[
    static int m_nNumDeathPlayer;                          // ���񂾃v���C���[�l��
    static RESERVE_SHOOT m_aReserveShoot[MAX_PLAYER];      // �V���[�g�̗\��

    static CPlayer *m_pSpPlayer;                           // �X�g���C�N�V���[�g�v���C���[�̃|�C���^
    static bool m_bCurrentSpShot;                          // ���݃X�g���C�N�V���[�g�������Ă��邩�ǂ���
    static CText *m_pSpText;                               // �X�g���C�N�V���[�g�̖��O

    int m_nCntGameTime;                                    // �Q�[�����Ԃ̃J�E���^
    bool m_bFirestRound;                                   // �ŏ��̃��E���h���ǂ���
};

#endif