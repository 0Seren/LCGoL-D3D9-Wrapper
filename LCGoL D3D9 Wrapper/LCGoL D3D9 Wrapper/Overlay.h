#pragma once
#include "Stdafx.h"

namespace Overlay {
	void Initialize(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
	void Draw();
	void DrawIGTRectangle();
	void DrawIGT();
	void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
	void OnLostDevice();
	void Release();
}