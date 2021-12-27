// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
			TEXT("OpenWorldEdMode"),
			FText::FromString(TEXT("Open World Path Asset Editor")),
			NAME_None,
			FName("OpenWorldPlugin")
			)
	{}

	virtual void RegisterCommands() override;

	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands() { return Get().Commands; }

public:
	TSharedPtr<FUICommandInfo> PathAsset_SelectTool;
	TSharedPtr<FUICommandInfo> PathAsset_CreateTool;
	TSharedPtr<FUICommandInfo> PathAsset_LinkTool;
protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
};