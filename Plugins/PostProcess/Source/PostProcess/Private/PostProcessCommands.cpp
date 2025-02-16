// Copyright Epic Games, Inc. All Rights Reserved.

#include "PostProcessCommands.h"

#define LOCTEXT_NAMESPACE "FPostProcessModule"

void FPostProcessCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "PostProcess", "Bring up PostProcess window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
