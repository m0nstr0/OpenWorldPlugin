// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeCategories.h"
#include "Styling/SlateStyle.h"

class FOpenWorldEditorPluginModule : public IModuleInterface
{
public:
	EAssetTypeCategories::Type OpenWorldAssetCategoryBit;

protected:
	TArray<TSharedPtr<IAssetTypeActions>> RegisteredAssetTypeActions;

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);
	void UnRegisterAssetTypeAction();
};
