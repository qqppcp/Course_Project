// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSDemoAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSDEMO_API AFPSDemoAIController : public AAIController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	virtual void BeginPlay() override;
};
