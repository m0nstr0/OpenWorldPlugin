// Fill out your copyright notice in the Description page of Project Settings.


#include "Editors/PathAssetEdMode/OWPathAssetEdModeCommands.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE ""
void FOWPathAssetEdModeCommands::RegisterCommands()
{
	UI_COMMAND(SnapNodeToGroundCommand, "Snap", "Snap Node to Ground", EUserInterfaceActionType::Button, FInputChord(EModifierKey::None, EKeys::End));
	UI_COMMAND(DeleteNodeCommand, "Delete", "Delete Node", EUserInterfaceActionType::Button, FInputChord(EModifierKey::None, EKeys::Delete));
	UI_COMMAND(AddConnectedNodeCommand, "Add Connected Node", "Add Connected Node", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Alt, EKeys::LeftMouseButton));
	UI_COMMAND(AddUnConnectedNodeCommand, "Add Disconnected Node", "Add Disconnected Node", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Alt | EModifierKey::Control, EKeys::LeftMouseButton));
	UI_COMMAND(AddConnectionCommand, "Add Connection", "Connect Two Nodes", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Shift, EKeys::LeftMouseButton));
	UI_COMMAND(DeleteConnectionCommand, "Delete Connection", "Disconnect Two Nodes", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Shift | EModifierKey::Control, EKeys::LeftMouseButton));
	UI_COMMAND(SelectCommand, "Select", "Select Node or Connection", EUserInterfaceActionType::Button, FInputChord(EModifierKey::None, EKeys::LeftMouseButton));
}
#undef LOCTEXT_NAMESPACE