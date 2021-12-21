// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetBaseTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "OWPathAsset.h"
#include "OWPathAssetHitProxies.h"
#include "OWPathAssetLink.h"

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
}

void UOWPathAssetBaseTool::Shutdown(EToolShutdownType ShutdownType)
{
	UInteractiveTool::Shutdown(ShutdownType);
}

void UOWPathAssetBaseTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!GetAsset().IsValid()) {
		return;
	}

 	constexpr FColor NormalColor(200, 200, 200);
 
 	for (int32 i = 0; i < GetAsset()->Nodes.Num(); i++) {
		RenderNode(NormalColor, GetAsset()->Nodes[i], RenderAPI);
 	}

	FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();
	for (int32 i = 0; i < GetAsset()->Links.Num(); i++) {
		PDI->DrawLine(FVector(GetAsset()->Links[i]->LeftNode->Location), FVector(GetAsset()->Links[i]->RightNode->Location), NormalColor, SDPG_Foreground);
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

void UOWPathAssetBaseTool::RenderNode(const FColor DrawColor, UOWPathAssetNode* PathAssetNode, IToolsContextRenderAPI* RenderAPI) const
{
	const int32 CapsuleSides = FMath::Clamp<int32>(GetAsset()->Radius / 4.f, 16, 64);
    FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();

	PDI->SetHitProxy(new HOWPathAssetNodeHitProxy(PathAssetNode));
	DrawWireCapsule(PDI, PathAssetNode->Location, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, DrawColor, GetAsset()->Radius, GetAsset()->Height / 2.f, CapsuleSides, SDPG_Foreground);
	PDI->DrawPoint(PathAssetNode->Location, DrawColor, 30.f, SDPG_Foreground);
	PDI->SetHitProxy(nullptr);
}