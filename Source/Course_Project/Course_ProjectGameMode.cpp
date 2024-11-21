// Copyright Epic Games, Inc. All Rights Reserved.

#include "Course_ProjectGameMode.h"
#include "Course_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACourse_ProjectGameMode::ACourse_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
