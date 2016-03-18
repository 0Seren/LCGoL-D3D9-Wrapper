#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
using OffsetT = DWORD;



template<typename T>
public class DeepPointer {
public:

	DeepPointer(std::vector<OffsetT> offsets) {
		_offsets = offsets;
	}

	void Update() {
		if (_Old != NULL) {
			delete _Old;
		}
		if (_Current != NULL) {
			_Old = _Current;
		}
		_Current = Deref();
	}

	bool OldNull() {
		return _Old == NULL || _Old == nullptr || IsBadReadPtr(_Old, 4);
	}

	bool CurrentNull() {
		return _Current == NULL || _Current == nullptr || IsBadReadPtr(_Old, 4);
	}

	T Old() {
		return *_Old;
	}
	T Current() {
		return *_Current;
	}
private:
	T* Deref() {
		if (BaseEXEAddress == 0) {
			BaseEXEAddress = (DWORD)GetModuleHandle(nullptr);
		}
		final_address = (char*)BaseEXEAddress;
		for (OffsetT const& offset : _offsets) {
			final_address = (char*)(final_address + offset);
		}
		return (T*)final_address;
	}
	char* final_address;
	std::vector<OffsetT> _offsets;
	T *_Old = NULL;
	T *_Current = NULL;
	DWORD BaseEXEAddress = 0;
	std::ofstream myfile;
};