#pragma once
#include "DeepPointer.h"

public class LCGoL {
public:
	void UpdateAll();
	bool IsLoading();
	bool ValidSettings();
	LCGoL();
	DeepPointer<std::string> CurrentMap = DeepPointer<std::string>(std::vector<OffsetT>(0xCA8E1C), process_name);
	DeepPointer<bool> IsOnEndScreen = DeepPointer<bool>(std::vector<OffsetT>(0x7C0DD0), process_name);
	DeepPointer<byte> SpLoading = DeepPointer<byte>(std::vector<OffsetT>(0xA84CAC), process_name);
	DeepPointer<byte> MpLoading = DeepPointer<byte>(std::vector<OffsetT>(0xCEB5F8), process_name);
	DeepPointer<byte> MpLoading2 = DeepPointer<byte>(std::vector<OffsetT>(0xCA8D0B), process_name);
	DeepPointer<UINT32> GameTime = DeepPointer<UINT32>(std::vector<OffsetT>(0xCA8EE4), process_name);
	DeepPointer<byte> NumPlayers = DeepPointer<byte>(std::vector<OffsetT>(0xD7F8EC, 0x10), process_name);
	DeepPointer<int> RefreshRate = DeepPointer<int>(std::vector<OffsetT>(0x0884554, 0x228), process_name);
	DeepPointer<int> VSyncPresentationInterval = DeepPointer<int>(std::vector<OffsetT>(0x0884554, 0x22C), process_name);
private:
	bool LoadingStart();
	bool LoadingEnd();
	bool isLoading = false;
	char* process_name = "lcgol.exe";
};