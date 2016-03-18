#pragma once
#include "DeepPointer.h"
public class LCGoL {
private:
	std::vector<OffsetT> CurrentMapV = { 0xCA8E1C };
	std::vector<OffsetT> IsOnEndScreenV = { 0x7C0DD0 };
	std::vector<OffsetT> SpLoadingV = { 0xA84CAC };
	std::vector<OffsetT> MpLoadingV = { 0xCEB5F8 };
	std::vector<OffsetT> MpLoading2V = { 0xCA8D0B };
	std::vector<OffsetT> GameTimeV = { 0xCA8EE4 };
	std::vector<OffsetT> NumPlayersV = { 0xD7F8EC, 0x10 };
	std::vector<OffsetT> RefreshRateV = { 0x0884554, 0x228 };
	std::vector<OffsetT> VSyncPresentationIntervalV = { 0x0884554, 0x22C };
public:
	void UpdateAll();
	bool IsLoading();
	bool ValidSettings();
	LCGoL();

	DeepPointer<std::string> CurrentMap = DeepPointer<std::string>(CurrentMapV);
	DeepPointer<bool> IsOnEndScreen = DeepPointer<bool>(IsOnEndScreenV);
	DeepPointer<byte> SpLoading = DeepPointer<byte>(SpLoadingV);
	DeepPointer<byte> MpLoading = DeepPointer<byte>(MpLoadingV);
	DeepPointer<byte> MpLoading2 = DeepPointer<byte>(MpLoading2V);
	DeepPointer<UINT32> GameTime = DeepPointer<UINT32>(GameTimeV);
	DeepPointer<byte> NumPlayers = DeepPointer<byte>(NumPlayersV);
	DeepPointer<int> RefreshRate = DeepPointer<int>(RefreshRateV);
	DeepPointer<int> VSyncPresentationInterval = DeepPointer<int>(VSyncPresentationIntervalV);
private:
	bool LoadingStart();
	bool LoadingEnd();
	bool isLoading = false;
};