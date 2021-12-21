// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetEdModeToolkit.h"
#include "OWOpenWorldEdMode.h"
#include "EditorModeManager.h"
#include "EdModeInteractiveToolsContext.h"
#include "OpenWorldEditorPluginStyle.h"
#include "Tools/OWPathAssetConnectionTool.h"

void FOWPathAssetEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
    FModeToolkit::Init(InitToolkitHost, InOwningMode);
    Cast<UOpenWorldEdMode>(InOwningMode.Get())->OnEdModeObjectsChanged.AddSP(this, &FOWPathAssetEdModeToolkit::UpdateModeProperties);

    LinkDirectionTypeItems.Add(MakeShareable(new FLinkDirectionType{ FString(TEXT("G ↔ B")),  EOWPathAssetDirectionType::LAR }));
    LinkDirectionTypeItems.Add(MakeShareable(new FLinkDirectionType{ FString(TEXT("G ← B")),  EOWPathAssetDirectionType::R2L }));
    LinkDirectionTypeItems.Add(MakeShareable(new FLinkDirectionType{ FString(TEXT("G → B")),  EOWPathAssetDirectionType::L2R }));

    CreateViewportOverlayWidgetForConnectionTool();
    LinkDirectionTypeComboBox->SetSelectedItem(LinkDirectionTypeItems[0]);
}

void FOWPathAssetEdModeToolkit::CreateViewportOverlayWidgetForConnectionTool()
{
    SAssignNew(ViewportOverlayWidgetForConnectionTool, SHorizontalBox)
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
            .Padding(FMargin(12.0, 1.5f, 14.f, 1.5f))
            [
                SNew(STextBlock)
                .Visibility_Lambda([this]() { return !IsTwoNodesSelectedToLink ? EVisibility::Visible : EVisibility::Collapsed; })
                .TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>(TEXT("ButtonText")))
                .Justification(ETextJustify::Center)
                .Text(FText::FromString("Select two nodes"))
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(FMargin(0.0, 0.f, 2.f, 0.f))
            [
                SAssignNew(LinkDirectionTypeComboBox, SComboBox< TSharedPtr<FLinkDirectionType> >)
                .Visibility_Lambda([this]() { return IsTwoNodesSelectedToLink ? EVisibility::Visible : EVisibility::Collapsed; })
                .ComboBoxStyle(FOpenWorldEditorPluginStyle::Get(), "OpenWorldEdMode.PathAssetConnectionTool.LinkTypeCombo")
                .OptionsSource(&LinkDirectionTypeItems)
                .HasDownArrow(false)
                .ToolTipText(FText::FromString("Direction type"))
                .IsEnabled_Lambda([this]() { return IsTwoNodesSelectedToLink; })
                .OnGenerateWidget_Lambda([](TSharedPtr<FLinkDirectionType> Item) {
                    return  SNew(STextBlock)
                        .TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>(TEXT("ButtonText")))
                        .Justification(ETextJustify::Center)
                        .Text(FText::FromString(Item->Text));
                })
                .OnSelectionChanged_Lambda([this](TSharedPtr<FLinkDirectionType> Item, ESelectInfo::Type InSelectInfo) {
                    if (Item.IsValid()) {
                        if (LinkDirectionTypeTextWidget.IsValid()) {
                            LinkDirectionTypeTextWidget->SetText(FText::FromString(Item->Text));
                        }
                    }
                })
                [
                    SAssignNew(LinkDirectionTypeTextWidget, STextBlock)
                    .TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>(TEXT("ButtonText")))
                    .Justification(ETextJustify::Center)
                ]
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(FMargin(2.0, 0.f, 0.f, 0.f))
            [
                SNew(SButton) //SPrimaryButton
                .Visibility_Lambda([this]() { return IsTwoNodesSelectedToLink ? EVisibility::Visible : EVisibility::Collapsed; })
                .HAlign(HAlign_Center)
                .ToolTipText(FText::FromString("Link the nodes or change direction [Enter]"))
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
                        .Text_Lambda([this]() { return IsTwoNodesSelectedToLink && IsTwoNodesHaveLink ? FText::FromString("Change direction") : FText::FromString("Link"); })
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
                .IsEnabled_Lambda([this]() { return IsTwoNodesSelectedToLink && IsTwoNodesHaveLink; })
                .Visibility_Lambda([this](){ return IsTwoNodesSelectedToLink && IsTwoNodesHaveLink ? EVisibility::Visible : EVisibility::Collapsed; })
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


void FOWPathAssetEdModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(UOpenWorldEdMode::PathAssetSelect_Tool);
}

void FOWPathAssetEdModeToolkit::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	UpdateActiveToolProperties(Tool);

	Tool->OnPropertySetsModified.AddSP(this, &FOWPathAssetEdModeToolkit::UpdateActiveToolProperties, Tool);

    if (UOWPathAssetConnectionTool* PathAssetConnectionTool = ExactCast<UOWPathAssetConnectionTool>(Tool)) {
		GetToolkitHost()->AddViewportOverlayWidget(ViewportOverlayWidgetForConnectionTool.ToSharedRef());
        PathAssetConnectionTool->OnNodeChanged.AddSP(this, &FOWPathAssetEdModeToolkit::OnConnectionToolNodesSelected);
        IsTwoNodesSelectedToLink = false;
	}

	FModeToolkit::OnToolStarted(Manager, Tool);
}

void FOWPathAssetEdModeToolkit::OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	Tool->OnPropertySetsModified.RemoveAll(this);

	if (UOWPathAssetConnectionTool* PathAssetConnectionTool = ExactCast<UOWPathAssetConnectionTool>(Tool)) {
        PathAssetConnectionTool->OnNodeChanged.RemoveAll(this);
        if (IsHosted()) {
            GetToolkitHost()->RemoveViewportOverlayWidget(ViewportOverlayWidgetForConnectionTool.ToSharedRef());
        }
	}

	FModeToolkit::OnToolEnded(Manager, Tool);
}

void FOWPathAssetEdModeToolkit::UpdateModeProperties(const TArray<UObject*>& Objects) const
{
	ModeDetailsView->SetObjects(Objects);
}

void FOWPathAssetEdModeToolkit::UpdateActiveToolProperties(UInteractiveTool* Tool) const
{
	if (Tool) {
		DetailsView->SetObjects(Tool->GetToolProperties(false));
	}
}

void FOWPathAssetEdModeToolkit::OnConnectionToolNodesSelected(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, bool HaveLink, EOWPathAssetDirectionType Direction)
{
    IsTwoNodesSelectedToLink = LeftNode.IsValid() && RightNode.IsValid();
    IsTwoNodesHaveLink = HaveLink;

    if (IsTwoNodesHaveLink) {
        LinkDirectionTypeComboBox->SetSelectedItem(*(LinkDirectionTypeItems.FindByPredicate([Direction](const TSharedPtr<FLinkDirectionType>& Type) { return Type->Direction == Direction; })));
    }
}

void FOWPathAssetEdModeToolkit::ConnectionToolProcessSelectedNodes(const bool IsNeedToLink) const
{
    const TSharedPtr<FLinkDirectionType> Link = LinkDirectionTypeComboBox->GetSelectedItem();

    if (UOWPathAssetConnectionTool* PathAssetConnectionTool = ExactCast<UOWPathAssetConnectionTool>(GetScriptableEditorMode()->GetToolManager(EToolsContextScope::EdMode)->GetActiveTool(EToolSide::Left))) {
        if (IsNeedToLink)
        {
            PathAssetConnectionTool->Link(Link->Direction);
        } else
        {
            PathAssetConnectionTool->Unlink();
        }
    }
}
