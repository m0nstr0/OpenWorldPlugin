// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset_BaseTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "OWPathAsset.h"
#include "OWPathAssetHitProxies.h"
#include "OWPathAssetLink.h"

/*
 * ToolBuilder
 */
UInteractiveTool* UOWPathAsset_BaseToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAsset_BaseTool* NewTool = NewObject<UOWPathAsset_BaseTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

/*
 * UOWPathAsset_BaseTool
 */
void UOWPathAsset_BaseTool::SetWorld(UWorld* World)
{
	check(World);
	this->TargetWorld = World;
}

void UOWPathAsset_BaseTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!GetAsset().IsValid()) {
		return;
	}

 	constexpr FColor NormalColor(200, 200, 200);

	for (UOWPathAssetNode* Node : GetAsset()->Nodes) {
		RenderNode(NormalColor, Node, RenderAPI);
	}

	for (const UOWPathAssetLink* Link : GetAsset()->Links) {
		RenderAPI->GetPrimitiveDrawInterface()->DrawLine(FVector(Link->LeftNode->Location), FVector(Link->RightNode->Location), NormalColor, SDPG_Foreground);
	}
}

bool UOWPathAsset_BaseTool::RemoveToolPropertySourceObject(UObject* PropertySet)
{
    const int32 NumRemoved = ToolPropertyObjects.Remove(PropertySet);
	if (NumRemoved == 0) {
		return false;
	}

	OnPropertySetsModified.Broadcast();
	return true;
}

void UOWPathAsset_BaseTool::CleanToolPropertySource(TObjectPtr<UObject> ObjectToAdd)
{
	ToolPropertyObjects.Empty();
	if (ObjectToAdd)
	{
		ToolPropertyObjects.Add(ObjectToAdd);
	}
	OnPropertySetsModified.Broadcast();
}

void UOWPathAsset_BaseTool::RenderNode(const FColor DrawColor, UOWPathAssetNode* PathAssetNode, IToolsContextRenderAPI* RenderAPI) const
{
	const int32 CapsuleSides = FMath::Clamp<int32>(GetAsset()->Radius / 4.f, 16, 64);
    FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();

	PDI->SetHitProxy(new HOWPathAssetNodeHitProxy(PathAssetNode));
	DrawWireCapsule(PDI, PathAssetNode->Location, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, DrawColor, GetAsset()->Radius, GetAsset()->Height / 2.f, CapsuleSides, SDPG_Foreground);
	PDI->DrawPoint(PathAssetNode->Location, DrawColor, 30.f, SDPG_Foreground);
	PDI->SetHitProxy(nullptr);
}

void UOWPathAsset_BaseTool::RenderNode(const FColor DrawColor, const FVector& Location, IToolsContextRenderAPI* RenderAPI) const
{
	const int32 CapsuleSides = FMath::Clamp<int32>(GetAsset()->Radius / 4.f, 16, 64);
	FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();

	PDI->SetHitProxy(nullptr);
	DrawWireCapsule(PDI, Location, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, DrawColor, GetAsset()->Radius, GetAsset()->Height / 2.f, CapsuleSides, SDPG_Foreground);
	PDI->DrawPoint(Location, DrawColor, 30.f, SDPG_Foreground);
	PDI->SetHitProxy(nullptr);
}