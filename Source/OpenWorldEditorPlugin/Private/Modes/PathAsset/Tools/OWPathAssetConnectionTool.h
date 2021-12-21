// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "InteractiveToolChange.h"
#include "OWPathAssetBaseTool.h"
#include "OWPathAssetLink.h"
#include "OWPathAssetNode.h"
#include "BaseBehaviors/BehaviorTargetInterfaces.h"
#include "OWPathAssetConnectionTool.generated.h"


/**
 * Builder for UOWPathAssetConnectionTool
 */
UCLASS()
class  UOWPathAssetConnectionToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};


/**
 * Node selection transaction for UOWPathAssetConnectionTool
 */
class FOWPathAssetConnectionToolNodeSetChangeCommand : public FToolCommandChange
{
public:
	TWeakObjectPtr<class UOWPathAssetNode> NewLeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> NewRightNode;
	TWeakObjectPtr<class UOWPathAssetNode> OldLeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> OldRightNode;

	FOWPathAssetConnectionToolNodeSetChangeCommand(TWeakObjectPtr<class UOWPathAssetNode> InNewLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InNewRightNode, TWeakObjectPtr<class UOWPathAssetNode> InOldLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InOldRightNode) :
		NewLeftNode(InNewLeftNode), NewRightNode(InNewRightNode), OldLeftNode(InOldLeftNode), OldRightNode(InOldRightNode)
	{}

	virtual void Apply(UObject* Object) override;
	virtual void Revert(UObject* Object) override;
	virtual FString ToString() const override { return TEXT("ConnectionTool:FOWPathAssetConnectionToolNodeSetChangeCommand"); };
};

/**
 * Node link/unlink transaction for UOWPathAssetConnectionTool
 */
class FOWPathAssetConnectionToolLinkNodeChangeCommand : public FToolCommandChange
{
public:
	enum class OperationType
	{
	    Link,
		Unlink
	};

	TWeakObjectPtr<class UOWPathAssetNode> LeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> RightNode;
	EOWPathAssetDirectionType Direction;
	OperationType Operation;

	FOWPathAssetConnectionToolLinkNodeChangeCommand(TWeakObjectPtr<class UOWPathAssetNode> InLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InRightNode, EOWPathAssetDirectionType InDirection, OperationType InOperation) :
		LeftNode(InLeftNode), RightNode(InRightNode), Direction(InDirection), Operation(InOperation)
	{}

	virtual void Apply(UObject* Object) override;
	virtual void Revert(UObject* Object) override;
	virtual FString ToString() const override { return TEXT("ConnectionTool:FOWPathAssetConnectionToolLinkNodeChangeCommand"); };
};

/**
 * Node's link change direction transaction for UOWPathAssetConnectionTool
 */
class FOWPathAssetConnectionToolLinkChangeDirectionCommand : public FToolCommandChange
{
public:
	TWeakObjectPtr<class UOWPathAssetNode> LeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> RightNode;
	EOWPathAssetDirectionType NewDirection;
	EOWPathAssetDirectionType OldDirection;

	FOWPathAssetConnectionToolLinkChangeDirectionCommand(TWeakObjectPtr<class UOWPathAssetNode> InLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InRightNode, EOWPathAssetDirectionType InNewDirection, EOWPathAssetDirectionType InOldDirection) :
		LeftNode(InLeftNode), RightNode(InRightNode), NewDirection(InNewDirection), OldDirection(InOldDirection)
	{}

	virtual void Apply(UObject* Object) override;
	virtual void Revert(UObject* Object) override;
	virtual FString ToString() const override { return TEXT("ConnectionTool:FOWPathAssetConnectionToolLinkChangeDirectionCommand"); };
};


/**
 * UOWPathAssetConnectionTool
 */
UCLASS()
class UOWPathAssetConnectionTool : public UOWPathAssetBaseTool, public IClickBehaviorTarget
{
	GENERATED_BODY()

public:
	/* UOWPathAssetBaseTool */
    virtual void Setup() override;
	virtual void Shutdown(EToolShutdownType ShutdownType) override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	/* UOWPathAssetBaseTool */

	/* IClickBehaviorTarget */
    virtual FInputRayHit IsHitByClick(const FInputDeviceRay& ClickPos) override { return FInputRayHit(0.f); }
    virtual void OnClicked(const FInputDeviceRay& ClickPos) override;
	/* IClickBehaviorTarget */

	void SetNode(UOWPathAssetNode* InNode);
	void Link(EOWPathAssetDirectionType Direction = EOWPathAssetDirectionType::LAR);
	void Unlink();

	void UndoNodeSet(const FOWPathAssetConnectionToolNodeSetChangeCommand* UndoCommand, bool IsUndo);
    void UndoLinkDirection(const FOWPathAssetConnectionToolLinkChangeDirectionCommand* UndoCommand, bool IsUndo) const;
    void UndoNodeLink(const FOWPathAssetConnectionToolLinkNodeChangeCommand* UndoCommand, bool IsUndo) const;

	DECLARE_MULTICAST_DELEGATE_FourParams(FOWPathAssetConnectionToolNodeSelected, const TWeakObjectPtr<class UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<class UOWPathAssetNode>& RightNode, bool Linked, EOWPathAssetDirectionType DirectionType);
	FOWPathAssetConnectionToolNodeSelected OnNodeChanged;

private:
	UPROPERTY()
	TWeakObjectPtr<class UOWPathAssetNode> LeftNode;

	UPROPERTY()
    TWeakObjectPtr<class UOWPathAssetNode> RightNode;

	bool Link_Internal(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, EOWPathAssetDirectionType Direction) const;
	UOWPathAssetLink* Unlink_Internal(const TWeakObjectPtr<UOWPathAssetNode>& InLeftNode, const TWeakObjectPtr<UOWPathAssetNode>& InRightNode) const;
    void OnNodeChanged_Internal() const;
};


