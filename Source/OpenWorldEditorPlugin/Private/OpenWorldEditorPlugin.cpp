// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenWorldEditorPlugin.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeActions/AssetTypeActions_OWPathAsset.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "OpenWorldEditorPluginStyle.h"
#include "Modes/PathAsset/OWPathAsset_EdModeCommands.h"
#include "Modes/PathAsset/Tools/OWPathAssetSelectTool.h"

#define LOCTEXT_NAMESPACE "FOpenWorldEditorPluginModule"

void FOpenWorldEditorPluginModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FOpenWorldEditorPluginModule::OnPostEngineInit);

	FOpenWorldEditorPluginStyle::Get();
	FOWPathAsset_EdModeCommands::Register();

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	OpenWorldAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("OpenWorld")), LOCTEXT("OpenWorldAssetCategory", "Open World"));

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_OWPathAsset(OpenWorldAssetCategoryBit)));
}

void FOpenWorldEditorPluginModule::OnPostEngineInit()
{
	FOWPathAssetSelectToolActionCommands::Register();
}

void FOpenWorldEditorPluginModule::ShutdownModule()
{
	FCoreDelegates::OnPostEngineInit.RemoveAll(this);

	FOWPathAssetSelectToolActionCommands::Unregister();
	FOWPathAsset_EdModeCommands::Unregister();
	UnRegisterAssetTypeAction();
}

void FOpenWorldEditorPluginModule::UnRegisterAssetTypeAction()
{
	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
	if (AssetToolsModule != nullptr) {
		IAssetTools& AssetTools = AssetToolsModule->Get();

		for (auto Action : RegisteredAssetTypeActions) {
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}
}

void FOpenWorldEditorPluginModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOpenWorldEditorPluginModule, OpenWorldEditorPlugin)