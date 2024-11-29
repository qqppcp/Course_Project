// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameModeBase.h"
#include "FPSDemoGameMode.generated.h"

struct FEnvQueryResult;

UCLASS(minimalapi)
class AFPSDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:

	int32 AvailableSpawnBots;
	int32 ExsitsSpawnBots;

	// 物体生成间隔和生成函数
	FTimerHandle TimerHandle_SpawnBots;
	void SpawnBotTimerElapsed();
	void OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);
	void OnSpawnActor(FVector SpawnLocation);
	
	// 游戏时长
	FTimerHandle TimerHandle_GameEnd;
	void GameEndTimerElapsed();

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Info")
	TObjectPtr<UEnvQuery> SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<AActor> SpawnClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSetting")
	float InitialGameTimeLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSetting")
	int32 InitialCubeScore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSetting")
	int32 InitialBonusCubeCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameSetting")
	float SpawnTimerInterval;
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	virtual void StartPlay() override;
	AFPSDemoGameMode();
};



