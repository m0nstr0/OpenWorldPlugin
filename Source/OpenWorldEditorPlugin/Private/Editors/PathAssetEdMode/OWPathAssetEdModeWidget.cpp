// Fill out your copyright notice in the Description page of Project Settings.


#include "Editors/PathAssetEdMode/OWPathAssetEdModeWidget.h"
#include "OWPathAssetEdMode.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Editor.h"
#include "OWPathAsset.h"
#include "SAssetDropTarget.h"
#include "PropertyCustomizationHelpers.h"
#include "Modules/ModuleManager.h"
#include "IDetailsView.h"
#include "Styling/AppStyle.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Styling/StyleColors.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "OpenWorldEditorPluginStyle.h"
#include <Widgets/Layout/SGridPanel.h>

void SOWPathAssetEdModeWidget::Construct(const FArguments& InArgs)
{
	FSlateFontInfo StandardFont = FEditorStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont"));

	CreatePathAssetDetailsView();

	FMargin StandartPadding(6.f, 3.f, 6.f, 3.f);

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.Padding(StandartPadding)
			[
				SNew(STextBlock)
				//.TextStyle(FAppStyle::Get(), "Font.Large")
				//.ColorAndOpacity(FStyleColors::Foreground)
				.Font(FAppStyle::Get().GetFontStyle("Font.Large"))
				.Text(FText::FromString("Path Asset"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(StandartPadding)
			[
				SNew(SRichTextBlock)
				.DecoratorStyleSet(&FAppStyle::Get())
				.Text(FText::FromString("<MonospacedUnderlinedText>Alt+LMB</> to create the connected node\n<MonospacedUnderlinedText>Ctrl+Alt+LMB</> to create the unconnected node\n<MonospacedUnderlinedText>Shift+LMB</> to connect two nodes\n<MonospacedUnderlinedText>Ctrl+Shift+LMB</> to disconnect two nodes\n<MonospacedUnderlinedText>Delete</> to remove the selected node or the connection\n<MonospacedUnderlinedText>End</> to snap the selected node to the ground"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.Padding(StandartPadding)
			[
				SNew(SObjectPropertyEntryBox)
				.AllowedClass(UOWPathAsset::StaticClass())
				.AllowClear(true)
				.DisplayThumbnail(true)
				.DisplayCompactSize(false)
				.OnObjectChanged(FOnSetObject::CreateSP(this, &SOWPathAssetEdModeWidget::OnPathAssetChanged))
				.ObjectPath_Lambda([&]()->FString { return GetPathAssetPath(); })
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.Padding(StandartPadding)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SBorder)
					.BorderImage(new FSlateRoundedBoxBrush(FStyleColors::Header, 6.f, FStyleColors::Foldout, 1.f))
				]
				+ SOverlay::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(5.f)
					.AutoHeight()
					[
						SNew(SGridPanel)
						+ SGridPanel::Slot(0, 0)
						[
							SNew(SBorder)
							.BorderImage(new FSlateRoundedBoxBrush(FStyleColors::White25, 3.f, FStyleColors::Foldout, 1.f))
							.Padding(3.f)
							[
								SNew(STextBlock)
								.Text(FText::FromString("CTRL"))
								.ColorAndOpacity(FStyleColors::White)
								.Font(FAppStyle::Get().GetFontStyle("SmallFont"))
							]
						]
						+ SGridPanel::Slot(1, 0)
						.Padding(3.f)
						[
							SNew(STextBlock)
							.Text(FText::FromString("+"))
							.Font(FAppStyle::Get().GetFontStyle("SmallFont"))
						]
						+ SGridPanel::Slot(2, 0)
						[
							SNew(SBorder)
							.BorderImage(new FSlateRoundedBoxBrush(FStyleColors::White25, 3.f, FStyleColors::Foldout, 1.f))
							.Padding(3.f)
							[
								SNew(STextBlock)
								.Text(FText::FromString("ALT"))
								.ColorAndOpacity(FStyleColors::White)
								.Font(FAppStyle::Get().GetFontStyle("SmallFont"))
							]
						]
						+ SGridPanel::Slot(3, 0)
						.Padding(3.f)
						[
							SNew(STextBlock)
							.Text(FText::FromString("+"))
							.Font(FAppStyle::Get().GetFontStyle("SmallFont"))
						]
						+ SGridPanel::Slot(4, 0)
						[
							SNew(SBorder)
							.BorderImage(new FSlateRoundedBoxBrush(FStyleColors::White25, 3.f, FStyleColors::Foldout, 1.f))
							.Padding(3.f)
							[
								SNew(STextBlock)
								.Text(FText::FromString("LMB"))
								.ColorAndOpacity(FStyleColors::White)
								.Font(FAppStyle::Get().GetFontStyle("SmallFont"))
							]
						]
						+ SGridPanel::Slot(5, 0)
						.Padding(3.f)
						[
							SNew(STextBlock)
							.Text(FText::FromString(" - Add disconnected node"))
							.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
						]
					]
				]
			]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
				.Padding(StandartPadding)
				[
					PathAssetDetailsView->AsShared()
				]
		];
}

void SOWPathAssetEdModeWidget::OnPathAssetChanged(const FAssetData& AssetData)
{
	PathAssetDetailsView->SetObject(AssetData.GetAsset());
	PathAssetChangedDelegate.ExecuteIfBound(AssetData.IsValid() ? Cast<UOWPathAsset>(AssetData.GetAsset()) : nullptr);
}

FString SOWPathAssetEdModeWidget::GetPathAssetPath()
{
	if (GetEdMode()->GetSelectedPathAsset().IsValid()) {
		FAssetData AssetData(GetEdMode()->GetSelectedPathAsset().Get(), true);
		return AssetData.ObjectPath.ToString();
	}
	return TEXT("");
}


void SOWPathAssetEdModeWidget::CreatePathAssetDetailsView()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea, true, nullptr, false, NAME_None);
	//DetailsViewArgs.NotifyHook = Editor;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowMultipleTopLevelObjects = false;


	PathAssetDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	//DetailsView->SetObject(Editor->GetStateMachine());


	/*FDetailsViewArgs DetailsViewArgs(
		/*bUpdateFromSelection=*/ //false,
		/*bLockable=*/ //false,
		/*bAllowSearch=*/ //false,
		//FDetailsViewArgs::HideNameArea,
		/*bHideSelectionTip=*/ //true,
		/*InNotifyHook=*/ //nullptr,
		/*InSearchInitialKeyFocus=*/ //false,
		/*InViewIdentifier=*/ //NAME_None);
	//DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Automatic;
	//DetailsViewArgs.bShowOptions = false;
	//DetailsViewArgs.bAllowMultipleTopLevelObjects = true;

	//CommonDetailsView = EditModule.CreateDetailView(DetailsViewArgs);
	//CommonDetailsView->SetObject(MeshFractureSettings->CommonSettings, true);
	//CommonDetailsView->OnFinishedChangingProperties().AddRaw(this, &SFractureSettingsWidget::OnDetailsPanelFinishedChangingProperties);*/
}

FOWPathAssetEdMode* SOWPathAssetEdModeWidget::GetEdMode() const
{
	return (FOWPathAssetEdMode*)GLevelEditorModeTools().GetActiveMode(FOWPathAssetEdMode::EM_OWPathAssetEdModeId);
}
