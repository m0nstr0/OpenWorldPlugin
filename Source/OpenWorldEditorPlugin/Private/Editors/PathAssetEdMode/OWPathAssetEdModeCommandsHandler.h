#pragma once

#include "CoreMinimal.h"

class FEditorViewportClient;
class FOWPathAssetEdMode;
class UOWPathAsset;
class UOWPathAssetNode;
class FUICommandList;

class FOWPathAssetEdModeCommandsHandler
{
public:
	TSharedPtr<FUICommandList> UICommandList;
public:
	FOWPathAssetEdModeCommandsHandler();
	~FOWPathAssetEdModeCommandsHandler();
	void Clean();
	void BindCommands();
	bool IsSomethingSelected() const;
	TArray<TWeakObjectPtr<UOWPathAssetNode>>& GetSelectedNodes();
	const TWeakObjectPtr<UOWPathAssetNode> GetLastValidSelectedNode() const;
	void OnPathAssetChanged(UOWPathAsset* InPathAsset);
private:
	void DoDeleteNodeCommand();
	void DoSnapNodeCommand();
	void DoSelectCommand();
	void DoAddToSelectCommand();
	void DoAddConnectedNodeCommand();
	void DoAddUnConnectedNodeCommand();

	void CleanUnReferencedObjects();
	UOWPathAssetNode* GetObjectFromHitProxy(FEditorViewportClient* ViewportClient);
	FEditorViewportClient* GetViewportClient() const;
	FOWPathAssetEdMode* GetEditorMode() const;
private:
	TWeakObjectPtr<UOWPathAsset> PathAsset;
	TArray<TWeakObjectPtr<UOWPathAssetNode>> Nodes;
};