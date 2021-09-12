//====================================================================
//
// �`�揈�� (renderer.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �N���X�錾
//================================================

// �����_�����O�N���X
class CRenderer
{
public:
    CRenderer();
    ~CRenderer();
    HRESULT Init(HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);    // �I�u�W�F�N�g�̃C���X�^���X���Ȃ��ƁA�֐����ĂׂȂ����ߎ����Ă���
    void Draw(void);      // �I�u�W�F�N�g�̃C���X�^���X���Ȃ��ƁA�֐����ĂׂȂ����ߎ����Ă���
    LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
#ifdef _DEBUG
    static void SwitchDispFont(void) { m_bDispFont = !m_bDispFont; }
#endif

private:
    LPDIRECT3D9				m_pD3D;			// Direct3D�I�u�W�F�N�g
    LPDIRECT3DDEVICE9		m_pD3DDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
    void DrawDebugData(void);
    void GameDebugData(void);
    LPD3DXFONT				m_pFont;        // �t�H���g�ւ̃|�C���^
    static bool             m_bDispFont;    // �t�H���g��\�����邩�ǂ���
#endif
};

#endif