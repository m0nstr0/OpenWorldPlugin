// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/UICommandInfo.h"

/**
 * 
 */
class FOWPathAssetEdModeCommands : public TCommands<FOWPathAssetEdModeCommands>
{
public:
	FOWPathAssetEdModeCommands()
		: TCommands<FOWPathAssetEdModeCommands>(
			TEXT("PathAssetEdMode"),
			FText::FromString(TEXT("Open World Path Asset Editor")),
			NAME_None,
			FEditorStyle::GetStyleSetName()
			)
	{}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> AddConnectedNodeCommand;
	TSharedPtr<FUICommandInfo> AddUnConnectedNodeCommand;
	TSharedPtr<FUICommandInfo> SnapNodeToGroundCommand;
	TSharedPtr<FUICommandInfo> DeleteNodeCommand;
	TSharedPtr<FUICommandInfo> AddConnectionCommand;
	TSharedPtr<FUICommandInfo> DeleteConnectionCommand;
	TSharedPtr<FUICommandInfo> SelectCommand;
};