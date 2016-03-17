#pragma once

#include <string>
#include <d3dx9.h>

// Vertex struct that Direct3D needs for drawing simple rectangles for the Text background windows
struct TLVERTEX {
	double x;
	double y;
	double z;
	double rhw;
	D3DCOLOR color;
	double u;
	double v;
};

// Predefined text attach methods
enum attach_method {
	ATTACH_TOP_RIGHT,
	ATTACH_TOP_LEFT,
	ATTACH_TOP_CENTER,
	ATTACH_BOTTOM_RIGHT,
	ATTACH_BOTTOM_LEFT,
	ATTACH_BOTTOM_CENTER,
	ATTACH_LEFT_CENTER,
	ATTACH_RIGHT_CENTER,
	ATTACH_CENTER_CENTER
};

struct block_attributes {
	RECT rect;
	int width;
	int height;
	attach_method attach;
	double x_percent_offset;
	double y_percent_offset;
	int x_pixel_offset;
	int y_pixel_offset;
	DWORD text_format;
	std::wstring text;
	DWORD text_color;
	DWORD block_color;
	int padding;
};

class TextBlock
{
private:
	IDirect3DDevice9* pDevice;
	D3DPRESENT_PARAMETERS* pPP;
	ID3DXFont *pFont;
	IDirect3DVertexBuffer9* vertexBuffer;
	TLVERTEX* vertices;
	block_attributes block;

public:
	TextBlock(IDirect3DDevice9* device_pointer, D3DPRESENT_PARAMETERS* params_pointer);

	void SetFont(LPCWSTR face, INT size, UINT weight, BOOL italic, DWORD quality);
	void SetBlockSize(int width, int height);
	void SetTextFormat(DWORD format);
	/* DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT, DT_TOP, DT_VCENTER
	DT_EXPANDTABS, DT_WORDBREAK, DT_WORD_ELLIPSIS
	*/
	void SetBlockColor(DWORD color);
	void SetTextColor(DWORD color);
	void SetTextPadding(int padding);
	void AttachBlock(attach_method side,
		float x_percent_offset, float y_percent_offset,
		int x_pixel_offset, int y_pixel_offset);
	void SetText(std::wstring text);
	void CalculateBlock();
	void Draw();

	int GetWidth();
	int GetHeight();

	void OnResetDevice(D3DPRESENT_PARAMETERS *params_pointer);
	void OnLostDevice();
	void Release();
};