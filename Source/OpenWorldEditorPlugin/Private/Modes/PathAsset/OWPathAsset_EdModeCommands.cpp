// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset_EdModeCommands.h"
#include "Framework/Commands/Commands.h"
#include "OWPathAsset_EdMode.h"

#define LOCTEXT_NAMESPACE ""
void FOWPathAsset_EdModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(UOWPathAsset_EdMode::PathAssetSelect_Tool);

	UI_COMMAND(PathAsset_SelectTool, "Select", "Select Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::One));
	ToolCommands.Add(PathAsset_SelectTool);

	UI_COMMAND(PathAsset_CreateTool, "Create", "Create Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::Two));
	ToolCommands.Add(PathAsset_CreateTool);

	UI_COMMAND(PathAsset_LinkTool, "Link", "Link Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::Three));
	ToolCommands.Add(PathAsset_LinkTool);
}
#undef LOCTEXT_NAMESPACE