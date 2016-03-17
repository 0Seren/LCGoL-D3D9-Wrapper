#include "Stdafx.h"
#include "Overlay.h"
#include "LCGoL.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

namespace Overlay {
	LPD3DXFONT pFont;
	LCGoL game;
	D3DCOLOR textColor;
	IDirect3DDevice9 *device;
	IDirect3DStateBlock9 *state;
	RECT rectangle;

	void Initialize(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPP) {
		device = pDevice;
		D3DXCreateFont(device, 16, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 5, DEFAULT_PITCH | FF_DONTCARE, L"Verdana", &pFont);
		textColor = D3DXCOLOR(207, 210, 232, 1);
		game = LCGoL();

		SetRect(&rectangle, 300, 0, 400, 100);

	}

	void Draw() {
		device->CreateStateBlock(D3DSBT_ALL, &state);
		state->Capture();
		device->SetVertexShader(nullptr);
		// draw
		try {
			game.CurrentMap.Update();
			if (!game.CurrentMap.CurrentNull()) {
				char *text = new char[5 + game.CurrentMap.Current().length()];
				sprintf_s(text, 5 + game.CurrentMap.Current().length(), "Map: %s", game.CurrentMap.Current().c_str());
				pFont->DrawTextA(NULL, text, -1, &rectangle, DT_CENTER, textColor);
				delete text;
			}
			pFont->DrawTextA(NULL, "No Data", -1, &rectangle, DT_CENTER, textColor);
		}
		catch (...) {
			pFont->DrawTextA(NULL, "Exception", -1, &rectangle, DT_CENTER, textColor);
		}
		//release
		state->Apply();
		state->Release();
	}

	void OnResetDevice(D3DPRESENT_PARAMETERS * _pPP) {
		if (pFont) pFont->OnResetDevice();
	}

	void OnLostDevice() {
		if (pFont) pFont->OnLostDevice();
	}

	void Release() {
		SAFE_RELEASE(pFont);
	}
}