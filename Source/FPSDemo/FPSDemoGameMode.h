// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSDemoGameMode.generated.h"

UCLASS(minimalapi)
class AFPSDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:

	int32 AvailableSpawnBots;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<AActor> SpawnClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSetting")
	float InitialGameTimeLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSetting")
	int32 InitialCubeScore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSetting")
	int32 InitialBonusCubeCount;

	void OnSpawnActor(FVector SpawnLocation);
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	AFPSDemoGameMode();
};



