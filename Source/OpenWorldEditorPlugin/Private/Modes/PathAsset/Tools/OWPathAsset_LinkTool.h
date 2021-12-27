// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "InteractiveToolChange.h"
#include "OWPathAsset_BaseTool.h"
#include "OWPathAssetLink.h"
#include "OWPathAssetNode.h"
#include "BaseBehaviors/BehaviorTargetInterfaces.h"
#include "OWPathAsset_LinkTool.generated.h"


/**
 * Builder for UOWPathAsset_LinkTool
 */
UCLASS()
class  UOWPathAsset_LinkToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};


/**
 * Node selection transaction for UOWPathAsset_LinkTool
 */
class FOWPathAsset_LinkToolNodeSetChangeCommand : public FToolCommandChange
{
public:
	TWeakObjectPtr<class UOWPathAssetNode> NewLeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> NewRightNode;
	TWeakObjectPtr<class UOWPathAssetNode> OldLeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> OldRightNode;

	FOWPathAsset_LinkToolNodeSetChangeCommand(TWeakObjectPtr<class UOWPathAssetNode> InNewLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InNewRightNode, TWeakObjectPtr<class UOWPathAssetNode> InOldLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InOldRightNode) :
		NewLeftNode(InNewLeftNode), NewRightNode(InNewRightNode), OldLeftNode(InOldLeftNode), OldRightNode(InOldRightNode)
	{}

	virtual void Apply(UObject* Object) override;
	virtual void Revert(UObject* Object) override;
	virtual FString ToString() const override { return TEXT("LinkTool:FOWPathAsset_LinkToolNodeSetChangeCommand"); };
};

/**
 * Node link/unlink transaction for UOWPathAsset_LinkTool
 */
class FOWPathAsset_LinkToolLinkNodeChangeCommand : public FToolCommandChange
{
public:
	TWeakObjectPtr<class UOWPathAssetNode> LeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> RightNode;
	EOWPathAssetDirectionType Direction;
	bool IsLinkOperation;

	FOWPathAsset_LinkToolLinkNodeChangeCommand(TWeakObjectPtr<class UOWPathAssetNode> InLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InRightNode, EOWPathAssetDirectionType InDirection, const bool InIsLinkOperation) :
		LeftNode(InLeftNode), RightNode(InRightNode), Direction(InDirection), IsLinkOperation(InIsLinkOperation)
	{}

	virtual void Apply(UObject* Object) override;
	virtual void Revert(UObject* Object) override;
	virtual FString ToString() const override { return TEXT("LinkTool:FOWPathAsset_LinkToolLinkNodeChangeCommand"); };
};

/**
 * Node's link change direction transaction for UOWPathAsset_LinkTool
 */
class FOWPathAsset_LinkToolLinkChangeDirectionCommand : public FToolCommandChange
{
public:
	TWeakObjectPtr<class UOWPathAssetNode> LeftNode;
	TWeakObjectPtr<class UOWPathAssetNode> RightNode;
	EOWPathAssetDirectionType NewDirection;
	EOWPathAssetDirectionType OldDirection;

	FOWPathAsset_LinkToolLinkChangeDirectionCommand(TWeakObjectPtr<class UOWPathAssetNode> InLeftNode, TWeakObjectPtr<class UOWPathAssetNode> InRightNode, EOWPathAssetDirectionType InNewDirection, EOWPathAssetDirectionType InOldDirection) :
		LeftNode(InLeftNode), RightNode(InRightNode), NewDirection(InNewDirection), OldDirection(InOldDirection)
	{}

	virtual void Apply(UObject* Object) override;
	virtual void Revert(UObject* Object) override;
	virtual FString ToString() const override { return TEXT("LinkTool:FOWPathAsset_LinkToolLinkChangeDirectionCommand"); };
};


/**
 * UOWPathAsset_LinkTool
 */
UCLASS()
class UOWPathAsset_LinkTool : public UOWPathAsset_BaseTool, public IClickBehaviorTarget
{
	GENERATED_BODY()

public:
	/* UOWPathAsset_BaseTool */
    virtual void Setup() override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	/* UOWPathAsset_BaseTool */

	/* IClickBehaviorTarget */
    virtual FInputRayHit IsHitByClick(const FInputDeviceRay& ClickPos) override { return FInputRayHit(0.f); }
    virtual void OnClicked(const FInputDeviceRay& ClickPos) override;
	/* IClickBehaviorTarget */

	void SetNode(UOWPathAssetNode* InNode);
	void Link(EOWPathAssetDirectionType Direction = EOWPathAssetDirectionType::LAR);
	void Unlink();

	void UndoNodeSet(const FOWPathAsset_LinkToolNodeSetChangeCommand* UndoCommand, bool IsUndo);
    void UndoLinkDirection(const FOWPathAsset_LinkToolLinkChangeDirectionCommand* UndoCommand, bool IsUndo) const;
    void UndoNodeLink(const FOWPathAsset_LinkToolLinkNodeChangeCommand* UndoCommand, bool IsUndo) const;

	DECLARE_MULTICAST_DELEGATE_FourParams(FOWPathAsset_LinkToolOnNodeSelected, const TWeakObjectPtr<class UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<class UOWPathAssetNode>& RightNode, bool Linked, EOWPathAssetDirectionType DirectionType);
	FOWPathAsset_LinkToolOnNodeSelected OnNodeChanged;

private:
	UPROPERTY()
	TWeakObjectPtr<class UOWPathAssetNode> LeftNode;

	UPROPERTY()
    TWeakObjectPtr<class UOWPathAssetNode> RightNode;

    UOWPathAssetLink* Link_Internal(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, EOWPathAssetDirectionType Direction) const;
	UOWPathAssetLink* Unlink_Internal(const TWeakObjectPtr<UOWPathAssetNode>& InLeftNode, const TWeakObjectPtr<UOWPathAssetNode>& InRightNode) const;
    void OnNodeChanged_Internal() const;
};


