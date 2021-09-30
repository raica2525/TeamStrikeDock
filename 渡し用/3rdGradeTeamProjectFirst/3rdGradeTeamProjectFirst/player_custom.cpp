//======================================================================================
//
// �v���C���[�̃J�X�^�}�C�Y����̏��� (player_custom.cpp)
// Author : �㓡�T�V��
//
//======================================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "player.h"

//=============================================================================
// �J�X�^�}�C�Y�̃p�[�c��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::CustomPartsName(char * pName, int nPartsType)
{
    switch (nPartsType)
    {
    case 0:
        wsprintf(pName, "�G�b�N�X�w�b�h");
        break;
    case 1:
        wsprintf(pName, "�G�b�N�X�{�f�B");
        break;
    case 2:
        wsprintf(pName, "�G�b�N�X���b�O");
        break;
    case 3:
        wsprintf(pName, "�K�т���");
        break;
    case 4:
        wsprintf(pName, "�N���C�m�[�g�w�b�h");
        break;
    case 5:
        wsprintf(pName, "�N���C�m�[�g�{�f�B");
        break;
    case 6:
        wsprintf(pName, "�N���C�m�[�g���b�O");
        break;
    case 7:
        wsprintf(pName, "�N���C�m�[�g�\�[�h");
        break;
    case 8:
        wsprintf(pName, "�C�J���X�w�b�h");
        break;
    case 9:
        wsprintf(pName, "�C�J���X�A�[�}�[");
        break;
    case 10:
        wsprintf(pName, "�C�J���X���b�O");
        break;
    case 11:
        wsprintf(pName, "�C�J���X�\�[�h");
        break;
    case 12:
        wsprintf(pName, "�����W���[�w�b�h");
        break;
    case 13:
        wsprintf(pName, "�����W���[�{�f�B");
        break;
    case 14:
        wsprintf(pName, "�����W���[���b�O");
        break;
    case 15:
        wsprintf(pName, "�i�C�g�w����");
        break;
    case 16:
        wsprintf(pName, "�i�C�g�A�[�}�[");
        break;
    case 17:
        wsprintf(pName, "�i�C�g�O���[��");
        break;
    case 18:
        wsprintf(pName, "�K�т���");
        break;
    case 19:
        wsprintf(pName, "���V�t�@�[�w�b�h");
        break;
    case 20:
        wsprintf(pName, "���V�t�@�[�A�[�}�[");
        break;
    case 21:
        wsprintf(pName, "���V�t�@�[���b�O");
        break;
    case 22:
        wsprintf(pName, "�w�u���Y�G�b�W");
        break;
    case 23:
        wsprintf(pName, "�^�J�w�b�h");
        break;
    case 24:
        wsprintf(pName, "�N�W���N�{�f�B");
        break;
    case 25:
        wsprintf(pName, "�R���h�����b�O");
        break;
    case 26:
        wsprintf(pName, "�e");
        break;
    default:
        wsprintf(pName, "�����ȃe�L�X�g");
        break;
    }
}

//=============================================================================
// �J�X�^�}�C�Y�̓���\�͖�
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::CustomExName(char* pName, int exFlag)
{
    // ����\�͂����݂��邩�ǂ���
    bool bExist = false;

    if (IS_BITON(exFlag, EX_FLAG_THREE_JUMP))
    {
        if (!bExist)
        {
            wsprintf(pName, "�O�i�W�����v");
        }
        else
        {
            strcat(pName, "/�O�i�W�����v");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_FAST_CHARGE))
    {
        if (!bExist)
        {
            wsprintf(pName, "���ߒZ�k");
        }
        else
        {
            strcat(pName, "/���ߒZ�k");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_TRAIL_GREEN))
    {
        if (!bExist)
        {
            wsprintf(pName, "����O�ՐF:��");
        }
        else
        {
            strcat(pName, "/����O�ՐF:��");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_TRAIL_PURPLE))
    {
        if (!bExist)
        {
            wsprintf(pName, "����O�ՐF:��");
        }
        else
        {
            strcat(pName, "/����O�ՐF:��");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_SAVE_SP_GAUGE))
    {
        if (!bExist)
        {
            wsprintf(pName, "�K�E�Q�[�W�����z��");
        }
        else
        {
            strcat(pName, "/�K�E�Q�[�W�����z��");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_ROUND_HEAL))
    {
        if (!bExist)
        {
            wsprintf(pName, "���E���h���W�F�l");
        }
        else
        {
            strcat(pName, "/���E���h���W�F�l");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_DONT_CATCH))
    {
        if (!bExist)
        {
            wsprintf(pName, "�L���b�`�g�p�s��");
        }
        else
        {
            strcat(pName, "/�L���b�`�~");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_FIRST_HIT_GUARD))
    {
        if (!bExist)
        {
            wsprintf(pName, "�t�@�[�X�g�q�b�g�K�[�h");
        }
        else
        {
            strcat(pName, "/��������");
        }
        bExist = true;
    }

    // ���݂��Ȃ��Ȃ�
    if (!bExist)
    {
        wsprintf(pName, "�Ȃ�");
    }
}

//=============================================================================
// �J�X�^�}�C�Y�̕K�E�Z��
// Author : �㓡�T�V��
//=============================================================================
void CPlayer::CustomSpName(char* pName)
{
    switch (m_spShot)
    {
    case SP_SHOT_HEAL:
        wsprintf(pName, "�q�[�����O");
        break;
    case SP_SHOT_DECOY:
        wsprintf(pName, "�Ԃ񂵂�{�[��");
        break;
    case SP_SHOT_WALL_THROUGH:
        wsprintf(pName, "�E�H�[���X���[�{�[��");
        break;
	case SP_SHOT_BARRIER:
		wsprintf(pName, "�o���A");
		break;
    default:
        wsprintf(pName, "�K�E�Z�͂���܂���");
        break;
    }
}