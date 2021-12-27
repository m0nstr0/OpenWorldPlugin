// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorldEditorPluginStyle.h"

FOpenWorldEditorPluginStyle::FOpenWorldEditorPluginStyle() :
    FSlateStyleSet("OpenWorldPlugin"),
    Image64x64(64.f, 64.f),
    Image40x40(40.f, 40.f),
    Image20x20(20.f, 20.f),
    Image16x16(16.f, 16.f)
{
    FSlateStyleSet::SetContentRoot(IPluginManager::Get().FindPlugin("OpenWorldPlugin")->GetBaseDir());

    {
        Set("ClassThumbnail.OWPathAsset", new IMAGE_BRUSH(TEXT("Resources/PathAsset128"), Image64x64));
        Set("ClassIcon.OWPathAsset", new IMAGE_BRUSH(TEXT("Resources/PathAsset128"), Image16x16));
    }
		
    {
        Set("PathAsset_EdMode", new IMAGE_BRUSH(TEXT("Resources/PathAsset128"), Image40x40));
        Set("PathAsset_EdMode.Small", new IMAGE_BRUSH(TEXT("Resources/PathAsset128"), Image20x20));
        Set("PathAsset_EdMode.SelectTool", new IMAGE_BRUSH(TEXT("Resources/Select_40x"), Image20x20));
        Set("PathAsset_EdMode.CreateTool", new IMAGE_BRUSH(TEXT("Resources/CreateNode_40x"), Image20x20));
        Set("PathAsset_EdMode.LinkTool", new IMAGE_BRUSH(TEXT("Resources/CreateConnection_40x"), Image20x20));
        Set("PathAsset_EdMode.LinkTool.Link", new IMAGE_BRUSH_SVG(TEXT("Resources/Linked"), Image16x16));
        Set("PathAsset_EdMode.LinkTool.Unlink", new IMAGE_BRUSH_SVG(TEXT("Resources/Unlinked"), Image16x16));

        FComboButtonStyle LinkTypeComboButtonStyle(FAppStyle::Get().GetWidgetStyle<FComboButtonStyle>("SimpleComboButton"));
        LinkTypeComboButtonStyle.SetButtonStyle(FAppStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));

        FComboBoxStyle LinkTypeComboBoxStyle(FAppStyle::Get().GetWidgetStyle<FComboBoxStyle>("SimpleComboBox"));
        LinkTypeComboBoxStyle.SetComboButtonStyle(LinkTypeComboButtonStyle);

        Set("PathAsset_EdMode.LinkTool.LinkTypeCombo", LinkTypeComboBoxStyle);
    }

    FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FOpenWorldEditorPluginStyle::~FOpenWorldEditorPluginStyle()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

FOpenWorldEditorPluginStyle& FOpenWorldEditorPluginStyle::Get()
{
    if (!Singleton.IsSet()) {
        Singleton.Emplace();
    }
    return Singleton.GetValue();
}

void FOpenWorldEditorPluginStyle::Destroy()
{
    Singleton.Reset();
}

TOptional<FOpenWorldEditorPluginStyle> FOpenWorldEditorPluginStyle::Singleton;