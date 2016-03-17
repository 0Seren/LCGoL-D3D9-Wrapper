#pragma once
#include <windows.h>
#include <vector>
using OffsetT = UINT32;



template<typename T>
public class DeepPointer {
public:
	DeepPointer(std::vector<OffsetT> offsets) {
		_offsets = offsets;
		final_address = 0;
		for (OffsetT const& offset : _offsets) {
			final_address = (char*)(final_address + offset);
		}
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
		return _Old == NULL;
	}

	bool CurrentNull() {
		return _Current == NULL;
	}

	T Old() {
		return *_Old;
	}
	T Current() {
		return *_Current;
	}
private:
	T* Deref() {
		/* This may be needed. We'll see. Unforunately it's super slow. (drops my fps to 36 from 100+) Will probably end up running most things in a separate thread anyways.
		if (_offsets.size() > 1) {
			final_address = 0;
			for (OffsetT const& offset : _offsets) {
				final_address = (char*)(final_address + offset);
			}
		}*/
		return (T*)final_address;
	}
	char* final_address;
	std::vector<OffsetT> _offsets;
	T *_Old = NULL;
	T *_Current = NULL;
};