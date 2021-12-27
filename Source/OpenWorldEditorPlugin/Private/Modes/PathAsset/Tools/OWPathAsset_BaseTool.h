// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "OWPathAssetLink.h"
#include "OWPathAsset_BaseTool.generated.h"


/**
 * Builder for UOWPathAsset_BaseTool
 */
UCLASS()
class  UOWPathAsset_BaseToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};

/**
 * Base tool for UOWPathAsset_EdMode's tools
 */
UCLASS()
class UOWPathAsset_BaseTool : public UInteractiveTool
{
	GENERATED_BODY()

public:
	const FColor DefaultColor = FColor::White;
	const FColor FirstColor = FColor::Green;
	const FColor SecondColor = FColor::Blue;
	const FColor ThirdColor = FColor::Red;
	const FColor FourthColor = FColor::Yellow;

	/* UInteractiveTool */
	void SetWorld(UWorld* World);
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	/* UInteractiveTool */

	virtual void SetAsset(TWeakObjectPtr<class UOWPathAsset> InPathAsset) { TargetPathAsset = InPathAsset; };
	TWeakObjectPtr<class UOWPathAsset> GetAsset() const { return TargetPathAsset; }

	bool RemoveToolPropertySourceObject(UObject* PropertySet);
	void CleanToolPropertySource(TObjectPtr<UObject> ObjectToAdd = nullptr);

protected:
	UWorld* TargetWorld = nullptr;
	void RenderNode(const FColor DrawColor, class UOWPathAssetNode* PathAssetNode, IToolsContextRenderAPI* RenderAPI) const;
    void RenderNode(const FColor DrawColor, const FVector& Location, IToolsContextRenderAPI* RenderAPI) const;
	void RenderLink(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, const EOWPathAssetDirectionType Direction, IToolsContextRenderAPI* RenderAPI) const;

private:
    TWeakObjectPtr<class UOWPathAsset> TargetPathAsset;
};


