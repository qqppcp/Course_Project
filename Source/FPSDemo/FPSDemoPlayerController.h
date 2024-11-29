// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSDemoPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class FPSDEMO_API AFPSDemoPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EndMenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> EndMenuInstance;

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();
	
	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing="OnRep_EndGame")
	bool bIsGameEnd;

	UFUNCTION()
	void OnRep_EndGame();
	
	void EndGame();	
public:
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	FString EndGameMessage;
	
	void SetEndGame()
	{
		bIsGameEnd = true;
		if (IsLocalController())
		{
			EndGame();
		}
	};
	
};
