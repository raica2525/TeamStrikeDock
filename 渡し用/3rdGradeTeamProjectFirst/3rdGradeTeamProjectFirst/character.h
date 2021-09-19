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
#define CHARCTER_PARTS_MAX 18       // キャラクターのパーツの最大数

//================================================
// 前方宣言 
//================================================
class CModel;
class CAnimation;

//================================================
// クラス宣言
//================================================

// キャラクタークラス
class CCharacter : public CScene
{
public:
    CCharacter(OBJTYPE objType);
    ~CCharacter();

    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void DeathDraw(void);                                         // やられた時の描画
    void LoadModelData(char* cFilePass);                          // テキストファイルから直接読み込む時に使う関数
    void LoadModelData(int nModelPosDefUp, int nModelPosDefDown); // モデルの初期位置データから読み込むときに使う関数

    /*========================================================
    // セッター
    //======================================================*/
    void SetPartPos(int index, D3DXVECTOR3 pos) { m_apModel[index]->SetPos(pos); }
    void SetPartRot(int index, D3DXVECTOR3 rot) { m_apModel[index]->SetRot(rot); }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetPartNum(int nPartNum) { m_nPartsNum = nPartNum; }                       // パーツの最大数を設定
    void SetPosOld(const D3DXVECTOR3 posOld) { m_posOld = posOld; }
    void SetAnimFilePass(char* cFilePass) { m_cAnimFilePass = cFilePass; }

    void BindParts(int nPartsIndex, int nPartsType) { m_aPartsType[nPartsIndex] = nPartsType; }

    /*========================================================
    // セッター
    //======================================================*/
    D3DXVECTOR3 GetDefaultPos(int index) { return m_aPosDefault[index]; }
    D3DXVECTOR3 GetPartsRot(int index) { return m_apModel[index]->GetRot(); }
    CModel * GetParts(int index) { return m_apModel[index]; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
    CAnimation * GetAnimation(void) { return m_pAnimation; }
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

    // アニメーションを使用しない場合のセッター
    void SetUnableAnimation(void) { m_bUseAnimation = false; }

private:
    int                     m_aPartsType[CHARCTER_PARTS_MAX];           // パーツタイプ
    D3DXVECTOR3				m_aPosDefault[CHARCTER_PARTS_MAX];          // 初期位置
    int						m_anIndexParent[CHARCTER_PARTS_MAX];        // 親設定

    D3DXVECTOR3		m_pos;                                              // 全ての親の位置
    D3DXVECTOR3		m_rot;                                              // 全ての親の向き
    int				m_nPartsNum;                                        // パーツ数

    CModel			*m_apModel[CHARCTER_PARTS_MAX];                     // モデルへのポインタ
    CAnimation		*m_pAnimation;                                      // アニメーションへのポインタ

    D3DXVECTOR3		m_posOld;                                           // 1F前の位置

    char* m_cAnimFilePass;                                              // アニメーションファイルのパス

    bool m_bUseAnimation;												// アニメーションさせるかどうか
};

#endif