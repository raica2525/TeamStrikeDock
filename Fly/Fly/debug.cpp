//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//
//	debug.cpp
//	Author:池田悠希
//
//・・・・・・・・・・・・・・・・・・・・・・・・・・・

//・・・・・・・・・・・・・・・・・・・・・・・・・・・
//インクルードファイル
//・・・・・・・・・・・・・・・・・・・・・・・・・・・
#include "debug.h"
#include "renderer.h"
#include "manager.h"
#include <stdio.h>
LPD3DXFONT CDebug::m_pFont = NULL;
char CDebug::m_str[512] = {};
CDebug::CDebug()
{
	
}

CDebug::~CDebug()
{

}

HRESULT CDebug::Init()
{
	D3DXCreateFont(CManager::GetRenderer()->GetDevice(), 50, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
	return S_OK;
}

void CDebug::Print(char* fmt, ...)
{
	ZeroMemory(m_str, sizeof(m_str));
	va_list list;
	va_start(list, fmt);
	for (char* pointer = fmt; *pointer != '\0'; pointer++)
	{
		char str[256] = {};
		ZeroMemory(str, sizeof(str));
		switch (*pointer)
		{
			pointer++;
			case 'd':
				wsprintf(str, "%d", va_arg(list, int));
				break;
			case 'f':
				wsprintf(str, "%f", va_arg(list, double));
				break;
			case 'c':
				wsprintf(str, "%c", va_arg(list, char));
				break;
			case 's':
				wsprintf(str, "%s", va_arg(list, const char*));
					break;
			case'%':
				break;
			default:
				memcpy(str, pointer, 1);
				break;
		}
		strcat(m_str, str);
	}
	va_end(list);
}

void CDebug::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	// テキスト描画
	m_pFont->DrawText(NULL, m_str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	ZeroMemory(m_str, sizeof(m_str));
}
void CDebug::Uninit()
{
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}