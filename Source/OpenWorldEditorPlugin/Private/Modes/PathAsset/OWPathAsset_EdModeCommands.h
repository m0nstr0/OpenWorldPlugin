// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenWorldEditorPluginStyle.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandInfo.h"

/**
 * UOWPathAsset_EdMode Tools
 */
class FOWPathAsset_EdModeCommands : public TCommands<FOWPathAsset_EdModeCommands>
{
public:
	FOWPathAsset_EdModeCommands()
		: TCommands<FOWPathAsset_EdModeCommands>(
			TEXT("PathAsset_EdMode"),
			FText::FromString(TEXT("Open World Path Asset Editor")),
			NAME_None,
			FOpenWorldEditorPluginStyle::Get().GetStyleSetName()
			)
	{}

	virtual void RegisterCommands() override;

	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands() { return Get().Commands; }

public:
	TSharedPtr<FUICommandInfo> SelectTool;
	TSharedPtr<FUICommandInfo> CreateTool;
	TSharedPtr<FUICommandInfo> LinkTool;
protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
};