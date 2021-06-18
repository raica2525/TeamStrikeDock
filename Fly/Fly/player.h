//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	player.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

#ifndef _PLAYER_H_
#define _PLAYER_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "main.h"
#include "scene.h"
#include "model.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#define START_LIFE (10)
#define PLAYER_MODEL_COUNT (32)
#define MAX_PLAYER (4)
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//前方宣言
//・・・・・・・・・・・・・・・・・・・・・・・・・・・


class CPlayer : public CScene
{
public:
	typedef enum
	{
		STATE_STAND = 0,
		STATE_FLOAT,
		STATE_FLY
	}STATE;
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer* Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 size);
	static void Load(void);
	static void Unload(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	static CPlayer* GetPlayer(int nIndex);
private:
	CModel* m_apModel[PLAYER_MODEL_COUNT];
	static CModel::MODELDATA m_modeldata[PLAYER_MODEL_COUNT];
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_addRot;
	STATE m_state;
	float m_fSpeed;
	int m_nLife;
	int m_nIndex;
	static CPlayer* m_apPlayer[MAX_PLAYER];
};
#endif
