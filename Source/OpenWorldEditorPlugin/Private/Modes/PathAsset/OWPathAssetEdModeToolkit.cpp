// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetEdModeToolkit.h"
#include "OWOpenWorldEdMode.h"
#include "EditorModeManager.h"
#include "EdModeInteractiveToolsContext.h"
#include "OpenWorldEditorPluginStyle.h"
#include "OWPathAssetEdModeCommands.h"
#include "Tools/OWPathAssetConnectionTool.h"

FOWPathAssetEdModeToolkit::FOWPathAssetEdModeToolkit()
{}

void FOWPathAssetEdModeToolkit::CreateViewportOverlayWidget()
{
    SAssignNew(ViewportOverlayWidget, SHorizontalBox)
    + SHorizontalBox::Slot()
    .HAlign(HAlign_Center)
    .VAlign(VAlign_Bottom)
    .Padding(FMargin(0.0f, 0.0f, 0.f, 15.f))
    [
        SNew(SBorder)
        .BorderImage(FAppStyle::Get().GetBrush("EditorViewport.OverlayBrush"))
        .Padding(8.f)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(FMargin(0.0, 0.f, 2.f, 0.f))
            [
                SNew(SButton) //SPrimaryButton
                .HAlign(HAlign_Center)
                .ToolTipText(FText::FromString("Link the nodes [Enter]"))
                .ButtonStyle(&FAppStyle::Get().GetWidgetStyle< FButtonStyle >("Button"))
                .OnClicked_Lambda([this]() { ConnectionToolProcessSelectedNodes(true); return FReply::Handled(); })
                .IsEnabled_Lambda([this]() { return IsTwoNodesSelectedToLink; })
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    [
                        SNew(SImage)
                        .ColorAndOpacity(FSlateColor::UseForeground())
                        .Image(FOpenWorldEditorPluginStyle::Get().GetBrush("OpenWorldEdMode.PathAssetConnectionTool.link"))
                    ]
                    + SHorizontalBox::Slot()
                    .VAlign(VAlign_Center)
                    .Padding(8.f, 0.5f, 0.f, 0.f)
                    .AutoWidth()
                    [
                        SNew(STextBlock)
                        .TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>(TEXT("ButtonText")))
                        .Justification(ETextJustify::Center)
                        .Text(FText::FromString("Link"))
                    ]
                ]
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(FMargin(2.0, 0.f, 0.f, 0.f))
            [
                SNew(SButton)
                .ToolTipText(FText::FromString("Unlink the nodes [Esc]"))
                .HAlign(HAlign_Center)
                .ButtonStyle(&FAppStyle::Get().GetWidgetStyle< FButtonStyle >("PrimaryButton"))
                .OnClicked_Lambda([this]() { ConnectionToolProcessSelectedNodes(false); return FReply::Handled(); })
                .IsEnabled_Lambda([this]() { return IsTwoNodesSelectedToLink; })
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    [
                        SNew(SImage)
                        .ColorAndOpacity(FSlateColor::UseForeground())
                        .Image(FOpenWorldEditorPluginStyle::Get().GetBrush("OpenWorldEdMode.PathAssetConnectionTool.unlink"))
                    ]
                    + SHorizontalBox::Slot()
                    .VAlign(VAlign_Center)
                    .Padding(8.f, 0.5f, 0.f, 0.f)  // Compensate down for the baseline since we're using all caps
                    .AutoWidth()
                    [
                        SNew(STextBlock)
                        .TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>(TEXT("ButtonText")))
                        .Justification(ETextJustify::Center)
                        .Text(FText::FromString("Unlink"))
                    ]
                ]
            ]
        ]
    ];
}

void FOWPathAssetEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
	Cast<UOpenWorldEdMode>(InOwningMode.Get())->OnEdModeObjectsChanged.AddSP(this, &FOWPathAssetEdModeToolkit::UpdateModeProperties);
	//	Cast<UOpenWorldEdMode>(OwningEditorMode.Get())->OnEdModeObjectsChanged.RemoveAll(this);

    CreateViewportOverlayWidget();
}

FName FOWPathAssetEdModeToolkit::GetToolkitFName() const
{
	return FName("OpenWorldEdMode");
}

