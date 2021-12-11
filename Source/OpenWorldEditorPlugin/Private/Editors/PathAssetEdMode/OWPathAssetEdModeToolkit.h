// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Widgets/SWidget.h"
#include "Editors/PathAssetEdMode/OWPathAssetEdModeWidget.h"

/**
 * 
 */
class FOWPathAssetEdModeToolkit: public FModeToolkit
{
public:

	FOWPathAssetEdModeToolkit();

	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FOWPathAssetEdMode* GetEditorMode() const override;
	virtual TSharedPtr<SWidget> GetInlineContent() const override { return OWPathAssetEdModeWidget; }
	virtual void CustomizeModeDetailsViewArgs(FDetailsViewArgs& ArgsInOut) override;
private:
	TSharedPtr<SOWPathAssetEdModeWidget> OWPathAssetEdModeWidget;
};
