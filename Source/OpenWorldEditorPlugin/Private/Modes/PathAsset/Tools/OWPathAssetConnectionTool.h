// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "OWPathAssetBaseTool.h"
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

UCLASS()
class UOWPathAssetConnectionToolSelectionContext : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class UOWPathAssetConnectionTool> OwningTool;

	UPROPERTY()
	TObjectPtr<class UOWPathAsset> PathAsset;

	UPROPERTY()
	TObjectPtr<class UOWPathAssetNode> LeftPathAssetNode;

	UPROPERTY()
	TObjectPtr<class UOWPathAssetNode> RightPathAssetNode;

public:
	void Setup(class UOWPathAssetConnectionTool* InOwningTool);
	void Shutdown();

	void SelectAsset(class UOWPathAsset* InPathAsset);
	class UOWPathAsset* GetSelectedAsset() const;
	bool IsAssetSelected() const;

	void SelectNode(class UOWPathAssetNode* InPathAssetNode);
	class UOWPathAssetNode* GetSelectedNode(bool IsLeftNode = true) const;

	bool IsNodeSelected(bool IsLeftNode = true) const;
	virtual void PostEditUndo() override;
    void LinkNodes();
    void UnlinkNodes();
};

/**
 *
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FOWPathAssetConnectionToolNodeSelected, const TObjectPtr<class UOWPathAssetNode> LeftNode, const TObjectPtr<class UOWPathAssetNode> RightNode)

UCLASS()
class UOWPathAssetConnectionTool : public UOWPathAssetBaseTool, public IClickBehaviorTarget
{
	GENERATED_BODY()

    friend UOWPathAssetConnectionToolSelectionContext;
public:
	/* UOWPathAssetBaseTool */
    virtual void Setup() override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	virtual void OnPathAssetChanged(UOWPathAsset* InPathAsset) override;
	/* UOWPathAssetBaseTool */

	/* IClickBehaviorTarget */
    virtual FInputRayHit IsHitByClick(const FInputDeviceRay& ClickPos) override;
    virtual void OnClicked(const FInputDeviceRay& ClickPos) override;
	/* IClickBehaviorTarget */

	void LinkSelectedNodes();
	void UnlinkSelectedNodes();

	FOWPathAssetConnectionToolNodeSelected OnPathAssetNodeSelected;
private:
	UPROPERTY(Transient)
    TObjectPtr<UOWPathAssetConnectionToolSelectionContext> SelectionContext;
};


