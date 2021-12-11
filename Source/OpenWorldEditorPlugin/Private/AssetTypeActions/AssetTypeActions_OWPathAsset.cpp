// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions/AssetTypeActions_OWPathAsset.h"
#include "OWPathAsset.h"
#include "Editors/PathAsset/OWPathAssetEditor.h"

#define LOCTEXT_NAMESPACE "FAssetTypeActions_OWPathAsset"

FAssetTypeActions_OWPathAsset::FAssetTypeActions_OWPathAsset(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{

}

bool FAssetTypeActions_OWPathAsset::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

void FAssetTypeActions_OWPathAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (auto Object : InObjects) {
		UOWPathAsset* NewAsset = Cast<UOWPathAsset>(Object);
		if (NewAsset != nullptr) {
			TSharedRef<FOWPathAssetEditor> NewEditor(new FOWPathAssetEditor());
			NewEditor->InitEditor(Mode, EditWithinLevelEditor, NewAsset);
		}
	}
}

FText FAssetTypeActions_OWPathAsset::GetName() const
{
	return LOCTEXT("OWPathNew", "Path Asset");
}

UClass* FAssetTypeActions_OWPathAsset::GetSupportedClass() const
{
	return UOWPathAsset::StaticClass();
}

FColor FAssetTypeActions_OWPathAsset::GetTypeColor() const
{
	return FColor::Blue;
}

uint32 FAssetTypeActions_OWPathAsset::GetCategories()
{
	return MyAssetCategory;
}

FText FAssetTypeActions_OWPathAsset::GetAssetDescription(const FAssetData& AssetData) const
{
	return LOCTEXT("OWPedPathNew", "Path Asset");
}

#undef LOCTEXT_NAMESPACE