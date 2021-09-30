//======================================================================================
//
// プレイヤーのカスタマイズ周りの処理 (player_custom.cpp)
// Author : 後藤慎之助
//
//======================================================================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "player.h"

//=============================================================================
// カスタマイズのパーツ名
// Author : 後藤慎之助
//=============================================================================
void CPlayer::CustomPartsName(char * pName, int nPartsType)
{
    switch (nPartsType)
    {
    case 0:
        wsprintf(pName, "エックスヘッド");
        break;
    case 1:
        wsprintf(pName, "エックスボディ");
        break;
    case 2:
        wsprintf(pName, "エックスレッグ");
        break;
    case 3:
        wsprintf(pName, "錆びた槍");
        break;
    case 4:
        wsprintf(pName, "クライノートヘッド");
        break;
    case 5:
        wsprintf(pName, "クライノートボディ");
        break;
    case 6:
        wsprintf(pName, "クライノートレッグ");
        break;
    case 7:
        wsprintf(pName, "クライノートソード");
        break;
    case 8:
        wsprintf(pName, "イカロスヘッド");
        break;
    case 9:
        wsprintf(pName, "イカロスアーマー");
        break;
    case 10:
        wsprintf(pName, "イカロスレッグ");
        break;
    case 11:
        wsprintf(pName, "イカロスソード");
        break;
    case 12:
        wsprintf(pName, "レンジャーヘッド");
        break;
    case 13:
        wsprintf(pName, "レンジャーボディ");
        break;
    case 14:
        wsprintf(pName, "レンジャーレッグ");
        break;
    case 15:
        wsprintf(pName, "ナイトヘルム");
        break;
    case 16:
        wsprintf(pName, "ナイトアーマー");
        break;
    case 17:
        wsprintf(pName, "ナイトグリーヴ");
        break;
    case 18:
        wsprintf(pName, "錆びた剣");
        break;
    case 19:
        wsprintf(pName, "ルシファーヘッド");
        break;
    case 20:
        wsprintf(pName, "ルシファーアーマー");
        break;
    case 21:
        wsprintf(pName, "ルシファーレッグ");
        break;
    case 22:
        wsprintf(pName, "ヘブンズエッジ");
        break;
    case 23:
        wsprintf(pName, "タカヘッド");
        break;
    case 24:
        wsprintf(pName, "クジャクボディ");
        break;
    case 25:
        wsprintf(pName, "コンドルレッグ");
        break;
    case 26:
        wsprintf(pName, "猟銃");
        break;
    default:
        wsprintf(pName, "無効なテキスト");
        break;
    }
}

//=============================================================================
// カスタマイズの特殊能力名
// Author : 後藤慎之助
//=============================================================================
void CPlayer::CustomExName(char* pName, int exFlag)
{
    // 特殊能力が存在するかどうか
    bool bExist = false;

    if (IS_BITON(exFlag, EX_FLAG_THREE_JUMP))
    {
        if (!bExist)
        {
            wsprintf(pName, "三段ジャンプ");
        }
        else
        {
            strcat(pName, "/三段ジャンプ");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_FAST_CHARGE))
    {
        if (!bExist)
        {
            wsprintf(pName, "溜め短縮");
        }
        else
        {
            strcat(pName, "/溜め短縮");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_TRAIL_GREEN))
    {
        if (!bExist)
        {
            wsprintf(pName, "武器軌跡色:緑");
        }
        else
        {
            strcat(pName, "/武器軌跡色:緑");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_TRAIL_PURPLE))
    {
        if (!bExist)
        {
            wsprintf(pName, "武器軌跡色:紫");
        }
        else
        {
            strcat(pName, "/武器軌跡色:紫");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_DONT_CATCH))
    {
        if (!bExist)
        {
            wsprintf(pName, "キャッチ使用不可");
        }
        else
        {
            strcat(pName, "/キャッチ×");
        }
        bExist = true;
    }

    if (IS_BITON(exFlag, EX_FLAG_FIRST_HIT_GUARD))
    {
        if (!bExist)
        {
            wsprintf(pName, "ファーストヒットガード");
        }
        else
        {
            strcat(pName, "/初撃封じ");
        }
        bExist = true;
    }

    // 存在しないなら
    if (!bExist)
    {
        wsprintf(pName, "なし");
    }
}

//=============================================================================
// カスタマイズの必殺技名
// Author : 後藤慎之助
//=============================================================================
void CPlayer::CustomSpName(char* pName)
{
    switch (m_spShot)
    {
    case SP_SHOT_HEAL:
        wsprintf(pName, "ヒーリング");
        break;
    case SP_SHOT_DECOY:
        wsprintf(pName, "ぶんしんボール");
        break;
    case SP_SHOT_WALL_THROUGH:
        wsprintf(pName, "ウォールスルーボール");
        break;
	case SP_SHOT_BARRIER:
		wsprintf(pName, "バリア");
		break;
    default:
        wsprintf(pName, "必殺技はありません");
        break;
    }
}