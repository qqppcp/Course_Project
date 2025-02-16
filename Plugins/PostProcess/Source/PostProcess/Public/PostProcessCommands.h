// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PostProcessStyle.h"

class FPostProcessCommands : public TCommands<FPostProcessCommands>
{
public:

	FPostProcessCommands()
		: TCommands<FPostProcessCommands>(TEXT("PostProcess"), NSLOCTEXT("Contexts", "PostProcess", "PostProcess Plugin"), NAME_None, FPostProcessStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};