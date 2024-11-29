// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSDemoPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScoreChanged, AFPSDemoPlayerState*, PlayerState, int32, NewScores, int32, Delta);

/**
 * Score在APlayerState已存在变量，新建Scores变量.
 */
UCLASS()
class FPSDEMO_API AFPSDemoPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Scores", Category="Scores")
	int32 Scores;

	UPROPERTY(BlueprintReadOnly)
	float PersonalRecordTime;

	UFUNCTION()
	void OnRep_Scores(int32 OldScores);

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoresChanged;
	
	/* Checks current record and only sets if better time was passed in. */
	UFUNCTION(BlueprintCallable)
	bool UpdatePersonalRecord(float NewTime);
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScores() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Delta);
};
