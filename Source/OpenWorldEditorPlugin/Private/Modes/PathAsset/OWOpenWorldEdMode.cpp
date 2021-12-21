// Fill out your copyright notice in the Description page of Project Settings.

#include "OWOpenWorldEdMode.h"
#include "OWPathAssetEdModeCommands.h"
#include "OpenWorldEditorPluginStyle.h"
#include "Tools/OWPathAssetBaseTool.h"
#include "InteractiveToolManager.h"
#include "OWPathAssetLink.h"
#include "OWPathAssetEdModeToolkit.h"
#include "Tools/OWPathAssetConnectionTool.h"
#include "Tools/OWPathAssetSelectTool.h"
#include "Tools/OWPathAssetCreateNodeTool.h"

const FEditorModeID UOpenWorldEdMode::EM_OpenWorldEdModeId = TEXT("EM_OpenWorldEdModeId");
const FName UOpenWorldEdMode::PathAssetSelect_Tool = FName("OpenWorldEdMode_PathAssetSelect_Tool");
const FName UOpenWorldEdMode::PathAssetCreateNode_Tool = FName("OpenWorldEdMode_PathAssetCreateNode_Tool");
const FName UOpenWorldEdMode::PathAssetConnection_Tool = FName("OpenWorldEdMode_PathAssetConnection_Tool");

UOpenWorldEdMode::UOpenWorldEdMode()
{
	Info = FEditorModeInfo(EM_OpenWorldEdModeId, FText::FromString("OpenWorld Editor"), FSlateIcon(FOpenWorldEditorPluginStyle::Get().GetStyleSetName(), "OpenWorldEdMode", "OpenWorldEdMode.small"), true);
	PathAssetBeingEdited = nullptr;
}

UOpenWorldEdMode::~UOpenWorldEdMode()
{

}

void UOpenWorldEdMode::Enter()
{
	UEdMode::Enter();

	const FOWPathAssetEdModeCommands& UICommands = FOWPathAssetEdModeCommands::Get();

	ClearModePropertyObject();

	ModeProperties = NewObject<UOWPathAssetModeProperties>(this);
	ModeProperties->WatchProperty(ModeProperties->PathAsset, [this](UOWPathAsset* InPathAsset) {
		DeactivateAllTools();
		ReplaceModePropertyObject(PathAssetBeingEdited, InPathAsset);
		PathAssetBeingEdited = InPathAsset;
		OnPathAssetSelected.Broadcast(InPathAsset);
	});
	AddModePropertyObject(ModeProperties);

	RegisterTool(UICommands.PathAssetSelectTool, PathAssetSelect_Tool.ToString(), NewObject<UOWPathAssetSelectToolBuilder>(this));
	RegisterTool(UICommands.PathAssetCreateNodeTool, PathAssetCreateNode_Tool.ToString(), NewObject<UOWPathAssetCreateNodeToolBuilder>(this));
	RegisterTool(UICommands.PathAssetConnectionTool, PathAssetConnection_Tool.ToString(), NewObject<UOWPathAssetConnectionToolBuilder>(this));

	GetToolManager()->SelectActiveToolType(EToolSide::Left, PathAssetSelect_Tool.ToString());
}

void UOpenWorldEdMode::Exit()
{
	UEdMode::Exit();

	ClearModePropertyObject();
}

void UOpenWorldEdMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FOWPathAssetEdModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UOpenWorldEdMode::GetModeCommands() const
{
	return FOWPathAssetEdModeCommands::Get().GetCommands();
}

void UOpenWorldEdMode::AddModePropertyObject(UObject* InObject)
{
	if (ModePropertyObjects.Contains(InObject)) {
		return;
	}

	ModePropertyObjects.Add(InObject);
	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOpenWorldEdMode::ReplaceModePropertyObject(UObject* OldObject, UObject* WithObject)
{
	if (OldObject == WithObject) {
		return;
	}

	if (ModePropertyObjects.Contains(WithObject)) {
		return;
	}

	if (!OldObject || !ModePropertyObjects.Contains(OldObject)) {
		if (WithObject) {
			ModePropertyObjects.Add(WithObject);
			OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
		}
		return;
	}

	if (ModePropertyObjects.Contains(OldObject)) {
		int32 Index = ModePropertyObjects.Find(OldObject);
		if (WithObject) {
			ModePropertyObjects[Index] = WithObject;
		}
		else {
			ModePropertyObjects.Remove(OldObject);
		}
	}

	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOpenWorldEdMode::ClearModePropertyObject()
{
	ModePropertyObjects.Empty();
	//if (ObjectToAdd) {
	//	ModePropertyObjects.Add(ObjectToAdd);
	//}
	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOpenWorldEdMode::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	FOWPathAssetSelectToolActionCommands::Get().BindCommandsForCurrentTool(Toolkit->GetToolkitCommands(), Tool);

	UOWPathAssetBaseTool* BaseTool = Cast<UOWPathAssetBaseTool>(Tool);
	OnPathAssetSelected.AddUObject(BaseTool, &UOWPathAssetBaseTool::SetAsset);
	BaseTool->SetAsset(PathAssetBeingEdited);
}

void UOpenWorldEdMode::OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	if (ExactCast<UOWPathAssetSelectTool>(Tool)) {
		FOWPathAssetSelectToolActionCommands::Get().UnbindActiveCommands(Toolkit->GetToolkitCommands());
	}

	OnPathAssetSelected.RemoveAll(Tool);
}

void UOpenWorldEdMode::ModeTick(float DeltaTime)
{
	ModeProperties->CheckAndUpdateWatched();
}

void UOpenWorldEdMode::DeactivateAllTools() const
{
	GetToolManager()->DeactivateTool(EToolSide::Left, EToolShutdownType::Completed);
}

bool UOpenWorldEdMode::ShouldToolStartBeAllowed(const FString& ToolIdentifier) const
{
	if (!PathAssetBeingEdited) {
		return false;
	}

	return Super::ShouldToolStartBeAllowed(ToolIdentifier);
}


