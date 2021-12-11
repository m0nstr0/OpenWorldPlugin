// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "OWPathAsset.h"
#include "OWPathAssetEdModeCommandsHandler.h"

/**
 * 
 */

class AOWPathAssetEdModeActor;

class FOWPathAssetEdMode: public FEdMode
{
public:
	const static FEditorModeID EM_OWPathAssetEdModeId;

public:
	FOWPathAssetEdMode();
	virtual ~FOWPathAssetEdMode();

	void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;

	void AddPathAssetNode(FVector InLocation);
	
	void SelectPathAsset(UOWPathAsset* InPathAsset) { SelectedPathAsset = InPathAsset; }
	TWeakObjectPtr<UOWPathAsset> GetSelectedPathAsset() const { return SelectedPathAsset; }
	
	virtual bool MouseEnter(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) { bIsMouseInsideViewport = true; 	return true; };
	virtual bool MouseLeave(FEditorViewportClient* ViewportClient, FViewport* Viewport)	{ bIsMouseInsideViewport = false; return true;	};
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y);

	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event);

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	

	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;

	bool UsesToolkits() const override { return true; };
	virtual bool ShowModeWidgets() const override { return true; };
	virtual bool ShouldDrawWidget() const override { return true; };
	virtual bool UsesTransformWidget() const override;
	virtual FVector GetWidgetLocation() const override;

	void DoDeleteNodeCommand();
	void DoSnapNodeCommand();
	void DoSelectCommand();
	void DoAddConnectedNodeCommand();
	void DoAddUnConnectedNodeCommand();
private:
	FVector TraceForPlacement(FEditorViewportClient* InViewportClient);

private:
	FOWPathAssetEdModeCommandsHandler CommandsHandler;
	bool bIsMouseInsideViewport;
	bool bShowPathNodePlaceHolder;
	FVector PlaceHolderLocation;
	TWeakObjectPtr<UOWPathAsset> SelectedPathAsset;
	TWeakObjectPtr<UOWPathAssetNode> SelectedPathAssetNode;
	TSharedPtr<FUICommandList> UICommandList;
};