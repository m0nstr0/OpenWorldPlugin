// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EditorStyleSet.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Styling/StyleColors.h"
#include "Styling/AppStyle.h"
#include "Styling/StarshipCoreStyle.h"

class FOpenWorldEditorPluginStyle : public FSlateStyleSet
{
public:
	FOpenWorldEditorPluginStyle() : FSlateStyleSet("OpenWorldPlugin")
	{
		SetContentRoot(IPluginManager::Get().FindPlugin("OpenWorldPlugin")->GetBaseDir());

		FSlateImageBrush* OWPathAssetThumnailBrush64 = new FSlateImageBrush(RootToContentDir(TEXT("Resources/PathAsset128"), TEXT(".png")), FVector2D(64.f, 64.f));
		FSlateImageBrush* OWPathAssetThumnailBrush16 = new FSlateImageBrush(RootToContentDir(TEXT("Resources/PathAsset128"), TEXT(".png")), FVector2D(16.f, 16.f));
		FSlateImageBrush* OWPathAssetEdModeThumnailBrush40 = new FSlateImageBrush(RootToContentDir(TEXT("Resources/PathAsset128"), TEXT(".png")), FVector2D(40.f, 40.f));
		FSlateImageBrush* OWPathAssetEdModeThumnailBrush20 = new FSlateImageBrush(RootToContentDir(TEXT("Resources/PathAsset128"), TEXT(".png")), FVector2D(20.f, 20.f));

		if (OWPathAssetThumnailBrush64 && OWPathAssetThumnailBrush16) {
			Set("ClassThumbnail.OWPathAsset", OWPathAssetThumnailBrush64);
			Set("ClassIcon.OWPathAsset", OWPathAssetThumnailBrush16);
		}

		if (OWPathAssetEdModeThumnailBrush20 && OWPathAssetEdModeThumnailBrush40) {
			Set("OWPathAssetEdMode", OWPathAssetEdModeThumnailBrush40);
			Set("OWPathAssetEdMode.small", OWPathAssetEdModeThumnailBrush20);
		}
		
		{
			Set("OpenWorldEdMode.PathAssetSelectTool", new FSlateImageBrush(RootToContentDir(TEXT("Resources/Select_40x"), TEXT(".png")), FVector2D(20.f, 20.f)));
			Set("OpenWorldEdMode.PathAssetCreateNodeTool", new FSlateImageBrush(RootToContentDir(TEXT("Resources/CreateNode_40x"), TEXT(".png")), FVector2D(20.f, 20.f)));
			Set("OpenWorldEdMode.PathAssetConnectionTool", new FSlateImageBrush(RootToContentDir(TEXT("Resources/CreateConnection_40x"), TEXT(".png")), FVector2D(20.f, 20.f)));
		}

		FSlateStyleRegistry::RegisterSlateStyle(*this);
	}

	~FOpenWorldEditorPluginStyle()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*this);
	}

public:

	static FOpenWorldEditorPluginStyle& Get()
	{
		if (!Singleton.IsSet()) {
			Singleton.Emplace();
		}
		return Singleton.GetValue();
	}

	static void Destroy()
	{
		Singleton.Reset();
	}

private:
	static TOptional<FOpenWorldEditorPluginStyle> Singleton;
};

TOptional<FOpenWorldEditorPluginStyle> FOpenWorldEditorPluginStyle::Singleton;