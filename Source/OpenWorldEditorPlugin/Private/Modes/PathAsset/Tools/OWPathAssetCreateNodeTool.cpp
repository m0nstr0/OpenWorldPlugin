// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetCreateNodeTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "OWPathAssetHitProxies.h"
#include "OWPathAsset.h"
#include "OWPathAssetNode.h"
#include "BaseBehaviors/SingleClickBehavior.h"
#include "CollisionQueryParams.h"
#include "SceneManagement.h"
#include "LevelEditorViewport.h"
#include "BaseBehaviors/MouseHoverBehavior.h"
#include "BaseGizmos/TransformProxy.h"

/*
 * ToolBuilder
 */
UInteractiveTool* UOWPathAssetCreateNodeToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAssetCreateNodeTool* NewTool = NewObject<UOWPathAssetCreateNodeTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

void UOWPathAssetCreateNodeTool::Setup()
{
	UInteractiveTool::Setup();

	UMouseHoverBehavior* MouseHoverBehavior = NewObject<UMouseHoverBehavior>();
	MouseHoverBehavior->Initialize(this);
	AddInputBehavior(MouseHoverBehavior);

    USingleClickInputBehavior* MouseClickBehavior = NewObject<USingleClickInputBehavior>();
    MouseClickBehavior->Initialize(this);
    AddInputBehavior(MouseClickBehavior);
    
}


void UOWPathAssetCreateNodeTool::Shutdown(EToolShutdownType ShutdownType)
{
	UInteractiveTool::Shutdown(ShutdownType);
}

void UOWPathAssetCreateNodeTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!IsPathAssetSelected())	{
		return;
	}

    Super::Render(RenderAPI);

    const TObjectPtr<UOWPathAsset> PathAsset = GetPathAsset();
    const int32 CapsuleSides = FMath::Clamp<int32>(PathAsset->Radius / 4.f, 16, 64);
	FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();

	//PDI->SetHitProxy(new HOWPathAssetNodeHitProxy(PathAssetNode));
	DrawWireCapsule(PDI, PlaceHolderPosition, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, FColor::Blue, PathAsset->Radius, PathAsset->Height / 2.f, CapsuleSides, SDPG_Foreground);
	PDI->DrawPoint(PlaceHolderPosition, FColor::Blue, 30.f, SDPG_Foreground);
	PDI->SetHitProxy(nullptr);
}

FHitResult UOWPathAssetCreateNodeTool::TraceForNodePlacement(const FInputDeviceRay& PressPos) const
{
    const TObjectPtr<UOWPathAsset> PathAsset = GetPathAsset();
    const FCollisionObjectQueryParams CollisionObjectQueryParams(FCollisionObjectQueryParams::AllObjects);
    const FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(DragDropTrace), true);

    const FViewport* Viewport = GEditor->GetActiveViewport();
    const FEditorViewportClient* ViewportClient = static_cast<FEditorViewportClient*>(Viewport->GetClient());
    const ELevelViewportType ViewportType = ViewportClient->GetViewportType();
    const auto RayOrigin = PressPos.WorldRay.Origin;
    const auto PointAt = PressPos.WorldRay.PointAt(HALF_WORLD_MAX);

    if (ViewportType == LVT_OrthoXY || ViewportType == LVT_OrthoXZ || ViewportType == LVT_OrthoYZ ||
        ViewportType == LVT_OrthoNegativeXY || ViewportType == LVT_OrthoNegativeXZ || ViewportType == LVT_OrthoNegativeYZ) {
        //PointAt = PressPos.WorldRay.PointAt(HALF_WORLD_MAX / 2);
    }

    FHitResult Result;
    if (TArray<FHitResult> Hits; TargetWorld->SweepMultiByObjectType(Hits, RayOrigin, PointAt, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeCapsule(PathAsset->Radius, PathAsset->Height / 2.f), CollisionQueryParams)) {
        float ClosestHitDistanceSqr = TNumericLimits<float>::Max();
        for (const FHitResult& Hit : Hits) {
            if (const float DistanceToHitSqr = (Hit.ImpactPoint - RayOrigin).SizeSquared(); DistanceToHitSqr < ClosestHitDistanceSqr) {
                ClosestHitDistanceSqr = DistanceToHitSqr;
                Result = Hit;
            }
        }
    } else {
        const float DistanceMultiplier = (ViewportType == LVT_Perspective) ? GetDefault<ULevelEditorViewportSettings>()->BackgroundDropDistance : 0.0f;
        Result = FHitResult(RayOrigin, PressPos.WorldRay.PointAt(PathAsset->Radius + DistanceMultiplier));
        Result.bBlockingHit = true;
        Result.Location = PressPos.WorldRay.PointAt(PathAsset->Radius + DistanceMultiplier);
        Result.Distance = PressPos.WorldRay.Dist(Result.Location);
    }

    return Result;
}

FInputRayHit UOWPathAssetCreateNodeTool::BeginHoverSequenceHitTest(const FInputDeviceRay& PressPos)
{
    if (!IsPathAssetSelected()) {
        return  FInputRayHit();
    }

    const FHitResult Result = TraceForNodePlacement(PressPos);
    return Result.IsValidBlockingHit() ? FInputRayHit(Result.Distance) : FInputRayHit();
}

void UOWPathAssetCreateNodeTool::OnBeginHover(const FInputDeviceRay& DevicePos)
{
    if (!IsPathAssetSelected()) {
        return;
    }

    if (const FHitResult Result = TraceForNodePlacement(DevicePos); Result.IsValidBlockingHit()) {
        PlaceHolderPosition = Result.Location;
    }
}

bool UOWPathAssetCreateNodeTool::OnUpdateHover(const FInputDeviceRay& DevicePos)
{
    if (!IsPathAssetSelected()) {
        return false;
    }

    if (const FHitResult Result = TraceForNodePlacement(DevicePos); Result.IsValidBlockingHit()) {
        PlaceHolderPosition = Result.Location;
    }

	return true;
}

FInputRayHit UOWPathAssetCreateNodeTool::IsHitByClick(const FInputDeviceRay& ClickPos)
{
    if (!IsPathAssetSelected()) {
        return  FInputRayHit();
    }

    const FHitResult Result = TraceForNodePlacement(ClickPos);
    return Result.IsValidBlockingHit() ? FInputRayHit(Result.Distance) : FInputRayHit();
}

void UOWPathAssetCreateNodeTool::OnClicked(const FInputDeviceRay& ClickPos)
{
    if (!IsPathAssetSelected()) {
        return;
    }

    if (const FHitResult Result = TraceForNodePlacement(ClickPos); Result.IsValidBlockingHit()) {
        AddNodeAtLocation(Result.Location);
    }
}

void UOWPathAssetCreateNodeTool::AddNodeAtLocation(const FVector& InNodeLocation) const
{
    if (!IsPathAssetSelected()) {
        return;
    }

    const TObjectPtr<UOWPathAsset> PathAsset = GetPathAsset();
    if (UOWPathAssetNode* Node = NewObject<UOWPathAssetNode>(PathAsset); Node) {
        const FScopedTransaction Transaction(FText::FromString("OpenWorld PathAsset Add New Node"));
        Node->SetFlags(RF_Transactional);
        Node->Location = InNodeLocation;
        PathAsset->Modify();
        PathAsset->Nodes.Add(Node);
    }
}
