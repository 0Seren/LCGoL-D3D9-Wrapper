#include "Stdafx.h"
#include "Overlay.h"
#include "LCGoL.h"
#include <string>

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
		D3DXCreateFont(device, 32, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 5, DEFAULT_PITCH | FF_DONTCARE, L"Verdana", &pFont);
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
			game.GameTime.Update();
			if (!game.GameTime.CurrentNull()) {
				char *text = new char[5 + 30];
				sprintf_s(text, 5 + 30, "%u", game.GameTime.Current());
				pFont->DrawTextA(NULL, text, -1, &rectangle, DT_CENTER, textColor);
				delete text;
			}
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