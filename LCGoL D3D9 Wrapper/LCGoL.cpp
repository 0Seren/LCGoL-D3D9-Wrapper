#include "stdafx.h"
#include "LCGoL.h"
#include "DeepPointer.h"

void LCGoL::UpdateAll()
{
	CurrentMap.UpdateString();
	IsOnEndScreen.Update();
	NumPlayers.Update();
	SpLoading.Update();
	MpLoading.Update();
	MpLoading2.Update();
	GameTime.Update();
	RefreshRate.Update();
	VSyncPresentationInterval.Update();

}

LCGoL::LCGoL()
{
}
