#include "Stdafx.h"
#include "Overlay.h"
#include "GameData.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

namespace Overlay {
	D3DDEVICE_CREATION_PARAMETERS cparams;
	LPD3DXFONT pFont;
	GameData memory;
	D3DCOLOR textColor;
	D3DCOLOR rectColor;
	IDirect3DDevice9 *device;
	IDirect3DStateBlock9 *state;
	D3DRECT rectangle;
	RECT textrect;
	RECT gameWindow;
	double heightModifier;
	double widthModifier;

	void Initialize(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPP) {
		device = pDevice;
		device->GetCreationParameters(&cparams);
		GetClientRect(cparams.hFocusWindow, &gameWindow);
		heightModifier = gameWindow.bottom / 1080.0;
		widthModifier = gameWindow.right / 1920.0;
		D3DXCreateFont(device, (int)(40 * heightModifier * (heightModifier + .85 * (1-heightModifier))), 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 5, DEFAULT_PITCH | FF_DONTCARE, L"Verdana", &pFont);
		textColor = D3DXCOLOR(207, 210, 232, 1);
		rectColor = D3DCOLOR_XRGB(32, 32, 32);
		memory = GameData();

		int left = (int)(855 * widthModifier);
		int top = (int)(304 * heightModifier);
		int right = (int)(left + 285 * widthModifier);
		int bottom = (int)(top + 50 * heightModifier);
		rectangle = { left, top, right, bottom};
		textrect = { left, top, right, bottom};
	}

	void Draw() {
		device->CreateStateBlock(D3DSBT_ALL, &state);
		state->Capture();
		device->SetVertexShader(nullptr);
		// draw
		try {
			if (memory.UpdateAll()) {
				device->GetCreationParameters(&cparams);
				GetClientRect(cparams.hFocusWindow, &gameWindow);
				if (gameWindow.bottom / 1080.0 != heightModifier || gameWindow.right / 1920.0 != widthModifier) {
					heightModifier = gameWindow.bottom / 1080.0;
					widthModifier = gameWindow.right / 1920.0;
					D3DXCreateFont(device, (int)(40 * heightModifier * (heightModifier + .75 * (1 - heightModifier))), 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 5, DEFAULT_PITCH | FF_DONTCARE, L"Verdana", &pFont);
					int left = (int)(855 * widthModifier);
					int top = (int)(304 * heightModifier);
					int right = (int)(left + 285 * widthModifier);
					int bottom = (int)(top + 50 * heightModifier);
					rectangle = { left, top, right, bottom };
					textrect = { left, top, right, bottom };
				}
				if (memory.IsOnEndScreen.Current()) {
					DrawIGTRectangle();
					DrawIGT();
				}
			}
		}
		catch (...) {
			pFont->DrawTextA(NULL, "Exception", -1, &textrect, DT_CENTER, textColor);
		}
		//release
		state->Apply();
		state->Release();
	}

	void DrawIGTRectangle() {
		device->Clear(1, &rectangle, D3DCLEAR_TARGET, rectColor, 0, 0);
	}

	void DrawIGT() {
		UINT32 igt = memory.GameTime.Current();
		UINT32 millis = igt % 1000;
		UINT32 seconds = (igt / 1000) % 60;
		UINT32 minutes = ((igt / (1000 * 60)) % 60);
		UINT32 hours = ((igt / (1000 * 60 * 60)) % 24);

		char timer_text[128];
		sprintf_s(timer_text, 128, "%01u:%02u:%02u.%03u", hours, minutes, seconds, millis);
		pFont->DrawTextA(NULL, timer_text, -1, &textrect, DT_CENTER | DT_VCENTER, textColor);
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