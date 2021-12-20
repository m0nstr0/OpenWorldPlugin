// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetConnectionTool.h"
#include "InteractiveToolManager.h"
#include "OWPathAsset.h"
#include "OWPathAssetHitProxies.h"
#include "OWPathAssetLink.h"
#include "BaseBehaviors/SingleClickBehavior.h"

/*
 * ToolBuilder
 */
UInteractiveTool* UOWPathAssetConnectionToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAssetBaseTool* NewTool = NewObject<UOWPathAssetConnectionTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

void UOWPathAssetConnectionTool::Setup()
{
	UInteractiveTool::Setup();
	
	USingleClickInputBehavior* MouseClickBehavior = NewObject<USingleClickInputBehavior>();
	MouseClickBehavior->Initialize(this);
	AddInputBehavior(MouseClickBehavior);

	SelectionContext = NewObject<UOWPathAssetConnectionToolSelectionContext>(this);
	SelectionContext->SetFlags(RF_Transactional);
	SelectionContext->Setup(this);
}

void UOWPathAssetConnectionTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!IsPathAssetSelected())	{
		return;
	}

	Super::Render(RenderAPI);

	if (SelectionContext->IsNodeSelected())	{
		DrawNode(FColor::Green, SelectionContext->GetSelectedAsset(), SelectionContext->GetSelectedNode(), RenderAPI);
	}

	if (SelectionContext->IsNodeSelected(false)) {
		DrawNode(FColor::Blue, SelectionContext->GetSelectedAsset(), SelectionContext->GetSelectedNode(false), RenderAPI);
	}
}

void UOWPathAssetConnectionTool::OnPathAssetChanged(UOWPathAsset* InPathAsset)
{
    Super::OnPathAssetChanged(InPathAsset);
	SelectionContext->SelectAsset(InPathAsset);
}

FInputRayHit UOWPathAssetConnectionTool::IsHitByClick(const FInputDeviceRay& ClickPos)
{
    return FInputRayHit(0.f);
}

void UOWPathAssetConnectionTool::OnClicked(const FInputDeviceRay& ClickPos)
{
	if (FViewport* Viewport = GEditor->GetActiveViewport()) {
		HHitProxy* HitProxy = Viewport->GetHitProxy(Viewport->GetMouseX(), Viewport->GetMouseY());
		if (HitProxy && HitProxy->IsA(HOWPathAssetNodeHitProxy::StaticGetType())) {
			return SelectionContext->SelectNode(static_cast<HOWPathAssetNodeHitProxy*>(HitProxy)->RefObject);
		}
	}
	SelectionContext->SelectNode(nullptr);
}

void UOWPathAssetConnectionTool::LinkSelectedNodes()
{
	SelectionContext->LinkNodes();
}

void UOWPathAssetConnectionTool::UnlinkSelectedNodes()
{
	SelectionContext->UnlinkNodes();
}


void UOWPathAssetConnectionToolSelectionContext::Setup(UOWPathAssetConnectionTool* InOwningTool)
{
	OwningTool = InOwningTool;
}

void UOWPathAssetConnectionToolSelectionContext::Shutdown()
{}

void UOWPathAssetConnectionToolSelectionContext::SelectAsset(UOWPathAsset* InPathAsset)
{
	PathAsset = InPathAsset;
	//SelectNode(nullptr, true);
}

UOWPathAsset* UOWPathAssetConnectionToolSelectionContext::GetSelectedAsset() const
{
	return PathAsset;
}

bool UOWPathAssetConnectionToolSelectionContext::IsAssetSelected() const
{
	return PathAsset != nullptr;
}

void UOWPathAssetConnectionToolSelectionContext::SelectNode(UOWPathAssetNode* InPathAssetNode)
{
    if (!IsAssetSelected()) {
		return;
	}

	const FScopedTransaction Transaction(FText::FromString("Select OpenWorld PathAsset Node"));

	Modify();

	if (InPathAssetNode)
    {
        if (LeftPathAssetNode == InPathAssetNode || RightPathAssetNode == InPathAssetNode)
        {
            LeftPathAssetNode = LeftPathAssetNode == InPathAssetNode ? nullptr : LeftPathAssetNode;
            RightPathAssetNode = RightPathAssetNode == InPathAssetNode ? nullptr : RightPathAssetNode;
        }
        else if (LeftPathAssetNode)
        {
            RightPathAssetNode = InPathAssetNode;
        }
        else if (RightPathAssetNode)
        {
            LeftPathAssetNode = InPathAssetNode;
        }
        else if (!LeftPathAssetNode)
        {
            LeftPathAssetNode = InPathAssetNode;
        }
        else if (!RightPathAssetNode)
        {
            RightPathAssetNode = InPathAssetNode;
        }

		if (!LeftPathAssetNode && RightPathAssetNode)
		{
			LeftPathAssetNode = RightPathAssetNode;
			RightPathAssetNode = nullptr;
		}
    }
    else
    {
        LeftPathAssetNode = nullptr;
        RightPathAssetNode = nullptr;
    }

	OwningTool->OnPathAssetNodeSelected.Broadcast(LeftPathAssetNode, RightPathAssetNode);
}

UOWPathAssetNode* UOWPathAssetConnectionToolSelectionContext::GetSelectedNode(const bool IsLeftNode) const
{
	return IsLeftNode ? LeftPathAssetNode : RightPathAssetNode;
}

bool UOWPathAssetConnectionToolSelectionContext::IsNodeSelected(const bool IsLeftNode) const
{
	if (IsLeftNode) {
		return LeftPathAssetNode != nullptr;
	}

    return RightPathAssetNode != nullptr;
}

void UOWPathAssetConnectionToolSelectionContext::PostEditUndo()
{
	UObject::PostEditUndo();
}

void UOWPathAssetConnectionToolSelectionContext::LinkNodes()
{
    if (!IsAssetSelected() || !IsNodeSelected() || !IsNodeSelected(false)) {
        return;
    }

	TObjectPtr<UOWPathAsset> Asset = GetSelectedAsset();
	TObjectPtr<UOWPathAssetNode> LeftNode = GetSelectedNode();
	TObjectPtr<UOWPathAssetNode> RightNode = GetSelectedNode(false);

	const FScopedTransaction Transaction(FText::FromString("Link OpenWorld PathAsset Nodes"));

	Asset->Modify();
	UOWPathAssetLink* Link = NewObject<UOWPathAssetLink>(Asset);
	Link->SetFlags(RF_Transactional);
	Link->LeftNode = LeftNode;
	Link->RightNode = RightNode;
	Asset->Links.Add(Link);
}

void UOWPathAssetConnectionToolSelectionContext::UnlinkNodes()
{

}