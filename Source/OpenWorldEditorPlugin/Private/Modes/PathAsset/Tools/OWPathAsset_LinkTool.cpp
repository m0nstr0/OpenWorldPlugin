// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset_LinkTool.h"
#include "InteractiveToolManager.h"
#include "OWPathAsset.h"
#include "OWPathAssetHitProxies.h"
#include "OWPathAssetLink.h"
#include "BaseBehaviors/SingleClickBehavior.h"

/*
 * ToolBuilder
 */
UInteractiveTool* UOWPathAsset_LinkToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UOWPathAsset_BaseTool* NewTool = NewObject<UOWPathAsset_LinkTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}


/*
 * ConnectionTool
 */
void UOWPathAsset_LinkTool::Setup()
{
	UInteractiveTool::Setup();
	
	USingleClickInputBehavior* MouseClickBehavior = NewObject<USingleClickInputBehavior>();
	MouseClickBehavior->Initialize(this);
	AddInputBehavior(MouseClickBehavior);
}

void UOWPathAsset_LinkTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!GetAsset().IsValid()) {
		return;
	}

	UOWPathAsset_BaseTool::Render(RenderAPI);

	if (LeftNode.IsValid())	{
		RenderNode(FColor::Green, LeftNode.Get(), RenderAPI);
	}

	if (RightNode.IsValid()) {
		RenderNode(FColor::Blue, RightNode.Get(), RenderAPI);
	}

	if (LeftNode.IsValid() && RightNode.IsValid()) {
		bool bCondDraw = true;
		for (const TWeakObjectPtr<UOWPathAssetLink> Link : LeftNode->LinksFrom) {
		    if (Link->LeftNode == RightNode.Get() || Link->RightNode == RightNode.Get()) {
                const FColor DrawColor = Link->Direction == EOWPathAssetDirectionType::LAR ? FColor::Red : FColor::Green;
				RenderAPI->GetPrimitiveDrawInterface()->DrawLine(LeftNode->Location, RightNode->Location, DrawColor, SDPG_Foreground);
				bCondDraw = false;
				break;
		    }
		}
		if (bCondDraw) {
			for (const TWeakObjectPtr<UOWPathAssetLink> Link : RightNode->LinksFrom) {
				if (Link->LeftNode == LeftNode.Get() || Link->RightNode == LeftNode.Get()) {
                    const FColor DrawColor = Link->Direction == EOWPathAssetDirectionType::LAR ? FColor::Red : FColor::Blue;
					RenderAPI->GetPrimitiveDrawInterface()->DrawLine(LeftNode->Location, RightNode->Location, DrawColor, SDPG_Foreground);
					break;
				}
			}
		}
	}
}

void UOWPathAsset_LinkTool::OnClicked(const FInputDeviceRay& ClickPos)
{
	if (FViewport* Viewport = GEditor->GetActiveViewport()) {
		HHitProxy* HitProxy = Viewport->GetHitProxy(Viewport->GetMouseX(), Viewport->GetMouseY());
		if (HitProxy && HitProxy->IsA(HOWPathAssetNodeHitProxy::StaticGetType())) {
			return SetNode(static_cast<HOWPathAssetNodeHitProxy*>(HitProxy)->RefObject);
		}
	}
	SetNode(nullptr);
}

void UOWPathAsset_LinkTool::OnNodeChanged_Internal() const
{
    EOWPathAssetDirectionType DirectionContext = EOWPathAssetDirectionType::LAR;
    UOWPathAssetLink* Link = GetAsset()->FindLink(LeftNode, RightNode, DirectionContext);
    OnNodeChanged.Broadcast(LeftNode, RightNode, Link != nullptr, DirectionContext);
}

void UOWPathAsset_LinkTool::SetNode(UOWPathAssetNode* InNode)
{
    if (!GetAsset().IsValid()) {
		return;
	}

	TWeakObjectPtr<UOWPathAssetNode> OldLeftNode = LeftNode;
	TWeakObjectPtr<UOWPathAssetNode> OldRightNode = RightNode;

	if (InNode)
    {
        if (LeftNode == InNode || RightNode == InNode)
        {
            LeftNode = LeftNode == InNode ? nullptr : LeftNode;
            RightNode = RightNode == InNode ? nullptr : RightNode;
        }
        else if (LeftNode.IsValid())
        {
            RightNode = InNode;
        }
        else if (RightNode.IsValid())
        {
            LeftNode = InNode;
        }
        else if (!LeftNode.IsValid())
        {
            LeftNode = InNode;
        }
        else if (!RightNode.IsValid())
        {
            RightNode = InNode;
        }

		if (!LeftNode.IsValid() && RightNode.IsValid())
		{
			LeftNode = RightNode;
			RightNode = nullptr;
		}
    }
    else
    {
        LeftNode = nullptr;
        RightNode = nullptr;
    }

	GetToolManager()->EmitObjectChange(
		this, 
		MakeUnique<FOWPathAsset_LinkToolNodeSetChangeCommand>(LeftNode, RightNode, OldLeftNode, OldRightNode), 
		FText::FromString("Select Node")
	);

	OnNodeChanged_Internal();
}

void UOWPathAsset_LinkTool::Link(EOWPathAssetDirectionType Direction)
{
	if (!GetAsset().IsValid() || !LeftNode.IsValid() || !RightNode.IsValid()) {
		return;
	}

	const UOWPathAssetLink* Link = GetAsset()->FindLink(LeftNode.Get(), RightNode.Get());
	if (UOWPathAssetLink* NewLink = Link_Internal(LeftNode, RightNode, Direction)) {
		if (NewLink != Link) {
			GetToolManager()->EmitObjectChange(
				this,
				MakeUnique<FOWPathAsset_LinkToolLinkNodeChangeCommand>(LeftNode, RightNode, Direction, true),
				FText::FromString("Link Node")
			);
		} else {
			GetToolManager()->EmitObjectChange(
				this,
				MakeUnique<FOWPathAsset_LinkToolLinkChangeDirectionCommand>(LeftNode, RightNode, Link->Direction, Direction),
				FText::FromString("Change Link Direction")
			);
		}
	}
}

