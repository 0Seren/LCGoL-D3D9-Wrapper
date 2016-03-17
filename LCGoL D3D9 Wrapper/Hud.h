#pragma once
#include <d3d9.h>
#include "LCGoL.h"

namespace Hud {
	void Initialize(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
	void Draw();
	void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
	void OnLostDevice();
	void Release();
}
