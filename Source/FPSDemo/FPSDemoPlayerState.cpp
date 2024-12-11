// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemoPlayerState.h"

#include "FPSDemo.h"
#include "Net/UnrealNetwork.h"

void AFPSDemoPlayerState::OnRep_Scores(int32 OldScores)
{
	FString Msg = FString::Printf(TEXT("Nama: %s, Id: %d, Score: %d"), *GetNameSafe(this), GetPlayerId(), this->GetScores());
	LogOnScreen(this, Msg, FColor::Red, 4.0f);
	OnScoresChanged.Broadcast(this, Scores, Scores - OldScores);
}

bool AFPSDemoPlayerState::UpdatePersonalRecord(float NewTime)
{
	if (NewTime > PersonalRecordTime)
	{
		float OldRecord = PersonalRecordTime;

		PersonalRecordTime = NewTime;

		return true;
	}

	return false;
}

int32 AFPSDemoPlayerState::GetScores() const
{
	return Scores;
}

void AFPSDemoPlayerState::AddScore(int32 Delta)
{
	Scores += Delta;
	OnScoresChanged.Broadcast(this, Scores, Delta);
}

void AFPSDemoPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSDemoPlayerState, Scores);
}