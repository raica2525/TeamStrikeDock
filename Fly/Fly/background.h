//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	background.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene.h"

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//前方宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CPolygon3d;

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CBackground : public CScene
{
public:
    CBackground();     //コンストラクタ
    ~CBackground();    //デストラクタ

    HRESULT Init(void);    //初期化処理
    void Uninit(void);     //終了処理
    void Update(void);     //更新処理
    void Draw(void);       //描画処理

    static CBackground* Create(float fDistanceFromCamera);    //ポリゴン生成処理
	static void Load(void);
private:
    static LPDIRECT3DTEXTURE9 m_pTexture;    // テクスチャへのポインタ
    D3DXVECTOR3 m_pos;                // ポリゴンの位置
    D3DXVECTOR3 m_size;
    CPolygon3d* m_pPolygon3d;
	float m_fDistanceFromCamera;
};

#endif
