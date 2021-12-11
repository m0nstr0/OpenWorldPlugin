// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "OWPathAssetEdModeDelegates.h"

/**
 * 
 */
class SOWPathAssetEdModeWidget: public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SOWPathAssetEdModeWidget) {}
		SLATE_ARGUMENT(OWPathAssetEdMode::FOWPathAssetChangedDelegate, PathAssetChanged)
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	FString GetPathAssetPath();

	void CreatePathAssetDetailsView();

	TSharedPtr<class IDetailsView> PathAssetDetailsView;

	class FOWPathAssetEdMode* GetEdMode() const;

	void OnPathAssetChanged(const FAssetData& AssetData);
	OWPathAssetEdMode::FOWPathAssetChangedDelegate PathAssetChangedDelegate;
};
