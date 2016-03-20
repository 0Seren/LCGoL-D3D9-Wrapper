#pragma once
#include <windows.h>
#include <vector>
#include <fstream>
using OffsetT = DWORD;

template<typename T>
public class DeepPointer {
public:

	DeepPointer(std::vector<OffsetT> offsets) : _offsets(offsets) {}

	bool Update() {
		T* temp = Deref();
		if (!(IsBadReadPtr(temp, sizeof(T)) || temp == nullptr || temp == NULL)) {
			_Current = *temp;
			return true;
		}
		return false;
	}
	T Current() {
		return _Current;
	}
private:
	T* Deref() {
		if (BaseEXEAddress == 0) {
			BaseEXEAddress = (DWORD)GetModuleHandle(nullptr);
		}
		char* final_address = (char*)(((char*)BaseEXEAddress) + _offsets[0]);
		for (size_t i = 1; i < _offsets.size(); i++) {
			final_address = (char*)(*((OffsetT*)final_address) + _offsets[i]);
		}
		return (T*)final_address;
	}
	T _Current;
	DWORD BaseEXEAddress = 0;
	std::vector<OffsetT> _offsets;
};