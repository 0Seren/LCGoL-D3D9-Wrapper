#include "stdafx.h"
#include "GameMemoryPool.h"

bool GameMemoryPool::UpdateAll()
{
	return CurrentMap.Update() &&
		IsOnEndScreen.Update() &&
		NumPlayers.Update() &&
		SpLoading.Update() &&
		MpLoading.Update() &&
		MpLoading2.Update() &&
		GameTime.Update() &&
		RefreshRate.Update() &&
		VSyncPresentationInterval.Update();

}

bool GameMemoryPool::ValidSettings()
{
	if (VSyncPresentationInterval.Update() && RefreshRate.Update() && GameTime.Update()) {
		if (VSyncPresentationInterval.Current() != 0x00000001
			|| (RefreshRate.Current() != 59 && RefreshRate.Current() != 60)
			&& (GameTime.Current() > 0 && RefreshRate.Current() != 0)) { // avoid false detection on game startup and zeroed memory on exit
			return false;
		}
	}
	return true;
}

GameMemoryPool::GameMemoryPool()
{
}

