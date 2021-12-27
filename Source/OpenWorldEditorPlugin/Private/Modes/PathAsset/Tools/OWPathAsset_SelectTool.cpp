// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset_SelectTool.h"
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


/*
 * Tool Builder
 */
UInteractiveTool* UOWPathAsset_SelectToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAsset_SelectTool* NewTool = NewObject<UOWPathAsset_SelectTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

/*
 * FOWPathAsset_SelectToolActionCommands
 */
void FOWPathAsset_SelectToolActionCommands::GetToolDefaultObjectList(TArray<UInteractiveTool*>& ToolCDOs)
{
	ToolCDOs.Add(GetMutableDefault<UOWPathAsset_SelectTool>());
}

/*
 * UOWPathAsset_SelectTool
 */
void UOWPathAsset_SelectTool::SetAsset(TWeakObjectPtr<UOWPathAsset> InPathAsset)
{
    UOWPathAsset_BaseTool::SetAsset(InPathAsset);
	SelectionContext->SelectAsset(InPathAsset.Get());
}

void UOWPathAsset_SelectTool::Setup()
{
	UInteractiveTool::Setup();

	SelectionContext = NewObject<UOWPathAssetSelectToolSelectionContext>(this);
	SelectionContext->SetFlags(EObjectFlags::RF_Transactional);
	SelectionContext->Setup(this);
}

void UOWPathAsset_SelectTool::Shutdown(EToolShutdownType ShutdownType)
{
	UInteractiveTool::Shutdown(ShutdownType);
    SelectionContext->Shutdown();
}

void UOWPathAsset_SelectTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	UOWPathAsset_BaseTool::Render(RenderAPI);

	if (!GetAsset().IsValid() || !SelectionContext->IsNodeSelected()) {
		return;
	}

	RenderNode(FColor(255, 128, 0), SelectionContext->GetSelectedNode(), RenderAPI);
}


void UOWPathAsset_SelectTool::RegisterActions(FInteractiveToolActionSet& ActionSet)
{
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

void UOWPathAsset_SelectTool::DoSelectAction()
{
	FViewport* Viewport = GEditor->GetActiveViewport();
	if (!Viewport) {
		return;
	}
	 
	HHitProxy* HitProxy = Viewport->GetHitProxy(Viewport->GetMouseX(), Viewport->GetMouseY());
	if (HitProxy && HitProxy->IsA(HOWPathAsset_NodeHitProxy::StaticGetType())) {
        const HOWPathAsset_NodeHitProxy* PathAssetEdModeHitProxy = static_cast<HOWPathAsset_NodeHitProxy*>(HitProxy);
		SelectionContext->SelectNode(Cast<UOWPathAssetNode>(PathAssetEdModeHitProxy->RefObject));
	}
}

void UOWPathAsset_SelectTool::DoSnapAction() const
{

}

void UOWPathAssetSelectToolSelectionContext::Setup(UOWPathAsset_SelectTool* InOwningTool)
{
	OwningTool = InOwningTool;

    UInteractiveGizmoManager* GizmoManager = OwningTool->GetToolManager()->GetPairedGizmoManager();
	TransformGizmo = GizmoManager->CreateCustomTransformGizmo(ETransformGizmoSubElements::TranslateAllAxes, this);
	TransformGizmo->SetVisibility(false);
}

void UOWPathAssetSelectToolSelectionContext::Shutdown()
{
	UInteractiveGizmoManager* GizmoManager = OwningTool->GetToolManager()->GetPairedGizmoManager();
	GizmoManager->DestroyAllGizmosByOwner(this);
	TransformGizmo = nullptr;
}

void UOWPathAssetSelectToolSelectionContext::SelectAsset(UOWPathAsset* InPathAsset)
{
	PathAsset = InPathAsset;
	SelectNode(nullptr, true);
}

UOWPathAsset* UOWPathAssetSelectToolSelectionContext::GetSelectedAsset() const
{
	return PathAsset;
}

bool UOWPathAssetSelectToolSelectionContext::IsAssetSelected() const
{
	return PathAsset != nullptr;
}

void UOWPathAssetSelectToolSelectionContext::SelectNode(UOWPathAssetNode* InPathAssetNode, bool ForceUpdate)
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

UOWPathAssetNode* UOWPathAssetSelectToolSelectionContext::GetSelectedNode() const
{
	return PathAssetNode;
}

bool UOWPathAssetSelectToolSelectionContext::IsNodeSelected() const
{
	return PathAssetNode != nullptr;
}

void UOWPathAssetSelectToolSelectionContext::DeleteSelectedNode()
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

void UOWPathAssetSelectToolSelectionContext::FocusSelectedNode() const
{
	if (!IsNodeSelected() || !IsAssetSelected()) {
		return;
	}

	if (GCurrentLevelEditingViewportClient) {
		GCurrentLevelEditingViewportClient->CenterViewportAtPoint(PathAssetNode->Location, false);
	}
}

void UOWPathAssetSelectToolSelectionContext::CreateTransformGizmo()
{
	if (!IsAssetSelected() || !IsNodeSelected()) {
		TransformGizmo->SetVisibility(false);
		return;
	}

	UOWPathAssetNodeTransformProxy* Proxy = NewObject<UOWPathAssetNodeTransformProxy>(this);
	Proxy->PathAssetNodeRef = nullptr;
	Proxy->OnTransformChanged.AddUObject(this, &UOWPathAssetSelectToolSelectionContext::DoMoveNode);
	//Proxy->OnTransformChangedUndoRedo
	Proxy->PathAssetNodeRef = PathAssetNode;
	Proxy->SetTransform(FTransform(PathAssetNode->Location));

    TransformGizmo->SetActiveTarget(Proxy);
	TransformGizmo->SetVisibility(true);
}

void UOWPathAssetSelectToolSelectionContext::DoMoveNode(UTransformProxy* TransformProxy, FTransform NewTransform)
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

void UOWPathAssetSelectToolSelectionContext::PostEditUndo()
{
	UObject::PostEditUndo();
	OwningTool->CleanToolPropertySource(PathAssetNode);
	CreateTransformGizmo();
}
