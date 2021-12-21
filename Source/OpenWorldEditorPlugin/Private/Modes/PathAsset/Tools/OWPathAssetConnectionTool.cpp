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


/*
 * ConnectionTool
 */
void UOWPathAssetConnectionTool::Setup()
{
	UInteractiveTool::Setup();
	
	USingleClickInputBehavior* MouseClickBehavior = NewObject<USingleClickInputBehavior>();
	MouseClickBehavior->Initialize(this);
	AddInputBehavior(MouseClickBehavior);
}

void UOWPathAssetConnectionTool::Shutdown(EToolShutdownType ShutdownType)
{
    Super::Shutdown(ShutdownType);
}

void UOWPathAssetConnectionTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!GetAsset().IsValid()) {
		return;
	}

	Super::Render(RenderAPI);

	if (LeftNode.IsValid())	{
		RenderNode(FColor::Green, LeftNode.Get(), RenderAPI);
	}

	if (RightNode.IsValid()) {
		RenderNode(FColor::Blue, RightNode.Get(), RenderAPI);
	}
}

void UOWPathAssetConnectionTool::OnClicked(const FInputDeviceRay& ClickPos)
{
	if (FViewport* Viewport = GEditor->GetActiveViewport()) {
		HHitProxy* HitProxy = Viewport->GetHitProxy(Viewport->GetMouseX(), Viewport->GetMouseY());
		if (HitProxy && HitProxy->IsA(HOWPathAssetNodeHitProxy::StaticGetType())) {
			return SetNode(static_cast<HOWPathAssetNodeHitProxy*>(HitProxy)->RefObject);
		}
	}
	SetNode(nullptr);
}

void UOWPathAssetConnectionTool::OnNodeChanged_Internal() const
{
    EOWPathAssetDirectionType DirectionContext = EOWPathAssetDirectionType::LAR;
    UOWPathAssetLink* Link = GetAsset()->FindLink(LeftNode, RightNode, DirectionContext);
    OnNodeChanged.Broadcast(LeftNode, RightNode, Link != nullptr, DirectionContext);
}

void UOWPathAssetConnectionTool::SetNode(UOWPathAssetNode* InNode)
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
		MakeUnique<FOWPathAssetConnectionToolNodeSetChangeCommand>(LeftNode, RightNode, OldLeftNode, OldRightNode), 
		FText::FromString("Select Node")
	);

	OnNodeChanged_Internal();
}

void UOWPathAssetConnectionTool::Link(EOWPathAssetDirectionType Direction)
{
	if (!GetAsset().IsValid() || !LeftNode.IsValid() || !RightNode.IsValid()) {
		return;
	}

	if (UOWPathAssetLink* Link = GetAsset()->FindLink(LeftNode.Get(), RightNode.Get()))
	{
		//TODO: Rewrite
		if (Link->Direction != Direction) {
			GetToolManager()->EmitObjectChange(
				this,
				MakeUnique<FOWPathAssetConnectionToolLinkChangeDirectionCommand>(LeftNode, RightNode, Link->Direction, Direction),
				FText::FromString("Change Link Direction")
			);
			GetAsset()->MarkPackageDirty();
			Link->Direction = Direction;
			OnNodeChanged.Broadcast(LeftNode, RightNode, true, Direction);
		}
	} else if (Link_Internal(LeftNode, RightNode, Direction)) {
		GetToolManager()->EmitObjectChange(
			this,
			MakeUnique<FOWPathAssetConnectionToolLinkNodeChangeCommand>(LeftNode, RightNode, Direction, FOWPathAssetConnectionToolLinkNodeChangeCommand::OperationType::Link),
			FText::FromString("Link Node")
		);
	}
}

bool UOWPathAssetConnectionTool::Link_Internal(const TWeakObjectPtr<UOWPathAssetNode>& InLeftNode, const TWeakObjectPtr<UOWPathAssetNode>& InRightNode, EOWPathAssetDirectionType Direction) const
{
	if (!GetAsset().IsValid() || !InLeftNode.IsValid() || !InRightNode.IsValid()) {
		return false;
	}

	UOWPathAssetLink* Link = GetAsset()->Link(InLeftNode, InRightNode, Direction);
	OnNodeChanged.Broadcast(InLeftNode, InRightNode, Link != nullptr, Direction);
	return Link != nullptr;
}

