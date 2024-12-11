// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSDemoGameMode.h"

#include "EngineUtils.h"
#include "FPSDemo.h"
#include "FPSDemoAttributeComponent.h"
#include "FPSDemoCharacter.h"
#include "FPSDemoPlayerController.h"
#include "FPSDemoPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "UObject/ConstructorHelpers.h"

template <typename T>
void ShuffleArray(TArray<T>& Array) {
	int32 Size = Array.Num();
	for (int32 i = Size - 1; i > 0; --i) {
		// 随机选择一个索引
		int32 SwapIndex = FMath::RandRange(0, i);

		// 交换元素
		Array.Swap(i, SwapIndex);
	}
}

void AFPSDemoGameMode::OnSpawnActor(FVector SpawnLocation)
{
	AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(SpawnClass, SpawnLocation, FRotator::ZeroRotator);
	if(spawnedActor)
	{
		FVector Scale = spawnedActor->GetActorScale();
		spawnedActor->SetActorScale3D(Scale * FMath::RandRange(1.0f, 1.777f));
		//设置Bonus
		float russian = FMath::RandRange(0.0f, 1.0f);
		if (russian > 0.7f)
		{
			UFPSDemoAttributeComponent* AttributeComponent = spawnedActor->GetComponentByClass<UFPSDemoAttributeComponent>();
			AttributeComponent->SetBonus();
		}
	}
}

void AFPSDemoGameMode::GameEndTimerElapsed()
{
	UGameplayStatics::SetGamePaused(this, true);
	int AllScores = 0;
	TMultiMap<int32, FString> GameScores;
	TArray<AFPSDemoPlayerState*> Players;
	for (AFPSDemoPlayerState* PlayerState : TActorRange<AFPSDemoPlayerState>(GetWorld()))
	{
		AllScores += PlayerState->GetScores();
		GameScores.Add({PlayerState->GetScores(), GetNameSafe(PlayerState)});
		Players.Add(PlayerState);
	}
	FString Msg = FString::Printf(TEXT("All Scores: %d\n"), AllScores);
	for (AFPSDemoPlayerState* PlayerState : Players)
	{
		FString App = FString::Printf(TEXT("Name: %s, Id: %d, Score: %d\n"), *GetNameSafe(PlayerState), PlayerState->GetPlayerId(), PlayerState->GetScores());
		Msg.Append(App);
	}
	// for (TPair<int32, FString> Score : GameScores)
	// {
	// 	FString App = FString::Printf(TEXT("Name: %s, Score: %d\n"), *Score.Value, Score.Key);
	// 	Msg.Append(App);
	// }
	for (AFPSDemoPlayerController* Controller : TActorRange<AFPSDemoPlayerController>(GetWorld()))
	{
		Controller->EndGameMessage = Msg;
		Controller->SetEndGame();
	}
}

void AFPSDemoGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	
	TArray<UFPSDemoAttributeComponent*> FoundActors;
	// 设置场景中的Bonus物体
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		UFPSDemoAttributeComponent* AttributeComponent = Actor->GetComponentByClass<UFPSDemoAttributeComponent>();
		if(AttributeComponent)
		{
			FoundActors.Add(AttributeComponent);
		}
	}
	ExsitsSpawnBots = FoundActors.Num();
	ShuffleArray(FoundActors);
	for (int32 i = 0; i < FMath::Min(ExsitsSpawnBots, InitialBonusCubeCount); ++i)
	{
		FoundActors[i]->SetBonus();
		
	}
}

void AFPSDemoGameMode::OnActorKilled(AActor* VictimActor, AActor* Killer)
{

	ACharacter* character = Cast<ACharacter>(Killer);
	if (character)
	{
		AFPSDemoPlayerState* PlayerState = character->GetPlayerState<AFPSDemoPlayerState>();
		UFPSDemoAttributeComponent* AttributeComponent = VictimActor->GetComponentByClass<UFPSDemoAttributeComponent>();
		if (PlayerState && AttributeComponent)
		{
			UE_LOGFMT(LogTemp, Log, "OnActorKilled: Victim: {victim}, Killer: {killer}, PlayerState: {player}, PlayerId: {playerid}",
				GetNameSafe(VictimActor), GetNameSafe(Killer), GetNameSafe(PlayerState), PlayerState->GetPlayerId());
			PlayerState->AddScore(InitialCubeScore * (AttributeComponent->IsBonus() ? 2 : 1));
			ExsitsSpawnBots--;
		}
		FString Msg = FString::Printf(TEXT("Killer: {%s},PlayerState: %s, Id: %d, Score: %d"),
			*GetNameSafe(Killer), *GetNameSafe(PlayerState), PlayerState->GetPlayerId(), PlayerState->GetScores());
		LogOnScreen(this, Msg, FColor::Red, 4.0f);
	}
	
}

void AFPSDemoGameMode::OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		return;
	}

	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	if (Locations.IsValidIndex(0))
	{
		OnSpawnActor(Locations[0]);
		ExsitsSpawnBots++;
	}
}

void AFPSDemoGameMode::StartPlay()
{
	Super::StartPlay();

	if (AvailableSpawnBots > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AFPSDemoGameMode::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	}
	GetWorldTimerManager().SetTimer(TimerHandle_GameEnd, this, &AFPSDemoGameMode::GameEndTimerElapsed, InitialGameTimeLength, false);
}

void AFPSDemoGameMode::SpawnBotTimerElapsed()
{
	if (AvailableSpawnBots == ExsitsSpawnBots)
	{
		return;
	}
	FEnvQueryRequest Request(SpawnBotQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &AFPSDemoGameMode::OnBotSpawnQueryCompleted);
}


AFPSDemoGameMode::AFPSDemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	InitialGameTimeLength = 120.0f;
	InitialCubeScore = 5;
	InitialBonusCubeCount = 4;
	SpawnTimerInterval = 3.0f;

	AvailableSpawnBots = 20;
	ExsitsSpawnBots = 0;
}
