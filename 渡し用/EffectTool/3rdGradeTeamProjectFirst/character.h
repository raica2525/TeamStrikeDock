//==========================================================================================
//
// キャラクターの処理 (character.h)
// Author : 後藤慎之助
//
//==========================================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"
#include "model.h"

//================================================
// マクロ定義
//================================================
#define CHARCTER_PARTS_MAX 25       // キャラクターのパーツの最大数
#define CHARCTER_COLLISION_MAX 18   // キャラクターの当たり判定の最大数

//================================================
// 前方宣言 
//================================================
class CModel;
class CAnimation;
class CShadow;

//================================================
// クラス宣言
//================================================

// キャラクタークラス
class CCharacter : public CScene
{
public:
    CCharacter();
    CCharacter(OBJTYPE objType);
    ~CCharacter();

    // 種族(animation.hにも追加が必要)
    typedef enum
    {
        TRIBE_NONE = 0,
        TRIBE_PLAYER,			// プレイヤー
        TRIBE_MAX			    // 種類の最大数
    }TRIBE;

    // 当たり判定の情報
    typedef struct
    {
        D3DXVECTOR3 pos;        // 位置
        D3DXVECTOR3 size;       // 矩形のサイズ
    }COLLISION;

    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void LoadModelData(char* cFilePass);                          // テキストファイルから直接読み込む時に使う関数
    void LoadModelData(int nModelPosDefUp, int nModelPosDefDown); // モデルの初期位置データから読み込むときに使う関数

    // セッター
    void SetPartPos(int index, D3DXVECTOR3 pos) { m_apModel[index]->SetPos(pos); }
    void SetPartRot(int index, D3DXVECTOR3 rot) { m_apModel[index]->SetRot(rot); }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetTribe(TRIBE tribe) { m_tribe = tribe; }
    void SetPartNum(int nPartNum) { m_nPartsNum = nPartNum; }                       // パーツの最大数を設定
    void SetPosOld(const D3DXVECTOR3 posOld) { m_posOld = posOld; }
    void SetAnimFilePass(char* cFilePass) { m_cAnimFilePass = cFilePass; }

    void BindParts(int nPartsIndex, int nPartsType) { m_aPartsType[nPartsIndex] = nPartsType; }     // パーツの種類を設定

    // ゲッター
    D3DXVECTOR3 GetDefaultPos(int index) { return m_posDefault[index]; }
    D3DXVECTOR3 GetPartsRot(int index) { return m_apModel[index]->GetRot(); }
    CModel * GetParts(int index) { return m_apModel[index]; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    TRIBE GetTribe(void) { return m_tribe; }
    CAnimation * GetAnimation(void) { return m_pAnimation; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

    // 当たり判定に関するゲッターやセッター
    void SetActiveCollisionSize(const D3DXVECTOR3 collisionSize) { m_activeCollisionSize = collisionSize; }
    D3DXVECTOR3 GetActiveCollisionSize(void) { return m_activeCollisionSize; }
    COLLISION GetCollision(int nCnt) { return m_aCollision[nCnt]; }                         // 各パーツの、衝突に関する情報を返す
    void SetCollisionPos(int nCnt, D3DXVECTOR3 pos) { m_aCollision[nCnt].pos = pos; }       // 各パーツの、衝突の位置を設定
    void SetCollisionSize(int nCnt, D3DXVECTOR3 size) { m_aCollision[nCnt].size = size; }   // 各パーツの、衝突のサイズを設定
    int GetCntCollision(void) { return m_nCntCollision; }                                   // 当たり判定の数を返す
    void SetCntCollision(int nCnt) { m_nCntCollision = nCnt; }                              // 当たり判定の数を設定

    // アニメーションを使用しない場合のセッター
    void SetUnableAnimation(void) { m_bUseAnimation = false; }

private:
    int                     m_aPartsType[CHARCTER_PARTS_MAX];           // パーツタイプ
    D3DXVECTOR3				m_posDefault[CHARCTER_PARTS_MAX];           // 初期位置
    int						m_nIndexParent[CHARCTER_PARTS_MAX];         // 親設定

    D3DXVECTOR3		m_pos;                                              // 全ての親の位置
    D3DXVECTOR3		m_rot;                                              // 全ての親の向き
    TRIBE           m_tribe;                                            // 種族
    int				m_nPartsNum;                                        // パーツ数

    CModel			*m_apModel[CHARCTER_PARTS_MAX];                     // モデルへのポインタ
    CAnimation		*m_pAnimation;                                      // アニメーションへのポインタ
    CShadow         *m_pShadow;                                         // 影へのポインタ
    CShadow         *m_pFrame;                                          // ふちどりへのポインタ

    D3DXVECTOR3 m_activeCollisionSize;                                  // 能動的な当たり判定の大きさ(壁に対して使ったりする)
    D3DXVECTOR3		m_posOld;                                           // 1F前の位置

    char* m_cAnimFilePass;                                              // アニメーションファイルのパス

    COLLISION m_aCollision[CHARCTER_COLLISION_MAX];                     // 当たり判定の情報
    int m_nCntCollision;                                                // 当たり判定の数

    bool m_bUseAnimation;												// アニメーションさせるかどうか
};

#endif