#pragma once
#include "Stdafx.h"
#include <curl\curl.h>

using table = std::map<std::string, std::map<std::string, int>>;

public class Leaderboard {

public:
	Leaderboard();

	table GetTable();
	void UpdateTableFromOnline();
	void NewBestTime(std::string playerName, std::string levelName, int timeMillis);
	void GetPlayerData(std::string playerName);
	void GetAllLevelData(std::string levelName);
	void GetPlayerLevelData(std::string playerName, std::string levelName);

private:
	table leaderboard;
	const std::string apiKey = SensitiveData::airTableApiKey();
};