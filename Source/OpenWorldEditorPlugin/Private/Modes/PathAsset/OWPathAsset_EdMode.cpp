// Fill out your copyright notice in the Description page of Project Settings.

#include "OWPathAsset_EdMode.h"
#include "OWPathAsset_EdModeCommands.h"
#include "OpenWorldEditorPluginStyle.h"
#include "Tools/OWPathAsset_BaseTool.h"
#include "InteractiveToolManager.h"
#include "OWPathAsset_EdModeToolkit.h"
#include "Tools/OWPathAsset_LinkTool.h"
#include "Tools/OWPathAsset_SelectTool.h"
#include "Tools/OWPathAsset_CreateTool.h"

const FEditorModeID UOWPathAsset_EdMode::EM_OpenWorldEdModeId = TEXT("EM_OpenWorldEdModeId");
const FName UOWPathAsset_EdMode::PathAssetSelect_Tool = FName("OpenWorldEdMode_PathAssetSelect_Tool");
const FName UOWPathAsset_EdMode::PathAssetCreateNode_Tool = FName("OpenWorldEdMode_PathAssetCreateNode_Tool");
const FName UOWPathAsset_EdMode::PathAssetConnection_Tool = FName("OpenWorldEdMode_PathAssetConnection_Tool");

UOWPathAsset_EdMode::UOWPathAsset_EdMode()
{
	Info = FEditorModeInfo(EM_OpenWorldEdModeId, FText::FromString("OpenWorld PathAsset Editor"), FSlateIcon(FOpenWorldEditorPluginStyle::Get().GetStyleSetName(), "PathAsset_EdMode", "PathAsset_EdMode.Small"), true);
	PathAssetBeingEdited = nullptr;
}

UOWPathAsset_EdMode::~UOWPathAsset_EdMode()
{

}

void UOWPathAsset_EdMode::Enter()
{
	UEdMode::Enter();

	const FOWPathAsset_EdModeCommands& UICommands = FOWPathAsset_EdModeCommands::Get();

	ClearModePropertyObject();

	ModeProperties = NewObject<UOWPathAsset_EdModeProperties>(this);
	ModeProperties->WatchProperty(ModeProperties->PathAsset, [this](UOWPathAsset* InPathAsset) {
		DeactivateAllTools();
		ReplaceModePropertyObject(PathAssetBeingEdited, InPathAsset);
		PathAssetBeingEdited = InPathAsset;
		OnPathAssetSelected.Broadcast(InPathAsset);
	});
	AddModePropertyObject(ModeProperties);

	RegisterTool(UICommands.SelectTool, PathAssetSelect_Tool.ToString(), NewObject<UOWPathAsset_SelectToolBuilder>(this));
	RegisterTool(UICommands.CreateTool, PathAssetCreateNode_Tool.ToString(), NewObject<UOWPathAsset_CreateToolBuilder>(this));
	RegisterTool(UICommands.LinkTool, PathAssetConnection_Tool.ToString(), NewObject<UOWPathAsset_LinkToolBuilder>(this));

	GetToolManager()->SelectActiveToolType(EToolSide::Left, PathAssetSelect_Tool.ToString());
}

void UOWPathAsset_EdMode::Exit()
{
	UEdMode::Exit();

	ClearModePropertyObject();
}

void UOWPathAsset_EdMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FOWPathAsset_EdModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UOWPathAsset_EdMode::GetModeCommands() const
{
	return FOWPathAsset_EdModeCommands::Get().GetCommands();
}

void UOWPathAsset_EdMode::AddModePropertyObject(UObject* InObject)
{
	if (ModePropertyObjects.Contains(InObject)) {
		return;
	}

	ModePropertyObjects.Add(InObject);
	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOWPathAsset_EdMode::ReplaceModePropertyObject(UObject* OldObject, UObject* WithObject)
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

void UOWPathAsset_EdMode::ClearModePropertyObject()
{
	ModePropertyObjects.Empty();
	OnEdModeObjectsChanged.Broadcast(ModePropertyObjects);
}

void UOWPathAsset_EdMode::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	FOWPathAsset_SelectToolActionCommands::Get().BindCommandsForCurrentTool(Toolkit->GetToolkitCommands(), Tool);

	UOWPathAsset_BaseTool* BaseTool = Cast<UOWPathAsset_BaseTool>(Tool);
	OnPathAssetSelected.AddUObject(BaseTool, &UOWPathAsset_BaseTool::SetAsset);
	BaseTool->SetAsset(PathAssetBeingEdited);
}

void UOWPathAsset_EdMode::OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	if (ExactCast<UOWPathAsset_SelectTool>(Tool)) {
		FOWPathAsset_SelectToolActionCommands::Get().UnbindActiveCommands(Toolkit->GetToolkitCommands());
	}

	OnPathAssetSelected.RemoveAll(Tool);
}

void UOWPathAsset_EdMode::ModeTick(float DeltaTime)
{
	ModeProperties->CheckAndUpdateWatched();
}

void UOWPathAsset_EdMode::DeactivateAllTools() const
{
	GetToolManager()->DeactivateTool(EToolSide::Left, EToolShutdownType::Completed);
}

bool UOWPathAsset_EdMode::ShouldToolStartBeAllowed(const FString& ToolIdentifier) const
{
	if (!PathAssetBeingEdited) {
		return false;
	}

	return UEdMode::ShouldToolStartBeAllowed(ToolIdentifier);
}


