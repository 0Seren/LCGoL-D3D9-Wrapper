#pragma once
#include "DeepPointer.h"
public class GameData {

public:
	bool UpdateAll();
	bool ValidSettings();
	bool IsLoading();
	GameData();

	DeepPointer<char> CurrentMap = DeepPointer<char>{ 0xCA8E1C };
	DeepPointer<bool> IsOnEndScreen = DeepPointer<bool>{ 0x7C0DD0 };
	DeepPointer<byte> SpLoading = DeepPointer<byte>{ 0xA84CAC };
	DeepPointer<byte> MpLoading = DeepPointer<byte>{ 0xCEB5F8 };
	DeepPointer<byte> MpLoading2 = DeepPointer<byte>{ 0xCA8D0B };
	DeepPointer<UINT32> GameTime = DeepPointer<UINT32>{ 0xCA8EE4 };
	DeepPointer<byte> NumPlayers = DeepPointer<byte>{ 0xD7F8EC, 0x10 };
	DeepPointer<int> RefreshRate = DeepPointer<int>{ 0x0884554, 0x228 };
	DeepPointer<int> VSyncPresentationInterval = DeepPointer<int>{ 0x0884554, 0x22C };
	
private:
	bool _inLoad = false;
	bool _validsettings = true;
};