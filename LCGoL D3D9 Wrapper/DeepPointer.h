#pragma once
#include <windows.h>
#include <vector>
#include <TlHelp32.h>
#include <string>

#define UNINITIALIZED NULL
using OffsetT = UINT32;

template<typename T>
public class DeepPointer {
public:
	DeepPointer(std::vector<OffsetT> offsets, std::wstring process_name = L"lcgol.exe") {
		_process_name = process_name;
		_process_id = FindProcessId();
		_base_address = FindExeAddress() + offsets[0];
		offsets.erase(offsets.begin());
		_process_handle = OpenProcess(PROCESS_VM_READ, TRUE, _process_id);
		_offsets = offsets;
	}
	void Update() {
		_Old = _New;
		_New = Deref();
	}
	void UpdateString() {
		_Old = _New;
		_New = DerefString();
	}
	T Old() {
		return _Old;
	}
	T New() {
		return _New;
	}
private:
	T Deref() {
		OffsetT address = FindOffsetAddress();
		T value;
		ReadProcessMemory(_process_handle, (LPCVOID)address, &value, sizeof(value), NULL);
		return value;
	}
	std::string DerefString(int numcharacters = 256) {
		OffsetT address = FindOffsetAddress();
		char* value = new char[numcharacters];
		ReadProcessMemory(_process_handle, (LPCVOID)address, value, sizeof(value), NULL);
		return value;
	}
	DWORD FindProcessId() {
		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof(processInfo);

		HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (processesSnapshot == INVALID_HANDLE_VALUE)
			return 0;

		Process32First(processesSnapshot, &processInfo);
		if (!_process_name.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}

		while (Process32Next(processesSnapshot, &processInfo))
		{
			if (!_process_name.compare(processInfo.szExeFile))
			{
				CloseHandle(processesSnapshot);
				return processInfo.th32ProcessID;
			}
		}

		CloseHandle(processesSnapshot);
		return 0;
	}
	OffsetT FindExeAddress() {
		OffsetT baseAddress = UNINITIALIZED;
		HANDLE moduleSnapshotHandle_ = INVALID_HANDLE_VALUE;
		MODULEENTRY32 moduleEntry_;

		/* Take snapshot of all the modules in the process */
		moduleSnapshotHandle_ = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _process_id);

		/* Snapshot failed */
		if (moduleSnapshotHandle_ == INVALID_HANDLE_VALUE) {
			return baseAddress;
		}

		/* Size the structure before usage */
		moduleEntry_.dwSize = sizeof(MODULEENTRY32);

		/* Retrieve information about the first module */
		if (!Module32First(moduleSnapshotHandle_, &moduleEntry_)) {
			CloseHandle(moduleSnapshotHandle_);
			return baseAddress;
		}

		/* Find base address */
		/* Find module of the executable */
		do {

			/* Compare the name of the process to the one we want */
			char temp_moduleEntry[MAX_PATH];
			wcstombs(temp_moduleEntry, moduleEntry_.szModule, MAX_PATH);
			char temp_process_name[MAX_PATH];
			wcstombs(temp_process_name, _process_name.c_str(), MAX_PATH);

			if (!strcmp(temp_moduleEntry, temp_process_name)) {  //?
																 /* Save the processID and break out */
				baseAddress = (unsigned int)moduleEntry_.modBaseAddr;
				break;
			}

		} while (Module32Next(moduleSnapshotHandle_, &moduleEntry_));

		/* Found module and base address successfully */
		CloseHandle(moduleSnapshotHandle_);
		return baseAddress;
	}
	OffsetT FindOffsetAddress() {
		OffsetT address = _base_address;
		OffsetT offset = 0;
		for (std::vector<int>::size_type i = 0; i != _offsets.size(); i++) {
			offset = _offsets[i];
			ReadProcessMemory(_process_handle, (LPCVOID)address, &address, 4, NULL);
			address += offset;
		}
		return address;
	}
	DWORD _process_id;
	std::vector<OffsetT> _offsets;
	std::wstring _process_name;
	OffsetT _base_address;
	HANDLE _process_handle;
	T _Old;
	T _New;
};