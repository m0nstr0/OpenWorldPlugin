// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetSelectTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "OWPathAssetHitProxies.h"
#include "OWPathAsset.h"
#include "OWPathAssetNode.h"
#include "CollisionQueryParams.h"
#include "SceneManagement.h"
#include "LevelEditorViewport.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/TransformProxy.h"


UInteractiveTool* UOWPathAssetSelectToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAssetSelectTool* NewTool = NewObject<UOWPathAssetSelectTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

void FOWPathAssetSelectToolActionCommands::GetToolDefaultObjectList(TArray<UInteractiveTool*>& ToolCDOs)
{
	ToolCDOs.Add(GetMutableDefault<UOWPathAssetSelectTool>());
}

void UOWPathAssetSelectTool::OnPathAssetChanged(UOWPathAsset* InPathAsset)
{
    Super::OnPathAssetChanged(InPathAsset);
	SelectionContext->SelectAsset(InPathAsset);
}

void UOWPathAssetSelectTool::Setup()
{
	UInteractiveTool::Setup();

	SelectionContext = NewObject<UOWPathAssetSelectionContext>(this);
	SelectionContext->SetFlags(EObjectFlags::RF_Transactional);
	SelectionContext->Setup(this);
}

void UOWPathAssetSelectTool::Shutdown(EToolShutdownType ShutdownType)
{
	UInteractiveTool::Shutdown(ShutdownType);
    SelectionContext->Shutdown();
}

void UOWPathAssetSelectTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	UOWPathAssetBaseTool::Render(RenderAPI);

	if (!IsPathAssetSelected() || !SelectionContext->IsNodeSelected()) {
		return;
	}

    const TObjectPtr<UOWPathAsset> PathAsset = GetPathAsset();
    const TObjectPtr<UOWPathAssetNode> PathAssetNode = SelectionContext->GetSelectedNode();
	constexpr FColor DrawColor(255, 128, 0);
	const int32 CapsuleSides = FMath::Clamp<int32>(PathAsset->Radius / 4.f, 16, 64);

	FPrimitiveDrawInterface* PDI = RenderAPI->GetPrimitiveDrawInterface();

	PDI->SetHitProxy(new HOWPathAssetNodeHitProxy(PathAssetNode));
	DrawWireCapsule(PDI, PathAssetNode->Location, FVector::ForwardVector, FVector::RightVector, FVector::UpVector, DrawColor, PathAsset->Radius, PathAsset->Height / 2.f, CapsuleSides, SDPG_Foreground);
	PDI->DrawPoint(PathAssetNode->Location, DrawColor, 30.f, SDPG_Foreground);
	PDI->SetHitProxy(nullptr);
}


void UOWPathAssetSelectTool::RegisterActions(FInteractiveToolActionSet& ActionSet)
{
	ActionSet.RegisterAction(this, static_cast<int32>(EStandardToolActions::BaseClientDefinedActionID) + 1,
		TEXT("Select"),
		FText::FromString("Select"),
		FText::FromString("Select Path Node"),
		EModifierKey::None, EKeys::LeftMouseButton,
		[this]() { DoSelectAction(); });

	ActionSet.RegisterAction(this, static_cast<int32>(EStandardToolActions::BaseClientDefinedActionID) + 2,
		TEXT("Focus"),
		FText::FromString("Focus"),
		FText::FromString("Focus Selected Node"),
		EModifierKey::None, EKeys::F,
		[this]() { SelectionContext->FocusSelectedNode(); });

	ActionSet.RegisterAction(this, static_cast<int32>(EStandardToolActions::BaseClientDefinedActionID) + 3,
		TEXT("Snap"),
		FText::FromString("Snap"),
		FText::FromString("Snap Selected Node To The Ground"),
		EModifierKey::None, EKeys::End,
		[this]() { DoSnapAction(); });

	ActionSet.RegisterAction(this, static_cast<int32>(EStandardToolActions::BaseClientDefinedActionID) + 4,
		TEXT("Delete"),
		FText::FromString("Delete"),
		FText::FromString("Delete Selected Node"),
		EModifierKey::None, EKeys::Delete,
		[this]() { SelectionContext->DeleteSelectedNode(); });
}

void UOWPathAssetSelectTool::DoSelectAction()
{
	FViewport* Viewport = GEditor->GetActiveViewport();
	if (!Viewport) {
		return;
	}
	 
	HHitProxy* HitProxy = Viewport->GetHitProxy(Viewport->GetMouseX(), Viewport->GetMouseY());
	if (HitProxy && HitProxy->IsA(HOWPathAssetNodeHitProxy::StaticGetType())) {
        const HOWPathAssetNodeHitProxy* PathAssetEdModeHitProxy = static_cast<HOWPathAssetNodeHitProxy*>(HitProxy);
		SelectionContext->SelectNode(Cast<UOWPathAssetNode>(PathAssetEdModeHitProxy->RefObject));
	}
}

