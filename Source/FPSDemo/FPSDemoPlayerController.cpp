// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSDemoPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AFPSDemoPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void AFPSDemoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AFPSDemoPlayerController::OnRep_EndGame()
{
	EndGame();
}

void AFPSDemoPlayerController::EndGame()
{
	if (!IsLocalController())
	{
		return;
	}
	UGameplayStatics::SetGamePaused(this, true);
	EndMenuInstance = CreateWidget(this, EndMenuClass);
	if (EndMenuInstance)
	{
		EndMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AFPSDemoPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSDemoPlayerController, bIsGameEnd);
	DOREPLIFETIME(AFPSDemoPlayerController, EndGameMessage);
}
