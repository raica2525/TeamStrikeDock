//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	camera.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//マクロ定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#define CAMERA_VIEW_RANGE (1000000.0f)
#define CAMERA_FOV (45.0f)
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CCamera
{
public:

	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	D3DXVECTOR3 GetPosV();
	D3DXVECTOR3 GetPosR();
	D3DXVECTOR3 GetRot();
private:
	D3DXVECTOR3 m_posV;//カメラの座標
	D3DXVECTOR3 m_posR;//注視点の座標
	D3DXVECTOR3 m_vecU;//カメラの上方向
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			//ビューマトリックス
	D3DXVECTOR3 m_rot;			//向き所得用
	D3DXVECTOR3 m_addRot;
	float m_fDistance;			//視点-注意点の距離
	int m_nIndex;
	static int m_nCntCam;
};

#endif
