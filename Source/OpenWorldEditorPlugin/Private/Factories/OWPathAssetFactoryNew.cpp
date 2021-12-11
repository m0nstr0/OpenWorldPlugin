// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/OWPathAssetFactoryNew.h"
#include "OWPathAsset.h"

UOWPathAssetFactoryNew::UOWPathAssetFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	bEditorImport = false;
	SupportedClass = UOWPathAsset::StaticClass();
}

UObject* UOWPathAssetFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UOWPathAsset>(InParent, InClass, InName, Flags | RF_Transactional);
}

bool UOWPathAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}
