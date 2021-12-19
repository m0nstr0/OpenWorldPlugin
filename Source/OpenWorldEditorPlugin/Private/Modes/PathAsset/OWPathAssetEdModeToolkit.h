// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Widgets/SWidget.h"
//#include "Modes/thAssetEdMode/OWPathAssetEdModeWidget.h"

/**
 *
 */
class FOWPathAssetEdModeToolkit : public FModeToolkit
{
public:
	FOWPathAssetEdModeToolkit();
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual void GetToolPaletteNames(TArray<FName>& PaletteNames) const override;
	virtual FText GetToolPaletteDisplayName(FName Palette) const override;
	virtual void CustomizeModeDetailsViewArgs(FDetailsViewArgs& ArgsInOut) override;
	void OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;
	void OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;
	void UpdateModeProperties(const TArray<UObject*>& Objects);
	void UpdateActiveToolProperties(UInteractiveTool* Tool);
	void OnToolPaletteChanged(FName PaletteName) override;
};