// Fill out your copyright notice in the Description page of Project Settings.

#include "OWOpenWorldEdMode.h"
#include "OWPathAssetEdModeCommands.h"
#include "OpenWorldEditorPluginStyle.h"
#include "Tools/OWPathAssetBaseTool.h"
#include "InteractiveToolManager.h"
#include "OWPathAssetLink.h"
#include "OWPathAssetEdModeToolkit.h"
#include "Tools/OWPathAssetConnectionTool.h"
#include "Tools/OWPathAssetSelectTool.h"
#include "Tools/OWPathAssetCreateNodeTool.h"

const FEditorModeID UOpenWorldEdMode::EM_OpenWorldEdModeId = TEXT("EM_OpenWorldEdModeId");
const FName UOpenWorldEdMode::PathAssetSelect_Tool = FName("OpenWorldEdMode_PathAssetSelect_Tool");
const FName UOpenWorldEdMode::PathAssetCreateNode_Tool = FName("OpenWorldEdMode_PathAssetCreateNode_Tool");
const FName UOpenWorldEdMode::PathAssetConnection_Tool = FName("OpenWorldEdMode_PathAssetConnection_Tool");

UOpenWorldEdMode::UOpenWorldEdMode()
{
	Info = FEditorModeInfo(EM_OpenWorldEdModeId, FText::FromString("OpenWorld Editor"), FSlateIcon(FOpenWorldEditorPluginStyle::Get().GetStyleSetName(), "OpenWorldEdMode", "OpenWorldEdMode.small"), true);
	PathAssetBeingEdited = nullptr;
}

UOpenWorldEdMode::~UOpenWorldEdMode()
{

}

void UOpenWorldEdMode::Enter()
{
	UEdMode::Enter();

	const FOWPathAssetEdModeCommands& UICommands = FOWPathAssetEdModeCommands::Get();

	ClearModePropertyObject();

	ModeProperties = NewObject<UOWPathAssetModeProperties>(this);
	ModeProperties->WatchProperty(ModeProperties->PathAsset, [this](UOWPathAsset* InPathAsset) {
		ReplaceModePropertyObject(PathAssetBeingEdited, InPathAsset);
		PathAssetBeingEdited = InPathAsset;
		OnPathAssetSelected.Broadcast(InPathAsset);
	});
	AddModePropertyObject(ModeProperties);

	RegisterTool(UICommands.PathAssetSelectTool, PathAssetSelect_Tool.ToString(), NewObject<UOWPathAssetSelectToolBuilder>(this));
	RegisterTool(UICommands.PathAssetCreateNodeTool, PathAssetCreateNode_Tool.ToString(), NewObject<UOWPathAssetCreateNodeToolBuilder>(this));
	RegisterTool(UICommands.PathAssetConnectionTool, PathAssetConnection_Tool.ToString(), NewObject<UOWPathAssetConnectionToolBuilder>(this));

	//GetToolManager()->SelectActiveToolType(EToolSide::Left, PathAssetSelect_Tool.ToString());
	//GetToolManager()->SelectActiveToolType(EToolSide::Left, PathAssetCreateNode_Tool.ToString());
	//GetToolManager()->SelectActiveToolType(EToolSide::Left, PathAssetConnection_Tool.ToString());
}

void UOpenWorldEdMode::Exit()
{
	UEdMode::Exit();

	ClearModePropertyObject();
}

void UOpenWorldEdMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FOWPathAssetEdModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UOpenWorldEdMode::GetModeCommands() const
{
	return FOWPathAssetEdModeCommands::Get().GetCommands();
}

