// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetEdModeToolkit.h"
#include "OWOpenWorldEdMode.h"
#include "EditorModeManager.h"
#include "OWPathAssetEdModeCommands.h"

//#include "Editors/PathAssetEdMode/OWPathAssetEdModeWidget.h"

//#include "Engine/Selection.h"
//#include "Widgets/Input/SButton.h"
//#include "Widgets/Text/STextBlock.h"


FOWPathAssetEdModeToolkit::FOWPathAssetEdModeToolkit()
{}

void FOWPathAssetEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
	Cast<UOpenWorldEdMode>(InOwningMode.Get())->OnEdModeObjectsChanged.AddSP(this, &FOWPathAssetEdModeToolkit::UpdateModeProperties);
	//	Cast<UOpenWorldEdMode>(OwningEditorMode.Get())->OnEdModeObjectsChanged.RemoveAll(this);

}

FName FOWPathAssetEdModeToolkit::GetToolkitFName() const
{
	return FName("OpenWorldEdMode");
}

FText FOWPathAssetEdModeToolkit::GetBaseToolkitName() const
{
	return FText::FromString("OpenWorldEdMode Toolkit");
}

void FOWPathAssetEdModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(UOpenWorldEdMode::PathAssetSelect_Tool);
	
	//PaletteNames.Add(UOpenWorldEdMode::MapZoneAsset_Tool);
}

FText FOWPathAssetEdModeToolkit::GetToolPaletteDisplayName(FName Palette) const
{
	if (Palette == UOpenWorldEdMode::PathAssetSelect_Tool) {
		return FText::FromString("Path Asset");
	}

	if (Palette == UOpenWorldEdMode::PathAssetCreateNode_Tool) {
		return FText::FromString("Map Zone");
	}

	return FText();
}

void FOWPathAssetEdModeToolkit::CustomizeModeDetailsViewArgs(FDetailsViewArgs& ArgsInOut)
{
	return;
}

void FOWPathAssetEdModeToolkit::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	UpdateActiveToolProperties(Tool);

	Tool->OnPropertySetsModified.AddSP(this, &FOWPathAssetEdModeToolkit::UpdateActiveToolProperties, Tool);
	
	FModeToolkit::OnToolStarted(Manager, Tool);
}

void FOWPathAssetEdModeToolkit::OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	if (Tool) {
		Tool->OnPropertySetsModified.RemoveAll(this);
	}

	FModeToolkit::OnToolEnded(Manager, Tool);
}

void FOWPathAssetEdModeToolkit::UpdateModeProperties(const TArray<UObject*>& Objects)
{
	ModeDetailsView->SetObjects(Objects);
}

void FOWPathAssetEdModeToolkit::UpdateActiveToolProperties(UInteractiveTool* Tool)
{
	if (Tool) {
		DetailsView->SetObjects(Tool->GetToolProperties(false));
	}
}

void FOWPathAssetEdModeToolkit::OnToolPaletteChanged(FName PaletteName)
{
	//ToolkitCommands
	//GetToolkitCommands()->CanExecuteAction();
	//if (PaletteName == UOpenWorldEdMode::PathAsset_Tool) {
	//	GetToolkitCommands()->TryExecuteAction(FOWPathAssetEdModeCommands::Get().PathAssetTool.ToSharedRef());
	//}

	//GetCommands
	//FOWPathAssetEdModeCommands::Get()
}
