// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "BaseBehaviors/BehaviorTargetInterfaces.h"
#include "Tools/InteractiveToolsCommands.h"
#include "OWPathAsset_BaseTool.h"
#include "BaseGizmos/TransformProxy.h"
#include "OWPathAsset_SelectTool.generated.h"


/**
 * Builder for UOWPathAssetInteractiveTool
 */
UCLASS()
class  UOWPathAsset_SelectToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};

/**
 * TInteractiveToolCommands implementation for this module that provides standard Editor hotkey support
 */
class FOWPathAsset_SelectToolActionCommands : public TInteractiveToolCommands<FOWPathAsset_SelectToolActionCommands>
{
public:
	FOWPathAsset_SelectToolActionCommands() :
		TInteractiveToolCommands<FOWPathAsset_SelectToolActionCommands>(
			TEXT("OpenWorldPathAssetSelectToolActions"),
			FText::FromString(TEXT("Open World Path Asset Editor")),
			NAME_None,
			FName("OpenWorldPlugin")
			)
	{};

	virtual void GetToolDefaultObjectList(TArray<UInteractiveTool*>& ToolCDOs) override;
};

UCLASS()
class UOWPathAssetSelectToolSelectionContext : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class UOWPathAsset_SelectTool> OwningTool;

	UPROPERTY()
    TObjectPtr<class UOWPathAsset> PathAsset;

	UPROPERTY()
	TObjectPtr<class UOWPathAssetNode> PathAssetNode;

	UPROPERTY()
    TObjectPtr<class UCombinedTransformGizmo> TransformGizmo;
	
	void Setup(class UOWPathAsset_SelectTool* InOwningTool);
    void Shutdown();

    void SelectAsset(class UOWPathAsset* InPathAsset);
	class UOWPathAsset* GetSelectedAsset() const;
	bool IsAssetSelected() const;

	void SelectNode(class UOWPathAssetNode* InPathAssetNode, bool ForceUpdate = false);
	class UOWPathAssetNode* GetSelectedNode() const;
	bool IsNodeSelected() const;
	void DeleteSelectedNode();
	void FocusSelectedNode() const;

	void CreateTransformGizmo();
    void DoMoveNode(UTransformProxy* Proxy, FTransform NewTransform);

    virtual void PostEditUndo() override;
};

UCLASS()
class UOWPathAssetNodeTransformProxy: public UTransformProxy
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UOWPathAssetNode* PathAssetNodeRef;
};

UCLASS()
class UOWPathAsset_SelectTool : public UOWPathAsset_BaseTool
{
	GENERATED_BODY()

public:
	virtual void SetAsset(TWeakObjectPtr<UOWPathAsset> InPathAsset) override;
	virtual void Setup() override;
	virtual void Shutdown(EToolShutdownType ShutdownType) override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;

	friend UOWPathAssetSelectToolSelectionContext;

protected:
    virtual void RegisterActions(FInteractiveToolActionSet& ActionSet) override;

	void DoSelectAction();
	void DoFocusAction() const;
	void DoSnapAction() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<class UCombinedTransformGizmo> TransformGizmo;

	UPROPERTY(Transient)
	TObjectPtr<UOWPathAssetSelectToolSelectionContext> SelectionContext;
};


