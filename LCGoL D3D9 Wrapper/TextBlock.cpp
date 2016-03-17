#include "stdafx.h"
#include "TextBlock.h"
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

TextBlock::TextBlock(IDirect3DDevice9* device_pointer, D3DPRESENT_PARAMETERS* params_pointer)
{
	pDevice = device_pointer;
	pDevice->CreateVertexBuffer
		(sizeof(TLVERTEX) * 4, NULL, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &vertexBuffer, NULL);
	pPP = params_pointer;
}

void TextBlock::SetFont(LPCWSTR face, INT size, UINT weight, BOOL italic, DWORD quality)
{
	D3DXCreateFont(pDevice,					// D3D device
		size,					// Height
		0,                     // Width
		weight,				// Weight
		1,                     // MipLevels, 0 = autogen mipmaps
		italic,                // Italic
		DEFAULT_CHARSET,       // CharSet
		OUT_DEFAULT_PRECIS,    // OutputPrecision
		quality,				// Quality
		DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
		face,					// pFaceName
		&pFont);				// ppFont
}

void TextBlock::SetBlockSize(int width, int height)
{
	block.width = width;
	block.height = height;
}

void TextBlock::SetTextFormat(DWORD format)
{
	block.text_format = format;
}

void TextBlock::AttachBlock(attach_method attach,
	float x_percent_offset, float y_percent_offset,
	int x_pixel_offset, int y_pixel_offset)
{
	block.attach = attach;
	block.x_percent_offset = x_percent_offset;
	block.y_percent_offset = y_percent_offset;
	block.x_pixel_offset = x_pixel_offset;
	block.y_pixel_offset = y_pixel_offset;
}

void TextBlock::SetText(std::wstring text)
{
	block.text = text;
}

void TextBlock::SetBlockColor(DWORD color)
{
	block.block_color = color;
}

void TextBlock::SetTextColor(DWORD color)
{
	block.text_color = color;
}

void TextBlock::SetTextPadding(int padding)
{
	block.padding = padding;
}

int TextBlock::GetHeight()
{
	return block.height;
}

int TextBlock::GetWidth()
{
	return block.width;
}

void TextBlock::CalculateBlock()
{
	SetRect(&block.rect, block.width, 0, 0, 0);
	block.height = pFont->DrawText(NULL, block.text.c_str(), -1, &block.rect, DT_CALCRECT | block.text_format, 0);

	// add padding for the actual block around the text
	block.height += block.padding * 2;

	int MAXX = pPP->BackBufferWidth;
	int MAXY = pPP->BackBufferHeight;

	// calculate vertices
	switch (block.attach)
	{
	case ATTACH_TOP_RIGHT:
		block.rect.left = MAXX - block.width;
		block.rect.top = 0;
		block.rect.right = MAXX;
		block.rect.bottom = block.height;
		break;

	case ATTACH_TOP_LEFT:
		block.rect.left = 0;
		block.rect.top = 0;
		block.rect.right = block.width;
		block.rect.bottom = block.height;
		break;

	case ATTACH_TOP_CENTER:
		block.rect.left = MAXX / 2 - block.width / 2;
		block.rect.top = 0;
		block.rect.right = MAXX / 2 + block.width / 2;
		block.rect.bottom = block.height;
		break;

	case ATTACH_BOTTOM_RIGHT:
		block.rect.left = MAXX - block.width;
		block.rect.top = MAXY - block.height;
		block.rect.right = MAXX;
		block.rect.bottom = MAXY;
		break;

	case ATTACH_BOTTOM_LEFT:
		block.rect.left = 0;
		block.rect.top = MAXY - block.height;
		block.rect.right = block.width;
		block.rect.bottom = MAXY;
		break;

	case ATTACH_BOTTOM_CENTER:
		block.rect.left = MAXX / 2 - block.width / 2;
		block.rect.top = MAXY - block.height;
		block.rect.right = MAXX / 2 + block.width / 2;
		block.rect.bottom = MAXY;
		break;

	case ATTACH_LEFT_CENTER:
		block.rect.left = 0;
		block.rect.top = MAXY / 2 - block.height / 2;
		block.rect.right = block.width;
		block.rect.bottom = MAXY / 2 + block.height / 2;
		break;

	case ATTACH_RIGHT_CENTER:
		block.rect.left = MAXX - block.width;
		block.rect.top = MAXY / 2 - block.height / 2;
		block.rect.right = MAXX;
		block.rect.bottom = MAXY / 2 + block.height / 2;
		break;

	case ATTACH_CENTER_CENTER:
		block.rect.left = MAXX / 2 - block.width / 2;
		block.rect.top = MAXY / 2 - block.height / 2;
		block.rect.right = MAXX / 2 + block.width / 2;
		block.rect.bottom = MAXY / 2 + block.height / 2;
		break;
	}

	// shift the block
	block.rect.left += static_cast<long>(block.x_percent_offset / 100.0*MAXX + block.x_pixel_offset);
	block.rect.top += static_cast<long>(block.y_percent_offset / 100.0*MAXY + block.y_pixel_offset);
	block.rect.right += static_cast<long>(block.x_percent_offset / 100.0*MAXX + block.x_pixel_offset);
	block.rect.bottom += static_cast<long>(block.y_percent_offset / 100.0*MAXY + block.y_pixel_offset);

	// convert block to raw vertices
	vertexBuffer->Lock(0, 0, (void**)&vertices, NULL);

	vertices[0].x = block.rect.left;
	vertices[0].y = block.rect.top;
	vertices[0].z = 0; vertices[0].rhw = 1.0f; vertices[0].u = 0; vertices[0].v = 0; vertices[0].color = block.block_color;

	vertices[1].x = block.rect.right;
	vertices[1].y = block.rect.top;
	vertices[1].z = 0; vertices[1].rhw = 1.0f; vertices[1].u = 1; vertices[1].v = 0; vertices[1].color = block.block_color;

	DWORD bottom_color = D3DXCOLOR(0, 0, 0, 0);

	vertices[2].x = block.rect.right;
	vertices[2].y = block.rect.bottom;
	vertices[2].z = 0; vertices[2].rhw = 1.0f; vertices[2].u = 1; vertices[2].v = 1; vertices[2].color = bottom_color;

	vertices[3].x = block.rect.left;
	vertices[3].y = block.rect.bottom;
	vertices[3].z = 0; vertices[3].rhw = 1.0f; vertices[3].u = 0; vertices[3].v = 1; vertices[3].color = bottom_color;

	vertexBuffer->Unlock();
}

void TextBlock::Draw()
{
	// set appropriate render states
	pDevice->SetVertexShader(NULL);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// draw block
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// add padding for the text
	RECT text_rect;
	SetRect(&text_rect,
		block.rect.left + block.padding,
		block.rect.top + block.padding,
		block.rect.right - block.padding,
		block.rect.bottom - block.padding);

	// draw text
	char block_text[MAX_PATH];
	wcstombs_s(NULL, block_text, block.text.c_str(), MAX_PATH);
	pFont->DrawTextA(NULL, block_text, -1, &text_rect, block.text_format, block.text_color);
}

void TextBlock::OnResetDevice(D3DPRESENT_PARAMETERS *params_pointer)
{
	if (pFont) pFont->OnResetDevice();
	pPP = params_pointer;
}

void TextBlock::OnLostDevice()
{
	if (pFont) pFont->OnLostDevice();
}

void TextBlock::Release()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(vertexBuffer);
}