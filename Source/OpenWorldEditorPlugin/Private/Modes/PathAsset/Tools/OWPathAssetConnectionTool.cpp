// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetConnectionTool.h"
#include "InteractiveToolManager.h"

/*
 * ToolBuilder
 */
UInteractiveTool* UOWPathAssetConnectionToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAssetBaseTool* NewTool = NewObject<UOWPathAssetConnectionTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}