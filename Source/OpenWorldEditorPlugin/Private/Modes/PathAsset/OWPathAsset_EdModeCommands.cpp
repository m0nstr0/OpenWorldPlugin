// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset_EdModeCommands.h"
#include "Framework/Commands/Commands.h"
#include "OWPathAsset_EdMode.h"

#define LOCTEXT_NAMESPACE ""
void FOWPathAsset_EdModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(UOWPathAsset_EdMode::PathAssetSelect_Tool);

	UI_COMMAND(SelectTool, "Select", "Select Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::One));
	ToolCommands.Add(SelectTool);

	UI_COMMAND(CreateTool, "Create", "Create Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::Two));
	ToolCommands.Add(CreateTool);

	UI_COMMAND(LinkTool, "Link", "Link Nodes", EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::None, EKeys::Three));
	ToolCommands.Add(LinkTool);
}
#undef LOCTEXT_NAMESPACE