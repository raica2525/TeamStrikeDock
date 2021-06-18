//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	light.h
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#ifndef _LIGHT_H_
#define _LIGHT_H_

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//クラス定義
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
class CLight
{
public:
	CLight();
	~CLight();
	void Init(void);
	void Uninit(void);
	void Update(void);
private:
	D3DLIGHT9 m_light;//カメラ情報
};

#endif