void UOWPathAssetSelectTool::DoSnapAction() const
{

}

void UOWPathAssetSelectionContext::Setup(UOWPathAssetSelectTool* InOwningTool)
{
	OwningTool = InOwningTool;

    UInteractiveGizmoManager* GizmoManager = OwningTool->GetToolManager()->GetPairedGizmoManager();
	TransformGizmo = GizmoManager->CreateCustomTransformGizmo(ETransformGizmoSubElements::TranslateAllAxes, this);
	TransformGizmo->SetVisibility(false);
}

void UOWPathAssetSelectionContext::Shutdown()
{
	UInteractiveGizmoManager* GizmoManager = OwningTool->GetToolManager()->GetPairedGizmoManager();
	GizmoManager->DestroyAllGizmosByOwner(this);
	TransformGizmo = nullptr;
}

void UOWPathAssetSelectionContext::SelectAsset(UOWPathAsset* InPathAsset)
{
	PathAsset = InPathAsset;
	SelectNode(nullptr, true);
}

UOWPathAsset* UOWPathAssetSelectionContext::GetSelectedAsset() const
{
	return PathAsset;
}

bool UOWPathAssetSelectionContext::IsAssetSelected() const
{
	return PathAsset != nullptr;
}

void UOWPathAssetSelectionContext::SelectNode(UOWPathAssetNode* InPathAssetNode, bool ForceUpdate)
{
	if (!ForceUpdate && (!IsAssetSelected() || InPathAssetNode == PathAssetNode)) {
        return;
    }

	const FScopedTransaction Transaction(FText::FromString("Select Open World Path Asset Node"));

	Modify();
	PathAssetNode = InPathAssetNode;
	CreateTransformGizmo();
	OwningTool->CleanToolPropertySource(PathAssetNode);
}

UOWPathAssetNode* UOWPathAssetSelectionContext::GetSelectedNode() const
{
	return PathAssetNode;
}

bool UOWPathAssetSelectionContext::IsNodeSelected() const
{
	return PathAssetNode != nullptr;
}

void UOWPathAssetSelectionContext::DeleteSelectedNode()
{
	if (!IsNodeSelected() || !IsAssetSelected()) {
		return;
	}

	{
		const FScopedTransaction Transaction(FText::FromString("Delete Node"));
		PathAsset->Modify();
		PathAsset->Nodes.Remove(PathAssetNode);

		Modify();
		PathAssetNode = PathAsset->Nodes.Num() > 0 ? PathAsset->Nodes.Last() : nullptr;
		OwningTool->CleanToolPropertySource(PathAssetNode);
	}

	CreateTransformGizmo();
}

void UOWPathAssetSelectionContext::FocusSelectedNode() const
{
	if (!IsNodeSelected() || !IsAssetSelected()) {
		return;
	}

	if (GCurrentLevelEditingViewportClient) {
		GCurrentLevelEditingViewportClient->CenterViewportAtPoint(PathAssetNode->Location, false);
	}
}

void UOWPathAssetSelectionContext::CreateTransformGizmo()
{
	if (!IsAssetSelected() || !IsNodeSelected()) {
		TransformGizmo->SetVisibility(false);
		return;
	}

	UOWPathAssetNodeTransformProxy* Proxy = NewObject<UOWPathAssetNodeTransformProxy>(this);
	Proxy->PathAssetNodeRef = nullptr;
	Proxy->OnTransformChanged.AddUObject(this, &UOWPathAssetSelectionContext::DoMoveNode);
	//Proxy->OnTransformChangedUndoRedo
	Proxy->PathAssetNodeRef = PathAssetNode;
	Proxy->SetTransform(FTransform(PathAssetNode->Location));

    TransformGizmo->SetActiveTarget(Proxy);
	TransformGizmo->SetVisibility(true);
}

void UOWPathAssetSelectionContext::DoMoveNode(UTransformProxy* TransformProxy, FTransform NewTransform)
{
	UOWPathAssetNodeTransformProxy* Proxy = Cast<UOWPathAssetNodeTransformProxy>(TransformProxy);
	if (Proxy->PathAssetNodeRef) {
		Proxy->PathAssetNodeRef->Location = NewTransform.GetLocation();
		if (TransformGizmo->ActiveTarget != Proxy) {
			TransformGizmo->SetActiveTarget(Proxy);
			TransformGizmo->SetVisibility(true);
		}
	}
}

void UOWPathAssetSelectionContext::PostEditUndo()
{
	UObject::PostEditUndo();
	OwningTool->CleanToolPropertySource(PathAssetNode);
	CreateTransformGizmo();
}