FText FOWPathAssetEdModeToolkit::GetBaseToolkitName() const
{
	return FText::FromString("OpenWorldEdMode Toolkit");
}

void FOWPathAssetEdModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(UOpenWorldEdMode::PathAssetSelect_Tool);
	
	//PaletteNames.Add(UOpenWorldEdMode::MapZoneAsset_Tool);
}

FText FOWPathAssetEdModeToolkit::GetToolPaletteDisplayName(FName Palette) const
{
	if (Palette == UOpenWorldEdMode::PathAssetSelect_Tool) {
		return FText::FromString("Path Asset");
	}

	if (Palette == UOpenWorldEdMode::PathAssetCreateNode_Tool) {
		return FText::FromString("Map Zone");
	}

	return FText();
}

void FOWPathAssetEdModeToolkit::CustomizeModeDetailsViewArgs(FDetailsViewArgs& ArgsInOut)
{
	return;
}

void FOWPathAssetEdModeToolkit::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	UpdateActiveToolProperties(Tool);

	Tool->OnPropertySetsModified.AddSP(this, &FOWPathAssetEdModeToolkit::UpdateActiveToolProperties, Tool);

    if (UOWPathAssetConnectionTool* PathAssetConnectionTool = ExactCast<UOWPathAssetConnectionTool>(Tool)) {
		GetToolkitHost()->AddViewportOverlayWidget(ViewportOverlayWidget.ToSharedRef());
        PathAssetConnectionTool->OnPathAssetNodeSelected.AddSP(this, &FOWPathAssetEdModeToolkit::OnConnectionToolNodesSelected);
        IsTwoNodesSelectedToLink = false;
	}

	FModeToolkit::OnToolStarted(Manager, Tool);
}

void FOWPathAssetEdModeToolkit::OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	Tool->OnPropertySetsModified.RemoveAll(this);

	if (UOWPathAssetConnectionTool* PathAssetConnectionTool = ExactCast<UOWPathAssetConnectionTool>(Tool)) {
        PathAssetConnectionTool->OnPathAssetNodeSelected.RemoveAll(this);
        if (IsHosted()) {
            GetToolkitHost()->RemoveViewportOverlayWidget(ViewportOverlayWidget.ToSharedRef());
        }
	}

	FModeToolkit::OnToolEnded(Manager, Tool);
}

void FOWPathAssetEdModeToolkit::UpdateModeProperties(const TArray<UObject*>& Objects)
{
	ModeDetailsView->SetObjects(Objects);
}

void FOWPathAssetEdModeToolkit::UpdateActiveToolProperties(UInteractiveTool* Tool)
{
	if (Tool) {
		DetailsView->SetObjects(Tool->GetToolProperties(false));
	}
}

void FOWPathAssetEdModeToolkit::OnToolPaletteChanged(FName PaletteName)
{
	//ToolkitCommands
	//GetToolkitCommands()->CanExecuteAction();
	//if (PaletteName == UOpenWorldEdMode::PathAsset_Tool) {
	//	GetToolkitCommands()->TryExecuteAction(FOWPathAssetEdModeCommands::Get().PathAssetTool.ToSharedRef());
	//}

	//GetCommands
	//FOWPathAssetEdModeCommands::Get()
}

void FOWPathAssetEdModeToolkit::OnConnectionToolNodesSelected(const TObjectPtr<UOWPathAssetNode> LeftNode, const TObjectPtr<UOWPathAssetNode> RightNode)
{
    IsTwoNodesSelectedToLink = LeftNode && RightNode;
}

void FOWPathAssetEdModeToolkit::ConnectionToolProcessSelectedNodes(const bool IsNeedToLink) const
{
    if (UOWPathAssetConnectionTool* PathAssetConnectionTool = ExactCast<UOWPathAssetConnectionTool>(GetScriptableEditorMode()->GetToolManager(EToolsContextScope::EdMode)->GetActiveTool(EToolSide::Left))) {
        if (IsNeedToLink)
        {
            PathAssetConnectionTool->LinkSelectedNodes();
        } else
        {
            PathAssetConnectionTool->UnlinkSelectedNodes();
        }
    }
}
