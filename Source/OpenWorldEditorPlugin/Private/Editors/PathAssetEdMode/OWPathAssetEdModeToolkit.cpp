// Fill out your copyright notice in the Description page of Project Settings.


#include "Editors/PathAssetEdMode/OWPathAssetEdModeToolkit.h"
#include "Editors/PathAssetEdMode/OWPathAssetEdModeWidget.h"
#include "OWPathAssetEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FPedPathEdModeToolkit"

FOWPathAssetEdModeToolkit::FOWPathAssetEdModeToolkit()
{
}

void FOWPathAssetEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{

	FModeToolkit::Init(InitToolkitHost);

	SAssignNew(OWPathAssetEdModeWidget, SOWPathAssetEdModeWidget);

	return;
}

FName FOWPathAssetEdModeToolkit::GetToolkitFName() const
{
	return FName("OWPathAssetEdMode");
}

FText FOWPathAssetEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("PedPathEdModeToolkit", "DisplayName", "PathAssetEdMode Tool");
}

FOWPathAssetEdMode* FOWPathAssetEdModeToolkit::GetEditorMode() const
{
	return (FOWPathAssetEdMode*)GLevelEditorModeTools().GetActiveMode(FOWPathAssetEdMode::EM_OWPathAssetEdModeId);
}

void FOWPathAssetEdModeToolkit::CustomizeModeDetailsViewArgs(FDetailsViewArgs& ArgsInOut)
{
	return;
}

#undef LOCTEXT_NAMESPACE