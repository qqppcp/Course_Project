// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "FPSDemoAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class FPSDEMO_API UFPSDemoAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
public:
	AActor* GetClosetEnemy() const;
};
