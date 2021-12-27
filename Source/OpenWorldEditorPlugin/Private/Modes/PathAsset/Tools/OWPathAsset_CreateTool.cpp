// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset_CreateTool.h"
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
UInteractiveTool* UOWPathAsset_CreateToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAsset_CreateTool* NewTool = NewObject<UOWPathAsset_CreateTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

/*
 * UOWPathAsset_CreateTool
 */
void UOWPathAsset_CreateTool::Setup()
{
	UInteractiveTool::Setup();

	UMouseHoverBehavior* MouseHoverBehavior = NewObject<UMouseHoverBehavior>();
	MouseHoverBehavior->Initialize(this);
	AddInputBehavior(MouseHoverBehavior);

    USingleClickInputBehavior* MouseClickBehavior = NewObject<USingleClickInputBehavior>();
    MouseClickBehavior->Initialize(this);
    AddInputBehavior(MouseClickBehavior);
    
}

void UOWPathAsset_CreateTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!GetAsset().IsValid()) {
		return;
	}

    UOWPathAsset_BaseTool::Render(RenderAPI);

    const int32 CapsuleSides = FMath::Clamp<int32>(GetAsset()->Radius / 4.f, 16, 64);
	FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();

    RenderNode(FirstColor, PlaceHolderLocation, RenderAPI);
}

FHitResult UOWPathAsset_CreateTool::TraceForNodePlacement(const FInputDeviceRay& PressPos) const
{
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
    if (TArray<FHitResult> Hits; TargetWorld->SweepMultiByObjectType(Hits, RayOrigin, PointAt, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeCapsule(GetAsset()->Radius, GetAsset()->Height / 2.f), CollisionQueryParams)) {
        float ClosestHitDistanceSqr = TNumericLimits<float>::Max();
        for (const FHitResult& Hit : Hits) {
            if (const float DistanceToHitSqr = (Hit.ImpactPoint - RayOrigin).SizeSquared(); DistanceToHitSqr < ClosestHitDistanceSqr) {
                ClosestHitDistanceSqr = DistanceToHitSqr;
                Result = Hit;
            }
        }
    } else {
        const float DistanceMultiplier = (ViewportType == LVT_Perspective) ? GetDefault<ULevelEditorViewportSettings>()->BackgroundDropDistance : 0.0f;
        Result = FHitResult(RayOrigin, PressPos.WorldRay.PointAt(GetAsset()->Radius + DistanceMultiplier));
        Result.bBlockingHit = true;
        Result.Location = PressPos.WorldRay.PointAt(GetAsset()->Radius + DistanceMultiplier);
        Result.Distance = PressPos.WorldRay.Dist(Result.Location);
    }

    return Result;
}

bool UOWPathAsset_CreateTool::OnUpdateHover(const FInputDeviceRay& DevicePos)
{
    if (const FHitResult Result = TraceForNodePlacement(DevicePos); GetAsset().IsValid() && Result.IsValidBlockingHit()) {
        PlaceHolderLocation = Result.Location;
        return true;
    }

	return false;
}

void UOWPathAsset_CreateTool::OnClicked(const FInputDeviceRay& ClickPos)
{
    if (const FHitResult Result = TraceForNodePlacement(ClickPos); GetAsset().IsValid() && Result.IsValidBlockingHit()) {
        AddNodeAtLocation(Result.Location);
    }
}

void UOWPathAsset_CreateTool::AddNodeAtLocation(const FVector& InNodeLocation) const
{
    if (GetAsset().IsValid()) {
        GetAsset()->CreateNode(InNodeLocation);
    }
}
