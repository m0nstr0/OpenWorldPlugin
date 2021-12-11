
#include "OWPathAssetEdModeCommandsHandler.h"
#include "Editors/PathAssetEdMode/OWPathAssetEdMode.h"
#include "Framework/Commands/UICommandList.h"
#include "OWPathAssetNode.h"
#include "OWPathAsset.h"
#include "OWPathAssetEdModeCommands.h"

FOWPathAssetEdModeCommandsHandler::FOWPathAssetEdModeCommandsHandler()
{
	FOWPathAssetEdModeCommands::Register();
	UICommandList = MakeShareable(new FUICommandList);
	Clean();
}

FOWPathAssetEdModeCommandsHandler::~FOWPathAssetEdModeCommandsHandler()
{
	FOWPathAssetEdModeCommands::Unregister();
	Clean();
}

void FOWPathAssetEdModeCommandsHandler::Clean()
{
	Nodes.Empty();
	PathAsset = nullptr;
}

void FOWPathAssetEdModeCommandsHandler::BindCommands()
{
	UICommandList->MapAction(
		FOWPathAssetEdModeCommands::Get().SnapNodeToGroundCommand,
		FExecuteAction::CreateRaw(this, &FOWPathAssetEdModeCommandsHandler::DoSnapNodeCommand)
	);

	UICommandList->MapAction(
		FOWPathAssetEdModeCommands::Get().DeleteNodeCommand,
		FExecuteAction::CreateRaw(this, &FOWPathAssetEdModeCommandsHandler::DoDeleteNodeCommand)
	);

	UICommandList->MapAction(
		FOWPathAssetEdModeCommands::Get().SelectCommand,
		FExecuteAction::CreateRaw(this, &FOWPathAssetEdModeCommandsHandler::DoSelectCommand)
	);

	UICommandList->MapAction(
		FOWPathAssetEdModeCommands::Get().AddConnectedNodeCommand,
		FExecuteAction::CreateRaw(this, &FOWPathAssetEdModeCommandsHandler::DoAddConnectedNodeCommand)
	);

	UICommandList->MapAction(
		FOWPathAssetEdModeCommands::Get().AddUnConnectedNodeCommand,
		FExecuteAction::CreateRaw(this, &FOWPathAssetEdModeCommandsHandler::DoAddUnConnectedNodeCommand)
	);
}

void FOWPathAssetEdModeCommandsHandler::DoDeleteNodeCommand()
{

}

void FOWPathAssetEdModeCommandsHandler::DoSnapNodeCommand()
{

}


UOWPathAssetNode* FOWPathAssetEdModeCommandsHandler::GetObjectFromHitProxy(FEditorViewportClient* ViewportClient)
{
	HHitProxy* HitProxy = ViewportClient->Viewport->GetHitProxy(ViewportClient->Viewport->GetMouseX(), ViewportClient->Viewport->GetMouseY());
	if (HitProxy && HitProxy->IsA(HOWPathAssetEdModeHitProxy::StaticGetType())) {
		HOWPathAssetEdModeHitProxy* PathAssetEdModeHitProxy = (HOWPathAssetEdModeHitProxy*)HitProxy;
		return Cast<UOWPathAssetNode>(PathAssetEdModeHitProxy->RefObject);
	}
	return nullptr;
}

void FOWPathAssetEdModeCommandsHandler::DoSelectCommand()
{
	FEditorViewportClient* ViewportClient = GetViewportClient();

	if (!PathAsset.IsValid() || !ViewportClient) {
		return;
	}

	UOWPathAssetNode* Node = GetObjectFromHitProxy(ViewportClient);
	if (Node) {
		Nodes.Empty();
		Nodes.Add(Node);
	}
}

void FOWPathAssetEdModeCommandsHandler::DoAddToSelectCommand()
{
	FEditorViewportClient* ViewportClient = GetViewportClient();

	if (!PathAsset.IsValid() || !ViewportClient) {
		return;
	}

	CleanUnReferencedObjects();

	UOWPathAssetNode* Node = GetObjectFromHitProxy(ViewportClient);
	if (Node) {
		Nodes.Add(Node);
	}
}

void FOWPathAssetEdModeCommandsHandler::DoAddConnectedNodeCommand()
{

}

void FOWPathAssetEdModeCommandsHandler::DoAddUnConnectedNodeCommand()
{

}


void FOWPathAssetEdModeCommandsHandler::CleanUnReferencedObjects()
{
	Nodes.RemoveAll([](TWeakObjectPtr<UOWPathAssetNode> Node)->bool {return !Node.IsValid(); });
}

bool FOWPathAssetEdModeCommandsHandler::IsSomethingSelected() const
{
	return Nodes.Num() > 0;
}

TArray<TWeakObjectPtr<UOWPathAssetNode>>& FOWPathAssetEdModeCommandsHandler::GetSelectedNodes()
{
	return Nodes;
}

const TWeakObjectPtr<UOWPathAssetNode> FOWPathAssetEdModeCommandsHandler::GetLastValidSelectedNode() const
{
	for (auto i = Nodes.Num() - 1; i >= 0; i--) {
		if (Nodes.Last(i).IsValid()) {
			return Nodes.Last(i);
		}
	}

	return nullptr;
}

void FOWPathAssetEdModeCommandsHandler::OnPathAssetChanged(UOWPathAsset* InPathAsset)
{
	PathAsset = InPathAsset;
}

FEditorViewportClient* FOWPathAssetEdModeCommandsHandler::GetViewportClient() const
{
	return (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
}

FOWPathAssetEdMode* FOWPathAssetEdModeCommandsHandler::GetEditorMode() const
{
	return (FOWPathAssetEdMode*)GLevelEditorModeTools().GetActiveMode(FOWPathAssetEdMode::EM_OWPathAssetEdModeId);
}

