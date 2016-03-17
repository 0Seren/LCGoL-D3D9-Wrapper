#pragma once
#include <d3dx9.h>

namespace Overlay {
	void Initialize(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
	void Draw();
	void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
	void OnLostDevice();
	void Release();
}