UOWPathAssetLink* UOWPathAsset_LinkTool::Link_Internal(const TWeakObjectPtr<UOWPathAssetNode>& InLeftNode,  const TWeakObjectPtr<UOWPathAssetNode>& InRightNode, EOWPathAssetDirectionType Direction) const
{
	if (!GetAsset().IsValid() || !InLeftNode.IsValid() || !InRightNode.IsValid()) {
		return nullptr;
	}

	UOWPathAssetLink* Link = GetAsset()->Link(InLeftNode, InRightNode, Direction);
	OnNodeChanged.Broadcast(InLeftNode, InRightNode, Link != nullptr, Direction);
	return Link;
}

void UOWPathAsset_LinkTool::Unlink()
{
	if (UOWPathAssetLink* Link = Unlink_Internal(LeftNode, RightNode)) {
		GetToolManager()->EmitObjectChange(
			this,
			MakeUnique<FOWPathAsset_LinkToolLinkNodeChangeCommand>(LeftNode, RightNode, Link->Direction, false),
			FText::FromString("Unlink Node")
		);
	}
}

UOWPathAssetLink* UOWPathAsset_LinkTool::Unlink_Internal(const TWeakObjectPtr<UOWPathAssetNode>& InLeftNode, const TWeakObjectPtr<UOWPathAssetNode>& InRightNode) const
{
	if (!GetAsset().IsValid() || !InLeftNode.IsValid() || !InRightNode.IsValid()) {
		return nullptr;
	}

	if (UOWPathAssetLink* Link = GetAsset()->Unlink(InLeftNode, InRightNode)) {
		OnNodeChanged.Broadcast(InLeftNode, InRightNode, false, Link->Direction);
		return Link;
	}

	return nullptr;
}

void UOWPathAsset_LinkTool::UndoNodeSet(const FOWPathAsset_LinkToolNodeSetChangeCommand* UndoCommand, bool IsUndo)
{
	LeftNode = IsUndo ? UndoCommand->OldLeftNode : UndoCommand->NewLeftNode;
	RightNode = IsUndo ? UndoCommand->OldRightNode : UndoCommand->NewRightNode;
	OnNodeChanged_Internal();
}

void UOWPathAsset_LinkTool::UndoLinkDirection(const FOWPathAsset_LinkToolLinkChangeDirectionCommand* UndoCommand, bool IsUndo) const
{
	if (IsUndo) {
		Link_Internal(UndoCommand->LeftNode, UndoCommand->RightNode, UndoCommand->OldDirection);
	} else {
		Link_Internal(UndoCommand->LeftNode, UndoCommand->RightNode, UndoCommand->NewDirection);
	}
}

void UOWPathAsset_LinkTool::UndoNodeLink(const FOWPathAsset_LinkToolLinkNodeChangeCommand* UndoCommand, bool IsUndo) const
{
	if (UndoCommand->IsLinkOperation) {
		if (IsUndo) {
			Unlink_Internal(UndoCommand->LeftNode, UndoCommand->RightNode);
		} else {
			Link_Internal(UndoCommand->LeftNode, UndoCommand->RightNode, UndoCommand->Direction);
		}
	} else {
		if (IsUndo) {
			Link_Internal(UndoCommand->LeftNode, UndoCommand->RightNode, UndoCommand->Direction);
		} else {
			Unlink_Internal(UndoCommand->LeftNode, UndoCommand->RightNode);
		}
	}
}


/*
 * ConnectionToolNodeSetChangeCommand
 */
void FOWPathAsset_LinkToolNodeSetChangeCommand::Apply(UObject* Object)
{
	if (UOWPathAsset_LinkTool* Tool = Cast<UOWPathAsset_LinkTool>(Object)) {
		Tool->UndoNodeSet(this, false);
	}
}

void FOWPathAsset_LinkToolNodeSetChangeCommand::Revert(UObject* Object)
{
	if (UOWPathAsset_LinkTool* Tool = Cast<UOWPathAsset_LinkTool>(Object)) {
		Tool->UndoNodeSet(this, true);
	}
}

/*
 * ConnectionToolLinkNodeChangeCommand
 */
void FOWPathAsset_LinkToolLinkNodeChangeCommand::Apply(UObject* Object)
{
	if (const UOWPathAsset_LinkTool* Tool = Cast<UOWPathAsset_LinkTool>(Object)) {
		Tool->UndoNodeLink(this, false);
	}
}

void FOWPathAsset_LinkToolLinkNodeChangeCommand::Revert(UObject* Object)
{
	if (const UOWPathAsset_LinkTool* Tool = Cast<UOWPathAsset_LinkTool>(Object)) {
		Tool->UndoNodeLink(this, true);
	}
}

/*
 * ConnectionToolLinkChangeDirectionCommand
 */
void FOWPathAsset_LinkToolLinkChangeDirectionCommand::Apply(UObject* Object)
{
	if (const UOWPathAsset_LinkTool* Tool = Cast<UOWPathAsset_LinkTool>(Object)) {
		Tool->UndoLinkDirection(this, false);
	}
}

void FOWPathAsset_LinkToolLinkChangeDirectionCommand::Revert(UObject* Object)
{
	if (const UOWPathAsset_LinkTool* Tool = Cast<UOWPathAsset_LinkTool>(Object)) {
		Tool->UndoLinkDirection(this, true);
	}
}