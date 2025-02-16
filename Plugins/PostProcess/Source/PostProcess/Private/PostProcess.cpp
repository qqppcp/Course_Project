// Copyright Epic Games, Inc. All Rights Reserved.

#include "PostProcess.h"
#include "PostProcessStyle.h"
#include "PostProcessCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"

static const FName PostProcessTabName("PostProcess");

#define LOCTEXT_NAMESPACE "FPostProcessModule"

void FPostProcessModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPostProcessStyle::Initialize();
	FPostProcessStyle::ReloadTextures();

	FPostProcessCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPostProcessCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPostProcessModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPostProcessModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PostProcessTabName, FOnSpawnTab::CreateRaw(this, &FPostProcessModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FPostProcessTabTitle", "PostProcess"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// Shaders is the folder with a private folder inside
	// Requires Projects
	const FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("PostProcess"))->GetBaseDir(), TEXT("Shaders"));
	// Requires RenderCore
	if(!AllShaderSourceDirectoryMappings().Contains(TEXT("/CustomShaders")))
	{
		AddShaderSourceDirectoryMapping(TEXT("/CustomShaders"), PluginShaderDir);
	}
}

void FPostProcessModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FPostProcessStyle::Shutdown();

	FPostProcessCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PostProcessTabName);
}

TSharedRef<SDockTab> FPostProcessModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FPostProcessModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("PostProcess.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FPostProcessModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(PostProcessTabName);
}

void FPostProcessModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FPostProcessCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FPostProcessCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPostProcessModule, PostProcess)