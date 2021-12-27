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
#include "Styling/SlateStyleMacros.h"
#include "Styling/StarshipCoreStyle.h"

class FOpenWorldEditorPluginStyle : public FSlateStyleSet
{
public:
	FOpenWorldEditorPluginStyle();
    virtual ~FOpenWorldEditorPluginStyle() override;

private:
	const FVector2D Image64x64;
	const FVector2D Image40x40;
	const FVector2D Image20x20;
	const FVector2D Image16x16;

public:

	static FOpenWorldEditorPluginStyle& Get();

    static void Destroy();

private:
	static TOptional<FOpenWorldEditorPluginStyle> Singleton;
};