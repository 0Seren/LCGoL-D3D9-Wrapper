#include "stdafx.h"
#include "LCGoL.h"

void LCGoL::UpdateAll()
{
	CurrentMap.Update();
	IsOnEndScreen.Update();
	NumPlayers.Update();
	SpLoading.Update();
	MpLoading.Update();
	MpLoading2.Update();
	GameTime.Update();
	RefreshRate.Update();
	VSyncPresentationInterval.Update();

}

bool LCGoL::IsLoading()
{
	NumPlayers.Update();
	SpLoading.Update();
	MpLoading.Update();
	MpLoading2.Update();
	IsOnEndScreen.Update();

	if (isLoading) {
		isLoading = !LoadingEnd();
	}
	else {
		isLoading = LoadingStart();
	}
	return isLoading;
}

bool LCGoL::ValidSettings()
{
	VSyncPresentationInterval.Update();
	RefreshRate.Update();
	GameTime.Update();

	if	(	 VSyncPresentationInterval.Current() != 0x00000001
		|| ( RefreshRate.Current() != 59		 && RefreshRate.Current() != 60)
		&& ( GameTime.Current() > 0				 && RefreshRate.Current() != 0 )) { // avoid false detection on game startup and zeroed memory on exit
			return false;
	}
	return true;
}

bool LCGoL::LoadingStart()
{
	return	((  NumPlayers.Current() == 1 && SpLoading.Current() == 1  && SpLoading.Old() != 1 && !IsOnEndScreen.Current())		//single player loading
			|| (NumPlayers.Current() > 1  && MpLoading.Current() == 2  && MpLoading.Old() == 7							  )		// new game
			|| (NumPlayers.Current() > 1  && MpLoading2.Current() == 1 && MpLoading2.Old() == 0							  )		// death
			|| (NumPlayers.Current() > 1  && MpLoading.Current() == 2  && MpLoading.Old() == 1							  ));	// change level
}

bool LCGoL::LoadingEnd()
{
	return (   (NumPlayers.Current() == 1 && SpLoading.Current() != 1 && SpLoading.Old() == 1 && !IsOnEndScreen.Current())
		   ||  (NumPlayers.Current() > 1  && MpLoading.Current() == 1 && MpLoading.Old() == 3							 ));
}

LCGoL::LCGoL()
{
}

