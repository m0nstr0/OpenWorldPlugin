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

	for (UOWPathAssetNode* Node : GetAsset()->Nodes) {
		RenderNode(DefaultColor, Node, RenderAPI);
	}

	for (const UOWPathAssetLink* Link : GetAsset()->Links) {
		RenderAPI->GetPrimitiveDrawInterface()->DrawLine(FVector(Link->LeftNode->Location), FVector(Link->RightNode->Location), DefaultColor, SDPG_Foreground);
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

	PDI->SetHitProxy(new HOWPathAsset_NodeHitProxy(PathAssetNode));
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

void UOWPathAsset_BaseTool::RenderLink(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, const EOWPathAssetDirectionType Direction, IToolsContextRenderAPI* RenderAPI) const
{
	FColor DrawColor = DefaultColor;
	if (Direction == EOWPathAssetDirectionType::L2R) {
		DrawColor = FirstColor;
	}

	if (Direction == EOWPathAssetDirectionType::R2L) {
		DrawColor = SecondColor;
	}

    if (Direction == EOWPathAssetDirectionType::LAR) {
		DrawColor = ThirdColor;
	}
		
	FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();
	PDI->DrawLine(LeftNode->Location, RightNode->Location, DrawColor, SDPG_Foreground);
	PDI->SetHitProxy(nullptr);
}
