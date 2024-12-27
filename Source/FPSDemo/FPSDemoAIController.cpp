// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemoAIController.h"

void AFPSDemoAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
