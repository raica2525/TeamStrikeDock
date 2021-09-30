//====================================================================
//
// シーン上のオブジェクト処理 (scene.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// マクロ定義
//================================================

//================================================
// クラス宣言
//================================================

// オブジェクトのクラス
class CScene
{
public:
    // オブジェクトの種類(描画順)
    typedef enum
    {
        OBJTYPE_NONE,               // なし
        OBJTYPE_NONE_DRAW,          // 描画しない
        OBJTYPE_BG,                 // 背景
        OBJTYPE_EFFECT3D_Z_NONE,    // エフェクト3D（Zバッファを無視）
        OBJTYPE_BALL,               // ボール
        //OBJTYPE_ENEMY,            // エネミー（今回はいない、その代わりCPUがいる）
        OBJTYPE_PLAYER,             // プレイヤー
        //OBJTYPE_BLOCK,            // ブロック（今回は使っていない）
        OBJTYPE_MODEL_EFFECT,       // モデルエフェクト
        //OBJTYPE_ITEM,             // アイテム（今回は使っていない）
        OBJTYPE_EFFECT3D,           // エフェクト3D
        OBJTYPE_EFFECT2D_BACK_UI,   // エフェクト2D（UIより後ろ）
        OBJTYPE_UI_BACK_TEXT,       // UI（テキストより後ろ）
        OBJTYPE_TEXT,               // テキスト
        OBJTYPE_UI_FRONT_TEXT,      // UI（テキストより手前）
        OBJTYPE_EFFECT2D_FRONT_UI,  // エフェクト2D（UIより手前）
        OBJTYPE_WAVE,               // 画面上に出るウェーブ
        OBJTYPE_MAX                 // オブジェクトの種類の最大数
    }OBJTYPE;

    CScene(OBJTYPE objType);    // イニシャライザでオブジェクトタイプを紐づけ
    virtual ~CScene();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;
    virtual void Uninit(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    static void UpdateAll(void);                                                        // 全て更新
    static void SpUpdate(void);                                                         // 必殺技使用時のアップデート
    static void DrawExceptWave(void);                                                   // 波以外を描画
    static void DrawExceptWaveAndUI(void);                                              // 波とUI以外を描画
    static void DrawWave(void);                                                         // 波を描画
    static void DrawPlayer(void);                                                       // プレイヤーを描画
    static void DrawInBlowMoment(void);                                                 // 決着の一撃時の描画
    static void QuickSort(CScene*apScene[], int left, int right);                       // クイックソート
    static void ReleaseAll(void);                                                       // 全て開放
#ifdef _DEBUG
    static void ReleaseReloadUI(void);                                                  // リロードするUIを削除
    void SetReloadUI(void) { m_bReloadUI = true; }                                      // リロードするUIを設定
#endif

    static int GetNumObjAll(void) { return m_nNumObjectAll; }                           // シーン上の全てのオブジェクト個数
    static int GetNumAll(OBJTYPE objtype) { return m_nNumAll[objtype]; }                // その種類の全体の数を取得
    static CScene*GetTopScene(OBJTYPE objtype) { return m_apTop[objtype]; }              // その種類の先頭のオブジェクトを取得
    CScene* GetNextScene(void) { return m_pNext; }                                      // 次のオブジェクトを取得

    /*============================================================================================================================
    // オブジェクトのタイプごとでコンストラクタで次のシーン等を記憶しているため、絶対にオブジェクトタイプを書き換えてはいけない
    //void SetObjType(OBJTYPE objtype) { m_objType = objtype; }
    // トップに種類を入れて取得できるので、ゲッターも使う機会がなくなった
    //OBJTYPE GetObjType(void) { return m_objType; }
    ============================================================================================================================*/

protected:
    void Release(void);                 // 単体を開放

private:
    static CScene *m_apTop[OBJTYPE_MAX];  // 先頭のオブジェクトへのポインタ
    static CScene *m_apCur[OBJTYPE_MAX];  // 現在における最新のオブジェクトへのポインタ
    CScene *m_pPrev;                      // 前のオブジェクトへのポインタ
    CScene *m_pNext;                      // 次のオブジェクトへのポインタ

    static int m_nNumObjectAll;          // シーン上のすべてのオブジェクトの数
    static int m_nNumAll[OBJTYPE_MAX];   // シーン上の種類ごとのオブジェクトの最大数
    OBJTYPE m_objType;                   // オブジェクトの種類

    bool m_bUse;                         // 使用するかどうか

#ifdef _DEBUG
    bool m_bReloadUI;                    // リロードするUIかどうか
#endif
};

#endif