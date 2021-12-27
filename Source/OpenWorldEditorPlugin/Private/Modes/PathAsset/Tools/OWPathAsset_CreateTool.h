// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "BaseBehaviors/BehaviorTargetInterfaces.h"
#include "OWPathAsset_BaseTool.h"
#include "OWPathAsset_CreateTool.generated.h"


/**
 * Builder for UOWPathAssetInteractiveTool
 */
UCLASS()
class UOWPathAsset_CreateToolBuilder : public UInteractiveToolBuilder
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
class UOWPathAsset_CreateTool : public UOWPathAsset_BaseTool, public IHoverBehaviorTarget, public IClickBehaviorTarget
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
    FHitResult TraceForNodePlacement(const FInputDeviceRay& PressPos) const;

    /* IHoverBehaviorTarget */
	virtual FInputRayHit BeginHoverSequenceHitTest(const FInputDeviceRay& PressPos) override { return  FInputRayHit(0.f); }
    virtual void OnBeginHover(const FInputDeviceRay& DevicePos) override {}
    virtual bool OnUpdateHover(const FInputDeviceRay& DevicePos) override;
	virtual void OnEndHover() override {}
	/* IHoverBehaviorTarget */

	/* IClickBehaviorTarget */
	virtual FInputRayHit IsHitByClick(const FInputDeviceRay& ClickPos) override { return  FInputRayHit(0.f); }
    virtual void OnClicked(const FInputDeviceRay& ClickPos) override;
	/* IClickBehaviorTarget */

private:
	void AddNodeAtLocation(const FVector& InNodeLocation) const;

	FVector PlaceHolderLocation;
};


