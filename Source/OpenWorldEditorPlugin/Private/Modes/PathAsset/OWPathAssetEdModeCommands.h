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
			TEXT("OpenWorldEdMode"),
			FText::FromString(TEXT("Open World Path Asset Editor")),
			NAME_None,
			FName("OpenWorldPlugin")
			)
	{}

	virtual void RegisterCommands() override;

	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands();

public:
	TSharedPtr<FUICommandInfo> PathAssetSelectTool;
	TSharedPtr<FUICommandInfo> PathAssetCreateNodeTool;
	TSharedPtr<FUICommandInfo> PathAssetConnectionTool;
protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
};

