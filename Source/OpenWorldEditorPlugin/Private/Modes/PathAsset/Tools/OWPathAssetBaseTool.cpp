// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetBaseTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "OWPathAsset.h"
#include "OWPathAssetHitProxies.h"

/*
 * ToolBuilder
 */
UInteractiveTool* UOWPathAssetBaseToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAssetBaseTool* NewTool = NewObject<UOWPathAssetBaseTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

void UOWPathAssetBaseTool::SetWorld(UWorld* World)
{
	check(World);
	this->TargetWorld = World;
}


void UOWPathAssetBaseTool::Setup()
{
	UInteractiveTool::Setup();

	//Properties = NewObject<UOWPathAssetInteractiveToolProperties>(this, "Path Asset");
	//AddToolPropertySource(Properties);
}


void UOWPathAssetBaseTool::Shutdown(EToolShutdownType ShutdownType)
{
	UInteractiveTool::Shutdown(ShutdownType);
}

void UOWPathAssetBaseTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!IsPathAssetSelected()) {
		return;
	}

    const TObjectPtr<UOWPathAsset> PathAsset = GetPathAsset();
 	constexpr FColor NormalColor(200, 200, 200);
 
 	for (int32 i = 0; i < PathAsset->Nodes.Num(); i++) {
		DrawNode(NormalColor, PathAsset, PathAsset->Nodes[i], RenderAPI);
 	}
}

bool UOWPathAssetBaseTool::RemoveToolPropertySourceObject(UObject* PropertySet)
{
    const int32 NumRemoved = ToolPropertyObjects.Remove(PropertySet);
	if (NumRemoved == 0) {
		return false;
	}

	OnPropertySetsModified.Broadcast();
	return true;
}

void UOWPathAssetBaseTool::CleanToolPropertySource(TObjectPtr<UObject> ObjectToAdd)
{
	ToolPropertyObjects.Empty();
	if (ObjectToAdd)
	{
		ToolPropertyObjects.Add(ObjectToAdd);
	}
	OnPropertySetsModified.Broadcast();
}

void UOWPathAssetBaseTool::DrawNode(const FColor DrawColor, const TObjectPtr<UOWPathAsset> PathAsset, const TObjectPtr<UOWPathAssetNode> PathAssetNode, IToolsContextRenderAPI* RenderAPI) const
{
	const int32 CapsuleSides = FMath::Clamp<int32>(PathAsset->Radius / 4.f, 16, 64);
    FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();

	PDI->SetHitProxy(new HOWPathAssetNodeHitProxy(PathAssetNode));
	DrawWireCapsule(PDI, PathAssetNode->Location, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, DrawColor, PathAsset->Radius, PathAsset->Height / 2.f, CapsuleSides, SDPG_Foreground);
	PDI->DrawPoint(PathAssetNode->Location, DrawColor, 30.f, SDPG_Foreground);
	PDI->SetHitProxy(nullptr);
}

void UOWPathAssetBaseTool::OnPathAssetChanged(UOWPathAsset* InPathAsset)
{
	PathAssetBeingEdited = InPathAsset;
}