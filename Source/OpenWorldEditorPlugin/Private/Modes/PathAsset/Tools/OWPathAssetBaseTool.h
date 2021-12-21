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
class  UOWPathAssetBaseToolBuilder : public UInteractiveToolBuilder
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

	virtual void SetAsset(TWeakObjectPtr<class UOWPathAsset> InPathAsset) { TargetPathAsset = InPathAsset; };
	TWeakObjectPtr<class UOWPathAsset> GetAsset() const { return TargetPathAsset; }

	bool RemoveToolPropertySourceObject(UObject* PropertySet);
	void CleanToolPropertySource(TObjectPtr<UObject> ObjectToAdd = nullptr);

protected:
	UWorld* TargetWorld = nullptr;
	void RenderNode(const FColor DrawColor, UOWPathAssetNode* PathAssetNode, IToolsContextRenderAPI* RenderAPI) const;

private:
    TWeakObjectPtr<class UOWPathAsset> TargetPathAsset;
};


