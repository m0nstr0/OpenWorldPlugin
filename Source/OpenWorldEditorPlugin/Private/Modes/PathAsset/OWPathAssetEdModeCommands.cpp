// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetEdModeCommands.h"
#include "Framework/Commands/Commands.h"
#include "OWOpenWorldEdMode.h"

#define LOCTEXT_NAMESPACE ""
void FOWPathAssetEdModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(UOpenWorldEdMode::PathAssetSelect_Tool);

	UI_COMMAND(PathAssetSelectTool, "Select", "Select Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::One));
	ToolCommands.Add(PathAssetSelectTool);

	UI_COMMAND(PathAssetCreateNodeTool, "Create", "Create Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::Two));
	ToolCommands.Add(PathAssetCreateNodeTool);
}
#undef LOCTEXT_NAMESPACE

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FOWPathAssetEdModeCommands::GetCommands()
{
	return FOWPathAssetEdModeCommands::Get().Commands;
}