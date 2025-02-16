// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "CustomSceneViewExtension.h"
#include "CustomRenderSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class POSTPROCESS_API UCustomRenderSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
private:
	TSharedPtr<class FCustomSceneViewExtension, ESPMode::ThreadSafe> CustomSceneViewExtension;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
