#pragma once
#include "Stdafx.h"
using OffsetT = DWORD;

template<typename T>
public class DeepPointer {
public:

	DeepPointer(std::initializer_list<OffsetT> offsets) : DeepPointer(nullptr, offsets){}

	DeepPointer(LPCSTR module, std::initializer_list<OffsetT> offsets) : _offsets(offsets), _module(module){}

	bool Update() {
		try {
			T* temp = Deref();
			if (!(IsBadReadPtr(temp, sizeof(T)) || temp == nullptr || temp == NULL)) {
				_Old = _Current;
				_Current = temp;
				return true;
			}
			return false;
		}
		catch (...) {
			return false;
		}
	}

	bool Changed() {
		return _Current != _Old || (_Current != nullptr && Old != nullptr && *_Current != *_Old);
	}

	T* Pointer() {
		return _Current;
	}

	T Current() {
		return *_Current;
	}

	T Old() {
		return *_Old;
	}

private:
	T* Deref() {
		DWORD BaseEXEAddress = (DWORD)GetModuleHandleA(_module);
		char* final_address = (char*)((char*)BaseEXEAddress + _offsets[0]);
		for (size_t i = 1; i < _offsets.size(); i++) {
			final_address = (char*)(*(OffsetT*)final_address + _offsets[i]);
		}
		return (T*)final_address;
	}

	T* _Current = nullptr;
	T* _Old = nullptr;
	std::vector<OffsetT> _offsets;
	LPCSTR _module;
};