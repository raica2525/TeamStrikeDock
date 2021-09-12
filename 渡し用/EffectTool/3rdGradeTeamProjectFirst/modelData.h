//=============================================================================
//
// モデルデータの処理 [modelData.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_MODEL_DATA (256)    // モデルそのもののデータ
#define MAX_PARTS_LIST (128)    // パーツリスト
#define MAX_MODEL_POS_DEF (64)  // モデルの初期位置データ
#define MAX_MODEL_PARTS_SET (8) // モデルパーツの最大セット数（上半身と下半身の各パーツの最大）
#define MAX_PARAM (8)           // 汎用のパラメータ
#define MAX_PARTS (4)           // パーツの最大数
#define MAX_MATERIAL (32)       // マテリアルの最大数

//=============================================================================
// モデルデータクラス
//=============================================================================
class CModelData
{
public:

    typedef struct
    {
        //int nNumTexture;                              // 読み込むテクスチャの番号  （xファイルから直接読み込むため削除）
        LPDIRECT3DTEXTURE9	apTexMat[MAX_MATERIAL];     // 読み込むテクスチャをxファイルからマテリアル数分取得
        LPD3DXMESH pMesh;                               // 読み込むメッシュ
        LPD3DXBUFFER pBuffMat;                          // 読み込むマテリアルのバッファ
        DWORD nNumMat;                                  // 読み込むマテリアル数
    }ModelData; // モデルデータ

    typedef struct
    {
        int nIndex[MAX_MODEL_PARTS_SET];                // インデックス
        int nParent[MAX_MODEL_PARTS_SET];               // 親
        D3DXVECTOR3 pos[MAX_MODEL_PARTS_SET];           // 位置
    }ModelPosDef; // モデルの初期位置データ

    typedef struct
    {
        float fAtk;                    // 攻撃
        float fDef;                    // 防御
        float fSpd;                    // 速さ
        float fWei;                    // 重さ
        int nEx;                       // 特殊能力番号
        float afParam[MAX_PARAM];      // 汎用パラメータ
    }PartsParam;    // パーツパラメータ

    typedef struct
    {
        float fAtkRate;                // 攻撃割合
        float fDefRate;                // 防御割合
        float fSpdRate;                // 速さ割合
        float fWeiRate;                // 重さ割合
    }PartsRate;    // パーツレート

    CModelData();	// コンストラクタ
    ~CModelData();	// デストラクタ

    HRESULT Init(void);
    void LoadModelPosDef(void);
    void LoadPartsList(void);
    void LoadPartsRate(void);
    void Uninit(void);

    ModelData* GetModelData(const int nNum);    // モデル情報
    ModelPosDef* GetPosDef(const int nNum);     // モデルの初期位置情報
    PartsParam* GetPartsList(const int nNum);   // パーツリスト情報
    PartsRate* GetPartsRate(const int nNum);    // パーツレート情報

private:
    ModelData m_aModelData[MAX_MODEL_DATA];         // モデルデータ
    ModelPosDef m_aModelPosDef[MAX_MODEL_POS_DEF];  // モデルの初期位置データ
    PartsParam m_aPartsList[MAX_PARTS_LIST];        // パーツリスト
    PartsRate m_aPartsRate[MAX_PARTS];              // パーツレート
};
#endif