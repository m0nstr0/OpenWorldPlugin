// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class FAssetTypeActions_OWPathAsset : public FAssetTypeActions_Base
{
public:

	FAssetTypeActions_OWPathAsset(EAssetTypeCategories::Type InAssetCategory);


	bool HasActions(const TArray<UObject*>& InObjects) const override;


	void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;


	FText GetName() const override;


	UClass* GetSupportedClass() const override;


	FColor GetTypeColor() const override;


	uint32 GetCategories() override;

protected:
	FText GetAssetDescription(const FAssetData& AssetData) const override;

	EAssetTypeCategories::Type MyAssetCategory;
};
