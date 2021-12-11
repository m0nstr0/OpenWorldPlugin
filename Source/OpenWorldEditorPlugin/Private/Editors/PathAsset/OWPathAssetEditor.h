// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "OWPathAsset.h"
#include "Misc/NotifyHook.h"

/**
 * 
 */
class FOWPathAssetEditor: public FAssetEditorToolkit, public FNotifyHook
{
public:
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UOWPathAsset* InPathAsset);

	//virtual void InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const FName AppIdentifier, const TSharedRef<FTabManager::FLayout>& StandaloneDefaultLayout, const bool bCreateDefaultStandaloneMenu, const bool bCreateDefaultToolbar, UObject* ObjectToEdit, const bool bInIsToolbarFocusable = false, const bool bInUseSmallToolbarIcons = false);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager);

	TSharedRef<SDockTab> SpawnTab_GraphEditor(const FSpawnTabArgs& Args);
	
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);

	FName GetToolkitFName() const override;


	FText GetBaseToolkitName() const override;


	FString GetWorldCentricTabPrefix() const override;


	void InvokeTab(const struct FTabId& TabId) override;


	FLinearColor GetWorldCentricTabColorScale() const override;

	virtual ~FOWPathAssetEditor();
};
