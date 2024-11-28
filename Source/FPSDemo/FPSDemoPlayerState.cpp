// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemoPlayerState.h"

#include "Net/UnrealNetwork.h"

void AFPSDemoPlayerState::OnRep_Scores()
{
	
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