#include "stdafx.h"
#include "GameData.h"

bool GameData::UpdateAll()
{
	//memory data
	bool updated = CurrentMap.Update() &&
		IsOnEndScreen.Update() &&
		NumPlayers.Update() &&
		SpLoading.Update() &&
		MpLoading.Update() &&
		MpLoading2.Update() &&
		GameTime.Update() &&
		RefreshRate.Update() &&
		VSyncPresentationInterval.Update();

	//extraneous updates
	if (updated) {
		//loading data
		bool startLoading = ((NumPlayers.Current() == 1 && SpLoading.Current() == 1 && SpLoading.Old() != 1 && !IsOnEndScreen.Current())
			|| (NumPlayers.Current() > 1 && MpLoading.Current() == 2 && MpLoading.Old() == 7)   // new game
			|| (NumPlayers.Current() > 1 && MpLoading2.Current() == 1 && MpLoading2.Old() == 0) // death
			|| (NumPlayers.Current() > 1 && MpLoading.Current() == 2 && MpLoading.Old() == 1));
		bool endLoading = ((NumPlayers.Current() == 1 && SpLoading.Current() != 1 && SpLoading.Old() == 1 && !IsOnEndScreen.Current())
			|| (NumPlayers.Current() > 1 && MpLoading.Current() == 1 && MpLoading.Old() == 3));

		if (_inLoad) {
			_inLoad = !endLoading;
		}
		else {
			_inLoad = startLoading;
		}

		//settings data
		if (VSyncPresentationInterval.Update() && RefreshRate.Update() && GameTime.Update()) {
			if (VSyncPresentationInterval.Current() != 0x00000001
				|| (RefreshRate.Current() != 59 && RefreshRate.Current() != 60)
				&& (GameTime.Current() > 0 && RefreshRate.Current() != 0)) { // avoid false detection on game startup and zeroed memory on exit
				_validsettings = false;
			}
		}
		_validsettings = true;
	}
	return updated;
}

bool GameData::ValidSettings()
{
	return _validsettings;
}

bool GameData::IsLoading()
{
	return _inLoad;
}

GameData::GameData()
{
}