void UOpenWorldEdMode::AddModePropertyObject(UObject* InObject)
{
	if (ModePropertyObjects.Contains(InObject)) {
		return;
	}

	ModePropertyObjects.Add(InObject);
	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOpenWorldEdMode::ReplaceModePropertyObject(UObject* OldObject, UObject* WithObject)
{
	if (OldObject == WithObject) {
		return;
	}

	if (ModePropertyObjects.Contains(WithObject)) {
		return;
	}

	if (!OldObject || !ModePropertyObjects.Contains(OldObject)) {
		if (WithObject) {
			ModePropertyObjects.Add(WithObject);
			OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
		}
		return;
	}

	if (ModePropertyObjects.Contains(OldObject)) {
		int32 Index = ModePropertyObjects.Find(OldObject);
		if (WithObject) {
			ModePropertyObjects[Index] = WithObject;
		}
		else {
			ModePropertyObjects.Remove(OldObject);
		}
	}

	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOpenWorldEdMode::ClearModePropertyObject()
{
	ModePropertyObjects.Empty();
	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOpenWorldEdMode::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	FOWPathAssetSelectToolActionCommands::Get().BindCommandsForCurrentTool(Toolkit->GetToolkitCommands(), Tool);

	UOWPathAssetBaseTool* BaseTool = Cast<UOWPathAssetBaseTool>(Tool);
	OnPathAssetSelected.AddUObject(BaseTool, &UOWPathAssetBaseTool::OnPathAssetChanged);
	BaseTool->OnPathAssetChanged(PathAssetBeingEdited);
}

void UOpenWorldEdMode::OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	if (ExactCast<UOWPathAssetSelectTool>(Tool)) {
		FOWPathAssetSelectToolActionCommands::Get().UnbindActiveCommands(Toolkit->GetToolkitCommands());
	}

	OnPathAssetSelected.RemoveAll(Tool);
}

void UOpenWorldEdMode::ModeTick(float DeltaTime)
{
	ModeProperties->CheckAndUpdateWatched();
}

bool UOpenWorldEdMode::ShouldToolStartBeAllowed(const FString& ToolIdentifier) const
{
	if (!PathAssetBeingEdited) {
		return false;
	}

	return Super::ShouldToolStartBeAllowed(ToolIdentifier);
}

// void UOWPathAssetEdMode::DoDeleteNodeCommand()
// {
// 	if (!SelectedPathAsset.IsValid() || !SelectedPathAssetNode.IsValid()) {
// 		return;
// 	}
// 
// 	const FScopedTransaction Transaction(FText::FromString("Delete Node"));
// 	
// 	SelectedPathAsset->Modify();
// 	SelectedPathAsset->Nodes.Remove(SelectedPathAssetNode.Get());
// 
// 	SelectedPathAssetNode->Modify();
// 	SelectedPathAssetNode->ConditionalBeginDestroy();
// }
// 
// void UOWPathAssetEdMode::DoSnapNodeCommand()
// {
// 	if (!SelectedPathAsset.IsValid() || !SelectedPathAssetNode.IsValid()) {
// 		return;
// 	}
// 
// 	const FVector RayStart = SelectedPathAssetNode->Location;
// 	const FVector RayEnd = RayStart + FVector::DownVector * HALF_WORLD_MAX;
// 
// 	FHitResult Hit;
// 	if (GetWorld()->SweepSingleByObjectType(Hit, RayStart, RayEnd, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects), FCollisionShape::MakeCapsule(SelectedPathAsset->Radius, SelectedPathAsset->Height / 2.f), FCollisionQueryParams(SCENE_QUERY_STAT(DragDropTrace), true))) {
// 		const FScopedTransaction Transaction(FText::FromString("Snap Node to ground"));
// 		SelectedPathAssetNode->Modify();
// 		SelectedPathAssetNode->Location = Hit.Location;
// 	}
// }
// 
// 
// void UOWPathAssetEdMode::DoAddConnectedNodeCommand()
// {
// 	FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
// 	if (!ViewportClient) {
// 		return;
// 	}
// 
// 	AddPathAssetNode(TraceForPlacement(ViewportClient));
// }
// 
// void UOWPathAssetEdMode::DoAddUnConnectedNodeCommand()
// {
// 	FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
// 	if (!ViewportClient) {
// 		return;
// 	}
// 
// 	AddPathAssetNode(TraceForPlacement(ViewportClient));
// }
// 
// void UOWPathAssetEdMode::AddPathAssetNode(FVector InLocation)
// {
// 	if (!SelectedPathAsset.IsValid()) {
// 		return;
// 	}
// 
// 	const FScopedTransaction Transaction(FText::FromString("Add Path Asset Point"));
// 
// 	SelectedPathAsset->Modify();
// 	UOWPathAssetNode* NewPathAssetNode = NewObject<UOWPathAssetNode>(SelectedPathAsset.Get());
// 	NewPathAssetNode->SetFlags(RF_Transactional);
// 	NewPathAssetNode->Location = InLocation;
// 	SelectedPathAsset->Nodes.Add(NewPathAssetNode);
// 	SelectedPathAssetNode = NewPathAssetNode;
// }





// void UOWPathAssetEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
// {
// 	const FColor NormalColor(200, 200, 200);
// 	const FColor SelectedColor(255, 128, 0);
// 
// 	UWorld* World = GetWorld();
// 	if (SelectedPathAsset.IsValid()) {
// 		UOWPathAsset* PathAsset = SelectedPathAsset.Get();
// 		const int32 CapsuleSides = FMath::Clamp<int32>(PathAsset->Radius / 4.f, 16, 64);
// 
// 		for (int32 i = 0; i < PathAsset->Nodes.Num(); i++) {
// 			UOWPathAssetNode* PathAssetNode = PathAsset->Nodes[i];
// 
// 			FColor DrawColor = NormalColor;
// 			if (PathAssetNode == SelectedPathAssetNode) {
// 				DrawColor = SelectedColor;
// 			}
// 
// 			PDI->SetHitProxy(new HOWPathAssetEdModeHitProxy(PathAssetNode));
// 			DrawWireCapsule(PDI, PathAssetNode->Location, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, DrawColor, PathAsset->Radius, PathAsset->Height / 2.f, CapsuleSides, SDPG_Foreground);
// 			PDI->DrawPoint(PathAssetNode->Location, DrawColor, 30.f, SDPG_Foreground);
// 			PDI->SetHitProxy(NULL);
// 
// 
// 			if (i > 0) {
// 				/*FInterpCurveVector InterpVector;
// 				//InterpVector.AddPoint()
// 
// 				FVector P0 = PathAsset->Nodes[i - 1]->Location;
// 				FVector P1 = PathAsset->Nodes[i - 1]->Location + PathAssetNode->Location;
// 				FVector P3 = PathAssetNode->Location;
// 				float oldX = PathAsset->Nodes[i - 1]->Location.X;
// 				float oldY = PathAsset->Nodes[i - 1]->Location.Z;
// 				float oldZ = PathAsset->Nodes[i - 1]->Location.Z;
// 				for (float t = 0; t <= 1.f; t += 0.5f) {
// 					float x = (1 - t) * (1 - t) * P0.X + 2 * (1 - t) * t * P1.X + P3.X;
// 					float y = (1 - t) * (1 - t) * P0.Y + 2 * (1 - t) * t * P1.Y + P3.Y;
// 					float z = (1 - t) * (1 - t) * P0.Z + 2 * (1 - t) * t * P1.Z + P3.Z;
// 					PDI->DrawLine(FVector(oldX, oldY, oldZ), FVector(x, y, z), normalColor, SDPG_Foreground);
// 					oldY = y;
// 					oldX = x;
// 					oldZ = z;
// 				}*/
// 				PDI->DrawLine(PathAsset->Nodes[i - 1]->Location, PathAssetNode->Location, NormalColor, SDPG_Foreground);
// 			}
// 
// 		}
// 
// 		if (Viewport->KeyState(EKeys::LeftControl) && bIsMouseInsideViewport) {
// 			DrawWireCapsule(PDI, PlaceHolderLocation, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, FColor::Blue, PathAsset->Radius, PathAsset->Height / 2.f, CapsuleSides, SDPG_Foreground);
// 			PDI->DrawLine(PathAsset->Nodes[PathAsset->Nodes.Num() - 1]->Location, PlaceHolderLocation, FColor::Blue, SDPG_Foreground);
// 		}
// 	}
// 
// 
// 	/*for (TActorIterator<AOWPathAssetEdModeActor> It(World); It; ++It) {
// 		AOWPathAssetEdModeActor* PathAsset = (*It);
// 		if (PathAsset) {
// 			FVector PathAssetLocation = PathAsset->GetActorLocation();
// 			for (int i = 0; i < PathAsset->Points.Num(); i++) {
// 				bool bSelected = (actor == currentSelectedTarget && i == currentSelectedIndex);
// 				const FColor& color = bSelected ? selectedColor : normalColor;
// 				// set hit proxy and draw
// 				PDI->SetHitProxy(new HExamplePointProxy(actor, i));
// 				PDI->DrawPoint(actor->Points[i], color, 15.f, SDPG_Foreground);
// 				PDI->DrawLine(actor->Points[i], actorLoc, color, SDPG_Foreground);
// 				PDI->SetHitProxy(NULL);
// 			}
// 		}
// 	}*/
// 
// 	//FEdMode::Render(View, Viewport, PDI);
// }
// 
// bool UOWPathAssetEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
// {
// 	if (!SelectedPathAsset.IsValid()) {
// 		return false;
// 	}
// 
// 	PlaceHolderLocation = TraceForPlacement(ViewportClient);
// 
// 	return true;
// }
// 
// bool UOWPathAssetEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
// {
// 	if (Event == IE_Pressed) {
// 		return CommandsHandler.UICommandList->ProcessCommandBindings(Key, FSlateApplication::Get().GetModifierKeys(), false);
// 	}
// 
// 	return false;
// }
// 
// bool UOWPathAssetEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
// {
// 	if (InViewportClient->GetCurrentWidgetAxis() == EAxisList::None) {
// 		return false;
// 	}
// 
// 	if (!InDrag.IsZero()) {
// 		TArray<TWeakObjectPtr<UOWPathAssetNode>>& Nodes = CommandsHandler.GetSelectedNodes();
// 		for (TWeakObjectPtr<UOWPathAssetNode> Node : Nodes) {
// 			if (Node.IsValid()) {
// 				SelectedPathAsset->Modify();
// 				Node->Modify();
// 				Node->Location += InDrag;
// 			}
// 		}
// 		return true;
// 	}
// 	
// 	return false;
// }
// 
// bool UOWPathAssetEdMode::UsesTransformWidget() const
// {
// 	const TWeakObjectPtr<UOWPathAssetNode> Node = CommandsHandler.GetLastValidSelectedNode();
// 	return Node.IsValid();
// }
// 
// FVector UOWPathAssetEdMode::GetWidgetLocation() const
// {
// 	const TWeakObjectPtr<UOWPathAssetNode> Node = CommandsHandler.GetLastValidSelectedNode();
// 	if (Node.IsValid()) {
// 		return Node->Location;
// 	}
// 	return FVector::ZeroVector;
// }
// 
// FVector UOWPathAssetEdMode::TraceForPlacement(FEditorViewportClient* InViewportClient)
// {
// 	if (!SelectedPathAsset.IsValid()) {
// 		return FVector(0.f);
// 	}
// 
// 	//const FVector Min(0.f);
// 	//const FVector Max(SelectedPathAsset->Radius * 2, SelectedPathAsset->Height, SelectedPathAsset->Radius * 2);
// 	//FVector Extend = 0.5f * (Max - Min) - FVector((Min + Max) * 0.5f);
// 
// 	FViewportCursorLocation Cursor = InViewportClient->GetCursorWorldLocationFromMousePos();
// 
// 	const ELevelViewportType ViewportType = InViewportClient->GetViewportType();
// 	FVector RayStart = Cursor.GetOrigin();
// 	if (ViewportType == LVT_OrthoXY || ViewportType == LVT_OrthoXZ || ViewportType == LVT_OrthoYZ ||
// 		ViewportType == LVT_OrthoNegativeXY || ViewportType == LVT_OrthoNegativeXZ || ViewportType == LVT_OrthoNegativeYZ) {
// 		RayStart -= Cursor.GetDirection() * HALF_WORLD_MAX / 2;
// 	}
// 
// 	const FVector RayEnd = RayStart + Cursor.GetDirection() * HALF_WORLD_MAX;
// 
// 	TArray<FHitResult> Hits;
// 	FVector Location;
// 	FVector Normal;
// 	if (InViewportClient->GetWorld()->SweepMultiByObjectType(Hits, RayStart, RayEnd, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects), FCollisionShape::MakeCapsule(SelectedPathAsset->Radius, SelectedPathAsset->Height / 2.f), FCollisionQueryParams(SCENE_QUERY_STAT(DragDropTrace), true))) {
// 		float ClosestHitDistanceSqr = TNumericLimits<float>::Max();
// 		for (const auto& Hit : Hits) {
// 			const float DistanceToHitSqr = (Hit.ImpactPoint - RayStart).SizeSquared();
// 			if (DistanceToHitSqr < ClosestHitDistanceSqr) {
// 				ClosestHitDistanceSqr = DistanceToHitSqr;
// 				Location = Hit.Location;
// 				Normal = Hit.Normal.GetSafeNormal();
// 			}
// 		}
// 		//FPlane Plane(Location, Normal);
// 		//Location = Location + Plane * (FVector::BoxPushOut(Plane, Extend));
// 	}
// 	else {
// 		const float DistanceMultiplier = (InViewportClient->GetViewportType() == LVT_Perspective) ? GetDefault<ULevelEditorViewportSettings>()->BackgroundDropDistance : 0.0f;
// 		Location = Cursor.GetOrigin() + Cursor.GetDirection() * 33.f + Cursor.GetDirection() * DistanceMultiplier;
// 	}
// 
// 	FSnappingUtils::SnapPointToGrid(Location, FVector::ZeroVector);
// 	return Location;
// }