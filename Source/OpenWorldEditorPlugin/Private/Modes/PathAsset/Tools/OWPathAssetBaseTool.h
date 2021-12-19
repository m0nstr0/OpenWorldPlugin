// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "OWPathAssetNode.h"
#include "OWPathAssetBaseTool.generated.h"


/**
 * Builder for UOWPathAssetInteractiveTool
 */
UCLASS()
class  UOWPathAssetInteractiveToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};

/**
 *
 */
UCLASS()
class UOWPathAssetBaseTool : public UInteractiveTool
{
	GENERATED_BODY()

public:
	/* UInteractiveTool */
	void SetWorld(UWorld* World);
	virtual void Setup() override;
	virtual void Shutdown(EToolShutdownType ShutdownType) override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	/* UInteractiveTool */

	virtual void OnPathAssetChanged(class UOWPathAsset* InPathAsset);
	bool RemoveToolPropertySourceObject(UObject* PropertySet);
	void CleanToolPropertySource(TObjectPtr<UObject> ObjectToAdd = nullptr);
protected:
	UWorld* TargetWorld = nullptr;
	bool IsPathAssetSelected() const { return PathAssetBeingEdited != nullptr; }
	TObjectPtr<class UOWPathAsset> GetPathAsset() const { return PathAssetBeingEdited; }
	void DrawNode(const FColor DrawColor, TObjectPtr<class UOWPathAsset> PathAsset, TObjectPtr<class UOWPathAssetNode> PathAssetNode, IToolsContextRenderAPI* RenderAPI) const;
private:
	UPROPERTY()
	TObjectPtr<class UOWPathAsset> PathAssetBeingEdited;
};


