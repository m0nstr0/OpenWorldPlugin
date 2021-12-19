// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "OWPathAssetBaseTool.h"
#include "OWPathAssetNode.h"
#include "OWPathAssetConnectionTool.generated.h"


/**
 * Builder for UOWPathAssetConnectionTool
 */
UCLASS()
class  UOWPathAssetConnectionToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};

/**
 *
 */
UCLASS()
class UOWPathAssetConnectionTool : public UOWPathAssetBaseTool
{
	GENERATED_BODY()
};