void UOWPathAssetConnectionTool::Unlink()
{
	if (UOWPathAssetLink* Link = Unlink_Internal(LeftNode, RightNode)) {
		GetToolManager()->EmitObjectChange(
			this,
			MakeUnique<FOWPathAssetConnectionToolLinkNodeChangeCommand>(LeftNode, RightNode, Link->Direction, FOWPathAssetConnectionToolLinkNodeChangeCommand::OperationType::Unlink),
			FText::FromString("Unlink Node")
		);
	}
}

UOWPathAssetLink* UOWPathAssetConnectionTool::Unlink_Internal(const TWeakObjectPtr<UOWPathAssetNode>& InLeftNode, const TWeakObjectPtr<UOWPathAssetNode>& InRightNode) const
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

void UOWPathAssetConnectionTool::UndoNodeSet(const FOWPathAssetConnectionToolNodeSetChangeCommand* UndoCommand, bool IsUndo)
{
	LeftNode = IsUndo ? UndoCommand->OldLeftNode : UndoCommand->NewLeftNode;
	RightNode = IsUndo ? UndoCommand->OldRightNode : UndoCommand->NewRightNode;
	OnNodeChanged_Internal();
}

void UOWPathAssetConnectionTool::UndoLinkDirection(const FOWPathAssetConnectionToolLinkChangeDirectionCommand* UndoCommand, bool IsUndo) const
{
	if (IsUndo) {
		Link_Internal(UndoCommand->LeftNode, UndoCommand->RightNode, UndoCommand->OldDirection);
	} else {
		Link_Internal(UndoCommand->LeftNode, UndoCommand->RightNode, UndoCommand->NewDirection);
	}
}

void UOWPathAssetConnectionTool::UndoNodeLink(const FOWPathAssetConnectionToolLinkNodeChangeCommand* UndoCommand, bool IsUndo) const
{
	if (UndoCommand->Operation == FOWPathAssetConnectionToolLinkNodeChangeCommand::OperationType::Link) {
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
void FOWPathAssetConnectionToolNodeSetChangeCommand::Apply(UObject* Object)
{
	if (UOWPathAssetConnectionTool* Tool = Cast<UOWPathAssetConnectionTool>(Object)) {
		Tool->UndoNodeSet(this, false);
	}
}

void FOWPathAssetConnectionToolNodeSetChangeCommand::Revert(UObject* Object)
{
	if (UOWPathAssetConnectionTool* Tool = Cast<UOWPathAssetConnectionTool>(Object)) {
		Tool->UndoNodeSet(this, true);
	}
}

/*
 * ConnectionToolLinkNodeChangeCommand
 */
void FOWPathAssetConnectionToolLinkNodeChangeCommand::Apply(UObject* Object)
{
	if (const UOWPathAssetConnectionTool* Tool = Cast<UOWPathAssetConnectionTool>(Object)) {
		Tool->UndoNodeLink(this, false);
	}
}

void FOWPathAssetConnectionToolLinkNodeChangeCommand::Revert(UObject* Object)
{
	if (const UOWPathAssetConnectionTool* Tool = Cast<UOWPathAssetConnectionTool>(Object)) {
		Tool->UndoNodeLink(this, true);
	}
}

/*
 * ConnectionToolLinkChangeDirectionCommand
 */
void FOWPathAssetConnectionToolLinkChangeDirectionCommand::Apply(UObject* Object)
{
	if (const UOWPathAssetConnectionTool* Tool = Cast<UOWPathAssetConnectionTool>(Object)) {
		Tool->UndoLinkDirection(this, false);
	}
}

void FOWPathAssetConnectionToolLinkChangeDirectionCommand::Revert(UObject* Object)
{
	if (const UOWPathAssetConnectionTool* Tool = Cast<UOWPathAssetConnectionTool>(Object)) {
		Tool->UndoLinkDirection(this, true);
	}
}