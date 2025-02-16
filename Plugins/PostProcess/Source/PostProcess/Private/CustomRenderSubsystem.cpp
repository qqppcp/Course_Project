// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomRenderSubsystem.h"

#include "CustomRenderSubsystem.h"


void UCustomRenderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CustomSceneViewExtension = FSceneViewExtensions::NewExtension<FCustomSceneViewExtension>();
	
}

void UCustomRenderSubsystem::Deinitialize()
{
	Super::Deinitialize();
	CustomSceneViewExtension.Reset();
	CustomSceneViewExtension = nullptr;
}
