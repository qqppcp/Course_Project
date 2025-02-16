// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemoAIPerceptionComponent.h"

#include "AIController.h"
#include "Perception/AISense_Sight.h"

AActor* UFPSDemoAIPerceptionComponent::GetClosetEnemy() const
{
	// TArray<AActor*> PerciveActors;
	// GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerciveActors);
	//
	// if (PerciveActors.Num() == 0)
	// {
	// 	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerciveActors);
	// 	if (PerciveActors.Num() == 0) return nullptr;
	// }
	//
	// const auto Controller = Cast<AAIController>(GetOwner());
	// if (Controller == nullptr) return nullptr;
	// const auto Pawn = Controller->GetPawn();
	// if (Pawn == nullptr) return nullptr;
	//
	// float ClosetDistance = MAX_FLT;
	// AActor* ClosetActor = nullptr;
	// for (const auto PerciveActor : PerciveActors)
	// {
	// 	
	// }
	return nullptr;
}
