// Fill out your copyright notice in the Description page of Project Settings.


#include "Editors/PathAsset/OWPathAssetEditor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Graph/EdGraph_OWPathAsset.h"
#include "Graph/EdGraphSchema_OWPathAsset.h"
//#include "Toolkits/AssetEditorManager.h"

#define LOCTEXT_NAMESPACE "FOWPathAssetEditor"

void FOWPathAssetEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UOWPathAsset* InPathAsset)
{
	//FAssetEditorManager::Get().CloseOtherEditors(InitQuestBook, this);
	//QuestBookBeingEdited = InitQuestBook;

	// Create the appearance info
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString("Some Text");

	// Default layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_OWPathAssetEditor_Layout_v5")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(

				FTabManager::NewStack()
				->SetSizeCoefficient(0.8f)
				->SetHideTabWell(true)
				->AddTab(FName("Viewport"), ETabState::OpenedTab)

			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->SetHideTabWell(true)
				->AddTab(FName("Details"), ETabState::OpenedTab)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, FName(TEXT("OWPathAssetEditorAppName")), StandaloneDefaultLayout, true, true, InPathAsset);
	//RegenerateMenusAndToolbars();
}

//const FName FCustomAssetEditor::ToolkitFName(TEXT("CustomAssetEditor"));
//const FName FCustomAssetEditor::PropertiesTabId(TEXT("CustomAssetEditor_Properties"));

//void FOWPathAssetEditor::InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const FName AppIdentifier, const TSharedRef<FTabManager::FLayout>& StandaloneDefaultLayout, const bool bCreateDefaultStandaloneMenu, const bool bCreateDefaultToolbar, UObject* ObjectToEdit, const bool bInIsToolbarFocusable /*= false*/, const bool bInUseSmallToolbarIcons /*= false*/)
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}

void FOWPathAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(FText::FromString("Custom Editor"));
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	TabManager->RegisterTabSpawner(FName("Viewport"), FOnSpawnTab::CreateSP(this, &FOWPathAssetEditor::SpawnTab_GraphEditor))
		.SetDisplayName(FText::FromString("Viewport"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	TabManager->RegisterTabSpawner(FName("Details"), FOnSpawnTab::CreateSP(this, &FOWPathAssetEditor::SpawnTab_Details))
		.SetDisplayName(FText::FromString("Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FOWPathAssetEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

TSharedRef<SDockTab> FOWPathAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnTab = SNew(SDockTab)
		.Label(LOCTEXT("SpanDockName", "Test"));

	return SpawnTab;
}

TSharedRef<SDockTab> FOWPathAssetEditor::SpawnTab_GraphEditor(const FSpawnTabArgs& Args)
{
	UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(GetEditingObject(), NAME_None, UEdGraph_OWPathAsset::StaticClass(), UEdGraphSchema_OWPathAsset::StaticClass());
	TSharedRef<SDockTab> SpawnTab = SNew(SDockTab)
		.Label(LOCTEXT("SpanDockName", "Test"))
		[
			SNew(SGraphEditor)
			.GraphToEdit(Graph)
		];
	return SpawnTab;
}


FName FOWPathAssetEditor::GetToolkitFName() const
{
	return FName("PathAssetEditor");
}

FText FOWPathAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("PathAssetToolkinName", "Path Asset Editor");
}

FString FOWPathAssetEditor::GetWorldCentricTabPrefix() const
{
	return FString("");
}

void FOWPathAssetEditor::InvokeTab(const struct FTabId& TabId)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

FLinearColor FOWPathAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Blue;
}

FOWPathAssetEditor::~FOWPathAssetEditor()
{

}

#undef LOCTEXT_NAMESPACE