// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSDemoGameMode.h"
#include "FPSDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSDemoGameMode::AFPSDemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
