#pragma once
#include "stdafx.h"
namespace MemoryProtectionHack {
	void Initialize();
	void memcpy_protected(void* dest, const void* src, std::size_t size);
}